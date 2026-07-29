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

if(EMSCRIPTEN)
    add_executable(doubleslit_wasm
        src/bindings.cpp
    )

    target_link_libraries(doubleslit_wasm
        PRIVATE doubleslit_core
    )

    target_link_options(doubleslit_wasm PRIVATE
        "--bind"
        "-sMODULARIZE=1"
        "-sEXPORT_NAME=createDoubleSlitModule"
        "-sENVIRONMENT=web"
    )

    set_target_properties(doubleslit_wasm PROPERTIES
        SUFFIX ".js"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/web"
    )
endif()