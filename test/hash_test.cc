#include "../util/hash.h"
#include <iostream>
using namespace std;

int main(void)
{
    int N=10;
    cout<<ionet::Hash("key")%N<<endl;
    cout<<ionet::Hash("key")%N<<endl;
    cout<<ionet::Hash("apple")%N<<endl;
    return 0;
}
