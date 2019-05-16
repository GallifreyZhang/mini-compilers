cmake_minimum_required(VERSION 3.13)
project(Compiler C)

set(CMAKE_C_STANDARD 11)
# build目录：${CMAKE_CURRENT_BINARY_DIR}
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
SET(CMAKE_BUILE_TYPE DEBUG)
SET(CMAKE_C_FLAGS_DEBUG "-g -Wall")
SET(SOURCE_FILES                                  #设置变量，表示所有的源文件
        main.c
        parser.c
        utils.c
        header_obj.c)

include_directories(.)

install(FILES "source.txt" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(Compiler ${SOURCE_FILES})


