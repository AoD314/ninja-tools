project(gtest)

find_package(Threads)
set(CMAKE_CXX_FLAGS "")
include_directories(SYSTEM ".")

add_compile_options(-std=c++11)

add_library(gtest STATIC "gtest/gtest-all.cc" "gtest/gtest.h")
target_link_libraries(gtest ${CMAKE_THREAD_LIBS_INIT})
