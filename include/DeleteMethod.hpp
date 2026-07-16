#ifndef DELETEMETHOD_HPP
#define DELETEMETHOD_HPP

#include "AMethod.hpp"

class DeleteMethod : public AMethod
{
public:
    DeleteMethod();
    virtual ~DeleteMethod();

    virtual Response execute(const HttpRequest& request);
};

#endif