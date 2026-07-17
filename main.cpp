#include "include/HttpRequest.hpp"
#include "include/GET.hpp"
#include "include/POST.hpp"
#include "include/DeleteMethod.hpp"

#include <iostream>


int main()
{
    HttpRequest request;

    request.method = "GET";
    request.path = "./www/index.html";
    request.version = "HTTP/1.1";

    request.headers["Host"] = "localhost";
    request.headers["Connection"] = "keep-alive";

    AMethod* method = new GET();

    Response response = method->execute(request);

    std::cout << "========== RAW HTTP RESPONSE ==========\n";
    std::cout << response.toString() << std::endl;
    std::cout << "=======================================\n";

    delete method;
}