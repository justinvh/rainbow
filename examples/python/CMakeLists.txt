project(python)
find_package(PythonLibs REQUIRED)

set(python_interpreter "python_interpreter.cpp")

add_executable(python_interpreter ${python_interpreter})
target_link_libraries(python_interpreter rainbow)
