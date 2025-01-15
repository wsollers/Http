#include "http.h"


Http::Response Http::Client::sendRequest(const std::string &url, const Http::Method method,
                           const Http::Headers &headers,
                           const std::string &body) {
    
    Http::Response response;

    return response;

}
Http::Response Http::Client::sendRequest(const Http::Request &request) {
    Http::Response response;

    return response;
}

std::string_view Http::Response::getHeader(Http::HeaderKeys headerkey) {
    return std::string_view();
}

std::string_view Http::Response::getHeader(const std::string &headerkey) {
    return "";
}