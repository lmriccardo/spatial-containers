cmake_minimum_required(VERSION 3.19)

project(spatial_container)

# Changable C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS YES)

add_compile_options(-Wall -Wextra -pedantic)

# Append the cmake submodules folder 
if(EXISTS "${CMAKE_SOURCE_DIR}/cmake")
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

    file(GLOB ALL_CMAKE_FILES "${CMAKE_MODULE_PATH}/*.cmake")
    foreach(FILE_PATH IN LISTS ALL_CMAKE_FILES)
        get_filename_component(BASENAME "${FILE_PATH}" NAME_WE)
        message(STATUS "Including CMake module: ${BASENAME}")
        include("${BASENAME}")
    endforeach()

endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

# target_compile_options(App PRIVATE
#     $<$<CONFIG:Debug>:-O0>
#     $<$<CONFIG:Release>:-O3>
# )

# Use the test folder if present
if(EXISTS "${CMAKE_SOURCE_DIR}/test")
    enable_testing()
    add_subdirectory(test)
else()
    message(INFO "Enable Testing is disabled.")
endif()