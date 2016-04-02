#include "../net/Epoller.h"
#include <iostream>

using namespace std;

int main(void)
{
    ionet::Epoller epoller(NULL);
    epoller.addEvent(4,2);
    return 0;
}
