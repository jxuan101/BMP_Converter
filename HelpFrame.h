#ifndef HELP_FRAME_H_
#define HELP_FRAME_H_

#include <wx/frame.h>
#include <wx/wx.h>

// This class defines the help window of our program.
// It provides useful info about the program.
class HelpFrame : public wxFrame {
  public:
    HelpFrame(const wxString& title);

  private:
    void HandleButton(wxCommandEvent& WXUNUSED(event));

    DECLARE_EVENT_TABLE();
};

#endif // HELP_FRAME_H_