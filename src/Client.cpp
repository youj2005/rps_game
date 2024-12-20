#include "Client.h"
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

using std::cout;
using std::endl;

using namespace client;
#define MAXLINE 1024

Client::Client(string destination, int port) : m_DestinationAddress(destination), m_Port(port), m_AddrLen(sizeof(m_ServerAddress))
{
    memset(&m_ServerAddress, 0, sizeof(m_ServerAddress));
    m_ServerAddress.sin_family = AF_INET;
    m_ServerAddress.sin_addr.s_addr = INADDR_ANY;
    m_ServerAddress.sin_port = htons(m_Port);
    inet_pton(AF_INET, m_DestinationAddress.c_str(), &m_ServerAddress.sin_addr);
    if (!createSocket())
    {
        cout << "Failed to create socket" << endl;
    }
    cout << "Created" << endl;
}

Client::~Client()
{

}

bool Client::createSocket()
{
    m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_Socket < 0)
    {
        return false;
    }
    return true;
}

bool Client::send(move makeMove)
{
    cout << "Mining" << endl;
    const char* message = mine(1, 20, makeMove, 1);
    cout << "Sending" << message << endl;
    while (true)
    {
        sendto(m_Socket, message, strlen((char*)message), 0, (struct sockaddr*)&m_ServerAddress, m_AddrLen);
        char buffer[MAXLINE] = {0};

        int n = recvfrom(m_Socket, (char*)buffer, MAXLINE, 0, (struct sockaddr*) &m_ServerAddress, &m_AddrLen);
        if (n < 0)
        {
            cout << "Failed to receive" << endl;
            continue;
        }
        buffer[n] = '\0';
        cout << "Received: " << buffer << endl;
        if (!strcmp(buffer, "ACK"))
        {
            return true;
        }
        else
        {
            cout << "Failed to receive" << endl;
            continue;
        }
    }
}
