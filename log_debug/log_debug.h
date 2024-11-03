#pragma once
#include <stdio.h>

// 定义日志级别
enum LogLevel {
    LOG_LEVEL_DEBUG = 1,
    LOG_LEVEL_INFO = 2,
    LOG_LEVEL_WARNING = 3,
    LOG_LEVEL_ERROR = 4
};

// 全局变量，用于存储当前的日志级别
LogLevel currentLogLevel = LOG_LEVEL_DEBUG;

// 设置日志级别的函数
void setLogLevel(LogLevel level) {
    currentLogLevel = level;
}

// 日志宏定义
#ifdef _DEBUG
#define LOG_DEBUG(...) if (currentLogLevel <= LOG_LEVEL_DEBUG) printf(__VA_ARGS__)
#define LOG_INFO(...) if (currentLogLevel <= LOG_LEVEL_INFO) printf(__VA_ARGS__)
#define LOG_WARNING(...) if (currentLogLevel <= LOG_LEVEL_WARNING) printf(__VA_ARGS__)
#define LOG_ERROR(...) if (currentLogLevel <= LOG_LEVEL_ERROR) printf(__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARNING(...)
#define LOG_ERROR(...)
#endif