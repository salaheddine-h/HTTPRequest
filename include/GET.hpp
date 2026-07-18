#ifndef GET_HPP
#define GET_HPP

#include "AMethod.hpp"
#include "Response.hpp"
#include "PathType.hpp"
#include "HttpRequest.hpp"


#include <dirent.h> // open directory
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <stdexcept>
#include <sys/stat.h>


class GET : public AMethod
{
    private:
        // Utilities
        // bool fileExists(const std::string& path) const;
        // bool isDirectory(const std::string& path) const;
        std::string readFile(const std::string& path) const;
        std::string getContentType(const std::string& path) const;
        // Internal handlers
        Response serveFile(const std::string& path) const;
        Response handleDirectory(const std::string& path) const;
        std::string generateAutoIndex(const std::string& path) const;
        void    buildAutoIndexHeader(std::ostringstream& html,const std::string& path) const;
        void    buildAutoIndexFooter(std::ostringstream& html) const;
        void    handleDirectoryEntries(DIR* dir,const std::string& currentPath,std::ostringstream& html) const;
        bool    isHiddenEntry(const std::string& name) const;
        bool    isDirectoryEntry(const std::string& currentPath,const std::string& name) const;
        void    appendDirectoryLink(std::ostringstream& html,const std::string& name) const;
        void    appendFileLink(std::ostringstream& html,const std::string& name) const;
        Response buildFileResponse(const std::string& body,const std::string& contentType) const;


    public:
        GET();
        virtual ~GET();

        virtual Response execute(const HttpRequest& request);
};


#endif