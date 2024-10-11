#include <bx/platform/graphics_opengl.hpp>

#include <bx/core/macros.hpp>
#include <bx/engine/imgui.hpp>

#include <backends/imgui_impl_opengl3.h>

bool GraphicsOpenGL::InitializeImGui()
{
    ImGui::SetCurrentContext(ImGuiManager::GetCurrentContext());

#if defined BX_GRAPHICS_OPENGL_BACKEND
    if (!ImGui_ImplOpenGL3_Init("#version 460 core\n"))
#elif defined BX_GRAPHICS_OPENGLES_BACKEND
    if (!ImGui_ImplOpenGL3_Init("#version 300 es\n"))
#endif
    {
        BX_LOGE("Failed to initialize ImGui OpenGL backend!");
        return false;
    }

    return true;
}

void GraphicsOpenGL::ShutdownImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
}

void GraphicsOpenGL::NewFrameImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
}

void GraphicsOpenGL::EndFrameImGui()
{
    GraphicsHandle renderTarget = GetCurrentBackBufferRT();
    GraphicsHandle depthStencil = GetDepthBuffer();
    SetRenderTarget(renderTarget, depthStencil);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}