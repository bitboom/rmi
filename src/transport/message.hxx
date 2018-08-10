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
 * @file        message.hxx
 * @author      Jaemin Ryu (jm77.ryu@samsung.com)
 *              Sangwan Kwon (sangwan.kwon@samsung.com)
 * @brief       The unit of socket communication.
 */

#pragma once

#include <string>
#include <atomic>
#include <vector>

#include "../stream/archive.hxx"

using namespace rmi::stream;

namespace rmi {
namespace transport {

enum MessageType : unsigned int {
	Invalid,
	MethodCall,
	Reply,
	Error,
	Signal
};

struct MessageHeader {
	unsigned int id;
	unsigned int type;
	size_t length;
};

class Message final {
public:
	explicit Message(void) = default;
	explicit Message(unsigned int type, const std::string& signature);
	~Message(void) noexcept = default;

	Message(const Message&) = default;
	Message(Message&&) = default;

	Message& operator=(const Message&) = default;
	Message& operator=(Message&&) = default;

	template<typename T>
	void encode(const T& device);
	template<typename T>
	void decode(const T& device);

	template<typename... Args>
	void enclose(Args&&... args);
	template<typename... Args>
	void disclose(Args&... args);

	MessageHeader header;
	std::string signature;
	Archive archive;

private:
	static std::atomic<unsigned int> sequence;
};

template<typename T>
void Message::encode(const T& device)
{
	header.id = sequence++;
	header.length = archive.buffer.size();

	device.write(&header, sizeof(header));
	device.write(this->archive.buffer.data(), header.length);
}

template<typename T>
void Message::decode(const T& device)
{
	MessageHeader header;
	device.read(&header, sizeof(header));

	this->archive.buffer.reserve(header.length);
	device.read(this->archive.buffer.data(), header.length);
	this->disclose(this->signature);
}

template<typename... Args>
void Message::enclose(Args&&... args)
{
	this->archive.pack(std::forward<Args>(args)...);
}

template<typename... Args>
void Message::disclose(Args&... args)
{
	this->archive.unpack(args...);
}

} // namespace transport
} // namespace rmi
