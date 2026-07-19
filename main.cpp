// #include "include/HTTPRequest.hpp"
#include "include/MethodFactory.hpp"
#include "include/Response.hpp"
#include "include/AMethod.hpp"

#include <iostream>

// void testDELETE(const std::string& path)
// {
//     HttpRequest request;

//     request.method = "DELETE";
//     request.path = path;
//     request.version = "HTTP/1.1";
//     request.headers["Host"] = "localhost";

//     DeleteMethod method;

//     Response response = method.execute(request);

//     std::cout << "=========================================\n";
//     std::cout << "DELETE \"" << path << "\"\n";
//     std::cout << response.toString() << std::endl;
// }

int main()
{
    HttpRequest request;

    request.method = "DELETE";
    request.path = "./uploads/test.txt";

    AMethod* method = MethodFactory::createMethod(request);
{
    if (method == NULL)
    {
        Response response;

        response.setStatusCode(405);
        response.setReasonPhrase("Method Not Allowed");
        response.addHeader("Content-Type", "text/plain");
        response.setBody("Method Not Allowed");

        std::cout << response.toString() << std::endl;
        return 0;
    }
    else
    {
        Response response = method->execute(request);

        std::cout << response.toString() << std::endl;

        delete method;
    }
    // // Test: Empty path -> 400 Bad Request
    // testDELETE("");

    return 0;
}