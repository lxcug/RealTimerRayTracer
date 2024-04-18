file(
        GLOB_RECURSE walnut_src
        ${CMAKE_CURRENT_SOURCE_DIR}/Walnut/src/Walnut/*.cpp
)
file(
        GLOB walnut_app_src
        ${CMAKE_CURRENT_SOURCE_DIR}/WalnutApp/src/*.cpp
)
file(
        GLOB_RECURSE walnut_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/Walnut/src/Walnut/*.h
)

find_package(Vulkan REQUIRED)

add_library(walnut
        ${walnut_src}
        ${walnut_headers}
)

target_compile_definitions(
        walnut
        PUBLIC WL_PLATFORM_WINDOWS
)

target_include_directories(walnut
        PUBLIC ./
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Walnut/src
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/WalnutApp/src
        PUBLIC ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
        walnut
        PUBLIC glfw
        PUBLIC stb
        PUBLIC imgui
        PUBLIC glm
        PUBLIC Dwmapi.lib
        PUBLIC ${Vulkan_LIBRARIES}
)

add_subdirectory(vendor)
