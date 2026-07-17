#ifndef GET_HPP
#define GET_HPP

#include "AMethod.hpp"
#include "HttpRequest.hpp"
#include "Response.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <iostream>


class GET : public AMethod
{
    private:

        // Check if the requested file exists
        bool fileExists(const std::string& path) const;

        // Read the file content
        std::string readFile(const std::string& path) const;

        // Detect MIME type from file extension
        std::string getContentType(const std::string& path) const;

        // Build a successful HTTP response
        Response buildSuccessResponse(
            const std::string& body,
            const std::string& contentType
        ) const;

        // Build an error response
        Response buildErrorResponse(
            int statusCode,
            const std::string& message
        ) const;
    public:

        GET();
        virtual ~GET();
        
        virtual Response execute(const HttpRequest& request);
};

#endif