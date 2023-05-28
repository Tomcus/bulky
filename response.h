#ifndef BULKY_RESPONSE_H
#define BULKY_RESPONSE_H

#include <stdlib.h>
#include "sv.h"

typedef struct {
    String_View http_version;
    String_View reason;
    String_View headers;
    String_View body;
    uint16_t code;
} ParsedResponse;

typedef struct {
    String_View name;
    String_View value;
} Header;

typedef struct {
    Header* data;
    size_t count;
    size_t capacity;
} Headers;

ParsedResponse bulky_parse_response(char * data, size_t len);

Headers bulky_init_headers();
void bulky_free_headers(Headers* headers);
void bulky_headers_add_header(Headers* headers, Header header);
Header bulky_parse_header(String_View* src);
Headers bulky_parse_headers(String_View* src);
Header* bulky_find_header(Headers* headers, const char* name);

#endif//BULKY_RESPONSE_H
