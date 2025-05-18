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
* @brief ��־����ö��
*/
enum class LogLevel : uint8_t 
{
    TRACE,  ///< ������Ϣ������ϸ
    DEBUG,  ///< ������Ϣ
    INFO,   ///< һ����Ϣ
    WARN,   ///< ������Ϣ
    ERROR,  ///< ������Ϣ
    FATAL   ///< ���ش���ͨ�����³�����ֹ
};

/**
* @class Logger
* @brief ������־��������֧��ͬ��/�첽д�ļ����ն�������ļ���ת�ȹ���
*/
class Logger 
{
public:
    /**
    * @brief ��ȡ��־����������
    * @return Logger& ��������
    */
    static Logger& Instance();

    /**
    * @brief ��ʼ����־ϵͳ
    * @param logDir ��־�ļ����Ŀ¼�����������򴴽�
    * @param level ��־���˼��𣬵��ڴ˼������־��������
    * @param maxFileSize ������־�ļ����ߴ磨�ֽڣ�����������ת
    * @param maxFiles �����־�ļ�������������ɾ�����
    * @param async �Ƿ������첽д�루��̨�̣߳�
    * @param isTerminal �Ƿ�ͬʱ����־������ն�
    */
    void Init(const std::string& logDir = "logs",
        LogLevel level = LogLevel::TRACE,
        size_t maxFileSize = 1048576,  // 1MB
        size_t maxFiles = 10,
        bool async = false,
        bool isTerminal = true,
        bool sourceFileTrace = false);

    /**
	* @brief ��¼һ����־
    * @param level ��־����
    * @param message ��־����
    * @param loc ���ô�Դ��λ����Ϣ��Ĭ��ʹ�� std::source_location::current()
    */
    void Log(LogLevel level, const std::string& message);

    /**
    * @brief ���������������̨�̲߳�ˢ��ʣ����־
    */
    ~Logger();

    /**
    * @brief �޸���־���Ŀ¼
    * @param logDir �µ���־Ŀ¼
    */
    void SetLogDir(const std::string& logDir);

    /**
    * @brief �޸���־����
    * @param level �µ���־����
    */
    void SetLogLevel(LogLevel level);

    /**
    * @brief �޸ĵ�����־�ļ����ߴ�
    * @param size �µ�����ļ��ߴ磨�ֽڣ�
    */
    void SetMaxFileSize(size_t size);

    /**
    * @brief �޸������־�ļ�����
    * @param maxFiles �µ������־�ļ�����
    */
    void SetMaxFiles(size_t maxFiles);

    /**
    * @brief �޸��Ƿ������첽д��
    * @param async �Ƿ������첽
    */
    void SetAsync(bool async);

    /**
    * @brief �޸��Ƿ�ͬʱ������ն�
    * @param isTerminal �Ƿ�������ն�
    */
    void setTerminal(bool isTerminal);

	/**
	* @brief �޸��Ƿ����Դ�ļ�λ��
	* @param sourceFileTrace �Ƿ����Դ�ļ�λ��
    */
	void setSourceFileTrace(bool sourceFileTrace);

private:
    Logger() = default;  ///< ˽�й��캯����ȷ������

    /// @brief �ڲ���װ����־��Ϣ�ṹ
    struct LogMessage 
    {
        std::string message;                                      ///< ��������־�ı�
        LogLevel level;                                           ///< ��־����
        std::chrono::system_clock::time_point timestamp;          ///< ����ʱ��
    };

    // ������������ ���ú�״̬ ������������
    std::atomic<bool> running_{ false };             ///< ��̨�߳�����״̬
    std::atomic<LogLevel> level_{ LogLevel::INFO };  ///< ��ǰ��־������ֵ
    std::atomic<bool> async_{ true };                ///< �Ƿ��첽д��־
    std::atomic<bool> isTerminal_{ true };           ///< �Ƿ�������ն�
	std::atomic<bool> sourceFileTrace_{ false };     ///< �Ƿ����Դ�ļ�λ��

    std::ofstream logFile_;                      ///< ��ǰ�򿪵���־�ļ���
    std::string logDir_;                         ///< ��־Ŀ¼
    size_t maxFileSize_{ 0 };                    ///< ���ļ����ߴ�
    size_t maxFiles_{ 0 };                       ///< ����ļ�����

    // ������������ �첽������� ������������
    std::queue<LogMessage> logQueue_;          ///< ��д����־����
    std::mutex queueMutex_;                    ///< ���л�����
    std::condition_variable condition_;        ///< ����֪ͨ����
    std::thread workerThread_;                 ///< ��̨д��־�߳�

    // ������������ �ļ���ת״̬ ������������
    std::string currentFilename_;              ///< ��ǰ��־�ļ���
    size_t currentFileSize_{ 0 };              ///< ��ǰ�ļ���д���С
    std::mutex fileMutex_;                     ///< �ļ�����������
	std::mutex rotateMutex_; 			       ///< ��ת����������

    // ������������ �ڲ����� ������������
    void Worker();                                       ///< �첽д���߳����
    void RotateFile();                                   ///< ��ת��־�ļ�
    void WriteToFile(const std::string& message);        ///< ������д�ļ������նˣ�����
    std::string LevelToString(LogLevel level);           ///< ������תΪ�ı�
    std::string GetTimeStamp();                          ///< ��ȡ��ǰʱ����ַ���
    std::string GetThreadId();                           ///< ��ȡ��ǰ�߳� ID �ַ���
    std::string RemoveAnsiCodes(const std::string& str); ///< ȥ��ANSIת���ַ�
};


#endif // LOGGER_H
