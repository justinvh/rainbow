cmake_minimum_required(VERSION 2.8)
project(rainbow)

option(rainbow_examples "Build the Rainbow examples." OFF)
option(rainbow_tests "Run the Rainbow tests." OFF)

# CMAKE extensions
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${rainbow_SOURCE_DIR}/cmake")

# Required packages
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(Lua51 REQUIRED)

# Warning pedantic flags for all
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic ${CMAKE_CXX_FLAGS}")

# Dependencies and include locations
include_directories("include")
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2TTF_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories("ext/oolua/include")

# OOLua Sources
set(lua_sources "ext/oolua/src/class_from_stack.cpp"
                "ext/oolua/src/lua_function.cpp"
                "ext/oolua/src/lua_ref.cpp"
                "ext/oolua/src/lua_stack_dump.cpp"
                "ext/oolua/src/lua_table.cpp"
                "ext/oolua/src/oolua.cpp"
                "ext/oolua/src/oolua_check_result.cpp"
                "ext/oolua/src/oolua_error.cpp"
                "ext/oolua/src/oolua_push_pull.cpp"
                "ext/oolua/src/oolua_registration.cpp"
                "ext/oolua/src/oolua_storage.cpp"
                "ext/oolua/src/push_pointer_internal.cpp")

# Rainbow Sources
set(sources "src/rainbow/display/display.cpp"
            "src/rainbow/display/gui.cpp"
            "src/rainbow/input.cpp"
            "src/rainbow/file.cpp"
            "src/rainbow/renderer/shader.cpp"
            "src/rainbow/renderer/renderer.cpp"
            "src/rainbow/renderer/camera.cpp"
            "src/rainbow/map/map.cpp"
            "src/rainbow/model/model.cpp"
            "src/rainbow/model/obj.cpp"
            "src/rainbow/math/random.cpp"
)

# Construct the actual engine library
add_library(rainbow ${sources})
target_link_libraries(rainbow
                      ${SDL2_LIBRARY}
                      ${SDL2TTF_LIBRARY}
                      ${OPENGL_LIBRARIES}
                      ${GLEW_LIBRARY}
                      ${LUA_LIBRARIES}
                      ${FREETYPE_LIBRARIES})

# Build the examples
if(${rainbow_examples})
    add_subdirectory(examples)
endif()

# Build the tests
if(${rainbow_tests})
    add_subdirectory(testing)
endif()
