cmake_minimum_required(VERSION 2.6)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project(othello)

set(EXECUTABLE_OUTPUT_PATH .)

include_directories(.)
add_library(lib OthelloPoint.cpp Board.cpp Player.cpp PersonPlayerMover.cpp ComputerPlayerMover.cpp)

add_executable(othello main.cpp)
target_link_libraries(othello lib)

install(TARGETS othello RUNTIME DESTINATION bin)
