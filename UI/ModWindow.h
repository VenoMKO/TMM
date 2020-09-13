#pragma once
#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/hyperlink.h>
#include <wx/gauge.h>

#include "../Model/Mod.h"
#include "../Model/CompositeMapper.h"

wxDECLARE_EVENT(RELOAD_MOD_LIST, wxCommandEvent);

class ModWindow : public wxFrame
{
public:
	ModWindow(wxWindow* parent, const std::vector<ModEntry>& entries, wxWindowID id = wxID_ANY, const wxString& title = _("Tera Mod Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(721, 434), long style = wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU | wxTAB_TRAVERSAL);

protected:
	void OnAddClicked(wxCommandEvent& event);
	void OnRemoveClicked(wxCommandEvent& event);
	void OnTurnOnAllClicked(wxCommandEvent& event);
	void OnTurnOffAllClicked(wxCommandEvent& event);
	void OnChangeDirClicked(wxCommandEvent& event);
	void OnResetClicked(wxCommandEvent& event);
	void OnMoreModsClicked(wxCommandEvent& event);
	void OnIdle(wxIdleEvent& event);
	void OnToggleMod(wxDataViewEvent& event);
	void OnRealoadModList(wxCommandEvent&);

	void LoadModList();
	void InstallMod(const ModFile& mod);

	bool TurnOnMod(const ModFile& mod);
	bool TurnOffMod(const ModFile& mod);

private:
	wxDataViewCtrl* ModListView = nullptr;
	wxButton* AddButton = nullptr;
	wxButton* RemoveButton = nullptr;
	wxButton* AllOnButton = nullptr;
	wxButton* AllOffButton = nullptr;
	wxButton* ChangeDirButton = nullptr;
	wxButton* RestoreButton = nullptr;
	wxButton* MoreModsButton = nullptr;
	wxHyperlinkCtrl* GitHubLink = nullptr;
	wxGauge* ProgressBar = nullptr;

	std::vector<ModEntry> ModData;
	CompositeMapperFile CompositeMap;
	CompositeMapperFile BackupMap;

	wxDECLARE_EVENT_TABLE();
};