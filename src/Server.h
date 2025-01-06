#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "hash.h"
#include <unordered_map>
#include <utility>

using std::string;
using rpsd::RPSD;
using std::unordered_map;
using std::pair;

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
            static std::mutex balanceMtx;
            static std::mutex betMtx;
            static unordered_map<string, uint16_t> balances;
            static unordered_map<string, pair<move, uint16_t>> bets;
            bool startServer();
            void handleClient(int sockfd, struct sockaddr_in client_addr, char* buffer, int len);
    };
}

#endif