#ifndef HTTP_CLIENT_WIN64_H
#define HTTP_CLIENT_WIN64_H

// Windows-specific implementation of HTTP client using libtls
// This implementation uses the Windows Sockets API (Winsock) to create a TCP socket
// and then uses libtls to establish a secure connection over TLS.
#if defined(_WIN64) || defined(_WIN32)

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <string>

#include <tls.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "Ws2_32.lib")

inline void initializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(result));
    }
}

// Helper function to resolve hostname to IP
std::string resolveHostname(const std::string &hostname) {
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname.c_str(), nullptr, &hints, &res) != 0) {
        throw std::runtime_error("Failed to resolve hostname.");
    }

    char ip[INET_ADDRSTRLEN];
    auto *addr = (struct sockaddr_in *)res->ai_addr;
    inet_ntop(AF_INET, &addr->sin_addr, ip, sizeof(ip));

    freeaddrinfo(res);
    return std::string(ip);
}

inline void createTLSSocket(const std::string &hostname, const std::string &port) {
    struct tls_config *config = nullptr;
    struct tls *tls = nullptr;

    try {
        // Initialize Winsock
        initializeWinsock();

        // Initialize TLS library
        if (tls_init() < 0) {
            throw std::runtime_error("Failed to initialize TLS.");
        }

        // Create TLS configuration
        config = tls_config_new();
        if (!config) {
            throw std::runtime_error("Failed to create TLS configuration.");
        }

        // Set the CA file path explicitly for Windows
        const char *ca_path = "\\Program Files\\Common Files\\ssl\\cert.pem"; // Adjust path as needed
        if (tls_config_set_ca_file(config, ca_path) < 0) {
            throw std::runtime_error(std::string("Failed to set CA file: ") + tls_config_error(config));
        }

        // Enforce TLS 1.2+
        if (tls_config_set_protocols(config, TLS_PROTOCOL_TLSv1_2 | TLS_PROTOCOL_TLSv1_3) < 0) {
            throw std::runtime_error("Failed to set TLS protocols.");
        }

        // Create TLS client context
        tls = tls_client();
        if (!tls) {
            throw std::runtime_error("Failed to create TLS client context.");
        }

        // Configure TLS client
        if (tls_configure(tls, config) < 0) {
            throw std::runtime_error(std::string("TLS configuration error: ") + tls_error(tls));
        }

        // Resolve hostname
        std::string ip = resolveHostname(hostname);

        // Create socket and connect
        SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            throw std::runtime_error("Failed to create socket: " + std::to_string(WSAGetLastError()));
        }

        struct sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(std::stoi(port));
        if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
            closesocket(sock);
            throw std::runtime_error("Invalid address or address not supported.");
        }

        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
            closesocket(sock);
            throw std::runtime_error("Connection failed: " + std::to_string(WSAGetLastError()));
        }

        // Attach socket to TLS
        if (tls_connect_socket(tls, sock, hostname.c_str()) < 0) {
            closesocket(sock);
            throw std::runtime_error(std::string("TLS connection error: ") + tls_error(tls));
        }

        // Perform TLS handshake
        if (tls_handshake(tls) < 0) {
            closesocket(sock);
            throw std::runtime_error(std::string("TLS handshake failed: ") + tls_error(tls));
        }

        std::cout << "Connected to " << hostname << " over TLS successfully." << std::endl;

        // Clean up
        tls_close(tls);
        closesocket(sock);
        WSACleanup();

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        if (tls) tls_free(tls);
        if (config) tls_config_free(config);
        WSACleanup();
        return;
    }

    // Final cleanup
    if (tls) tls_free(tls);
    if (config) tls_config_free(config);
    WSACleanup();
}


#endif // defined(_WIN64) || defined(_WIN32)

#endif // HTTP_CLIENT_WIN64_H
