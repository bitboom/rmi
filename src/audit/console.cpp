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
 * @file        console.hxx
 * @author      Sangwan Kwon (sangwan.kwon@samsung.com)
 */

#include "console.hxx"

namespace rmi {
namespace audit {

void Console::log(const LogLevel level, const std::string& message) noexcept
{
	switch (level) {
	case LogLevel::INFO:
		std::cout << Colorize(Code::GREEN);
		break;
	case LogLevel::DEBUG:
		std::cout << Colorize(Code::DEFAULT);
		break;
	case LogLevel::WARNING:
		std::cout << Colorize(Code::MAGENTA);
		break;
	case LogLevel::ERROR:
	default:
		std::cout << Colorize(Code::RED);
		break;
	}

	std::cout << static_cast<char>(level) << "/" << message << std::endl;

	std::cout << Colorize(Code::DEFAULT);
}

} // namespace audit
} // namespace rmi
