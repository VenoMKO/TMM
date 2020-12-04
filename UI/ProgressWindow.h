#pragma once
#include <wx/wx.h>

class ProgressWindow : public wxDialog {
public:
	enum { PW_IndeterminateProgress = -1 };
	ProgressWindow(wxWindow* parent, const wxString& action = wxT("Preparing..."), const wxString& title = wxT("Please wait..."));
	~ProgressWindow();

	void SetProgress(int value);
	void SetMaxProgress(int value);
	void SetActionText(const wxString& text);
	void SetCanCancel(bool value);
	bool IsCanceled() const;
	void Close(int result);

protected:
	void DoSetProgress(wxCommandEvent& event);
	void DoSetMaxProgress(wxCommandEvent& event);
	void DoSetActionText(wxCommandEvent& event);
	void DoSetCanCancel(wxCommandEvent& event);
	void DoClose(wxCommandEvent& event);

	void OnCancelClicked(wxCommandEvent& event);

private:
	wxStaticText* ActionTextField = nullptr;
	wxGauge* ProgressBar = nullptr;
	wxButton* CancelButton = nullptr;
	bool CanCancel = false;
	std::atomic_bool Canceled = { false };
};