#include "Utils.h"

#include <algorithm>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#include <tchar.h>

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

bool IsTeraRunning(bool& error, const std::wstring& teraDir)
{
  DWORD processes[2048];
  DWORD arraySize = 0;
  if (!EnumProcesses(processes, sizeof(processes), &arraySize))
  {
    error = true;
    return false;
  }
  DWORD count = arraySize / sizeof(DWORD);
  
  bool anySuccess = false;
  for (unsigned int idx = 0; idx < count; ++idx)
  {
    TCHAR processName[MAX_PATH] = TEXT("None");
    if (HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[idx]))
    {
      HMODULE hMod;
      DWORD cbNeeded;
      if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
      {
        if (GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(TCHAR)))
        {
          anySuccess = true;
          if (!_tcscmp(processName, _T("TERA.exe")))
          {
            if (teraDir.empty())
            {
              CloseHandle(hProcess);
              return true;
            }
            else if (GetModuleFileNameEx(hProcess, NULL, processName, MAX_PATH) > teraDir.size())
            {
              if (!_tcsncmp(processName, teraDir.c_str(), teraDir.size()))
              {
                CloseHandle(hProcess);
                return true;
              }
            }
          }
        }
      }
      CloseHandle(hProcess);
    }
  }
  if (!anySuccess)
  {
    error = true;
  }
  return false;
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
