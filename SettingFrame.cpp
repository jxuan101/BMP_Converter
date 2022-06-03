#include <wx/wx.h>

#include "SettingFrame.h"

enum IDS {
	SELECT_FOLDER_BUTTON_ID = 1,
	TEXT_ID = 2,
	SAVE_FOLDER_BUTTON_ID = 3,
	CLOSE_BUTTON_ID = 4,
	RESULT_TEXT_ID = 5
};

BEGIN_EVENT_TABLE(SettingFrame, wxFrame)
	EVT_BUTTON(SELECT_FOLDER_BUTTON_ID, SettingFrame::OnSelectFolderButton)
	EVT_BUTTON(SAVE_FOLDER_BUTTON_ID, SettingFrame::OnSaveButtonClick)
	EVT_BUTTON(CLOSE_BUTTON_ID, SettingFrame::Quit)
END_EVENT_TABLE()

SettingFrame::SettingFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 180), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	wxPanel* panel = new wxPanel(this, wxID_ANY);
	wxButton* open_folder_button = new wxButton(panel, SELECT_FOLDER_BUTTON_ID, "Select Folder", wxPoint(465, 29), wxSize(100, 35));
	folder_path_box_ = new wxTextCtrl(panel, TEXT_ID, "", wxPoint(20, 35), wxSize(425, -1));
	folder_path_box_->SetHint("C:/example/");
	folder_path_box_->SetFocus();
	wxStaticText* input_text = new wxStaticText(panel, wxID_ANY, "Output Destination Directory", wxPoint(20, 15), wxDefaultSize, 0);
	wxButton* save_folder_button = new wxButton(panel, SAVE_FOLDER_BUTTON_ID, "Save", wxPoint(350, 100), wxSize(100, 35));
	wxButton* close_button = new wxButton(panel, CLOSE_BUTTON_ID, "Close", wxPoint(465, 100), wxSize(100, 35));
	status_message_ = new wxStaticText(panel, RESULT_TEXT_ID, "", wxPoint(20, 60), wxSize(425, 30), wxST_NO_AUTORESIZE | wxST_ELLIPSIZE_END);
}

void SettingFrame::Quit(wxCommandEvent &WXUNUSED(event))
{
	Close(true);
}

void SettingFrame::OnSelectFolderButton(wxCommandEvent& WXUNUSED(event)) {
	wxDirDialog* OpenDialog = new wxDirDialog(this, "Select your output directory", wxEmptyString, wxDD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize, wxDirDialogNameStr);

	if (OpenDialog->ShowModal() == wxID_OK) {
		current_folder_path_ = OpenDialog->GetPath();
		for (size_t i = 0; i < current_folder_path_.Length(); i++) {
			if (current_folder_path_.GetChar(i) == wxString("\\")) {
				current_folder_path_.SetChar(i, wxUniChar('/'));
			}
		}
		folder_path_box_->ChangeValue(current_folder_path_);
	}
}

void SettingFrame::OnTextChanged(wxCommandEvent &event) {
	current_folder_path_ = event.GetString();
}

void SettingFrame::OnSaveButtonClick(wxCommandEvent& WXUNUSED(event)) {
	
}