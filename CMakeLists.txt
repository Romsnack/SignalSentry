cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

project(SignalSentry VERSION 1.0)

set(SRC
    ${PROJECT_SOURCE_DIR}/main.cpp
)

add_executable(
    SignalSentry
    ${SRC}
)

target_include_directories(SignalSentry PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(SignalSentry PUBLIC -g3 -ggdb)