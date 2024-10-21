# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(3drenderer VERSION 1.0)

# Enable generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify the C standard
set(CMAKE_C_STANDARD 99) # Change this if needed
set(CMAKE_C_STANDARD_REQUIRED True)

# Set the output directories for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ~/Desktop/3drenderer/build/bin)

# Include directories for header files
include_directories(
src/include
src/
)

# Add source files
set(SOURCES
    ~/Desktop/3drenderer/src/display.c
    ~/Desktop/3drenderer/src/main.c
    ~/Desktop/3drenderer/src/mesh.c
)

# Create the executable target
add_executable(renderer ~/Desktop/3drenderer/src/display.c ~/Desktop/3drenderer/src/main.c  ~/Desktop/3drenderer/src/mesh.c)

# Link any necessary libraries (e.g., SDL2)
target_link_libraries(renderer PRIVATE ~/Desktop/3drenderer/src/lib/SDL2.dll)

# Optionally set the library search path if you have other libraries
link_directories( ~/Desktop/3drenderer/build/lib)

# If you want to specify a specific output name for the executable
set_target_properties(renderer PROPERTIES OUTPUT_NAME "renderer")


