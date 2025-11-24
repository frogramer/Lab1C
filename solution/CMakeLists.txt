cmake_minimum_required(VERSION 4.0)
project(assignment-1-image-transform-master C)

set(CMAKE_C_STANDARD 17)


set(SOURCES
        src/main.c
        src/image.c
        src/bmp_io.c
        src/transform.c
)


set(HEADERS
        include/image.h
        include/bmp_io.h
        include/transform.h
)


add_executable(image-transform ${SOURCES} ${HEADERS})


target_include_directories(image-transform PRIVATE include)