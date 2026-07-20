// #include "MultipartUploadStrategy.hpp"


// bool MultipartUploadStrategy::isMultipartUpload(const HttpRequest& request)
// {
//     if (request.headers.find("Content-Type") == request.headers.end())
//         return false;

//     std::string contentType = request.headers.at("Content-Type");

//     size_t position = contentType.find("multipart/form-data");

//     if (position == std::string::npos)
//         return false;

//     return true;
// }


// PartHeaders MultipartUploadStrategy::readPartHeaders(const HttpRequest& request ,size_t delimiterPosition)
// {
//     PartHeaders partHeaders;

//     partHeaders.filename = "";
//     partHeaders.contentType = "";
//     partHeaders.name = "";
//     skip_delimiterPosition();
// }

// Response MultipartUploadStrategy::handleMultipartUpload(const HttpRequest& request)
// {
//     Response response;
//     std::string boundary = extractBoundary(request);

//     std::string delimiter = "--" + boundary;
//     size_t DelimiterPosition = findFirstDelimiter(request, delimiter);
//     PartHeaders returnheader = readPartHeaders(request, DelimiterPosition);

//     std::cout << boundary << std::endl;

//     if(isMultipartUpload(request))
//     {
//         response.setStatusCode(501);
//         response.setReasonPhrase("Not Implemented");
//         response.addHeader("Content-Type", "text/plain");
//         response.setBody("Multipart Upload");
//     }
//     return response;
// }

// size_t MultipartUploadStrategy::findFirstDelimiter(const HttpRequest& request, const std::string& delimiter)
// {
//     size_t start = request.body.find(delimiter);

//     if (start == std::string::npos)
//         return std::string::npos;

//     return start;
// }

// std::string MultipartUploadStrategy::extractBoundary(const HttpRequest& request)
// {
//     if (request.headers.find("Content-Type") == request.headers.end())
//         return "";

//     std::string contentType = request.headers.at("Content-Type");

//     size_t position = contentType.find("boundary=");

//     if (position == std::string::npos)
//         return "";

//     return contentType.substr(position + 9); // 9 is the length of "boundary="
// }


#include "MultipartUploadStrategy.hpp"
#include <iostream>

bool MultipartUploadStrategy::isMultipartUpload(const HttpRequest& request)
{
    if (request.headers.find("Content-Type") == request.headers.end())
        return false;

    std::string contentType = request.headers.at("Content-Type");

    return contentType.find("multipart/form-data") != std::string::npos;
}

std::string MultipartUploadStrategy::extractBoundary(const HttpRequest& request)
{
    std::string contentType = request.headers.at("Content-Type");

    size_t pos = contentType.find("boundary=");

    if (pos == std::string::npos)
        return "";

    return contentType.substr(pos + 9);
}


Response MultipartUploadStrategy::handleMultipartUpload(const HttpRequest& request)
{
    Response response;

    if (!isMultipartUpload(request))
    {
        response.setStatusCode(400);
        response.setReasonPhrase("Bad Request");
        response.addHeader("Content-Type", "text/plain");
        response.setBody("Not a multipart request");
        return response;
    }

    std::string boundary = extractBoundary(request);

    if (boundary.empty())
    {
        response.setStatusCode(400);
        response.setReasonPhrase("Bad Request");
        response.addHeader("Content-Type", "text/plain");
        response.setBody("Boundary not found");
        return response;
    }

    std::string delimiter = "--" + boundary;

    size_t delimiterPosition = findFirstDelimiter(request, delimiter);

    if (delimiterPosition == std::string::npos)
    {
        response.setStatusCode(400);
        response.setReasonPhrase("Bad Request");
        response.addHeader("Content-Type", "text/plain");
        response.setBody("Invalid multipart body");
        return response;
    }

    PartHeaders headers = readPartHeaders(request, delimiterPosition);

    std::cout << "Boundary : " << boundary << std::endl;
    std::cout << "Delimiter position : " << delimiterPosition << std::endl;
    std::cout << "Filename : " << headers.filename << std::endl;
    std::cout << "Name : " << headers.name << std::endl;
    std::cout << "Content-Type : " << headers.contentType << std::endl;

    response.setStatusCode(501);
    response.setReasonPhrase("Not Implemented");
    response.addHeader("Content-Type", "text/plain");
    response.setBody("Multipart Upload");

    return response;
}


size_t MultipartUploadStrategy::findFirstDelimiter(const HttpRequest& request,const std::string& delimiter)
{
    return request.body.find(delimiter);
}

PartHeaders MultipartUploadStrategy::readPartHeaders(const HttpRequest& request,size_t delimiterPosition)
{
    (void)request;
    (void)delimiterPosition;

    PartHeaders headers;

    headers.filename = "";
    headers.contentType = "";
    headers.name = "";

    return headers;
}
