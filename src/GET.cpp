#include "GET.hpp"

GET::GET()
{
}

GET::~GET()
{
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

    if (extension == "txt")
        return "text/plain";
    else if (extension == "html" || extension == "htm")
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
    //else  if(extension == "pdf")
    //     return "application/pdf";
    // else if(extension == "xml")
    //     return "application/xml";
    // else if(extension == "zip")
    //     return "application/zip";
        

    return "application/octet-stream";
}

Response GET::serveFile(const std::string& path) const
{
    try
    {
        std::string body = readFile(path);
        std::string type = getContentType(path);

        return buildFileResponse(body, type);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return buildErrorResponse(500, "Internal Server Error");
    }
}

Response GET::handleDirectory(const std::string& path) const
{
    std::string indexPath = path + "/index.html";

    if (fileExists(indexPath))
        return serveFile(indexPath);

    std::string html = generateAutoIndex(path);

    return buildFileResponse(html, "text/html");
}

std::string GET::generateAutoIndex(const std::string& path) const
{
    DIR* dir = opendir(path.c_str());

    if (dir == NULL)
        return "";

    std::ostringstream html;

    buildAutoIndexHeader(html, path);

    handleDirectoryEntries(dir, path, html);

    buildAutoIndexFooter(html);

    closedir(dir);

    return html.str();
}

Response GET::buildFileResponse(const std::string& body,const std::string& contentType) const
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
    std::string target = resolveTarget(request);

    std::string path = target;
    if (path.empty())
        return buildErrorResponse(400, "Bad Request");
    switch (getPathType(target))
    {
        case NOT_FOUND:
            return buildErrorResponse(404, "Not Found");

        case FILE_PATH:
            return serveFile(target);

        case DIRECTORY_PATH:
            return handleDirectory(target);
        default:
            return buildErrorResponse(500, "Internal Server Error");
    }
}