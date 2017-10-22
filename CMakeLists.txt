cmake_minimum_required (VERSION 2.6)

project (OpenSofa)
set(TARGET_LIBRARY_NAME opensofa)
set(CMAKE_CXX_STANDARD 11)

# dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# source files
include_directories(include)
set(PROJECT_SRC
  "src/TCPServer.cpp"
  "src/TCPClient.cpp"
  "src/SyncQueue.cpp"
  "src/NetworkConnection.cpp"
  "src/BufferedConnection.cpp")
# library
add_library(${TARGET_LIBRARY_NAME} ${PROJECT_SRC})
target_link_libraries(${TARGET_LIBRARY_NAME} Threads::Threads)

# test
enable_testing()
add_subdirectory(test)

# Examples
add_subdirectory(examples)
