cmake_minimum_required(VERSION 3.1)
project(asteroids)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_COMPILER /usr/local/Cellar/llvm/5.0.1/bin/clang++)
#add_definitions(-DDODEBUG)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARIES})

include_directories("${CMAKE_SOURCE_DIR}/engine")
include_directories("${CMAKE_SOURCE_DIR}/lib")

FILE(GLOB _ALL_FILES ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/*)
FOREACH(_FILE ${_ALL_FILES})
  IF(IS_DIRECTORY ${_FILE})
    INCLUDE_DIRECTORIES(${_FILE})
  ENDIF()
ENDFOREACH()

file(GLOB ENGINE_SOURCES "${CMAKE_SOURCE_DIR}/engine/*.cpp")
file(GLOB GAME_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*/*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -nostdinc++ -std=c++17 -Wall -pedantic -Wextra -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -I/usr/local/opt/llvm/include -I/usr/local/opt/llvm/include/c++/v1/ -Wno-reserved-id-macro -Wno-padded")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fprofile-instr-generate -fcoverage-mapping ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast ${CMAKE_CXX_FLAGS}")

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lprofiler -stdlib=libc++ -lSDL2 -lSDL2_gfx -lSDL2_gpu -L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib")
add_executable(asteroids ${CMAKE_SOURCE_DIR}/src/main.cpp ${ENGINE_SOURCES} ${GAME_SOURCES})
