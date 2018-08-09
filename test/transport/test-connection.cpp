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
 * @file        test-connection.cpp
 * @author      Sangwan Kwon (sangwan.kwon@samsung.com)
 */

#include "transport/connection.hxx"
#include "transport/socket.hxx"
#include "event/mainloop.hxx"
#include "event/eventfd.hxx"

#include <klay/testbench.h>

#include <string>
#include <thread>

using namespace rmi::transport;
using namespace rmi::event;

TESTCASE(SOCKET_COMMUNICATION)
{
	std::string sockPath = ("./sock");

	// server-side
	Mainloop mainloop;
	Socket socket(sockPath);

	std::string requestSignature = "request signature";
	int request1 = 100;
	bool request2 = true;
	std::string request3 = "request argument";

	std::string responseSignature = "response signature";
	int response1 = 300;
	bool response2 = false;
	std::string response3 = "response argument";

	auto onAccept = [&]() {
		Connection conn(socket.accept());
		Message request = conn.recv();
		TEST_EXPECT_LAMBDA(SERVER_SIDE, requestSignature, request.signature);

		int recv1;
		bool recv2;
		std::string recv3;
		request.disclose(recv1, recv2, recv3);
		TEST_EXPECT_LAMBDA(SERVER_SIDE, request1, recv1);
		TEST_EXPECT_LAMBDA(SERVER_SIDE, request2, recv2);
		TEST_EXPECT_LAMBDA(SERVER_SIDE, request3, recv3);

		Message reply(MessageType::Reply, responseSignature);
		reply.enclose(response1, response2, response3);
		conn.send(reply);

		mainloop.removeHandler(socket.getFd());
		mainloop.stop();
	};

	mainloop.addHandler(socket.getFd(), std::move(onAccept));
	auto serverThread = std::thread([&]() { mainloop.run(); });

	// client-side
	Connection conn(sockPath);
	Message msg(MessageType::Signal, requestSignature);
	msg.enclose(request1, request2, request3);

	Message reply = conn.request(msg);
	TEST_EXPECT(reply.signature, responseSignature);

	int recv1;
	bool recv2;
	std::string recv3;
	reply.disclose(recv1, recv2, recv3);
	TEST_EXPECT(response1, recv1);
	TEST_EXPECT(response2, recv2);
	TEST_EXPECT(response3, recv3);

	if (serverThread.joinable())
		serverThread.join();
}
