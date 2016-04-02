#ifndef _LOGGER_
#define _LOGGER_

#include <assert.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdarg.h>

#include "Nocopyable.h"
namespace ionet
{
    const int LEVEL_SIZE=5;//默认日志级别为5
    class Logger:public nocopyable
    {
    public:
        Logger()=default;
        enum{TRACE=0,DEBUG,INFO,WARN,ERROR};//定义日志级别
        //日志输出函数
        static void logging(int level, const std::string &file, int line, const char* fmat,...);
        //设置日志级别
        static void setCurrentLevel(int level);
    private:
        //输出流,可以是cout输出到显示器,也可以是ofstream输出到文件
        static std::ostream& output;
        //当前的日志级别
        static int currentLevel;
        //日志级别和日志级别字符串的映射
        static std::vector<std::string> levelToStr;
    };
}

#endif
