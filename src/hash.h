#ifndef hash_H
#define hash_H

#include <string>
#include <cstring>
#include <ctime>
#include <stdint.h>
#include <iostream>
#include "RPSD.h"

using std::string;

const static char* base64Chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* mine(uint8_t version, uint8_t difficulty, move playVal, uint8_t id);

char* headerStr(char* outputBuffer, uint8_t* version, uint8_t* difficulty, move* playVal, uint8_t* id, char* random);

static char* uint32ToBase64(uint32_t value, char* ptr) {
    if (!ptr)
    {
        ptr = new char[7];
    }
    for (int i = 5; i >= 0; --i)
    {
        ptr[i] = base64Chars[value % 64];
        value /= 64;
    }
    ptr[6] = '\0';
    return ptr;
}

bool isDifficulty(const unsigned char* buffer, uint8_t difficulty);

#endif