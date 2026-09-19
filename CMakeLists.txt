cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

project(TaskManager)

add_executable(main main.cpp taskmanager.cpp)

target_compile_options(main PRIVATE -Wall -Wshadow -Wextra -fsanitize=address,undefined)
target_link_options(main PRIVATE -fsanitize=address,undefined)