// TMM.cpp : Defines the entry point for the application.
//

#include "TMM.h"
#include "UI/ModWindow.h"

wxIMPLEMENT_APP(TMM);

int TMM::OnRun()
{
  ModWindow* mainWindow = new ModWindow(nullptr);
  mainWindow->Show();
  return wxApp::OnRun();
}
