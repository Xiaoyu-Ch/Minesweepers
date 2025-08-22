cmake_minimum_required(VERSION 3.16)
project(MinesweeperMilestone1)

set(CMAKE_CXX_STANDARD 11)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(MinesweeperMilestone1 main.cpp WelcomeWindow.cpp gameWindow.cpp)

target_link_libraries(MinesweeperMilestone1 sfml-graphics sfml-window sfml-system)
