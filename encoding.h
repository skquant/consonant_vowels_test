#pragma once


wchar_t* toUnicode(const char* s, int codepage=1/*CP_OEMCP*/);
char* toMBCS(const wchar_t* ws, int codepage=1/*CP_OEMCP*/);
