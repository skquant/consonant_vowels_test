#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>

#include <memory>
#include "encoding.h"

#ifdef _MFC_VER
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#endif

using namespace std;


int convert(const char* str, wchar_t* w_str, int len, int codepage);
int convert(const wchar_t* w_str, char* str, int len, int codepage);
int chklen(const char* str, int codepage);
int chklen(const wchar_t* w_str, int codepage);

static std::auto_ptr<wchar_t> g_wbuf;
static std::auto_ptr<char> g_cbuf;
static int g_wbuflen;
static int g_cbuflen;


wchar_t* toUnicode(const char* s, int codepage)
{
	if (!s)
		return 0;

	int len = chklen(s, codepage);
	if (len > g_wbuflen) {
		g_wbuflen = len;
		g_wbuf.reset(new wchar_t[g_wbuflen]);
	}
	if (len) {
		int ret = convert(s, g_wbuf.get(), g_wbuflen, codepage);
		return g_wbuf.get();
	} else {
		return 0;
	}
}

char* toMBCS(const wchar_t* ws, int codepage)
{
	if (!ws)
		return 0;

	int len = chklen(ws, codepage);
	if (len > g_cbuflen) {
		g_cbuflen = len;
		g_cbuf.reset(new char[g_cbuflen]);
	}
	if (len) {
		int ret = convert(ws, g_cbuf.get(), g_cbuflen, codepage);
		return g_cbuf.get();
	} else {
		return 0;
	}
}

int convert(const char* str, wchar_t* w_str, int len, int codepage)
{
	int ret = MultiByteToWideChar(codepage, MB_ERR_INVALID_CHARS, str, -1, w_str, len);
	return ret;
}

int convert(const wchar_t* w_str, char* str, int len, int codepage)
{
	int ret = WideCharToMultiByte(codepage, 0, w_str, -1, str, len, 0, 0);
	return ret;
}

int chklen(const char* str, int codepage)
{
	int ret = MultiByteToWideChar(codepage, MB_ERR_INVALID_CHARS, str, -1, 0, 0);
	return ret;
}

int chklen(const wchar_t* w_str, int codepage)
{
	int ret = WideCharToMultiByte(codepage, 0, w_str, -1, 0, 0, 0, 0);
	return ret;
}

