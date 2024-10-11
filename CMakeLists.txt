cmake_minimum_required (VERSION 3.22)

project (bx-platform-common)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Define options for window backend
set (BX_WINDOW_BACKEND "GLFW" CACHE STRING "Choose the window backend: GLFW")
set_property (CACHE BX_WINDOW_BACKEND PROPERTY STRINGS "GLFW" "Custom")
message ("BX window backend: ${BX_WINDOW_BACKEND}")

# Define options for graphics backend
set (BX_GRAPHICS_BACKEND "OpenGL" CACHE STRING "Choose the graphics backend: OpenGL, OpenGLES, Vulkan")
set_property (CACHE BX_GRAPHICS_BACKEND PROPERTY STRINGS "OpenGL" "OpenGLES" "Vulkan" "Custom")
message ("BX graphics backend: ${BX_GRAPHICS_BACKEND}")

# Define options for audio backend
set (BX_AUDIO_BACKEND "PortAudio" CACHE STRING "Choose the audio backend: PortAudio")
set_property (CACHE BX_AUDIO_BACKEND PROPERTY STRINGS "PortAudio" "Custom")
message ("BX audio backend: ${BX_AUDIO_BACKEND}")

# bx-platform-common lib
add_library(bx-platform-common SHARED)
target_link_libraries(bx-platform-common PRIVATE bx)
target_include_directories(bx-platform-common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(bx-platform-common PRIVATE BUILD_SHARED)
target_compile_definitions(bx-platform-common PUBLIC BX_PLATFORM_API=API) 

# Externals
add_subdirectory(extern)

set (BX_PLATFORM_COMMON_SRCS)
set (BX_PLATFORM_COMMON_LIBS)

# Configure the window backend based on selection
if (BX_WINDOW_BACKEND STREQUAL "GLFW")
    message(STATUS "Using GLFW as the window backend")
    target_compile_definitions(bx-platform-common PUBLIC BX_WINDOW_GLFW_BACKEND)

    target_sources(bx-platform-common PRIVATE
        "src/bx/platform/window_glfw.cpp"
        "src/bx/platform/input_glfw.cpp"
        "src/bx/platform/imgui_glfw.cpp"

        # ImGui GLFW (TODO: temporary solution until a fully custom solution is made)
        "../imgui/backends/imgui_impl_glfw.cpp"
    )
    target_link_libraries(bx-platform-common PRIVATE glfw)

elseif (BX_WINDOW_BACKEND STREQUAL "Custom")
    message(STATUS "Using a custom window backend")
    target_compile_definitions(bx-platform-common PUBLIC BX_WINDOW_CUSTOM_BACKEND)

else ()
    message(FATAL_ERROR "Unknown window backend: ${BX_WINDOW_BACKEND}")
endif ()


# Setup graphics backend
if (BX_GRAPHICS_BACKEND STREQUAL "OpenGL")
    message(STATUS "Using OpenGL as the graphics backend")
    target_compile_definitions(bx-platform-common PUBLIC BX_GRAPHICS_OPENGL_BACKEND)

    target_sources(bx-platform-common PRIVATE
        "src/bx/platform/graphics_opengl.cpp"
        "src/bx/platform/imgui_opengl.cpp"

        # ImGui OpenGL3 (TODO: temporary solution until a fully custom solution is made)
        "../imgui/backends/imgui_impl_opengl3.cpp"
    )

    target_link_libraries(bx-platform-common PRIVATE glad)

#elseif (BX_GRAPHICS_BACKEND STREQUAL "OpenGLES")
#    message(STATUS "Using OpenGLES as the graphics backend")
#    target_compile_definitions(bx-platform PUBLIC BX_GRAPHICS_OPENGLES_BACKEND)
#
#elseif (BX_GRAPHICS_BACKEND STREQUAL "Vulkan")
#    message(STATUS "Using Vulkan as the graphics backend")
#    target_compile_definitions(bx-platform PUBLIC BX_GRAPHICS_VULKAN_BACKEND)

elseif (BX_GRAPHICS_BACKEND STREQUAL "Custom")
    message(STATUS "Using a custom graphics backend")
    target_compile_definitions(bx PUBLIC BX_GRAPHICS_CUSTOM_BACKEND)

else ()
    message(FATAL_ERROR "Unknown graphics backend: ${GRAPHICS_BACKEND}")
endif ()

## Setup audio backend
#if (BX_AUDIO_BACKEND STREQUAL "PortAudio")
#    message(STATUS "Using PortAudio as the audio backend")
#    target_compile_definitions(bx PUBLIC BX_AUDIO_PORTAUDIO_BACKEND)
#	target_link_libraries(bx-platform-portaudio PRIVATE bx)
#	target_link_libraries(bx PUBLIC bx-platform-portaudio)
#
#elseif (BX_AUDIO_BACKEND STREQUAL "Custom")
#    message(STATUS "Using a custom audio backend")
#    target_compile_definitions(bx PUBLIC BX_AUDIO_CUSTOM_BACKEND)
#
#else ()
#    message(FATAL_ERROR "Unknown audio backend: ${WINDOW_BACKEND}")
#endif ()

# Set the output directories for bx-platform-common to the 'modules' folder within the main output directory
set_target_properties(bx-platform-common PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules"
)

# Optional: Post-build step to ensure the library is placed correctly for easy access
add_custom_command(TARGET bx-platform-common POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:bx>/modules"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:bx-platform-common>" "$<TARGET_FILE_DIR:bx>/extern/"
    COMMENT "Copying bx-platform-common to bx's modules directory."
)