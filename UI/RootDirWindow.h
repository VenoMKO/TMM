#pragma once
#include <wx/wx.h>

class RootDirWindow : public wxDialog
{
public:
	RootDirWindow(wxWindow* parent, const wxString& rootDir, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(515, 170), long style = wxDEFAULT_DIALOG_STYLE);
	~RootDirWindow();

	wxString GetPath() const;

protected:
	void OnTextEvent(wxCommandEvent& event);
	void OnTextEnterEvent(wxCommandEvent& event);
	void OnBrowseClicked(wxCommandEvent& event);

private:
	wxTextCtrl* PathField = nullptr;
	wxButton* BrowseButton = nullptr;
	wxButton* OkButton = nullptr;
};