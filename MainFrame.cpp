#include "MainFrame.h"
#include <wx/wx.h>

enum IDS {
	OPEN_FILE_BUTTON_ID = 2,
	CREATE_NEGATIVE_ID = 3,
	TEXT_ID = 4
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_BUTTON(OPEN_FILE_BUTTON_ID, MainFrame::OpenFile)
	EVT_BUTTON(CREATE_NEGATIVE_ID, MainFrame::OnNegativeButtonClick)
	EVT_TEXT(TEXT_ID, MainFrame::OnTextChanged)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 135), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	wxPanel *panel = new wxPanel(this, wxID_ANY);
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, "Input File Path", wxPoint(25, 18), wxDefaultSize, 0);
	wxButton *open_file_button = new wxButton(panel, OPEN_FILE_BUTTON_ID, "Open File", wxPoint(465, 10), wxSize(100, 35));
	FilePathBox = new wxTextCtrl(panel, TEXT_ID, "C:/", wxPoint(20, 35), wxSize(425, -1));
	wxButton* negative_button = new wxButton(panel, CREATE_NEGATIVE_ID, "Negative Copy", wxPoint(465, 50), wxSize(100, 35));
}

void MainFrame::Quit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MainFrame::OpenFile(wxCommandEvent &WXUNUSED(event)) {
	wxFileDialog *OpenDialog = new wxFileDialog(this, "Choose a file to open", wxEmptyString, wxEmptyString, "*.bmp", wxFD_DEFAULT_STYLE, wxDefaultPosition);

	if (OpenDialog->ShowModal() == wxID_OK) {
		CurrentDocPath = OpenDialog->GetPath();
		for (size_t i = 0; i < CurrentDocPath.length(); i++) {
			if (CurrentDocPath.GetChar(i) == wxString("\\")) {
				CurrentDocPath.SetChar(i, wxUniChar('/'));
			}
		}
		FilePathBox->ChangeValue(CurrentDocPath);
	}
}

void MainFrame::OnTextChanged(wxCommandEvent &event) {
	CurrentDocPath = event.GetString();
}

void MainFrame::OnNegativeButtonClick(wxCommandEvent &WXUNUSED(event)) {
	
}