#include "Client.h"
#include "RPSD.h"

using namespace client;

int main()
{
    Client* cl = new Client("127.0.0.1", 8080);
    cl->send(PAPER);
}