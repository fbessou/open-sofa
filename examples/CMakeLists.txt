
# Chat example
add_executable(chat_server chat_server.cpp)
target_link_libraries(chat_server ${TARGET_LIBRARY_NAME})

add_executable(chat_client chat_client.cpp)
target_link_libraries(chat_client ${TARGET_LIBRARY_NAME})

