#pragma once
#include <string>

// Wide string to UTF8
std::string W2A(const wchar_t* str, int len = -1);
std::string W2A(const std::wstring& str);
// UTF8 string to wide
std::wstring A2W(const char* str, int len = -1);
std::wstring A2W(const std::string& str);

// Creates an upper-case copy of the provided string
std::string ToUpper(const std::string& data);