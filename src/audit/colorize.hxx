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
 * @file        colorize.hxx
 * @author      Sangwan Kwon (sangwan.kwon@samsung.com)
 * @brief       Colorize console text.
 */

#pragma once

#include <ostream>

namespace rmi {
namespace audit {

enum class Code {
	BLACK = 30,
	RED = 31,
	GREEN = 32,
	YELLOW = 33,
	BLUE = 34,
	MAGENTA = 35,
	CYAN = 36,
	WHITE = 37,
	DEFAULT = 39
};

class Colorize {
public:
	explicit Colorize(Code code = Code::DEFAULT) : code(code) {}

	friend std::ostream& operator<<(std::ostream& os, const Colorize& c)
	{
		return os << "\033[" << static_cast<int>(c.code) << "m";
	}

private:
	Code code;
};

} // namespace audit
} // namespace rmi
