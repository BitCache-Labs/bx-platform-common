#pragma once

#include <bx/platform/input.hpp>
#include <GLFW/glfw3.h>

class BX_PLATFORM_API InputGLFW final : public Input
{
	RTTR_ENABLE(Input)

public:
	InputGLFW() = default;

public:
	bool Initialize() override;
	void Reload() override;
	void Shutdown() override;

	void Poll() override;
	float GetAxis(GamepadAxis axis) override;
	bool GetButton(GamepadButton button) override;
	bool GetButtonOnce(GamepadButton button) override;
	bool GetKey(Key key) override;
	bool GetKeyOnce(Key key) override;
	bool GetMouse() override;
	bool GetMouseButton(MouseButton button) override;
	bool GetMouseButtonOnce(MouseButton button) override;
	float GetMouseX() override;
	float GetMouseY() override;
	int GetNumTouches() override;
	int GetTouchId(int index) override;
	float GetTouchX(int index) override;
	float GetTouchY(int index) override;
	void SetPadVibration(int leftRumble, int rightRumble) override;
	void SetPadLightbarColor(float r, float g, float b) override;
	void ResetPadLightbarColor() override;

private:
	friend class InputAccess;

	GLFWgamepadstate m_currGamepadState;
	GLFWgamepadstate m_prevGamepadState;

	static constexpr int nr_keys = 350;
	bool m_currKeysDown[nr_keys];
	bool m_prevKeysDown[nr_keys];
	bool m_keysDownChanged[nr_keys];

	static constexpr int nr_mousebuttons = 8;
	bool m_currMouseButtonsDown[nr_mousebuttons];
	bool m_prevMouseButtonsDown[nr_mousebuttons];
	bool m_mouseButtonsDownChanged[nr_keys];

	float m_mousePos[2];
	bool m_gamepadConnected;
};