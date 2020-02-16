#pragma once

#include <string>

void WansungToJohab(std::string in, std::string& out);
void JohabToWansung(std::string in, std::string& out);

void WansungBreakJamo(std::string in, std::wstring& out);
void WansungBreakJamo(std::string in, std::string& out);

void JohabBreakJamo(std::string in, std::wstring& out);
void JohabBreakJamo(std::string in, std::string& out);

void BreakJamo(std::wstring in, std::wstring& out);
