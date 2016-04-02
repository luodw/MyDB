#include <algorithm>
#include <functional>
#include <unistd.h>
#include "Epoller.h"
#include "Fdevent.h"
#include "Ionet.h"
#include "EventLoop.h"
#include "../util/Logger.h"
using namespace std;

namespace ionet
{
    EventLoop::EventLoop()
    {
        epoller=new Epoller(this);
    }
    EventLoop::~EventLoop()
    {
        delete epoller;
        //STL和lambda函数结合,简直完美,非常简练,省去写仿函数或者函数.
        for_each(map_callback.begin(), map_callback.end(), [](pair<int,Fdevent*> iter)
        {
            close(iter.first);
            delete iter.second;
        });
    }
    void EventLoop::addEvent(int fd,int event,Fdevent::fd_callback cb)
    {
        if(event & Ionet::IONET_READ)
        {
            /*
            如果是添加可读事件,说明是客户端刚连接的时候,所以这时候要new一个Fdevent类
            并且设置可读,读回调函数
            */
            Fdevent *tmpEvent=new Fdevent();
            tmpEvent->setReadable(1);
            tmpEvent->setReadCb(cb);
            map_callback[fd]=tmpEvent;
            //Logger::logging(Logger::TRACE,__FILE__,__LINE__, "Add a fd read event,fd=%d!",fd);
            epoller->addEvent(fd, event);//底层添加进epoller事件监听循环中
        }
        else if(event & Ionet::IONET_WRITE)
        {
            /*
            如果是是添加可写事件,则需要开启可读以及可读回调函数
            必须先关闭可写事件,因为先必须先处理完第一次客户端请求,才能继续接收客户端请求
            */
            map_callback[fd]->setWriteable(1);
            map_callback[fd]->setWriteCb(cb);
            map_callback[fd]->setReadable(0);//写事件先把读事件关闭,写事件回调函数执行之后再打开.
            //Logger::logging(Logger::TRACE,__FILE__,__LINE__, "Add a fd write event,fd=%d!",fd);
            epoller->addEvent(fd, event);//底层添加可读事件
        }
        else
        {
            Logger::logging(Logger::TRACE,__FILE__,__LINE__, "Wrong event type!");
        }
    }
    void EventLoop::delEvent(int fd,int event)
    {
        if(event & Ionet::IONET_READ)
        {
            /*
            删除可写事件,需要将事件从map_callback中删除,再到底层epoller删除
            */
            unordered_map<int,Fdevent*>::iterator iter=map_callback.find(fd);
            map_callback.erase(iter);
            epoller->delEvent(fd, event);
            delete iter->second;
            close(fd);//关闭这个文件描述符,否则不能重复利用
            //Logger::logging(Logger::TRACE,__FILE__,__LINE__, "Delete a fd read event,fd=%d!",fd);
        }
        else if(event & Ionet::IONET_WRITE)
        {
            /*
            删除可读事件,只需关闭可读事件,以及底层删除可读事件
            */
            map_callback[fd]->setWriteable(0);
            epoller->delEvent(fd, event);
            map_callback[fd]->setReadable(1);
            //Logger::logging(Logger::TRACE,__FILE__,__LINE__, "Delete a fd write event,fd=%d!",fd);
        }
        else
        {
            Logger::logging(Logger::TRACE,__FILE__,__LINE__, "Wrong event type!");
        }
    }
    void EventLoop::startLoop()
    {
        /*
        从底层获取活跃的事件fd,并调用相应的回调函数
        */
        epoller->epollWait(activeEvent);
        //声明一个临时变量vector,清空原先活跃fd数组.
        vector<int> tmpvec;
        activeEvent.swap(tmpvec);
        for(auto &x:tmpvec)
        {
            map_callback[x]->handler(x);//只需活跃fd的回调函数
        }
    }
}
