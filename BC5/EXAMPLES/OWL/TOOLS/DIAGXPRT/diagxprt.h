//----------------------------------------------------------------------------
// ObjectWindows
// (C) Copyright 1995, 1995 by Borland International, All Rights Reserved
//
//----------------------------------------------------------------------------
#if !defined(DIAGXPRT_H)
#define DIAGXPRT_H

#include <owl/applicat.h>
#include <owl/decframe.h>
#include <owl/controlb.h>
#include <owl/editfile.h>
#include <owl/buttonga.h>
#include <owl/dialog.h>
#include <owl/tinycapt.h>
#include <owl/menu.h>
#include <owl/statusba.h>
#include <owl/serialze.h>

#if !defined(BI_PLAT_WIN16)
# error  This sample requires TOOLHELP and must be built for the WIN16 env.
#endif

#define DIAG_INI  "owl.ini"
#define DIAG_CLS  "owldiag"
#define DIAG_RECT "rect"
#define DIAG_MODE "mode"
#define DIAG_SIZE "size"

#define CM_LOGPARAMERROR  WM_USER+1000
#define CM_LOGERROR       CM_LOGPARAMERROR+1
#define CM_OUTSTR         CM_LOGERROR+1
#define CM_DEFAULT        CM_OUTSTR+1

//
//
//
int CALLBACK Callback(uint16 id, uint32 data);

//
//
//
class TDispData {
  public:
    char   ModuleName[80];   // Sizes?
    char   ProcName[4096];   // Also gets used for OutputDebugString messages
    uint   ErrorCode;
    DWORD  Data;
    void far* far* Param;
};

//
//
//
class TDiagFrame : public TDecoratedFrame, public TTinyCaption,
                   public TSerializeReceiver {
  public:
    TDiagFrame(const char* t, TWindow* c);
    char far* GetClassName() { return "DiagXprt"; }
    void DataReceived(uint32 length, void* data);

  protected:
    LRESULT  EvCommand(uint id, HWND hWndCtl, uint notifyCode);
    void     EvSysCommand(uint cmdType, TPoint& p);
    void     EvSize(uint type, TSize& size);
    void     SetupWindow();
    void     CleanupWindow();
    void     CmToggleSize();

  private:
    TRect    rect;

  DECLARE_RESPONSE_TABLE(TDiagFrame);
};

//
//
//
class TToolHelp {
  public:
    struct   ERRMAP { uint err; char* szErr; };
    static   ERRMAP errmap[];
    void     FormatLogParamError(char* p, int size, TDispData* d);
    void     FormatLogError(char* p, int size, TDispData* d);

    char*    GetTaskName();
    char*    GetProcName(FARPROC p);

  private:
    static   char buffer[1024];
    char     TaskName[10];
    char     ProcName[80];
};

//
//
//
class TDiagClient : public TToolHelp, public TEditFile {
  public:
    enum { maxLines = 512 };

    enum     Mode { normalMode = 1, ripMode = 2 } mode;
    enum     Size { bigSize = 1, smallSize = 2 } size;

    Size     GetSize() {return size;}
    void     SetSize(Size s);
    Mode     GetMode() {return mode;}
    void     SetMode(Mode m) {mode = m;}
    void     SetupWindow();

    void     CleanupWindow();
    static   TDiagClient* pThis;
    TFont*   pFont0;
    TFont*   pFont1;
    FARPROC  pThunk;

  protected:
    int     nActive;

    void    CmFileNewEnable(TCommandEnabler& c){ c.Enable(1); }
    void    CmFileOpenEnable(TCommandEnabler& c){ c.Enable(1); }
    void    CmFileSaveEnable(TCommandEnabler& c) { TEditFile::CmSaveEnable(c); }
    void    CmFileSaveAsEnable(TCommandEnabler& c) { c.Enable(1); }
    void    CmStartStopEnable(TCommandEnabler& c) { c.Enable(1); }
    void    CmSetupEnable(TCommandEnabler& c) { c.Enable(1); }
    void    CmStartEnable(TCommandEnabler& c) { c.Enable(!nActive); }
    void    CmStopEnable(TCommandEnabler& c) { c.Enable(nActive); }
    void    CmFileNew() { TEditFile::CmFileNew(); }
    void    CmFileOpen() { TEditFile::CmFileOpen(); }
    void    CmFileSave() { TEditFile::CmFileSave(); }
    void    CmFileSaveAs() { TEditFile::CmFileSaveAs(); }
    void    CmSetup();
    void    CmStart();
    void    CmStop();
    void    CmMem();
    void    CmHelp();
    LRESULT CmOutStr(WPARAM wParam, LPARAM lParam);
    LRESULT CmLogError(WPARAM wParam, LPARAM lParam);
    LRESULT CmLogParamError(WPARAM wParam, LPARAM lParam);
    LRESULT CmDefault(WPARAM wParam, LPARAM lParam);
    void    LoadMode();
    void    SaveMode();

    // Override of TEditFile::CanClear()
    //
    bool    CanClear();

  DECLARE_RESPONSE_TABLE(TDiagClient);
};

//
//
//
class TOwlDiagApp : public TApplication {
  public:
    void  InitMainWindow();

  private:
    static int   speedbar[];
};

#endif