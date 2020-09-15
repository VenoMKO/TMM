#pragma once
#include <string>
#include <fstream>
#include <vector>

// Struct represents a composite GPK file
struct ModFile {
  struct CompositePackage {
    friend std::ifstream& operator>>(std::ifstream& s, CompositePackage& p);

    std::string GetIncompleteObjectPath() const
    {
      size_t pos = ObjectPath.find('.');
      if (pos == std::string::npos)
      {
        return ObjectPath;
      }
      return ObjectPath.substr(pos + 1);
    }

    std::string ObjectPath;
    int Offset = 0;
    int Size = 0;
    unsigned short FileVersion = 0;
    unsigned short LicenseeVersion = 0;
  };

  friend std::ifstream& operator>>(std::ifstream& s, ModFile& m);

  bool RegionLock = false;
  std::string ModName;
  std::string Container;
  std::string ModAuthor;
  std::vector<CompositePackage> Packages;
};

// A mod entry serialized from a config file or created when a mod is added
struct ModEntry {
  std::string File;
  bool Enabled = false;
  ModFile Mod;
};

// A file containing game-specific info
struct GameConfigFile {
  friend std::ifstream& operator>>(std::ifstream& s, GameConfigFile& cfg);
  friend std::ofstream& operator<<(std::ofstream& s, GameConfigFile& cfg);

  std::vector<ModEntry> Mods;
};