#include "../net/Ionet.h"
int main(void)
{
    ionet::Ionet mydb;
    mydb.netInit(9999);
    mydb.run();
    return 0;
}
