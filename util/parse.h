#ifndef _PARSE_
#define _PARSE_

#include "Nocopyable.h"
#include <string>

namespace ionet
{
    struct Client;
    class Parse:public nocopyable
    {
    public:
        Parse()=default;
        Parse(const char* query);
        ~Parse();
        void parseArgs(Client&);
        // std::string getCommand();
        // std::string getKey();
        // std::string getValue();
    private:
        char *query;
    };
}


#endif
