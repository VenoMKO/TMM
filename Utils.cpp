#include "Utils.h"

#include <algorithm>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

bool IncompletePathsEqual(const std::string& a, const std::string& b)
{
  auto pos = a.find('.');
  if (pos == std::string::npos)
  {
    return false;
  }
  const std::string_view objPathA(&a[pos + 1]);
  pos = a.rfind('_', pos);
  if (pos == std::string::npos)
  {
    return false;
  }
  const std::string_view compositeA(&a[0], pos);


  pos = b.find('.');
  if (pos == std::string::npos)
  {
    return false;
  }
  const std::string_view objPathB(&b[pos + 1]);
  pos = b.rfind('_', pos);
  if (pos == std::string::npos)
  {
    return false;
  }
  const std::string_view compositeB(&b[0], pos);

  return compositeA == compositeB && objPathA == objPathB;
}

std::string W2A(const wchar_t* str, int len)
{
  if (len == -1)
  {
    len = lstrlenW(str);
  }
  int size = WideCharToMultiByte(CP_UTF8, 0, str, len, NULL, 0, NULL, NULL);
  std::string result(size, 0);
  WideCharToMultiByte(CP_UTF8, 0, str, len, &result[0], size, NULL, NULL);
  return result;
}

std::string W2A(const std::wstring& str)
{
  return W2A(&str[0], (int)str.length());
}

std::wstring A2W(const char* str, int len)
{
  if (len == -1)
  {
    len = (int)strlen(str);
  }
  int size = MultiByteToWideChar(CP_UTF8, 0, str, len, NULL, 0);
  std::wstring result(size, 0);
  MultiByteToWideChar(CP_UTF8, 0, str, len, &result[0], size);
  return result;
}

std::wstring A2W(const std::string& str)
{
  return A2W(&str[0], (int)str.length());
}

std::string ToUpper(const std::string& data)
{
  std::string result = data;
  std::for_each(result.begin(), result.end(), [](char& c) {
    c = ::toupper(c);
  });
  return result;
}

bool IsAnsi(const std::string& str)
{
  for (const unsigned char& ch : str)
  {
    if (ch > 127)
    {
      return false;
    }
  }
  return true;
}
