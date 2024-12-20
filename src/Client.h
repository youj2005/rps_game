#ifndef CLIENT_H
#define CLIENT_H
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include "hash.h"
#include <netinet/in.h>

using std::string;

namespace client
{
    class Client
    {
        public:
            Client(string address, int port);
            ~Client();
            bool send(move makeMove);
            void receive();
        private:
            string m_DestinationAddress;
            int m_Port;
            int m_Socket;
            struct sockaddr_in m_ServerAddress;
            socklen_t m_AddrLen;
            bool createSocket();
    };
}

#endif