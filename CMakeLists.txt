cmake_minimum_required(VERSION 3.0)
project(aerocore)

#set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DFC_USE_SDL_GPU)
add_definitions(-DSDL_GPU_DISABLE_GLES)
add_definitions(-DASSET_PATH="${CMAKE_SOURCE_DIR}/assets/")

string( REPLACE "/DNDEBUG" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

if (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
    set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/release)
else()
    set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/debug)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories(lib/sdl-gpu/include)
include_directories(lib/sdl-gpu/src/externals/glew/GL)
include_directories(lib/sdl-gpu/src/externals/stb_image)
include_directories(lib/sdl-gpu/src/externals/stb_image_write)
include_directories(lib/nfont/NFont)
include_directories(lib/nfont/SDL_FontCache)
include_directories(lib/tinyxml2)
include_directories(lib/Brofiler)
include_directories(lib)

if (WIN32)
    include_directories(lib/SDL2/WINDOWS/include)
    include_directories(lib/SDL2_ttf/WINDOWS/include)
    include_directories(lib/SDL2_mixer/WINDOWS/include)

    string(CONCAT SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/SDL2/WINDOWS/lib/x64/SDL2.lib;"
                                 "${CMAKE_CURRENT_LIST_DIR}/lib/SDL2/WINDOWS/lib/x64/SDL2main.lib;"
                                 "${CMAKE_CURRENT_LIST_DIR}/lib/SDL2_mixer/WINDOWS/lib/x64/SDL2_mixer.lib;"
                                 "${CMAKE_CURRENT_LIST_DIR}/lib/SDL2_ttf/WINDOWS/lib/x64/SDL2_ttf.lib;"
                                 )

    string(CONCAT BROFILER_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/Brofiler/ProfilerCore64.lib;")

    file(GLOB SDL2_DLLS "${CMAKE_CURRENT_LIST_DIR}/lib/SDL2/WINDOWS/lib/x64/*.dll")
    file(GLOB SDL2_MIXER_DLLS "${CMAKE_CURRENT_LIST_DIR}/lib/SDL2_mixer/WINDOWS/lib/x64/*.dll")
    file(GLOB SDL2_TTF_DLLS "${CMAKE_CURRENT_LIST_DIR}/lib/SDL2_ttf/WINDOWS/lib/x64/*.dll")

    file(GLOB BROFILER_DLLS "${CMAKE_CURRENT_LIST_DIR}/lib/Brofiler/*.dll")

    add_custom_target(CopyDLL
       COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_DLLS} ${CMAKE_BINARY_DIR}
       COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_MIXER_DLLS} ${CMAKE_BINARY_DIR}
       COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_TTF_DLLS} ${CMAKE_BINARY_DIR}
       COMMAND ${CMAKE_COMMAND} -E copy ${BROFILER_DLLS} ${CMAKE_BINARY_DIR}
    )
endif (WIN32)

if (UNIX)
    INCLUDE(FindPkgConfig)
    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
    PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
    PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS})
endif (UNIX)

install(FILES ${SDL_DLLS} DESTINATION ${CMAKE_BINARY_DIR})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
find_package (Threads)

include_directories(src)

file(GLOB GAME_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*/*.cpp")
file(GLOB THIRD_PARTY_SOURCES "${CMAKE_SOURCE_DIR}/lib/nfont/NFont/*.cpp" "${CMAKE_SOURCE_DIR}/lib/nfont/SDL_FontCache/*.c" "${CMAKE_SOURCE_DIR}/lib/tinyxml2/tinyxml2.cpp")
file(GLOB SDL_GPU_SOURCES "${CMAKE_SOURCE_DIR}/lib/sdl-gpu/src/*.c" "${CMAKE_SOURCE_DIR}/lib/sdl-gpu/src/externals/stb_image/*.c" "${CMAKE_SOURCE_DIR}/lib/sdl-gpu/src/externals/stb_image_write/*.c" "${CMAKE_SOURCE_DIR}/lib/sdl-gpu/src/externals/glew/*.c")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS}")

SET(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
add_executable(aerocore ${CMAKE_SOURCE_DIR}/src/main.cpp ${GAME_SOURCES} ${THIRD_PARTY_SOURCES} ${SDL_GPU_SOURCES})

target_link_libraries(aerocore ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${BROFILER_LIBRARIES})

if (WIN32)
    add_dependencies(aerocore CopyDLL)
endif (WIN32)

if (UNIX)
    TARGET_LINK_LIBRARIES(aerocore ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2MIXER_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif (UNIX)

