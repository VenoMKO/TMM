#include "Mod.h"
#include "../Utils.h"

#define PACKAGE_MAIGC 0x9E2A83C1
#define MAX_STRLEN 1024

const char* ModPrefix = "MOD:";

std::string GetString(std::ifstream& s)
{
  int size = 0;
  s.read((char*)&size, sizeof(size));
  std::string result;
  if (size > 0)
  {
    if (size > MAX_STRLEN)
    {
      return {};
    }
    result.resize(size);
    s.read(&result[0], size);
  }
  else if (size < 0)
  {
    if (-size > MAX_STRLEN)
    {
      return {};
    }
    std::wstring wstr;
    wstr.resize(-size);
    size = -size * 2;
    s.read((char*)&wstr[0], size);
    result = W2A(wstr);
  }
  return result;
}

std::ifstream& operator>>(std::ifstream& s, Mod& m)
{
  s.seekg(0, std::ios::end);
  const size_t end = s.tellg();
  size_t pos = end - sizeof(int);
  s.seekg(pos);
  unsigned int magic = 0;
  s.read((char*)&magic, sizeof(magic));

  if (magic == PACKAGE_MAIGC)
  {
    // Package has metadata
    pos -= sizeof(int);
    s.seekg(pos);
    int compositeCount = 0;
    s.read((char*)&compositeCount, sizeof(compositeCount));

    pos -= sizeof(int);
    int offsetsOffset = 0;
    s.seekg(pos);
    s.read((char*)&offsetsOffset, sizeof(offsetsOffset));

    pos -= sizeof(int);
    int containerOffset = 0;
    s.seekg(pos);
    s.read((char*)&containerOffset, sizeof(containerOffset));

    pos -= sizeof(int);
    int nameOffset = 0;
    s.seekg(pos);
    s.read((char*)&nameOffset, sizeof(nameOffset));

    pos -= sizeof(int);
    int authorOffset = 0;
    s.seekg(pos);
    s.read((char*)&authorOffset, sizeof(authorOffset));

    s.seekg(authorOffset);
    m.ModAuthor = GetString(s);
    s.seekg(nameOffset);
    m.ModName = GetString(s);
    s.seekg(containerOffset);
    m.Container = GetString(s);

    s.seekg(offsetsOffset);
    m.Packages.resize(compositeCount);
    std::vector<int> offsets(compositeCount);
    for (int idx = 0; idx < compositeCount; ++idx)
    {
      s.read((char*)&offsets[idx], sizeof(offsets[idx]));
    }
    for (int idx = 0; idx < compositeCount; ++idx)
    {
      s.seekg(offsets[idx]);
      if (idx)
      {
        m.Packages[idx - 1].Size = (int)s.tellg() - m.Packages[idx - 1].Offset;
      }
      s >> m.Packages[idx];
    }
    m.Packages.back().Size = end - m.Packages.back().Offset;
  }
  else
  {
    // Package has no metadata, but it might have incomplete object path which should be enough for patching
    Mod::CompositePackage& p = m.Packages.emplace_back();
    p.Size = (int)end;
    s.seekg(0);
    s >> p;
  }
  return s;
}

std::ifstream& operator>>(std::ifstream& s, Mod::CompositePackage& p)
{
  p.Offset = s.tellg();
  s.seekg(p.Offset + 12);
  std::string folderName = GetString(s);
  if (folderName.find(ModPrefix) == 0)
  {
    p.IncompleteObjectPath = folderName.substr(strlen(ModPrefix));
  }
  return s;
}
