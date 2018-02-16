cmake_minimum_required(VERSION 3.1)
project(asteroids)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#SET(CMAKE_CXX_COMPILER /usr/local/Cellar/llvm/5.0.1/bin/clang-5.0)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("${CMAKE_SOURCE_DIR}/engine")
file(GLOB ENGINE_SOURCES "${CMAKE_SOURCE_DIR}/engine/*.cpp")
file(GLOB GAME_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g")
# -Wall -pedantic -Wextra -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lSDL2 -lSDL2_gfx") #-L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib")
add_executable(asteroids ${CMAKE_SOURCE_DIR}/src/main.cpp ${ENGINE_SOURCES} ${GAME_SOURCES})
