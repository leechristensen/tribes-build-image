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
// Functions for handling main window messages.  The message-dispatching
// mechanism expects all message-handling functions to have the following
// prototype:
//
//     LRESULT FunctionName(HWND, UINT, WPARAM, LPARAM);

// **TODO**  Add message-handling function prototypes here.  Be sure to
//           add the function names to the main window message table in
//           generic.c.

LRESULT MsgCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDestroy(HWND, UINT, WPARAM, LPARAM);


//-------------------------------------------------------------------------
// Functions for handling main window commands--ie. functions for
// processing WM_COMMAND messages based on the wParam value.
// The message-dispatching mechanism expects all command-handling
// functions to have the following prototype:
//
//     LRESULT FunctionName(HWND, WORD, WORD, HWND);

// **TODO**  Add message-handling function prototypes here.  Be sure to
//           add the function names to the main window command table in
//           generic.c.

LRESULT CmdExit(HWND, WORD, WORD, HWND);
LRESULT CmdAbout(HWND, WORD, WORD, HWND);


//-------------------------------------------------------------------------
// Global function prototypes.

// **TODO**  Add global function prototypes here.

BOOL InitApplication(HINSTANCE, int);
BOOL CenterWindow(HWND, HWND);

    // Callback functions.  These are called by Windows.

// **TODO**  Add new callback function prototypes here.

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//-------------------------------------------------------------------------
// Command ID definitions.  These definitions are used to associate menu
// items with commands.

// **TODO**  Add IDs for all menu commands here.  Be sure that the IDs you
//           add are also added to the main window command table in generic.c
//           and the menu statements in the resource file.

// File menu
#define IDM_EXIT    1000

// Help menu
#define IDM_ABOUT   1100

//-------------------------------------------------------------------------
// String Table ID definitions.

#define IDS_APPNAME     1
#define IDS_DESCRIPTION 2

//-------------------------------------------------------------------------
//  About dialog defines.

#define IDD_VERFIRST    100
#define IDD_VERLAST     104


//-------------------------------------------------------------------------
// Global variable declarations.

extern HINSTANCE hInst;          // The current instance handle
extern char      szAppName[];    // The name of this application
extern char      szTitle[];      // The title bar text

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



//-------------------------------------------------------------------------
// Version string definitions--Leave these alone.

#define SZRCOMPANYNAME "CompanyName"
#define SZRDESCRIPTION "FileDescription"
#define SZRVERSION     "FileVersion"
#define SZRAPPNAME     "InternalName"
#define SZRCOPYRIGHT   "LegalCopyright"
#define SZRTRADEMARK   "LegalTrademarks"
#define SZRPRODNAME    "ProductName"
#define SZRPRODVER     "ProuctVersion"
