
#pragma warning(disable:4996) // The POSIX name

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <sys/stat.h>
#ifdef _MSC_VER
#include <direct.h>
#else
#include <dirent.h>
#endif
#include <exception>
#include "LZ4/lz4.h"
#include "LZ4/lz4hc.h"

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

#define FOR_GAME32K 1
#if FOR_GAME32K
typedef uint16 filesize_t;
#else
typedef uint32 filesize_t;
#endif
const size_t HEADER_SIZE = 32;
const size_t MAX_FILESIZE = (filesize_t)-1;
const size_t MAX_FILENAME = 32 - sizeof(filesize_t);
const size_t MAX_FILEPATH = 1024;
const size_t MAX_FILELENAME = 256;
const size_t MAX_FILEINFO = sizeof(filesize_t);

const char* g_usageText =
{
	"Command line should be with syntax : [Mode] Args\n"
	"Encode file:\n"
	"\t-encode output_file input_file\n"
	"Decode file:\n"
	"\t-decode output_file input_file\n"
	"Pack files:\n"
	"\t-pack output_file input_file1 input_file2 ...\n"
	//"Unpack files:\n"
	//"\t-unpack output_directory input_file\n"
	"Do convert binary file to text:\n"
	"\t-text output_file input_file\n"
};

//----------------------------------------------------------------------------//
// LZ4 
//----------------------------------------------------------------------------//

///\return size of compressed data written to destination buffer.
int LZ4Compress(const void* _src, void* _dst, size_t _srcSize)
{
	return _src ? LZ4_compressHC((const char*)_src, (char*)_dst, _srcSize) : 0;
}

///\return negative value, when out of range, size of destination buffer otherwise.
int LZ4Decompress(const void* _src, void* _dst, size_t _srcSize, size_t _dstSize, bool _test = false, int* _undecoded = nullptr)
{
	// specification: http://fastcompression.blogspot.in/2011/05/lz4-explained.html

	if (!_src || !_srcSize)	// empty source
		return 0;

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
		if (_dstp + _length > _dste || (!_test && (_srcp + _length > _srce)))
			break; // out of range

		if (!_test)
			memcpy(_dstp, _srcp, _length);
		_srcp += _length;
		_dstp += _length;

		if (_dstp > _dste8 || (!_test && (_srcp > _srce8)))
			break; // EoF or overflow destination buffer

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
			if (!_test)
			{
				_dstp[0] = _ref[0];
				_dstp[1] = _ref[1];
				_dstp[2] = _ref[2];
				_dstp[3] = _ref[3];
			}
			_dstp += 4;
		}
		else
		{
			_length += 4;
		}

		if (_dstp + _length > _dste)
			_length = _dste - _dstp; // EoF

		if (_test)
		{
			_dstp += _length;
		}
		else
		{
			while (_length--)
				*_dstp++ = *_ref++;
		}

		if (_dstp == _dste)
			break;
	}
	assert(_srcp <= _srce);
	assert(_dstp <= _dste);

	if (_undecoded)
		*_undecoded = (int)(_srce - _srcp); // has negative value on error

	return _srcp <= _srce ? (_dstp - (uint8*)_dst) : (_srcp - _srce);
}

//----------------------------------------------------------------------------//
// I/O
//----------------------------------------------------------------------------//

#define LOG(msg, ...) printf(msg, ##__VA_ARGS__), printf("\n")
#ifdef _DEBUG
#	define DLOG(msg, ...) LOG(msg, ##__VA_ARGS__)
#else
#	define DLOG(msg, ...)
#endif

class StreamBase
{
public:

	StreamBase(void) :
		m_mbegin(nullptr),
		m_mend(nullptr),
		m_pos(nullptr),
		m_end(nullptr)
	{
	}

	virtual ~StreamBase(void)
	{
		Close();
	}

	void Resize(size_t _size, bool _quantize = false)
	{
		_Realloc(_size, _quantize);
		m_end = m_mbegin + _size;
		assert(m_end <= m_mend);
		assert(m_pos <= m_end);
		if (m_pos > m_end)
			m_pos = m_end;
	}

	void Upsize(size_t _size, bool _quantize = true)
	{
		size_t _s = Size() + _size;
		_Realloc(_s, _quantize);
		m_end = m_mbegin + _s;
		assert(m_end <= m_mend);
		assert(m_pos <= m_end);
	}

	void Reserve(size_t _size)
	{
		_Realloc(_size, false);
	}

	void Clear(void)
	{
		m_pos = m_mbegin;
		m_end = m_mbegin;
	}

	void Close(void)
	{
		if (m_mbegin)
		{
			delete[] m_mbegin;
			m_mbegin = nullptr;
			m_mend = nullptr;
			m_pos = nullptr;
			m_end = nullptr;
		}
	}

	const char* operator () (size_t _relativeOffset) const
	{
		assert(m_pos + _relativeOffset < m_end);
		return m_pos + _relativeOffset;
	}

	const char* operator [] (size_t _offset) const
	{
		assert(m_mbegin + _offset < m_mend);
		return m_mbegin + _offset;
	}

	template <class T> const T& Elem(size_t _relativeOffset) const
	{
		assert(m_pos + _relativeOffset < m_end);
		return reinterpret_cast<T*>(m_pos + _relativeOffset);
	}

	template <class T> const T& At(size_t _offset) const
	{
		assert(m_mbegin + _offset < m_mend);
		return *reinterpret_cast<T*>(m_mbegin + _offset);
	}

	bool EoF(void) const
	{
		return m_pos == m_end;
	}

	size_t Size(void) const
	{
		return m_end - m_mbegin;
	}

	size_t MaxSize(void) const
	{
		return m_mend - m_mbegin;
	}

	size_t Pos(void) const
	{
		return m_pos - m_mbegin;
	}

	size_t Remain(void) const
	{
		return m_end - m_pos;
	}

	void SetPos(size_t _absOffset)
	{
		size_t _cap = m_end - m_pos;
		if (_absOffset > _cap)
			_absOffset = _cap;
		m_pos = m_mbegin + _absOffset;
	}

	const char* Ptr(void) const
	{
		return m_pos;
	}

	const char* Base(void) const
	{
		return m_mbegin;
	}

protected:

	void _Realloc(size_t _size, bool _quantize = true) // can be throw the std::bad_alloc
	{
		if (m_mbegin + _size >= m_mend)
		{
			_size = _quantize ? (_size << 1) : _size;

			char* _oldData = m_mbegin;
			size_t _oldPos = m_pos - m_mbegin;
			size_t _oldEnd = m_end - m_mbegin;
			size_t _oldSize = m_mend - m_mbegin;

			m_mbegin = new char[_size];
			m_pos = m_mbegin + _oldPos;
			m_end = m_mbegin + _oldEnd;
			m_mend = m_mbegin + _size;

			assert(Pos() == _oldPos);
			assert(Size() == _oldEnd);

			if (_oldSize)
			{
				memcpy(m_mbegin, _oldData, _oldSize);
				delete[] _oldData;
			}
		}
	}

	char* m_mbegin;
	char* m_mend;
	char* m_pos;
	char* m_end;

private:

	StreamBase(const StreamBase&) = delete;
	StreamBase& operator = (const StreamBase&) = delete;
};

class Reader : public StreamBase
{
public:

	Reader(void)
	{
	}

	~Reader(void)
	{
	}

	bool Load(const char* _name)
	{
		assert(_name != nullptr);

		Clear(); // reset pos

		bool _success = false;
		FILE* _file = fopen(_name, "rb");
		if (_file)
		{
			fseek(_file, 0, SEEK_END);
			size_t _size = (size_t)ftell(_file);
			Resize(_size);
			fseek(_file, 0, SEEK_SET);
			if (fread(m_mbegin, 1, _size, _file) == _size)
				_success = true;
			else
				LOG("Error: IO error on reading file \"%s\"", _name);
			fclose(_file);
		}
		else
			LOG("Error: Unable to open file \"%s\"", _name);

		return _success;
	}

	size_t Read(void* _dst, size_t _size)
	{
		assert(_size == 0 || _dst != nullptr);
		size_t _cap = m_end - m_pos;
		if (_size > _cap)
		{
			LOG("Unexpected EoF");
			_size = _cap;
		}
		memcpy(_dst, m_pos, _size);
		m_pos += _size;
		return _size;
	}

	size_t ReadString(char* _dst, size_t _maxSize) // read null-terminated string
	{
		assert(_dst != nullptr);

		char* _p = _dst;
		char* _e = _dst + _maxSize;
		while (m_pos < m_end && _p < _e && *m_pos)
			*_p++ = *m_pos++;
		if(_p < _e)
			*_p = 0;
		return _p - _dst;
	}

	template <class T> T Read(void)
	{
		T _dst = static_cast<T>(0);
		Read(&_dst, sizeof(T));
		return _dst;
	}

private:

	Reader(const Reader&) = delete;
	Reader& operator = (const Reader&) = delete;
};

class Writer : public StreamBase
{
public:

	Writer(void)
	{
	}

	~Writer(void)
	{
	}

	bool Open(size_t _size)
	{
		Clear(); // reset pos
		_Realloc(_size, false);
		return true;
	}

	bool Save(const char* _name)
	{
		assert(_name != nullptr);

		bool _success = false;
		FILE* _file = fopen(_name, "wb");
		if (_file)
		{
			size_t _size = Size();
			if (fwrite(m_mbegin, 1, _size, _file) == _size)
				_success = true;
			else
				LOG("Error: IO error on reading file \"%s\"", _name);
			fclose(_file);
		}
		else
			LOG("Error: Couldn't create file \"%s\"", _name);

		return _success;
	}

	char* operator () (size_t _relativeOffset)
	{
		assert(m_pos + _relativeOffset < m_end);
		return m_pos + _relativeOffset;
	}

	char* operator [] (size_t _offset)
	{
		assert(m_mbegin + _offset < m_mend);
		return m_mbegin + _offset;
	}

	template <class T> T& Elem(size_t _relativeOffset)
	{
		assert(m_pos + _relativeOffset < m_end);
		return reinterpret_cast<T*>(m_pos + _relativeOffset);
	}

	template <class T> T& At(size_t _offset)
	{
		assert(m_mbegin + _offset < m_mend);
		return *reinterpret_cast<T*>(m_mbegin + _offset);
	}

	char* Ptr(void)
	{
		return m_pos;
	}

	char* Base(void)
	{
		return m_mbegin;
	}

	size_t Write(const void* _src, size_t _size)
	{
		assert(_size == 0 || _src != nullptr);

		Upsize(_size);
		memcpy(m_pos, _src, _size);
		m_pos += _size;
		return _size;
	}

	size_t WriteString(const char* _str) // write null-terminated string
	{
		size_t _w = 0;
		if (_str)
			_w = Write(_str, strlen(_str));

		char _n = 0;
		return Write(&_n, 1) + _w;
	}

	template <class T> size_t Write(T _src)
	{
		return Write(&_src, sizeof(T));
	}

private:

	Writer(const Writer&) = delete;
	Writer& operator = (const Writer&) = delete;
};

//----------------------------------------------------------------------------//
// Encode/Decode
//----------------------------------------------------------------------------//

bool Encode(const char* _srcFile, const char* _dstFile)
{
	LOG("Encode: \"%s\" -> \"%s\" ...", _srcFile, _dstFile);
	Reader _src;
	Writer _dst;

	if (!_src.Load(_srcFile))
		return false;
	
	size_t _srcSize = _src.Size();
	if (_srcSize > MAX_FILESIZE)
	{
		LOG("Error: File \"%s\" is too big (size = %d KB, limit = %d KB)", _srcFile, _srcSize / 1024, MAX_FILESIZE / 1024);
		return false;
	}

	size_t _dstSize = LZ4_COMPRESSBOUND(_srcSize);
	_dst.Resize(sizeof(filesize_t) + _dstSize);
	int _written = LZ4Compress(_src.Base(), _dst.Base() + sizeof(filesize_t), _srcSize);
	if (_written < 0)
	{
		LOG("Error: Couldn't encode file\"%s\"", _srcFile);
		return false;
	}
	
	_dst.At<filesize_t>(0) = (filesize_t)_srcSize;
	_dst.Resize(_written + sizeof(filesize_t));

	float _ratio = (float)_srcSize / (float)_written;
	if (_ratio != _ratio) // NaN
		_ratio = 1;
	LOG("Success: %d->%d bytes (ratio: %.2f)", _srcSize, _written, _ratio);

	return _dst.Save(_dstFile);
}

bool Decode(const char* _srcFile, const char* _dstFile)
{
	LOG("Decode: \"%s\" -> \"%s\" ...", _srcFile, _dstFile);

	Reader _src;
	Writer _dst;

	if (!_src.Load(_srcFile))
		return false;

	size_t _dstSize = _src.Read<filesize_t>();
	size_t _srcSize = _src.Remain();
	_dst.Resize(_dstSize);

#if 1
	int _written = LZ4Decompress(_src.Ptr(), _dst.Base(), _srcSize, _dstSize);
#else
	int _written = LZ4_decompress_fast(_src.Ptr(), _dst.Base(), _dstSize);
#endif
	if (_written < 0)
	{
		LOG("Error: Couldn't decode file \"%s\"", _srcFile);
		return false;
	}
	if ((size_t)_written != _dstSize)
	{
		LOG("Error: Compressed file \"%s\" is corrupted (Size is different: %d)", _srcFile, int(_written - (int)_dstSize));
		return false;
	}

	float _ratio = (float)_written / (float)_srcSize;
	if (_ratio != _ratio) // NaN
		_ratio = 1;
	LOG("Success: %d->%d bytes (ratio: %.2f)", _srcSize, _written, _ratio);

	return _dst.Save(_dstFile);
}

//----------------------------------------------------------------------------//
// Package
//----------------------------------------------------------------------------//

bool FileExists(Writer& _pack, const char* _name)
{
	const char* s = _pack.Base();
	const char* e = _pack.Base() + _pack.Size();
	if (!s)
		return false;

	char _str[MAX_FILEPATH + 1];
	char *_strp, *_stre = _str + MAX_FILEPATH;
	while (s < e)
	{
		_strp = _str;
		while (*s && s < e && _strp < _stre)
			*_strp++ = *s++;
		*_strp++ = *s++;
		if (!stricmp(_str, _name))
			return true; // file exists

		if (s + MAX_FILEINFO >= e)
			break;
		filesize_t _size = *(const filesize_t*)s;
		s += MAX_FILEINFO + _size; // go to next file
	}

	return false;
}

bool PackDirectory(Writer& _dst, const char* _dir, const char* _filter, const char* _base, size_t& _sizeSum)
{
	Reader _src;

	if (!_dir)
		_dir = "";
	if (!_filter)
		_filter = "";
	if (!_base)
		_base = "";

	char _path[MAX_FILEPATH], _intname[MAX_FILEPATH], _name[MAX_FILEPATH];
	strcpy(_path, _dir);
	size_t _l = strlen(_path);
	if (_l && !strchr("\\/", _path[_l - 1]))
		strcat(_path, "/");
	strcat(_path, _filter);

	_finddata_t _fi;
	intptr_t _fd = _findfirst(_path, &_fi);
	if (_fd == -1)
	{
		if (*_filter && !strchr(_filter, '*'))
		{
			LOG("Error: Couldn't find file \"%s\"", _path);
			return false;
		}
		return true;
	}

	bool _r = true;
	do
	{
		if (!strcmp(_fi.name, ".") || !strcmp(_fi.name, "..") || (_fi.attrib & _A_HIDDEN))
			continue;

		strcpy(_intname, _base);
		strcat(_intname, _fi.name);

		if (*_dir)
		{
			strcpy(_name, _dir);
			strcat(_name, "/");
			strcat(_name, _fi.name);
		}
		else
		{
			strcpy(_name, _fi.name);
		}

		if (_fi.attrib & _A_SUBDIR)
		{
			strcat(_intname, "/");
			LOG("Directory: \"%s\", filter: \"*\"", _name);

			if (!PackDirectory(_dst, _name, "*", _intname, _sizeSum))
			{
				_r = false;
				break;
			}
		}
		else
		{
			LOG("File: \"%s\" %d bytes", _intname, _fi.size);

			if (!FileExists(_dst, _intname))
			{
				_sizeSum += _fi.size;

				if (strlen(_intname) > MAX_FILELENAME)
				{
					LOG("Error: Filename \"%s\" is too large (size = %d bytes, limit = %d bytes)", _intname, strlen(_intname), MAX_FILELENAME);
					return false;
				}

				if (_fi.size > MAX_FILESIZE)
				{
					LOG("Error: File \"%s\" is too big (size = %d KB, limit = %d KB)", _name, _fi.size, MAX_FILESIZE / 1024);
					return false;
				}

				if (!_src.Load(_name))
					return false;

				_dst.WriteString(_intname);
				_dst.Write<filesize_t>((filesize_t)_fi.size);
				_dst.Write(_src.Base(), _src.Size());
			}
			else
			{
				LOG("Warning: File \"%s\" ignored", _name);
			}
		}


	} while (_findnext(_fd, &_fi) == 0);

	_findclose(_fd);

	return _r;
}

bool PackFiles(Writer& _dst, const char* _srcName, const char* _base, size_t& _sizeSum)
{
	assert(_srcName != nullptr);

	char _dir[MAX_FILEPATH], _filter[MAX_FILEPATH];
	strcpy(_dir, _srcName);
	char* s = _dir + strlen(_dir);
	if (s > _dir)
		--s;
	while (s > _dir && (*s == '/' || *s == '\\'))
		*s-- = 0;
	for(; s > _dir; s--)
	{
		if (*s == '/' || *s == '\\')
		{
			s[0] = 0;
			++s;
			break;
		}
	}
	while (s > _dir && (*s == '/' || *s == '\\'))
		*s-- = 0;

	if (s == _dir)
	{
		if (!strcmp(_dir, ".") || !strcmp(_dir, ".."))
		{
			strcpy(_filter, "*");
		}
		else
		{
			strcpy(_filter, _dir);
			*_dir = 0;
		}
	}
	else
	{
		strcpy(_filter, s);
	}

	if(!*_dir)
		strcpy(_dir, ".");

	LOG("Directory: \"%s\", filter: \"%s\"", _dir, _filter);

	return PackDirectory(_dst, _dir, _filter, _base, _sizeSum);
}

bool Pack(const char** _srcFiles, size_t _numFiles, const char* _dstFile)
{
	LOG("Pack: \"%s\" ...", _dstFile);
	size_t _sizeSum = 0;
	Writer _dst;
	for (size_t i = 0; i < _numFiles; ++i)
	{
		if (!PackFiles(_dst, _srcFiles[i], "", _sizeSum))
			return false;
	}

	size_t _srcSize = _dst.Size();
	size_t _dstSize = LZ4_COMPRESSBOUND(_srcSize);
	Writer _pack;
	_pack.Resize(sizeof(filesize_t) + _dstSize);
	int _written = LZ4Compress(_dst.Base(), _pack.Ptr() + sizeof(filesize_t), _srcSize);

	_pack.At<filesize_t>(0) = (filesize_t)_srcSize;
	_pack.Resize(_written + sizeof(filesize_t));

	if (!_pack.Save(_dstFile))
		return false;

	float _ratio = (float)_srcSize / (float)_written;
	if (_ratio != _ratio) // NaN
		_ratio = 1;
	LOG("Completed: %d->%d bytes total (service info: %d bytes) (ratio: %.2f)", _srcSize, _written, _srcSize - _sizeSum, _ratio);

	return true;
}

bool Unpack(const char* _srcFile, const char* _dstDir)
{
	// TODO
	return false;
}

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

bool Text(const char* _srcFile, const char* _dstFile)
{
	LOG("Text: \"%s\" -> \"%s\" ...", _srcFile, _dstFile);

	Reader _src;
	if (!_src.Load(_srcFile))
		return false;

	char _buff[256];
	sprintf(_buff, "// Created from \"%s\", %u bytes\n", _srcFile, _src.Size());
	Writer _dst;
	_dst.Write(_buff, strlen(_buff));
	
	_dst.Reserve(_src.Size() + _src.Size() / 15 + 1);
	const char* s = _src.Base();
	const char* e = _src.Base() + _src.Size();

	while (s < e)
	{
		for (int i = 0; i < 15; ++i)
		{
			uint8 c = *s++;
			sprintf(_buff, "0x%02x, ", c);
			_dst.Write(_buff, 6);
		}
		_dst.Write("\n", 1);
	}

	return _dst.Save(_dstFile);
}

//----------------------------------------------------------------------------//
// App
//----------------------------------------------------------------------------//

const char* g_mode = nullptr;
const char** g_args = nullptr;
size_t g_numArgs = 0;
size_t g_maxArgs = 0;

bool ExecCmds(void)
{
	DLOG("Exec");

	if (!g_mode)
	{
		LOG("Error: Mode is not specified");
		return false;
	}
	if (g_numArgs < 1)
	{
		LOG("Error: Output name is not specified");
		return false;
	}
	if (g_numArgs < 2)
	{
		LOG("Error: Input name is not specified");
		return false;
	}

	bool _r = false;
	if (!strcmp(g_mode, "encode"))
	{
		_r = Encode(g_args[1], g_args[0]);
	}
	else if (!strcmp(g_mode, "decode"))
	{
		_r = Decode(g_args[1], g_args[0]);
	}
	else if (!strcmp(g_mode, "pack"))
	{
		_r = Pack(g_args + 1, g_numArgs -1, g_args[0]);
	}
	else if (!strcmp(g_mode, "text"))
	{
		_r = Text(g_args[1], g_args[0]);
	}

	g_mode = nullptr;
	g_numArgs = 0;

	return _r;
}

bool SetMode(const char* _mode)
{
	DLOG("SetMode(%s)", _mode);

	assert(_mode != nullptr);

	if (g_mode)
	{
		if (!ExecCmds())
			return false;
	}

	g_mode = _mode;
	if (strcmp(g_mode, "encode") && strcmp(g_mode, "decode") && strcmp(g_mode, "pack") && strcmp(g_mode, "text"))
	{
		LOG("Error: Unknown mode \"%s\"", g_mode);
		return false;
	}

	return true;
}


bool AddArg(const char* _arg)
{
	DLOG("AddArg(%s) (%d)", _arg, g_numArgs);

	assert(_arg != 0);

	if (!g_mode)
	{
		LOG("Error: Mode is not specified");
		return false;
	}

	if (strcmp(g_mode, "pack") && g_numArgs >= 2)
	{
		LOG("Error: Too many input names");
		return false;
	}

	// realloc
	if (g_numArgs == g_maxArgs)
	{
		size_t _oldSize = g_maxArgs;
		const char** _oldData = g_args;
		g_maxArgs = (g_maxArgs + 1) << 1;
		DLOG("Realloc args (%d->%d)", _oldSize, g_maxArgs);
		g_args = new const char*[g_maxArgs];
		if (_oldSize)
		{
			memcpy(g_args, _oldData, _oldSize * sizeof(const char*));
			delete[] _oldData;
		}
	}

	g_args[g_numArgs++] = _arg;
	return true;
}


bool HandleArg(const char* _cmd)
{
	DLOG("AddCmd(%s)", _cmd);

	assert(_cmd != 0);

	if (!_cmd) // it is last command; execute previous commands
		return ExecCmds();
	
	if (*_cmd == '-') // mode
	{
		if (!SetMode(_cmd + 1))	// change mode
			return false;
	}
	else // argument
	{
		if (!AddArg(_cmd))
			return false;
	}

	return true;
}

bool HandleCmdLine(int _argc, const char** _argv)
{
	if (!_argc)
	{
		LOG("%s", g_usageText);
		return true;
	}

	while (_argc--)
	{
		assert(_argv != 0);
		assert(*_argv != 0);

		if (!HandleArg(*_argv++))
			return false;
	}

	if (g_mode || g_numArgs)
		return ExecCmds();

	return true;
}

int main(int _argc, const char** _argv)
{
	int _r = 0;
	try
	{
#if 0 // test
		//Encode("Packer.cpp", "in.lz4");
		//Decode("in.lz4", "out.txt");

		const char* _files[] =
		{
			"."
		};
		Pack(_files, 1, "test.lz4");
#else
		if (_argc)
			_argc--, _argv++; // skip exe filename

		_r = HandleCmdLine(_argc, _argv) ? 0 : -1;
		delete[] g_args;
#endif
	}
	catch (std::exception _e)
	{
		LOG("Exception: %s", _e.what());
	}
#ifdef _DEBUG
	system("pause");
#endif
	return _r;
}