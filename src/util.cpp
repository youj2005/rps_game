#include "util.h"
#include <iostream>

size_t cb(char *data, size_t size, size_t nmemb, void *clientp)
{
  size_t realsize = size * nmemb;
  
  struct memory *mem = (struct memory *)clientp;

  char *ptr = (char*) realloc(mem->response, mem->size + realsize + 1);
  if(!ptr)
    return 0;  /* out of memory */
 
  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;
 
  return realsize;
}