// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995 Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   filesys.c
//
//  PURPOSE:  Implements the window procedure for the main application
//            window.  
//
//  FUNCTIONS:
//    WndProc      - Processes messages for the main window.
//    MsgCommand   - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy   - Handles the WM_DESTROY message by calling 
//                   PostQuitMessage().
//    CmdExit      - Handles the file exit command by calling destory 
//                   window on the main window.
//    CmdDemo      - Displays the "Demo" dialog box
//
//  COMMENTS:
//

#include <windows.h>          // required for all Windows applications
#include "globals.h"          // prototypes specific to this application
#include "demo.h"
#include "resource.h"


// Main window message table definition.
MSD rgmsd[] =
{
    {WM_COMMAND, MsgCommand},
    {WM_DESTROY, MsgDestroy}
};

MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpWindow
};

// Main window command table definition.
CMD rgcmd[] =
{
    {IDM_DEMOFILESYS, CmdDemo},
    {IDM_EXIT,        CmdExit},
    {IDM_ABOUT,       CmdAbout}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispMessage() function with the main window's message dispatch
//    information (msdiMain) and the message specific information.
//

LRESULT CALLBACK WndProc(HWND   hwnd, 
                         UINT   uMessage, 
                         WPARAM wparam, 
                         LPARAM lparam)
{

    return DispMessage(&msdiMain, hwnd, uMessage, wparam, lparam);

}


//
//  FUNCTION: MsgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_COMMAND (Unused)
//    GET_WM_COMMAND_ID(wparam, lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam, lparam) - Control handle
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispCommand() function with the main window's command dispatch
//    information (cmdiMain) and the command specific information.
//

#pragma argsused
LRESULT MsgCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiMain, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls PostQuitMessage().
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PostQuitMessage(0);
    return 0;
}


//
//  FUNCTION: CmdExit(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_EXIT (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT CmdExit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DestroyWindow(hwnd);
    return 0;
}


//
//  FUNCTION: CmdDemo(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the "Demo" dialog box
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_DEMOTABCTRL     (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    To process the IDM_DEMOFILESYS message, call DialogBox which
//    will create the box according to the information in your resource
//    script file and turn control over to the Demo() function.
//
//

#pragma argsused
LRESULT CmdDemo(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DialogBox(hInst, "FILESYSTEM", hwnd, (DLGPROC) DemoDlgProc);
    return 0;
}