cmake_minimum_required(VERSION 3.22.1)
include (../Config.cmake)

project(Nuim)
file(GLOB_RECURSE NUIM_SOURCE_CPP_FILES "src/*.cpp")
file(GLOB_RECURSE NUIM_SOURCE_C_FILES "src/*.c")
add_executable(${PROJECT_NAME}
    ${NUIM_SOURCE_CPP_FILES}
    ${NUIM_SOURCE_C_FILES}
)
set(EXECUTABLE_OUTPUT_PATH ${SOLUTION_DIR}/bin)

target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${SOLUTION_DIR}/Engine/src
    )
target_link_directories(${PROJECT_NAME}
    PRIVATE
        ${SOLUTION_DIR}/bin
    )
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Engine
        glfw
        GL
    )


install(TARGETS ${PROJECT_NAME} DESTINATION bin)