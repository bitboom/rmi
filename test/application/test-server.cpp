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

#include "application/server.hxx"
#include "transport/connection.hxx"

#include <klay/testbench.h>

#include <string>
#include <thread>
#include <memory>
#include <iostream>
#include <chrono>

using namespace rmi::application;
using namespace rmi::transport;

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

TESTCASE(SERVER)
{
	std::string sockPath = ("./server");

	Server server;
	server.listen(sockPath);

	auto foo = std::make_shared<Foo>();
	server.expose(foo, "Foo::setName", &Foo::setName);
	server.expose(foo, "Foo::getName", &Foo::getName);

	std::string param = "RMI-TEST";

	auto client = std::thread([&]() {
		std::this_thread::sleep_for(std::chrono::seconds(1));

		Connection conn(sockPath);
		{
			Message msg(MessageType::MethodCall, "Foo::setName");
			msg.enclose(param);

			Message reply = conn.request(msg);
			bool ret = true;
			reply.disclose(ret);

			TEST_EXPECT_LAMBDA(CLIENT_SIDE, ret, false);
		}

		{
			Message msg(MessageType::MethodCall, "Foo::getName");

			Message reply = conn.request(msg);
			std::string ret;
			reply.disclose(ret);

			TEST_EXPECT_LAMBDA(CLIENT_SIDE, ret, param);
		}

		server.stop();
	});

	server.start();

	if (client.joinable())
		client.join();
}
