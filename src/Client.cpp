#include "Client.h"
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <curl/curl.h>
#include "util.h"
#include <nlohmann/json.hpp>
#include <cstdlib>

using std::cout;
using std::endl;
using json = nlohmann::json;

using namespace client;
#define MAXLINE 1024

Client::Client(string destination, int port) : m_DestinationAddress(destination), m_Port(port), m_AddrLen(sizeof(m_ServerAddress))
{
    memset(&m_ServerAddress, 0, sizeof(m_ServerAddress));
    m_ServerAddress.sin_family = AF_INET;
    m_ServerAddress.sin_addr.s_addr = INADDR_ANY;
    m_ServerAddress.sin_port = htons(m_Port);
    apiKey = std::getenv("API_KEY");
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
        auto start = std::chrono::system_clock::now();
        auto startTime = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
        char buffer[MAXLINE] = {0};

        int n = recvfrom(m_Socket, (char*)buffer, MAXLINE, 0, (struct sockaddr*) &m_ServerAddress, &m_AddrLen);
        auto end = std::chrono::system_clock::now();
        auto endTime = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count();
        cout << (endTime - startTime) << endl;
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

string Client::payment_method(string number, string month, string year, string cvc, string city, string country, 
                        string line1, string line2, string postal_code, string state, string name)
{
    CURL* handle = curl_easy_init();
    memory res;
    res.response = nullptr;
    res.size = 0;

    string data;
    data.append("card[number]=");
    data.append(number);
    data.append("&card[exp_month]=");
    data.append(month);
    data.append("&card[exp_year]=");
    data.append(year);
    data.append("&card[cvc]=");
    data.append(cvc);
    /*
    data.append("&billing_details[address][city]=");
    data.append(city);
    data.append("&billing_details[address][country]=");
    data.append(country);
    data.append("&billing_details[address][line1]=");
    data.append(line1);
    data.append("&billing_details[address][line2]=");
    data.append(line2);
    data.append("&billing_details[address][postal_code]=");
    data.append(postal_code);
    data.append("&billing_details[address][state]=");
    data.append(state);
    data.append("&billing_details[name]=");
    data.append(name);
    */
    const char* url = "https://api.stripe.com/v1/tokens";

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, string("Authorization: Basic ").append(apiKey).c_str());
    headers = curl_slist_append(headers, "application/x-www-form-urlencoded");

    cout << data << endl;

    curl_easy_setopt(handle, CURLOPT_URL, url);
    // curl_easy_setopt(handle, CURLOPT_VERBOSE, 1);

    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &res);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);

    CURLcode res_code = curl_easy_perform(handle);
    
    if(res_code != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res_code));
        return nullptr;
    }

    json jsonResponse = json::parse(res.response);
    std::cout << jsonResponse.dump(4) << std::endl; // Pretty-print JSON

    return jsonResponse.contains("error") ? jsonResponse["error"]["message"] : jsonResponse["id"];
}