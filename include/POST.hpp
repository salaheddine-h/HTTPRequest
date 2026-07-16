#ifndef POST_HPP
#define POST_HPP

#include "AMethod.hpp"

class POST : public AMethod
{
public:
    POST();
    virtual ~POST();

    virtual Response execute(const HttpRequest& request);
};

#endif