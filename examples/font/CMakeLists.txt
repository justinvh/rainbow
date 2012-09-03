project(font)

set(font_rendering_test "font_rendering_test.cpp")
add_executable(font_rendering_test ${font_rendering_test})
target_link_libraries(font_rendering_test rainbow)
