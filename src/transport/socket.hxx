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
 * @file        socket.hxx
 * @author      Jaemin Ryu (jm77.ryu@samsung.com)
 *              Sangwan Kwon (sangwan.kwon@samsung.com)
 * @brief       Define Unix Domain Socket.
 */

#pragma once

#include <string>
#include <iostream>
#include <stdexcept>

namespace rmi {
namespace transport {

class Socket {
public:
	explicit Socket(int fd) noexcept;
	explicit Socket(const std::string& path);
	virtual ~Socket(void); 

	Socket(const Socket&) = delete;
	Socket& operator=(const Socket&) = delete;

	Socket(Socket&&);
	Socket& operator=(Socket&&);

	Socket accept(void) const;
	static Socket connect(const std::string& path);

	void write(const void* buffer, const size_t size) const;
	void read(void* buffer, const size_t size) const;

	int getFd(void) const noexcept;

private:
	const int MAX_BACKLOG_SIZE = 100;

	int fd;
};

} // namespace transport
} // namespace rmi
