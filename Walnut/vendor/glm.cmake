set(glm_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm)

file(
        GLOB_RECURSE glm_SOURCES
        ${glm_SOURCE_DIR}/glm/*.cpp
)

add_library(
        glm
        ${glm_SOURCES}
)

target_include_directories(
        glm
        PUBLIC ${glm_SOURCE_DIR}
)
