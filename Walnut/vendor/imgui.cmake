set(imgui_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

file(
        GLOB imgui_sources CONFIGURE_DEPENDS
        ${imgui_SOURCE_DIR}/*.cpp
)

file(
        GLOB imgui_impl CONFIGURE_DEPENDS
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)

find_package(Vulkan REQUIRED)

add_library(
        imgui STATIC
        ${imgui_sources}
        ${imgui_impl}
)

target_include_directories(
        imgui
        PUBLIC ${imgui_SOURCE_DIR}
        PUBLIC ${imgui_SOURCE_DIR}/backends
        PRIVATE ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
        imgui
        PRIVATE glfw
        PRIVATE ${Vulkan_LIBRARIES}
)   