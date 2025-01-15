//
// Created by wsoll on 1/9/2025.
//

#ifndef HTTP_H
#define HTTP_H

#include "include/ContentTypes.h"
#include "include/HttpBase.h"


struct Hello {
  const char *value = "HELLO WORLD\n";
};

namespace Http {

template <Http::Transport TransportType, Http::Protocol ProtocolType>
struct Sender {
  void sendRequest(const Http::Request &request) {
    std::cout << "Sending request to ";
    request.printRequest();
    std::cout << std::endl;
  }
};

template <>
struct Sender<Http::Transport::TLS, Http::Protocol::HTTP1_1> {
  void sendRequest(const Http::Request &request) {
    //Encrypted HTTPS request
    std::cout << "Sending request to ";
    request.printRequest();
    std::cout << " over TLS" << std::endl;
  }
};

template <>
struct Sender<Http::Transport::HTTP, Http::Protocol::HTTP1_1> {
  void sendRequest(const Http::Request &request) {
    //Unencrypted HTTP request
    std::cout << "Sending request to ";
    request.printRequest();
    std::cout << " over HTTP" << std::endl;
  }
};

class Client {
public:
  Client(const Http::Transport transport, const Http::Protocol protocol)
      : transport(transport), protocol(protocol){};

  virtual ~Client() = default;

  Http::Response sendRequest(const std::string &url, const Http::Method method,
                   const Http::Headers &headers, const std::string &body);
  Http::Response sendRequest(const Http::Request &request);

private:
  Http::Transport transport;
  Http::Protocol protocol;
};

} // namespace Http

#endif // HTTP_H
