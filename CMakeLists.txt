cmake_minimum_required(VERSION 3.16)
project(hack_embedded)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
find_package(Boost REQUIRED)

#Object files holding recompiled Hack assembly
SET(OBJS
        ${CMAKE_CURRENT_SOURCE_DIR}/hack_add.o
        )

add_executable(hack_embedded ${OBJS} main.cpp)

#Mark object files as already-built source to be linked into the target
SET_SOURCE_FILES_PROPERTIES(
        ${OBJS}
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
)

target_link_libraries(hack_embedded sfml-graphics pthread Boost::headers)