#  Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License
#
# @file      CMakeLists.txt
# @author    Sangwan kwon (sangwan.kwon@samsung.com)
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

SET(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
SET(RMI_DIR ${PROJECT_SOURCE_DIR}/src)
SET(TEST_DIR ${PROJECT_SOURCE_DIR}/test)

INCLUDE_DIRECTORIES(${LIB_DIR} ${RMI_DIR})

FUNCTION(BUILD_TEST TEST_NAME TEST_SRCS)
	ADD_EXECUTABLE(${TEST_NAME} ${TEST_SRCS})


	INSTALL(TARGETS ${TEST_NAME}
			DESTINATION ${PROJECT_SOURCE_DIR}
			PERMISSIONS OWNER_READ
						OWNER_WRITE
						OWNER_EXECUTE
						GROUP_READ
						GROUP_EXECUTE
						WORLD_READ
						WORLD_EXECUTE)

	TARGET_LINK_LIBRARIES(${TEST_NAME} gtest_main -ldl -lrt)

	ADD_TEST(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
ENDFUNCTION(BUILD_TEST)

SET(RMI_SRCS  ${RMI_DIR}/application/server.cpp
			  ${RMI_DIR}/application/client.cpp
			  ${RMI_DIR}/stream/archive.cpp
			  ${RMI_DIR}/transport/socket.cpp
			  ${RMI_DIR}/transport/message.cpp
			  ${RMI_DIR}/transport/connection.cpp
			  ${RMI_DIR}/event/eventfd.cpp
			  ${RMI_DIR}/event/mainloop.cpp)

SET(TEST_SRCS ${RMI_SRCS}
			  ${TEST_DIR}/klass/test-functor.cpp
			  ${TEST_DIR}/stream/test-archive.cpp
			  ${TEST_DIR}/transport/test-socket.cpp
			  ${TEST_DIR}/transport/test-connection.cpp
			  ${TEST_DIR}/ho/test-logger.cpp)

BUILD_TEST(${PROJECT_NAME}-test "${TEST_SRCS}")
