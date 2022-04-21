cmake_minimum_required(VERSION 3.21)
project(CSCClub_Tetris)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)

find_package(SDL_ttf REQUIRED)

include_directories(${SDL2_INCLUDE-DIRS} ${SDL2_TTF_INCLUDE_DIR})

add_executable(CSCClub_Tetris main.cpp Shape.cpp Shape.h Board.cpp Board.h Game.cpp Game.h Color.h scene.cpp Bag.cpp Bag.h)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_ttf)