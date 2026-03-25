cmake_minimum_required(VERSION 3.16)
project(libft C)

file(GLOB LIBFT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

add_library(ft STATIC ${LIBFT_SRCS})
target_include_directories(ft PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
