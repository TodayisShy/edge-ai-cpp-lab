cmake_minimum_required(VERSION 3.20)

project(edge-ai-cpp-lab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(edge-ai-cpp-lab
    src/main.cpp
    src/Matrix.cpp
    src/Tensor.cpp
)

target_include_directories(edge-ai-cpp-lab PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(edge-ai-cpp-lab PRIVATE
        /W4
        /permissive-
        /utf-8
    )
endif()
