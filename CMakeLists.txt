cmake_minimum_required(VERSION 3.20)

project(edge-ai-cpp-lab LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(
        /W4
        /permissive-
        /utf-8
    )
endif()

# 创建静态库目标
add_library(libtensor STATIC
    src/Matrix.cpp
    src/Tensor.cpp
)

# 把 include 设为 PUBLIC 导出：只要链接了 libtensor，自动获得 include 搜索路径！
target_include_directories(libtensor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(example src/main.cpp)
target_link_libraries(example PRIVATE libtensor)

add_executable(unit_test tests/TensorBufferTest.cpp)
target_link_libraries(unit_test PRIVATE libtensor)
add_test(NAME unit_test COMMAND unit_test)

add_executable(segfault_test tests/segfault_test.cpp)
target_link_libraries(segfault_test PRIVATE libtensor)

add_executable(benchmark benchmark/benchmark.cpp)
target_link_libraries(benchmark PRIVATE libtensor)



