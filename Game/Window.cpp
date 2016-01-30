#pragma once

#include "Window.hpp"

//----------------------------------------------------------------------------//
// Window
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Window::Window(void)
{
	LOG("Create Window");

	// window
	WNDCLASS _wc;
	memset(&_wc, 0, sizeof(_wc));
	_wc.style = CS_DBLCLKS;
	_wc.lpfnWndProc = _MsgCallback;
	_wc.hInstance = 0; //GetModuleHandle(0);
	_wc.hCursor = LoadCursor(0, IDC_ARROW);
	_wc.lpszClassName = "Game32K";
	RegisterClass(&_wc);
	m_handle = CreateWindow(_wc.lpszClassName, _wc.lpszClassName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, _wc.hInstance, 0);
	if (!m_handle)
		Fatal("Couldn't create window");

	// size
	RECT _rect;
	GetClientRect(m_handle, &_rect);
	m_size.Set(_rect.right - _rect.left, _rect.bottom - _rect.top);

	// state
	m_shouldClose = false;
	m_active = true;
}
//----------------------------------------------------------------------------//
Window::~Window(void)
{
	LOG("Destroy Window");

#ifdef _DEBUG
	ShowWindow(m_handle, SW_HIDE);
#endif

	// ... do nothing
}
//----------------------------------------------------------------------------//
void Window::PollEvents(void)
{
	for (MSG _msg; PeekMessage(&_msg, 0, 0, 0, PM_REMOVE);)
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}
}
//----------------------------------------------------------------------------//
LRESULT CALLBACK Window::_MsgCallback(HWND _wnd, UINT _msg, WPARAM _w_param, LPARAM _l_param)
{
	switch (_msg)
	{
	case WM_SETFOCUS:
		s_instance->m_active = true;
		//s_instance->OnDeactivate();
		break;

	case WM_KILLFOCUS:
		s_instance->m_active = false;
		//s_instance->OnActivate();
		break;

	case WM_CLOSE:
		if (!s_instance->m_shouldClose)
		{
			s_instance->m_shouldClose = true;
			//s_instance->OnClose();
		}
		return 0;

	case WM_SIZE:
		if (_w_param == SIZE_MAXIMIZED || _w_param == SIZE_RESTORED)
		{
			s_instance->m_size.Set(LOWORD(_l_param), HIWORD(_l_param));
			//s_instance->OnResize();
		}
		break;
	}
	return DefWindowProc(_wnd, _msg, _w_param, _l_param);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
