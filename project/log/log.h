/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef LOG_H
#define LOG_H

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/helpers/stringhelper.h>

#define TRACE_LOG(p) LOG4CPLUS_TRACE(walletfront::log::LoggerInstance(), p)
#define DEBUG_LOG(p) LOG4CPLUS_DEBUG(walletfront::log::LoggerInstance(), p)
#define NOTICE_LOG(p) LOG4CPLUS_INFO(walletfront::log::LoggerInstance(), p)
#define WARNING_LOG(p) LOG4CPLUS_WARN(walletfront::log::LoggerInstance(), p)
#define FATAL_LOG(p) LOG4CPLUS_ERROR(walletfront::log::LoggerInstance(), p)
#define TRACE_METHOD_LOG(p) LOG4CPLUS_TRACE_METHOD(walletfront::log::LoggerInstance(), p)

namespace walletfront {

class log
{
    public:
        static log4cplus::Logger & LoggerInstance();

    private:
        void OpenConfigFile();

    private:
        log();
        virtual ~log();
};
}
#endif