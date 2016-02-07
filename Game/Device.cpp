#pragma once

#include "Device.hpp"

//----------------------------------------------------------------------------//
// Window
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Device::Device(void)
{
	LOG("Create Device");

	// window
	WNDCLASS _wc;
	memset(&_wc, 0, sizeof(_wc));
	_wc.style = CS_DBLCLKS;
	_wc.lpfnWndProc = _MsgCallback;
	_wc.hInstance = 0; //GetModuleHandle(0);
	_wc.hCursor = LoadCursor(0, IDC_ARROW);
	_wc.lpszClassName = "Game32k";
	RegisterClass(&_wc);
	m_handle = CreateWindow(_wc.lpszClassName, _wc.lpszClassName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, _wc.hInstance, 0);
	if (!m_handle)
		Fatal("Couldn't create window");

	// size
	RECT _rect;
	GetClientRect(m_handle, &_rect);
	m_width = _rect.right - _rect.left;
	m_height = _rect.bottom - _rect.top;
	m_size.Set((float)m_width, (float)m_height);

	// state
	m_shouldClose = false;
	m_active = true;
	m_fullscreen = false;

	// cursor
	m_cursorMode = CM_Default;
	m_cursorPos = 0;
	m_cursorPrevPos = 0;
	m_cameraDelta = 0;
	m_cameraSensitivity = 1;
	m_resetCameraDelta = true;
}
//----------------------------------------------------------------------------//
Device::~Device(void)
{
	LOG("Destroy Device");

#ifdef _DEBUG
	ShowWindow(m_handle, SW_HIDE);
#endif

	// ... do nothing
}
//----------------------------------------------------------------------------//
void Device::PollEvents(void)
{
	for (MSG _msg; PeekMessage(&_msg, 0, 0, 0, PM_REMOVE);)
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}

	if (m_cursorMode == CM_Camera && m_active)
	{
		_CursorToCenter();
	}
}
//----------------------------------------------------------------------------//
void Device::_CursorToCenter(void)
{
	Vec2i _c(m_width >> 1, m_height >> 1);
	m_cameraDelta = m_resetCameraDelta ? 0 : (m_cursorPos - _c);
	m_resetCameraDelta = false;
	POINT _pt;
	_pt.x = _c.x;
	_pt.y = _c.y;
	ClientToScreen(m_handle, &_pt);
	SetCursorPos(_pt.x, _pt.y);

	//if (m_cameraDelta.x || m_cameraDelta.y)
	//	printf("%.1f %.1f\n", m_cameraDelta.x, m_cameraDelta.y);
	//Vec2i _cd = m_cursorPos - m_cursorPrevPos;
	//if (_cd.x || _cd.y)
	//	printf("%d %d\n", _cd.x, _cd.y);
}
//----------------------------------------------------------------------------//
LRESULT Device::_HandleMsg(UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case WM_SETFOCUS:
		m_active = true;

		m_resetCameraDelta = true;

		// todo: ...

		//s_instance->OnDeactivate();
		break;

	case WM_KILLFOCUS:
		m_active = false;
		ClipCursor(nullptr);
		//s_instance->OnActivate();
		break;

	case WM_CLOSE:
		if (!m_shouldClose)
		{
			m_shouldClose = true;
			//s_instance->OnClose();
		}
		return 0;

	case WM_MOUSEMOVE:
	{
		if ((m_cursorMode == CM_Camera || m_fullscreen) && m_active)
		{
			RECT _rc, _crc;
			POINT _pt;
			GetClientRect(m_handle, &_rc);
			_pt.x = _rc.left;
			_pt.y = _rc.top;
			ClientToScreen(m_handle, &_pt);
			_crc.left = _pt.x;
			_crc.top = _pt.y;
			_crc.right = _pt.x + _rc.right;
			_crc.bottom = _pt.y + _rc.bottom;
			ClipCursor(&_crc);
		}
		else
			ClipCursor(nullptr);

		//mods = _wParam;
		m_cursorPrevPos = m_cursorPos;
		m_cursorPos.Set(LOWORD(_lParam), HIWORD(_lParam));
	}
	break;

	case WM_SIZE:
		if (_wParam == SIZE_MAXIMIZED || _wParam == SIZE_RESTORED)
		{
			m_width = LOWORD(_lParam);
			m_height = HIWORD(_lParam);
			m_size.Set((float)m_width, (float)m_height);
			//s_instance->OnResize();
		}
		break;
	}

	return DefWindowProc(m_handle, _msg, _wParam, _lParam);
}
//----------------------------------------------------------------------------//
LRESULT CALLBACK Device::_MsgCallback(HWND _wnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (_wnd == s_instance->m_handle)
		return s_instance->_HandleMsg(_msg, _wParam, _lParam);

	return DefWindowProc(_wnd, _msg, _wParam, _lParam);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
