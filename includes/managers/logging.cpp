//
// Created by ginko on 12/2/24.
//


#include "logging.h"

void CustomLogCallback(int logType, const char *text, va_list args) {
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), text, args);

    switch (logType) {
        default:
            SPDLOG_TRACE("[RAYLIB]");
            break;
    }
}

namespace Log {
    void Instantiate() {
//        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
//        console_sink->set_level(spdlog::level::info);
//        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
//        SetTraceLogCallback(CustomLogCallback);
//
//        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("../logs/log.txt", true);
//        file_sink->set_level(spdlog::level::trace);
//
//        spdlog::logger logger("multi_sink", {console_sink, file_sink});
//        logger.set_level(spdlog::level::debug);
//
//        SPDLOG_INFO("Instantiating logger");
    }


//    template<typename... T>
//    std::basic_string<char> runtime(std::string_view fmt, T &&... args) {
//        return fmt::format(fmt::runtime(fmt), args...);
//    }

}

