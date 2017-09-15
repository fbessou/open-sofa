cmake_minimum_required (VERSION 2.6)

project (OpenSofa)
set(TARGET_LIBRARY_NAME opensofa)
set (CMAKE_CXX_STANDARD 11)

# dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# source files
include_directories(include)
set(PROJECT_SRC
	"src/client.cpp"
	"src/tcpserver.cpp")

# library
add_library(${TARGET_LIBRARY_NAME} ${PROJECT_SRC})
target_link_libraries(${TARGET_LIBRARY_NAME} Threads::Threads)

# test programs
#add_executable(TCPClientTest tcp_client_test.cpp)
#target_link_libraries(TCPClientTest ${PROJECT_LIBRARY_NAME})

#add_executable(TCPServerTest tcp_server_test.cpp)
#target_link_libraries(TCPServerTest ${PROJECT_LIBRARY_NAME})
