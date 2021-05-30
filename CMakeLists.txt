cmake_minimum_required(VERSION 3.17)
project(Battleship)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/Users/sveta/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp GameManager/GameManager.cpp GameManager/GameManager.h Menu/Menu.cpp Menu/Menu.h Grid/Grid.cpp Grid/Grid.h WinnersManager/WinnerManager.cpp WinnersManager/WinnerManager.h Help/Help.cpp Help/Help.h Bonus/Bonus.cpp Bonus/Bonus.h)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES})

add_executable(Battleship_exe main.cpp)