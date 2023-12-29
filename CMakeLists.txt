cmake_minimum_required(VERSION 3.18)

project(json2pb-c)

message("c_CMAKE_BINARY_DIR:          " ${CMAKE_BINARY_DIR} )
message("c_CMAKE_CURRENT_SOURCE_DIR:  " ${CMAKE_CURRENT_SOURCE_DIR} )
message("c_CMAKE_SOURCE_DIR:          " ${CMAKE_SOURCE_DIR} )

include(ExternalProject)

# cJSON external dependency
#-------------------------------------------------------------------------------
ExternalProject_Add(cJSON
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/_prefix/cJSON
    GIT_REPOSITORY git@github.com:DaveGamble/cJSON.git
    GIT_TAG "v1.7.17"
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/build -DBUILD_SHARED_AND_STATIC_LIBS=On
    )
add_library(libcJSON STATIC  IMPORTED)
add_dependencies(libcJSON cJSON)

set(CMAKE_C_FLAGS "-std=c99")
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_compile_options(-DDEBUG_ENABLE)

execute_process(COMMAND protoc --c_out=${PROJECT_SOURCE_DIR}/src/ -I ${PROJECT_SOURCE_DIR}/proto/ msg.proto)
if(EXISTS ${PROJECT_SOURCE_DIR}/src/msg.pb-c.h)
    execute_process(COMMAND mv ${PROJECT_SOURCE_DIR}/src/msg.pb-c.h  ${PROJECT_SOURCE_DIR}/include )
else()
    return()
endif()


include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/build/include)
link_directories(   ${PROJECT_SOURCE_DIR}/lib
                    ${PROJECT_SOURCE_DIR}/build/lib)

set(UTILITIES_SOURCES ${PROJECT_SOURCE_DIR}/src/utilities.c)
add_library(Utilities ${UTILITIES_SOURCES})

set(SOURCES ${PROJECT_SOURCE_DIR}/src/cvt.c
            ${PROJECT_SOURCE_DIR}/src/msg.pb-c.c
            ${PROJECT_SOURCE_DIR}/src/msg.c)
add_library(json2pb ${SOURCES})
target_link_libraries(json2pb   libprotobuf-c.a
                                Utilities
                                cjson)

add_executable(main test/main.c)
target_link_libraries(main  json2pb
                            libprotobuf-c.a
                            Utilities
                            cjson) 

enable_testing()
add_test(NAME J2P COMMAND main -j${PROJECT_SOURCE_DIR}/resource/Request.json -mj2r) 