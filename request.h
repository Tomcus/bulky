#ifndef BULKY_REQUEST_H
#define BULKY_REQUEST_H

#include <stdlib.h>

typedef struct {
    char* data;
    size_t len;
} Request;

Request bulky_make_request(const char* method, const char* path, const char* host);

void bulky_request_add_header(Request* request, const char* name, const char* value);
void bulky_set_body(Request* request, const char* body);

void bulky_free_request(Request* request);

#endif//BULKY_REQUEST_H
