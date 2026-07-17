#include "GET.hpp"

GET::GET()
{
}

GET::~GET()
{
}


bool GET::fileExists(const std::string& path) const
{
    struct stat info;

    return (stat(path.c_str(), &info) == 0);
}

std::string GET::readFile(const std::string& path) const
{
    std::ifstream file(path.c_str());

    if (!file.is_open())
        throw std::runtime_error("Could not open file.");

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}


std::string GET::getContentType(const std::string& path) const
{
    size_t dotPos = path.find_last_of('.');

    if (dotPos == std::string::npos)
        return "application/octet-stream";

    std::string extension = path.substr(dotPos + 1);

    if (extension == "html" || extension == "htm")
        return "text/html";
    else if (extension == "css")
        return "text/css";
    else if (extension == "js")
        return "application/javascript";
    else if (extension == "json")
        return "application/json";
    else if (extension == "png")
        return "image/png";
    else if (extension == "jpg" || extension == "jpeg")
        return "image/jpeg";
    else if (extension == "gif")
        return "image/gif";
    else
        return "application/octet-stream";
}
Response GET::buildErrorResponse(int statusCode,
                                 const std::string& message) const
{
    Response response;

    response.setStatusCode(statusCode);
    response.setReasonPhrase(message);

    std::ostringstream html;

    html << "<html>"
         << "<body>"
         << "<h1>"
         << statusCode
         << " "
         << message
         << "</h1>"
         << "</body>"
         << "</html>";

    response.setBody(html.str());
    response.addHeader("Content-Type", "text/html");

    return response;
}

Response GET::buildSuccessResponse(const std::string& body,
                                   const std::string& contentType) const
{
        Response response;

        response.setStatusCode(200);
        response.setReasonPhrase("OK");
        response.setBody(body);
        response.addHeader("Content-Type", contentType);
        std::ostringstream oss;
        oss << body.size();
        response.addHeader("Content-Length", oss.str());
        return response;
}

Response GET::execute(const HttpRequest& request)
{
    std::string path = request.path;

    if (path.empty())
        return buildErrorResponse(400, "Bad Request");

    if (!fileExists(path))
        return buildErrorResponse(404, "Not Found");

    try
    {
        std::string body = readFile(path);

        std::string type = getContentType(path);

        return buildSuccessResponse(body, type);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;

        return buildErrorResponse(500, "Internal Server Error");
    }
}