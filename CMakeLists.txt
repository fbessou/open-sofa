cmake_minimum_required (VERSION 2.6)

project (OpenSofa)
set(TARGET_LIBRARY_NAME opensofa)
set(CMAKE_CXX_STANDARD 17)

option(TEST "Build & run tests" OFF)

# dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# source files
include_directories("msgpack-c/include")
include_directories(include)
set(PROJECT_SRC
  "src/Client.cpp"
  "src/Server.cpp"
  "src/Event.cpp"
  "src/io/TCPServer.cpp"
  "src/io/TCPClient.cpp"
  "src/io/SyncQueue.cpp"
  "src/io/MessagePackIOStream.cpp"
  "src/io/NetworkConnection.cpp"
  "src/io/BufferedConnection.cpp")

# library
add_library(${TARGET_LIBRARY_NAME} ${PROJECT_SRC})
target_link_libraries(${TARGET_LIBRARY_NAME} Threads::Threads)

# test
if(TEST)
  enable_testing()
  add_subdirectory(test)
endif()

# Examples
add_subdirectory(examples)
