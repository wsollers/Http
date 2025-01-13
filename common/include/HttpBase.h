//
// Created by wsoll on 1/9/2025.
//

#ifndef HTTPBASE_H
#define HTTPBASE_H

#include <iostream>
#include <string_view>
#include <string>
#include <cstdint>

#include "HttpHeaders.h"

namespace Http {
    enum class Transport { HTTP, TLS };

    constexpr std::string_view getTransportString(Transport transport) {
        switch (transport) {
            case Transport::HTTP: return "HTTP";
            case Transport::TLS: return "HTTPS";
        }
        return "Unknown Transport"; //Toss Exception?
    }
    enum class Protocol { HTTP1_0, HTTP1_1, HTTP2, HTTP3 };

    // Map protocols to their string views
    constexpr std::string_view getProtocolString(Protocol protocol) {
        switch (protocol) {
            case Protocol::HTTP1_0: return "HTTP/1.0";
            case Protocol::HTTP1_1: return "HTTP/1.1";
            case Protocol::HTTP2: return "HTTP/2";
            case Protocol::HTTP3: return "HTTP/3";
        }
        return "Unknown Protocol"; //Toss Exception?
    }

    enum class Method { GET, POST, PUT, DELETE_, HEAD, OPTIONS, PATCH, TRACE, CONNECT };
    constexpr std::string_view getMethodString(Method method) {
        switch (method) {
            case Method::GET: return "GET";
            case Method::POST: return "POST";
            case Method::PUT: return "PUT";
            case Method::DELETE_: return "DELETE";
            case Method::HEAD: return "HEAD";
            case Method::OPTIONS: return "OPTIONS";
            case Method::PATCH: return "PATCH";
            case Method::TRACE: return "TRACE";
            case Method::CONNECT: return "CONNECT";
        }
        return "Unknown Method"; //Exception?
    }

    constexpr std::string_view USER_AGENT { "https://github.com/wsollers/Http/README.md"};
    // Template class for HttpRequest
    template
    <Transport TransportType, Protocol ProtocolType, Method MethodType>
    class Request {
    public:
        using RequestType = Request<TransportType, ProtocolType, MethodType>;

        explicit Request(const std::string &url, int port = 443) : url(url), port(port) {}

        void printRequest() const {
            std::cout << "Requesting " << url << " using protocol " << getProtocolString(ProtocolType) <<
                " " << port << " with method " << getMethodString(MethodType) <<
                " with transport: " << getTransportString(TransportType)<< std::endl;
        }

        RequestType& addHeader(const std::string &key, const std::string &value) { return *this; }
        RequestType& removeHeader(const std::string &key) {return *this;}
        RequestType& removeAllHeaders() {return *this;}

        void printRequestHeaders() const {}

    private:
        std::string url;
        uint16_t port;

        Http::Headers headers;
    };
} //namespace http
#endif //HTTPBASE_H
