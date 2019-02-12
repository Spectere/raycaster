cmake_minimum_required(VERSION 3.13)
project(raycaster C)

set(CMAKE_C_STANDARD 90)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(raycaster
        main.c
        game.c
        log.c
        video.c
        event.c
        render.c
        player.c
        palette/palette_cga.c
        palette/palette.c)

target_link_libraries(raycaster m ${SDL2_LIBRARIES})
