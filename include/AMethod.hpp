#ifndef AMETHOD_HPP
#define AMETHOD_HPP


#include "Response.hpp"
#include "HttpRequest.hpp"

class AMethod
{
public:
    virtual ~AMethod();

    virtual Response execute(const HttpRequest& request) = 0;
};

#endif