#include "include/MethodFactory.hpp"
#include "include/Response.hpp"
#include "include/AMethod.hpp"
#include "include/HttpRequest.hpp"
#include "multiplexing/header.hpp"

#include <iostream>

void runTest(const std::string& label, const HttpRequest& request)
{
    Server_block server;
    server.root = "./www";

    std::cout << "=========================================================="
              << std::endl;
    std::cout << "TEST: " << label << std::endl;
    std::cout << "=========================================================="
              << std::endl;

    AMethod* method = MethodFactory::createMethod(request);

    if (!method)
    {
        std::cout << "[NO METHOD HANDLER FOUND]" << std::endl << std::endl;
        return;
    }

    Response response = method->execute(request, server, NULL);

    std::cout << "Status : "
              << response.getStatusCode()
              << " "
              << response.getReasonPhrase()
              << std::endl;

    const std::map<std::string, std::string>& headers = response.getHeaders();

    for (std::map<std::string, std::string>::const_iterator it = headers.begin();
         it != headers.end();
         ++it)
    {
        std::cout << it->first
                  << ": "
                  << it->second
                  << std::endl;
    }

    std::cout << "Body size : "
              << response.getBody().size()
              << std::endl
              << std::endl;

    delete method;
}

int main()
{
    // ==========================================================
    // GET Existing File
    // ==========================================================
    {
        HttpRequest req;
        req.method = "GET";
        req.path = "/index.html";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("GET existing file", req);
    }

    // ==========================================================
    // GET JavaScript
    // ==========================================================
    {
        HttpRequest req;
        req.method = "GET";
        req.path = "/app.js";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("GET javascript file", req);
    }

    // ==========================================================
    // GET PNG
    // ==========================================================
    {
        HttpRequest req;
        req.method = "GET";
        req.path = "/logo.png";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("GET png file", req);
    }

    // ==========================================================
    // GET Unknown Extension
    // ==========================================================
    {
        HttpRequest req;
        req.method = "GET";
        req.path = "/test.xyz";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("GET unknown extension", req);
    }

    // ==========================================================
    // GET Missing File
    // ==========================================================
    {
        HttpRequest req;
        req.method = "GET";
        req.path = "/notfound.txt";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("GET missing file", req);
    }

    // ==========================================================
    // GET Root Directory
    // ==========================================================
    {
        HttpRequest req;
        req.method = "GET";
        req.path = "/";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("GET root directory", req);
    }

    // ==========================================================
    // GET Directory Without Index
    // ==========================================================
    {
        HttpRequest req;
        req.method = "GET";
        req.path = "/uploads";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("GET directory without trailing slash", req);
    }

    // ==========================================================
    // GET Path Traversal Attempt
    // ==========================================================
    {
        HttpRequest req;
        req.method = "GET";
        req.path = "/../../../../etc/passwd";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("GET path traversal attempt", req);
    }

    // ==========================================================
    // POST Plain Text
    // ==========================================================
    {
        HttpRequest req;
        req.method = "POST";
        req.path = "/hello.txt";
        req.version = "HTTP/1.1";
        req.body = "Hello from POST";
        req.headers["Host"] = "localhost";
        req.headers["Content-Type"] = "text/plain";
        runTest("POST plain text file", req);
    }

    // ==========================================================
    // POST Empty Body
    // ==========================================================
    {
        HttpRequest req;
        req.method = "POST";
        req.path = "/empty.txt";
        req.version = "HTTP/1.1";
        req.body = "";
        req.headers["Host"] = "localhost";
        req.headers["Content-Type"] = "text/plain";
        runTest("POST empty body", req);
    }

    // ==========================================================
    // POST Empty Path
    // ==========================================================
    {
        HttpRequest req;
        req.method = "POST";
        req.path = "";
        req.version = "HTTP/1.1";
        req.body = "should fail";
        req.headers["Host"] = "localhost";
        req.headers["Content-Type"] = "text/plain";
        runTest("POST empty path", req);
    }

    // ==========================================================
    // POST To Non-Existent Parent Directory
    // ==========================================================
    {
        HttpRequest req;
        req.method = "POST";
        req.path = "/no/such/dir/file.txt";
        req.version = "HTTP/1.1";
        req.body = "should fail 404";
        req.headers["Host"] = "localhost";
        req.headers["Content-Type"] = "text/plain";
        runTest("POST to non-existent parent directory", req);
    }

    // ==========================================================
    // Multipart Upload Valid
    // ==========================================================
    {
        HttpRequest req;
        req.method = "POST";
        req.path = "/uploads/test.txt";
        req.version = "HTTP/1.1";

        req.body =
            "------WebKitXYZ\r\n"
            "Content-Disposition: form-data; name=\"photo\"; filename=\"cat.png\"\r\n"
            "Content-Type: image/png\r\n"
            "\r\n"
            "HELLO_IMAGE_CONTENT\r\n"
            "------WebKitXYZ--\r\n";

        req.headers["Host"] = "localhost";
        req.headers["Content-Type"] = "multipart/form-data; boundary=----WebKitXYZ";
        runTest("Multipart upload valid single file", req);
    }

    // ==========================================================
    // Multipart Upload Multiple Files
    // ==========================================================
    {
        HttpRequest req;
        req.method = "POST";
        req.path = "/uploads";
        req.version = "HTTP/1.1";

        req.body =
            "------WebKitXYZ\r\n"
            "Content-Disposition: form-data; name=\"file1\"; filename=\"a.txt\"\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "AAAA\r\n"
            "------WebKitXYZ\r\n"
            "Content-Disposition: form-data; name=\"file2\"; filename=\"b.txt\"\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "BBBB\r\n"
            "------WebKitXYZ--\r\n";

        req.headers["Host"] = "localhost";
        req.headers["Content-Type"] = "multipart/form-data; boundary=----WebKitXYZ";
        runTest("Multipart upload multiple files", req);
    }

    // ==========================================================
    // Multipart Upload With Non-File Field (no filename)
    // ==========================================================
    {
        HttpRequest req;
        req.method = "POST";
        req.path = "/uploads";
        req.version = "HTTP/1.1";

        req.body =
            "------WebKitXYZ\r\n"
            "Content-Disposition: form-data; name=\"description\"\r\n"
            "\r\n"
            "just a text field\r\n"
            "------WebKitXYZ--\r\n";

        req.headers["Host"] = "localhost";
        req.headers["Content-Type"] = "multipart/form-data; boundary=----WebKitXYZ";
        runTest("Multipart upload with non-file field only", req);
    }

    // ==========================================================
    // Multipart Missing Boundary
    // ==========================================================
    {
        HttpRequest req;
        req.method = "POST";
        req.path = "/uploads";
        req.version = "HTTP/1.1";

        req.body =
            "------WebKitXYZ\r\n"
            "Content-Disposition: form-data; name=\"photo\"; filename=\"cat.png\"\r\n"
            "\r\n"
            "DATA\r\n"
            "------WebKitXYZ--\r\n";

        req.headers["Host"] = "localhost";
        req.headers["Content-Type"] = "multipart/form-data";
        runTest("Multipart missing boundary in header", req);
    }

    // ==========================================================
    // Multipart Malformed Body (no closing delimiter)
    // ==========================================================
    {
        HttpRequest req;
        req.method = "POST";
        req.path = "/uploads";
        req.version = "HTTP/1.1";

        req.body =
            "------WebKitXYZ\r\n"
            "Content-Disposition: form-data; name=\"photo\"; filename=\"cat.png\"\r\n"
            "\r\n"
            "DATA_WITHOUT_CLOSING_BOUNDARY";

        req.headers["Host"] = "localhost";
        req.headers["Content-Type"] = "multipart/form-data; boundary=----WebKitXYZ";
        runTest("Multipart malformed body missing closing delimiter", req);
    }

    // ==========================================================
    // Multipart Empty Body
    // ==========================================================
    {
        HttpRequest req;
        req.method = "POST";
        req.path = "/uploads";
        req.version = "HTTP/1.1";
        req.body = "";
        req.headers["Host"] = "localhost";
        req.headers["Content-Type"] = "multipart/form-data; boundary=----WebKitXYZ";
        runTest("Multipart empty body", req);
    }

    // ==========================================================
    // Multipart Target Is Not A Directory
    // ==========================================================
    {
        HttpRequest req;
        req.method = "POST";
        req.path = "/index.html";
        req.version = "HTTP/1.1";

        req.body =
            "------WebKitXYZ\r\n"
            "Content-Disposition: form-data; name=\"photo\"; filename=\"cat.png\"\r\n"
            "\r\n"
            "DATA\r\n"
            "------WebKitXYZ--\r\n";

        req.headers["Host"] = "localhost";
        req.headers["Content-Type"] = "multipart/form-data; boundary=----WebKitXYZ";
        runTest("Multipart target resolves to a file, not a directory", req);
    }

    // ==========================================================
    // Multipart Filename With Path Traversal
    // ==========================================================
    {
        HttpRequest req;
        req.method = "POST";
        req.path = "/uploads";
        req.version = "HTTP/1.1";

        req.body =
            "------WebKitXYZ\r\n"
            "Content-Disposition: form-data; name=\"photo\"; filename=\"../../evil.txt\"\r\n"
            "\r\n"
            "MALICIOUS\r\n"
            "------WebKitXYZ--\r\n";

        req.headers["Host"] = "localhost";
        req.headers["Content-Type"] = "multipart/form-data; boundary=----WebKitXYZ";
        runTest("Multipart filename with path traversal attempt", req);
    }

    // ==========================================================
    // DELETE Existing File
    // ==========================================================
    {
        HttpRequest req;
        req.method = "DELETE";
        req.path = "/hello.txt";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("DELETE existing file", req);
    }

    // ==========================================================
    // DELETE Missing File
    // ==========================================================
    {
        HttpRequest req;
        req.method = "DELETE";
        req.path = "/abcdef.txt";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("DELETE missing file", req);
    }

    // ==========================================================
    // DELETE Directory
    // ==========================================================
    {
        HttpRequest req;
        req.method = "DELETE";
        req.path = "/uploads";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("DELETE a directory", req);
    }

    // ==========================================================
    // DELETE Path Traversal Attempt
    // ==========================================================
    {
        HttpRequest req;
        req.method = "DELETE";
        req.path = "/../../../../etc/passwd";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("DELETE path traversal attempt", req);
    }

    // ==========================================================
    // DELETE Empty Path
    // ==========================================================
    {
        HttpRequest req;
        req.method = "DELETE";
        req.path = "";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("DELETE empty path", req);
    }

    // ==========================================================
    // Unknown Method
    // ==========================================================
    {
        HttpRequest req;
        req.method = "PATCH";
        req.path = "/hello.txt";
        req.version = "HTTP/1.1";
        req.headers["Host"] = "localhost";
        runTest("Unknown/unsupported HTTP method", req);
    }

    return 0;
}