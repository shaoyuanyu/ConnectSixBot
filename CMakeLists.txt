cmake_minimum_required(VERSION 3.27)
project(ConnectSixBot)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        ConnectSixBot
        src/main.cpp
        src/Bot.cpp src/Bot.h
        src/Grid.cpp src/Grid.h
        src/GameConfig.h
        src/Result.h
)
