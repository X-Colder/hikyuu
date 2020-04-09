/*
 * Log.cpp
 *
 *  Created on: 2013-2-1
 *      Author: fasiondog
 */

#include "GlobalInitializer.h"
#include "Log.h"

#if USE_SPDLOG_LOGGER
#include <spdlog/sinks/stdout_color_sinks.h>
#include <iostream>
//#include "spdlog/sinks/ostream_sink.h"
//#include "spdlog/sinks/rotating_file_sink.h"

#if HKU_USE_SPDLOG_ASYNC_LOGGER
#include <spdlog/async.h>
#endif /* HKU_USE_SPDLOG_ASYNC_LOGGER */
#endif /* #if USE_SPDLOG_LOGGER */

namespace hku {

static LOG_LEVEL g_log_level = TRACE;

LOG_LEVEL get_log_level() {
    return g_log_level;
}

/**********************************************
 * Use SPDLOG for logging
 *********************************************/
#if USE_SPDLOG_LOGGER
#if HKU_USE_SPDLOG_ASYNC_LOGGER
void init_logger() {
    // auto stdout_sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(std::cout, true);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    stdout_sink->set_level(spdlog::level::trace);

    spdlog::init_thread_pool(8192, 1);
    std::vector<spdlog::sink_ptr> sinks{stdout_sink};
    auto logger = std::make_shared<spdlog::async_logger>("hikyuu", sinks.begin(), sinks.end(),
                                                         spdlog::thread_pool(),
                                                         spdlog::async_overflow_policy::block);

    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
    // logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v [%!]");
    logger->set_pattern("%Y-%m-%d %H:%M:%S.%e [%^HKU-%L%$] - %v [%!]");
    spdlog::register_logger(logger);
}

std::shared_ptr<spdlog::async_logger> getHikyuuLogger() {
    return spdlog::get("hikyuu");
}

#else /* #if HKU_USE_SPDLOG_ASYNC_LOGGER */

void init_logger() {
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    stdout_sink->set_level(spdlog::level::trace);
    auto logger = std::make_shared<spdlog::logger>("hikyuu", stdout_sink);
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
    logger->set_pattern("%Y-%m-%d %H:%M:%S.%e [%^HKU-%L%$] - %v [%!]");
    spdlog::register_logger(logger);
}

std::shared_ptr<spdlog::logger> HKU_API getHikyuuLogger() {
    return spdlog::get("hikyuu");  // inner::g_hikyuu_logger;
}

#endif /* #if HKU_USE_SPDLOG_ASYNC_LOGGER */

void set_log_level(LOG_LEVEL level) {
    g_log_level = level;
    getHikyuuLogger()->set_level((spdlog::level::level_enum)level);
}

#else  /* #if USE_SPDLOG_LOGGER */
/**********************************************
 * Use SPDLOG for logging
 *********************************************/
void init_logger() {}

void set_log_level(LOG_LEVEL level) {
    g_log_level = level;
}
#endif /* #if USE_SPDLOG_LOGGER */

}  // namespace hku
