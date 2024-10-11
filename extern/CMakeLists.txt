# GLFW lib
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(GLFW_BUILD_WAYLAND OFF)
endif ()

add_subdirectory(glfw)

# GLAD lib
add_subdirectory (glad)

# Portaudio lib
add_subdirectory (portaudio)