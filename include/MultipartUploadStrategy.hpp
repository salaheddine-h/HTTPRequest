#ifndef MultipartUploadStrategy_HPP
#define MultipartUploadStrategy_HPP

#include "HttpRequest.hpp"
#include "Response.hpp"
#include "AMethod.hpp"
#include <iostream>

struct PartHeaders
{
    std::string filename;
    std::string contentType;
    std::string name;
};

class MultipartUploadStrategy
{
    public:
        PartHeaders readPartHeaders(size_t delimiterPosition);
        bool isMultipartUpload(const HttpRequest& request);
        Response handleMultipartUpload(const HttpRequest& request);
        std::string extractBoundary(const HttpRequest& request);
        size_t findFirstDelimiter(const HttpRequest& request, const std::string& delimiter);
        PartHeaders readPartHeaders(const HttpRequest& request,size_t delimiterPosition);

};
#endif // MultipartUploadStrategy