#include "Logger.h"
#include <iostream>
#include <algorithm>

// 实例化单例对象
Logger& Logger::Instance() 
{
    static Logger inst;
    return inst;
}

// 初始化日志系统
void Logger::Init(const std::string& logDir,
    LogLevel level,
    size_t maxFileSize,
    size_t maxFiles,
    bool async,
    bool isTerminal,
    bool sourceFileTrace)
{
    std::lock_guard<std::mutex> lock(fileMutex_);

    // 配置参数
    logDir_ = logDir;
    level_.store(level, std::memory_order_relaxed);
    maxFileSize_ = maxFileSize;
    maxFiles_ = maxFiles;
    async_.store(async, std::memory_order_relaxed);
    isTerminal_.store(isTerminal, std::memory_order_relaxed);
	sourceFileTrace_.store(sourceFileTrace, std::memory_order_relaxed);

    // 确保目录存在
    namespace fs = std::filesystem;
    fs::create_directories(logDir_);

	RotateFile();

    // 异步模式下启动后台线程
    if (async_.load()) 
    {
        running_.store(true);
        workerThread_ = std::thread(&Logger::Worker, this);
    }
}

// 日志记录函数
void Logger::Log(LogLevel level, const std::string& message) 
{
    // 级别过滤
    if (level < level_.load(std::memory_order_relaxed)) 
    {
        return;
    }

    std::ostringstream ss;

	if (sourceFileTrace_.load(std::memory_order_relaxed))
	{
        // 拼接日志行：时间 + 线程 + 级别 + 源文件:行号 + 消息
		std::source_location loc = std::source_location::current();
        ss << GetTimeStamp()
            << " [" << GetThreadId() << "] "
            << LevelToString(level) << " "
            << loc.file_name() << ":" << loc.line() << " "
            << message << "\n";
	}
    else {
		// 拼接日志行：时间 + 线程 + 级别 + 消息
		ss << GetTimeStamp()
			<< " [" << GetThreadId() << "] "
			<< LevelToString(level) << " "
			<< message << "\n";
    }
    

    LogMessage logMsg{ ss.str(), level, std::chrono::system_clock::now() };

    if (async_.load()) 
    {
        // 异步：入队并通知
        {
            std::lock_guard<std::mutex> lock(queueMutex_);
            logQueue_.push(std::move(logMsg));
        }
        condition_.notify_one();
    }
    else 
    {
        // 同步：直接写
        WriteToFile(logMsg.message);
    }
}

// 析构函数，停止后台线程并刷新剩余日志
Logger::~Logger() 
{
    if (async_.load()) 
    {
        // 停止后台线程
        running_.store(false);
        condition_.notify_one();
        if (workerThread_.joinable()) {
            workerThread_.join();
        }
    }
}

// 修改日志存放目录，需在init之前生效
void Logger::SetLogDir(const std::string& logDir) 
{
    std::lock_guard<std::mutex> lock(fileMutex_);
    logDir_ = logDir;
}

// 修改日志级别
void Logger::SetLogLevel(LogLevel level) 
{
    level_.store(level, std::memory_order_relaxed);
}

// 修改单个日志文件最大尺寸
void Logger::SetMaxFileSize(size_t size) 
{
    std::lock_guard<std::mutex> lock(fileMutex_);
    maxFileSize_ = size;
}

// 修改最大日志文件数量
void Logger::SetMaxFiles(size_t maxFiles) 
{
    std::lock_guard<std::mutex> lock(fileMutex_);
    maxFiles_ = maxFiles;
}

// 修改是否启用异步写入
void Logger::SetAsync(bool async) 
{
    async_.store(async, std::memory_order_relaxed);
}

// 修改是否同时输出到终端
void Logger::setTerminal(bool isTerminal) 
{
    isTerminal_.store(isTerminal, std::memory_order_relaxed);
}

// 修改是否输出源文件位置
void Logger::setSourceFileTrace(bool sourceFileTrace)
{
	sourceFileTrace_.store(sourceFileTrace, std::memory_order_relaxed);
}

// 异步写入日志线程
void Logger::Worker()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(queueMutex_);

        // 使用 wait 直到队列不为空或者线程停止运行
        condition_.wait(lock, [this] {
            return !logQueue_.empty() || !running_.load();
            });

        // 如果线程已经停止且队列为空，则退出线程
        if (!running_.load() && logQueue_.empty())
        {
            break;
        }

        // 交换队列，减少锁持有时间
        std::queue<LogMessage> temp;
        std::swap(temp, logQueue_);
        lock.unlock();

        // 批量写入
        while (!temp.empty())
        {
            WriteToFile(temp.front().message);
            temp.pop();
        }
    }
}

// 文件轮转
void Logger::RotateFile() 
{
    std::lock_guard<std::mutex> lock(rotateMutex_);
    namespace fs = std::filesystem;

    // 收集所有 .log 文件
    std::vector<std::string> files;
    for (auto& entry : fs::directory_iterator(logDir_)) 
    {
        if (entry.path().extension() == ".log") 
        {
            files.push_back(entry.path().string());
        }
    }

    // 超出数量时删除多余日志
    if (files.size() >= maxFiles_) 
    {
        std::sort(files.begin(), files.end());
		size_t filesToDelete = files.size() - maxFiles_ + 1;
		for (size_t i = 0; i < filesToDelete; ++i)
		{
			fs::remove(files[i]);
		}
    }

    // 新文件名
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm_now, &t);
#else
    localtime_r(&t, &tm_now);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d_%H-%M-%S") << ".log";
    currentFilename_ = logDir_ + "/" + oss.str();
    currentFileSize_ = 0;
}

// 写入文件和终端
void Logger::WriteToFile(const std::string& message) 
{
    std::lock_guard<std::mutex> lock(fileMutex_);

    // 移除ANSI转义字符，写入文件
    std::string messageToWrite = RemoveAnsiCodes(message);

    // 超出大小则轮转
    if (currentFileSize_ + messageToWrite.size() > maxFileSize_) 
    {
        RotateFile();
    }

    // 打开文件（若尚未打开）
    if (!logFile_.is_open()) 
    {
        logFile_.open(currentFilename_, std::ios::app);
    }

    // 写入并刷新
    logFile_ << messageToWrite;
    logFile_.flush();
    currentFileSize_ += messageToWrite.size();

    // 如果需要终端输出，同步打印
    if (isTerminal_.load()) 
    {
        std::cout << message;
    }
}

// 日志级别转换为字符串
std::string Logger::LevelToString(LogLevel level) 
{
    switch (level) 
    {
    case LogLevel::TRACE:
        return isTerminal_ ? "\033[36mTRACE\033[0m" : "TRACE"; // Cyan
    case LogLevel::DEBUG:
        return isTerminal_ ? "\033[32mDEBUG\033[0m" : "DEBUG"; // Green
    case LogLevel::INFO:
        return isTerminal_ ? "\033[34mINFO \033[0m" : "INFO";  // Blue
    case LogLevel::WARN:
        return isTerminal_ ? "\033[33mWARN \033[0m" : "WARN";  // Yellow
    case LogLevel::ERROR:
        return isTerminal_ ? "\033[31mERROR\033[0m" : "ERROR"; // Red
    case LogLevel::FATAL:
        return isTerminal_ ? "\033[35mFATAL\033[0m" : "FATAL"; // Magenta
    default:
        return "UNKNOWN";
    }
}

// 获取当前时间戳
std::string Logger::GetTimeStamp() 
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm_now, &t);
#else
    localtime_r(&t, &tm_now);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// 获取当前线程ID
std::string Logger::GetThreadId() 
{
    std::ostringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

// 去除ANSI转义字符
std::string Logger::RemoveAnsiCodes(const std::string& str) 
{
    // 使用正则表达式匹配所有以ESC字符(\x1B)开头、
    // 后跟 '['，接任意数字或分号，最终以 'm' 结束的序列，
    // 例如 "\033[35m" 和 "\033[0m" 都能被匹配到。
    std::regex ansi_escape_pattern("\x1B\\[[0-9;]*m");
    return std::regex_replace(str, ansi_escape_pattern, "");
}
