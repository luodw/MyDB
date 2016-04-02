#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <algorithm>
#include "Ionet.h"
#include "../util/Logger.h"
#include "Fdevent.h"
#include "../util/buffer.h"
#include "../util/hash.h"
#include "../util/parse.h"
#include "../db/hashtable.h"

using namespace std;
namespace ionet
{
    void setnoblocking(int fd)
    {
        //设置文件描述符为非阻塞
        fcntl(fd,F_SETFL,fcntl(fd, F_GETFL,0)|O_NONBLOCK);
    }
    void Ionet::FdWrite(int connfd)
    {
        string buf=clients[connfd].outbuf;
        int ret=send(connfd,buf.c_str(),buf.length(),0);
        if(ret<0)
        {
            Logger::logging(Logger::ERROR,__FILE__,__LINE__, strerror(errno));
            exit(-1);
        }
        //写事件结束之前必须关闭这个客户端的写事件,因为没有数据可写了
        loop->delEvent(connfd, Ionet::IONET_WRITE);
    }
    void Ionet::FdHandler(int connfd)
    {
        //这只是个简单的服务器,支持回显,所以客户端读事件简单读取客户端发送的内容,在回显给客户端
        char buf[1024];
        int nr=recv(connfd,buf,sizeof(buf),0);
        buf[nr]='\0';//手动添加字符串结束符
        if(nr<0)//读取失败
        {
            Logger::logging(Logger::ERROR,__FILE__,__LINE__, strerror(errno));
            exit(-1);
        }
        else if(nr==0)//客户端关闭,服务器读取0字节字符
        {
            Logger::logging(Logger::TRACE,__FILE__,__LINE__, "Client %d exit....",connfd);
            vector<string> v(3,string());
            clients[connfd].args.swap(v);//删除connfd对应的client上的字符串
            loop->delEvent(connfd, Ionet::IONET_READ);
        }else
        {//到这表示正常读取客户端数据
            Parse parse(buf);//解析字符串为命令参数
            vector<string> v(3,string());
            clients[connfd].args.swap(v);//每次运行必须清空,否则会影响后面的命令
            parse.parseArgs(clients[connfd]);
            if(clients[connfd].args[0]==string("set"))
            {
                string key=clients[connfd].args[1];
                string value=clients[connfd].args[2];
                if(!key.empty() && !value.empty())
                {
                    HashNode *node=new HashNode(key,value);
                    htable->insert(Hash(key), node);
                    memset(buf,sizeof(buf),0);
                    sprintf(buf,"Client %d set key:%s value:%s!" ,connfd,key.c_str(),value.c_str());
                    Logger::logging(Logger::TRACE,__FILE__,__LINE__, buf);
                    clients[connfd].outbuf=string("ok\r\n");
                }
                else{
                    clients[connfd].outbuf=string("usage:set key value\r\n");
                }

            }//set命令
            else if(clients[connfd].args[0]==string("get"))
            {
                string key=clients[connfd].args[1];
                if(!key.empty() && key!=string("all"))
                {
                    string value=htable->find(Hash(key),key);
                    memset(buf,sizeof(buf),0);
                    sprintf(buf,"Client %d get key:%s!" ,connfd,key.c_str());
                    Logger::logging(Logger::TRACE,__FILE__,__LINE__, buf);
                    if(value.empty())
                    {
                        clients[connfd].outbuf=string("No such key\r\n");
                    }
                    else
                    {
                        clients[connfd].outbuf=value+string("\r\n");
                    }
                }
                else if(!key.empty() && key==string("all")){
                    vector<string> vkey;
                    htable->getAll(vkey);
                    memset(buf,sizeof(buf),0);
                    sprintf(buf,"Client %d get all!" ,connfd);
                    Logger::logging(Logger::TRACE,__FILE__,__LINE__, buf);
                    if(vkey.size()!=0)
                    {
                        string result;
                        for_each(vkey.begin(),vkey.end(),[&result](string &str)
                        {
                            result.append(str);
                            result.append(string("\r\n"));
                        });
                        clients[connfd].outbuf=result;
                    }
                    else{
                        clients[connfd].outbuf=string("null\r\n");
                    }

                }
                else{
                    clients[connfd].outbuf=string("usage:get key or get all\r\n");
                }

            }//get命令,分为获取get key和get all
            else if(clients[connfd].args[0]==string("del"))
            {
                string key=clients[connfd].args[1];
                if(!key.empty())
                {
                    int ret=htable->del(Hash(key), key);
                    memset(buf,sizeof(buf),0);
                    sprintf(buf,"Client %d del key:%s!" ,connfd,key.c_str());
                    Logger::logging(Logger::TRACE,__FILE__,__LINE__, buf);
                    if(ret==-1)
                    {
                        clients[connfd].outbuf=string("No such key\r\n");
                    }
                    else
                    {
                        clients[connfd].outbuf=string("ok\r\n");
                    }
                }
                else{
                    clients[connfd].outbuf=string("usage:del key\r\n");
                }

            }
            else{
                clients[connfd].outbuf=string("wrong command\r\n");
            }
            //因为读取客户端数据,服务器处理之后就要回复客户端,所以必须注册一个客户端写事件,这里直接用
            //c++11的lambda函数注册
            loop->addEvent(connfd, Ionet::IONET_WRITE, [this](int connfd)
            {
                this->FdWrite(connfd);
            });//每次解析命令之后,都要注册一个回复客户端的写事件
        }
    }
    void Ionet::FdAccept(int connfd)
    {
        int clientfd=accept(listenfd,NULL,NULL);
        setnoblocking(clientfd);
        clients[clientfd].fd=clientfd;
        Logger::logging(Logger::TRACE,__FILE__,__LINE__, "Accept a clientfd=%d",clientfd);
        if(clientfd==-1)
        {
            Logger::logging(Logger::ERROR,__FILE__,__LINE__, "Accept error!");
            exit(-1);
        }
        loop->addEvent(clientfd, Ionet::IONET_READ, [this](int connfd)
        {
            this->FdHandler(connfd);//客户端可读事件的回调函数
        });
    }
    Ionet::Ionet(){
        loop=new EventLoop;
        htable=new HashTable(4);
        clients=new Client[1024];
    }
    Ionet::~Ionet()
    {
        delete loop;
        delete htable;
        delete[] clients;
    }
    //服务器初始化
    void Ionet::netInit(short port)
    {
        int opt=1,n;
        int ret;
        char buf[5];
        sprintf(buf, "%d",port);
        string portStr(buf);
        struct addrinfo hints,*res,*ressave;
        bzero(&hints,sizeof(struct addrinfo));
        hints.ai_flags=AI_PASSIVE;
        hints.ai_family=AF_UNSPEC;
        hints.ai_socktype=SOCK_STREAM;
        if((n=getaddrinfo(0,portStr.c_str(), &hints,&res))!=0)
        {
            Logger::logging(Logger::ERROR,__FILE__,__LINE__, gai_strerror(n));
            exit(-1);
        }
        ressave=res;
        do
        {
            listenfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
            if(listenfd<0)
                continue;
            setnoblocking(listenfd);
            setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
            ret=bind(listenfd, res->ai_addr,res->ai_addrlen);
            if(ret==0)
            {
                break;
            }
            close(listenfd);
        } while ( (res=res->ai_next)!=NULL);
        ret=listen(listenfd,5);
        if(ret==-1)
        {
            Logger::logging(Logger::ERROR,__FILE__,__LINE__, strerror(errno));
            exit(-1);
        }
        //listenfd可读事件的回调函数,即accept接收客户端请求
        loop->addEvent(listenfd, Ionet::IONET_READ, [this](int listenfd)
        {
            this->FdAccept(listenfd);
        });
        freeaddrinfo(ressave);
        Logger::logging(Logger::TRACE,__FILE__,__LINE__, "mydb init successfully!");
    }

    //Ionet执行,进入事件监听循环
    void Ionet::run()
    {
        while(1)
        {
            loop->startLoop();
        }
    }
}
