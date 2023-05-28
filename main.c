#include <stdio.h>
#include <uv.h>
#include <stdatomic.h>

#include "request.h"
#include "response.h"
#define SV_IMPLEMENTATION
#include "sv.h"

#define PORT 80
#define IP "93.184.216.34"
#define NUMBER_OF_SOCKETS 100

uv_loop_t* loop = NULL;
atomic_int successes;
atomic_int errors;

void allocate(uv_handle_t* handle, size_t size, uv_buf_t* buf) {
    (void)handle;
    *buf = uv_buf_init(malloc(size), (unsigned int)size);
}

void on_write(uv_write_t* write_req, int status) {
    free(write_req);
    if (status < 0) {
        fprintf(stderr, "ERROR: %s", uv_strerror(status));
    }
}

void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buffer) {
    if (nread < 0) {
        fprintf(stderr, "ERROR: %s", uv_strerror((int)nread));
        return;
    }

    ParsedResponse resp = bulky_parse_response(buffer->base, buffer->len);

    if (resp.code == 200) {
        ++successes;
    } else {
        ++errors;
        fprintf(stderr, "Response error: %d, "SV_Fmt"\n", resp.code, SV_Arg(resp.reason));
    }
    
    uv_close((uv_handle_t *)stream, NULL);
    free(buffer->base);
}

void make_request(uv_stream_t* stream) {
    Request req = bulky_make_request("GET", "/", "example.com");
    bulky_request_add_header(&req, "X-Test", "1");
    bulky_set_body(&req, ""); // finishes building the request

    uv_buf_t buffer;
    buffer.base = req.data;
    buffer.len = req.len;

    uv_write_t* write_req = malloc(sizeof(uv_write_t));

    uv_write(write_req, stream, &buffer, 1, on_write);

    bulky_free_request(&req);

    uv_read_start(stream, allocate, on_read);
}

void on_connect(uv_connect_t* connection, int status) {
    if (status < 0) {
        fprintf(stderr, "ERROR: %s", uv_strerror(status));
        return;
    }

    uv_stream_t* stream = connection->handle;
    free(connection);
    make_request(stream);
}

void run_uv_loop() {
    loop = malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);
    uv_tcp_t* sockets = malloc(NUMBER_OF_SOCKETS*sizeof(uv_tcp_t));

    struct sockaddr_in dest;
    uv_ip4_addr(IP, PORT, &dest);

    for (size_t i = 0; i < NUMBER_OF_SOCKETS; ++i) {
        uv_tcp_init(loop, &sockets[i]);
        uv_tcp_keepalive(&sockets[i], 1, 60);

        uv_connect_t* conn = malloc(sizeof(uv_connect_t));
        uv_tcp_connect(conn, &sockets[i], (const struct sockaddr*)&dest, on_connect);
    }
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    free(sockets);
    free(loop);
}

int main() {
    run_uv_loop();
    printf("# oks: %d\n# errors: %d\n", successes, errors);
    return 0;
}
