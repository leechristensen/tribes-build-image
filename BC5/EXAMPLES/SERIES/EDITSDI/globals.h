// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE:
//    Contains declarations for all globally scoped names in the program.
//

//-------------------------------------------------------------------------
// Product identifier string defines

//  **TODO** Change these strings to the name of your application.

#define SZAPPNAME     "EditSDI"
#define SZDESCRIPTION "SDI Editor Example Application"


//-------------------------------------------------------------------------
// Functions for handling main window messages.  The message-dispatching
// mechanism expects all message-handling functions to have the following
// prototype:
//
//     LRESULT FunctionName(HWND, UINT, WPARAM, LPARAM);

// **TODO**  Add message-handling function prototypes here.  Be sure to
//           add the function names to the main window message table in
//           editsdi.c.

LRESULT MsgCommand(HWND, UINT, WPARAM, LPARAM);             // Misc
LRESULT MsgDestroy(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgSetFocus(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgSize(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgQueryEndSession(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgClose(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgInitMenu(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgFindReplace(HWND, UINT, WPARAM, LPARAM);         // Search

//-------------------------------------------------------------------------
// Functions for handling main window commands--ie. functions for
// processing WM_COMMAND messages based on the wParam value.
// The message-dispatching mechanism expects all command-handling
// functions to have the following prototype:
//
//     LRESULT FunctionName(HWND, WORD, WORD, HWND);

// **TODO**  Add message-handling function prototypes here.  Be sure to
//           add the function names to the main window command table in
//           editsdi.c.

LRESULT CmdExit(HWND, WORD, WORD, HWND);                    // Misc.
LRESULT CmdAbout(HWND, WORD, WORD, HWND);
LRESULT CmdClip(HWND, WORD, WORD, HWND);                    // Clipboard
LRESULT CmdFileNew(HWND, WORD, WORD, HWND);                 // File
LRESULT CmdFileOpen(HWND, WORD, WORD, HWND);
LRESULT CmdFileSave(HWND, WORD, WORD, HWND);
LRESULT CmdFileSaveAs(HWND, WORD, WORD, HWND);
LRESULT CmdPrint(HWND, WORD, WORD, HWND);                   // Print
LRESULT CmdPrintSU(HWND, WORD, WORD, HWND);
LRESULT CmdEdit(HWND, WORD, WORD, HWND);                    // Edit
LRESULT CmdFindReplace(HWND, WORD, WORD, HWND);             // Search
LRESULT CmdFindNext(HWND, WORD, WORD, HWND);

//-------------------------------------------------------------------------
// Global type declarations.

typedef enum {
    frtFind,
    frtReplace,
    frtRepAll
} FRT;  // Find/Replace tag


//-------------------------------------------------------------------------
// Global function prototypes.

// **TODO**  Add global function prototypes here.

BOOL InitApplication(HINSTANCE);                            // Misc
BOOL InitInstance(HINSTANCE, int);
BOOL CenterWindow(HWND, HWND);
VOID Open(LPSTR, BOOL, HWND);                               // File
VOID SaveAs(LPSTR, HWND);
BOOL QuerySaveFile(HWND);
VOID Print                                                  // Print
    (HWND, HDC, BOOL, BOOL, BOOL, BOOL, UINT, UINT, UINT, HGLOBAL);
DWORD GetPageRange(VOID);
BOOL InitEdit(HWND);
VOID SetEditText(HWND, HANDLE);
char *LockEditText(HWND);
VOID UnlockEditText(HWND);
BOOL InitFindReplace(VOID);                                 // Search
BOOL IsFindReplaceMsg(LPMSG);
BOOL CanFind(VOID);
VOID FindReplace(char *, char *, FRT, BOOL, BOOL, BOOL);

extern char szFName[];
extern HWND hwndEdit;

char *GetFName(VOID);
HWND GetEditWindow(VOID);


    // Callback functions.  These are called by Windows.

// **TODO**  Add new callback function prototypes here.  Win16 compiles
//           require the __export keyword to generate proper prolog
//           and epilog code for exported functions.

#ifdef WIN16

LRESULT CALLBACK __export WndProc(HWND, UINT, WPARAM, LPARAM);

#else

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#endif


//-------------------------------------------------------------------------
// Global macro definitions.


#define MAKEBOOL(i) ((BOOL) !!(i))                          // General
#ifdef WIN16
#define GETSEL(ichS,ichE) \
    { \
        DWORD dwSel = SendMessage(GetEditWindow(), EM_GETSEL, 0, 0L); \
        ichS = LOWORD(dwSel); \
        ichE = HIWORD(dwSel); \
    }
#define SETSEL(ichS,ichE) \
    SendMessage(GetEditWindow(), EM_SETSEL, FALSE, MAKELONG(ichS, ichE))
#else
#define GETSEL(ichS,ichE) \
    SendMessage(GetEditWindow(), EM_GETSEL, (WPARAM) &ichS, (LPARAM) &ichE)
#define SETSEL(ichS,ichE) \
    SendMessage(GetEditWindow(), EM_SETSEL, (WPARAM) ichS, (LPARAM) ichE)
#endif


//-------------------------------------------------------------------------
// Global variable declarations.

extern HINSTANCE hInst;          // The current instance handle
extern HWND      hwndMain;       // Main application window
extern char      szAppName[];    // The name of this application
extern char      szTitle[];      // The title bar text
extern HCURSOR hcursHourGlass;   // Hourglass cursor

// **TODO**  For NON-MDI applications, uncomment line 1 below and comment
//           line 2.  For MDI applications, uncomment line 2 below, comment
//           line 1, and then define hwndMDIClient as a global variable in
//           INIT.C
#define hwndMDIClient NULL        /* (1) Stub for NON-MDI applications. */
// extern HWND hwndMDIClient;     /* (2) For MDI applications.          */


//-------------------------------------------------------------------------
// Message and command dispatch infrastructure.  The following type
// definitions and functions are used by the message and command dispatching
// mechanism and do not need to be changed.

    // Function pointer prototype for message handling functions.
typedef LRESULT (*PFNMSG)(HWND,UINT,WPARAM,LPARAM);

    // Function pointer prototype for command handling functions.
typedef LRESULT (*PFNCMD)(HWND,WORD,WORD,HWND);

    // Enumerated type used to determine which default window procedure
    // should be called by the message- and command-dispatching mechanism
    // if a message or command is not handled explicitly.
typedef enum
{
   edwpNone,            // Do not call any default procedure.
   edwpWindow,          // Call DefWindowProc.
   edwpDialog,          // Call DefDlgProc (This should be used only for
                        // custom dialogs - standard dialog use edwpNone).
   edwpMDIChild,        // Call DefMDIChildProc.
   edwpMDIFrame         // Call DefFrameProc.
} EDWP;                // Enumeration for Default Window Procedures

    // This structure maps messages to message handling functions.
typedef struct _MSD
{
    UINT   uMessage;
    PFNMSG pfnmsg;
} MSD;                 // MeSsage Dispatch structure

    // This structure contains all of the information that a window
    // procedure passes to DispMessage in order to define the message
    // dispatching behavior for the window.
typedef struct _MSDI
{
    int  cmsd;          // Number of message dispatch structs in rgmsd
    MSD *rgmsd;         // Table of message dispatch structures
    EDWP edwp;          // Type of default window handler needed.
} MSDI, FAR *LPMSDI;   // MeSsage Dipatch Information

    // This structure maps command IDs to command handling functions.
typedef struct _CMD
{
    WORD   wCommand;
    PFNCMD pfncmd;
} CMD;                 // CoMmand Dispatch structure

    // This structure contains all of the information that a command
    // message procedure passes to DispCommand in order to define the
    // command dispatching behavior for the window.
typedef struct _CMDI
{
    int  ccmd;          // Number of command dispatch structs in rgcmd
    CMD *rgcmd;         // Table of command dispatch structures
    EDWP edwp;          // Type of default window handler needed.
} CMDI, FAR *LPCMDI;   // CoMmand Dispatch Information

    // Message and command dispatching functions.  They look up messages
    // and commands in the dispatch tables and call the appropriate handler
    // function.
LRESULT DispMessage(LPMSDI, HWND, UINT, WPARAM, LPARAM);
LRESULT DispCommand(LPCMDI, HWND, WPARAM, LPARAM);

    // Message dispatch information for the main window
extern MSDI msdiMain;
    // Command dispatch information for the main window
extern CMDI cmdiMain;
