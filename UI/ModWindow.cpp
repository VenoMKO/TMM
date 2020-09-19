#include "ModWindow.h"
#include "RootDirWindow.h"
#include "../Utils.h"
#include "../TMM.h"

#include <wx/statline.h>
#include <wx/filename.h>
#include <fstream>

const char* MoreModsLink = "https://www.tumblr.com/tagged/tera+mods";
const char* UpdateLink = "https://github.com/VenoMKO/TMM/releases";

enum ControlElementId
{
	Add = 1000,
	Remove,
	AllOn,
	AllOff,
	GameDir,
	RestoreMapper,
	GetMods,
	GitHub
};

wxDEFINE_EVENT(RELOAD_MOD_LIST, wxCommandEvent);

ModWindow::ModWindow(wxWindow* parent, const std::vector<ModEntry>& entries, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
	, ModList(entries)
{
	SetIcon(wxICON(#101));
	SetSizeHints(wxDefaultSize, wxDefaultSize);
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	ModListView = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_MULTIPLE);
	bSizer2->Add(ModListView, 1, wxALL | wxEXPAND, 5);

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer(wxVERTICAL);

	wxPanel* m_panel1;
	m_panel1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME | wxTAB_TRAVERSAL);
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	AddButton = new wxButton(m_panel1, ControlElementId::Add, _("Add"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(AddButton, 1, wxALL, 5);

	RemoveButton = new wxButton(m_panel1, ControlElementId::Remove, _("Remove"), wxDefaultPosition, wxDefaultSize, 0);
	RemoveButton->Enable(false);

	bSizer4->Add(RemoveButton, 1, wxALL, 5);


	bSizer3->Add(bSizer4, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer(wxHORIZONTAL);

	OnButton = new wxButton(m_panel1, ControlElementId::AllOn, _("Turn on"), wxDefaultPosition, wxDefaultSize, 0);
	OnButton->Enable(false);

	bSizer41->Add(OnButton, 1, wxALL, 5);

	OffButton = new wxButton(m_panel1, ControlElementId::AllOff, _("Turn off"), wxDefaultPosition, wxDefaultSize, 0);
	OffButton->Enable(false);

	bSizer41->Add(OffButton, 1, wxALL, 5);


	bSizer3->Add(bSizer41, 0, wxEXPAND, 5);

	wxStaticLine* m_staticline1;
	m_staticline1 = new wxStaticLine(m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	bSizer3->Add(m_staticline1, 0, wxEXPAND | wxALL, 5);

	ChangeDirButton = new wxButton(m_panel1, ControlElementId::GameDir, _("Change game folder"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(ChangeDirButton, 0, wxALL | wxEXPAND, 5);

	RestoreButton = new wxButton(m_panel1, ControlElementId::RestoreMapper, _("Restore original .dat"), wxDefaultPosition, wxDefaultSize, 0);

	bSizer3->Add(RestoreButton, 0, wxALL | wxEXPAND, 5);

	wxStaticLine* m_staticline2;
	m_staticline2 = new wxStaticLine(m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	bSizer3->Add(m_staticline2, 0, wxEXPAND | wxALL, 5);

	MoreModsButton = new wxButton(m_panel1, ControlElementId::GetMods, _("Get more mods"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(MoreModsButton, 0, wxALL | wxEXPAND, 5);

	wxPanel* m_panel2;
	m_panel2 = new wxPanel(m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	bSizer3->Add(m_panel2, 1, wxEXPAND | wxALL, 5);

	ProgressBar = new wxGauge(m_panel1, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	ProgressBar->SetValue(-1);
	bSizer3->Add(ProgressBar, 0, wxALL, 5);


	m_panel1->SetSizer(bSizer3);
	m_panel1->Layout();
	bSizer3->Fit(m_panel1);
	bSizer12->Add(m_panel1, 1, wxALL, 5);

	wxHyperlinkCtrl* GitHubLink;
	GitHubLink = new wxHyperlinkCtrl(this, ControlElementId::GitHub, wxString::Format(_("Tera Mod Manager v.%d.%d"), APP_VER_MAJOR, APP_VER_MINOR), UpdateLink, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
	bSizer12->Add(GitHubLink, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);


	bSizer2->Add(bSizer12, 0, wxEXPAND, 5);


	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);


	SetSizer(bSizer1);
	Layout();

	Centre(wxBOTH);

	ModListView->AppendToggleColumn(_("On/Off"), ModUIModel::Col_Check, wxDATAVIEW_CELL_ACTIVATABLE, 55);
	ModListView->AppendTextColumn(_("Name"), ModUIModel::Col_Name, wxDATAVIEW_CELL_INERT, 270, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
	ModListView->AppendTextColumn(_("Author"), ModUIModel::Col_Author, wxDATAVIEW_CELL_INERT, 100, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
	ModListView->AppendTextColumn(_("File"), ModUIModel::Col_File, wxDATAVIEW_CELL_INERT, 130, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);

	// Connect Events
	AddButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnAddClicked), NULL, this);
	RemoveButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnRemoveClicked), NULL, this);
	OnButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnTurnOnClicked), NULL, this);
	OffButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnTurnOffClicked), NULL, this);
	ChangeDirButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnChangeDirClicked), NULL, this);
	RestoreButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnResetClicked), NULL, this);
	MoreModsButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnMoreModsClicked), NULL, this);
	Connect(wxEVT_IDLE, wxIdleEventHandler(ModWindow::OnIdle), NULL, this);
	ProgressBar->Show(false);
}

bool ModWindow::OnModStateChange(ModEntry& mod)
{
	bool ok = false;
	if (mod.Enabled)
	{
		ok = TurnOnMod(mod.Mod);
	}
	else
	{
		ok = TurnOffMod(mod.Mod);
	}

	if (ok)
	{
		try
		{
			CompositeMap.Save();
		}
		catch (...)
		{
			wxMessageBox(_("Failed to save the CompositePackageMapper.dat!"), _("Error!"), wxICON_ERROR);
			CompositeMap.Reload();
			mod.Enabled = !mod.Enabled;
			return false;
		}
		GetApp()->UpdateModsList(ModList);
	}
	else
	{
		CompositeMap.Reload();
		mod.Enabled = !mod.Enabled;
		wxQueueEvent(this, new wxCommandEvent(RELOAD_MOD_LIST));
		return false;
	}
	return true;
}

void ModWindow::OnAddClicked(wxCommandEvent& event)
{
	wxFileDialog fileDialog(this, wxT("Add mods"), wxEmptyString,
		wxEmptyString, wxT("Tera Game Package (*.gpk)|*.gpk"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);

	if (fileDialog.ShowModal() != wxID_OK)
	{
		return;
	}
	wxArrayString result;
	fileDialog.GetPaths(result);
	if (result.empty())
	{
		return;
	}
	bool needsSave = false;
	for (const wxString& path : result)
	{
		if (InstallMod(path.ToStdWstring(), false))
		{
			needsSave = true;
		}
	}
	if (needsSave)
	{
		CompositeMap.Save();
	}

	GetApp()->UpdateModsList(ModList);
	wxQueueEvent(this, new wxCommandEvent(RELOAD_MOD_LIST));
}

void ModWindow::OnRemoveClicked(wxCommandEvent& event)
{
	wxDataViewItemArray selection;
	ModListView->GetSelections(selection);
	if (!selection.GetCount())
	{
		return;
	}
	else if (selection.GetCount() == 1)
	{
		ModEntry& e = ModList[int(selection[0].GetID()) - 1];
		wxString modName = e.Mod.ModName.size() ? e.Mod.ModName : (e.File + ".gpk");
		if (wxMessageBox(wxString::Format(_("You are sure you want to delete the \"%s\" mod?"), modName.c_str()), _("Warning!"), wxICON_EXCLAMATION | wxYES_NO) != wxYES)
		{
			return;
		}
	}
	else
	{
		if (wxMessageBox(_("You are sure you want to delete all selected mods?"), _("Warning!"), wxICON_EXCLAMATION | wxYES_NO) != wxYES)
		{
			return;
		}
	}

	std::vector<ModEntry> itemsToRemove;
	for (auto& item : selection)
	{
		itemsToRemove.push_back(ModList[int(item.GetID()) - 1]);
	}
	for (ModEntry& e : itemsToRemove)
	{
		wxString modName = e.Mod.ModName.size() ? e.Mod.ModName : (e.File + ".gpk");
		if (e.Enabled && !TurnOffMod(e.Mod))
		{
			continue;
		}

		std::error_code err;
		if (std::filesystem::exists(GetApp()->GetModsDir() / (e.File + ".gpk")) && !std::filesystem::remove(GetApp()->GetModsDir() / (e.File + ".gpk"), err))
		{
			wxMessageBox(_("Failed to delete the ") + e.File + ".gpk!", _("Error!"), wxICON_ERROR);
			continue;
		}
		ModList.erase(std::remove(ModList.begin(), ModList.end(), e), ModList.end());
	}
	
	GetApp()->UpdateModsList(ModList);
	wxQueueEvent(this, new wxCommandEvent(RELOAD_MOD_LIST));
}

void ModWindow::OnTurnOnClicked(wxCommandEvent& event)
{
	wxDataViewItemArray selection;
	ModListView->GetSelections(selection);
	if (!selection.GetCount())
	{
		return;
	}
	std::vector<ModEntry*> items;
	for (auto& item : selection)
	{
		items.push_back(&ModList[int(item.GetID()) - 1]);
	}
	std::vector<bool> tmp;
	for (ModEntry* e : items)
	{
		tmp.push_back(e->Enabled);
		if (!e->Enabled)
		{
			e->Enabled = true;
			if (!TurnOnMod(e->Mod))
			{
				e->Enabled = false;
			}
		}
	}

	try
	{
		CompositeMap.Save();
	}
	catch (...)
	{
		wxMessageBox(_("Failed to save the CompositePackageMapper.dat!"), _("Error!"), wxICON_ERROR);
		CompositeMap.Reload();
		int idx = 0;
		for (ModEntry* e : items)
		{
			e->Enabled = tmp[idx];
			idx++;
		}
		return;
	}
	GetApp()->UpdateModsList(ModList);
	wxQueueEvent(this, new wxCommandEvent(RELOAD_MOD_LIST));
}

void ModWindow::OnTurnOffClicked(wxCommandEvent& event)
{
	wxDataViewItemArray selection;
	ModListView->GetSelections(selection);
	if (!selection.GetCount())
	{
		return;
	}
	std::vector<ModEntry*> items;
	for (auto& item : selection)
	{
		items.push_back(&ModList[int(item.GetID()) - 1]);
	}
	std::vector<bool> tmp;
	for (ModEntry* e : items)
	{
		tmp.push_back(e->Enabled);
		if (e->Enabled)
		{
			e->Enabled = false;
			if (!TurnOffMod(e->Mod))
			{
				e->Enabled = true;
			}
		}
	}

	try
	{
		CompositeMap.Save();
	}
	catch (...)
	{
		wxMessageBox(_("Failed to save the CompositePackageMapper.dat!"), _("Error!"), wxICON_ERROR);
		CompositeMap.Reload();
		int idx = 0;
		for (ModEntry* e : items)
		{
			e->Enabled = tmp[idx];
			idx++;
		}
		return;
	}

	GetApp()->UpdateModsList(ModList);
	wxQueueEvent(this, new wxCommandEvent(RELOAD_MOD_LIST));
}

void ModWindow::OnChangeDirClicked(wxCommandEvent& event)
{
	std::filesystem::path path;
	RootDirWindow rootWin = RootDirWindow(this, GetApp()->GetRootDir().wstring());
	while (1)
	{
		if (rootWin.ShowModal() != wxID_OK)
		{
			return;
		}
		path = rootWin.GetPath().ToStdWstring();
		if (!std::filesystem::exists(path))
		{
			wxMessageBox(_("Failed to open the folder. Try a different location."), _("Error!"), wxICON_ERROR);
			continue;
		}
		break;
	}
	GetApp()->ChangeRootDir(path);
	Close();
}

void ModWindow::OnResetClicked(wxCommandEvent& event)
{
	if (ModList.size() && wxMessageBox(_("Restoring original CompositePackageMapper.dat will turn off all of your mods.\nDo you want to continue?"), _("Warning!"), wxICON_INFORMATION | wxYES_NO) != wxYES)
	{
		return;
	}
	if (!std::filesystem::exists(GetApp()->GetBackupCompositeMapperPath()))
	{
		wxMessageBox(_("Try to delete CompositePackageMapperFile.dat and repair the client using game's launcher."), _("Couldn't locate the original file!"), wxICON_ERROR);
		return;
	}

	std::error_code err;
	if (std::filesystem::exists(GetApp()->GetCompositeMapperPath()) && !std::filesystem::remove(GetApp()->GetCompositeMapperPath(), err))
	{
		wxMessageBox(_("Couldn't delete the CompositePackageMapperFile.dat file! Make sure the game is not running!"), _("Error!"), wxICON_ERROR);
		return;
	}

	for (ModEntry& entry : ModList)
	{
		entry.Enabled = false;
	}
	GetApp()->UpdateModsList(ModList);

	if (!std::filesystem::copy_file(GetApp()->GetBackupCompositeMapperPath(), GetApp()->GetCompositeMapperPath(), err))
	{
		wxMessageBox(_("Couldn't to restore the CompositePackageMapperFile.dat file! Try to repair the client using game's launcher!"), _("Error!"), wxICON_ERROR);
		GetApp()->ExitMainLoop();
		return;
	}

	// Force reload
	GetApp()->ChangeRootDir(GetApp()->GetRootDir());
	Close();
}

void ModWindow::OnMoreModsClicked(wxCommandEvent& event)
{
	wxLaunchDefaultBrowser(MoreModsLink);
}

void ModWindow::OnIdle(wxIdleEvent& event)
{
	Disconnect(wxEVT_IDLE, wxIdleEventHandler(ModWindow::OnIdle), NULL, this);

	std::vector<ModEntry> missing;
	for (ModEntry& entry : ModList)
	{
		std::filesystem::path path(GetApp()->GetModsDir() / entry.File);
		path.replace_extension(".gpk");
		if (!std::filesystem::exists(path))
		{
			missing.push_back(entry);
		}
		std::ifstream s(path, std::ios::binary | std::ios::in);
		s >> entry.Mod;
		if (entry.Mod.Container.empty())
		{
			entry.Mod.ModName = path.filename().string();
			entry.Mod.ModAuthor = "-";
			entry.Mod.Container = path.filename().string();
		}
	}

	bool teraUpdated = false;
	try
	{
		CompositeMap = CompositeMapperFile(GetApp()->GetCompositeMapperPath().wstring());
		if (!CompositeMap.IsMarked())
		{
			CompositeMap.Mark();
			CompositeMap.Save();
			GetApp()->BackupCompositeMapperFile();
			teraUpdated = true;
		}
		if (!std::filesystem::exists(GetApp()->GetBackupCompositeMapperPath()))
		{
			if (teraUpdated)
			{
				wxMessageBox(_("Failed to create ") + GetApp()->GetBackupCompositeMapperPath().filename().wstring() + _(" file!"), _("Error!"), wxICON_ERROR);
			}
			else
			{
				wxString msg = _("File ") + GetApp()->GetBackupCompositeMapperPath().filename().wstring();
				msg += _(" is missing!\nIf you have delete it, please restore it and restart the TMM");
				msg += _(", otherwise delete the CompositePackageMapper.dat and repair your client!");
				wxMessageBox( msg, _("Error!"), wxICON_ERROR);
			}
			GetApp()->ExitMainLoop();
			return;
		}
		BackupMap = CompositeMapperFile(GetApp()->GetBackupCompositeMapperPath().wstring());
	}
	catch (...)
	{
		wxMessageBox(_("Failed to load the CompositePackageMapper.dat"), _("Error!"), wxICON_ERROR);
		GetApp()->ChangeRootDir({});
		Close();
	}

	bool saveList = false;
	if (teraUpdated)
	{
		for (ModEntry& entry : ModList)
		{
			if (!entry.Enabled)
			{
				continue;
			}

			// Find first turned on mod and ask user. On NO turn off all mods.
			if (wxMessageBox(_("Do you want to enable mods you used with the previous version?"), _("Your game was updated or repaired"), wxICON_INFORMATION | wxYES_NO) != wxYES)
			{
				saveList = true;
				for (ModEntry& tentry : ModList)
				{
					tentry.Enabled = false;
				}
			}
			break;
		}
	}

	// Turn on all mods to make sure tmm is in sync with the map file
	for (ModEntry& entry : ModList)
	{
		if (!entry.Enabled)
		{
			if (!teraUpdated)
			{
				// Make sure modlist and dat file are synced
				TurnOffMod(entry.Mod, /*silent*/true);
			}
			continue;
		}
		if (!TurnOnMod(entry.Mod))
		{
			saveList = true;
			entry.Enabled = false;
		}
	}

	CompositeMap.Save();

	if (saveList)
	{
		GetApp()->UpdateModsList(ModList);
	}

	wxQueueEvent(this, new wxCommandEvent(RELOAD_MOD_LIST));
}

void ModWindow::OnRealoadModList(wxCommandEvent&)
{
	ModListView->AssociateModel(new ModUIModel(ModList, this));
	OffButton->Enable(ModListView->HasSelection());
	OnButton->Enable(ModListView->HasSelection());
	RemoveButton->Enable(ModListView->HasSelection());
	for (const auto& mod : ModList)
	{
		if (mod.Enabled)
		{
			OffButton->Enable(true);
			if (OffButton->IsEnabled() && OnButton->IsEnabled())
			{
				break;
			}
		}
		else
		{
			OnButton->Enable(true);
			if (OffButton->IsEnabled() && OnButton->IsEnabled())
			{
				break;
			}
		}
	}
}

void ModWindow::OnModSelectionChanged(wxDataViewEvent& event)
{
	OnButton->Enable(ModListView->HasSelection());
	OffButton->Enable(ModListView->HasSelection());
	RemoveButton->Enable(ModListView->HasSelection());
}

bool ModWindow::InstallMod(const std::wstring& path, bool save)
{
	std::ifstream s(path, std::ios::binary | std::ios::in);
	ModFile mod;
	s >> mod;
	if (mod.Packages.empty())
	{
		std::filesystem::path item(path);
		wxMessageBox(wxString::Format(_("File \"%s\" is not a mod!"), item.c_str()), _("Error!"), wxICON_ERROR);
		return false;
	}

	if (mod.Container.empty())
	{
		wxString name;
		wxFileName::SplitPath(path, nullptr, nullptr, &name, nullptr);
		mod.ModName = name;
		mod.ModAuthor = "-";
		mod.Container = name;
	}

	// Check if the GPK is ok
	for (const auto& package : mod.Packages)
	{
		if (package.FileVersion == VER_TERA_MODERN)
		{
			CompositeEntry unused;
			if (package.ObjectPath.empty())
			{
				// One of the packages is not a composite package.
				wxMessageBox(wxString::Format(_("One of the packages in \"%s\" mod is not composite.\nTry to copy this file to a \"_Mods\" folder or ask mod author how to install it!"), mod.ModName.c_str()), _("Failed to add the mod!"), wxICON_ERROR);
				return false;
			}
			else
			{
				if (mod.RegionLock)
				{
					if (!CompositeMap.GetEntryByObjectPath(package.ObjectPath, unused))
					{
						wxMessageBox(wxString::Format(_("Can't add \"%s\" mod because it is not compatible with your region.\nAsk mod author for more details!"), mod.ModName.c_str()), _("Failed to add the mod!"), wxICON_ERROR);
						return false;
					}
				}
				else
				{
					if (!CompositeMap.GetEntryByIncompleteObjectPath(package.ObjectPath, unused))
					{
						wxMessageBox(wxString::Format(_("Failed to add \"%s\" mod because your client has no \"%s\".\nTry to disable all mods, or contact mod author!"), mod.ModName.c_str(), package.ObjectPath.c_str()), _("Failed to add the mod!"), wxICON_ERROR);
						return false;
					}
				}
			}
			continue;
		}
		if (package.FileVersion == VER_TERA_CLASSIC)
		{
			wxMessageBox(wxString::Format(_("Can't add \"%s\" mod because it is a 32-bit mod! Ask mod author to update it!"), mod.ModName.c_str()), _("Failed to add the mod!"), wxICON_ERROR);
		}
		else
		{
			wxMessageBox(wxString::Format(_("Can't add \"%s\" mod! Unknown package version %uh."), mod.ModName.c_str(), package.FileVersion), _("Error!"), wxICON_ERROR);
		}
		return false;
	}

	std::filesystem::path source(path);
	std::filesystem::path dest = GetApp()->GetModsDir() / (mod.Container + ".gpk");

	// Search for conflicts
	int entryToDelete = -1;
	for (int idx = 0; idx < ModList.size(); ++idx)
	{
		const ModEntry& entry = ModList[idx];
		if (entry.Mod.ModName == mod.ModName && entry.Mod.ModAuthor == mod.ModAuthor)
		{
			entryToDelete = idx;
			if (wxMessageBox(wxString::Format(_("You already have \"%s\" installed.\nDo you want to update it?"), mod.ModName.c_str()), _("This mod already exists!"), wxICON_INFORMATION | wxYES_NO) != wxYES)
			{
				return false;
			}
			break;
		}

		// Check if the mod GPK file already exists under another mod
		if (entry.Mod.Container == mod.Container && std::filesystem::exists(dest))
		{
			auto result = wxMessageBox(wxString::Format(_("You already have \"%s\" file installed as a mod \"%s\".\nDo you want to replace it with the new one?"), mod.Container.c_str(), entry.Mod.ModName.c_str()),
				_("File already exists!"), wxICON_INFORMATION | wxYES_NO);
			if (result != wxYES)
			{
				return false;
			}
			std::error_code err;
			if (!std::filesystem::remove(dest, err))
			{
				wxMessageBox(wxString::Format(_("Failed to delete the old mod file \"%s\""), dest.filename().c_str()), _("Error!"), wxICON_ERROR);
				return false;
			}

			entryToDelete = idx;
			break;
		}
	}

	// Copy the mod file unless in is already in the mod dir
	if (source != dest)
	{
		std::error_code err;
		if (std::filesystem::exists(dest))
		{
			if (!std::filesystem::remove(dest, err))
			{
				wxMessageBox(wxString::Format(_("Failed to copy \"%s\" to the mod folder. File already exists!"), source.filename().c_str()), _("Error!"), wxICON_ERROR);
				return false;
			}
		}
		if (!std::filesystem::copy_file(source, dest, err))
		{
			wxMessageBox(wxString::Format(_("Failed to copy \"%s\" to the mod folder. Try to run TMM as an administrator."), source.filename().c_str()), _("Error!"), wxICON_ERROR);
			return false;
		}
	}

	if (entryToDelete >= 0)
	{
		ModList.erase(ModList.begin() + entryToDelete);
	}

	ModEntry newMod;
	if (TurnOnMod(mod))
	{
		newMod.Enabled = true;
		if (save)
		{
			CompositeMap.Save();
		}
	}

	newMod.File = mod.Container;
	newMod.Mod = mod;
	ModList.push_back(newMod);
	return true;
}

bool ModWindow::TurnOnMod(const ModFile& mod)
{
	// First run to check if we can turn this ON
	// Check if any of the imported composite GPKs already used
	for (const ModEntry& existingEntry : ModList)
	{
		if (!existingEntry.Enabled || (&existingEntry.Mod == &mod))
		{
			continue;
		}
		for (const ModFile::CompositePackage& existingPackage : existingEntry.Mod.Packages)
		{
			const std::string& objPath = existingPackage.ObjectPath;
			if (std::any_of(mod.Packages.begin(), mod.Packages.end(), [&objPath](const ModFile::CompositePackage& p) { return IncompletePathsEqual(objPath, p.ObjectPath); }))
			{
				const char* newModName = mod.ModName.c_str();
				const char* existingModName = existingEntry.Mod.ModName.c_str();
				wxMessageBox(wxString::Format(_("Can't turn on \"%s\" mod because it's conflicting with the \"%s\" mod.\nConflicting path: \"%s\"\nTry to turn off \"%s\" mod first."), newModName, existingModName, objPath.c_str(), existingModName),
					_("Mod conflict!"), wxICON_ERROR);
				return false;
			}
		}
	}
	
	for (const auto& package : mod.Packages)
	{
		CompositeEntry entry;
		if (mod.RegionLock)
		{
			if (!CompositeMap.GetEntryByObjectPath(package.ObjectPath, entry))
			{
				wxMessageBox(wxString::Format(_("Failed to get composite entry: %s"), package.ObjectPath.c_str()), _("Error!"), wxICON_ERROR);
				return false;
			}
		}
		else
		{
			if (!CompositeMap.GetEntryByIncompleteObjectPath(package.ObjectPath, entry))
			{
				wxMessageBox(wxString::Format(_("Failed to get composite entry: %s"), package.ObjectPath.c_str()), _("Error!"), wxICON_ERROR);
				return false;
			}
		}
		
	}

	for (const auto& package : mod.Packages)
	{
		CompositeEntry entry;
		if (mod.RegionLock)
		{
			CompositeMap.GetEntryByObjectPath(package.ObjectPath, entry);
		}
		else
		{
			CompositeMap.GetEntryByIncompleteObjectPath(package.ObjectPath, entry);
		}
		entry.Filename = mod.Container;
		entry.Offset = package.Offset;
		entry.Size = package.Size;
		CompositeMap.AddEntry(entry);
	}
}

bool ModWindow::TurnOffMod(const ModFile& mod, bool silent)
{
	std::vector<CompositeEntry> backup;
	for (const auto& package : mod.Packages)
	{
		CompositeEntry entry;
		if (!BackupMap.GetEntryByIncompleteObjectPath(package.ObjectPath, entry))
		{
			if (!silent)
			{
				wxMessageBox(wxString::Format(_("Failed to find original entry: %s. Try to restore the CompositePackageMapper.dat file."), package.ObjectPath.c_str()), _("Error!"), wxICON_ERROR);
			}
			return false;
		}
		backup.push_back(entry);
	}
	for (const CompositeEntry& entry : backup)
	{
		CompositeMap.AddEntry(entry);
	}
	return true;
}

wxBEGIN_EVENT_TABLE(ModWindow, wxFrame)
EVT_COMMAND(wxID_ANY, RELOAD_MOD_LIST, ModWindow::OnRealoadModList)
EVT_DATAVIEW_SELECTION_CHANGED(wxID_ANY, ModWindow::OnModSelectionChanged)
wxEND_EVENT_TABLE()