/* MIT License
 *
 * Copyright (c) 2026 Yaron Dayan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#pragma once

#include <iostream>
#include <chrono>
#include <mutex>
#include <ctime>
#include <string_view>
#include <fstream>
#include <format>


class Logger {
private:
    static std::mutex& get_mutex() {
        static std::mutex mtx;
        return mtx;
    }

    static std::ofstream& get_file_stream() {
        static std::ofstream log_file("logger.log", std::ios::out | std::ios::app);
        return log_file;
    }

public:
    enum class Level { INFO, WARN, ERROR };

    template <typename... Args>
    static void log(Level level, std::string_view fmt_str, Args&&... args) {
        // 1. Thread-local work (Parallelized across multiple calling threads)
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        const std::time_t time_t_now = std::chrono::system_clock::to_time_t(now);
        std::tm tm_now{};

    #ifdef _WIN32
        localtime_s(&tm_now, &time_t_now);
    #else
        localtime_r(&time_t_now, &tm_now);
    #endif

        char time_buf[20];
        std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &tm_now);

        std::string_view level_str = (level == Level::WARN) ? "WARN" : (level == Level::ERROR) ? "ERROR" : "INFO";

        // Pre-allocate and format locally with strict std:: boundaries
        const std::string full_layout = std::format("[{}] [{}] {}\n", time_buf, level_str, fmt_str);
        const std::string msg = std::vformat(full_layout, std::make_format_args(args...));

        // 2. Critical Section (Briefly serialized to protect stream state)
        {
            std::lock_guard<std::mutex> lock(get_mutex());
            auto& stream = get_file_stream();
            if (stream.is_open()) {
                stream << msg;
            }
        }
    }
};


#define LOG_INFO(fmt, ...)  Logger::log(Logger::Level::INFO, fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...)  Logger::log(Logger::Level::WARN, fmt, __VA_ARGS__)
#define LOG_ERR(fmt, ...)   Logger::log(Logger::Level::ERROR, fmt, __VA_ARGS__)