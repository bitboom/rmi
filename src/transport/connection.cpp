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
 * @file        connection.cpp
 * @author      Jaemin Ryu (jm77.ryu@samsung.com)
 *              Sangwan Kwon (sangwan.kwon@samsung.com)
 * @brief       Implementation of the socket communication session.
 */

#include "connection.hxx"

#include <utility>

namespace rmi {
namespace transport {

Connection::Connection(transport::Socket&& socket) noexcept : socket(std::move(socket))
{
}

Connection::Connection(const std::string& path) :
	socket(transport::Socket::connect(path))
{
}

void Connection::send(Message& message) const
{
	std::lock_guard<std::mutex> lock(this->transmitMutex);
	message.encode(this->socket);
}

Message Connection::recv(void) const
{
	std::lock_guard<std::mutex> lock(this->receiveMutex);

	Message reply;
	reply.decode(this->socket);

	return reply;
}

Message Connection::request(Message& message) const
{
	this->send(message);
	return this->recv();
}

int Connection::getFd(void) const noexcept
{
	return this->socket.getFd();
}

} // namespace transport
} // namespace rmi
