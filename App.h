#include <wx/wx.h>
#include <iostream>

// This class contains a method OnInit
// derived from wxApp that will start
// our wxwidget app.
class App : public wxApp {
  public:
    bool OnInit();
};