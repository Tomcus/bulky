#include "request.h"

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 2048
#define BUFFER char buffer[2048]

Request bulky_make_request(const char* method, const char* path, const char* host) {
    BUFFER;
    size_t count = (size_t)snprintf(buffer, BUFFER_SIZE, "%s %s\r\nHost: %s\r\n", method, path, host);
    Request res;
    res.data = (char *)malloc(count);
    res.len = count;
    memcpy(res.data, buffer, count);
    return res;
}

void bulky_request_add_header(Request* request, const char* name, const char* value) {
    BUFFER;
    size_t count = (size_t)snprintf(buffer, BUFFER_SIZE, "%s: %s\r\n", name, value);
    request->data = (char *) realloc(request->data, request->len + count);
    memcpy(request->data + request->len, buffer, count);
    request->len += count;
}

void bulky_set_body(Request* request, const char* body) {
    size_t count = strlen(body);
    request->data = (char *) realloc(request->data, request->len + count + 2);
    memcpy(request->data + request->len, "\r\n", 2);
    memcpy(request->data + request->len + 2, body, count);
    request->len += count + 2;
}

void bulky_free_request(Request* request) {
    free(request->data);
    request->data = NULL;
    request->len = 0;
}
