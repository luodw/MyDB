#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "EventLoop.h"
#include "Epoller.h"
#include "../util/Logger.h"
#include "Ionet.h"
#include <assert.h>
using namespace std;
namespace ionet
{
    const int MAXSIZE=1024;
    Epoller::Epoller(EventLoop *loop)
    {
        epfd=epoll_create1(0);

        if(epfd==-1)
        {
            Logger::logging(Logger::ERROR,__FILE__,__LINE__, "epoll_create fail");
            exit(-1);
        }
        maxEventNum=MAXSIZE;
        events=(struct epoll_event*)calloc(maxEventNum,sizeof(struct epoll_event));
        if(events==NULL)
        {
            Logger::logging(Logger::ERROR,__FILE__,__LINE__, "events malloc fail");
            exit(-1);
        }
    }
    Epoller::~Epoller()
    {
        close(epfd);
        free(events);
    }

    void Epoller::addEvent(int fd,int event)
    {
        struct epoll_event tmpEvent;
        bzero(&tmpEvent,sizeof(tmpEvent));
        if(event & Ionet::IONET_READ)
        {
            tmpEvent.data.u64=0;
            tmpEvent.data.fd=fd;
            tmpEvent.events = EPOLLIN;
            int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&tmpEvent);
            if(ret==-1)
            {
                Logger::logging(Logger::ERROR,__FILE__,__LINE__, strerror(errno));
                exit(-1);
            }
        }
        else if(event & Ionet::IONET_WRITE)
        {
            tmpEvent.data.u64=0;
            tmpEvent.data.fd=fd;
            tmpEvent.events |= EPOLLOUT;
            int ret=epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&tmpEvent);
            if(ret==-1)
            {
                Logger::logging(Logger::ERROR,__FILE__,__LINE__, strerror(errno));
                exit(-1);
            }
        }

    }
    void Epoller::delEvent(int fd, int event)
    {
        struct epoll_event tmpEvent;
        bzero(&tmpEvent,sizeof(tmpEvent));
        if(event & Ionet::IONET_READ)
        {
            int ret=epoll_ctl(epfd,EPOLL_CTL_DEL,fd,0);
            if(ret==-1)
            {
                Logger::logging(Logger::ERROR,__FILE__,__LINE__, strerror(errno));
                exit(-1);
            }
        }
        else if(event & Ionet::IONET_WRITE)
        {
            tmpEvent.data.u64=0;
            tmpEvent.data.fd=fd;
            tmpEvent.events=0;
            tmpEvent.events = EPOLLIN;
            int ret=epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&tmpEvent);
            if(ret==-1)
            {
                Logger::logging(Logger::ERROR,__FILE__,__LINE__, strerror(errno));
                exit(-1);
            }
        }

    }

    void Epoller::epollWait(std::vector<int> &vfd)
    {
        int nread=-1;
        nread=epoll_wait(epfd, events, maxEventNum, -1);
        if(nread==-1)
        {
            Logger::logging(Logger::ERROR,__FILE__,__LINE__, strerror(errno));
            exit(-1);
        }
        for(int i=0;i<nread;i++)
        {
            vfd.push_back(events[i].data.fd);
        }
    }
}
