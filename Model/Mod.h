#pragma once
#include <string>
#include <fstream>
#include <vector>

struct Mod {
  struct CompositePackage {
    friend std::ifstream& operator>>(std::ifstream& s, CompositePackage& p);

    std::string IncompleteObjectPath;
    int Offset = 0;
    int Size = 0;
  };

  friend std::ifstream& operator>>(std::ifstream& s, Mod& m);

  std::string ModName;
  std::string Container;
  std::string ModAuthor;
  std::vector<CompositePackage> Packages;
};

struct ModInfo {
  std::string Name;
  std::string Author;
  std::string Key;
  std::string File;
  bool Enabled = false;
};