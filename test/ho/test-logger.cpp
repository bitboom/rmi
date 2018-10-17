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
 * @file        test-logger.cpp
 * @author      Sangwan Kwon (sangwan.kwon@samsung.com)
 */

#include <ho/logger.hxx>

#include <gtest/gtest.h>

/*
TEST(AUDIT, CONCOLE_LOGGER_ARGS)
{
	CONSOLE_I("LogLevel - Info: " << 1 << '2' << "3");
	CONSOLE_D("LogLevel - Debug: " << 1 << '2' << "3");
	CONSOLE_W("LogLevel - Warning: " << 1 << '2' << "3");
	CONSOLE_E("LogLevel - Error: " << 1 << '2' << "3");
}
*/

TEST(HO_LOGGER, CONCOLE_LOGGER_TEXT)
{
	ho::log(INFO, "LogLevel - Info");
	ho::log(DEBUG, "LogLevel - Debug");
	ho::log(WARN, "LogLevel - Warn");
	ho::log(ERROR, "LogLevel - Error");
}

TEST(HO_LOGGER, CONCOLE_LOGGER_ARGS)
{
	ho::log(INFO, 1);
	ho::log(DEBUG, 1.1f);
	ho::log(WARN, std::string("String"));
}
