cmake_minimum_required(VERSION 3.28)
project(Simulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/test)

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

add_executable(
  simulation
  src/main.cpp
  src/maze.cpp
  src/maze_renderer.cpp
  src/maze_generator.cpp
  src/maze_solver.cpp
)

target_include_directories(simulation PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(simulation PRIVATE SFML::Graphics)

# Compile Test Program
add_executable(
  test_solve_times
  test/test_solve_times.cpp
  src/maze.cpp
  src/maze_renderer.cpp
  src/maze_generator.cpp
  src/maze_solver.cpp
)

set_target_properties(test_solve_times PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

target_compile_features(test_solve_times PRIVATE cxx_std_17)
target_link_libraries(test_solve_times PRIVATE SFML::Graphics)

# Compile Test Program
add_executable(
  test_generate_times
  test/test_generate_times.cpp
  src/maze.cpp
  src/maze_renderer.cpp
  src/maze_generator.cpp
  src/maze_solver.cpp
)

set_target_properties(test_generate_times PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

target_compile_features(test_generate_times PRIVATE cxx_std_17)
target_link_libraries(test_generate_times PRIVATE SFML::Graphics)
