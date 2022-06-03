#include <wx/frame.h>
#include <wx/wx.h>
#include <string>

class MainFrame : public wxFrame {
  public:
    MainFrame(const wxString &title);

    void Quit(wxCommandEvent &WXUNUSED(event));
  
  private:
    wxString current_doc_path_;
    wxTextCtrl *file_path_box_;
    wxStaticText *status_message_;
    
    void OpenFile(wxCommandEvent &WXUNUSED(event));
    void OnTextChanged(wxCommandEvent &event);
    void OnHelpButtonClick(wxCommandEvent &WXUNUSED(event));
    void OnChangeDestButtonClick(wxCommandEvent &WXUNUSED(event));
    void OnNegativeButtonClick(wxCommandEvent &WXUNUSED(event));
    void CreatePhotoNegative();

    DECLARE_EVENT_TABLE();
};