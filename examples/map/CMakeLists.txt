project(map)

set(map_basic_loading "map_basic_loading.cpp")

add_executable(map_basic_loading ${map_basic_loading})
target_link_libraries(map_basic_loading rainbow)
