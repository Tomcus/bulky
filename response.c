#include "response.h"

bool is_space(char c) {
    return (bool)isspace(c); 
}

ParsedResponse bulky_parse_response(char* data, size_t len) {
    String_View src = sv_from_parts(data, len);
    ParsedResponse response;
    String_View delimiter = sv_from_parts("\r\n", 2);
    response.http_version = sv_chop_by_delim(&src, ' ');
    response.code = (uint16_t)sv_chop_u64(&src);
    sv_chop_left_while(&src, is_space);
    response.reason = sv_chop_by_sv(&src, delimiter);

    delimiter = sv_from_parts("\r\n\r\n", 4);
    response.headers = sv_chop_by_sv(&src, delimiter);

    response.body = src;

    return response;
}

Headers bulky_init_headers() {
    Headers res;
    res.count = 0;
    res.capacity = 20;
    res.data = (Header *) malloc(res.capacity*sizeof(Header));

    return res;
}

void bulky_free_headers(Headers* headers) {
    free(headers->data);
    headers->data = NULL;
    headers->count = 0;
}

Header bulky_parse_header(String_View* src) {
    Header res;
    res.name = sv_chop_by_delim(src, ':');
    sv_chop_left_while(src, is_space);
    String_View delim = sv_from_parts("\r\n", 2);
    res.value = sv_chop_by_sv(src, delim);
    return res;
}

void bulky_headers_add_header(Headers* headers, Header header) {
    if (headers->count == headers->capacity) {
        headers->capacity *= 2;
        headers->data = realloc(headers->data, headers->capacity * sizeof(Header));
    }
    headers->data[headers->count++] = header;
}

Headers bulky_parse_headers(String_View* src) {
    Headers headers = bulky_init_headers();
    sv_chop_left_while(src, is_space);
    while(src->count > 0) {
        Header header = bulky_parse_header(src);
        bulky_headers_add_header(&headers, header);
        sv_chop_left_while(src, is_space);
    }
    return headers;
}

Header* bulky_find_header(Headers* headers, const char* name) {
    String_View nameSv = sv_from_cstr(name);
    for (size_t i = 0; i < headers->count; ++i) {
        if (sv_eq_ignorecase(headers->data[i].name, nameSv)) {
            return &headers->data[i];
        }
    }
    return NULL;
}
