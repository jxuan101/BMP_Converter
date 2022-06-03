#include <wx/wx.h>

#include "App.h"
#include "MainFrame.h"

IMPLEMENT_APP(App);

// Serves as our program's main function
// that is executed upon startup.
bool App::OnInit() {
  MainFrame* main_frame = new MainFrame("BMP Negative Converter");
  main_frame->Center();
  main_frame->Show();
  SetTopWindow(main_frame);
  return true;
}