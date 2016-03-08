#pragma once

#include "Math.hpp"
#include "Object.hpp"
#include <Windows.h>

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gDevice Device::Get()

//----------------------------------------------------------------------------//
// Keyboard
//----------------------------------------------------------------------------//

enum InputKey : uint
{
	IK_Unknown = 0,
};

//----------------------------------------------------------------------------//
// Mouse
//----------------------------------------------------------------------------//

enum CursorMode : uint
{
	CM_Default = 0x0,
	CM_Hidden = 0x1,
	CM_Camera = 0x2 | CM_Hidden,
};

//----------------------------------------------------------------------------//
// Device
//----------------------------------------------------------------------------//

class Device : public Singleton<Device>
{
public:

	Device(void);
	~Device(void);

	HWND WindowHandle(void) { return m_handle; }
	uint WindowWidth(void) { return m_width; }
	uint WindowHeight(void) { return m_height; }
	const Vec2& WindowSize(void) { return m_size; }
	bool ShouldClose(void) { return m_shouldClose; }
	void PollEvents(void);

	void SetCursorMode(CursorMode _mode);
	const Vec2& GetCameraDelta(void) { return m_cameraDelta; }

	void SetTitle(const char* _title, ...)
	{
		char _buff[256];
		va_list _args;
		va_start(_args, _title);
		vsprintf(_buff, _title, _args);
		va_end(_args);
		SetWindowText(WindowHandle(), _buff);
	}

protected:
	void _CursorToCenter(void);
	LRESULT _HandleMsg(UINT _msg, WPARAM _wParam, LPARAM _lParam);
	static LRESULT CALLBACK _MsgCallback(HWND _wnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

	// window
	HWND m_handle;
	bool m_shouldClose;
	bool m_active;
	bool m_fullscreen;
	uint m_width;
	uint m_height;
	Vec2 m_size;

	// cursor
	CursorMode m_cursorMode;
	Vec2i m_cursorPos;
	Vec2i m_cursorPrevPos; //todo: remove it (?)
	Vec2 m_cameraDelta;
	Vec2 m_cameraSensitivity;
	bool m_resetCameraDelta; //todo: rename to m_resetCursor
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
