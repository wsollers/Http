//
// Created by wsoll on 1/9/2025.
//

#ifndef HTTPBASE_H
#define HTTPBASE_H

#include <iostream>
#include <array>
#include <string_view>
#include <string>
#include <cstdint>
#include <unordered_map>

enum class Protocol { HTTP1_0, HTTP1_1, HTTP2, HTTP3 };

// Map protocols to their string views
/*
const std::unordered_map<Protocol, std::string_view> ProtocolStrings{
    {Protocol::HTTP1_0, "HTTP/1.0"},
    {Protocol::HTTP1_1, "HTTP/1.1"},
    {Protocol::HTTP2, "HTTP/2"},
    {Protocol::HTTP3, "HTTP/3"}
};
*/

constexpr std::string_view getProtocolString(Protocol protocol) {
    switch (protocol) {
        case Protocol::HTTP1_0: return "HTTP/1.0";
        case Protocol::HTTP1_1: return "HTTP/1.1";
        case Protocol::HTTP2: return "HTTP/2";
        case Protocol::HTTP3: return "HTTP/3";
    }
    return "Unknown Protocol"; //Toss Exception?
}

enum class Method { GET, POST, PUT, DELETE, HEAD, OPTIONS, PATCH, TRACE, CONNECT };
constexpr std::string_view getMethodString(Method method) {
    switch (method) {
        case Method::GET: return "GET";
        case Method::POST: return "POST";
        case Method::PUT: return "PUT";
        case Method::DELETE: return "DELETE";
        case Method::HEAD: return "HEAD";
        case Method::OPTIONS: return "OPTIONS";
        case Method::PATCH: return "PATCH";
        case Method::TRACE: return "TRACE";
        case Method::CONNECT: return "CONNECT";
    }
    return "Unknown Method"; //Exception?
}


// Template class for HttpRequest
template
<Protocol ProtocolType, Method MethodType>
class HttpRequest {
public:
    explicit HttpRequest(const std::string &url, int port = 443) : url(url), port(port) {}

    void printRequest() const {
        std::cout << "Requesting " << url << " using protocol " << getProtocolString(ProtocolType) <<
            " " << port << " with method " << getMethodString(MethodType) << std::endl;
    }

private:
    std::string url;
    uint16_t port;
};

#endif //HTTPBASE_H
