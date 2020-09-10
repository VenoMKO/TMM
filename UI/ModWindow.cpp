#include "ModWindow.h"
#include "../Model/Mod.h"

#include <wx/statline.h>

#include <fstream>

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

ModWindow::ModWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	SetSizeHints(wxDefaultSize, wxDefaultSize);
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	ModListView = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(ModListView, 1, wxALL | wxEXPAND, 5);

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

	GitHubLink = new wxHyperlinkCtrl(m_panel1, ControlElementId::GitHub, _("Tera Mod Manager v.1.00"), wxT("https://github.com/VenoMKO/TMM/Releases"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
	bSizer3->Add(GitHubLink, 0, wxALL | wxEXPAND, 5);


	m_panel1->SetSizer(bSizer3);
	m_panel1->Layout();
	bSizer3->Fit(m_panel1);
	bSizer2->Add(m_panel1, 0, wxEXPAND | wxALL, 5);


	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);


	SetSizer(bSizer1);
	Layout();

	Centre(wxBOTH);

	// Connect Events
	AddButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnAddClicked), NULL, this);
	RemoveButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnRemoveClicked), NULL, this);
	AllOnButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnTurnOnAllClicked), NULL, this);
	AllOffButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnTurnOffAllClicked), NULL, this);
	ChangeDirButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnChangeDirClicked), NULL, this);
	RestoreButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnResetClicked), NULL, this);
	MoreModsButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ModWindow::OnMoreModsClicked), NULL, this);
}

void ModWindow::OnAddClicked(wxCommandEvent& event)
{
	const wxString extensions = wxS("Package files (*.gpk; *.gmp; *.u; *.umap; *.upk)|*.gpk;*.gmp;*.u;*.umap;*.upk");
	wxString path = wxFileSelector("Open a package", wxEmptyString, wxEmptyString, extensions, extensions, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (path.empty())
	{
		return;
	}

	std::ifstream s(path.ToStdWstring(), std::ios::binary | std::ios::in);
	Mod mod;
	s >> mod;
}

void ModWindow::OnRemoveClicked(wxCommandEvent& event)
{
}

void ModWindow::OnTurnOnAllClicked(wxCommandEvent& event)
{
}

void ModWindow::OnTurnOffAllClicked(wxCommandEvent& event)
{
}

void ModWindow::OnChangeDirClicked(wxCommandEvent& event)
{
}

void ModWindow::OnResetClicked(wxCommandEvent& event)
{
}

void ModWindow::OnMoreModsClicked(wxCommandEvent& event)
{
}
