find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

macro(gentest testname src)
add_executable(${testname} ${src})

target_link_libraries(${testname}
    ${TARGET_LIBRARY_NAME}
    ${GTEST_BOTH_LIBRARIES}
)
add_test(${testname} ${testname})

endmacro()

gentest(TCPServer tcpserver_test.cpp)
gentest(MessagePack messagepack_test.cpp)
