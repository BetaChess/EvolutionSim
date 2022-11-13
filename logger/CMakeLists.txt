# CMakeList.txt : CMake project for Triangle, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project("logger")

set(SOURCES
    "logger.h"
    "logger.cpp"
)

# Add source to this project's executable.
add_library (${PROJECT_NAME} STATIC ${SOURCES})

set_target_properties(${PROJECT_NAME} 
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ../vendor/spdlog/include
)
