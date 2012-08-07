cmake_minimum_required(VERSION 2.8)
project(rainbow)

find_package(OpenGL REQUIRED)
find_package(SDL REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ ${CMAKE_CXX_FLAGS}")

set(sources "src/rainbow/main.cpp"
            "src/rainbow/display.cpp"
            "src/rainbow/input.cpp"
            "src/rainbow/renderer.cpp")

include_directories("include")
include_directories(${SDL_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})

add_executable(rainbow ${sources})
target_link_libraries(rainbow ${SDL_LIBRARY} ${OPENGL_LIBRARIES})
