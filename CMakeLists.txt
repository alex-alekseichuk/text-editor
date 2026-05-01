cmake_minimum_required(VERSION 3.28)
project(text_editor C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(text_editor
        buffer.c
        controller.c
        file.c
        line.c
        main.c
        view.c)

target_link_libraries(text_editor ncurses tinfo)
