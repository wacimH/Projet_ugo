cmake_minimum_required(VERSION 4.3)
project(Projet_ugo C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Projet_ugo
        testing.c
        testing.h
        Main.c)
