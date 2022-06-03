#include "HelpFrame.h"

enum IDS {
  LEARN_MORE_BUTTON_ID = 1
};

BEGIN_EVENT_TABLE(HelpFrame, wxFrame)
  EVT_BUTTON(LEARN_MORE_BUTTON_ID, HelpFrame::HandleButton)
END_EVENT_TABLE()

HelpFrame::HelpFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(500, 300), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
  // We create a panel which allows us to create
  // and place widgets on.
  wxPanel* panel = new wxPanel(this, wxID_ANY);

  wxString help_text = "ABOUT:\nBMP Negative Converter is a non-destructive image processing program that takes in a .BMP image file and creates a new .BMP image file with the photo negative filter on. It also assumes that the .BMP image file that you input is uncompressed and is in 24 bpp (bits per pixel) format.";
  wxString how_to_text = "INSTRUCTIONS:\nTo use this program, simply input the path to the file (ex. C:/Jin/image.bmp) in the text box or click the \"Open File\" button to use your system's file explorer. You can also choose to change the output destination directory by clicking the \"Change Output Destination\" button. After you're done inputting your file path and/or your output destination path, click the \"Apply Filter\" button and if all your inputs are valid and fits the assumptions, you'll see a successful indicator and you're good to go!";

  // Help Info
  wxStaticText* about_text = new wxStaticText(panel, wxID_ANY, help_text, wxPoint(10, 10), wxSize(463, 70), 0);
  about_text->Wrap(463);
  wxStaticText* instruction_text = new wxStaticText(panel, wxID_ANY, how_to_text, wxPoint(10, 100), wxSize(463, 50), 0);
  instruction_text->Wrap(463);

  // Learn more button, opens user default browser
  wxButton* learn_more_button = new wxButton(panel, LEARN_MORE_BUTTON_ID, "Learn More", wxPoint(195, 220), wxSize(100, 35));
}

void HelpFrame::HandleButton(wxCommandEvent& WXUNUSED(event)) {
  wxLaunchDefaultBrowser("https://github.com/jxuan101/BMP_Converter/blob/master/README.md");
}
