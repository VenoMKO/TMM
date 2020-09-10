#pragma once
#include <string>
#include <fstream>
#include <vector>

struct ModInfo {
  std::string File;
  std::string IncompleteObjectPath;
  bool Enabled = false;
};

struct GameConfigFile {
  friend std::ifstream& operator>>(std::ifstream& s, GameConfigFile& cfg);
  friend std::ofstream& operator<<(std::ofstream& s, GameConfigFile& cfg);

  std::vector<ModInfo> Mods;
};

struct Mod {
  struct CompositePackage {
    friend std::ifstream& operator>>(std::ifstream& s, CompositePackage& p);

    std::string IncompleteObjectPath;
    int Offset = 0;
    int Size = 0;
    unsigned short FileVersion = 0;
    unsigned short LicenseeVersion = 0;
  };

  friend std::ifstream& operator>>(std::ifstream& s, Mod& m);

  std::string ModName;
  std::string Container;
  std::string ModAuthor;
  std::vector<CompositePackage> Packages;
};

