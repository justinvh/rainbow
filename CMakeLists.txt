cmake_minimum_required(VERSION 2.8)
project(rainbow)

add_subdirectory(testing/gtest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${rainbow_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ ${CMAKE_CXX_FLAGS}")

# Warning pedantic flags for all
set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic ${CMAKE_CXX_FLAGS}")

set(sources "src/rainbow/main.cpp"
            "src/rainbow/display.cpp"
            "src/rainbow/input.cpp"
            "src/rainbow/shader.cpp"
            "src/rainbow/file.cpp"
            "src/rainbow/renderer.cpp"
            "src/rainbow/map/map.cpp"
            "src/rainbow/model/model.cpp"
            "src/rainbow/model/obj.cpp")

include_directories("include")
include_directories("testing/gtest/include")
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})

add_executable(rainbow ${sources})
target_link_libraries(rainbow 
                      ${SDL2_LIBRARY} 
                      ${OPENGL_LIBRARIES} 
                      ${GLEW_LIBRARY}
                      gtest)
