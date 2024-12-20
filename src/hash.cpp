#include "hash.h"
#include <iostream>
#include <random>
#include <openssl/sha.h>
#include <bitset>
#include <climits>

char* mine(uint8_t version, uint8_t difficulty, move playVal, uint8_t id)
{
    uint32_t nonce = 0;
    char* nonceStr = uint32ToBase64(nonce, nullptr);
    size_t size = sizeof(uint8_t) * 3 + sizeof(move) + 12 + 5 * sizeof(char);
    char* buffer = new char[size];
    srand(time(0));
    uint32_t randomVal = rand();
    char* randomStr = uint32ToBase64(randomVal, nullptr);
    char* end = headerStr(buffer, &version, &difficulty, &playVal, &id, randomStr);
    char* ptr = end;
    const uint8_t zero = 0;
    memcpy(ptr, nonceStr, 6);
    ptr += 6;
    memcpy(ptr, &zero, 1);
    unsigned char* hashBuffer = SHA256((unsigned char*) buffer, strlen(buffer), nullptr);
    while (!isDifficulty(hashBuffer, difficulty)) {
        nonce++;
        nonceStr = uint32ToBase64(nonce, nonceStr);
        ptr = end;
        memcpy(ptr, nonceStr, 6);
        ptr += 6;
        memcpy(ptr, &zero, 1);
        hashBuffer = SHA256((unsigned char*) buffer, strlen((char*)buffer), nullptr);
    }
    for (size_t i=0; i<32; ++i)
    {
        std::cout << std::bitset<8>(hashBuffer[i]);
    }
    std::cout << std::endl;
    return buffer;
}

char* headerStr(char* outputBuffer, uint8_t* version, uint8_t* difficulty, move* playVal, uint8_t* id, char* random)
{
    char* ptr = outputBuffer;
    memcpy(ptr, version, sizeof(uint8_t));
    ptr += sizeof(uint8_t);
    memcpy(ptr, ":", sizeof(char));
    ptr += sizeof(char);
    memcpy(ptr, difficulty, sizeof(uint8_t));
    ptr += sizeof(uint8_t);
    memcpy(ptr, ":", sizeof(char));
    ptr += sizeof(char);
    memcpy(ptr, playVal, sizeof(move));
    ptr += sizeof(move);
    memcpy(ptr, ":", sizeof(char));
    ptr += sizeof(char);
    memcpy(ptr, id, sizeof(uint8_t));
    ptr += sizeof(uint8_t);
    memcpy(ptr, ":", sizeof(char));
    ptr += sizeof(char);
    memcpy(ptr, random, strlen(random));
    ptr += strlen(random);
    memcpy(ptr, ":", sizeof(char));
    ptr += sizeof(char);
    return ptr;
}

bool isDifficulty(const unsigned char* str, uint8_t difficulty)
{
    for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        char temp = str[i];
        for (size_t j = 0; j < CHAR_BIT; ++j)
        {
            if (!(temp & (1 << (CHAR_BIT - j - 1))))
            {
                --difficulty;
                if (!difficulty)
                {
                    return true;
                }
            } else
            {
                return false;
            }
        }
    }
    return true;
}