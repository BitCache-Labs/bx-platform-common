#include <bx/platform/window_glfw.hpp>

#include <bx/core/macros.hpp>
#include <bx/engine/imgui.hpp>

#include <backends/imgui_impl_glfw.h>

bool WindowGLFW::InitializeImGui()
{
    ImGui::SetCurrentContext(ImGuiManager::GetCurrentContext());

#if defined BX_GRAPHICS_OPENGL_BACKEND
    if (!ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true))
#endif
    {
        BX_LOGE("Failed to initialize ImGui GLFW backend!");
        return false;
    }

    return true;
}

void WindowGLFW::ShutdownImGui()
{
    ImGui_ImplGlfw_Shutdown();
}

void WindowGLFW::NewFrameImGui()
{
    ImGui_ImplGlfw_NewFrame();
}

void WindowGLFW::EndFrameImGui()
{
    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}