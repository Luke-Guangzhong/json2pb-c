cmake_minimum_required(VERSION 3.22)

project(json2pb-c)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(main test/main.c)