
cmake_minimum_required (VERSION 3.8)

project ("EvolutionSim")

# Include sub-projects.
add_subdirectory ("vendor/glfw")
add_subdirectory ("vendor/glm")
add_subdirectory ("renderer")

add_subdirectory ("asexual_evo_sim")
