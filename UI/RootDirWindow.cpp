#include "RootDirWindow.h"

const wxString RootDir = wxS("S1Game");

bool IsValidDir(const wxString& path)
{
	return path.EndsWith(wxFILE_SEP_PATH + RootDir) || path.EndsWith(wxFILE_SEP_PATH + RootDir + wxFILE_SEP_PATH);
}

RootDirWindow::RootDirWindow(wxWindow* parent, const wxString& rootDir, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style)
{
	SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer(wxVERTICAL);

	wxStaticText* m_staticText1;
	m_staticText1 = new wxStaticText(this, wxID_ANY, _("Select your Tera S1Game folder. Press the Browse button or enter path directly to the text field. Example path: D:\\Games\\Gameforge\\tera\\Client\\S1Game"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(500);
	bSizer8->Add(m_staticText1, 0, wxALL, 5);

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText(this, wxID_ANY, _("S1Game:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2->Wrap(-1);
	bSizer9->Add(m_staticText2, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	PathField = new wxTextCtrl(this, wxID_ANY, rootDir, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	bSizer9->Add(PathField, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	BrowseButton = new wxButton(this, wxID_ANY, _("Browse"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer9->Add(BrowseButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);


	bSizer8->Add(bSizer9, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer(wxHORIZONTAL);

	wxPanel* m_panel3;
	m_panel3 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	bSizer10->Add(m_panel3, 1, wxEXPAND | wxALL, 5);

	OkButton = new wxButton(this, wxID_OK, _("Apply"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer10->Add(OkButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	OkButton->Enable(false);

	wxButton* CancelButton;
	CancelButton = new wxButton(this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer10->Add(CancelButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);


	bSizer8->Add(bSizer10, 1, wxEXPAND, 5);


	SetSizer(bSizer8);
	Layout();

	Centre(wxBOTH);

	// Connect Events
	PathField->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(RootDirWindow::OnTextEvent), NULL, this);
	PathField->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(RootDirWindow::OnTextEnterEvent), NULL, this);
	BrowseButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RootDirWindow::OnBrowseClicked), NULL, this);
}

RootDirWindow::~RootDirWindow()
{
	// Disconnect Events
	PathField->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(RootDirWindow::OnTextEvent), NULL, this);
	PathField->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(RootDirWindow::OnTextEnterEvent), NULL, this);
	BrowseButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RootDirWindow::OnBrowseClicked), NULL, this);

}

wxString RootDirWindow::GetPath() const
{
	return PathField->GetValue();
}

void RootDirWindow::OnTextEvent(wxCommandEvent& event)
{
	if (!IsValidDir(PathField->GetValue()))
	{
		OkButton->Enable(false);
		OkButton->SetToolTip(wxT("You must specify the correct \"" + RootDir + "\". Press \"Browse\" and select the folder."));
	}
	else
	{
		OkButton->Enable(true);
	}
}

void RootDirWindow::OnTextEnterEvent(wxCommandEvent& event)
{
	if (OkButton->IsEnabled())
	{
		EndModal(wxID_OK);
	}
}

void RootDirWindow::OnBrowseClicked(wxCommandEvent& event)
{
	wxDirDialog openPanel(this, wxS("Select \"") + RootDir + wxS("\" folder"), PathField->GetLabelText());
	openPanel.Center();
	if (openPanel.ShowModal() == wxID_OK)
	{
		const wxString path = openPanel.GetPath();
		if (IsValidDir(path))
		{
			PathField->SetLabelText(path);
		}
		else
		{
			wxMessageBox(wxS("Folder must be called \"") + RootDir + wxS("\""), wxS("Error: Invalid path!"), wxICON_ERROR);
		}
	}
}
