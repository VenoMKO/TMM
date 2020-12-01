#pragma once
#include <wx/wx.h>

#include <vector>
#include <filesystem>

#include "Model/Mod.h"

#define APP_VER_MAJOR 1
#define APP_VER_MINOR 1

class TMM : public wxApp {
public:
  void LoadAppConfig();
  void SaveAppConfig();

  void LoadGameConfig();
  void SaveGameConfig();

  void ChangeRootDir(const std::filesystem::path& newRootDir);
  void UpdateModsList(const std::vector<ModEntry> modData);

  bool BackupCompositeMapperFile();

  inline std::filesystem::path GetRootDir() const
  {
    return RootDir;
  }

  inline std::filesystem::path GetModsDir() const
  {
    return ModsDir;
  }

  inline std::filesystem::path GetCompositeMapperPath() const
  {
    return CompositeMapperPath;
  }

  inline std::filesystem::path GetBackupCompositeMapperPath() const
  {
    return BackupCompositeMapperPath;
  }

  inline std::filesystem::path GetGameConfigPath() const
  {
    return GameConfigPath;
  }

protected:
  int OnRun() override;

  bool SetupPaths();

private:
  std::filesystem::path RootDir; // S1Game
  std::filesystem::path ModsDir; // S1Game/_CookedPC
  std::filesystem::path CompositeMapperPath; // S1Game/CookedPC/CompositePackageMapper.dat
  std::filesystem::path BackupCompositeMapperPath; // S1Game/_CookedPC/CompositePackageMapper.backup
  std::filesystem::path GameConfigPath; // S1Game/_CookedPC/Config.tmm

  GameConfigFile GameConfig;
};

inline TMM* GetApp()
{
  return (TMM*)wxTheApp;
}