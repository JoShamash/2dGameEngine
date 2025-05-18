#ifndef LOGGER_H
#define LOGGER_H

#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <chrono>
#include <source_location>
#include <string>
#include <thread>
#include <regex>

/**
* @enum LogLevel
* @brief 日志级别枚举
*/
enum class LogLevel : uint8_t 
{
    TRACE,  ///< 跟踪信息，最详细
    DEBUG,  ///< 调试信息
    INFO,   ///< 一般信息
    WARN,   ///< 警告信息
    ERROR,  ///< 错误信息
    FATAL   ///< 严重错误，通常导致程序中止
};

/**
* @class Logger
* @brief 单例日志管理器，支持同步/异步写文件与终端输出、文件轮转等功能
*/
class Logger 
{
public:
    /**
    * @brief 获取日志管理器单例
    * @return Logger& 单例引用
    */
    static Logger& Instance();

    /**
    * @brief 初始化日志系统
    * @param logDir 日志文件存放目录，若不存在则创建
    * @param level 日志过滤级别，低于此级别的日志将被忽略
    * @param maxFileSize 单个日志文件最大尺寸（字节），超出则轮转
    * @param maxFiles 最大日志文件数量，超出则删除最旧
    * @param async 是否启用异步写入（后台线程）
    * @param isTerminal 是否同时将日志输出到终端
    */
    void Init(const std::string& logDir = "logs",
        LogLevel level = LogLevel::TRACE,
        size_t maxFileSize = 1048576,  // 1MB
        size_t maxFiles = 10,
        bool async = false,
        bool isTerminal = true,
        bool sourceFileTrace = false);

    /**
	* @brief 记录一条日志
    * @param level 日志级别
    * @param message 日志内容
    * @param loc 调用处源码位置信息，默认使用 std::source_location::current()
    */
    void Log(LogLevel level, const std::string& message);

    /**
    * @brief 析构函数，清理后台线程并刷新剩余日志
    */
    ~Logger();

    /**
    * @brief 修改日志存放目录
    * @param logDir 新的日志目录
    */
    void SetLogDir(const std::string& logDir);

    /**
    * @brief 修改日志级别
    * @param level 新的日志级别
    */
    void SetLogLevel(LogLevel level);

    /**
    * @brief 修改单个日志文件最大尺寸
    * @param size 新的最大文件尺寸（字节）
    */
    void SetMaxFileSize(size_t size);

    /**
    * @brief 修改最大日志文件数量
    * @param maxFiles 新的最大日志文件数量
    */
    void SetMaxFiles(size_t maxFiles);

    /**
    * @brief 修改是否启用异步写入
    * @param async 是否启用异步
    */
    void SetAsync(bool async);

    /**
    * @brief 修改是否同时输出到终端
    * @param isTerminal 是否输出到终端
    */
    void setTerminal(bool isTerminal);

	/**
	* @brief 修改是否输出源文件位置
	* @param sourceFileTrace 是否输出源文件位置
    */
	void setSourceFileTrace(bool sourceFileTrace);

private:
    Logger() = default;  ///< 私有构造函数，确保单例

    /// @brief 内部封装的日志消息结构
    struct LogMessage 
    {
        std::string message;                                      ///< 完整的日志文本
        LogLevel level;                                           ///< 日志级别
        std::chrono::system_clock::time_point timestamp;          ///< 生成时间
    };

    // —————— 配置和状态 ——————
    std::atomic<bool> running_{ false };             ///< 后台线程运行状态
    std::atomic<LogLevel> level_{ LogLevel::INFO };  ///< 当前日志级别阈值
    std::atomic<bool> async_{ true };                ///< 是否异步写日志
    std::atomic<bool> isTerminal_{ true };           ///< 是否输出到终端
	std::atomic<bool> sourceFileTrace_{ false };     ///< 是否输出源文件位置

    std::ofstream logFile_;                      ///< 当前打开的日志文件流
    std::string logDir_;                         ///< 日志目录
    size_t maxFileSize_{ 0 };                    ///< 单文件最大尺寸
    size_t maxFiles_{ 0 };                       ///< 最大文件数量

    // —————— 异步队列相关 ——————
    std::queue<LogMessage> logQueue_;          ///< 待写入日志队列
    std::mutex queueMutex_;                    ///< 队列互斥锁
    std::condition_variable condition_;        ///< 队列通知条件
    std::thread workerThread_;                 ///< 后台写日志线程

    // —————— 文件轮转状态 ——————
    std::string currentFilename_;              ///< 当前日志文件名
    size_t currentFileSize_{ 0 };              ///< 当前文件已写入大小
    std::mutex fileMutex_;                     ///< 文件操作互斥锁
	std::mutex rotateMutex_; 			       ///< 轮转操作互斥锁

    // —————— 内部方法 ——————
    void Worker();                                       ///< 异步写入线程入口
    void RotateFile();                                   ///< 轮转日志文件
    void WriteToFile(const std::string& message);        ///< 真正的写文件（和终端）操作
    std::string LevelToString(LogLevel level);           ///< 将级别转为文本
    std::string GetTimeStamp();                          ///< 获取当前时间戳字符串
    std::string GetThreadId();                           ///< 获取当前线程 ID 字符串
    std::string RemoveAnsiCodes(const std::string& str); ///< 去除ANSI转义字符
};


#endif // LOGGER_H
