#ifndef _IONET_
#define _IONET_
#include "Fdevent.h"
#include "EventLoop.h"
#include "../util/Nocopyable.h"
namespace ionet
{
    class EventLoop;
    class HashTable;
    struct HashNode
    {
        HashNode(std::string k,std::string v):key(k),value(v){}
        std::string key;
        std::string value;
        struct HashNode *next;
    };//哈希表节点类型
    struct Client
    {
        Client()
        {
            std::vector<std::string> tmp(3,std::string());
            args.swap(tmp);
        }//因为目前最多三个参数,所以暂时分配三个string
        int fd;//这个客户端的fd
        std::string outbuf;//存储回复客户端的数据
        std::vector<std::string> args;//存储用户输入的命令
    };//客户端结构体
    class Ionet:public nocopyable
    {
    public:
        enum {
            IONET_NONE=0,
            IONET_READ=1,
            IONET_WRITE=2,
            IONET_ERROR=4
        };//事件类型
        Ionet();
        ~Ionet();
        //用ip和端口来初始化这个服务器
        void netInit(short port);
        void run();//执行事件监听循环
    private:
        void FdWrite(int);
        void FdAccept(int);//listened读事件回调函数
        void FdHandler(int);//客户端读事件回调函数
        int listenfd;//服务监听的文件描述符
        EventLoop *loop;
        HashTable *htable;
        Client *clients;
    };
}

#endif
