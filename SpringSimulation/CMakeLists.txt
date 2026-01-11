cmake_minimum_required(VERSION 3.28)
project(SpringSimulation LANGUAGES CXX)

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
        SYSTEM
    )
    FetchContent_MakeAvailable(SFML)
else()
    message(STATUS "Using system-installed SFML")
endif()

add_executable(simulation src/main.cpp)
target_compile_features(simulation PRIVATE cxx_std_17)

target_link_libraries(simulation PRIVATE SFML::Graphics)
