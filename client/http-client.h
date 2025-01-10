//
// Created by wsoll on 1/9/2025.
//

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <iostream>

//inline void testOpenTlsSocket(){}

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

inline void testOpenTlsSocket() {
    // Initialize OpenSSL
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* sslCtx = SSL_CTX_new(method);

    if (!sslCtx) {
        std::cerr << "Unable to create SSL context" << std::endl;
        ERR_print_errors_fp(stderr);
        return;
    }

    // Set up the SSL connection
    SSL* ssl = SSL_new(sslCtx);
    BIO* bio = BIO_new_ssl_connect(sslCtx);

    SSL_set_connect_state(ssl);
    BIO_set_conn_hostname(bio, "cnn.com:443");

    if (BIO_do_connect(bio) <= 0) {
        std::cerr << "Failed to connect to cnn.com" << std::endl;
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        SSL_CTX_free(sslCtx);
        return;
    }

    if (BIO_do_handshake(bio) <= 0) {
        std::cerr << "Failed to establish SSL/TLS handshake" << std::endl;
        ERR_print_errors_fp(stderr);
        BIO_free_all(bio);
        SSL_CTX_free(sslCtx);
        return;
    }

    std::cout << "TLS connection to cnn.com established successfully!" << std::endl;

    // Clean up
    BIO_free_all(bio);
    SSL_CTX_free(sslCtx);
}


#endif //HTTP_CLIENT_H
