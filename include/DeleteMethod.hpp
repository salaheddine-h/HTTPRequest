#ifndef DELETEMETHOD_HPP
#define DELETEMETHOD_HPP

#include "AMethod.hpp"
#include "Response.hpp"
#include "PathType.hpp"
#include "HttpRequest.hpp"

#include <sys/stat.h>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>

class DeleteMethod : public AMethod
{
    public:
        DeleteMethod();
        virtual ~DeleteMethod();
        Response buildNoContentResponse() const;
        bool canDelete(const std::string& path) const;
        virtual Response execute(const HttpRequest& request);
};

#endif