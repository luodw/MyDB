#include "../util/parse.h"
#include <iostream>
#include <string>
#include <vector>
#include "../net/Ionet.h"
using namespace std;
using namespace ionet;
int main(void)
{
    Client client;
    cout<<client.args.size()<<endl;
    Parse parse("set name jason\r\n");
    parse.parseArgs(client);
    cout<<client.args[0]<<" "<<client.args[1]<<" "<<client.args[2];
    return 0;
}
