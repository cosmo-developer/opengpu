cmake_minimum_required(VERSION 3.10)

project(opengpu)

add_subdirectory("third-party/glm")

include_directories("third-party")

add_executable(opengpu "test.cpp" "third-party/src/glad.c" "third-party/src/stb.cpp")

target_link_libraries(opengpu opengl32 glm)