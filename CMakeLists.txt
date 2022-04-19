cmake_minimum_required(VERSION 3.21)
project(CSCClub_Tetris)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE-DIRS})

add_executable(CSCClub_Tetris main.cpp Shape.cpp Shape.h Board.cpp Board.h Game.cpp Game.h Utilities.cpp Utilities.h scene.cpp arduino/lib/serialib.h arduino/lib/serialib.cpp arduino/controller.h arduino/controller.cpp arduino/DecodeStatusCodes.h  arduino/DecodeStatusCodes.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})