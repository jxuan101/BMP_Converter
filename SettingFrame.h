#ifndef SETTING_FRAME_H_
#define SETTING_FRAME_H_

#include <wx/frame.h>
#include <wx/wx.h>

class SettingFrame : public wxFrame {
public:
  SettingFrame(const wxString& title);

  void Quit(wxCommandEvent& WXUNUSED(event));

private:
  wxString current_folder_path_;
  wxTextCtrl* folder_path_box_;
  wxStaticText* status_message_;

  void OnSelectFolderButton(wxCommandEvent& WXUNUSED(event));
  void OnTextChanged(wxCommandEvent& event);
  void OnSaveButtonClick(wxCommandEvent& WXUNUSED(event));

  DECLARE_EVENT_TABLE();
};
#endif // SETTING_FRAME_H_