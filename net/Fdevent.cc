#include "Fdevent.h"

namespace ionet
{
    Fdevent::Fdevent():readable(false),writeable(false)
        {

        }
    void Fdevent::setReadable(int flag)
    {
        readable=flag;
    }
    void Fdevent::setWriteable(int flag)
    {
        writeable=flag;
    }
    void Fdevent::setReadCb(fd_callback cb)
    {
        read_callback=cb;
    }
    void Fdevent::setWriteCb(fd_callback cb)
    {
        write_callback=cb;
    }

    /*
    客户端刚接入时,是可读事件,当服务器读取用户数据,并且需要向客户端发送数据时
    需要先关闭读类型,再执行输出事件回调函数;输出回调函数结束之后,再开启可读事件;
    因为epoll默认用的是水平触发,所以某一次未读的数据可以留在下次继续读取
    */
    void Fdevent::handler(int connfd)
    {
        if(readable && read_callback)
            read_callback(connfd);
        if(writeable && write_callback)
            write_callback(connfd);
    }
}
