#include <wx/wx.h>

#include "BMP.h"
#include "App.h"
#include "MainFrame.h"

IMPLEMENT_APP(App);

bool App::OnInit() {
  MainFrame *main_frame = new MainFrame("BMP Photo Negative Filter");
  //try {
  //  BMP bmp(main_frame->GetCurrentDocPath());
  //  bmp.CreatePhotoNegative();
  //}
  //catch (const std::runtime_error& err) {
  //  std::cerr << err.what() << std::endl;
  //}
  main_frame->Center();
  main_frame->Show();
  SetTopWindow(main_frame);
  return true;
}