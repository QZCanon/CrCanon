//
// Created by qinzhou on 23-5-9.
//

#include "log.h"

#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include <cstdarg>

std::string LogOutPath = "./";

std::string Logger::mLogFile = LogOutPath;
Logger::LogLevel Logger::mThisLogLevel = Logger::LogDebug;

Logger::Logger()
	:
	mCurPtr(mData)
{}

Logger::~Logger() = default;

void Logger::setLogFile(std::string& file)
{
	Logger::mLogFile = file;
}

std::string Logger::getLogFile()
{
	return Logger::mLogFile;
}

void Logger::setLogLevel(LogLevel level)
{
	Logger::mThisLogLevel = level;
}

Logger::LogLevel Logger::getLogLevel()
{
	return Logger::mThisLogLevel;
}

void Logger::write(Logger::LogLevel level, const char *file, const char *func,
				   int line, const char *format, ...)
{
	struct timeval now = {0, 0};
	gettimeofday(&now, nullptr);
	struct tm *sysTime = localtime(&(now.tv_sec));

	snprintf(mCurPtr, 4096, "[%d-%02d-%02d %02d:%02d:%02d] ",
			sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday,
			sysTime->tm_hour, sysTime->tm_min, sysTime->tm_sec);

	printf("%s", mCurPtr);
	std::string outString;
	if (level == Logger::LogInfo) {
		outString += "[\e[1;32mINF0\e[0m]";
	} else if (level == Logger::LogDebug) {
		outString += "[DEBUG]";
	}
	else if (level == Logger::LogWarning) {
		outString += "[\e[1;33mWARN\e[0m]";
	}
	else if (level == Logger::LogError) {
		outString += "[\e[1;31mERROR\e[0m]";
	}
	else {
		return;
	}

	if (level == Logger::LogError) {
		outString += "\e[1;31m["
			+ std::string(file)
			+ "] [line:" + std::to_string(line) + "] "
			+ std::string(func) + "() | \e[0m";
	} else {
		outString += std::string(file)
			+ "] [line:" + std::to_string(line)
			+ "] " + std::string(func) + "() | ";
	}

	printf("%s", outString.c_str());

	va_list valst;
	va_start(valst, format);
	vsnprintf(mCurPtr, sizeof(mData) - (mCurPtr - mData), format, valst);
	va_end(valst);

	if (level == Logger::LogInfo) {
		printf("\e[1;32m%s\e[0m", mCurPtr);
	} else if (level == Logger::LogWarning) {
		printf("\e[1;33m%s\e[0m", mCurPtr);
	} else if (level == Logger::LogError) {
		printf("\e[1;31m%s\e[0m", mCurPtr);
	} else {
		printf("%s", mCurPtr);
	}
}

