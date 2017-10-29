
# Chat example
add_executable(chat_server chat_server.cpp)
target_link_libraries(chat_server ${TARGET_LIBRARY_NAME})

add_executable(chat_client chat_client.cpp)
target_link_libraries(chat_client ${TARGET_LIBRARY_NAME})

# Pong example
add_executable(pong pong.cpp)
target_link_libraries(pong ${TARGET_LIBRARY_NAME})

add_executable(remote_control remote_control.cpp)
target_link_libraries(remote_control ${TARGET_LIBRARY_NAME})

