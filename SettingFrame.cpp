#include <wx/wx.h>

#include "SettingFrame.h"
#include "MainFrame.h"

// Enumerated IDs that will be used
// to identify our widgets and also
// give functionality to our widgets.
enum IDS {
	SELECT_FOLDER_BUTTON_ID = 1,
	TEXT_ID = 2,
	SAVE_PATH_BUTTON_ID = 3,
	CLOSE_BUTTON_ID = 4,
	RESULT_TEXT_ID = 5
};

// An event table that binds functions
// to widgets using their IDs which calls
// their bound functions when an event occurs.
BEGIN_EVENT_TABLE(SettingFrame, wxFrame)
	EVT_BUTTON(SELECT_FOLDER_BUTTON_ID, SettingFrame::OnSelectFolderButton)
	EVT_BUTTON(SAVE_PATH_BUTTON_ID, SettingFrame::OnSaveButtonClick)
	EVT_TEXT(TEXT_ID, SettingFrame::OnTextChanged)
	EVT_BUTTON(CLOSE_BUTTON_ID, SettingFrame::Quit)
END_EVENT_TABLE()

// This constructor creates our "Settings" window and widgets.
SettingFrame::SettingFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 180), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	// We create a panel which allows us to create
	// and place widgets on.
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	// Static texts
	wxStaticText* input_text = new wxStaticText(panel, wxID_ANY, "Output Destination Directory", wxPoint(20, 15), wxDefaultSize, 0);
  status_message_ = new wxStaticText(panel, RESULT_TEXT_ID, "", wxPoint(20, 60), wxSize(425, 30), wxST_NO_AUTORESIZE | wxST_ELLIPSIZE_END);

	// Buttons
	wxButton* save_folder_button = new wxButton(panel, SAVE_PATH_BUTTON_ID, "Save", wxPoint(350, 100), wxSize(100, 35));
	wxButton* close_button = new wxButton(panel, CLOSE_BUTTON_ID, "Close", wxPoint(465, 100), wxSize(100, 35));
  wxButton* open_folder_button = new wxButton(panel, SELECT_FOLDER_BUTTON_ID, "Select Folder", wxPoint(465, 29), wxSize(100, 35));

	// Text Box
  folder_path_box_ = new wxTextCtrl(panel, TEXT_ID, "", wxPoint(20, 35), wxSize(425, -1));
  folder_path_box_->SetHint("C:/example/");
  folder_path_box_->SetFocus();

}

void SettingFrame::Quit(wxCommandEvent &WXUNUSED(event)) {
	Close(true);
}

void SettingFrame::FormatPath() {
	// Replace '\\' with '/' to avoid formatting issues
	for (size_t i = 0; i < current_folder_path_.Length(); i++) {
		if (current_folder_path_.GetChar(i) == wxString("\\")) {
			current_folder_path_.SetChar(i, wxUniChar('/'));
		}
	}
}

// This function will open a new dialog box that prompts
// the users to select a folder or directory using the 
// system's builtin file explorer.
void SettingFrame::OnSelectFolderButton(wxCommandEvent& WXUNUSED(event)) {
	wxDirDialog* OpenDialog = new wxDirDialog(this, "Select your output directory", wxEmptyString, wxDD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize, wxDirDialogNameStr);

	// Update the path if the user successfully
	// chooses a directory.
	if (OpenDialog->ShowModal() == wxID_OK) {
		current_folder_path_ = OpenDialog->GetPath();
		FormatPath();
		folder_path_box_->ChangeValue(current_folder_path_);
	}
}

void SettingFrame::OnTextChanged(wxCommandEvent &event) {
	current_folder_path_ = event.GetString();
}

void SettingFrame::OnSaveButtonClick(wxCommandEvent& WXUNUSED(event)) { 
	// Check whether the path is valid using stat
	// from <sys/stat.h>
	bool valid = false;
	struct stat info;
	if (stat(current_folder_path_, &info) == 0) {
		if (info.st_mode & S_IFDIR) {
			valid = true;
			FormatPath();
			status_message_->SetForegroundColour("#579D23");
			status_message_->SetLabel(wxDateTime::Now().FormatTime() + " Success! The output directory is now: " + current_folder_path_);
			status_message_->Wrap(425);
		}
		else if (info.st_mode & S_IFREG) {
			status_message_->SetForegroundColour("#DA3E1C");
			status_message_->SetLabel(
				wxDateTime::Now().FormatTime() + " Error: Your provided path is a file not a directory.");
			return;
		}
		else {
			status_message_->SetForegroundColour("#DA3E1C");
			status_message_->SetLabel(wxDateTime::Now().FormatTime() + " Error: Your provided path is not a valid directory.");
			return;
		}
	}
	else {
		status_message_->SetForegroundColour("#DA3E1C");
		status_message_->SetLabel(wxDateTime::Now().FormatTime() + " Error: Your path cannot be found.");
		return;
	}

	folder_path_box_->ChangeValue(current_folder_path_);
	((MainFrame*)GetParent())->SetOutputPath(current_folder_path_);
}