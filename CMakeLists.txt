cmake_minimum_required(VERSION 3.10)
project(TensorPool CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(tensorpool
)

add_executable(example example/main.cpp)
target_link_libraries(example tensorpool)
