#include <bx/platform/input_glfw.hpp>

#include <bx/core/macros.hpp>
#include <bx/core/guard.hpp>
#include <bx/core/profiler.hpp>
#include <bx/meta/enum.hpp>
#include <bx/platform/window_glfw.hpp>

#include <rttr/registration.h>
RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<InputGLFW>("InputGLFW")
		.constructor();
}

class InputAccess
{
public:
	static void joystick_callback(int joy, int event) {}

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		auto& glfwInput = static_cast<InputGLFW&>(Input::Get());

		// get the screen-to-game scaling parameters
		//const auto& screen_to_game = xs::configuration::get_scale_to_game(xs::device::get_width(), xs::device::get_height());

		// translate the mouse position to game coordinates
		//xs::configuration::scale_to_game(static_cast<int>(xpos), static_cast<int>(ypos), screen_to_game, m_mousePos[0], m_mousePos[1]);

		glfwInput.m_mousePos[0] = (float)xpos;
		glfwInput.m_mousePos[1] = (float)ypos;
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto& glfwInput = static_cast<InputGLFW&>(Input::Get());
		if (action == GLFW_PRESS || action == GLFW_RELEASE)
			glfwInput.m_keysDownChanged[key] = true;
	}

	static void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
	{
		auto& glfwInput = static_cast<InputGLFW&>(Input::Get());
		if (action == GLFW_PRESS || action == GLFW_RELEASE)
			glfwInput.m_mouseButtonsDownChanged[button] = true;
	}
};

bool InputGLFW::Initialize()
{
#ifdef BX_WINDOW_GLFW_BACKEND
	const auto& glfwWin = static_cast<WindowGLFW&>(Window::Get());
	glfwSetJoystickCallback(InputAccess::joystick_callback);
	glfwSetCursorPosCallback(glfwWin.GetWindowPtr(), InputAccess::cursor_position_callback);
	glfwSetKeyCallback(glfwWin.GetWindowPtr(), InputAccess::key_callback);
	glfwSetMouseButtonCallback(glfwWin.GetWindowPtr(), InputAccess::mousebutton_callback);
#endif

	PollEvents();

	return true;
}

void InputGLFW::Reload()
{
}

void InputGLFW::Shutdown()
{
#ifdef BX_WINDOW_GLFW_BACKEND
	const auto& glfwWin = static_cast<WindowGLFW&>(Window::Get());
	glfwSetJoystickCallback(NULL);
	glfwSetCursorPosCallback(glfwWin.GetWindowPtr(), NULL);
#endif
}

void InputGLFW::PollEvents()
{
	PROFILE_FUNCTION();
	for (int i = 0; i < nr_keys; ++i)
	{
		m_prevKeysDown[i] = m_currKeysDown[i];
		if (m_keysDownChanged[i])
		{
			m_currKeysDown[i] = !m_currKeysDown[i];
			m_keysDownChanged[i] = false;
		}
	}

	for (int i = 0; i < nr_mousebuttons; ++i)
	{
		m_prevMouseButtonsDown[i] = m_currMouseButtonsDown[i];
		if (m_mouseButtonsDownChanged[i])
		{
			m_currMouseButtonsDown[i] = !m_currMouseButtonsDown[i];
			m_mouseButtonsDownChanged[i] = false;
		}
	}

	m_prevGamepadState = m_currGamepadState;

	if (glfwJoystickPresent(0) && glfwJoystickIsGamepad(0))
		m_gamepadConnected = glfwGetGamepadState(0, &m_currGamepadState);
}

float InputGLFW::GetAxis(GamepadAxis axis)
{
	if (!m_gamepadConnected) return 0.0;

	int axis_id = int(axis);
	BX_ENSURE(axis_id >= 0 && axis_id <= GLFW_GAMEPAD_AXIS_LAST);
	return static_cast<float>(m_currGamepadState.axes[axis_id]);
}

bool InputGLFW::GetButton(GamepadButton button)
{
	if (!m_gamepadConnected) return false;

	int button_id = int(button);
	BX_ENSURE(button_id >= 0 && button_id <= GLFW_GAMEPAD_BUTTON_LAST);
	return static_cast<bool>(m_currGamepadState.buttons[button_id]);
}

bool InputGLFW::GetButtonOnce(GamepadButton button)
{
	if (!m_gamepadConnected) return false;

	int button_id = int(button);
	BX_ENSURE(button_id >= 0 && button_id <= GLFW_GAMEPAD_BUTTON_LAST);
	return
		!static_cast<bool>(m_prevGamepadState.buttons[button_id]) &&
		static_cast<bool>(m_currGamepadState.buttons[button_id]);
}

bool InputGLFW::GetKey(Key key)
{
	BX_ENSURE(key >= GLFW_KEY_SPACE && key <= GLFW_KEY_LAST);
	// TODO: CD: In the future with our custom containers we can have an overload that indexes with enum values so the conversion isn't needed
	return m_currKeysDown[Enum::as_value(key)];
}

bool InputGLFW::GetKeyOnce(Key key)
{
	//BX_ENSURE(key >= GLFW_KEY_SPACE && key <= GLFW_KEY_LAST);
	// TODO: CD: In the future with our custom containers we can have an overload that indexes with enum values so the conversion isn't needed
	return m_currKeysDown[Enum::as_value(key)] && !m_prevKeysDown[Enum::as_value(key)];
}

bool InputGLFW::GetMouse()
{
	return true;
}

bool InputGLFW::GetMouseButton(MouseButton button)
{
	return m_currMouseButtonsDown[int(button)];
}

bool InputGLFW::GetMouseButtonOnce(MouseButton button)
{
	return m_currMouseButtonsDown[int(button)] && !m_prevMouseButtonsDown[int(button)];
}

float InputGLFW::GetMouseX()
{
	return static_cast<float>(m_mousePos[0]);
}

float InputGLFW::GetMouseY()
{
	return static_cast<float>(m_mousePos[1]);
}

int InputGLFW::GetNumTouches()
{
	return 0;
}

int InputGLFW::GetTouchId(int index)
{
	return 0;
}

float InputGLFW::GetTouchX(int index)
{
	return 0;
}

float InputGLFW::GetTouchY(int index)
{
	return 0;
}

void InputGLFW::SetPadVibration(int leftRumble, int rightRumble)
{
	// Unimplemented on the PC
}

void InputGLFW::SetPadLightbarColor(float r, float g, float b)
{
	// Unimplemented on the PC (specific dualshock 5 controller mechanic)
}

void InputGLFW::ResetPadLightbarColor()
{
	// Unimplemented on the PC (specific dualshock 5 controller mechanic)
}