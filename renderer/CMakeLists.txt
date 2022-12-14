# CMakeList.txt : CMake project for Triangle, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project("renderer")

set(SOURCES
    ${IMGUI_SOURCES}
    "phm_buffer.cpp"
    "phm_buffer.h"
    "phm_descriptor.cpp"
    "phm_descriptor.h"
    "phm_model.h" 
    "phm_model.cpp" 
    "phm_window.h" 
    "phm_window.cpp" 
    "phm_swapchain.h" 
    "phm_swapchain.cpp" 
    "phm_pipeline.h" 
    "phm_pipeline.cpp" 
    "phm_device.h" 
    "phm_device.cpp" 
    "phm_model.h" 
    "phm_model.cpp" 
    "phm_object.h" 
    "phm_renderer.h" 
    "phm_renderer.cpp"
)

# Add source to this project's executable.
add_library (${PROJECT_NAME} STATIC ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PUBLIC -DImTextureID=ImU64)

set_target_properties(${PROJECT_NAME} 
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ../logger/
	PUBLIC $ENV{VULKAN_SDK}/Include/ 
	PUBLIC ../vendor/glfw/include/
    PUBLIC ../vendor/glm/
    PUBLIC ../vendor/spdlog/include
    PUBLIC ../vendor/imgui/
	PUBLIC ../vendor/imgui/backends/
)

target_link_directories(${PROJECT_NAME}
    PRIVATE ../logger/
	PRIVATE ../vendor/glfw/src
    PRIVATE $ENV{VULKAN_SDK}/Lib
)
 
target_link_libraries(${PROJECT_NAME}
	glfw
    vulkan-1
    logger
)
