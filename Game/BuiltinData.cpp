#pragma once

#include "BuiltinData.hpp"

//----------------------------------------------------------------------------//
// builtin data
//----------------------------------------------------------------------------//

static const uint8 g_builtinData[] =
{
#include "Assets.inc"
	0, 0,
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void LZ4Decompress(const void* _src, void* _dst, size_t _srcSize, size_t _dstSize)
{
	// specification: http://fastcompression.blogspot.in/2011/05/lz4-explained.html

	if (!_src || !_srcSize)	// empty source
		return;

	const uint8*_ref;
	const uint8* _srcp = (const uint8*)_src;
	const uint8* _srce = _srcp + _srcSize;
	const uint8* _srce6 = _srce - 6; // end - (lastliterals + marker)
	const uint8* _srce8 = _srce - 8; // end - (offset + lastliterals + marker)
	uint8* _dstp = (uint8*)_dst;
	const uint8* _dste = _dstp + _dstSize;
	const uint8* _dste8 = _dste - 8; // end - (copylength)
	const uint8* _dste12 = _dste - 12; // end - (copylength + minmatch)
	uint8 _marker;
	size_t _length;

	for (;;)
	{
		// marker
		_marker = *_srcp++;
		_length = (_marker >> 4);

		// length
		if (_length == 0xf)
		{
			for (uint8 s = 0xff; _srcp < _srce && s == 0xff;)
				_length += (s = *_srcp++);
		}

		// literals
		if (_dstp + _length > _dste /*|| _srcp + _length > _srce*/)
			break; // out of range

		memcpy(_dstp, _srcp, _length);
		_srcp += _length;
		_dstp += _length;

		if (_dstp > _dste8 /*|| _srcp > _srce8*/) // EoF or overflow destination buffer
			break;

		// offset
		_ref = _dstp - *(uint16*)_srcp;
		_srcp += 2;
		if (_ref > _dste) // overflow destination buffer
			break;

		// matchlength
		_length = _marker & 0xf;
		if (_length == 0xf)
		{
			for (uint8 s = 0xff; _srcp < _srce8 && s == 0xff;)
				_length += (s = *_srcp++);
		}

		// copy 
		if (_dstp - _ref < 4)
		{
			_dstp[0] = _ref[0];
			_dstp[1] = _ref[1];
			_dstp[2] = _ref[2];
			_dstp[3] = _ref[3];
			_dstp += 4;
		}
		else
		{
			_length += 4;
		}

		if (_dstp + _length > _dste)
			_length = _dste - _dstp; // EoF

		while (_length--)
			*_dstp++ = *_ref++;

		if (_dstp == _dste)
			break;
	}
	ASSERT(_srcp <= _srce);
	ASSERT(_dstp <= _dste);

	//return _srcp <= _srce ? (_dstp - (uint8*)_dst) : (_srcp - _srce);
}
//----------------------------------------------------------------------------//
bool ExtractBuiltinData(void)
{
	bool _outOfDate = true;
	uint16 _size = *(const decltype(_size)*)g_builtinData;
	uint _hash = Hash(g_builtinData, sizeof(g_builtinData) - sizeof(_size));
	float _ratio =  ((float)*(const decltype(_size)*)g_builtinData) / ((float)(sizeof(g_builtinData) - sizeof(_size)));
	LOG("Builtin data: size = %d x %.2f = %d, hash = 0x%08x", sizeof(g_builtinData) - sizeof(_size), _ratio, _size, _hash);

	FILE* _file = fopen("hash", "rb");
	if (_file)
	{
		uint _checksum = 0;
		fread(&_checksum, 1, 4, _file);
		fclose(_file);
		_outOfDate = _checksum != _hash;
	}

	if (_outOfDate)
	{
		LOG("Extract builtin data ...");

		{
			char* _unpacked = new char[_size];
			char _name[256]; // 
			LZ4Decompress(g_builtinData + sizeof(_size), _unpacked, sizeof(g_builtinData) - sizeof(_size), _size);

			char* s = _unpacked;
			char* e = _unpacked + _size;
			while (s < e)
			{
				// name
				char* _str = _name;
				while (*s && s < e)
				{
					ASSERT(_str < _name + sizeof(_name));
					*_str++ = *s++;
				}
				*_str++ = *s++;

				// size
				decltype(_size) _fsize = *(const decltype(_size)*)s;
				s += sizeof(_fsize);

				// file
				LOG("Extract file: \"%s\", %d bytes", _name, _fsize);
				_file = fopen(_name, "wb");
				if (!_file)
					Fatal("Couldn't create file");
				fwrite(s, 1, _fsize, _file);
				fclose(_file);

				s += _fsize; // go to next file 
			}
		}

		_file = fopen("hash", "wb");
		if (_file)
		{
			fwrite(&_hash, 1, 4, _file);
			fclose(_file);
		}
		else
			Fatal("Couldn't save hash");
	}

	return _outOfDate;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

