#include "RequestAdapter.hpp"

HttpRequest RequestAdapter::toHttpRequest(const ClientRequest& client)
{
    HttpRequest request;

    request.method  = client.getMethod();
    request.path    = client.getRequestPath();
    request.version = client.getVersion();
    request.headers = client.getHeaders();
    request.body    = client.getBody();

    return request;
}