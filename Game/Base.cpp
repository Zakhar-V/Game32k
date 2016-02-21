#include "Base.hpp"
#include <Windows.h>

//----------------------------------------------------------------------------//
// Debug
//----------------------------------------------------------------------------//

#if USE_LOG	& 0x2
FILE* g_log = nullptr;
#endif

//----------------------------------------------------------------------------//
void LogMsg(const char* _msg, ...)
{
#if USE_LOG
	va_list _args;
	va_start(_args, _msg);

#	if USE_LOG & 0x2
	if(!g_log)
		g_log = fopen("log.txt", "w");
	if (g_log)
	{
		vfprintf(g_log, _msg, _args);
		fwrite("\n", 1, 1, g_log);
		fflush(g_log);
	}
	else
		MessageBox(0, "Couldn't create log.txt", "Warning", MB_OK | MB_ICONWARNING | MB_APPLMODAL | MB_TOPMOST);
#	endif

#	if (USE_LOG & 0x1) && defined(_DEBUG)
	vprintf(_msg, _args);
	printf("\n");
#	endif

	va_end(_args);
#endif
}
//----------------------------------------------------------------------------//
void Fatal(const char* _error)
{
	LOG("Fatal: %s", _error);
	MessageBoxA(0, _error, "Fatal error", MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_TOPMOST);
	ExitProcess(-1);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// String
//----------------------------------------------------------------------------//

String::Buffer String::s_null;

//----------------------------------------------------------------------------//
String& String::operator = (const String& _str)
{
	_str.m_buffer->AddRef();
	m_buffer->Release();
	m_buffer = _str.m_buffer;
	return *this;
}
//----------------------------------------------------------------------------//
String& String::Clear(void)
{
	m_buffer->Release();
	m_buffer = s_null.AddRef();
	return *this;
}
//----------------------------------------------------------------------------//
String& String::Append(const char* _str, int _length, bool _quantize)
{
	_length = _Length(_str, _length);
	if (_length)
	{
		_Realloc(_length + m_buffer->length, _quantize);
		memcpy(m_buffer->str + m_buffer->length, _str, _length);
		m_buffer->length += _length;
		m_buffer->str[m_buffer->length] = 0;
	}
	return *this;
}
//----------------------------------------------------------------------------//
int String::_Length(const char* _str, int _length)
{
	int _strLength = strlen(_str ? _str : "");
	return _length >= 0 && _length < _strLength ? _length : _strLength;
}
//----------------------------------------------------------------------------//
String::Buffer* String::_New(uint _maxLength)
{
	if (!_maxLength)
		return s_null.AddRef();

	Buffer* _buff = reinterpret_cast<Buffer*>(new uint8[sizeof(Buffer) + _maxLength]);
	new(_buff) Buffer();
	_buff->size = _maxLength + 1;
	return _buff;
}
//----------------------------------------------------------------------------//
void String::_Realloc(uint _newSize, bool _quantize)
{
	if (_newSize < m_buffer->size && m_buffer->refs == 1)
		return;
	if (_quantize)
		_newSize = (_newSize + 1) << 1;

	Buffer* _newBuffer = _New(_newSize);
	memcpy(_newBuffer->str, m_buffer->str, m_buffer->length + 1);
	_newBuffer->length = m_buffer->length;
	m_buffer->Release();
	m_buffer = _newBuffer;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
