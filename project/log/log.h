/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef LOG_H
#define LOG_H

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/helpers/stringhelper.h>

#define DEBUG_LOG(p) LOG4CPLUS_DEBUG(walletfront::log::LoggerInstance(), p)
#define INFO_LOG(p)  LOG4CPLUS_INFO(walletfront::log::LoggerInstance(), p)
#define ERROR_LOG(p) LOG4CPLUS_ERROR(walletfront::log::LoggerInstance(), p)

namespace walletfront {

class log
{
    public:
        static log4cplus::Logger & LoggerInstance();

    private:
        void OpenConfigFile();

    private:
        log();
        ~log();
        log(const log&);
        log& operator=(const log&);
};
}
#endif //LOG_H