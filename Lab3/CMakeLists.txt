cmake_minimum_required(VERSION 3.12)
project(Lab3)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(Lab3 main.cpp Refnteinr.h Refnteinr_impl.h Info.cpp Info.h Lexer.cpp Lexer.h copyright.cpp copyright.h Builder.cpp Builder.h)