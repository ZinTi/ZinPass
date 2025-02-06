#include "datetime.h"
#include <chrono>
#include <iomanip>
#include <sstream>

std::string getCurrentDateTime() {
    // 获取当前系统时钟的时间点
    const auto now = std::chrono::system_clock::now();
    // 将时间点转换为 time_t 类型
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    // 转换为本地时间
    const std::tm tm = *std::localtime(&time);
    std::stringstream ss;
    // 按照 YYYY-MM-dd HH:mm:ss 的格式输出时间
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string getCurrentDate() {
    // 获取当前系统时钟的时间点
    const auto now = std::chrono::system_clock::now();
    // 将时间点转换为 time_t 类型
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    // 转换为本地时间
    const std::tm tm = *std::localtime(&time);
    std::stringstream ss;
    // 按照 YYYY-MM-dd 的格式输出日期
    ss << std::put_time(&tm, "%Y-%m-%d");
    return ss.str();
}

std::string getCurrentTime() {
    // 获取当前系统时钟的时间点
    const auto now = std::chrono::system_clock::now();
    // 将时间点转换为 time_t 类型
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    // 转换为本地时间
    const std::tm tm = *std::localtime(&time);
    std::stringstream ss;
    // 按照 HH:mm:ss 的格式输出时间
    ss << std::put_time(&tm, "%H:%M:%S");
    return ss.str();
}