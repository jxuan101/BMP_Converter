#include <wx/frame.h>
#include <wx/wx.h>
#include <string>

class MainFrame : public wxFrame {
  public:
    MainFrame(const wxString &title);

    void Quit(wxCommandEvent &WXUNUSED(event));
  
  private:
    wxString current_doc_path_ = "C:/";
    wxTextCtrl *file_path_box_;
    
    void OpenFile(wxCommandEvent &WXUNUSED(event));
    void OnTextChanged(wxCommandEvent &event);
    void OnNegativeButtonClick(wxCommandEvent &WXUNUSED(event));

    DECLARE_EVENT_TABLE();
};