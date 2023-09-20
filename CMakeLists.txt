cmake_minimum_required(VERSION 3.25)
project(Tarea_Damas)

set(CMAKE_CXX_STANDARD 17)

add_executable(Tarea_Damas main.cpp table.h Graphics.h)

target_link_libraries(Tarea_Damas -lOpenGL32 -lfreeGLUT -lglu32)

include_directories(${SOIL2_INCLUDE_DIR})

