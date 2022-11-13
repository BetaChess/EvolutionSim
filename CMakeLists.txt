
cmake_minimum_required (VERSION 3.8)

project ("EvolutionSim")

set(CXXVER 20)

# IMGUI Sources needed
set(IMGUI_DIR "../vendor/imgui")
set(IMGUI_SOURCES ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp)


# Include sub-projects.
add_subdirectory ("vendor/glfw")
add_subdirectory ("vendor/glm")
add_subdirectory ("logger")
add_subdirectory ("renderer")

add_subdirectory ("asexual_evo_sim")
