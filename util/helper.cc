#include "helper.h"
#include <unistd.h>
#include <time.h>
#include <string>
#include <stdlib.h>

using namespace std;
namespace helper
{
    //获取日志的日期
    string Date()
    {
        char buf[80];
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo=localtime(&rawtime);
        strftime(buf, sizeof(buf),  "%Y-%m-%d", timeinfo);
        return buf;
    }

    //获取日志的时间
    string Time()
    {
        char buf[80];
        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(buf, 80, "%H:%M:%S", timeinfo);

        return buf;
    }

    string strpid()
    {
        char buf[80];
        pid_t pid=getpid();
        sprintf(buf, "%d", (int)pid);
        return buf;
    }

}
