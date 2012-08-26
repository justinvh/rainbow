project(model)

set(model_obj_loading "model_obj_loading.cpp")
add_executable(model_obj_loading ${model_obj_loading})
target_link_libraries(model_obj_loading rainbow)
