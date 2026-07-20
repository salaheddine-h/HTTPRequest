#ifndef AMETHOD_HPP
#define AMETHOD_HPP

#include "Response.hpp"
#include "HttpRequest.hpp"
#include "PathType.hpp"
// #include "MultipartUploadStrategy.hpp"

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>

class AMethod
{
    public:
        virtual ~AMethod();
        virtual Response execute(const HttpRequest& request) = 0;

    protected:
        // Response helpers
        Response buildErrorResponse(int status,const std::string& message) const;
        std::string resolveTarget(const HttpRequest& request) const;
        // Filesystem helpers
        PathType getPathType(const std::string& path) const;
        bool fileExists(const std::string& path) const;
        bool isDirectory(const std::string& path) const;
};

#endif