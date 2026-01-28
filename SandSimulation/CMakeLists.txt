cmake_minimum_required(VERSION 3.28)
project(Simulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Prefer system-installed SFML if available
find_package(SFML 3 QUIET COMPONENTS Graphics)

if(NOT SFML_FOUND)
    message(STATUS "System SFML not found, fetching locally...")

    include(FetchContent)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(SFML)
else()
    message(STATUS "Using system-installed SFML")
endif()

add_executable(simulation src/main.cpp src/simulation.cpp src/elementType.cpp)
target_compile_features(simulation PRIVATE cxx_std_20)

target_link_libraries(simulation PRIVATE SFML::Graphics)
