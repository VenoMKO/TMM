// TMM.cpp : Defines the entry point for the application.
//

#include "TMM.h"
#include "UI/ModWindow.h"
#include "UI/RootDirWindow.h"

wxIMPLEMENT_APP(TMM);

void TMM::LoadConfig()
{
}

int TMM::OnRun()
{
  LoadConfig();

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
  BackupCompositeMapperPath = ModsDir / L"CompositePackageMapper.dat";

  if (!std::filesystem::exists(ModsDir))
  {
    std::filesystem::create_directory(ModsDir);
    std::filesystem::copy_file(CompositeMapperPath, BackupCompositeMapperPath);
  }
  else if (!std::filesystem::exists(ModsDir))
  {
    std::filesystem::copy_file(CompositeMapperPath, BackupCompositeMapperPath);
  }

  ModWindow* mainWindow = new ModWindow(nullptr);
  mainWindow->Show();
  return wxApp::OnRun();
}
