cmake_minimum_required(VERSION 2.8.12)

project(ninja-build-visualize C)

add_compile_options(-Wall -Wextra -Wpedantic -Wformat=2 -std=c11 -O0 -g3)

file(GLOB srcs "src/*.c")
file(GLOB hdrs "include/*.h")
file(GLOB tsts "tests/*.c")

include_directories("include")
add_executable(nbv ${srcs} ${hdrs})

add_executable(tests ${tsts})
