//
// Created by wsoll on 1/9/2025.
//
#include <cstdlib>
#include <cstdio>

#include "http-client.h"
#include "http.h"


int main (int argc, char *argv[]) {

    struct Hello hello;
    printf("Client started %s\n", hello.value);

    Http::Request<Http::Transport::HTTP, Http::Protocol::HTTP1_1, Http::Method::GET> request1("http://example.com", 80);
    request1.printRequest();

    Http::Request<Http::Transport::TLS, Http::Protocol::HTTP2, Http::Method::POST> request2("http://example.org", 443);
    request2.printRequest();

    testOpenTlsSocket();

    exit(EXIT_SUCCESS);

}
