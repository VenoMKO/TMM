#pragma once
#include <wx/wx.h>

#include <vector>
#include <filesystem>

class TMM : public wxApp {
public:

protected:
  void LoadConfig();
  int OnRun() override;

private:
  std::filesystem::path RootDir;
  std::filesystem::path ModsDir;
  std::filesystem::path CompositeMapperPath;
  std::filesystem::path BackupCompositeMapperPath;
};