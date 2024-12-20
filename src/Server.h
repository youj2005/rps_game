#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "hash.h"

using std::string;
using rpsd::RPSD;

namespace server
{
    class Server
    {
        public:
            Server(int port);
            ~Server();
        private:
            int m_Port;
            int m_Socket;
            RPSD* m_RPSD;
            struct sockaddr_in m_ServerAddress;
            socklen_t m_AddrLen;
            bool startServer();
            void handle_client(int sockfd, struct sockaddr_in client_addr, char* buffer, int len);
            void parseHeader(char* buffer);
    };
}

#endif