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
  3dsimulation
  src/main.cpp
  src/maze3d.cpp
  src/maze3d_renderer.cpp
)

target_include_directories(3dsimulation PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(3dsimulation PRIVATE SFML::Graphics)

# Compile Test Program
add_executable(
  test_maze
  test/test_maze3d.cpp
  src/maze3d.cpp
)

set_target_properties(test_maze PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

target_include_directories(test_maze PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_maze PRIVATE SFML::Graphics)

# Compile Test Program
add_executable(
  test_3dsolve_times
  test/test_3dsolve_times.cpp
  src/maze3d.cpp
  src/maze3d_renderer.cpp
  src/maze3d_generator.cpp
  src/maze3d_solver.cpp
)

set_target_properties(test_3dsolve_times PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

target_include_directories(test_3dsolve_times PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_3dsolve_times PRIVATE SFML::Graphics)

# Compile Test Program
add_executable(
  test_3dgenerate_times
  test/test_3dgenerate_times.cpp
  src/maze3d.cpp
  src/maze3d_renderer.cpp
  src/maze3d_generator.cpp
  src/maze3d_solver.cpp
)

set_target_properties(test_3dgenerate_times PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

target_include_directories(test_3dgenerate_times PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_3dgenerate_times PRIVATE SFML::Graphics)
