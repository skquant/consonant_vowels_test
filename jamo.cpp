#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN
#include <assert.h>
#include <windows.h>
#include <string>

#include "encoding.h"
#include "jamo.h"

#ifdef _MFC_VER
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#endif

using namespace std;

static const int CP_Hanguel_Johab = 1361;
static const int CP_Hanguel_Wansung = 949;


// return jobab character for ぁ,い,ぇ ...
wchar_t ordinal_to_syllabic(
						   wchar_t c, // johab alphabet ordinal [0 .. 1f]
						   int type) // 1:cho, 2:joong, 3:jong
{
	static const wchar_t CHO[] = {
		0,		// 0 : fill code
		0,		// 1 : fill code
		0x8841,	// 2 : ぁ, 1000 1000 0100 0001
		0x8c41,	// 3 : あ, 1000 1100 0100 0001
		0x9041,	// 4 : い, 1001 0000 0100 0001
		0x9441,	// 5 : ぇ, 1001 0100 0100 0001
		0x9841,	// 6 : え, 1001 1000 0100 0001
		0x9c41,	// 7 : ぉ, 1001 1100 0100 0001
		0xa041,	// 8 : け, 1010 0000 0100 0001
		0xa441,	// 9 : げ, 1010 0100 0100 0001
		0xa841,	// a : こ, 1010 1000 0100 0001
		0xac41,	// b : さ, 1010 1100 0100 0001
		0xb041,	// c : ざ, 1011 0000 0100 0001
		0xb441,	// d : し, 1011 0100 0100 0001
		0xb841,	// e : じ, 1011 1000 0100 0001
		0xbc41,	// f : す, 1011 1100 0100 0001
		0xc041,	// 10: ず, 1100 0000 0100 0001
		0xc441,	// 11: せ, 1100 0100 0100 0001
		0xc841,	// 12: ぜ, 1100 1000 0100 0001
		0xcc41,	// 13: そ, 1100 1100 0100 0001
		0xd041,	// 14: ぞ, 1101 0000 0100 0001
		0,		// 15: fill code
		0,		// 16: fill code
		0,		// 17: fill code
		0,		// 18: fill code
		0,		// 19: fill code
		0,		// 1a: fill code
		0,		// 1b: fill code
		0,		// 1c: fill code
		0,		// 1d: fill code
		0,		// 1e: fill code
		0,		// 1f: fill code
	};
	static const wchar_t JOONG[] = {
		0,		// 0 : fill code
		0,		// 1 : fill code
		0,		// 2 : fill code
		0x8461,	// 3 : た, 1000 0100 0110 0001
		0x8481,	// 4 : だ, 1000 0100 1000 0001
		0x84a1,	// 5 : ち, 1000 0100 1010 0001
		0x84c1,	// 6 : ぢ, 1000 0100 1100 0001
		0x84e1,	// 7 : っ, 1000 0100 1110 0001
		0,  	// 8 : fill code
		0,		// 9 : fill code
		0x8541,	// a : つ, 1000 0101 0100 0001
		0x8561,	// b : づ, 1000 0101 0110 0001
		0x8581,	// c : て, 1000 0101 1000 0001
		0x85a1,	// d : で, 1000 0101 1010 0001
		0x85c1,	// e : と, 1000 0101 1100 0001
		0x85e1,	// f : ど, 1000 0101 1110 0001
		0,		// 10: fill code
		0,		// 11: fill code
		0x8641,	// 12: な, 1000 0110 0100 0001
		0x8661,	// 13: に, 1000 0110 0110 0001
		0x8681,	// 14: ぬ, 1000 0110 1000 0001
		0x86a1,	// 15: ね, 1000 0110 1010 0001
		0x86c1,	// 16: の, 1000 0110 1100 0001
		0x86e1,	// 17: は, 1000 0110 1110 0001
		0,		// 18: fill code
		0,		// 19: fill code
		0x8741,	// 1a: ば, 1000 0111 0100 0001
		0x8761,	// 1b: ぱ, 1000 0111 0110 0001
		0x8781,	// 1c: ひ, 1000 0111 1000 0001
		0x87a1,	// 1d: び, 1000 0111 1010 0001
		0,		// 1e: fill code
		0,		// 1f: fill code
	};
	static const wchar_t JONG[] = {
		0,		// 0 : fill code
		0,		// 1 : fill code
		0x8442,	// 2 : ぁ, 1000 0100 0100 0010
		0x8443,	// 3 : あ, 1000 0100 0100 0011
		0x8444,	// 4 : ぃ, 1000 0100 0100 0100
		0x8445,	// 5 : い, 1000 0100 0100 0101
		0x8446,	// 6 : ぅ, 1000 0100 0100 0110
		0x8447,	// 7 : う, 1000 0100 0100 0111
		0x8448,	// 8 : ぇ, 1000 0100 0100 1000
		0x8449,	// 9 : ぉ, 1000 0100 0100 1001
		0x844a,	// a : お, 1000 0100 0100 1010
		0x844b,	// b : か, 1000 0100 0100 1011
		0x844c,	// c : が, 1000 0100 0100 1100
		0x844d,	// d : き, 1000 0100 0100 1101
		0x844e,	// e : ぎ, 1000 0100 0100 1110
		0x844f,	// f : く, 1000 0100 0100 1111
		0x8450,	// 10: ぐ, 1000 0100 0101 0000
		0x8451,	// 11: け, 1000 0100 0101 0001
		0,		// 12: fill code
		0x8453,	// 13: げ, 1000 0100 0101 0011
		0x8454,	// 14: ご, 1000 0100 0101 0100
		0x8455,	// 15: さ, 1000 0100 0101 0101
		0x8456,	// 16: ざ, 1000 0100 0101 0110
		0x8457,	// 17: し, 1000 0100 0101 0111
		0x8458,	// 18: じ, 1000 0100 0101 1000
		0x8459,	// 19: ず, 1000 0100 0101 1001
		0x845a,	// 1a: せ, 1000 0100 0101 1010
		0x845b,	// 1b: ぜ, 1000 0100 0101 1011
		0x845c,	// 1c: そ, 1000 0100 0101 1100
		0x845d, // 1d: ぞ, 1000 0100 0101 1101
		0,		// 1e: fill code
		0,		// 1f: fill code
	};

	if (c < 0 && c > 0x1f) {
		assert(c >= 0 && c <= 0x1f);
		return 0;
	}

	switch (type) {
		case 1:
			return CHO[c];
		case 2:
			return JOONG[c];
		case 3:
			return JONG[c];
		default:
			return 0;
	};
}

void break_jamo(std::string in, std::string& out)
{
	out.clear();
	size_t len = in.length();

	for (size_t i=0; i<len; ++i) {
		if (::IsDBCSLeadByte(in[i]) && i < len-1) {
			wchar_t c = MAKEWORD(in[i+1], in[i]);
			wchar_t cho =
				ordinal_to_syllabic((c & 0x7c00) >> 10, 1); // 1111 1000 0000 0000
			wchar_t joong =
				ordinal_to_syllabic((c & 0x03e0) >> 5, 2); // 0000 0011 1110 0000
			wchar_t jong = ordinal_to_syllabic(c & 0x001f, 3); // 0000 0000 0001 1111
			
			if (cho) {
				out.push_back(HIBYTE(cho));
				out.push_back(LOBYTE(cho));
			}
			if (joong) {
				out.push_back(HIBYTE(joong));
				out.push_back(LOBYTE(joong));
			}
			if (jong) {
				out.push_back(HIBYTE(jong));
				out.push_back(LOBYTE(jong));
			}
			++i;
		} else {
			out.push_back(in[i]);
		}
	}
}


void WansungToJohab(std::string in, std::string& out)
{
	out = toMBCS(toUnicode(in.c_str(), CP_Hanguel_Wansung),
		CP_Hanguel_Johab);
}

void JohabToWansung(std::string in, std::string& out)
{
	out = toMBCS(toUnicode(in.c_str(), CP_Hanguel_Johab),
		CP_Hanguel_Wansung);
}

void WansungBreakJamo(std::string in, std::wstring& out)
{
	string jstr;
	WansungToJohab(in, jstr);
	string jbrk;
	break_jamo(jstr, jbrk);
	out = toUnicode(jbrk.c_str(), CP_Hanguel_Johab);
}

void WansungBreakJamo(std::string in, std::string& out)
{
	wstring wstr;
	WansungBreakJamo(in, wstr);
	out = toMBCS(wstr.c_str(), CP_Hanguel_Wansung);
}

void JohabBreakJamo(std::string in, std::wstring& out)
{
	string jbrk;
	JohabBreakJamo(in, jbrk);
	out = toUnicode(jbrk.c_str(), CP_Hanguel_Johab);
}

void JohabBreakJamo(std::string in, std::string& out)
{
	break_jamo(in, out);
}

void BreakJamo(std::wstring in, std::wstring& out)
{
	JohabBreakJamo(toMBCS(in.c_str(), CP_Hanguel_Johab), out);
}