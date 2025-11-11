cmake_minimum_required(VERSION 3.19)

project(project_name_here)

# Changable C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS YES)

add_compile_options(-Wall -Wextra -pedantic)

# Append the cmake submodules folder 
if(EXISTS "${CMAKE_SOURCE_DIR}/cmake")
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

# Build the core library
add_library(core STATIC src/core/app.cpp)
target_include_directories(core PRIVATE "${CMAKE_SOURCE_DIR}/src/core")

# Create the app executable linking the core library
add_executable(App src/main.cpp)
target_link_libraries(App PRIVATE core)

target_compile_options(App PRIVATE
    $<$<CONFIG:Debug>:-O0>
    $<$<CONFIG:Release>:-O3>
)

# Use the test folder if present
if(EXISTS "${CMAKE_SOURCE_DIR}/test")
    enable_testing()
    add_subdirectory(test)
else()
    message(INFO "Enable Testing is disabled.")
endif()