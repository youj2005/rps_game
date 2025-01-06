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
            string payment_method(string number, string month, string year, string cvc, string city, string country, 
                        string line1, string line2, string postal_code, string state, string name);
            string payment_intent(string payment_method_id, string currency);
        private:
            string m_DestinationAddress;
            const char* apiKey;
            int m_Port;
            int m_Socket;
            struct sockaddr_in m_ServerAddress;
            socklen_t m_AddrLen;
            bool createSocket();
    };
}

#endif