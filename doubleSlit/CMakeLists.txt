cmake_minimum_required(VERSION 3.20)

project(DoubleSlit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(doubleslit_core
    src/DoubleSlitEngine.cpp
)

target_include_directories(doubleslit_core
    PUBLIC include
)

add_executable(native_smoke
    tests/native_smoke.cpp
)

target_link_libraries(native_smoke
    PRIVATE doubleslit_core
)