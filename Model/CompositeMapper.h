#pragma once
#include <string>
#include <map>
#include <vector>

struct CompositeEntry {
  std::string Filename;
  std::string ObjectPath;
  std::string CompositeName;
  int Offset = 0;
  int Size = 0;
};

class CompositeMapperFile {
public:
  CompositeMapperFile()
  {}

  CompositeMapperFile(const std::wstring& source);

  void Save();
  
  // Get an entry by a composite package name. Returns true if found.
  bool GetEntryByCompositeName(const std::string& compositeName, CompositeEntry& output);
  // Get an entry by its incomplete object path. Returns true if found.
  bool GetEntryByIncompleteObjectPath(const std::string& incompletePath, CompositeEntry& output);
  // Add/Modify an entry. Returns true if added, false - modified
  bool AddEntry(const CompositeEntry& entry);
  // Remove and entry. Returns true if removed.
  bool RemoveEntry(const CompositeEntry& entry);

private:
  void SerializeCompositeMapFromString(const std::string& decrypted);
  void SerializeCompositeMapToString(std::string& output);

private:
  std::wstring SourcePath;
  size_t SourceSize = 0;
  std::map<std::string, CompositeEntry> CompositeMap;
};