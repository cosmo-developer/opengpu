cmake_minimum_required(VERSION 3.10)

project(opengpu)

add_subdirectory("third-party/glfw");
add_subdirectory("third-party/glm")

include_directories("third-party" "third-party/glm" "third-party/glfw/include")


target_link_libraries(opengpu opengl32  glm)