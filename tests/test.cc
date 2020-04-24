#include <iostream>
#include "../skylu/log.h"
using namespace skylu;
int main(int argc,char **argv)
{
    Logger::ptr logger(new Logger());
    logger->addAppender(LogAppender::ptr(new StdoutLogAppender));
    LogEvent::ptr event(new LogEvent(logger,LogLevel::DEBUG,__FILE__, __LINE__,0,1,2,time(0),"test"));
    event->getSS()<< "hello sylar log";
    logger->log(LogLevel::DEBUG,event);

    std::cout<<"hello salyr"<<std::endl;

    return 0;

}
