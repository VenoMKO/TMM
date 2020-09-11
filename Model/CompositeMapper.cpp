#include "CompositeMapper.h"
#include "../Utils.h"

#include <fstream>
#include <array>
#include <thread>
#include <vector>
#include <exception>

const char Key1[] = { 12, 6, 9, 4, 3, 14, 1, 10, 13, 2, 7, 15, 0, 8, 5, 11 };
const char Key2[] = { 'G', 'e', 'n', 'e', 'r', 'a', 't', 'e', 'P', 'a', 'c', 'k', 'a', 'g', 'e', 'M', 'a', 'p', 'p', 'e', 'r' };
const char* Marker = "tmm_marker";

void EncryptMapper(const std::wstring& path, const std::string& decrypted)
{
  std::string encrypted;
  size_t size = decrypted.size();
  size_t offset = 0;
  encrypted.resize(size);
  for (offset = 0; offset < size; ++offset)
  {
    encrypted[offset] = decrypted[offset] ^ Key2[offset % sizeof(Key2)];
  }

  {
    size_t a = 1;
    size_t b = size - 1;
    for (offset = (size / 2 + 1) / 2; offset; --offset, a += 2, b -= 2)
    {
      std::swap(encrypted[a], encrypted[b]);
    }
  }

  std::array<char, sizeof(Key1)> tmp;
  for (size_t offset = 0; offset + sizeof(Key1) <= size; offset += sizeof(Key1))
  {
    memcpy(&tmp[0], &encrypted[offset], sizeof(Key1));
    for (size_t idx = 0; idx < sizeof(Key1); ++idx)
    {
      encrypted[offset + idx] = tmp[Key1[idx]];
    }
  }

  std::ofstream s(path, std::ios::binary | std::ios::trunc);
  s.write(&encrypted[0], encrypted.size());
}

void DecryptMapper(const std::wstring& path, std::string& decrypted)
{
  std::vector<char> encrypted;
  size_t size = 0;

  {
    std::ifstream s(path, std::ios::binary | std::ios::ate);
    if (!s.is_open())
    {
      throw L"Failed to open " + path;
    }
    s.seekg(0, std::ios_base::end);
    size = s.tellg();
    s.seekg(0, std::ios_base::beg);
    encrypted.resize(size);
    decrypted.resize(size);
    s.read(&encrypted[0], size);
  }

  size_t offset = 0;
  for (; offset + sizeof(Key1) <= size; offset += sizeof(Key1))
  {
    for (size_t idx = 0; idx < sizeof(Key1); ++idx)
    {
      decrypted[offset + idx] = encrypted[offset + Key1[idx]];
    }
  }
  for (; offset < size; ++offset)
  {
    decrypted[offset] = encrypted[offset];
  }

  {
    size_t a = 1;
    size_t b = size - 1;
    for (offset = (size / 2 + 1) / 2; offset; --offset, a += 2, b -= 2)
    {
      std::swap(decrypted[a], decrypted[b]);
    }
  }

  for (offset = 0; offset < size; ++offset)
  {
    decrypted[offset] ^= Key2[offset % sizeof(Key2)];
  }
}

CompositeMapperFile::CompositeMapperFile(const std::wstring& source)
  : SourcePath(source)
{
  std::string decrypted;
  try
  {
    DecryptMapper(source, decrypted);
    SerializeCompositeMapFromString(decrypted);
    SourceSize = decrypted.size();
  }
  catch (const std::exception& e)
  {
    throw L"Failed to serialize " + source + L". Error: " + A2W(e.what());
  }
}

void CompositeMapperFile::Save()
{
  std::string newMapper;
  SerializeCompositeMapToString(newMapper);
  EncryptMapper(SourcePath, newMapper);
}

bool CompositeMapperFile::GetEntryByCompositeName(const std::string& compositeName, CompositeEntry& output)
{
  if (!CompositeMap.count(compositeName))
  {
    return false;
  }
  output = CompositeMap[compositeName];
  return true;
}

bool CompositeMapperFile::GetEntryByIncompleteObjectPath(const std::string& incompletePath, CompositeEntry& output)
{
  const std::string incompletePathUpper = ToUpper(incompletePath);
  for (auto& pair : CompositeMap)
  {
    std::string& tmpPath = pair.second.ObjectPath;
    size_t pos = tmpPath.find('.');
    if (pos == std::string::npos)
    {
      continue;
    }
    std::string testPath = ToUpper(tmpPath.substr(pos + 1));
    if (testPath == incompletePathUpper)
    {
      output = pair.second;
      return true;
    }
  }
  return false;
}

bool CompositeMapperFile::AddEntry(const CompositeEntry& entry)
{
  bool exists = CompositeMap.count(entry.CompositeName);
  CompositeMap[entry.CompositeName] = entry;
  return exists;
}

bool CompositeMapperFile::RemoveEntry(const CompositeEntry& entry)
{
  if (CompositeMap.count(entry.CompositeName))
  {
    CompositeMap.erase(entry.CompositeName);
    return true;
  }
  return false;
}

void CompositeMapperFile::SerializeCompositeMapFromString(const std::string& decrypted)
{
  size_t pos = 0;
  size_t posEnd = 0;
  CompositeMap.clear();
  while ((pos = decrypted.find('?', posEnd)) != std::string::npos)
  {
    std::string filename = decrypted.substr(posEnd, pos - posEnd);
    posEnd = decrypted.find('!', pos);
    do
    {
      pos++;
      CompositeEntry entry;
      entry.Filename = filename;

      size_t elementEnd = decrypted.find(',', pos);
      entry.ObjectPath = decrypted.substr(pos, elementEnd - pos);
      std::swap(pos, elementEnd);
      pos++;

      elementEnd = decrypted.find(',', pos);
      entry.CompositeName = decrypted.substr(pos, elementEnd - pos);
      std::swap(pos, elementEnd);
      pos++;

      elementEnd = decrypted.find(',', pos);
      entry.Offset = (int)std::stoul(decrypted.substr(pos, elementEnd - pos));
      std::swap(pos, elementEnd);
      pos++;

      elementEnd = decrypted.find(',', pos);
      entry.Size = (int)std::stoul(decrypted.substr(pos, elementEnd - pos));
      std::swap(pos, elementEnd);
      pos++;
      CompositeMap[entry.CompositeName] = entry;
    } while (decrypted.find('|', pos) < posEnd - 1);
    pos++;
    posEnd++;
  }
}

void CompositeMapperFile::SerializeCompositeMapToString(std::string& output)
{
  std::map<std::string, std::vector<CompositeEntry>> reverseMap;
  for (auto& pair : CompositeMap)
  {
    reverseMap[pair.second.Filename].push_back(pair.second);
  }

  output.reserve(SourceSize);
  
  for (const auto& pair : reverseMap)
  {
    output += pair.first + '?';
    for (const auto& item : pair.second)
    {
      output += item.ObjectPath + ',' + item.CompositeName + ',' + std::to_string(item.Offset) + ',' + std::to_string(item.Size) + ",|";
    }
    output += '!';
  }
}
