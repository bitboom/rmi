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
 * @file        server.cpp
 * @author      Sangwan Kwon (sangwan.kwon@samsung.com)
 * @brief       Implementation of server application.
 */

#include "server.hxx"

#include "../transport/message.hxx"

namespace rmi {
namespace application {

void Server::start(void)
{
	for (const auto& path : this->socketPaths) {
		auto socket = std::make_shared<Socket>(path);
		auto accept = [this, socket]() {
			this->onAccept(std::make_shared<Connection>(socket->accept()));
		};

		this->mainloop.addHandler(socket->getFd(), std::move(accept));
	}

	this->mainloop.run();
}

void Server::stop(void)
{
	{
		std::lock_guard<std::mutex> lock(this->mutex);

		for (auto iter : this->connectionMap)
			this->mainloop.removeHandler(iter.first);
	}
	this->mainloop.stop();
}

void Server::listen(const std::string& socketPath)
{
	this->socketPaths.insert(socketPath);
}

void Server::onAccept(std::shared_ptr<Connection>&& connection)
{
	if (connection == nullptr)
		throw std::invalid_argument("Wrong connection.");

	auto onRead = [this, connection]() {
		std::lock_guard<std::mutex> lock(this->mutex);

		auto iter = this->connectionMap.find(connection->getFd());
		if (iter == this->connectionMap.end())
			throw std::runtime_error("Faild to find connection.");

		this->dispatch(iter->second);
	};

	auto close = [this, connection]() {
		this->onClose(connection);
	};

	int clientFd = connection->getFd();
	this->mainloop.addHandler(clientFd, std::move(onRead), std::move(close));

	std::lock_guard<std::mutex> lock(this->mutex);

	this->dispatch(connection);
	this->connectionMap[clientFd] = std::move(connection);
}

void Server::onClose(const std::shared_ptr<Connection>& connection)
{
	if (connection == nullptr)
		throw std::invalid_argument("Wrong connection.");

	std::lock_guard<std::mutex> lock(this->mutex);

	auto iter = this->connectionMap.find(connection->getFd());
	if (iter == this->connectionMap.end())
		throw std::runtime_error("Faild to find connection.");

	this->mainloop.removeHandler(iter->first);
	this->connectionMap.erase(iter);
}

void Server::dispatch(const std::shared_ptr<Connection>& connection)
{
	Message request = connection->recv();
	std::string funcName = request.signature;
	auto iter = this->functorMap.find(funcName);
	if (iter == this->functorMap.end())
		throw std::runtime_error("Faild to find function.");

	auto functor = iter->second;
	auto result = functor->invoke(request.archive);

	Message reply(MessageType::Reply, funcName);
	reply.enclose(result);

	connection->send(reply);
}

} // namespace application
} // namespace rmi
