// http_protocol_test.cpp
#include <gtest/gtest.h>
#include "http.h"



// Test case for HttpProtocol enum class
TEST(HttpTestSuite, MatchesExpectedValues) {
    EXPECT_EQ(Http::getProtocolString(Http::Protocol::HTTP1_0), "HTTP/1.0");
    EXPECT_EQ(Http::getProtocolString(Http::Protocol::HTTP1_1), "HTTP/1.1");
    EXPECT_EQ(Http::getProtocolString(Http::Protocol::HTTP2), "HTTP/2");
    EXPECT_EQ(Http::getProtocolString(Http::Protocol::HTTP3), "HTTP/3");
}

// Test case for Transport enum class
TEST(HttpTestSuite, MatchesExpectedValues) {
    EXPECT_EQ(Http::getTransportString(Http::Transport::TLS), "HTTPS");
    EXPECT_EQ(Http::getTransportString(Http::Transport::HTTP), "HTTP");

}

// Unit Test for getMethodString
TEST(HttpTestSuite, ReturnsExpectedStrings) {
    // Test all enum values
    EXPECT_EQ(Http::getMethodString(Http::Method::GET), "GET");
    EXPECT_EQ(Http::getMethodString(Http::Method::POST), "POST");
    EXPECT_EQ(Http::getMethodString(Http::Method::PUT), "PUT");
    EXPECT_EQ(Http::getMethodString(Http::Method::DELETE_), "DELETE");
    EXPECT_EQ(Http::getMethodString(Http::Method::HEAD), "HEAD");
    EXPECT_EQ(Http::getMethodString(Http::Method::OPTIONS), "OPTIONS");
    EXPECT_EQ(Http::getMethodString(Http::Method::PATCH), "PATCH");
    EXPECT_EQ(Http::getMethodString(Http::Method::TRACE), "TRACE");
    EXPECT_EQ(Http::getMethodString(Http::Method::CONNECT), "CONNECT");
}

// Unit Test for getHeaderKeyString
TEST(HttpTestSuite, ReturnsCorrectString) {
    using namespace Http;

    // Test known headers
    EXPECT_EQ(getHeaderKeyString(HeaderKeys::Accept), "Accept");
    EXPECT_EQ(getHeaderKeyString(HeaderKeys::ContentType), "Content-Type");
    EXPECT_EQ(getHeaderKeyString(HeaderKeys::Authorization), "Authorization");
    EXPECT_EQ(getHeaderKeyString(HeaderKeys::UserAgent), "User-Agent");
    EXPECT_EQ(getHeaderKeyString(HeaderKeys::Host), "Host");

    // Test Unknown header
    EXPECT_EQ(getHeaderKeyString(HeaderKeys::Unknown), "Unknown");
}

// Additional Test to Ensure No Unexpected Results
TEST(HttpTestSuite, HandlesAllKnownHeaders) {
    using namespace Http;

    EXPECT_EQ(getHeaderKeyString(HeaderKeys::AcceptEncoding), "Accept-Encoding");
    EXPECT_EQ(getHeaderKeyString(HeaderKeys::ContentLength), "Content-Length");
    EXPECT_EQ(getHeaderKeyString(HeaderKeys::Referer), "Referer");
    EXPECT_EQ(getHeaderKeyString(HeaderKeys::CacheControl), "Cache-Control");
    EXPECT_EQ(getHeaderKeyString(HeaderKeys::Connection), "Connection");
}

TEST(HttpTestSuite, InitialRequestHasZeroHeaders) {
    using namespace Http;
    Http::Request<Http::Transport::HTTP, Http::Protocol::HTTP1_1, Http::Method::GET> request("http://example.com", 80);
    EXPECT_EQ(request.getHeaderCount(), size_t(0));
}


// Http::Request<Http::Transport::HTTP, Http::Protocol::HTTP1_1, Http::Method::GET> request1("http://example.com", 80);
TEST(HttpTestSuite, CanAddHeaders) {
    using namespace Http;
    Http::Request<Http::Transport::HTTP, Http::Protocol::HTTP1_1, Http::Method::GET> request("http://example.com", 80);

    EXPECT_EQ(request.addHeader(HeaderKeys::Accept, "GGG"), request);
    EXPECT_EQ(request.addHeader(getHeaderKeyString(HeaderKeys::Accept), "GGG"), request);


    EXPECT_EQ(getHeaderKeyString(HeaderKeys::AcceptEncoding), "Accept-Encoding");

}

TEST(HttpTestSuite, CanRemoveHeaders) {
    using namespace Http;
    Http::Request<Http::Transport::HTTP, Http::Protocol::HTTP1_1, Http::Method::GET> request("http://example.com", 80);

    EXPECT_EQ(request.addHeader(HeaderKeys::Accept, "GGG"), request);
    EXPECT_EQ(request.removeHeader(HeaderKeys::Accept), request);
    EXPECT_EQ(request.addHeader(getHeaderKeyString(HeaderKeys::Accept), "GGG"), request);
    EXPECT_EQ(request.removeHeader(getHeaderKeyString(HeaderKeys::Accept)), request);
}