#pragma once
#include <string>
#include <fstream>
#include <vector>

#define VER_TERA_FILEMOD 2

std::string GetString(std::ifstream& s);

// Struct represents a composite GPK file
struct ModFile {
  struct CompositePackage {
    friend std::ifstream& operator>>(std::ifstream& s, CompositePackage& p);

    bool operator==(const CompositePackage& a) const
    {
      return ObjectPath == a.ObjectPath && Offset == a.Offset && Size == a.Size && FileVersion == a.FileVersion && LicenseeVersion == a.LicenseeVersion;
    }

    std::string ObjectPath;
    int Offset = 0;
    int Size = 0;
    unsigned short FileVersion = 0;
    unsigned short LicenseeVersion = 0;
  };

  struct TfcPackage {
    friend std::ifstream& operator>>(std::ifstream& s, TfcPackage& p);
    int Offset = 0;
    int Size = 0;
    int Idx = 0;
    int IdxOffset = 0;
  };

  friend std::ifstream& operator>>(std::ifstream& s, ModFile& m);

  bool RegionLock = false;
  int ModFileVersion = 1;
  std::string ModName;
  std::string Container;
  std::string ModAuthor;
  std::vector<CompositePackage> Packages;
  std::vector<TfcPackage> TfcPackages;
};

// A mod entry serialized from a config file or created when a mod is added
struct ModEntry {
  std::string File;
  bool Enabled = false;
  ModFile Mod;

  bool operator==(const ModEntry& a) const
  {
    return File == a.File && Mod.Container == a.Mod.Container && Mod.ModAuthor == a.Mod.ModAuthor && Mod.ModName == a.Mod.ModName && Mod.Packages == a.Mod.Packages;
  }
};

// A file containing game-specific info
struct GameConfigFile {
  friend std::ifstream& operator>>(std::ifstream& s, GameConfigFile& cfg);
  friend std::ofstream& operator<<(std::ofstream& s, GameConfigFile& cfg);

  std::vector<ModEntry> Mods;
};