cmake_minimum_required(VERSION 3.5)

project(CoordTrans LANGUAGES C)
set(HEADERS prototypes.h)

add_executable(CoordTrans main.c ${HEADERS})


target_link_libraries(CoordTrans m)
