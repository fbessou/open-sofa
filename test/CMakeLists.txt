file(GLOB TEST_SRC *.cpp)

add_executable(TCPServerTest ${TEST_SRC})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(TCPServerTest
    ${TARGET_LIBRARY_NAME}
    ${GTEST_BOTH_LIBRARIES}
)
add_test(TCPServerTest TCPServerTest)
