#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
  MainFrame *mainFrame = new MainFrame("BMP Photo Negative Filter");
  mainFrame->Center();
  mainFrame->Show();
  SetTopWindow(mainFrame);
  return true;
}