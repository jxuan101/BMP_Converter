#ifndef HELP_FRAME_H_
#define HELP_FRAME_H_

#include <wx/frame.h>
#include <wx/wx.h>

class HelpFrame : public wxFrame {
  public:
    HelpFrame(const wxString& title);

  private:
    DECLARE_EVENT_TABLE();
};

#endif // HELP_FRAME_H_