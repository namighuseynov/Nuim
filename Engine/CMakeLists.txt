cmake_minimum_required(VERSION 3.22.1)
include (../Config.cmake)

project(Engine)
set(LIBRARY_OUTPUT_PATH ${SOLUTION_DIR}/bin)
file(GLOB_RECURSE ENGINE_SOURCE_CPP_FILES "src/*.cpp")
file(GLOB_RECURSE ENGINE_SOURCE_C_FILES "src/*.c")

add_library(${PROJECT_NAME}
    ${ENGINE_SOURCE_CPP_FILES}
    ${ENGINE_SOURCE_C_FILES}
)

target_link_libraries(${PROJECT_NAME}
    glfw
    GL
)
