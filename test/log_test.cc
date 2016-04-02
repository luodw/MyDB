#include "../util/Logger.h"
#include <assert.h>

void test_trace()
{
    ionet::Logger::logging(ionet::Logger::TRACE,__FILE__,__LINE__,"test-trace ok=%d!" ,23);
}
void test_debug()
{
    ionet::Logger::logging(ionet::Logger::DEBUG,__FILE__,__LINE__,"test-debug ok!");
}
void test_info()
{
    ionet::Logger::logging(ionet::Logger::INFO,__FILE__,__LINE__,"test-info ok!");
}
void test_warn()
{
    ionet::Logger::logging(ionet::Logger::WARN,__FILE__,__LINE__,"test-warn ok!");
}
void test_error()
{
    ionet::Logger::logging(ionet::Logger::ERROR,__FILE__,__LINE__,"test-error ok!");
}
int main(void)
{
    test_trace();
    test_debug();
    test_info();
    test_warn();
    test_error();
    return 0;
}
