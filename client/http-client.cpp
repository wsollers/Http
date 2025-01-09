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

    HttpRequest<Protocol::HTTP1_1> request1("http://example.com");
    request1.printRequest();

    HttpRequest<Protocol::HTTP2> request2("http://example.org");
    request2.printRequest();


    exit(EXIT_SUCCESS);



}
