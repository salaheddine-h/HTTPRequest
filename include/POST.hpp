#ifndef POST_HPP
#define POST_HPP

#include "AMethod.hpp"
#include "Response.hpp"
#include "PathType.hpp"
#include "HttpRequest.hpp"
#include "MultipartUploadStrategy.hpp"
#include <sys/stat.h>
#include <fstream>
#include <unistd.h>

class POST : public AMethod
{
    private:
        MultipartUploadStrategy multiPart;
        Response buildCreatedResponse(int status,const std::string& message) const;
        std::string getParentDirectory(const std::string& target) const;
        bool canWrite(const std::string& path) const;
        bool validateParentDirectory(const std::string& target) const;
        bool saveBody(const std::string& path, const std::string& body) const;
    public:
        POST();
        virtual ~POST();

        virtual Response execute(const HttpRequest& request);
};
#endif