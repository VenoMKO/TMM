#include "Mod.h"
#include "../Utils.h"

#define PACKAGE_MAGIC 0x9E2A83C1
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

void SetString(std::ofstream& s, std::string str)
{
  int size = str.size();
  if (!IsAnsi(str))
  {
    std::wstring wstr = A2W(str);
    size = -(int)wstr.size();
    s.write((char*)&size, sizeof(size));
    s.write((char*)wstr.c_str(), size * -2);
  }
  else
  {
    s.write((char*)&size, sizeof(size));
    s.write((char*)str.c_str(), size);
  }
}

std::ifstream& operator>>(std::ifstream& s, ModFile& m)
{
  s.seekg(0, std::ios::end);
  const size_t end = s.tellg();
  size_t pos = end - sizeof(int);
  s.seekg(pos);
  unsigned int magic = 0;
  s.read((char*)&magic, sizeof(magic));

  if (magic == PACKAGE_MAGIC)
  {
    // Package has metadata
    pos -= sizeof(int);
    s.seekg(pos);
    int metaSize = 0;
    s.read((char*)&metaSize, sizeof(metaSize));

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

    pos -= sizeof(int);
    int regionLock = 0;
    s.seekg(pos);
    s.read((char*)&regionLock, sizeof(regionLock));
    m.RegionLock = regionLock;

    pos -= sizeof(int);
    int version = 0;
    s.seekg(pos);
    s.read((char*)&version, sizeof(version));

    int compositeEnd = 0;
    if (version != PACKAGE_MAGIC)
    {
      m.ModFileVersion = version;

      pos -= sizeof(int);
      s.seekg(pos);
      s.read((char*)&compositeEnd, sizeof(compositeEnd));

      int tfcOffsetsCount = 0;
      pos -= sizeof(int);
      s.seekg(pos);
      s.read((char*)&tfcOffsetsCount, sizeof(tfcOffsetsCount));

      int tfcOffsetsOffset = 0;
      pos -= sizeof(int);
      s.seekg(pos);
      s.read((char*)&tfcOffsetsOffset, sizeof(tfcOffsetsOffset));

      int tfcEnd = 0;
      pos -= sizeof(int);
      s.seekg(pos);
      s.read((char*)&tfcEnd, sizeof(tfcEnd));

      if (tfcOffsetsOffset && tfcOffsetsCount)
      {
        s.seekg(tfcOffsetsOffset);
        for (int idx = 0; idx < tfcOffsetsCount; ++idx)
        {
          ModFile::TfcPackage tfc;
          s >> tfc;
          m.TfcPackages.push_back(tfc);
        }
      }
    }

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
    m.Packages.back().Size = (compositeEnd ? compositeEnd : (end - metaSize)) - m.Packages.back().Offset;
  }
  else
  {
    // Package has no metadata, but it might have object path which should be enough for patching
    ModFile::CompositePackage& p = m.Packages.emplace_back();
    p.Size = (int)end;
    s.seekg(0);
    s >> p;
  }
  return s;
}

std::ifstream& operator>>(std::ifstream& s, GameConfigFile& cfg)
{
  s.seekg(0);
  int count = 0;
  s.read((char*)&count, sizeof(count));
  cfg.Mods.resize(count);
  for (int idx = 0; idx < count; ++idx)
  {
    int enabled = 0;
    s.read((char*)&enabled, sizeof(enabled));
    cfg.Mods[idx].Enabled = enabled;
    cfg.Mods[idx].File = GetString(s);
  }
  return s;
}

std::ofstream& operator<<(std::ofstream& s, GameConfigFile& cfg)
{
  int count = (int)cfg.Mods.size();
  s.write((char*)&count, sizeof(count));
  for (int idx = 0; idx < count; ++idx)
  {
    int enabled = cfg.Mods[idx].Enabled;
    s.write((char*)&enabled, sizeof(enabled));
    SetString(s, cfg.Mods[idx].File);
  }
  return s;
}

std::ifstream& operator>>(std::ifstream& s, ModFile::CompositePackage& p)
{
  p.Offset = s.tellg();
  s.seekg(p.Offset + 4);
  s.read((char*)&p.FileVersion, sizeof(p.FileVersion));
  s.read((char*)&p.LicenseeVersion, sizeof(p.LicenseeVersion));
  s.seekg(p.Offset + 12);
  std::string folderName = GetString(s);
  if (folderName.find(ModPrefix) == 0)
  {
    p.ObjectPath = folderName.substr(strlen(ModPrefix));
  }
  return s;
}

std::ifstream& operator>>(std::ifstream& s, ModFile::TfcPackage& p)
{
  s.read((char*)&p.Offset, sizeof(p.Offset));
  s.read((char*)&p.Size, sizeof(p.Size));
  p.IdxOffset = s.tellg();
  s.read((char*)&p.Idx, sizeof(p.Idx));
  return s;
}
