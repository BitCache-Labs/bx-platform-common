#pragma once

#include <bx/platform/window.hpp>

#include <GLFW/glfw3.h>

class BX_PLATFORM_API WindowGLFW final : public Window
{
	RTTR_ENABLE(Window)

public:
	WindowGLFW() = default;

	GLFWwindow* GetWindowPtr() const;

public:
	bool Initialize() override;
	void Reload() override;
	void Shutdown() override;

	bool IsOpen() override;
	void PollEvents() override;
	void Display() override;

	void GetSize(int* width, int* height) override;
	void SetCursorMode(CursorMode mode) override;

	WindowGLProc GetProcAddress(const char* name) override;

	bool InitializeImGui() override;
	void ShutdownImGui() override;
	void NewFrameImGui() override;
	void EndFrameImGui() override;

private:
	GLFWwindow* m_pWindow = nullptr;
};