cmake_minimum_required(VERSION 2.6)
project(othello)

add_executable(othello main.cpp)

install(TARGETS othello RUNTIME DESTINATION bin)
