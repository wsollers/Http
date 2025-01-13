#ifndef HTTPHEADERS_H
#define HTTPHEADERS_H

#include <string>
#include <unordered_map>

namespace Http {

    using Headers = std::unordered_map<std::string, std::string>;

    // Enum class for HeaderKeys
enum class HeaderKeys {
    Accept,
    AcceptCharset,
    AcceptEncoding,
    AcceptLanguage,
    Authorization,
    CacheControl,
    Connection,
    ContentEncoding,
    ContentLanguage,
    ContentLength,
    ContentType,
    Cookie,
    Date,
    ETag,
    Expect,
    Expires,
    Host,
    IfMatch,
    IfModifiedSince,
    IfNoneMatch,
    IfRange,
    IfUnmodifiedSince,
    LastModified,
    Location,
    Origin,
    Pragma,
    ProxyAuthorization,
    Range,
    Referer,
    Server,
    TE,
    Trailer,
    TransferEncoding,
    Upgrade,
    UserAgent,
    Vary,
    Via,
    Warning,
    WWWAuthenticate,
    Unknown
};

// Function to convert HeaderKeys to their string equivalents
constexpr std::string_view getHeaderKeyString(HeaderKeys key) {
    switch (key) {
        case HeaderKeys::Accept: return "Accept";
        case HeaderKeys::AcceptCharset: return "Accept-Charset";
        case HeaderKeys::AcceptEncoding: return "Accept-Encoding";
        case HeaderKeys::AcceptLanguage: return "Accept-Language";
        case HeaderKeys::Authorization: return "Authorization";
        case HeaderKeys::CacheControl: return "Cache-Control";
        case HeaderKeys::Connection: return "Connection";
        case HeaderKeys::ContentEncoding: return "Content-Encoding";
        case HeaderKeys::ContentLanguage: return "Content-Language";
        case HeaderKeys::ContentLength: return "Content-Length";
        case HeaderKeys::ContentType: return "Content-Type";
        case HeaderKeys::Cookie: return "Cookie";
        case HeaderKeys::Date: return "Date";
        case HeaderKeys::ETag: return "ETag";
        case HeaderKeys::Expect: return "Expect";
        case HeaderKeys::Expires: return "Expires";
        case HeaderKeys::Host: return "Host";
        case HeaderKeys::IfMatch: return "If-Match";
        case HeaderKeys::IfModifiedSince: return "If-Modified-Since";
        case HeaderKeys::IfNoneMatch: return "If-None-Match";
        case HeaderKeys::IfRange: return "If-Range";
        case HeaderKeys::IfUnmodifiedSince: return "If-Unmodified-Since";
        case HeaderKeys::LastModified: return "Last-Modified";
        case HeaderKeys::Location: return "Location";
        case HeaderKeys::Origin: return "Origin";
        case HeaderKeys::Pragma: return "Pragma";
        case HeaderKeys::ProxyAuthorization: return "Proxy-Authorization";
        case HeaderKeys::Range: return "Range";
        case HeaderKeys::Referer: return "Referer";
        case HeaderKeys::Server: return "Server";
        case HeaderKeys::TE: return "TE";
        case HeaderKeys::Trailer: return "Trailer";
        case HeaderKeys::TransferEncoding: return "Transfer-Encoding";
        case HeaderKeys::Upgrade: return "Upgrade";
        case HeaderKeys::UserAgent: return "User-Agent";
        case HeaderKeys::Vary: return "Vary";
        case HeaderKeys::Via: return "Via";
        case HeaderKeys::Warning: return "Warning";
        case HeaderKeys::WWWAuthenticate: return "WWW-Authenticate";
        case HeaderKeys::Unknown: return "Unknown";
    }
    return "Unknown";
}

} // namespace http`

#endif // HTTPHEADERS_H
