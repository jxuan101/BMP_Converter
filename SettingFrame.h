#ifndef SETTING_FRAME_H_
#define SETTING_FRAME_H_

#include <wx/frame.h>
#include <wx/wx.h>

// This class creates a new window named Settings
// which will be a child to our MainFrame and called
// when users click the "Change Output Destination" button.
class SettingFrame : public wxFrame {
public:
  SettingFrame(const wxString& title);

  void Quit(wxCommandEvent& WXUNUSED(event));

private:
  wxString current_folder_path_;        // The path of the directory that we
                                        // want to output files to
  wxTextCtrl* folder_path_box_;         // A pointer to a text box to track
                                        // user input
  wxStaticText* status_message_;        

  void OnSelectFolderButton(wxCommandEvent& WXUNUSED(event));
  void OnTextChanged(wxCommandEvent& event);
  void OnSaveButtonClick(wxCommandEvent& WXUNUSED(event));

  DECLARE_EVENT_TABLE();
};
#endif // SETTING_FRAME_H_