cmake_minimum_required(VERSION 3.18)

project(json2pb-c)

message("c_CMAKE_BINARY_DIR:          " ${CMAKE_BINARY_DIR} )
message("c_CMAKE_CURRENT_SOURCE_DIR:  " ${CMAKE_CURRENT_SOURCE_DIR} )
message("c_CMAKE_SOURCE_DIR:          " ${CMAKE_SOURCE_DIR} )

include(ExternalProject)

ExternalProject_Add(cJSON
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/_prefix/cJSON
    GIT_REPOSITORY git@gitee.com:luke_guangzhong/cJSON.git
    GIT_TAG "87d8f0961a01bf09bef98ff89bae9fdec42181ee"
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/build -DBUILD_SHARED_AND_STATIC_LIBS=On
    )
add_library(libcJSON STATIC  IMPORTED)
add_dependencies(libcJSON cJSON)

ExternalProject_Add(protobuf-c
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/_prefix/protobuf-c
    GIT_REPOSITORY git@gitee.com:luke_guangzhong/protobuf-c.git
    GIT_TAG "8c201f6e47a53feaab773922a743091eb6c8972a"
    CONFIGURE_COMMAND COMMAND <SOURCE_DIR>/autogen.sh && <SOURCE_DIR>/configure
                                                     --prefix=${CMAKE_SOURCE_DIR}/build
                                                     --enable-shared
                                                     --enable-static
                                                     --disable-protoc
	BUILD_IN_SOURCE 1
)
add_library(libprotobuf-c STATIC SHARED IMPORTED)
add_dependencies(libprotobuf-c protobuf-c)

ExternalProject_Add(base64
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/_prefix/base64
    GIT_REPOSITORY git@gitee.com:luke_guangzhong/base64.git
    GIT_TAG "develop"
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/build
    )
add_library(libbase64 STATIC  IMPORTED)
add_dependencies(libbase64 base64)

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
    message(FATAL_ERROR "Failed to generate msg.pb-c.h")
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
                                libcjson.a
                                libbase64.a)

add_executable(main test/main.c)
target_link_libraries(main  json2pb
                            libprotobuf-c.a
                            Utilities
                            libcjson.a
                            libbase64.a)

enable_testing()
add_test(NAME J2P COMMAND main -j${PROJECT_SOURCE_DIR}/resource/Request.json -mj2r) 