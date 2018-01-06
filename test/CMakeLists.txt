file(GLOB TEST_SRC *.cpp)

add_executable(Tests ${TEST_SRC})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(Tests
    ${TARGET_LIBRARY_NAME}
    ${GTEST_BOTH_LIBRARIES}
)
add_test(Tests Tests)
