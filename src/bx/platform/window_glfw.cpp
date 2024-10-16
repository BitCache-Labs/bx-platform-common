#include <bx/platform/window_glfw.hpp>

#include <bx/core/macros.hpp>
#include <bx/core/profiler.hpp>

#include <stdlib.h>

#include <rttr/registration.h>
RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<WindowGLFW>("WindowGLFW")
		.constructor();
		//.method("GetWindowPtr", &WindowGLFW::GetWindowPtr);
}

GLFWwindow* WindowGLFW::GetWindowPtr() const
{
	return m_pWindow;
}

bool WindowGLFW::IsOpen()
{
	return !glfwWindowShouldClose(m_pWindow);
}

void WindowGLFW::GetSize(int* width, int* height)
{
	glfwGetFramebufferSize(m_pWindow, width, height);
}

void WindowGLFW::SetCursorMode(CursorMode mode)
{
	switch (mode)
	{
	case CursorMode::NORMAL:
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case CursorMode::HIDDEN:
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		break;
	case CursorMode::DISABLED:
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	case CursorMode::CAPTURED:
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
		break;
	default:
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

WindowGLProc WindowGLFW::GetProcAddress(const char* name)
{
	return glfwGetProcAddress(name);
}

static void glfw_error_callback(int i, const char* c)
{
	BX_LOGE("GLFW ({}) {}", i, c);
}

static void glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
	//Screen::SetWidth(width);
	//Screen::SetHeight(height);
}

bool WindowGLFW::Initialize()
{
#ifdef __arm__
	if (putenv((char*)"DISPLAY=:0"))
	{
		BX_LOGE("Failed to set DISPLAY enviroment variable!");
		return false;
	}
#endif

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		BX_LOGE("Failed to initialize GLFW!");
		return false;
	}

	const String& title = "Titlte";// Data::GetString("Title", "Title", DataTarget::SYSTEM);
	int width = 800;// Data::GetInt("Width", 800, DataTarget::SYSTEM);
	int height = 600;// Data::GetInt("Height", 600, DataTarget::SYSTEM);

	GLFWmonitor* pMonitor = nullptr;

#ifdef BX_GRAPHICS_OPENGL_BACKEND

#if defined BX_DEBUG_BUILD || defined BX_EDITOR_BUILD
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#else
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
#endif

#ifdef WIN32
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

#elif defined __arm__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_SAMPLES, 2);

	pMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);
	width = pMode->width;
	height = pMode->height;

	glfwWindowHint(GLFW_RED_BITS, pMode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, pMode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, pMode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, pMode->refreshRate);
#endif

#else
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
#endif

	m_pWindow = glfwCreateWindow(width, height, title.c_str(), pMonitor, NULL);

	if (m_pWindow == NULL)
	{
		BX_LOGE("Failed to create GLFW window!");
		glfwTerminate();
		return false;
	}

	glfwGetWindowSize(m_pWindow, &width, &height);
	//Screen::SetWidth(width);
	//Screen::SetHeight(height);

	glfwSetWindowSizeCallback(m_pWindow, glfw_window_size_callback);

#ifdef BX_GRAPHICS_OPENGL_BACKEND
	glfwMakeContextCurrent(m_pWindow);
	glfwSwapInterval(1);

#elif defined BX_GRAPHICS_VULKAN_BACKEND
	if (!glfwVulkanSupported())
	{
		ENGINE_LOGE("GLFW: Vulkan Not Supported!");
		return false;
	}
#endif

	return true;
}

void WindowGLFW::Reload()
{
}

void WindowGLFW::Shutdown()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void WindowGLFW::PollEvents()
{
	PROFILE_FUNCTION();
	glfwPollEvents();

	if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_pWindow, true);
}

void WindowGLFW::Display()
{
	PROFILE_FUNCTION();

#ifdef BX_GRAPHICS_OPENGL_BACKEND
	glfwSwapBuffers(m_pWindow);
#endif
}