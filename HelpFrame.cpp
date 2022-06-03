#include "HelpFrame.h"

enum IDS {
  LEARN_MORE_BUTTON_ID = 1
};

BEGIN_EVENT_TABLE(HelpFrame, wxFrame)
  EVT_BUTTON(LEARN_MORE_BUTTON_ID, HelpFrame::HandleButton)
END_EVENT_TABLE()

HelpFrame::HelpFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 180), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
  // We create a panel which allows us to create
  // and place widgets on.
  wxPanel* panel = new wxPanel(this, wxID_ANY);

  wxString help_text = "";

  // Help Info
  wxStaticText* input_text = new wxStaticText(panel, wxID_ANY, help_text, wxPoint(20, 15), wxDefaultSize, 0);

  // Learn more button, opens user default browser
  wxButton* learn_more_button = new wxButton(panel, LEARN_MORE_BUTTON_ID, "Save", wxPoint(350, 100), wxSize(100, 35));
}

void HelpFrame::HandleButton(wxCommandEvent& WXUNUSED(event)) {
  wxLaunchDefaultBrowser("https://github.com/jxuan101/BMP_Converter/blob/master/README.md");
}
