cmake_minimum_required(VERSION 3.18)

project(json2pb-c)

set(CMAKE_C_FLAGS "-std=c99")

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


execute_process(COMMAND protoc --c_out=${PROJECT_SOURCE_DIR}/src/ -I ${PROJECT_SOURCE_DIR}/proto/ msg.proto)

if(EXISTS ${PROJECT_SOURCE_DIR}/src/msg.pb-c.h)
execute_process(COMMAND mv ${PROJECT_SOURCE_DIR}/src/msg.pb-c.h  ${PROJECT_SOURCE_DIR}/include )
else()
return()
endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(UTILITIES_SOURCES ${PROJECT_SOURCE_DIR}/src/utilities.c)
add_library(Utilities ${UTILITIES_SOURCES})

set(SOURCES ${PROJECT_SOURCE_DIR}/src/cvt.c
            ${PROJECT_SOURCE_DIR}/src/msg.pb-c.c
            ${PROJECT_SOURCE_DIR}/src/cJSON.c
            ${PROJECT_SOURCE_DIR}/src/msg.c)
add_compile_options(-DDEBUG_ENABLE)
add_library(json2pb ${SOURCES})
target_link_libraries(json2pb   libprotobuf-c.a
                                Utilities
)

add_executable(main test/main.c)
target_link_libraries(main  json2pb
                            libprotobuf-c.a
                            Utilities) 

enable_testing()
add_test(NAME J2P COMMAND main -j${PROJECT_SOURCE_DIR}/resource/Request.json -mj2r) 