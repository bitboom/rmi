/*
 *  Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */
/*
 * @file        server.hxx
 * @author      Sangwan Kwon (sangwan.kwon@samsung.com)
 * @brief       Server application for exposing function(method).
 */

#pragma once

#include <set>
#include <string>
#include <unordered_map>
#include <mutex>
#include <memory>

#include "../klass/functor.hxx"
#include "../event/mainloop.hxx"
#include "../transport/socket.hxx"
#include "../transport/connection.hxx"

using namespace rmi::klass;
using namespace rmi::transport;
using namespace rmi::event;

namespace rmi {
namespace application {

class Server {
public:
	explicit Server() = default;
	virtual ~Server() = default;

	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;

	Server(Server&&) = delete;
	Server& operator=(Server&&) = delete;

	void start(void);
	void stop(void);

	void listen(const std::string& socketPath);

	template<typename O, typename F>
	void expose(O&& object, const std::string& name, F&& func);

private:
	using ConnectionMap = std::unordered_map<int, std::shared_ptr<Connection>>;

	void onAccept(std::shared_ptr<Connection>&& connection);
	void onClose(const std::shared_ptr<Connection>& connection);

	void dispatch(const std::shared_ptr<Connection>& connection);

	Mainloop mainloop;

	std::set<std::string> socketPaths;

	ConnectionMap connectionMap;
	std::mutex connectionMutex;

	FunctorMap functorMap;
	std::mutex functorMutex;
};

template<typename O, typename F>
void Server::expose(O&& object, const std::string& name, F&& func)
{
	auto functor = make_functor_ptr(std::forward<O>(object), std::forward<F>(func));
	this->functorMap[name] = functor;
}

} // namespace application
} // namespace rmi
