#ifndef SETTING_FRAME_H_
#define SETTING_FRAME_H_

#include <wx/frame.h>
#include <wx/wx.h>

class SettingFrame : public wxFrame {
public:
  SettingFrame(const wxString &title);

  void Quit(wxCommandEvent &WXUNUSED(event));

private:
  wxString current_doc_path_;
  wxTextCtrl *file_path_box_;

  void OpenFile(wxCommandEvent &WXUNUSED(event));
  void OnTextChanged(wxCommandEvent &event);
  void OnSaveButtonClick(wxCommandEvent &WXUNUSED(event));

};
#endif // SETTING_FRAME_H_