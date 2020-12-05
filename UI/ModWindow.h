#pragma once
#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/hyperlink.h>
#include <wx/gauge.h>

#include "../Model/Mod.h"
#include "../Model/ModUIModel.h"
#include "../Model/CompositeMapper.h"

wxDECLARE_EVENT(RELOAD_MOD_LIST, wxCommandEvent);

class ModWindow : public wxFrame, public ModListDelegate
{
public:
	ModWindow(wxWindow* parent, const std::vector<ModEntry>& entries, wxWindowID id = wxID_ANY, const wxString& title = _("Tera Mod Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(821, 434), long style = wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU | wxTAB_TRAVERSAL);

	bool OnModStateChange(ModEntry& mod) override;

protected:
	void OnAddClicked(wxCommandEvent& event);
	void OnRemoveClicked(wxCommandEvent& event);
	void OnTurnOnClicked(wxCommandEvent& event);
	void OnTurnOffClicked(wxCommandEvent& event);
	void OnChangeDirClicked(wxCommandEvent& event);
	void OnResetClicked(wxCommandEvent& event);
	void OnMoreModsClicked(wxCommandEvent& event);
	void OnIdle(wxIdleEvent& event);
	void OnRealoadModList(wxCommandEvent&);
	void OnModSelectionChanged(wxDataViewEvent& event);
	void OnWaitForTeraChanged(wxCommandEvent&);

	bool InstallMod(const std::wstring& path, bool save = true);
	bool TurnOnMod(const ModFile& mod);
	bool TurnOffMod(const ModFile& mod, bool silent = false);
	int GetAvailableTfcIndex();

	void CommitChanges();

	void StartWaiting(bool value);
	void CheckTera(wxTimerEvent&);
	void OnTeraLaunched();
	void OnTeraClosed();

private:
	wxDataViewCtrl* ModListView = nullptr;
	wxButton* AddButton = nullptr;
	wxButton* RemoveButton = nullptr;
	wxButton* OnButton = nullptr;
	wxButton* OffButton = nullptr;
	wxButton* ChangeDirButton = nullptr;
	wxButton* RestoreButton = nullptr;
	wxButton* MoreModsButton = nullptr;
	wxHyperlinkCtrl* GitHubLink = nullptr;
	wxCheckBox* WaitTeraCheckbox = nullptr;

	bool TeraRunning = false;
	bool RestoreMap = false;
	wxTimer TeraTimer;

	std::vector<ModEntry> ModList;
	CompositeMapperFile CompositeMap;
	CompositeMapperFile BackupMap;

	wxDECLARE_EVENT_TABLE();
};