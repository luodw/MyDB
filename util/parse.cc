#include "../net/Ionet.h"
#include "parse.h"
#include <string>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

namespace ionet
{
    Parse::Parse(const char* q)
    {
        query=(char*)malloc(strlen(q)+1);
        strcpy(query,q);
    }
    Parse::~Parse()
    {
        free(query);
    }
    void Parse::parseArgs(Client &client)
    {
        int i=0;
        char *q=query;
        const char* delim=" ";
        char *p=strtok(q,delim);
        client.args[i++]=string(p);
        //strtok函数第二次调用时,必须传递NULL值,否则会无限迭代
        while((p=strtok(NULL,delim))!=NULL)
        {
            client.args[i++]=string(p);
        }
        i--;
        size_t pos=client.args[i].find(string("\r\n"));
        client.args[i].erase(pos);
    }
}
//     string Parse::getCommand()
//     {
//         char *p=query;
//         char *q1=p,*q2;
//         int len=0;
//         q2=strchr(q1,' ');
//         while(q1!=q2)
//         {
//             q1++;
//             len++;
//         }
//         char *result=(char*)malloc(len+1);
//         strncpy(result,p,len);
//         result[len]='\0';
//         string tmpStr(result);
//         free(result);
//         return tmpStr;
//     }
//     string Parse::getKey()
//     {
//         char *p=query;
//         char *q,*q1;
//         int len=0;
//         q1=strchr(p,' ');
//         q=++q1;
//         string cmd=getCommand();
//         if(cmd==string("set"))
//         {
//             while(*q!=' ')
//             {
//                 q++;
//                 len++;
//             }
//         }
//         if(cmd==string("get") || cmd==string("del") )
//         {
//             while(*q!='\r')
//             {
//                 q++;
//                 len++;
//             }
//         }
//
//         char *result=(char*)malloc(len+1);
//         strncpy(result,q1,len);
//         result[len]='\0';
//         string tmpStr(result);
//         free(result);
//         return tmpStr;
//     }
//     string Parse::getValue()
//     {
//         char *p=query;
//         char *q,*q1,*q2;
//         int len=0;
//         q1=strchr(p,' ');
//         q2=strchr(++q1,' ');
//         q=++q2;
//         while(*q2!='\r')
//         {
//             q2++;
//             len++;
//         }
//         char *result=(char*)malloc(len+1);
//         strncpy(result,q,len);
//         result[len]='\0';
//         string tmpStr(result);
//         free(result);
//         return tmpStr;
//     }
// }
