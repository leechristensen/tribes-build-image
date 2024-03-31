//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1996 by Borland International, All Rights Reserved
//
// $Revision:   10.8  $
//
// Sample illustrating ObjectWindows' Splitter Windows
//----------------------------------------------------------------------------
#include <owl/pch.h>
#include <owl/decframe.h>
#include <owl/applicat.h>
#include <owl/inputdia.h>
#include <owl/panespli.h>
#include <owl/controlb.h>
#include <owl/buttonga.h>
#include <winsys/color.h>
#include <stdlib.h>
#include <stdio.h>
#include "splitter.rh"


//----------------------------------------------------------------------------

const int MaxChildren = 50;

struct TChildInfo {
  TWindow*  Child;
  bool      CanClose;

  TChildInfo() {
    Child = 0;
    CanClose = true;
  }
};

TChildInfo  ChildInfo[MaxChildren+1];

//
// class TMyChildWindow
// ~~~~~ ~~~~~~~~~~~~~~
class TMyChildWindow : public TWindow {
  public:
    TMyChildWindow(TWindow* parent, int id, char far* title);

  protected:
    void  Paint(TDC&, bool, TRect&);

    void  EvLButtonDown(uint modKeys, TPoint& point) {
      SetFocus();
    }

    void EvSetFocus(THandle hWndLostFocus) {
      Invalidate();
      TWindow::EvSetFocus(hWndLostFocus);
    }

    void EvKillFocus(THandle hWndGetFocus) {
      Invalidate();
      TWindow::EvKillFocus(hWndGetFocus);
    }

    void CeCanClose(TCommandEnabler& ce) {
      ce.Enable(!ChildInfo[GetId()].CanClose);
    }

    void CmCanClose() {
      ChildInfo[GetId()].CanClose = true;
      Invalidate();
    }

    void CeCantClose(TCommandEnabler& ce) {
      ce.Enable(ChildInfo[GetId()].CanClose);
    }

    void CmCantClose() {
      ChildInfo[GetId()].CanClose = false;
      Invalidate();
    }

  DECLARE_RESPONSE_TABLE(TMyChildWindow);
};

DEFINE_RESPONSE_TABLE1(TMyChildWindow, TWindow)
  EV_WM_LBUTTONDOWN,
  EV_WM_SETFOCUS,
  EV_WM_KILLFOCUS,
  EV_COMMAND       (CM_CANCLOSE, CmCanClose),
  EV_COMMAND_ENABLE(CM_CANCLOSE, CeCanClose),
  EV_COMMAND       (CM_CANTCLOSE, CmCantClose),
  EV_COMMAND_ENABLE(CM_CANTCLOSE, CeCantClose),
END_RESPONSE_TABLE;


TMyChildWindow::TMyChildWindow(TWindow* parent, int id, char far* title)
               :TWindow(parent, title)
{
  SetBkgndColor(TColor::SysWindow);
  Attr.Style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS;
  Attr.ExStyle = WS_EX_CLIENTEDGE;
  Attr.Id = id;
}

void
TMyChildWindow::Paint(TDC& dc, bool, TRect&)
{
  char buf[20];
  if (GetFocus() == GetHandle())
    sprintf(buf, ">%d<", GetId());
  else
    sprintf(buf, " %d ", GetId());

  if (ChildInfo[GetId()].CanClose)
    strcat(buf, " ");
  else
    strcat(buf, "x");
  dc.TextOut(0, 0, buf);
}

//----------------------------------------------------------------------------

//
// class TMyPaneSplitter
// ~~~~~ ~~~~~~~~~~~~~~~
class TMyPaneSplitter : public TPaneSplitter {
  public:
    TMyPaneSplitter(TWindow* parent);

  protected:
    void SetupWindow();
    void InvalidPane(int num);

  private:
    void CmMove();
    void CmWidth();
    void CmSplit();
    void CmSplitVert();
    void CmSplitHorz();
    void CmShrink();
    void CmGrow();
    void CmRemove();
    void CmReplace();
    void CmSwap();
    void CmForEachPane();
    void CmSetSplitterWidth();
    void CmNumberOfPanes();
    void CmRemoveAllPanes();
    void CmNewPaneClient(){ SplitPane(ChildInfo[0].Child, 0, psNone);}

    void CeMove(TCommandEnabler& ce){ce.Enable(PaneCount() > 0);};
    void CeSplit(TCommandEnabler& ce){ce.Enable(PaneCount() > 0);};
    void CeSplitVert(TCommandEnabler& ce){ce.Enable(PaneCount() > 0);};
    void CeSplitHorz(TCommandEnabler& ce){ce.Enable(PaneCount() > 0);};
    void CeShrink(TCommandEnabler& ce){ce.Enable(PaneCount() > 0);};
    void CeGrow(TCommandEnabler& ce){ce.Enable(PaneCount() > 0);};
    void CeRemove(TCommandEnabler& ce){ce.Enable(PaneCount() > 1);};
    void CeReplace(TCommandEnabler& ce){ce.Enable(PaneCount() > 0);};
    void CeSwap(TCommandEnabler& ce){ce.Enable(PaneCount() > 1);};
    void CeNewPaneClient(TCommandEnabler& ce){ce.Enable(PaneCount() == 0);}

  DECLARE_RESPONSE_TABLE(TMyPaneSplitter);
};

DEFINE_RESPONSE_TABLE1(TMyPaneSplitter, TPaneSplitter)
  EV_COMMAND(CM_S_MOVE, CmMove),
  EV_COMMAND(CM_S_WIDTH, CmWidth),
  EV_COMMAND(CM_P_SPLIT, CmSplit),

  EV_COMMAND(CM_SPLITVERT, CmSplitVert),
  EV_COMMAND(CM_SPLITHORZ, CmSplitHorz),
  EV_COMMAND(CM_SHRINK, CmShrink),
  EV_COMMAND(CM_GROW, CmGrow),


  EV_COMMAND(CM_P_REMOVE, CmRemove),
  EV_COMMAND(CM_P_REPLACE, CmReplace),
  EV_COMMAND(CM_P_SWAP, CmSwap),
  EV_COMMAND(CM_P_FOR_EACH_PANE, CmForEachPane),
  EV_COMMAND(CM_S_SET_SPLITTER_WIDTH, CmSetSplitterWidth),
  EV_COMMAND(CM_P_NUMBER_OF_PANES, CmNumberOfPanes),
  EV_COMMAND(CM_P_REMOVE_ALL_PANES, CmRemoveAllPanes),
  EV_COMMAND(CM_CREATEPANECLIENT, CmNewPaneClient),

  EV_COMMAND_ENABLE(CM_S_MOVE, CeMove),
  EV_COMMAND_ENABLE(CM_P_SPLIT, CeSplit),

  EV_COMMAND_ENABLE(CM_SPLITVERT, CeSplitVert),
  EV_COMMAND_ENABLE(CM_SPLITHORZ, CeSplitHorz),
  EV_COMMAND_ENABLE(CM_SHRINK, CeShrink),
  EV_COMMAND_ENABLE(CM_GROW, CeGrow),

  EV_COMMAND_ENABLE(CM_P_REMOVE, CeRemove),
  EV_COMMAND_ENABLE(CM_P_REPLACE, CeReplace),
  EV_COMMAND_ENABLE(CM_P_SWAP, CeSwap),

  EV_COMMAND_ENABLE(CM_CREATEPANECLIENT, CeNewPaneClient),
END_RESPONSE_TABLE;

//
//
//
TMyPaneSplitter::TMyPaneSplitter(TWindow* parent)
:
  TPaneSplitter(parent)
{
  Attr.Style |= WS_CLIPCHILDREN;
  //
  // Don't allow the separators to get any closer than 20 pixels.
  // Prevents accidental closure of panes.
  // 
  SetSplitterCushion( 20 );
}

void
TMyPaneSplitter::InvalidPane(int num)
{
  char buff[50];
  wsprintf(buff, "Index %d is not a valid pane");
  MessageBox(buff, 0);
}

void
TMyPaneSplitter::CmMove()
{
  char buf[10] = "";
  if (TInputDialog(GetApplication()->GetMainWindow(),
                   "Move", "<pane #>, <distance>:",
                   buf, sizeof(buf)).Execute() == IDOK) {
    int pane = atoi(strtok(buf, ","));
    int dist = atoi(strtok(0, ","));

    if (pane >= 0 && pane < MaxChildren) {
      TWindow* child = ChildInfo[pane].Child;
      if (child && child->GetHandle())
        MoveSplitter(child, dist);
      else
        InvalidPane(pane);
    }
  }
}

void
TMyPaneSplitter::CmWidth()
{
  char buf[10] = "";

  itoa(GetSplitterWidth(), buf, 10);
  MessageBox(buf, "Splitter Width", MB_OK);
}

void
TMyPaneSplitter::CmSplit()
{
  char buf[10] = "";
  if (TInputDialog(GetApplication()->GetMainWindow(),
                   "Split", "<Target Indx>,<New Indx>,<0=H|1=V>:",
                   buf, sizeof buf).Execute() == IDOK) {
    int w1 = atoi(strtok(buf, ","));
    int w2 = atoi(strtok(0, ","));
    int hv = atoi(strtok(0, ","));

    // NOTE: The two panes can be the same window
    //       The first index must point to an existing pane
    //       The second index must *NOT* point to an existing pane
    //
    if ((w1 != w2) &&
        (ChildInfo[w1].Child->GetHandle() != 0) &&
        (ChildInfo[w2].Child->GetHandle() == 0)) {
      SplitPane(ChildInfo[w1].Child, ChildInfo[w2].Child,
                hv ? psVertical : psHorizontal);
      ChildInfo[w1].Child->SetFocus();
    }
  }
}

void
TMyPaneSplitter::CmSplitVert()
{
  TWindow* curChild = GetWindowPtr(GetApplication()->GetMainWindow()->GetCommandTarget());
  for (int i = 0; i < COUNTOF(ChildInfo); i++)
    if (!ChildInfo[i].Child->GetHandle()) {
      SplitPane(curChild, ChildInfo[i].Child, psVertical);
      ChildInfo[i].Child->SetFocus();
      return;
    }
}

void
TMyPaneSplitter::CmSplitHorz()
{
  TWindow* curChild = GetWindowPtr(GetApplication()->GetMainWindow()->GetCommandTarget());
  for (int i = 0; i < COUNTOF(ChildInfo); i++)
    if (!ChildInfo[i].Child->GetHandle()) {
      SplitPane(curChild, ChildInfo[i].Child, psHorizontal);
      ChildInfo[i].Child->SetFocus();
      return;
    }
}

void
TMyPaneSplitter::CmShrink()
{
  TWindow* curChild = GetWindowPtr(GetApplication()->GetMainWindow()->GetCommandTarget());
  if (curChild)
    MoveSplitter(curChild, -2);
}

void
TMyPaneSplitter::CmGrow()
{
  TWindow* curChild = GetWindowPtr(GetApplication()->GetMainWindow()->GetCommandTarget());
  if (curChild)
    MoveSplitter(curChild, 2);
}

void
TMyPaneSplitter::CmRemove()
{
  char buf[10] = "";
  if (TInputDialog(GetApplication()->GetMainWindow(),
                   "Remove", "Window #:", buf, sizeof buf).Execute() == IDOK) {
    int w1 = atoi(buf);
    RemovePane(ChildInfo[w1].Child);
  }
}

void
TMyPaneSplitter::CmReplace()
{
  char buf[10] = "";
  if (TInputDialog(GetApplication()->GetMainWindow(),
                   "Replace", "<Target pane index>,<New pane index>:",
                   buf, sizeof buf).Execute() == IDOK) {
    int w1 = atoi(strtok(buf, ","));
    int w2 = atoi(strtok(0, ","));

    // First index must specify an existing pane while the second a
    // brand new index..
    //
    if (ChildInfo[w1].Child->GetHandle() &&
        !ChildInfo[w2].Child->GetHandle())
      ReplacePane(ChildInfo[w1].Child, ChildInfo[w2].Child);
  }
}

void
TMyPaneSplitter::CmSwap()
{
  char buf[10] = "";
  if (TInputDialog(GetApplication()->GetMainWindow(),
                   "Swap", "<Window1>,<Window2>:",
                   buf, sizeof buf).Execute() == IDOK) {
    int w1 = atoi(strtok(buf, ","));
    int w2 = atoi(strtok(0, ","));

    if (ChildInfo[w1].Child->GetHandle() &&
        ChildInfo[w2].Child->GetHandle())
      SwapPanes(ChildInfo[w1].Child, ChildInfo[w2].Child);
  }
}

int
sIncrN(TWindow&, void* p)
{
  (*(unsigned*)p)++;
  return 1;
}

void
TMyPaneSplitter::CmForEachPane()
{
  unsigned n = 0;
  ForEachPane(sIncrN, &n);

  char buf[10];
  itoa(n, buf, 10);

  MessageBox(buf, "Number of panes:", MB_OK);
}

void
TMyPaneSplitter::CmSetSplitterWidth()
{
  char buf[10] = "";

  if (TInputDialog(GetApplication()->GetMainWindow(),
                   "Set Splitter Width", "New Width:",
                   buf, sizeof buf).Execute() == IDOK) {
    int n = atoi(buf);
    SetSplitterWidth(n);
  }
}

void
TMyPaneSplitter::CmNumberOfPanes()
{
  char buf[10];
  itoa(PaneCount(), buf, 10);
  MessageBox(buf, "Number of panes:", MB_OK);
}

void
TMyPaneSplitter::CmRemoveAllPanes()
{
  RemoveAllPanes();
}

void
TMyPaneSplitter::SetupWindow()
{
  TPaneSplitter::SetupWindow();
  SplitPane(ChildInfo[0].Child, 0, psNone);
}

//----------------------------------------------------------------------------

//
// class TPaneSplitterApp
// ~~~~~ ~~~~~~~~~~~~~~~~
class TPaneSplitterApp : public TApplication {
  public:
   ~TPaneSplitterApp();
    void InitMainWindow();
};


TPaneSplitterApp::~TPaneSplitterApp()
{
  for (int i = 0; i < MaxChildren; i++) {
    ChildInfo[i].Child->Destroy();
    delete ChildInfo[i].Child;
  }
}

void
TPaneSplitterApp::InitMainWindow()
{
  int i;
  for (i = 0; i < MaxChildren; i++) {
    ChildInfo[i].Child = new TMyChildWindow(0, i, 0);
  }
  ChildInfo[i].Child = 0;

  TDecoratedFrame* frame = new TDecoratedFrame(0, "Pane Splitters",
                                         new TMyPaneSplitter(0));
  frame->AssignMenu("IDM_LAYOUT");

  TControlBar* cb = new TControlBar(frame);
  cb->Insert(*new TButtonGadget(IDB_VERT, CM_SPLITVERT));
  cb->Insert(*new TButtonGadget(IDB_HORZ, CM_SPLITHORZ));
  cb->Insert(*new TSeparatorGadget);
  cb->Insert(*new TButtonGadget(IDB_SHRINK, CM_SHRINK));
  cb->Insert(*new TButtonGadget(IDB_GROW, CM_GROW));
  cb->Insert(*new TSeparatorGadget);
  cb->Insert(*new TButtonGadget(IDB_CANCLOSE, CM_CANCLOSE));
  cb->Insert(*new TButtonGadget(IDB_CANTCLOSE, CM_CANTCLOSE));
  frame->Insert(*cb);

  SetMainWindow(frame);
}

int
OwlMain(int, char**)
{
  return TPaneSplitterApp().Run();
}

