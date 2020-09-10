#pragma once
#include <wx/wx.h>

#include <vector>
#include <filesystem>

#include "Model/Mod.h"

class TMM : public wxApp {
public:
  void LoadAppConfig();
  void SaveAppConfig();

  void LoadGameConfig();
  void SaveGameConfig();

protected:
  int OnRun() override;

private:
  std::filesystem::path RootDir; // S1Game
  std::filesystem::path ModsDir; // S1Game/_CookedPC
  std::filesystem::path CompositeMapperPath; // S1Game/CookedPC/CompositePackageMapper.dat
  std::filesystem::path BackupCompositeMapperPath; // S1Game/_CookedPC/CompositePackageMapper.backup
  std::filesystem::path GameConfigPath; // S1Game/_CookedPC/Config.tmm

  GameConfigFile GameConfig;
};