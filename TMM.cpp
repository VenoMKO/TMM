// TMM.cpp : Defines the entry point for the application.
//

#include "TMM.h"
#include "UI/ModWindow.h"
#include "UI/RootDirWindow.h"

#include <wx/stdpaths.h>
#include <wx/fileconf.h>

wxIMPLEMENT_APP(TMM);
  

const char* ConfigFile = "Settings.ini";
const char* ConfigFileRootKey = "RootDir";
const char* ConfigFileWaitForTeraKey = "WaitForTera";

const char* GameConfigFilePath = "ModList.tmm";

const char* CompositeMapperFile = "CompositePackageMapper.dat";
const char* CompositeMapperBackupFile = "CompositePackageMapper.clean";

const char* CookedPcDir = "CookedPC";
const char* ModsStorageDir = "CookedPC";


wxString GetConfigPath()
{
  wxString path = wxStandardPaths::Get().GetUserLocalDataDir() + wxFILE_SEP_PATH;
  if (!wxDirExists(path))
  {
    wxMkDir(path);
  }
  path += ConfigFile;
  return path;
}

void TMM::LoadAppConfig()
{
  wxString path = GetConfigPath();
  wxFileConfig cfg(wxEmptyString, wxEmptyString, path);
  cfg.SetPath(path);

  RootDir = cfg.Read(ConfigFileRootKey, wxEmptyString).ToStdWstring();
  WaitForTera = cfg.ReadBool(ConfigFileWaitForTeraKey, false);
}

void TMM::SaveAppConfig()
{
  wxString path = GetConfigPath();
  wxFileConfig cfg(wxEmptyString, wxEmptyString, path);
  cfg.SetPath(path);

  wxString tmpPath = RootDir.wstring();
  cfg.Write(ConfigFileRootKey, tmpPath);
  cfg.Write(ConfigFileWaitForTeraKey, WaitForTera);
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

void TMM::ChangeRootDir(const std::filesystem::path& newRootDir)
{
  RootDir = newRootDir;
  if (!SetupPaths())
  {
    ExitMainLoop();
    return;
  }
  SaveAppConfig();
  GameConfig.Mods.clear();
  LoadGameConfig();
  ModWindow* mainWindow = new ModWindow(nullptr, GameConfig.Mods);
  mainWindow->Show();
}

void TMM::UpdateModsList(const std::vector<ModEntry> modData)
{
  GameConfig.Mods = modData;
  SaveGameConfig();
}

bool TMM::BackupCompositeMapperFile()
{
  if (!std::filesystem::exists(CompositeMapperPath))
  {
    return false;
  }
  std::error_code err;
  if (!std::filesystem::exists(BackupCompositeMapperPath))
  {
    return std::filesystem::copy_file(CompositeMapperPath, BackupCompositeMapperPath, err);
  }
  else
  {
    if (!std::filesystem::remove(BackupCompositeMapperPath, err))
    {
      return false;
    }
    return std::filesystem::copy_file(CompositeMapperPath, BackupCompositeMapperPath, err);
  }
  return true;
}

int TMM::OnRun()
{
  LoadAppConfig();

  if (!SetupPaths())
  {
    return 0;
  }

  SaveAppConfig();

  LoadGameConfig();
  ModWindow* mainWindow = new ModWindow(nullptr, GameConfig.Mods);
  mainWindow->Show();
  return wxApp::OnRun();
}

bool TMM::SetupPaths()
{
  if (RootDir.empty() || !std::filesystem::exists(RootDir))
  {
    RootDirWindow rooWin(nullptr, RootDir.wstring());
    if (rooWin.ShowModal() == wxID_OK)
    {
      RootDir = rooWin.GetPath().ToStdWstring();
    }
    if (RootDir.empty() || !std::filesystem::exists(RootDir))
    {
      return false;
    }
  }

  CompositeMapperPath = RootDir / CookedPcDir / CompositeMapperFile;

  while (!std::filesystem::exists(CompositeMapperPath))
  {
    wxMessageBox(_("Couldn't find \"S1Game\\CookedPC\\CompositePackageMapper.dat\" file."), _("Error!"), wxICON_ERROR);
    RootDirWindow rooWin(nullptr, RootDir.wstring());
    if (rooWin.ShowModal() != wxID_OK)
    {
      return false;
    }
    RootDir = rooWin.GetPath().ToStdWstring();
    if (RootDir.empty() || !std::filesystem::exists(RootDir))
    {
      return false;
    }
    CompositeMapperPath = RootDir / CookedPcDir / CompositeMapperFile;
  }

  ModsDir = RootDir / ModsStorageDir;
  BackupCompositeMapperPath = ModsDir / CompositeMapperBackupFile;
  GameConfigPath = ModsDir / GameConfigFilePath;
  return true;
}
