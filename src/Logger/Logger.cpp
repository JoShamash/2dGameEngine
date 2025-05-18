#include "Logger.h"
#include <iostream>
#include <algorithm>

// ʵ������������
Logger& Logger::Instance() 
{
    static Logger inst;
    return inst;
}

// ��ʼ����־ϵͳ
void Logger::Init(const std::string& logDir,
    LogLevel level,
    size_t maxFileSize,
    size_t maxFiles,
    bool async,
    bool isTerminal,
    bool sourceFileTrace)
{
    std::lock_guard<std::mutex> lock(fileMutex_);

    // ���ò���
    logDir_ = logDir;
    level_.store(level, std::memory_order_relaxed);
    maxFileSize_ = maxFileSize;
    maxFiles_ = maxFiles;
    async_.store(async, std::memory_order_relaxed);
    isTerminal_.store(isTerminal, std::memory_order_relaxed);
	sourceFileTrace_.store(sourceFileTrace, std::memory_order_relaxed);

    // ȷ��Ŀ¼����
    namespace fs = std::filesystem;
    fs::create_directories(logDir_);

	RotateFile();

    // �첽ģʽ��������̨�߳�
    if (async_.load()) 
    {
        running_.store(true);
        workerThread_ = std::thread(&Logger::Worker, this);
    }
}

// ��־��¼����
void Logger::Log(LogLevel level, const std::string& message) 
{
    // �������
    if (level < level_.load(std::memory_order_relaxed)) 
    {
        return;
    }

    std::ostringstream ss;

	if (sourceFileTrace_.load(std::memory_order_relaxed))
	{
        // ƴ����־�У�ʱ�� + �߳� + ���� + Դ�ļ�:�к� + ��Ϣ
		std::source_location loc = std::source_location::current();
        ss << GetTimeStamp()
            << " [" << GetThreadId() << "] "
            << LevelToString(level) << " "
            << loc.file_name() << ":" << loc.line() << " "
            << message << "\n";
	}
    else {
		// ƴ����־�У�ʱ�� + �߳� + ���� + ��Ϣ
		ss << GetTimeStamp()
			<< " [" << GetThreadId() << "] "
			<< LevelToString(level) << " "
			<< message << "\n";
    }
    

    LogMessage logMsg{ ss.str(), level, std::chrono::system_clock::now() };

    if (async_.load()) 
    {
        // �첽����Ӳ�֪ͨ
        {
            std::lock_guard<std::mutex> lock(queueMutex_);
            logQueue_.push(std::move(logMsg));
        }
        condition_.notify_one();
    }
    else 
    {
        // ͬ����ֱ��д
        WriteToFile(logMsg.message);
    }
}

// ����������ֹͣ��̨�̲߳�ˢ��ʣ����־
Logger::~Logger() 
{
    if (async_.load()) 
    {
        // ֹͣ��̨�߳�
        running_.store(false);
        condition_.notify_one();
        if (workerThread_.joinable()) {
            workerThread_.join();
        }
    }
}

// �޸���־���Ŀ¼������init֮ǰ��Ч
void Logger::SetLogDir(const std::string& logDir) 
{
    std::lock_guard<std::mutex> lock(fileMutex_);
    logDir_ = logDir;
}

// �޸���־����
void Logger::SetLogLevel(LogLevel level) 
{
    level_.store(level, std::memory_order_relaxed);
}

// �޸ĵ�����־�ļ����ߴ�
void Logger::SetMaxFileSize(size_t size) 
{
    std::lock_guard<std::mutex> lock(fileMutex_);
    maxFileSize_ = size;
}

// �޸������־�ļ�����
void Logger::SetMaxFiles(size_t maxFiles) 
{
    std::lock_guard<std::mutex> lock(fileMutex_);
    maxFiles_ = maxFiles;
}

// �޸��Ƿ������첽д��
void Logger::SetAsync(bool async) 
{
    async_.store(async, std::memory_order_relaxed);
}

// �޸��Ƿ�ͬʱ������ն�
void Logger::setTerminal(bool isTerminal) 
{
    isTerminal_.store(isTerminal, std::memory_order_relaxed);
}

// �޸��Ƿ����Դ�ļ�λ��
void Logger::setSourceFileTrace(bool sourceFileTrace)
{
	sourceFileTrace_.store(sourceFileTrace, std::memory_order_relaxed);
}

// �첽д����־�߳�
void Logger::Worker()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(queueMutex_);

        // ʹ�� wait ֱ�����в�Ϊ�ջ����߳�ֹͣ����
        condition_.wait(lock, [this] {
            return !logQueue_.empty() || !running_.load();
            });

        // ����߳��Ѿ�ֹͣ�Ҷ���Ϊ�գ����˳��߳�
        if (!running_.load() && logQueue_.empty())
        {
            break;
        }

        // �������У�����������ʱ��
        std::queue<LogMessage> temp;
        std::swap(temp, logQueue_);
        lock.unlock();

        // ����д��
        while (!temp.empty())
        {
            WriteToFile(temp.front().message);
            temp.pop();
        }
    }
}

// �ļ���ת
void Logger::RotateFile() 
{
    std::lock_guard<std::mutex> lock(rotateMutex_);
    namespace fs = std::filesystem;

    // �ռ����� .log �ļ�
    std::vector<std::string> files;
    for (auto& entry : fs::directory_iterator(logDir_)) 
    {
        if (entry.path().extension() == ".log") 
        {
            files.push_back(entry.path().string());
        }
    }

    // ��������ʱɾ��������־
    if (files.size() >= maxFiles_) 
    {
        std::sort(files.begin(), files.end());
		size_t filesToDelete = files.size() - maxFiles_ + 1;
		for (size_t i = 0; i < filesToDelete; ++i)
		{
			fs::remove(files[i]);
		}
    }

    // ���ļ���
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

// д���ļ����ն�
void Logger::WriteToFile(const std::string& message) 
{
    std::lock_guard<std::mutex> lock(fileMutex_);

    // �Ƴ�ANSIת���ַ���д���ļ�
    std::string messageToWrite = RemoveAnsiCodes(message);

    // ������С����ת
    if (currentFileSize_ + messageToWrite.size() > maxFileSize_) 
    {
        RotateFile();
    }

    // ���ļ�������δ�򿪣�
    if (!logFile_.is_open()) 
    {
        logFile_.open(currentFilename_, std::ios::app);
    }

    // д�벢ˢ��
    logFile_ << messageToWrite;
    logFile_.flush();
    currentFileSize_ += messageToWrite.size();

    // �����Ҫ�ն������ͬ����ӡ
    if (isTerminal_.load()) 
    {
        std::cout << message;
    }
}

// ��־����ת��Ϊ�ַ���
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

// ��ȡ��ǰʱ���
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

// ��ȡ��ǰ�߳�ID
std::string Logger::GetThreadId() 
{
    std::ostringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

// ȥ��ANSIת���ַ�
std::string Logger::RemoveAnsiCodes(const std::string& str) 
{
    // ʹ��������ʽƥ��������ESC�ַ�(\x1B)��ͷ��
    // ��� '['�����������ֻ�ֺţ������� 'm' ���������У�
    // ���� "\033[35m" �� "\033[0m" ���ܱ�ƥ�䵽��
    std::regex ansi_escape_pattern("\x1B\\[[0-9;]*m");
    return std::regex_replace(str, ansi_escape_pattern, "");
}
