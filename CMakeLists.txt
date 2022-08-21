cmake_minimum_required(VERSION 3.22)

project(SFMLTetris)

find_package(SFML REQUIRED window system graphics)

set(CMAKE_CXX_STANDARD 20)

add_executable(SFMLTetris main.cpp tetromino.cpp field.cpp game.cpp leaderboard.cpp)

target_link_libraries(SFMLTetris PUBLIC sfml-system sfml-graphics sfml-window)
