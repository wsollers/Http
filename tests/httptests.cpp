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
