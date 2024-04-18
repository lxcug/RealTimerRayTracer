set(stb_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stb)

file(
        GLOB stb_SOURCES
        ${stb_SOURCE_DIR}/*.h
)

add_library(
        stb
        INTERFACE ${stb_SOURCES}
)

target_include_directories(
        stb
        INTERFACE ${stb_SOURCE_DIR}
)
