#pragma once

#include "Base.hpp"
#include "Math.hpp"
#include <Windows.h>

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

typedef HWND WindowHandle;
#define gWindow Window::Get()

//----------------------------------------------------------------------------//
// Window
//----------------------------------------------------------------------------//

class Window : public Singleton<Window>
{
public:

	Window(void);
	~Window(void);

	WindowHandle Handle(void) { return m_handle; }
	const Vec2i& Size(void) { return m_size; }
	bool ShouldClose(void) { return m_shouldClose; }
	void PollEvents(void);

protected:
	static LRESULT CALLBACK _MsgCallback(HWND _wnd, UINT _msg, WPARAM _w_param, LPARAM _l_param);

	WindowHandle m_handle;
	bool m_shouldClose;
	bool m_active;
	Vec2i m_size;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
