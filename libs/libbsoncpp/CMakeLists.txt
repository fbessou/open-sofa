cmake_minimum_required (VERSION 2.6)

project (LibBsonCpp)
set(TARGET_LIBRARY_NAME bsoncpp-1.0)
set(CMAKE_CXX_STANDARD 11)

find_package (libbson-1.0 1.3 REQUIRED)

message ("--   libbson found version \"${BSON_VERSION}\"")
message ("--   libbson include path \"${BSON_INCLUDE_DIRS}\"")
message ("--   libbson libraries \"${BSON_LIBRARIES}\"")

cmake_minimum_required (VERSION 2.6)

set(TARGET_LIBRARY_NAME libbsoncpp-1.0)
set(CMAKE_CXX_STANDARD 11)

# source files
include_directories(include)
set(PROJECT_SRC )

add_library(${TARGET_LIBRARY_NAME} ${PROJECT_SRC})

target_include_directories (${TARGET_LIBRARY_NAME} PRIVATE ${BSON_STATIC_INCLUDE_DIRS})
target_link_libraries (${TARGET_LIBRARY_NAME} PRIVATE ${BSON_STATIC_LIBRARIES})
target_compile_definitions (${TARGET_LIBRARY_NAME} PRIVATE ${BSON_STATIC_DEFINITIONS})


