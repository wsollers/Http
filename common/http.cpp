#include "http.h"


void sendRequest(const std::string &url, const Http::Method method,
                           const Http::Headers &headers,
                           const std::string &body) {
  Http::Request<decltype(transport), declType(protocol), Http::Method::GET> request(url);
    


}
