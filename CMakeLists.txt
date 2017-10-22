cmake_minimum_required (VERSION 2.6)

project (OpenSofa)
set(TARGET_LIBRARY_NAME opensofa)
set(CMAKE_CXX_STANDARD 11)

# dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package (libbson-1.0 1.3 REQUIRED)

message ("--   libbson found version \"${BSON_VERSION}\"")
message ("--   libbson include path \"${BSON_INCLUDE_DIRS}\"")
message ("--   libbson libraries \"${BSON_LIBRARIES}\"")


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

target_include_directories (${TARGET_LIBRARY_NAME} PRIVATE ${BSON_STATIC_INCLUDE_DIRS})
target_link_libraries (${TARGET_LIBRARY_NAME} PRIVATE ${BSON_STATIC_LIBRARIES})
target_compile_definitions (${TARGET_LIBRARY_NAME} PRIVATE ${BSON_STATIC_DEFINITIONS})

target_link_libraries(${TARGET_LIBRARY_NAME} Threads::Threads)

# test
enable_testing()
add_subdirectory(test)

# Examples
add_subdirectory(examples)
