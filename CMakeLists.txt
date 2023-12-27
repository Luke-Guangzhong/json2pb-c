cmake_minimum_required(VERSION 3.18)

project(json2pb-c)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(main test/main.c)

execute_process(COMMAND protoc --c_out=${PROJECT_SOURCE_DIR}/src/ -I ${PROJECT_SOURCE_DIR}/proto/ msg.proto)

if(EXISTS ${PROJECT_SOURCE_DIR}/src/msg.pb-c.h)
    execute_process(COMMAND mv ${PROJECT_SOURCE_DIR}/src/msg.pb-c.h  ${PROJECT_SOURCE_DIR}/include )
else()
    return()
endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include
                    /usr/local/include/protobuf-c)

set(UTILITIES_SOURCES ${PROJECT_SOURCE_DIR}/src/utilities.c)
add_library(Utilities ${UTILITIES_SOURCES})

set(SOURCES ${PROJECT_SOURCE_DIR}/src/cvt.c
            ${PROJECT_SOURCE_DIR}/src/msg.pb-c.c
            ${PROJECT_SOURCE_DIR}/src/cJSON.c)
add_compile_options(-DDEBUG_ENABLE)
add_library(json2pb ${SOURCES})
target_link_libraries(json2pb   libprotobuf-c.a
                                Utilities
)
enable_testing()