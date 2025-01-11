//
// Created by wsoll on 1/9/2025.
//

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <iostream>
#include <vector>

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <tls.h>
#include <unistd.h>

// Helper function to resolve hostname to IP
inline std::string resolveHostname(const std::string &hostname) {
  struct addrinfo hints {
  }, *res;
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

void createTLSSocket(const std::string &hostname, const std::string &port) {
  struct tls_config *config = nullptr;
  struct tls *tls = nullptr;

  try {
    // Initialize TLS library
    if (tls_init() < 0) {
      throw std::runtime_error("Failed to initialize TLS.");
    }

    // Create TLS configuration
    config = tls_config_new();
    if (!config) {
      throw std::runtime_error("Failed to create TLS configuration.");
    }

    // Set the CA file path explicitly for Ubuntu
    const char *ca_path = "/etc/ssl/certs/ca-certificates.crt";
    if (tls_config_set_ca_file(config, ca_path) < 0) {
      throw std::runtime_error(std::string("Failed to set CA file: ") +
                               tls_config_error(config));
    }

    if (tls_config_set_protocols(config, TLS_PROTOCOL_TLSv1_2 |
                                             TLS_PROTOCOL_TLSv1_3) < 0) {
      throw std::runtime_error("Failed to set TLS protocols.");
    }
    /*
        // Set ECDHE curves, including X25519
        int rc = 0;
        if ((rc=tls_config_set_ecdhecurves(config,
       "X25519:prime256v1:secp384r1")) < 0) { std::cerr << "TLS configuration
       error: " << rc << " XXXX " << tls_error(tls) << std::endl; throw
       std::runtime_error("Failed to set ECDHE curves: " +
                                   std::string(tls_config_error(confg)));
        }
    // Specify cipher suites explicitly
    if (tls_config_set_ciphers(config, "TLS_AES_128_GCM_SHA256") < 0) {
      throw std::runtime_error("Failed to set cipher suites: " +
                               std::string(tls_config_error(config)));
    }
    */

    // Create TLS client context
    tls = tls_client();
    if (!tls) {
      throw std::runtime_error("Failed to create TLS client context.");
    }

    std::cout << "TLS client context created successfully." << std::endl;
    // Configure TLS client
    if (tls_configure(tls, config) < 0) {
      throw std::runtime_error(std::string("TLS configuration error: ") +
                               tls_error(tls));
    }

    // Resolve hostname
    std::string ip = resolveHostname(hostname);

    // Create socket and connect
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
      throw std::runtime_error("Failed to create socket.");
    }

    struct sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(std::stoi(port));
    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
      throw std::runtime_error("Invalid address or address not supported.");
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
        0) {
      close(sock);
      std::cerr << "Connection failed." << std::endl;
      throw std::runtime_error("Connection failed.");
    }

    std::cout << "Connected to " << hostname << " successfully." << std::endl;
    // Attach socket to TLS
    int oc = 0;
    if ((oc=tls_connect_socket(tls, sock, hostname.c_str())) < 0) {
      std::cerr << "TLS connection error: " << oc << std::endl;
      std::cerr << "TLS connection error: " << tls_error(tls) << std::endl;
      close(sock);
      throw std::runtime_error(std::string("TLS connection error: ") +
                               tls_error(tls));
    }

    std::cout << "Attached socket to TLS successfully." << std::endl;
    // Perform TLS handshake
    if (tls_handshake(tls) < 0) {
      close(sock);
      std::cerr << "TLS handshake failed: " << std::endl;
      std::cerr << "TLS handshake failed: " << tls_error(tls) << std::endl;
      throw std::runtime_error(std::string("TLS handshake failed: ") +
                               tls_error(tls));
    }

    std::cout << "Connected to " << hostname << " over TLS successfully."
              << std::endl;

    // Clean up
    tls_close(tls);
    close(sock);

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  if (tls)
    tls_free(tls);
  if (config)
    tls_config_free(config);
}
// Function to test and print supported ECDHE curves
inline void printSupportedCurves() {
  // List of common elliptic curves to test
  std::vector<std::string> curveList = {"X25519", "prime256v1", "secp384r1",
                                        "secp521r1"};

  // Initialize TLS
  if (tls_init() < 0) {
    std::cerr << "Failed to initialize TLS." << std::endl;
    return;
  }

  // Create a TLS configuration object
  struct tls_config *config = tls_config_new();
  if (!config) {
    std::cerr << "Failed to create TLS configuration." << std::endl;
    return;
  }

  std::cout << "Testing supported ECDHE curves..." << std::endl;

  // Test each curve
  for (const auto &curve : curveList) {
    if (tls_config_set_ecdhecurves(config, curve.c_str()) == 0) {
      std::cout << "Supported curve: " << curve << std::endl;
    } else {
      std::cout << "Unsupported curve: " << curve << std::endl;
    }
  }

  // Free TLS configuration
  tls_config_free(config);
}
#endif // HTTP_CLIENT_H
