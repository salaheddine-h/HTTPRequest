#include "MethodFactory.hpp"

AMethod* MethodFactory::createMethod(const HttpRequest& request)
{
    if (request.method == "GET")
        return new GET();

    if (request.method == "POST")
        return new POST();

    if (request.method == "DELETE")
        return new DeleteMethod();

    return NULL;
}