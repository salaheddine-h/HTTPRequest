#include "include/MethodFactory.hpp"
#include "include/Response.hpp"
#include "include/AMethod.hpp"
#include "include/HttpRequest.hpp"

#include <iostream>

void runTest(const HttpRequest& request)
{
    std::cout << "=========================================\n";
    std::cout << request.method << " " << request.path << "\n";
    std::cout << "=========================================\n";

    AMethod* method = MethodFactory::createMethod(request);

    if (method == NULL)
    {
        Response response;

        response.setStatusCode(405);
        response.setReasonPhrase("Method Not Allowed");
        response.addHeader("Content-Type", "text/plain");
        response.setBody("Method Not Allowed");

        std::cout << response.toString() << std::endl;
        return;
    }

    Response response = method->execute(request);

    std::cout << response.toString() << std::endl;

    delete method;
}

int main()
{
    // ==========================================================
    // GET Existing File
    // ==========================================================

    HttpRequest getFile;

    getFile.method = "GET";
    getFile.path = "/index.html";
    getFile.version = "HTTP/1.1";
    getFile.headers["Host"] = "localhost";

    runTest(getFile);

    // ==========================================================
    // GET JavaScript
    // ==========================================================

    HttpRequest getJs;

    getJs.method = "GET";
    getJs.path = "/app.js";
    getJs.version = "HTTP/1.1";
    getJs.headers["Host"] = "localhost";

    runTest(getJs);

    // ==========================================================
    // GET PNG
    // ==========================================================

    HttpRequest getImage;

    getImage.method = "GET";
    getImage.path = "/logo.png";
    getImage.version = "HTTP/1.1";
    getImage.headers["Host"] = "localhost";

    runTest(getImage);

    // ==========================================================
    // GET Unknown Extension
    // ==========================================================

    HttpRequest getUnknown;

    getUnknown.method = "GET";
    getUnknown.path = "/test.xyz";
    getUnknown.version = "HTTP/1.1";
    getUnknown.headers["Host"] = "localhost";

    runTest(getUnknown);

    // ==========================================================
    // GET Missing File
    // ==========================================================

    HttpRequest missing;

    missing.method = "GET";
    missing.path = "/notfound.txt";
    missing.version = "HTTP/1.1";
    missing.headers["Host"] = "localhost";

    runTest(missing);

    // ==========================================================
    // GET Root Directory
    // ==========================================================

    HttpRequest root;

    root.method = "GET";
    root.path = "/";
    root.version = "HTTP/1.1";
    root.headers["Host"] = "localhost";

    runTest(root);

    // ==========================================================
    // POST Plain Text
    // ==========================================================

    HttpRequest post;

    post.method = "POST";
    post.path = "/hello.txt";
    post.version = "HTTP/1.1";
    post.body = "Hello from POST";
    post.headers["Host"] = "localhost";
    post.headers["Content-Type"] = "text/plain";

    runTest(post);

    // ==========================================================
    // Multipart Upload
    // ==========================================================

    HttpRequest multipart;

    multipart.method = "POST";
    multipart.path = "/uploads/test.txt";
    multipart.version = "HTTP/1.1";

    multipart.body =
        "------WebKitXYZ\r\n"
        "Content-Disposition: form-data; name=\"photo\"; filename=\"cat.png\"\r\n"
        "Content-Type: image/png\r\n"
        "\r\n"
        "HELLO_IMAGE_CONTENT\r\n"
        "------WebKitXYZ--\r\n";

    multipart.headers["Host"] = "localhost";
    multipart.headers["Content-Type"] =
        "multipart/form-data; boundary=----WebKitXYZ";

    runTest(multipart);

    // ==========================================================
    // DELETE Existing File
    // ==========================================================

    HttpRequest del;

    del.method = "DELETE";
    del.path = "/hello.txt";
    del.version = "HTTP/1.1";
    del.headers["Host"] = "localhost";

    runTest(del);

    // ==========================================================
    // DELETE Missing File
    // ==========================================================

    HttpRequest delMissing;

    delMissing.method = "DELETE";
    delMissing.path = "/abcdef.txt";
    delMissing.version = "HTTP/1.1";
    delMissing.headers["Host"] = "localhost";

    runTest(delMissing);

    // ==========================================================
    // DELETE Directory
    // ==========================================================

    HttpRequest delDir;

    delDir.method = "DELETE";
    delDir.path = "/uploads";
    delDir.version = "HTTP/1.1";
    delDir.headers["Host"] = "localhost";

    runTest(delDir);

    // ==========================================================
    // Unsupported Method
    // ==========================================================

    HttpRequest put;

    put.method = "PUT";
    put.path = "/index.html";
    put.version = "HTTP/1.1";
    put.headers["Host"] = "localhost";

    runTest(put);

    return 0;
}