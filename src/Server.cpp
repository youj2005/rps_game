#include <vector>
#include <thread>
#include "Server.h"
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <openssl/sha.h>
#include <fstream>

using std::cout;
using std::vector;
using std::endl;
using namespace server;
#define MAXLINE 1024

Server::Server(int port) : m_Port(port), m_AddrLen(sizeof(m_ServerAddress))
{
    m_ServerAddress.sin_family = AF_INET;
    m_ServerAddress.sin_addr.s_addr = INADDR_ANY;
    m_ServerAddress.sin_port = htons(m_Port);
    m_RPSD = new RPSD();
    if (!startServer())
    {
        cout << "Failed to start server" << endl;
    }
}

Server::~Server()
{
}

void Server::handleClient(int sockfd, struct sockaddr_in client_addr, char* buffer, int len) {
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client_addr.sin_port);

    std::cout << "Handling client: " << client_ip << ":" << client_port << "\n";
    std::cout << "Received: " << buffer << std::endl;

    unsigned char* hashBuffer = SHA256((unsigned char*) buffer, strlen(buffer), nullptr);
    char* ptr = buffer;
    uint8_t command = *(uint8_t*) ptr;
    ptr += sizeof(uint8_t) + sizeof(char);
    if (command == 0)
    {
        uint8_t difficulty = *(uint8_t*) ptr;
        ptr += sizeof(uint8_t) + sizeof(char); 

        const char* response;
        if (isDifficulty(hashBuffer, difficulty))
        {
            response = "ACK";
            move makeMove = *(move*) ptr;
            printf("%d\n", makeMove);
            move currMove = m_RPSD->play(makeMove);
            printf("%d\n", currMove);
        } else
        {
            response = "NACK";
        }
        sendto(sockfd, response, strlen(response), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
        
        std::cout << "Response sent to " << client_ip << ":" << client_port << "\n";
    }
    if (command == 1)
    {
        client_ip[INET_ADDRSTRLEN] = '\0';
        std::string ip(client_ip);
        if (!balances.count(ip))
        {
            balanceMtx.lock();
            balances[ip] = 1000;
            balanceMtx.unlock();
        }
        uint32_t amount = *(uint32_t*) ptr;
        ptr += sizeof(uint32_t) + sizeof(char);
        if (amount > balances[ip])
        {
            sendto(sockfd, "NACK", strlen("NACK"), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
        } else
        {
            balanceMtx.lock();
            balances[ip] -= amount;
            balanceMtx.unlock();
            betMtx.lock();
            bets[ip] = pair<move, uint16_t>(*(move*) ptr, amount);
            betMtx.unlock();
            sendto(sockfd, "ACK", strlen("ACK"), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
        }
    }
}

bool Server::startServer()
{
    m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_Socket < 0)
    {
        cout << "Failed to create socket" << endl;
        return false;
    }
    if (bind(m_Socket, (const struct sockaddr*) &m_ServerAddress, sizeof(m_ServerAddress)) < 0)
    {
        cout << "Failed to bind socket" << endl;
        close(m_Socket);
        return false;
    }

    vector<std::thread> threads;

    while (true)
    {
        cout << "Listening on " << m_Port << endl;
        char buffer[MAXLINE] = {0};
        struct sockaddr_in clientAddress;
        int n = recvfrom(m_Socket, (char*)buffer, MAXLINE, 0, (struct sockaddr*) &clientAddress, &m_AddrLen);
        if (n < 0)
        {
            cout << "Failed to receive" << endl;
            continue;
        }
        buffer[n] = '\0';
        cout << "Received: " << buffer << endl;

        threads.emplace_back(std::thread(&Server::handleClient, this, m_Socket, clientAddress, buffer, n));

        for (vector<std::thread>::iterator it = threads.begin(); it != threads.end(); ) {
            if (it->joinable()) {
                it->join();
                it = threads.erase(it);
            } else {
                ++it;
            }
        }
    }
    close(m_Socket);
    return true;
}