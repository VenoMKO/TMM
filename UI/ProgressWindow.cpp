#include "ProgressWindow.h"

wxDEFINE_EVENT(PROGRESS_SET_VALUE, wxCommandEvent);
wxDEFINE_EVENT(PROGRESS_SET_MAX_VALUE, wxCommandEvent);
wxDEFINE_EVENT(PROGRESS_SET_ACTION_DESC, wxCommandEvent);
wxDEFINE_EVENT(PROGRESS_SET_CAN_CANCEL, wxCommandEvent);
wxDEFINE_EVENT(PROGRESS_CLOSE, wxCommandEvent);

ProgressWindow::ProgressWindow(wxWindow* parent, const wxString& action, const wxString& title)
  : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(444, 98), wxCAPTION | wxSYSTEM_MENU)
{
	SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	ActionTextField = new wxStaticText(this, wxID_ANY, action, wxDefaultPosition, wxDefaultSize, 0);
	ActionTextField->Wrap(-1);
	bSizer1->Add(ActionTextField, 0, wxTOP | wxRIGHT | wxLEFT, 5);

	bSizer1->Add(0, 0, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	ProgressBar = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL | wxGA_SMOOTH);
	ProgressBar->SetValue(PW_IndeterminateProgress);
	bSizer2->Add(ProgressBar, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	CancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	CancelButton->Enable(CanCancel);

	bSizer2->Add(CancelButton, 0, wxALL, 5);


	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);


	SetSizer(bSizer1);
	Layout();

	Centre(wxBOTH);

	// Connect Events
	CancelButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ProgressWindow::OnCancelClicked), NULL, this);
	Connect(PROGRESS_SET_VALUE, wxCommandEventHandler(ProgressWindow::DoSetProgress), NULL, this);
	Connect(PROGRESS_SET_MAX_VALUE, wxCommandEventHandler(ProgressWindow::DoSetMaxProgress), NULL, this);
	Connect(PROGRESS_SET_ACTION_DESC, wxCommandEventHandler(ProgressWindow::DoSetActionText), NULL, this);
	Connect(PROGRESS_SET_CAN_CANCEL, wxCommandEventHandler(ProgressWindow::DoSetCanCancel), NULL, this);
	Connect(PROGRESS_CLOSE, wxCommandEventHandler(ProgressWindow::DoClose), NULL, this);
}

ProgressWindow::~ProgressWindow()
{
	CancelButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ProgressWindow::OnCancelClicked), NULL, this);
	Disconnect(PROGRESS_SET_VALUE, wxCommandEventHandler(ProgressWindow::DoSetProgress), NULL, this);
	Disconnect(PROGRESS_SET_MAX_VALUE, wxCommandEventHandler(ProgressWindow::DoSetMaxProgress), NULL, this);
	Disconnect(PROGRESS_SET_ACTION_DESC, wxCommandEventHandler(ProgressWindow::DoSetActionText), NULL, this);
	Disconnect(PROGRESS_SET_CAN_CANCEL, wxCommandEventHandler(ProgressWindow::DoSetCanCancel), NULL, this);
	Disconnect(PROGRESS_CLOSE, wxCommandEventHandler(ProgressWindow::DoClose), NULL, this);
}

void ProgressWindow::SetProgress(int value)
{
	wxCommandEvent* e = new wxCommandEvent(PROGRESS_SET_VALUE);
	e->SetInt(value);
	wxQueueEvent(this, e);
}

void ProgressWindow::SetMaxProgress(int value)
{
	wxCommandEvent* e = new wxCommandEvent(PROGRESS_SET_MAX_VALUE);
	e->SetInt(value);
	wxQueueEvent(this, e);
}

void ProgressWindow::SetActionText(const wxString& text)
{
	wxCommandEvent* e = new wxCommandEvent(PROGRESS_SET_ACTION_DESC);
	e->SetString(text);
	wxQueueEvent(this, e);
}

void ProgressWindow::SetCanCancel(bool value)
{
	wxCommandEvent* e = new wxCommandEvent(PROGRESS_CLOSE);
	e->SetInt(value);
	wxQueueEvent(this, e);
}

bool ProgressWindow::IsCanceled() const
{
	return Canceled.load();
}

void ProgressWindow::Close(int result)
{
	wxCommandEvent* e = new wxCommandEvent(PROGRESS_CLOSE);
	e->SetInt(result);
	wxQueueEvent(this, e);
}

void ProgressWindow::DoSetProgress(wxCommandEvent& event)
{
	int value = event.GetInt();
	if (value == PW_IndeterminateProgress)
	{
		ProgressBar->Pulse();
	}
	else
	{
		ProgressBar->SetValue(value);
	}
}

void ProgressWindow::DoSetMaxProgress(wxCommandEvent& event)
{
	ProgressBar->SetRange(event.GetInt());
}

void ProgressWindow::DoSetActionText(wxCommandEvent& event)
{
	ActionTextField->SetLabel(event.GetString());
}

void ProgressWindow::DoSetCanCancel(wxCommandEvent& event)
{
	CanCancel = event.GetInt();
	CancelButton->Enable(CanCancel);
}

void ProgressWindow::DoClose(wxCommandEvent& event)
{
	EndModal(event.GetInt());
}

void ProgressWindow::OnCancelClicked(wxCommandEvent& event)
{
	ProgressBar->SetValue(PW_IndeterminateProgress);
	ActionTextField->SetLabel(wxT("Stopping..."));
	Canceled.store(true);
	CancelButton->Enable(false);
}
