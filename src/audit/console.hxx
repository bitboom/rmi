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

#pragma once

#include <cstring>
#include <sstream>
#include <iostream>

#include "logger.hxx"
#include "colorize.hxx"

namespace rmi {
namespace audit {

class Console final : public Logger {
protected:
	void log(const LogLevel level, const std::string& message) noexcept override;
};

#ifndef __FILENAME__
#define __FILENAME__                                                    \
	(::strrchr(__FILE__, '/') ? ::strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define PRETTY_MESSAGE(tokens)                                 \
	(static_cast<std::ostringstream &>(std::ostringstream() << \
	 __FILENAME__ << "(:" <<                                   \
	 __LINE__ << "), " <<                                      \
	 __func__ << "()/ " <<                                     \
	 tokens)).str()

#define CONSOLE_LOG(level, message)                       \
	do {				                                  \
			rmi::audit::Console console;                       \
			std::string pretty = PRETTY_MESSAGE(message); \
			switch (level) {                              \
			case rmi::audit::LogLevel::INFO:                   \
				console.info(pretty);                     \
				break;                                    \
			case rmi::audit::LogLevel::DEBUG:                  \
				console.debug(pretty);                    \
				break;                                    \
			case rmi::audit::LogLevel::WARNING:                \
				console.warning(pretty);                  \
				break;                                    \
			case rmi::audit::LogLevel::ERROR:                  \
			default:                                      \
				console.error(pretty);                    \
				break;                                    \
			}                                             \
	} while (0)

#define CONSOLE_I(message) CONSOLE_LOG(rmi::audit::LogLevel::INFO, message)
#define CONSOLE_D(message) CONSOLE_LOG(rmi::audit::LogLevel::DEBUG, message)
#define CONSOLE_W(message) CONSOLE_LOG(rmi::audit::LogLevel::WARNING, message)
#define CONSOLE_E(message) CONSOLE_LOG(rmi::audit::LogLevel::ERROR, message)

} // namespace audit
} // namespace rmi
