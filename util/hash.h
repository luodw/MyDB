#ifndef _HASH_
#define _HASH_
#include<string>

namespace ionet
{
  long Hash(std::string str)
  {
     long long b     = 3;
     long long a     = 4;
     long hash = 0;
     for(size_t i = 0; i < str.length(); i++)
     {
        hash = hash * a + str[i];
        a    = a * b;
     }
     return hash;
  }
}



#endif
