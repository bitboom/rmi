# C++ Remote Method Invocation


| Linux |
|:-------:|
| [![Build Status](https://travis-ci.org/bitboom/rmi.svg?branch=devel)](https://travis-ci.org/bitboom/rmi) |

Remote Method Invocation API wriiten in Modern C++. (support from C++11)  
This project is designed for simple server-client communication.  

*This project is forked from Tizen open-source platform. (KLAY Project)*  
*Comply with APACHE-2.0 License.*

### SEVER SIDE
```cpp
#include "application/server.hxx"

#include <memory>

using namespace rmi::application;

// 1. Define server-side class for exposing method.
struct Foo {
	bool setName(const std::string& name)
	{
		this->name = name;
		return false;
	}

	std::string getName(void)
	{
		return this->name;
	}

	std::string name;
};

auto main() -> int
{
// 2. Listen socket for accepting client request.
	Server server;
	server.listen("./server.sock");

// 3. Expose remote object and method.
	auto foo = std::make_shared<Foo>();
	server.expose(foo, "Foo::setName", &Foo::setName);
	server.expose(foo, "Foo::getName", &Foo::getName);

// 4. Run server.
	server.start();
  
  return 0;
}
```

### CLIENT SIDE
```cpp
#include "application/client.hxx"

#include <string>

using namespace rmi::application;

auto main() -> int
{
// 1. Connect to server socket.
	Client client("./server.sock");

// 2. Invoke retmote method and get the result.
	bool ret = client.invoke<bool>("Foo::setName", std::string("Name-parameter"));
	std::string name = client.invoke<std::string>("Foo::getName");

  return 0;
}
```
