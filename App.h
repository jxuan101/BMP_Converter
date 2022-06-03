#ifndef APP_H_
#define APP_H_

#include <wx/wx.h>
#include <iostream>

// This class contains a method OnInit
// derived from wxApp that will start
// our wxwidget app.
class App : public wxApp {
  public:
    bool OnInit();
};

#endif // APP_H_