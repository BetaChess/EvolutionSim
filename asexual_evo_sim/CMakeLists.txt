# CMakeList.txt : CMake project for Triangle, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project("asexual_sim")

set(SOURCES
    ${IMGUI_SOURCES}
    "main.cpp"
    "pch.h"
    "sim_app.cpp"
	"sim_app.h"
	"creature_render_system.cpp"
    "creature_render_system.h"
    "simulator.h" 
    "simulator.cpp" "frame_info.h")

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${SOURCES})

if (${CXXVER} GREATER_EQUAL 20) 
    add_compile_definitions(CXXGT20=${CXXVER})
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC -DImTextureID=ImU64)

set_target_properties(${PROJECT_NAME} 
    PROPERTIES
        CXX_STANDARD ${CXXVER}
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ../logger/
	PUBLIC $ENV{VULKAN_SDK}/Include/ 
	PUBLIC ../vendor/glfw/include/
    PUBLIC ../vendor/glm/
	PUBLIC ../renderer/
    PUBLIC ../vendor/imgui/
	PUBLIC ../vendor/imgui/backends/
)

target_link_directories(${PROJECT_NAME}
    PRIVATE ../logger/
	PRIVATE ../vendor/glfw/src
    PRIVATE $ENV{VULKAN_SDK}/Lib
	PRIVATE ../renderer/
)
 
target_link_libraries(${PROJECT_NAME}
	glfw
    vulkan-1
	renderer
    logger
)

# Precompiled header
target_precompile_headers(${PROJECT_NAME}
    PUBLIC "pch.h"
)

# TODO: Add tests and install targets if needed.

############## Build SHADERS #######################
 
# Find all vertex and fragment sources within shaders directory
# taken from VBlancos vulkan tutorial
# https://github.com/vblanco20-1/vulkan-guide/blob/all-chapters/CMakeLists.txt
find_program(GLSL_VALIDATOR glslangValidator HINTS 
  ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} 
  /usr/bin 
  /usr/local/bin 
  ${VULKAN_SDK_PATH}/Bin
  ${VULKAN_SDK_PATH}/Bin32
  $ENV{VULKAN_SDK}/Bin/ 
  $ENV{VULKAN_SDK}/Bin32/
)
 
# get all .vert and .frag files in shaders directory
file(GLOB_RECURSE GLSL_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/shaders/*.frag"
  "${PROJECT_SOURCE_DIR}/shaders/*.vert"
)
 
foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)
 
add_custom_target(
    ${PROJECT_NAME}_Shaders
    SOURCES ${GLSL_SOURCE_FILES}
    DEPENDS ${SPIRV_BINARY_FILES}
)

add_custom_command(
    TARGET ${PROJECT_NAME}_Shaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory              
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMENT "Copying shaders" VERBATIM 
    )

ADD_DEPENDENCIES(${PROJECT_NAME} ${PROJECT_NAME}_Shaders)