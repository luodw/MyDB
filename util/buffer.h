/*
用于存储用户发送的数据的缓存类
*/

#ifndef _BUFFER_
#define _BUFFER_
#include <stdlib.h>
namespace ionet
{
    class Buffer
    {
    public:
        char *buf;
        size_t len;
    };
}


#endif
