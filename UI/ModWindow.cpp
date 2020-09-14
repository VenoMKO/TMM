#include "ModWindow.h"
#include "RootDirWindow.h"
#include "../Utils.h"
#include "../TMM.h"

#include <wx/statline.h>
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
	SetSizeHints(wxDefaultSize, wxDefaultSize);
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	ModListView = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
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

	AllOnButton = new wxButton(m_panel1, ControlElementId::AllOn, _("Turn on all"), wxDefaultPosition, wxDefaultSize, 0);
	AllOnButton->Enable(false);

	bSizer41->Add(AllOnButton, 1, wxALL, 5);

	AllOffButton = new wxButton(m_panel1, ControlElementId::AllOff, _("Turn off all"), wxDefaultPosition, wxDefaultSize, 0);
	AllOffButton->Enable(false);

	bSizer41->Add(AllOffButton, 1, wxALL, 5);


	bSizer3->Add(bSizer41, 0, wxEXPAND, 5);

	wxStaticLine* m_staticline1;
	m_staticline1 = new wxStaticLine(m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	bSizer3->Add(m_staticline1, 0, wxEXPAND | wxALL, 5);

	ChangeDirButton = new wxButton(m_panel1, ControlElementId::GameDir, _("Change game folder"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(ChangeDirButton, 0, wxALL | wxEXPAND, 5);

	RestoreButton = new wxButton(m_panel1, ControlElementId::RestoreMapper, _("Restore original .dat"), wxDefaultPosition, wxDefaultSize, 0);
	RestoreButton->Enable(false);

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
	GitHubLink = new wxHyperlinkCtrl(this, ControlElementId::GitHub, _("Tera Mod Manager v.1.00"), UpdateLink, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
	bSizer12->Add(GitHubLink, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);


	bSizer2->Add(bSizer12, 0, wxEXPAND, 5);


	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);


	SetSizer(bSizer1);
	Layout();

	Centre(wxBOTH);

	ModListView->AppendToggleColumn(_("On/Off"), ModUIModel::Col_Check, wxDATAVIEW_CELL_ACTIVATABLE, 55);
	ModListView->AppendTextColumn(_("Name"), ModUIModel::Col_Name, wxDATAVIEW_CELL_INERT, 200, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
	ModListView->AppendTextColumn(_("Author"), ModUIModel::Col_Author, wxDATAVIEW_CELL_INERT, 100, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
	ModListView->AppendTextColumn(_("File"), ModUIModel::Col_File, wxDATAVIEW_CELL_INERT, 100, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);

	// Connect Events
	AddButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnAddClicked), NULL, this);
	RemoveButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnRemoveClicked), NULL, this);
	AllOnButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnTurnOnAllClicked), NULL, this);
	AllOffButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnTurnOffAllClicked), NULL, this);
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
			mod.Enabled != mod.Enabled;
			return false;
		}
		GetApp()->UpdateModsList(ModList);
	}
	else
	{
		CompositeMap.Reload();
		mod.Enabled != mod.Enabled;
		return false;
	}
	return true;
}

void ModWindow::OnAddClicked(wxCommandEvent& event)
{
	const wxString extensions = _("Package files (*.gpk; *.gmp; *.u; *.umap; *.upk)|*.gpk;*.gmp;*.u;*.umap;*.upk");
	wxString path = wxFileSelector(_("Open a package"), wxEmptyString, wxEmptyString, extensions, extensions, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (path.empty())
	{
		return;
	}

	std::ifstream s(path.ToStdWstring(), std::ios::binary | std::ios::in);
	ModFile mod;
	s >> mod;
	if (mod.Packages.empty())
	{
		wxMessageBox(_("File is not a mod!"), _("Error!"), wxICON_INFORMATION);
		return;
	}

	// Check if the GPK is ok
	for (const auto& package : mod.Packages)
	{
		if (package.FileVersion == VER_TERA_MODERN)
		{
			CompositeEntry unused;
			if (package.IncompleteObjectPath.empty())
			{
				// One of the packages is not a composite package.
				wxMessageBox(_("One of the packages in the mod is not composite."), _("Error!"), wxICON_ERROR);
				return;
			}
			else if (!CompositeMap.GetEntryByIncompleteObjectPath(package.IncompleteObjectPath, unused))
			{
				wxMessageBox(wxString::Format(_("Failed to install the mod because your client has no \"%s\""), package.IncompleteObjectPath.c_str()), _("Error!"), wxICON_ERROR);
				return;
			}
			continue;
		}
		if (package.FileVersion == VER_TERA_CLASSIC)
		{
			wxMessageBox(_("You are trying to install a 32-bit mod!"), _("Error!"), wxICON_ERROR);
		}
		else
		{
			wxMessageBox(wxString::Format(_("Unknown package version %uh."), package.FileVersion), _("Error!"), wxICON_ERROR);
		}
		return;
	}

	std::filesystem::path source(path.ToStdWstring());
	std::filesystem::path dest = GetApp()->GetModsDir() / (mod.Container + ".gpk");

	// Search for conflicts
	int entryToDelete = -1;
	for (int idx = 0; idx < ModList.size(); ++idx)
	{
		const ModEntry& entry = ModList[idx];
		if (entry.Mod.ModName == mod.ModName && entry.Mod.ModAuthor == mod.ModAuthor)
		{
			entryToDelete = idx;
			if (wxMessageBox(_("You already have this mod installed. Do you want to update it?"), _("Error!"), wxICON_QUESTION | wxYES_NO) != wxYES)
			{
				return;
			}
			break;
		}

		// Check if the mod GPK file already exists under another mod
		if (entry.Mod.Container == mod.Container && std::filesystem::exists(dest))
		{
			auto result = wxMessageBox(wxString::Format(_("You already have %s file installed as a mod %s. Do you want to override it?"), mod.Container.c_str(), entry.Mod.ModName.c_str()),
				_("File already exists!"), wxICON_QUESTION | wxYES_NO);
			if (result != wxYES)
			{
				return;
			}
			std::error_code err;
			if (!std::filesystem::remove(dest, err))
			{
				wxMessageBox(wxString::Format(_("Failed to delete the old mod file %s"), dest.filename().c_str()), _("Error!"), wxICON_ERROR);
				return;
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
				wxMessageBox(wxString::Format(_("Failed to copy %s to the mod folder. File already exists!"), source.filename().c_str()), _("Error!"), wxICON_ERROR);
				return;
			}
		}
		if (!std::filesystem::copy_file(source, dest, err))
		{
			wxMessageBox(wxString::Format(_("Failed to copy %s to the mod folder. Try to run TMM as an administrator."), source.filename().c_str()), _("Error!"), wxICON_ERROR);
			return;
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
		CompositeMap.Save();
	}
	
	newMod.File = mod.Container;
	newMod.Mod = mod;
	ModList.push_back(newMod);
	GetApp()->UpdateModsList(ModList);
	wxQueueEvent(this, new wxCommandEvent(RELOAD_MOD_LIST));
}

void ModWindow::OnRemoveClicked(wxCommandEvent& event)
{
	int index = int(ModListView->GetCurrentItem().GetID()) - 1;
	if ( ModList.size() <= index || index < 0)
	{
		return;
	}

	ModEntry& e = ModList[index];

	if (e.Enabled && !TurnOffMod(e.Mod))
	{
		return;
	}

	std::error_code err;
	if (std::filesystem::exists(GetApp()->GetModsDir() / (e.File + ".gpk")) && !std::filesystem::remove(GetApp()->GetModsDir() / (e.File + ".gpk"), err))
	{
		wxMessageBox(_("Failed to delete ") + e.File + ".gpk!", _("Error!"), wxICON_ERROR);
		return;
	}

	ModList.erase(ModList.begin() + index);
	GetApp()->UpdateModsList(ModList);
	wxQueueEvent(this, new wxCommandEvent(RELOAD_MOD_LIST));
}

void ModWindow::OnTurnOnAllClicked(wxCommandEvent& event)
{
	std::vector<bool> tmp;
	for (auto& mod : ModList)
	{
		tmp.push_back(mod.Enabled);
		if (!mod.Enabled)
		{
			mod.Enabled = true;
			TurnOnMod(mod.Mod);
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
		for (int idx = 0; idx < ModList.size(); ++idx)
		{
			ModList[idx].Enabled = tmp[idx];
		}
		return;
	}
	GetApp()->UpdateModsList(ModList);
	wxQueueEvent(this, new wxCommandEvent(RELOAD_MOD_LIST));
}

void ModWindow::OnTurnOffAllClicked(wxCommandEvent& event)
{
	std::vector<bool> tmp;
	for (auto& mod : ModList)
	{
		tmp.push_back(mod.Enabled);
		if (mod.Enabled)
		{
			mod.Enabled = false;
			TurnOffMod(mod.Mod);
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
		for (int idx = 0; idx < ModList.size(); ++idx)
		{
			ModList[idx].Enabled = tmp[idx];
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
	}

	try
	{
		CompositeMap = CompositeMapperFile(GetApp()->GetCompositeMapperPath().wstring());
		BackupMap = CompositeMapperFile(GetApp()->GetBackupCompositeMapperPath().wstring());
	}
	catch (...)
	{
		wxMessageBox(_("Failed to load the CompositePackageMapper.dat"), _("Error!"), wxICON_ERROR);
		GetApp()->ChangeRootDir({});
		Close();
	}

	wxQueueEvent(this, new wxCommandEvent(RELOAD_MOD_LIST));
}

void ModWindow::OnRealoadModList(wxCommandEvent&)
{
	ModListView->AssociateModel(new ModUIModel(ModList, this));
	AllOffButton->Enable(false);
	AllOnButton->Enable(false);
	RemoveButton->Enable(ModList.size());
	for (const auto& mod : ModList)
	{
		if (mod.Enabled)
		{
			AllOffButton->Enable(true);
			if (AllOffButton->IsEnabled() && AllOnButton->IsEnabled())
			{
				break;
			}
		}
		else
		{
			AllOnButton->Enable(true);
			if (AllOffButton->IsEnabled() && AllOnButton->IsEnabled())
			{
				break;
			}
		}
	}
}

void ModWindow::OnModSelectionChanged(wxDataViewEvent& event)
{
	RemoveButton->Enable(ModListView->GetCurrentItem().GetID());
}

bool ModWindow::TurnOnMod(const ModFile& mod)
{
	// First run to check if we can turn this ON
	// Check if any of the imported composite GPKs already used
	for (const ModEntry& existingEntry : ModList)
	{
		if (!existingEntry.Enabled)
		{
			continue;
		}
		for (const ModFile::CompositePackage& existingPackage : existingEntry.Mod.Packages)
		{
			const std::string& incompletePath = existingPackage.IncompleteObjectPath;
			if (std::any_of(mod.Packages.begin(), mod.Packages.end(), [&incompletePath](const ModFile::CompositePackage& p) { return incompletePath == p.IncompleteObjectPath; }))
			{
				wxMessageBox(wxString::Format(_("Can't turn on this mod because it's conflicting with the %s mod at %s"), existingEntry.Mod.ModName.c_str(), incompletePath.c_str()),
					_("File already exists!"), wxICON_ERROR);
				return false;
			}
		}
	}
	
	for (const auto& package : mod.Packages)
	{
		CompositeEntry entry;
		if (!CompositeMap.GetEntryByIncompleteObjectPath(package.IncompleteObjectPath, entry))
		{
			wxMessageBox(wxString::Format(_("Failed to get composite entry: %s"), package.IncompleteObjectPath.c_str()), _("Error!"), wxICON_ERROR);
			return false;
		}
	}

	for (const auto& package : mod.Packages)
	{
		CompositeEntry entry;
		CompositeMap.GetEntryByIncompleteObjectPath(package.IncompleteObjectPath, entry);
		entry.Filename = mod.Container;
		entry.Offset = package.Offset;
		entry.Size = package.Size;
		CompositeMap.AddEntry(entry);
	}
}

bool ModWindow::TurnOffMod(const ModFile& mod)
{
	std::vector<CompositeEntry> backup;
	for (const auto& package : mod.Packages)
	{
		CompositeEntry entry;
		if (!BackupMap.GetEntryByIncompleteObjectPath(package.IncompleteObjectPath, entry))
		{
			wxMessageBox(wxString::Format(_("Failed to find original entry: %s. Try to restore the CompositePackageMapper.dat file."), package.IncompleteObjectPath.c_str()), _("Error!"), wxICON_ERROR);
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