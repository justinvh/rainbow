# Finds V8

find_path(V8_INCLUDE_DIR v8.h
          HINTS ${V8_INCLUDEDIR} ${V8_INCLUDE_DIRS}
          PATH_SUFFIXES v8)

find_library(V8_LIBRARY
    NAMES V8
    HINTS
    $ENV{V8DIR}
    PATH_SUFFIXES lib64 lib
    PATHS ${V8_SEARCH_PATHS})

set(V8_INCLUDE_DIRS ${V8_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(V8 DEFAULT_MSG V8_INCLUDE_DIR V8_LIBRARY)

mark_as_advanced(V8_INCLUDE_DIR)
mark_as_advanced(V8_LIBRARY)
