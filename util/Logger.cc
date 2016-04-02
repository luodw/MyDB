#include "Logger.h"
#include "helper.h"
#include <iostream>
#include <stdio.h>
#include <stdarg.h>

using namespace std;

namespace ionet
{
    ostream& Logger::output(cout);
    //用于将日志级别转为字符串
    vector<string> Logger::levelToStr={"TRACE","DEBUG","INFO","WARN","ERROR"};
    //当前使用的日志级别
    int Logger::currentLevel=Logger::ERROR;

    //设置日志级别
    void Logger::setCurrentLevel(int level)
    {
        currentLevel=level;
    }
    //日志输出函数实现
    void Logger::logging(int level, const std::string &file, int line, const char* fmat,...)
    {
        if(level<=currentLevel)
        {
            char buf[1024];
            const string& levelstr=levelToStr[level];
            string msgContent;
            msgContent.append(helper::Date());//获取日期
            msgContent.push_back(' ');

            msgContent.append(helper::Time());//获取时间
            msgContent.push_back(' ');

            msgContent.append(helper::strpid());//获取进程ID
            msgContent.push_back(' ');

            msgContent.append(file);//输出源文件__FILE__
            msgContent.push_back(' ');

            sprintf(buf,"line=%d", line);
            msgContent.append(buf);//输出行数
            msgContent.push_back(' ');

            msgContent.push_back('[');
            msgContent.append(levelstr);//输出日志级别
            msgContent.push_back(']');

            msgContent.push_back(' ');
            msgContent.append("Ionet");//输出日志标识
            msgContent.push_back(':');
            msgContent.push_back(' ');

            va_list ap;
            va_start(ap,fmat);
            sprintf(buf,fmat,va_arg(ap,int));//日志内容
            va_end(ap);
            msgContent.append(string(buf));
            output<<msgContent<<endl;//输出到显示器或文件
        }


    }

}
