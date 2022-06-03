#include <wx/wx.h>

#include "MainFrame.h"
#include "SettingFrame.h"
#include "BMP.h"

enum IDS {
	OPEN_FILE_BUTTON_ID = 2,
	CREATE_NEGATIVE_ID = 3,
	TEXT_ID = 4,
	OUTPUT_TEXT_ID = 5,
	HELP_BUTTON_ID = 6,
	CHANGE_OUTPUT_DEST_ID = 7
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_BUTTON(OPEN_FILE_BUTTON_ID, MainFrame::OpenFile)
	EVT_BUTTON(HELP_BUTTON_ID, MainFrame::OnHelpButtonClick)
	EVT_BUTTON(CHANGE_OUTPUT_DEST_ID, MainFrame::OnChangeDestButtonClick)
	EVT_BUTTON(CREATE_NEGATIVE_ID, MainFrame::OnNegativeButtonClick)
	EVT_TEXT(TEXT_ID, MainFrame::OnTextChanged)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 215), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	wxPanel* panel = new wxPanel(this, wxID_ANY);
	wxStaticText* input_text = new wxStaticText(panel, wxID_ANY, "Input File Path (File must be in BMP format, 24 Bits Per Pixel, Uncompressed)", wxPoint(20, 15), wxDefaultSize, 0);
	wxButton* open_file_button = new wxButton(panel, OPEN_FILE_BUTTON_ID, "Open File", wxPoint(465, 10), wxSize(100, 35));
	file_path_box_ = new wxTextCtrl(panel, TEXT_ID, "", wxPoint(20, 35), wxSize(425, -1));
	wxButton* negative_button = new wxButton(panel, CREATE_NEGATIVE_ID, "Apply Filter", wxPoint(465, 50), wxSize(100, 35));
	status_message_ = new wxStaticText(panel, OUTPUT_TEXT_ID, "", wxPoint(20, 60), wxSize(425, 70), wxST_NO_AUTORESIZE | wxST_ELLIPSIZE_END);
	file_path_box_->SetHint("C:/example/example.bmp");
	file_path_box_->SetFocus();
	wxButton* help_button = new wxButton(panel, HELP_BUTTON_ID, "Help", wxPoint(465, 130), wxSize(100, 35));
	wxButton* set_output_button = new wxButton(panel, CHANGE_OUTPUT_DEST_ID, "Change Output Destination", wxPoint(20, 130), wxSize(200, 35));
}

void MainFrame::Quit(wxCommandEvent& WXUNUSED(event)){
	if (this->GetChildren().GetCount() > 0) {
		this->DestroyChildren();
	}
	Close(true);
}

void MainFrame::OpenFile(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog* OpenDialog = new wxFileDialog(this, "Choose a file to open", wxEmptyString, wxEmptyString, "*.bmp", wxFD_DEFAULT_STYLE, wxDefaultPosition);

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

void MainFrame::OnTextChanged(wxCommandEvent& event) {
	current_doc_path_ = event.GetString();
}

void MainFrame::OnHelpButtonClick(wxCommandEvent& WXUNUSED(event)) {
	MainFrame* help = new MainFrame("Change Output Destination");
	this->AddChild(help);
	help->Show(true);
}

void MainFrame::OnChangeDestButtonClick(wxCommandEvent& WXUNUSED(event)) {
	SettingFrame* settings = new SettingFrame("Settings");
	settings->Center();
	this->AddChild(settings);
	settings->Show(true);
}

void MainFrame::OnNegativeButtonClick(wxCommandEvent& WXUNUSED(event)) {
	if (current_doc_path_.empty()) {
		status_message_->SetForegroundColour("#DA3E1C");
		status_message_->SetLabel("Error: No file specified.");
	}
	else {
		CreatePhotoNegative();
	}
}

void MainFrame::CreatePhotoNegative() {
	try {
		BMP bmp(current_doc_path_.ToStdString());
		bmp.CreatePhotoNegative();
		status_message_->SetForegroundColour("#579D23");
		status_message_->SetLabel("Success!");
	}
	catch (const std::runtime_error &err) {
		status_message_->SetForegroundColour("#DA3E1C");
		status_message_->SetLabel(err.what());
	}
	catch (const std::logic_error &err) {
		status_message_->SetForegroundColour("#DA3E1C");
		status_message_->SetLabel(err.what());
	}
}