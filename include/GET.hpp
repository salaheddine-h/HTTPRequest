#ifndef GET_HPP
#define GET_HPP

#include "AMethod.hpp"

class GET : public AMethod
{
public:
    virtual Response execute(const HttpRequest& request);
};

#endif