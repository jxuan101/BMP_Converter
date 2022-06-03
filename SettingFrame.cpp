#include <wx/wx.h>

#include "SettingFrame.h"

SettingFrame::SettingFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 235), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {

}

void SettingFrame::Quit(wxCommandEvent &WXUNUSED(event))
{
	Close(true);
}

void SettingFrame::OpenFile(wxCommandEvent& WXUNUSED(event)) {

}

void SettingFrame::OnTextChanged(wxCommandEvent &event) {
	current_doc_path_ = event.GetString();
}

void SettingFrame::OnSaveButtonClick(wxCommandEvent& WXUNUSED(event)) {

}