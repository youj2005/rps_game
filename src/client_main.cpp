#include "Client.h"
#include "RPSD.h"

using namespace client;

int main()
{
    Client* cl = new Client("127.0.0.1", 8080);
    cl->payment_method("4242424242424242", "12", "2023", "123", "SanFrancisco", "US", "123MarketSt", "Apt2", "94107", "CA", "JennyRosen");
    cl->send(PAPER);
}