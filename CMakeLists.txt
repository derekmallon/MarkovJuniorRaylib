# Project
cmake_minimum_required (VERSION 3.22)
project (markov)

# Libraries
find_package(raylib CONFIG REQUIRED)

# Linking
add_executable(${PROJECT_NAME}
    "main.cpp"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set(CMAKE_BUILD_TYPE Debug)

target_compile_options(${PROJECT_NAME} PRIVATE -g -fprofile-instr-generate -fstandalone-debug -fno-limit-debug-info)
target_link_libraries(${PROJECT_NAME}
  PRIVATE raylib
)

