#include "logger.hpp"

#include <ctime>
#include <iostream>

namespace lg {
    class Logger {
    public:
        FILE *fp = nullptr;
        LogLevel stdout_level = LogLevel::Trace;
        LogLevel file_level = LogLevel::Info;

    public:
        Logger() = default;
        ~Logger() {
            if (this->fp) {
                fclose(this->fp);
            }
        }
    };

    static Logger g_logger;

    namespace intern {
        void log_message(LogLevel level, const std::string_view& text) {
            char time_buffer[64];
            time_t now = time(nullptr);
            strftime(time_buffer, sizeof(time_buffer), "%M:%S", localtime(&now));
            const std::string message = std::format("[{}] {}\n", time_buffer, text);

            if (level >= g_logger.stdout_level) {
                std::cout << message;
                fflush(stdout);
            }

            if (level >= g_logger.file_level && g_logger.fp) {
                fwrite(message.data(), message.length(), 1, g_logger.fp);
                fflush(g_logger.fp);
            }

            if (level == LogLevel::Fatal) {
                abort();
            }
        }
    }

    void set_file(const std::string& fname, bool append) {
        if (g_logger.fp) {
            fclose(g_logger.fp);
        }

        const char *mode = ((append) ? "a+" : "w+");
        ASSERT((g_logger.fp = fopen(fname.c_str(), mode)));
    }

    void set_file_level(LogLevel level) {
        g_logger.file_level = level;
    }

    void set_stdout_level(LogLevel level) {
        g_logger.stdout_level = level;
    }
}
