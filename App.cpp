#include <wx/wx.h>

#include "App.h"
#include "MainFrame.h"

IMPLEMENT_APP(App);

bool App::OnInit() {
  MainFrame* main_frame = new MainFrame("BMP Photo Negative Filter");
  main_frame->Center();
  main_frame->Show();
  SetTopWindow(main_frame);
  return true;
}