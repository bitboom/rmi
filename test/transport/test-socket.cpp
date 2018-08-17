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
 * @file        test-socket.cpp
 * @author      Sangwan Kwon (sangwan.kwon@samsung.com)
 */

#include "transport/socket.hxx"

#include <string>
#include <vector>
#include <limits>
#include <thread>
#include <chrono>
#include <cstring>

#include <gtest/gtest.h>

using namespace rmi::transport;

TEST(TRANSPORT, SOCKET_READ_WRITE)
{
	std::string sockPath = "./sock";
	Socket socket(sockPath);

	int input = std::numeric_limits<int>::max();
	bool input2 = true;

	int output = 0;
	bool output2 = false;

	auto client = std::thread([&]() {
		std::this_thread::sleep_for(std::chrono::seconds(1));

		// Send input to server.
		Socket connected = Socket::connect(sockPath);
		connected.send(&input);

		// Recv input2 from server.
		connected.recv(&output2);

		EXPECT_EQ(input2, output2);
	});

	Socket accepted = socket.accept();

	// Recv input from client.
	accepted.recv(&output);
	EXPECT_EQ(input, output);

	// Send input2 to client.
	accepted.send(&input2);

	if (client.joinable())
		client.join();
}

TEST(TRANSPORT, SOCKET_ABSTRACT)
{
	std::string sockPath = "@sock";
	Socket socket(sockPath);

	int input = std::numeric_limits<int>::max();
	bool input2 = true;

	int output = 0;
	bool output2 = false;

	auto client = std::thread([&]() {
		std::this_thread::sleep_for(std::chrono::seconds(1));

		// Send input to server.
		Socket connected = Socket::connect(sockPath);
		connected.send(&input);

		// Recv input2 from server.
		connected.recv(&output2);

		EXPECT_EQ(input2, output2);
	});

	Socket accepted = socket.accept();

	// Recv input from client.
	accepted.recv(&output);
	EXPECT_EQ(input, output);

	// Send input2 to client.
	accepted.send(&input2);

	if (client.joinable())
		client.join();
}
