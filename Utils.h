#pragma once
#include <string>

#define VER_TERA_CLASSIC 610
#define VER_TERA_MODERN 897

bool IncompletePathsEqual(const std::string& a, const std::string& b);

// Wide string to UTF8
std::string W2A(const wchar_t* str, int len = -1);
std::string W2A(const std::wstring& str);
// UTF8 string to wide
std::wstring A2W(const char* str, int len = -1);
std::wstring A2W(const std::string& str);

// Creates an upper-case copy of the provided string
std::string ToUpper(const std::string& data);

bool IsAnsi(const std::string& str);