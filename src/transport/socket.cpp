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
 * @file        socket.cpp
 * @author      Jaemin Ryu (jm77.ryu@samsung.com)
 *              Sangwan Kwon (sangwan.kwon@samsung.com)
 * @brief       Implementation of Unix Domain Socket.
 */

#include "socket.hxx"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/un.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "../audit/console.hxx"

namespace rmi {
namespace transport {

namespace {

void set_cloexec(int fd)
{
	if (::fcntl(fd, F_SETFD, FD_CLOEXEC) == -1)
		throw std::runtime_error("Failed to set CLOSEXEC.");
}

} // anonymous namespace

Socket::Socket(int fd) noexcept : fd(fd)
{
	CONSOLE_D("Socket created: " << fd);
}

Socket::Socket(const std::string& path)
{
	if (path.size() >= sizeof(::sockaddr_un::sun_path))
		throw std::invalid_argument("Socket path size is wrong.");

	int fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1)
		throw std::runtime_error("Failed to create socket.");

	set_cloexec(fd);

	::sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	::strncpy(addr.sun_path, path.c_str(), sizeof(sockaddr_un::sun_path));

	if (addr.sun_path[0] == '@')
		addr.sun_path[0] = '\0';
	else
		::unlink(path.c_str());

	if (::bind(fd, reinterpret_cast<::sockaddr*>(&addr), sizeof(::sockaddr_un)) == -1) {
		::close(fd);
		throw std::runtime_error("Failed to bind.");
	}

/*
	int optval = 1;
	if (::setsockopt(fd, SOL_SOCKET, SO_PASSCRED, &optval, sizeof(optval)) == -1) {
		::close(fd);
		throw std::runtime_error("Failed to set socket-option.");
	}
*/

	if (::listen(fd, MAX_BACKLOG_SIZE) == -1) {
		::close(fd);
		throw std::runtime_error("Failed to liten.");
	}

	this->fd = fd;
}

Socket::Socket(Socket&& that) : fd(that.fd)
{
	that.fd = -1;
}

Socket& Socket::operator=(Socket&& that)
{
	if (this == &that)
		return *this;

	this->fd = that.fd;
	that.fd = -1;

	return *this;
}

Socket::~Socket(void)
{
	CONSOLE_D("Socket closed: " << fd);
	if (fd != -1)
		::close(fd);
}

Socket Socket::accept(void) const
{
	int fd = ::accept(this->fd, nullptr, nullptr);
	if (fd == -1)
		throw std::runtime_error("Failed to accept.");

	set_cloexec(fd);

	return Socket(fd);
}

Socket Socket::connect(const std::string& path)
{
	if (path.size() >= sizeof(::sockaddr_un::sun_path))
		throw std::invalid_argument("Socket path size is wrong.");

	int fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1)
		throw std::runtime_error("Failed to create socket.");

	set_cloexec(fd);

	::sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	::strncpy(addr.sun_path, path.c_str(), sizeof(::sockaddr_un::sun_path));

	if (addr.sun_path[0] == '@')
		addr.sun_path[0] = '\0';

	if (::connect(fd, reinterpret_cast<::sockaddr*>(&addr), sizeof(sockaddr_un)) == -1) {
		::close(fd);
		throw std::runtime_error("Failed to connect.");
	}

	return Socket(fd);
}

int Socket::getFd(void) const noexcept
{
	return this->fd;
}

void Socket::read(void *buffer, const size_t size) const
{
	size_t total = 0;

	while (total < size) {
		auto rest = reinterpret_cast<unsigned char*>(buffer) + total;
		int bytes = ::read(this->fd, rest, size - total);
		if (bytes >= 0)
			total += bytes;
		else if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
			continue;
		else
			std::runtime_error("Failed to read.");
	}
}

void Socket::write(const void *buffer, const size_t size) const
{
	size_t written = 0;

	while (written < size) {
		auto rest = reinterpret_cast<const unsigned char*>(buffer) + written;
		int bytes = ::write(this->fd, rest, size - written);
		if (bytes >= 0)
			written += bytes;
		else if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
			continue;
		else
			std::runtime_error("Failed to write.");
	}
}

} // namespace transport
} // namespace rmi
