#ifndef LOGGING_H_
#define LOGGING_H_

#include <string>

class Logger;

#define LOG_ERROR(format, ...) \
    if(Logger::LogError <= Logger::getLogLevel()) \
        Logger().write(Logger::LogError, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define LOG_WARN(format, ...) \
    if(Logger::LogWarning <= Logger::getLogLevel()) \
        Logger().write(Logger::LogWarning, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define LOG_DEBUG(format, ...) \
    if(Logger::LogDebug <= Logger::getLogLevel()) \
        Logger().write(Logger::LogDebug, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define LOG_INFO(format, ...) \
    if(Logger::LogDebug <= Logger::getLogLevel()) \
        Logger().write(Logger::LogInfo, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

class Logger
{
public:
	enum LogLevel
	{
		LogInfo,
		LogError,
		LogWarning,
		LogDebug
	};

	Logger();
	~Logger();

	static void setLogFile(std::string& file);
	static std::string getLogFile();
	static void setLogLevel(LogLevel level);
	static LogLevel getLogLevel();

	void write(LogLevel level, const char *file, const char *func,
			   int line, const char *format, ...);

private:
	char mData[4096]{};
	char *mCurPtr;
	static LogLevel mThisLogLevel;
	static std::string mLogFile;
};

#endif // LOGGING_H_