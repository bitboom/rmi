CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(rmi)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

SET(LIB_DIR ${PROJECT_SOURCE_DIR}/../lib)
SET(RMI_DIR ${PROJECT_SOURCE_DIR}/../src)
SET(TEST_DIR ${PROJECT_SOURCE_DIR})

INCLUDE_DIRECTORIES(${LIB_DIR} ${RMI_DIR})

FUNCTION(BUILD_TEST TEST_NAME TEST_SRCS)
	ADD_EXECUTABLE(${TEST_NAME} ${TEST_SRCS})

	SET_TARGET_PROPERTIES(${TEST_NAME} PROPERTIES COMPILE_FLAGS "-fPIE")
	SET_TARGET_PROPERTIES(${TEST_NAME} PROPERTIES LINK_FLAGS "-pie -pthread")

	INSTALL(TARGETS ${TEST_NAME}
			DESTINATION ${PROJECT_SOURCE_DIR}
			PERMISSIONS OWNER_READ
						OWNER_WRITE
						OWNER_EXECUTE
						GROUP_READ
						GROUP_EXECUTE
						WORLD_READ
						WORLD_EXECUTE)

	TARGET_LINK_LIBRARIES(${TEST_NAME} -ldl -lrt)
ENDFUNCTION(BUILD_TEST)

SET(TEST_SRCS ${LIB_DIR}/klay/testbench.cpp
			  ${RMI_DIR}/audit/logger.cpp
			  ${RMI_DIR}/audit/console.cpp
			  ${RMI_DIR}/application/server.cpp
			  ${RMI_DIR}/application/client.cpp
			  ${RMI_DIR}/stream/archive.cpp
			  ${RMI_DIR}/transport/socket.cpp
			  ${RMI_DIR}/transport/message.cpp
			  ${RMI_DIR}/transport/connection.cpp
			  ${RMI_DIR}/event/eventfd.cpp
			  ${RMI_DIR}/event/mainloop.cpp
			  ${TEST_DIR}/test-main.cpp
			  ${TEST_DIR}/klass/test-functor.cpp
			  ${TEST_DIR}/stream/test-archive.cpp
			  ${TEST_DIR}/audit/test-console.cpp
			  ${TEST_DIR}/transport/test-socket.cpp
			  ${TEST_DIR}/transport/test-connection.cpp
			  ${TEST_DIR}/application/test-server-client.cpp)

BUILD_TEST(${PROJECT_NAME} "${TEST_SRCS}")
