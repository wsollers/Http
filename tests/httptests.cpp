// http_protocol_test.cpp
#include <gtest/gtest.h>
#include "http.h"



// Test case for HttpProtocol enum class
TEST(HttpProtocolTest, MatchesExpectedValues) {
    EXPECT_EQ(Http::getProtocolString(Http::Protocol::HTTP1_0), "HTTP/1.0");
    EXPECT_EQ(Http::getProtocolString(Http::Protocol::HTTP1_1), "HTTP/1.1");
    EXPECT_EQ(Http::getProtocolString(Http::Protocol::HTTP2), "HTTP/2");
    EXPECT_EQ(Http::getProtocolString(Http::Protocol::HTTP3), "HTTP/3");
}

// Test case for Transport enum class
TEST(HttpTransportTest, MatchesExpectedValues) {
    EXPECT_EQ(Http::getTransportString(Http::Transport::TLS), "HTTPS");
    EXPECT_EQ(Http::getTransportString(Http::Transport::HTTP), "HTTP");

}

// Unit Test for getMethodString
TEST(GetMethodStringTest, ReturnsExpectedStrings) {
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