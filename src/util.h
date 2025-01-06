#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct memory {
  char *response;
  size_t size;
};

size_t cb(char *data, size_t size, size_t nmemb, void *clientp);

#endif