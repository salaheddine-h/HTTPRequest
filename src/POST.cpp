#include "POST.hpp"
#include <unistd.h>

POST::POST()
{
}

POST::~POST()
{
}

// bool POST::isMultipartUpload(const HttpRequest& request) const
// {
//     if (request.headers.find("Content-Type") == request.headers.end())
//         return false;

//     std::string contentType = request.headers.at("Content-Type");

//     size_t position = contentType.find("multipart/form-data");

//     if (position == std::string::npos)
//         return false;

//     return true;
// }

// Response POST::handleMultipartUpload(const HttpRequest& request) const
// {
//     if (!isMultipartUpload(request))
//         return Response(); // Not a multipart upload, return an empty response
//     return Response();
// }


std::string POST::getParentDirectory(const std::string& target) const
{
    size_t pos = target.find_last_of('/');

    if (pos == std::string::npos)
        return "."; // Current directory

    return target.substr(0, pos);
}

bool POST::canWrite(const std::string& target) const
{
    std::string parent = getParentDirectory(target);
    return (access(parent.c_str(), W_OK) == 0);
}

Response POST::buildCreatedResponse(int statusCode, const std::string& message) const
{
    Response response;

    response.setStatusCode(statusCode);
    response.setReasonPhrase(message);

    // Empty body for 201 Created
    response.setBody("");
    response.addHeader("Content-Length", "0");
    response.addHeader("Content-Type", "text/plain");

    return response;
}

bool POST::saveBody(const std::string& path, const std::string& body) const
{
    std::ofstream file(path.c_str());

    if (!file.is_open())
        return false;

    file << body;
    file.close();

    return true;
}

bool POST::validateParentDirectory(const std::string& target) const
{
    std::string parent = getParentDirectory(target);

    return (getPathType(parent) == DIRECTORY_PATH);
}


Response POST::execute(const HttpRequest& request)
{
    if (request.path.empty())
        return buildErrorResponse(400, "Bad Request");

    MultipartUploadStrategy strategy;
    if (strategy.isMultipartUpload(request))
        return strategy.handleMultipartUpload(request);
    std::string target = resolveTarget(request);

    if (!validateParentDirectory(target))
        return buildErrorResponse(404, "Not Found");

    if (!canWrite(target))
        return buildErrorResponse(403, "Forbidden");

    if (!saveBody(target, request.body))
        return buildErrorResponse(500, "Internal Server Error");

    return buildCreatedResponse(201, "Created");
}
