#include "include/DeleteMethod.hpp"

#include <iostream>

void testDELETE(const std::string& path)
{
    HttpRequest request;

    request.method = "DELETE";
    request.path = path;
    request.version = "HTTP/1.1";
    request.headers["Host"] = "localhost";

    DeleteMethod method;

    Response response = method.execute(request);

    std::cout << "=========================================\n";
    std::cout << "DELETE \"" << path << "\"\n";
    std::cout << response.toString() << std::endl;
}

int main()
{
    // Test: Empty path -> 400 Bad Request
    testDELETE("");

    return 0;
}