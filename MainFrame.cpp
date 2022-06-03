#include <wx/wx.h>

#include "MainFrame.h"

enum IDS {
	OPEN_FILE_BUTTON_ID = 2,
	CREATE_NEGATIVE_ID = 3,
	TEXT_ID = 4
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_BUTTON(OPEN_FILE_BUTTON_ID, MainFrame::OpenFile)
	EVT_BUTTON(CREATE_NEGATIVE_ID, MainFrame::OnNegativeButtonClick)
	EVT_TEXT(TEXT_ID, MainFrame::OnTextChanged)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 135), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	wxPanel *panel = new wxPanel(this, wxID_ANY);
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, "Input File Path (File must be in BMP format, 24 Bits Per Pixel, Uncompressed)", wxPoint(25, 15), wxDefaultSize, 0);
	wxButton *open_file_button = new wxButton(panel, OPEN_FILE_BUTTON_ID, "Open File", wxPoint(465, 10), wxSize(100, 35));
	file_path_box_ = new wxTextCtrl(panel, TEXT_ID, "C:/", wxPoint(20, 35), wxSize(425, -1));
	wxButton* negative_button = new wxButton(panel, CREATE_NEGATIVE_ID, "Apply Filter", wxPoint(465, 50), wxSize(100, 35));
}

void MainFrame::Quit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MainFrame::OpenFile(wxCommandEvent &WXUNUSED(event)) {
	wxFileDialog *OpenDialog = new wxFileDialog(this, "Choose a file to open", wxEmptyString, wxEmptyString, "*.bmp", wxFD_DEFAULT_STYLE, wxDefaultPosition);

	if (OpenDialog->ShowModal() == wxID_OK) {
		current_doc_path_ = OpenDialog->GetPath();
		for (size_t i = 0; i < current_doc_path_.Length(); i++) {
			if (current_doc_path_.GetChar(i) == wxString("\\")) {
				current_doc_path_.SetChar(i, wxUniChar('/'));
			}
		}
		file_path_box_->ChangeValue(current_doc_path_);
	}
}

void MainFrame::OnTextChanged(wxCommandEvent &event) {
	current_doc_path_ = event.GetString();
}

void MainFrame::OnNegativeButtonClick(wxCommandEvent &WXUNUSED(event)) {
	
}