#pragma once
#include <string>
#include <fstream>
#include <vector>

struct ModFile {
  struct CompositePackage {
    friend std::ifstream& operator>>(std::ifstream& s, CompositePackage& p);

    std::string IncompleteObjectPath;
    int Offset = 0;
    int Size = 0;
    unsigned short FileVersion = 0;
    unsigned short LicenseeVersion = 0;
  };

  friend std::ifstream& operator>>(std::ifstream& s, ModFile& m);

  std::string ModName;
  std::string Container;
  std::string ModAuthor;
  std::vector<CompositePackage> Packages;
};

struct ModEntry {
  std::string File;
  bool Enabled = false;
  ModFile Mod;
};

struct GameConfigFile {
  friend std::ifstream& operator>>(std::ifstream& s, GameConfigFile& cfg);
  friend std::ofstream& operator<<(std::ofstream& s, GameConfigFile& cfg);

  std::vector<ModEntry> Mods;
};