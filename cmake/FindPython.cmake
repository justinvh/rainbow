# Public Domain by Justin Van Horne <justinvh@gmail.com>
exec_program(python3.3-config 
    ARGS --includes 
    OUTPUT_VARIABLE PYTHON_INCLUDE_DIR)

exec_program(python3.3-config 
    ARGS --ldflags
    OUTPUT_VARIABLE PYTHON_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PYTHON 
    DEFAULT_MSG PYTHON_INCLUDE_DIR PYTHON_LIBRARY)

mark_as_advanced(PYTHON_INCLUDE_DIR)
mark_as_advanced(PYTHON_LIBRARY)
