#pragma once

#include <format>
#include <string_view>

namespace lg {
    enum class LogLevel {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal
    };

    void set_file(const std::string& fname, bool append = false);
    void set_file_level(LogLevel level);
    void set_stdout_level(LogLevel level);

    namespace intern {
        void log_message(LogLevel level, const std::string_view& text);
    }

    template <typename... Args>
    void log(LogLevel level, const std::string_view& fmt, Args&&...args) {
        intern::log_message(level, std::vformat(fmt, std::make_format_args(args...)));
    }

    template <typename... Args>
    void trace(std::string_view fmt, Args&& ...args) {
        log(LogLevel::Trace, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void debug(std::string_view fmt, Args&& ...args) {
        log(LogLevel::Debug, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(std::string_view fmt, Args&& ...args) {
        log(LogLevel::Info, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(std::string_view fmt, Args&& ...args) {
        log(LogLevel::Warn, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(std::string_view fmt, Args&& ...args) {
        log(LogLevel::Error, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void fatal(std::string_view fmt, Args&& ...args) {
        log(LogLevel::Fatal, fmt, std::forward<Args>(args)...);
    }
}
