// TMM.cpp : Defines the entry point for the application.
//

#include "TMM.h"
#include "UI/ModWindow.h"
#include "UI/RootDirWindow.h"

#include <wx/stdpaths.h>
#include <wx/fileconf.h>

wxIMPLEMENT_APP(TMM);

wxString GetConfigPath()
{
  wxString path = wxStandardPaths::Get().GetUserLocalDataDir() + wxFILE_SEP_PATH;
  if (!wxDirExists(path))
  {
    wxMkDir(path);
  }
  path += wxS("Settings.ini");
  return path;
}

void TMM::LoadAppConfig()
{
  wxString path = GetConfigPath();
  wxFileConfig cfg(wxEmptyString, wxEmptyString, path);
  cfg.SetPath(path);

  RootDir = cfg.Read(wxT("RootDir"), wxT("")).ToStdWstring();
}

void TMM::SaveAppConfig()
{
  wxString path = GetConfigPath();
  wxFileConfig cfg(wxEmptyString, wxEmptyString, path);
  cfg.SetPath(path);

  wxString tmpPath = RootDir.wstring();
  cfg.Write(wxT("RootDir"), tmpPath);
}

void TMM::LoadGameConfig()
{
  if (!std::filesystem::exists(GameConfigPath))
  {
    SaveGameConfig();
  }
  std::ifstream s(GameConfigPath, std::ios::binary | std::ios::in);
  s >> GameConfig;
}

void TMM::SaveGameConfig()
{
  std::ofstream s(GameConfigPath, std::ios::binary | std::ios::out);
  s << GameConfig;
}

int TMM::OnRun()
{
  LoadAppConfig();

  if (RootDir.empty() || !std::filesystem::exists(RootDir))
  {
    RootDirWindow rooWin(nullptr, RootDir.wstring());
    if (rooWin.ShowModal() == wxID_OK)
    {
      RootDir = rooWin.GetPath().ToStdWstring();
    }
    if (RootDir.empty() || !std::filesystem::exists(RootDir))
    {
      return 0;
    }
  }

  CompositeMapperPath = RootDir / L"CookedPC" / L"CompositePackageMapper.dat";

  while (!std::filesystem::exists(CompositeMapperPath))
  {
    wxMessageBox(wxS("CompositePackageMapper.dat file was not found in the game folder. Select a new S1Game folder."), wxS("Error!"), wxICON_ERROR);
    RootDirWindow rooWin(nullptr, RootDir.wstring());
    if (rooWin.ShowModal() == wxID_OK)
    {
      RootDir = rooWin.GetPath().ToStdWstring();
      CompositeMapperPath = RootDir / L"CookedPC" / L"CompositePackageMapper.dat";
    }
    if (RootDir.empty() || !std::filesystem::exists(RootDir))
    {
      return 0;
    }
  }

  ModsDir = RootDir / L"_CookedPC";
  BackupCompositeMapperPath = ModsDir / L"CompositePackageMapper.backup";
  GameConfigPath = ModsDir / L"Config.tmm";

  if (!std::filesystem::exists(ModsDir))
  {
    std::filesystem::create_directory(ModsDir);
    std::filesystem::copy_file(CompositeMapperPath, BackupCompositeMapperPath);
  }

  if (!std::filesystem::exists(BackupCompositeMapperPath))
  {
    std::filesystem::copy_file(CompositeMapperPath, BackupCompositeMapperPath);
  }

  SaveAppConfig();
  LoadGameConfig();
  ModWindow* mainWindow = new ModWindow(nullptr);
  mainWindow->Show();
  return wxApp::OnRun();
}
