if (NOT TARGET glfw)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(GLFW)
endif ()

if (NOT TARGET glm)
    include(glm.cmake)
endif ()

if (NOT TARGET imgui)
    include(imgui.cmake)
endif ()

if (NOT TARGET stb)
    include(stb.cmake)
    set_target_properties(stb PROPERTIES LINKER_LANGUAGE CXX)
endif ()

