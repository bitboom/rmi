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
 * @file        logger.hxx
 * @author      Sangwan Kwon (sangwan.kwon@samsung.com)
 */

#pragma once

#include <string>

namespace rmi {
namespace audit {

enum class LogLevel : char {
	INFO = 'I',
	DEBUG = 'D',
	WARNING = 'W',
	ERROR = 'E'
};

class Logger {
public:
	virtual ~Logger() = default;

	void info(const std::string& message) noexcept;
	void debug(const std::string& message) noexcept;
	void warning(const std::string& message) noexcept;
	void error(const std::string& message) noexcept;

protected:
	virtual void log(const LogLevel level, const std::string& message) noexcept = 0;
};

} // namespace audit
} // namespace rmi