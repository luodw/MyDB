/*
用于管理底层事件监听函数,这个类可以写成泛型,
以支持底层不同的事件监听函数
*/
#ifndef _EVENTLOOP_
#define _EVENTLOOP_

#include <unordered_map>
#include <vector>
#include "Fdevent.h"
#include "Ionet.h"
#include "../util/Nocopyable.h"
namespace ionet
{
    class Epoller;
    class EventLoop:public nocopyable
    {
    public:
        EventLoop();
        ~EventLoop();
        //添加一个事件,fd为文件描述符,event为事件类型(见Ionet.h),事件回调函数
        void addEvent(int fd,int event,Fdevent::fd_callback cb);
        void delEvent(int fd,int event);//删除一个事件类型,可以是Ionet::IONET_READ或者Ionet::IONET_WRITE
        void startLoop();//开启事件循环
    private:
        std::unordered_map<int,Fdevent*> map_callback;//保存文件描述符和它的事件类
        std::vector<int> activeEvent;//激活的文件描述符,可以从底层epoll,select或poll获取
        Epoller *epoller;//epoll类,
    };
}


#endif
