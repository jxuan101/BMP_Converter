#include <wx/frame.h>
#include <wx/wx.h>

class MainFrame : public wxFrame {
  public:
    MainFrame(const wxString &title);

    void Quit(wxCommandEvent &WXUNUSED(event));
  
  private:
    wxString CurrentDocPath = "C:/";
    wxTextCtrl *FilePathBox;
    
    void OpenFile(wxCommandEvent &WXUNUSED(event));
    void OnTextChanged(wxCommandEvent &event);
    void OnNegativeButtonClick(wxCommandEvent &WXUNUSED(event));
    wxDECLARE_EVENT_TABLE();
};