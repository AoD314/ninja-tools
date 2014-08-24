cmake_minimum_required(VERSION 2.8.12)

project(ninja-tools)

# generic
add_compile_options(-std=c11)

# DEBUG
add_compile_options(-Wall -Wextra -Wpedantic -Wformat=2 -g3 -ggdb3 -O0)

#add_compile_options(-fcolor-diagnostics) # for clang

# RELEASE
#add_compile_options(-O3)

# find deps libs
find_package(GTest REQUIRED)

# find SDL2
FIND_PATH(SDL2_INCLUDE_DIR SDL.h
HINTS
$ENV{SDL2DIR}
PATH_SUFFIXES
include/SDL2 include
PATHS
/usr/local/include/SDL2
/usr/include/SDL2
/opt
)

IF(NOT APPLE)
    FIND_PACKAGE(Threads)
ENDIF(NOT APPLE)

FIND_LIBRARY(SDL2_LIBRARY
NAMES SDL2
HINTS
$ENV{SDL2DIR}
PATH_SUFFIXES lib64 lib
PATHS
/opt/local
)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2 REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR)

SET(SDL2_FOUND "YES")

###############################################################################

include_directories("parser/include")
include_directories("table/include")

set(src_parse_lib
    "parser/include/project_types.h"
    "parser/include/record.h"
    "parser/include/parse.h"
    "parser/src/record.c"
    "parser/src/parse.c"
)

set(src_stat_app
    "stat/main.c"
)

set(src_viz_app
    "table/include/node.h"
    "table/include/table.h"
    "table/src/node.c"
    "table/src/table.c"
    "viz/main.c"
    "viz/config.c"
    "viz/config.h"
    "viz/block.c"
    "viz/block.h"
)

# LIBS
add_library(ninja_parse_lib STATIC ${src_parse_lib})

# NINJA-STAT
add_executable(ninja-stat ${src_stat_app})
target_link_libraries(ninja-stat ninja_parse_lib)


# NINJA-VIZ
add_executable(ninja-viz ${src_viz_app})
target_link_libraries(ninja-viz ninja_parse_lib ${SDL2_LIBRARIES} -lSDL2_ttf)


# TESTS
file(GLOB src_tests "tests/*.c" "tests/*.cpp" )
add_subdirectory("3rdparty")
add_executable(unittests ${src_tests})
target_link_libraries(unittests gtest ninja_parse_lib)
