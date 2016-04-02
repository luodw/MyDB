#ifndef _EPOLLER_
#define _EPOLLER_
#include <sys/epoll.h>
#include <vector>
#include "../util/Nocopyable.h"
namespace ionet
{
    class EventLoop;
    class Epoller:public nocopyable
    {
    public:
        Epoller()=default;
        ~Epoller();
        Epoller(EventLoop *loop);
        void addEvent(int fd,int event);
        void delEvent(int fd,int event);
        void epollWait(std::vector<int>& vfd);
    private:
        //EventLoop *loop;
        int epfd;
        struct epoll_event *events;
        size_t maxEventNum;
    };
}


#endif
