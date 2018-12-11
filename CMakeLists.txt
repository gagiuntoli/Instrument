
cmake_minimum_required(VERSION 3.0)

project(Instrument VERSION 0.1 LANGUAGES C)

# Include Directories (for all targets)
include_directories(include ${CMAKE_BINARY_DIR})

# wildcard all the sources in src
file(GLOB SOURCESLIB src/*.c)

add_library(instrument ${SOURCESLIB})
target_link_libraries(instrument m)

enable_testing()
add_subdirectory(test)
