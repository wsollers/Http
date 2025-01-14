//
// Created by wsoll on 1/9/2025.
//

#ifdef __linux__
    #pragma message("__linux__ is defined")
#else
    #pragma message("__linux__ is not defined")
#endif

#include <cstdlib>
#include <cstdio>

#include "http-client.h"
#include "http.h"


int main (int argc, char *argv[]) {

    struct Hello hello;
    printf("Client started %s\n", hello.value);

    //printSupportedCurves();
    createTLSSocket("cnn.com", "443");


    Http::Client client(Http::Transport::TLS, Http::Protocol::HTTP1_0);


    exit(EXIT_SUCCESS);

}
