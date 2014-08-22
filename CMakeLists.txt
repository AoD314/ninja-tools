cmake_minimum_required(VERSION 2.8.12)

project(ninja-tools)

# generic
add_compile_options(-std=c11)

# DEBUG
add_compile_options(-Wall -Wextra -Wpedantic -Wformat=2 -g3 -ggdb3 -O0)

# RELEASE
#add_compile_options(-O3)

# find deps libs
find_package(GTest REQUIRED)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})

###############################################################################

include_directories("parser/include")

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
    "viz/main.c"
    "viz/config.c"
    "viz/config.h"
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
