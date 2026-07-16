# HTTP Request & Method Handling

## Goal

This module is responsible for processing an already parsed HTTP request.

Networking (socket, bind, listen, accept, recv) is handled by another part of the project.

The parser is also handled by another module.

Our responsibility starts after receiving a valid `HttpRequest` object.

---

# Global Flow

Client (Browser)
        │
        ▼
recv(clientFd)
        │
        ▼
HTTP Parser
        │
        ▼
HttpRequest
        │
        ▼
Routing
        │
        ▼
AMethod
        │
 ┌──────┼────────┐
 │      │        │
GET    POST    DeleteMethod
        │
        ▼
Response
        │
        ▼
HTTP Response String
        │
        ▼
send(clientFd)

---

# Input

The parser gives us an object similar to:

```cpp
class HttpRequest
{
public:
    std::string method;
    std::string path;
    std::string version;

    std::map<std::string, std::string> headers;

    std::string body;
};

src/

├── http/
│
│   ├── request/
│   │
│   │   ├── HttpRequest.hpp      (Parser team)
│   │   ├── ClientRequest.hpp    (Parser team)
│   │
│   ├── response/
│   │
│   │   ├── Response.hpp         (Your Team)
│   │   ├── Response.cpp
│   │   ├── StatusCode.hpp
│   │
│   └── methods/
│
│       ├── AMethod.hpp
│       ├── AMethod.cpp
│       │
│       ├── GET.hpp
│       ├── GET.cpp
│       │
│       ├── POST.hpp
│       ├── POST.cpp
│       │
│       ├── DeleteMethod.hpp
│       └── DeleteMethod.cpp
│
├── routing/
│
│   ├── Routing.hpp
│   └── Routing.cpp
│
├── cgi/
│
└── socket/
