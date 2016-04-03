# mydb
用C++实现的一个简单NoSQL

这是修改上次的回显服务器之后,开发而成的NoSQL键值对数据库;这只是初级版本,之后还会不断完善这个数据库

# 主程序代码
使用方法很简单,定义一个ionet::Ionet类,这次改为调用getaddrinfo函数,所以只需要传入绑定的端口即可,如下
```
#include "../net/Ionet.h"
int main(void)
{
    ionet::Ionet mydb;
    mydb.netInit(9999);
    mydb.run();
    return 0;
}
```

# 以下是运行的截图:
* 服务器运行时,等待客户端的连接;等到用户连接之后,输出用户所做的操作;
![服务器端的日志输出](http://7xjnip.com1.z0.glb.clouddn.com/%E9%80%89%E5%8C%BA_072.png "")


* 客户端的输出,目前支持的命令有set key value,get key,get all,del key,quit后续会慢慢补上其他实用命令;
![第一个客户端的操作输出](http://7xjnip.com1.z0.glb.clouddn.com/%E9%80%89%E5%8C%BA_071.png "")

* 底层封装了epoll系统调用,支持多客户端访问,而且多个客户端对服务器的操作,并能正确处理
![第二个客户端的操作输出](http://7xjnip.com1.z0.glb.clouddn.com/%E9%80%89%E5%8C%BA_073.png "")

