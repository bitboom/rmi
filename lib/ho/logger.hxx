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
 * @brief       Thread-safe logger for console. (Header Only)
 * @usage       ho::log(INFO, "The logger for console.");
 *              ho::log(DEBUG, "The logger for console.");
 *              ho::log(WARN, "The "The logger for console.");
 *              ho::log(ERROR, "The "The logger for console.");
 */

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <mutex>

namespace ho {

enum class LogLevel : char {
	Info = 'I',
	Debug = 'D',
	Warn = 'W',
	Error = 'E'
};

struct LogRecord {
	LogLevel level;
	std::string file;
	int line;
	std::string func;
};

template<typename T>
void log(const LogRecord& record, const T& message) noexcept;

#define INFO ho::LogRecord { ho::LogLevel::Info, __FILE__, __LINE__, __func__ }
#define DEBUG ho::LogRecord { ho::LogLevel::Debug, __FILE__, __LINE__, __func__ }
#define WARN ho::LogRecord { ho::LogLevel::Warn, __FILE__, __LINE__, __func__ }
#define ERROR ho::LogRecord { ho::LogLevel::Error, __FILE__, __LINE__, __func__ }

namespace {

std::string to_string(const ho::LogRecord& record)
{
	std::stringstream ss;
	ss << "[" <<static_cast<char>(record.level) << "/"
	   << record.file << "(:" << record.line << "), "
	   << record.func + "()] ";

	return ss.str();
}

enum class Code {
	Black = 30,
	Red = 31,
	Green = 32,
	Yellow = 33,
	Blue = 34,
	Magenta = 35,
	Cyan = 36,
	White = 37,
	Default = 39
};

class Colorize {
public:
	explicit Colorize(Code code = Code::Default) : code(code) {}

	friend std::ostream& operator<<(std::ostream& os, const Colorize& c)
	{
		return os << "\033[" << static_cast<int>(c.code) << "m";
	}

private:
	Code code;
};

} // anonymous namespace

template<typename T>
void log(const ho::LogRecord& record, const T& message) noexcept
{
	static std::mutex m;

	{
		std::lock_guard<std::mutex> l(m);

		switch (record.level) {
		case ho::LogLevel::Info:
			std::cout << Colorize(Code::Green);
			break;
		case ho::LogLevel::Debug:
			std::cout << Colorize(Code::Default);
			break;
		case ho::LogLevel::Warn:
			std::cout << Colorize(Code::Magenta);
			break;
		case ho::LogLevel::Error:
		default:
			std::cout << Colorize(Code::Red);
			break;
		}

		std::stringstream ss;
		ss << message;

		auto format = to_string(record);
		std::cout << format << ss.str() << std::endl;

		std::cout << Colorize(Code::Default);
	}
}

} // namespace ho
