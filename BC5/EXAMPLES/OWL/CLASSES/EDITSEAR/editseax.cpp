//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1991, 1995 by Borland International, All Rights Reserved
//----------------------------------------------------------------------------
#include <owl/pch.h>
#include <owl/applicat.h>
#include <owl/framewin.h>
#include <owl/editsear.h>

//
// class TEditApp
// ~~~~~ ~~~~~~~~
class TEditApp : public TApplication {
  public:
    TEditApp() : TApplication("Edit Search Application") {}
    void InitMainWindow();
};

void
TEditApp::InitMainWindow()
{
  MainWindow = new TFrameWindow(0, "EditWindow", new TEditSearch);
  MainWindow->AssignMenu(IDM_EDITSEARCH);
  MainWindow->Attr.AccelTable = TResId(IDA_EDITSEARCH);
  MainWindow->Attr.Style |= DS_LOCALEDIT;
}

int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  return TEditApp().Run();
}