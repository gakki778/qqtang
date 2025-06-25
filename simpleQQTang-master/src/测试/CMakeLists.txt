cmake_minimum_required(VERSION 3.10)
project(QQTangGame)
set(CMAKE_CXX_STANDARD 14)
file(GLOB SOURCES "*.cpp")
# Exclude 源.cpp and 计分.cpp from the main build to avoid multiple main() definitions
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/源.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/计分.cpp")
add_executable(QQTangGame ${SOURCES})

# Add include directories for dependencies
include_directories(/usr/include/freetype2)
include_directories(/usr/include/SOIL)
include_directories(/usr/include/GLFW)

# Link libraries
find_package(OpenGL REQUIRED)
target_link_libraries(QQTangGame GL SDL2 SDL2_image SDL2_mixer GLEW glfw SOIL freetype)
