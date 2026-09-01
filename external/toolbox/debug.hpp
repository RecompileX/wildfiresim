#pragma once

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

namespace toolbox {
namespace detail {

inline std::string& logFilePath() {
    static std::string path = "debug.log";
    return path;
}

inline std::mutex& logMutex() {
    static std::mutex mutex;
    return mutex;
}

inline std::string timestamp() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};
#if defined(_WIN32)
    if (localtime_s(&localTime, &time) != 0) {
        return "00:00:00";
    }
#else
    if (localtime_r(&time, &localTime) == nullptr) {
        return "00:00:00";
    }
#endif

    std::ostringstream stream;
    stream << std::put_time(&localTime, "%H:%M:%S");
    return stream.str();
}

template <typename... Args>
inline std::string buildMessage(Args&&... args) {
    std::ostringstream stream;
    (stream << ... << std::forward<Args>(args));
    return stream.str();
}

template <typename... Args>
inline void writeLog(std::string_view level, Args&&... args) {
    const std::string message = buildMessage(std::forward<Args>(args)...);

    std::lock_guard<std::mutex> lock(logMutex());
    std::ofstream file(logFilePath(), std::ios::app);

    if (!file) {
        std::cerr << "[toolbox] Failed to open log file: " << logFilePath() << '\n';
        std::cerr << '[' << timestamp() << "] [" << level << "] " << message << '\n';
        std::cerr.flush();
        return;
    }

    file << '[' << timestamp() << "] [" << level << "] " << message << '\n';
    file.flush();
}

} // namespace detail

inline bool startLog(std::string_view filename = "debug.log") {
    std::lock_guard<std::mutex> lock(detail::logMutex());

    detail::logFilePath() = filename.empty() ? "debug.log" : std::string(filename);

    std::ofstream file(detail::logFilePath(), std::ios::trunc);
    if (!file) {
        std::cerr << "[toolbox] Failed to start log file: " << detail::logFilePath() << '\n';
        std::cerr.flush();
        return false;
    }

    file.flush();
    return true;
}

template <typename... Args>
inline void info(Args&&... args) {
    detail::writeLog("INFO", std::forward<Args>(args)...);
}

template <typename... Args>
inline void warning(Args&&... args) {
    detail::writeLog("WARNING", std::forward<Args>(args)...);
}

template <typename... Args>
inline void error(Args&&... args) {
    detail::writeLog("ERROR", std::forward<Args>(args)...);
}

} // namespace toolbox
