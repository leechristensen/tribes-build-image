//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1994 by Borland International
//   Tutorial application -- cntrl17.cpp
//----------------------------------------------------------------------------
#include <owl/pch.h>
#include <owl/applicat.h>
#include <owl/framewin.h>
#include <owl/edit.h>
#include <strstrea.h>

#include <ocf/automacr.h>
#include <iostream.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#include "step17.cxx" 

class TMyApp : public TApplication
{
public:
  TMyApp(): TApplication("Cntrl17") {}
  void InitMainWindow();
  void InitInstance();
  void Output(ostrstream &ostr);

  bool DispServer; //True if automation server should be displayed when run

private:
  void TestAutomation(); //Routine to execute step17
  TEdit *Disp; //Client window
};

void
TMyApp::InitMainWindow()
{
  Disp=new TEdit(0,0,"",0,0,0,0,0,true);
  SetMainWindow(new TFrameWindow(0,"Step 17 Controller",Disp));
}

//
// Outputs ostrstream buf to client window 
//
void
TMyApp::Output(ostrstream &ostr)
{
  char *str = ostr.str(); //Get stream buffer as char*
  ostr.seekp(0);  //Reset stream
  Disp->Insert(str);
  Disp->Insert("\r\n");
}

void
TMyApp::InitInstance()
{
  TApplication::InitInstance();
  // Our windows are now created and we can output the test results
  TestAutomation();
}                           


long Colors[] = {
  RGB(0x00, 0x00, 0x00),
  RGB(0xFF, 0x00, 0x00),
  RGB(0x00, 0xFF, 0x00),
  RGB(0x00, 0x00, 0xFF),
  RGB(0xFF, 0xFF, 0x00),
  RGB(0x00, 0xFF, 0xFF),
  RGB(0xFF, 0x00, 0xFF)
};

const int NumColors = sizeof(Colors)/sizeof(Colors[0]);

//
// Excercises the server using the classes generated by AUTOGEN.EXE
//
void
TMyApp::TestAutomation()
{
  char buf[256];
  ostrstream ostr(buf, sizeof buf);

  try {
    TOleAllocator allocator(0);      // OLE requirement
    TDrawApp app;
    TDrawDoc doc;

    app.Bind("DrawPad.Application.17");

    ostr << "Application name =" << (const char far*)app.GetName() << ends;
    Output(ostr);
    ostr << "Application path =" << (const char far*)app.GetFullName() << ends;
    Output(ostr);
                 
    app.NewDocument(doc);

    // if there's one argument on command line, don't display the frame window.
    if (DispServer) {
      app.SetVisible(true);
    }
    else {
      app.SetVisible(false);
    }
    ostr << "Current automated pen size: " << doc.GetPenSize() << ends;
    Output(ostr);

    ostr << "Current automated pen color: " << doc.GetPenColor() << ends;
    Output(ostr);

    double startX   = 150;
    double startY   = 150;
    double radius   = 0;
    double theta    = 0;
    int colorIndex  = 0;
    short penSize   = 1; //Server expects a short arg in SetPenSize
    double x        = 0;
    double y        = 0;

    ostr << "Drawing in automated app ..."<<ends;
    Output(ostr);

    doc.AddPoint(startX, startY);
    for (int i=0; radius < 70; i++) {
      if (i % 10 == 0) {
        // use the next pen
        doc.AddLine();
        doc.AddPoint(startX+x, startY-y);
        doc.SetPenColor(Colors[colorIndex++]);
        doc.SetPenSize(penSize++);
        colorIndex %= NumColors;
      }
      radius += 0.2 * theta;
      x = 2 * radius * cos(theta);
      y = 2 * radius * sin(theta);
      doc.AddPoint(startX+x, startY-y);
      theta += 0.2;
    }
    doc.AddLine();

    // End of server automation test
    ::MessageBox(0, "End of Program", "CNTRL17.EXE", MB_OK);
    ostr << "Shutting server down..." << ends;
    Output(ostr);
    app.Quit();

    ostr << "Server shut down, test complete"<<ends;
    Output(ostr);

  }
  catch (TXBase& x) {
    ostr << x.why() << ends;
    Output(ostr);
  }

}

int
OwlMain(int argc, char* /*argv*/ [])
{
  TMyApp app;

  // Pass a dummy arg on command line to make the server invisible
  if (argc == 2)
    app.DispServer=false;
  else
    app.DispServer=true;

  return app.Run();
}

