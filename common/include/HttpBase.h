//
// Created by wsoll on 1/9/2025.
//

#ifndef HTTPBASE_H
#define HTTPBASE_H

#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include "HttpHeaders.h"

namespace Http {
enum class Transport { HTTP, TLS };

constexpr std::string_view getTransportString(Transport transport) {
  switch (transport) {
  case Transport::HTTP:
    return "HTTP";
  case Transport::TLS:
    return "HTTPS";
  }
  return "Unknown Transport"; // Toss Exception?
}
enum class Protocol { HTTP1_0, HTTP1_1, HTTP2, HTTP3 };

// Map protocols to their string views
constexpr std::string_view getProtocolString(Protocol protocol) {
  switch (protocol) {
  case Protocol::HTTP1_0:
    return "HTTP/1.0";
  case Protocol::HTTP1_1:
    return "HTTP/1.1";
  case Protocol::HTTP2:
    return "HTTP/2";
  case Protocol::HTTP3:
    return "HTTP/3";
  }
  return "Unknown Protocol"; // Toss Exception?
}

enum class Method {
  GET,
  POST,
  PUT,
  DELETE_,
  HEAD,
  OPTIONS,
  PATCH,
  TRACE,
  CONNECT
};
constexpr std::string_view getMethodString(Method method) {
  switch (method) {
  case Method::GET:
    return "GET";
  case Method::POST:
    return "POST";
  case Method::PUT:
    return "PUT";
  case Method::DELETE_:
    return "DELETE";
  case Method::HEAD:
    return "HEAD";
  case Method::OPTIONS:
    return "OPTIONS";
  case Method::PATCH:
    return "PATCH";
  case Method::TRACE:
    return "TRACE";
  case Method::CONNECT:
    return "CONNECT";
  }
  return "Unknown Method"; // Exception?
}

constexpr std::string_view USER_AGENT{
    "https://github.com/wsollers/Http/README.md"};
class Request {
public:
  explicit Request(Method method, const std::string &url, int port = 443)
      : method(method), url(url), port(port) {}

  void printRequest() const {
    std::cout << "Requesting " << url << " port " << port
              << " with method " << getMethodString(method)
              << std::endl;
  }

  Request &addHeader(const std::string_view &key,
                         const std::string_view &value) {
    headers.insert({key, value});
    return *this;
  }
  Request &addHeader(Http::HeaderKeys key, const std::string_view &value) {
    return *this;
  }

  Request & removeHeader(const std::string_view &key) {
    headers.erase(key);
    return *this;
  }
  Request & removeHeader(Http::HeaderKeys key) {
    headers.erase(getHeaderKeyString(key));
    return *this;
  }
  Request &removeAllHeaders() {
    headers.clear();
    return *this;
  }

  size_t getHeaderCount() const { return headers.size(); }

  void printRequestHeaders() const {}

private:
  Http::Method method;
  std::string url;
  uint16_t port;

  Http::Headers headers;

  std::optional<std::string> body;
};
} // namespace Http
#endif // HTTPBASE_H
