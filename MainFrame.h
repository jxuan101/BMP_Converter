#ifndef MAIN_FRAME_H_
#define MAIN_FRAME_H_

#include <wx/frame.h>
#include <wx/wx.h>

// Constants
const wxColour kSuccessRGB = "#579D23";
const wxColour kErrorRGB = "#DA3E1C";

// MainFrame is derived from wxFrame and
// contains methods that will create the
// main window of our program's GUI.
class MainFrame : public wxFrame {
  public:
    MainFrame(const wxString& title);

    void SetOutputPath(wxString& output_path);
    void Quit(wxCommandEvent& WXUNUSED(event));
    
  private:
    wxString current_doc_path_;           // The path of the file we want
                                          // to read and edit (non-destructive)
    wxTextCtrl* file_path_box_;           // A pointer to a text box to track 
                                          // user input
    wxString current_output_path_;        // The custom user input path to the 
                                          // directory where we place our output,
                                          // only used if there's input otherwise
                                          // this is not used
    wxStaticText* status_message_;        // A pointer to a static text component
                                          // that will display the status of our
                                          // program or error messages that are
                                          // caught
    
    void FormatPath();
    void OpenFile(wxCommandEvent& WXUNUSED(event));
    void OnTextChanged(wxCommandEvent& event);
    void OnHelpButtonClick(wxCommandEvent& WXUNUSED(event));
    void OnChangeDestButtonClick(wxCommandEvent& WXUNUSED(event));
    void OnNegativeButtonClick(wxCommandEvent& WXUNUSED(event));
    void CreatePhotoNegative();

    DECLARE_EVENT_TABLE();
};

#endif // MAIN_FRAME_H_