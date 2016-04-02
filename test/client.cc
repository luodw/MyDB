#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>

using namespace std;

int main()
{
    struct sockaddr_in cliaddr;
    cliaddr.sin_family=AF_INET;
    cliaddr.sin_port=htons(9999);
    cliaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    int ret=connect(clientfd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
    if(ret==-1)
    {
        cout<<strerror(errno)<<endl;
        exit(-1);
    }
    cout<<"connect successfully!"<<endl;

}
