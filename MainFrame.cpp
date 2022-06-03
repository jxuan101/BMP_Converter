#include "MainFrame.h"
#include "SettingFrame.h"
#include "HelpFrame.h"
#include "BMP.h"

// Enumerated IDs that will be used
// to identify our widgets and also
// give functionality to our widgets.
enum IDS {
	OPEN_FILE_BUTTON_ID = 2,
	CREATE_NEGATIVE_ID = 3,
	TEXT_ID = 4,
	OUTPUT_TEXT_ID = 5,
	HELP_BUTTON_ID = 6,
	CHANGE_OUTPUT_DEST_ID = 7
};

// An event table that binds functions
// to widgets using their IDs which calls
// their bound functions when an event occurs.
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_BUTTON(OPEN_FILE_BUTTON_ID, MainFrame::OpenFile)
	EVT_BUTTON(HELP_BUTTON_ID, MainFrame::OnHelpButtonClick)
	EVT_BUTTON(CHANGE_OUTPUT_DEST_ID, MainFrame::OnChangeDestButtonClick)
	EVT_BUTTON(CREATE_NEGATIVE_ID, MainFrame::OnNegativeButtonClick)
	EVT_TEXT(TEXT_ID, MainFrame::OnTextChanged)
END_EVENT_TABLE()

// This constructor initializes our program's main window and widgets.
MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 215), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	// Creates a panel that allows us to create
	// widgets and place them.
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	// Static Text
	wxStaticText* input_text = new wxStaticText(panel, wxID_ANY, "Input File Path (File must be in BMP format, 24 Bits Per Pixel, Uncompressed)", wxPoint(20, 15), wxDefaultSize, 0);
  status_message_ = new wxStaticText(panel, OUTPUT_TEXT_ID, "", wxPoint(20, 60), wxSize(425, 70), wxST_NO_AUTORESIZE | wxST_ELLIPSIZE_END);

	// Buttons
	wxButton* open_file_button = new wxButton(panel, OPEN_FILE_BUTTON_ID, "Open File", wxPoint(465, 10), wxSize(100, 35));
	wxButton* negative_button = new wxButton(panel, CREATE_NEGATIVE_ID, "Apply Filter", wxPoint(465, 50), wxSize(100, 35));
	wxButton* help_button = new wxButton(panel, HELP_BUTTON_ID, "Help", wxPoint(465, 130), wxSize(100, 35));
	wxButton* set_output_button = new wxButton(panel, CHANGE_OUTPUT_DEST_ID, "Change Output Destination", wxPoint(20, 130), wxSize(200, 35));

	// Text Box
  file_path_box_ = new wxTextCtrl(panel, TEXT_ID, "", wxPoint(20, 35), wxSize(425, -1));
  file_path_box_->SetHint("C:/example/example.bmp");
  file_path_box_->SetFocus();

}

// Set a custom user input directory path for output
void MainFrame::SetOutputPath(wxString& output_path) {
	current_output_path_ = output_path;
}

void MainFrame::Quit(wxCommandEvent& WXUNUSED(event)){
	// Closes every children if this window is closed.
	if (this->GetChildren().GetCount() > 0) {
		this->DestroyChildren();
	}
	Close(true);
}

// Replace '\\' with '/' to avoid formatting issues
void MainFrame::FormatPath() {
	for (size_t i = 0; i < current_doc_path_.Length(); i++) {
		if (current_doc_path_.GetChar(i) == wxString("\\")) {
			current_doc_path_.SetChar(i, wxUniChar('/'));
		}
	}
}

// This function will open a new dialog box that prompts
// the users to select a file using the system's built-in 
// file explorer.
void MainFrame::OpenFile(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog* OpenDialog = new wxFileDialog(this, "Choose a file to open", wxEmptyString, wxEmptyString, "*.bmp", wxFD_DEFAULT_STYLE, wxDefaultPosition);

	// Update the path to user's file if the user
	// successfully selects a file from the popup
	// dialog.
	if (OpenDialog->ShowModal() == wxID_OK) {
		current_doc_path_ = OpenDialog->GetPath();
		FormatPath();
		// Display the path that the user chose to our text box.
		file_path_box_->ChangeValue(current_doc_path_);
	}
}

void MainFrame::OnTextChanged(wxCommandEvent& event) {
	current_doc_path_ = event.GetString();
}

// Spawns a settings window where users can
// choose to set their own destination directory.
void MainFrame::OnHelpButtonClick(wxCommandEvent& WXUNUSED(event)) {
	HelpFrame* help = new HelpFrame("BMP Negative Converter - Help");
	this->AddChild(help);
	help->CenterOnParent();
	help->Show(true);
}

// Spawns a help window where users can read
// about the program's functionality and user
// instructions.
void MainFrame::OnChangeDestButtonClick(wxCommandEvent& WXUNUSED(event)) {
	SettingFrame* settings = new SettingFrame("BMP Negative Converter - Settings");
	this->AddChild(settings);
	settings->CenterOnParent();
	settings->Show(true);
}

void MainFrame::OnNegativeButtonClick(wxCommandEvent& WXUNUSED(event)) {
	if (current_doc_path_.empty()) {
		status_message_->SetForegroundColour("#DA3E1C");
		status_message_->SetLabel(wxDateTime::Now().FormatTime() + " Error: No file specified.");
	}
	else {
		FormatPath();
		file_path_box_->ChangeValue(current_doc_path_);
		CreatePhotoNegative();
	}
}

void MainFrame::CreatePhotoNegative() {
	try {
		BMP* bmp;
		// If the user provided no custom target directory
		// for the output.
		if (current_output_path_.empty()) {
			std::string file_path = current_doc_path_.ToStdString();
			bmp = new BMP(file_path);
			bmp->CreatePhotoNegative();
			current_output_path_ = file_path.substr(0, file_path.find_last_of('/') + 1);
		}
		// If the user did set a custom target directory.
		else {
			bmp = new BMP(current_doc_path_.ToStdString(), current_output_path_.ToStdString());
			bmp->CreatePhotoNegative();
		}
		status_message_->SetForegroundColour("#579D23");
		status_message_->SetLabel(wxDateTime::Now().FormatTime() + " Success! File created at: " + current_output_path_);
		status_message_->Wrap(425);
	}
	// If the file is not found.
	catch (const std::runtime_error &err) {
		status_message_->SetForegroundColour("#DA3E1C");
		status_message_->SetLabel(wxDateTime::Now().FormatTime() + " " + err.what());
		status_message_->Wrap(425);
	}
	// If the file is compressed, not 24 bits
	// per pixel, or not a BMP file.
	catch (const std::logic_error &err) {
		status_message_->SetForegroundColour("#DA3E1C");
		status_message_->SetLabel(wxDateTime::Now().FormatTime() + " " + err.what());
		status_message_->Wrap(425);
	}
}