cmake_minimum_required(VERSION 3.3)
project(giflib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SRC lib/*.c)
file(GLOB HDR lib/*.h)

include_directories(lib util)

add_library(giflib SHARED ${SRC} ${HDR})

add_executable(giftool util/giftool.c util/qprintf.c)
target_link_libraries(giftool giflib)

add_executable(gifhisto util/gifhisto.c util/qprintf.c util/getarg.c)
target_link_libraries(gifhisto giflib)

add_executable(giftext util/giftext.c util/qprintf.c util/getarg.c)
target_link_libraries(giftext giflib)

add_executable(gifsponge util/gifsponge.c util/qprintf.c util/getarg.c)
target_link_libraries(gifsponge giflib)

add_executable(gif2rgb util/gif2rgb.c util/qprintf.c util/getarg.c)
target_link_libraries(gif2rgb giflib)

add_executable(gifwedge util/gifwedge.c util/qprintf.c util/getarg.c)
target_link_libraries(gifwedge giflib)

add_executable(gifinto util/gifinto.c util/qprintf.c util/getarg.c)
target_link_libraries(gifinto giflib)

add_executable(giffix util/giffix.c util/qprintf.c util/getarg.c)
target_link_libraries(giffix giflib)

add_executable(gifbg util/gifbg.c util/qprintf.c util/getarg.c)
target_link_libraries(gifbg giflib)

add_executable(giffilter util/giffilter.c util/qprintf.c util/getarg.c)
target_link_libraries(giffilter giflib)

add_executable(gifbuild util/gifbuild.c util/qprintf.c util/getarg.c)
target_link_libraries(gifbuild giflib)

add_executable(gifclrmp util/gifclrmp.c util/qprintf.c util/getarg.c)
target_link_libraries(gifclrmp giflib)

add_executable(gifcolor util/gifcolor.c util/qprintf.c util/getarg.c)
target_link_libraries(gifcolor giflib)

add_executable(gifecho util/gifecho.c util/qprintf.c util/getarg.c)
target_link_libraries(gifecho giflib)