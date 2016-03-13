#pragma once

#include "File.hpp"

//----------------------------------------------------------------------------//
// FileSystem
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
RawData LoadFile(const char* _name)
{
	FILE* _file = fopen(_name, "rb");
	RawData _buff;
	if (_file)
	{
		fseek(_file, 0, SEEK_END);
		uint _size = (uint)ftell(_file);
		fseek(_file, 0, SEEK_SET);
		_buff.Resize(_size);
		fread(*_buff, 1, _size, _file);
		fclose(_file);
	}
	else
		LOG("Couldn't open file \"%s\"", _name);
	return Move(_buff);
}
//----------------------------------------------------------------------------//
void SaveFile(const char* _name, const void* _data, uint _size)
{
	FILE* _file = fopen(_name, "wb");
	if (_file)
	{
		fwrite(_data, 1, _size, _file);
		fclose(_file);
	}
	else
		LOG("Couldn't save file \"%s\"", _name);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

