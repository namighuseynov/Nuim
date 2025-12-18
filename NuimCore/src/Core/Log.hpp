#pragma once
#include "Base.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <mutex>

namespace Nuim {

    class Log {
    public:
        enum class Level { Info, Warn, Error };

        static void SetEnabled(bool enabled) { s_enabled = enabled; }

        template<typename... Args>
        static void Info(const std::string& fmt, Args&&... args) {
            Write(Level::Info, fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Warn(const std::string& fmt, Args&&... args) {
            Write(Level::Warn, fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Error(const std::string& fmt, Args&&... args) {
            Write(Level::Error, fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static std::string Format(const std::string& fmt, Args&&... args) {
            std::ostringstream oss;
            FormatImpl(oss, fmt, std::forward<Args>(args)...);
            return oss.str();
        }

    private:
        template<typename... Args>
        static void Write(Level lvl, const std::string& fmt, Args&&... args) {
            if (!s_enabled) return;

            std::lock_guard<std::mutex> lock(s_mutex);

            const char* prefix = "";
            switch (lvl) {
            case Level::Info:  prefix = "[Info] ";  break;
            case Level::Warn:  prefix = "[Warn] ";  break;
            case Level::Error: prefix = "[Error] "; break;
            }

            std::cout << prefix << Format(fmt, std::forward<Args>(args)...) << "\n";
        }

        static void FormatImpl(std::ostringstream& oss, const std::string& fmt) {
            oss << fmt;
        }

        template<typename T, typename... Rest>
        static void FormatImpl(std::ostringstream& oss, const std::string& fmt, T&& value, Rest&&... rest) {
            const size_t pos = fmt.find("{}");
            if (pos == std::string::npos) {
                oss << fmt << " " << value;
                (void)std::initializer_list<int>{ (oss << " " << rest, 0)... };
                return;
            }

            oss << fmt.substr(0, pos);
            oss << value;
            FormatImpl(oss, fmt.substr(pos + 2), std::forward<Rest>(rest)...);
        }

    private:
        inline static bool s_enabled = true;
        inline static std::mutex s_mutex;
    };

}
