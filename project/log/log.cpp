#include "log.h"

#define DEFAULT_CONFIG_FILE_PATH "/mnt/e/wallet_front/project/log/log.properties"


namespace walletfront {

log::log()
{
    OpenConfigFile();
}

log::~log()
{

}

void log::OpenConfigFile()
{
    log4cplus::initialize();

    log4cplus::Logger root = log4cplus::Logger::getRoot();
    log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(DEFAULT_CONFIG_FILE_PATH));
}

log4cplus::Logger & log::LoggerInstance()
{ 
    static log *_log = new log();
    static log4cplus::Logger logger  = log4cplus::Logger::getInstance("main_log");

    return logger;
}
}