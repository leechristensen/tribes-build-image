// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   writepad.c
//
//  PURPOSE:  Implement the window procedure for the main application
//            window.
//
//  FUNCTIONS:
//    WndProc            - Processes messages for the main window.
//    MsgCommand         - Handle the WM_COMMAND messages for the main window.
//    MsgCreate          - Handle the WM_CREATE message for the main window
//    MsgInitMenu        - Handle the WM_INITMENU message for the main window
//    MsgSize            - Handle the WM_SIZE message for the main window
//    MsgClose           - Handle the WM_CLOSE message for the main window
//    MsgQueryEndSession - Handle the WM_QUERYENDSESSION message 
//    MsgDestroy         - Handles the WM_DESTROY message by calling
//                         PostQuitMessage().
//    CmdFileNew         - Handler to create a new MDI child window
//    CmdFileClose       - Handler to close the active MDI child window
//    CmdClip            - Handler for cut, copy, paste
//    CmdWindowTile      - Handler to tile the MDI children
//    CmdWindowCascade   - Handler to cascade the MDI children
//    CmdWindowIcons     - Handler to arrange the iconic MDI children
//    CmdWindowCloseAll  - Handler to close all the MDI children
//    CmdExit            - Handles the file exit command by calling destory
//                         window on the main window.
//    CmdBold            - Applys/Removes bold formatting to selected text
//    CmdItalic          - Applys/Removes italic formatting to selected text
//    CmdUnderline       - Applys/Removes underline formatting to selected text
//    CmdFontDialog      - Lets user change text with Common Font dialog
//    CmdIncreaseFont    - Increase fonts size of selected text 2 points
//    CmdDecreaseFont    - Decrease fonts size of selected text 2 points
//    GetFName           - Get the current file name.
//
//
//  COMMENTS:
//    Message dispatch table -
//      For every message to be handled by the main window procedure
//      place the message number and handler function pointer in
//      rgmsd (the message dispatch table).  Place the prototype
//      for the function in globals.h and the definition of the
//      function in the appropriate module.
//    Command dispatch table -
//      For every command to be handled by the main window procedure
//      place the command number and handler function pointer in
//      rgcmd (the command dispatch table).  Place the prototype
//      for the function in globals.h and the definition of the
//      function in the appropriate module.
//    Globals.h Contains the definitions of the structures and dispatch.c
//      contains the functions that use these structures.
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <commctrl.h>
#include <richedit.h>
#include "globals.h"            // prototypes specific to this application
#include "resource.h"
#include "mdichild.h"
#include "toolbar.h"
#include "statbar.h"
#include "rtf.h"

LRESULT CmdFilePrintPreview(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl);

// Main window message table definition.
MSD rgmsd[] =
{
    {0,                  MsgFindReplace},
    {WM_INITMENU,        MsgInitMenu},
    {WM_COMMAND,         MsgCommand},
    {WM_CREATE,          MsgCreate},
    {WM_DESTROY,         MsgDestroy},
    {WM_MENUSELECT,      MsgMenuSelect},
    {WM_NOTIFY,          MsgNotify},
    {WM_TIMER,           MsgTimer},
    {WM_SIZE,            MsgSize},
    {WM_CLOSE,           MsgClose},
    {WM_QUERYENDSESSION, MsgQueryEndSession},
};

MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpMDIFrame
};


// Main window command table definition.
CMD rgcmd[] =
{
    {IDM_FILENEW,       CmdFileNew},
    {IDM_FILEOPEN,      CmdFileOpen},
    {IDM_FILESAVE,      CmdFileSave},
    {IDM_FILESAVEAS,    CmdFileSaveAs},
    {IDM_FILECLOSE,     CmdFileClose},
    {IDM_FILEPRINT,     CmdFilePrint},
    {IDM_FILEPRINTSU,   CmdFilePrintSU},
    {IDM_FILEPRINTPREV, CmdFilePrintPreview},
    {IDM_EDITCUT,       CmdClip},
    {IDM_EDITCOPY,      CmdClip},
    {IDM_EDITCLEAR,     CmdClip},
    {IDM_EDITPASTE,     CmdClip},
    {IDM_EDITUNDO,      CmdClip},
    {IDM_EDITBOLD,      CmdBold},
    {IDM_EDITITALIC,    CmdItalic},
    {IDM_EDITUNDERLINE, CmdUnderline},
    {IDM_EDITFONTDIALOG,CmdFontDialog},
    {IDM_EDITINCREASEFONT,CmdIncreaseFont},
    {IDM_EDITDECREASEFONT,CmdDecreaseFont},
    {IDM_EXIT,          CmdExit},
    {IDM_ABOUT,         CmdAbout},
    {IDM_FIND,          CmdSearchFindReplace},
    {IDM_REPLACE,       CmdSearchFindReplace},
    {IDM_FINDNEXT,      CmdSearchFindNext},
    {IDM_FINDPREV,      CmdSearchFindNext},
    {IDM_WINDOWTILE,    CmdWindowTile},
    {IDM_WINDOWCASCADE, CmdWindowCascade},
    {IDM_WINDOWICONS,   CmdWindowIcons},
    {IDM_WINDOWCLOSEALL,CmdWindowCloseAll}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpMDIFrame
};


//Module specific globals

HWND hwndMDIClient = NULL;      // The MDI client window handle.
UINT cUntitled = 1;
UINT cOpen;
HCURSOR hcursHourGlass;
extern char szChildName[];

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
//  FUNCTION: MsgCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_CREATE message for the main frame windwo.
//           Creates the MDI client window.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - WM_CREATE (unused)
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    CLIENTCREATESTRUCT ccs = {0};

    InitCommonControls();   // Initialize the common control library.

    CreateTBar(hwnd);       // Create tool bar
    CreateSBar(hwnd);       // Create status bar

    // Find window menu where children will be listed
    ccs.hWindowMenu  = GetSubMenu(GetMenu(hwnd), WINDOWMENU);
    ccs.idFirstChild = IDM_WINDOWCHILD;

    // Create the MDI client filling the client area
	 hwndMDIClient = CreateWindowEx(WS_EX_CLIENTEDGE,    // 3D edge (sunken)
                                   "mdiclient",
                                   NULL,
                                   WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL |
                                   WS_HSCROLL,
                                   0, 0, 0, 0,
                                   hwnd,
                                   (HMENU)0xCAC,
                                   hInst,
                                   (LPVOID)&ccs);

    ShowWindow(hwndMDIClient, SW_SHOW);

    hcursHourGlass = LoadCursor(NULL, IDC_WAIT);

	 return 0;
}


//
//  FUNCTION: MsgInitMenu(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Enable/Disable the menu items as appropriate for the active
//           MDI child window
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number (Unused)
//    wparam    - HMENU - The menu about to be activated
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    0 - The message was handled.
//    1 - The message was not handled - wrong menu.
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgInitMenu(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    if (GetMenu(hwnd) == (HMENU)wparam)
    {
        UINT ichStart;
        UINT ichEnd;
        UINT mf = (cOpen) ? MF_ENABLED : MF_GRAYED;

        EnableMenuItem((HMENU)wparam, IDM_FILECLOSE, mf);

        mf = MF_GRAYED;

		  // Enable/Disable Paste menu item
        if (cOpen && OpenClipboard(hwnd))
        {
            if (IsClipboardFormatAvailable(CF_TEXT) ||
                IsClipboardFormatAvailable(CF_OEMTEXT))
            {
                mf = MF_ENABLED;
            }
        }
        CloseClipboard();

        EnableMenuItem((HMENU)wparam, IDM_EDITPASTE, mf);

        // Enable/Disable Undo menu item
        if (cOpen)
				mf = SendMessage(GetEditWindow(NULL), EM_CANUNDO, 0, 0L) ?
                MF_ENABLED : MF_GRAYED;
        else
            mf = MF_GRAYED;

        EnableMenuItem((HMENU)wparam, IDM_EDITUNDO, mf);

        // Enable/Disable Cut/Copy/Delete menu items
        if (cOpen)
        {
            SendMessage(GetEditWindow(NULL), 
                        EM_GETSEL, 
                        (WPARAM)&ichStart, 
                        (LPARAM)&ichEnd);
            mf = (ichEnd != ichStart) ? MF_ENABLED : MF_GRAYED;
		  }
        else
            mf = MF_GRAYED;

        EnableMenuItem((HMENU)wparam, IDM_EDITCUT, mf);
        EnableMenuItem((HMENU)wparam, IDM_EDITCOPY, mf);
        EnableMenuItem((HMENU)wparam, IDM_EDITCLEAR, mf);

        // Enable/Disable Save menu item
        if (cOpen)
            mf = SendMessage(GetEditWindow(NULL), 
                             EM_GETMODIFY, 0, 0L) ? MF_ENABLED : MF_GRAYED;
        else
            mf = MF_GRAYED;

		  EnableMenuItem((HMENU)wparam, IDM_FILESAVE, mf);

        // Enable/Disable Save As, Print, Find and Replace menu items
        if (cOpen)
            mf = SendMessage(GetEditWindow(NULL),
                             WM_GETTEXTLENGTH, 0, 0L) ? MF_ENABLED : MF_GRAYED;
        else
            mf = MF_GRAYED;

        EnableMenuItem((HMENU)wparam, IDM_FILESAVEAS, mf);
        EnableMenuItem((HMENU)wparam, IDM_FILEPRINT, mf);
        EnableMenuItem((HMENU)wparam, IDM_FILEPRINTPREV, mf);
        EnableMenuItem((HMENU)wparam, IDM_FIND, mf);
        EnableMenuItem((HMENU)wparam, IDM_REPLACE, mf);

		  // Enable/Disable FindNext/FindPrev menu intes
        if (cOpen)
        {
            mf = CanFind() ? MF_ENABLED : MF_GRAYED;
        }
        else
            mf = MF_GRAYED;

        EnableMenuItem((HMENU)wparam, IDM_FINDNEXT, mf);
        EnableMenuItem((HMENU)wparam, IDM_FINDPREV, mf);

        return 0;
    }
    else
    {
		  return 1;
    }
}


//
//  FUNCTION: MsgSize(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  This function resizes the toolbar and statusbar controls.
//
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Used)
//    uMessage  - Message number (Used)
//    wparam    - Extra data     (Used)
//    lparam    - Extra data     (Used)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//    When the window procdure that has the status and tool bar controls
//    receive the WM_SIZE message, it has to pass the message on to these
//    controls so that these controls can adjust their size accordingly.
//
//

LRESULT MsgSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    SendMessage(hWndToolbar, uMessage, wparam, lparam);
    SendMessage(hWndStatusbar,  uMessage, wparam, lparam);

    // Re-position the panes in the status bar
    InitializeStatusBar(hwnd);

    // Position the MDI client window between the tool and status bars
    if (wparam != SIZE_MINIMIZED)
    {
        RECT rc, rcClient;

        GetClientRect(hwnd, &rcClient);

        GetWindowRect(hWndToolbar, &rc);
        ScreenToClient(hwnd, (LPPOINT)&rc.right);
        rcClient.top = rc.bottom;

        GetWindowRect(hWndStatusbar, &rc);
        ScreenToClient(hwnd, (LPPOINT)&rc.left);
        rcClient.bottom = rc.top;

        MoveWindow(hwndMDIClient,
                   rcClient.left,
                   rcClient.top,
                   rcClient.right-rcClient.left,
                   rcClient.bottom-rcClient.top,
                   TRUE);
    }

    return 0 ;
}


//
//  FUNCTION: MsgClose(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Close the editor.
//
//  PARAMETERS:
//    hwnd     - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam   - Message specific data (unused).
//    lparam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//
//

#pragma warn -pia
LRESULT MsgClose(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	 HWND hwndT;
	 BOOL bClose = TRUE;

    // As long as the MDI client has a child, query to save it
    while (hwndT = GetWindow(hwndMDIClient, GW_CHILD))
    {
        // Skip the icon and title windows
        while (hwndT && GetWindow(hwndT, GW_OWNER))
            hwndT = GetWindow(hwndT, GW_HWNDNEXT);

        if (hwndT)
        {
				if (!QuerySaveFile(hwnd))
            {
                bClose=FALSE;
                break;
            }
            else
                SendMessage(hwndMDIClient, WM_MDIDESTROY, (WPARAM)hwndT, 0L);
        }
        else
            break;
    }

    if (bClose)
        return DefWindowProc(hwnd, uMessage, wparam, lparam);
    else
		  return 0;
}
#pragma warn .pia


//
//  FUNCTION: MsgQueryEndSession(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles the case where user attempts to quit with unsaved changes.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    TRUE - Quiting is now safe.
//    FALSE - Don't quit.
//
//  COMMENTS:
//    Let the function QuerySaveFile handle the real work.
//

#pragma argsused
LRESULT MsgQueryEndSession(HWND hwnd,
									UINT uMessage,
                           WPARAM wparam,
                           LPARAM lparam)
{
    return QuerySaveFile(hwnd);
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
//  FUNCTION: CmdFileNew(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To create a new mdi child window.
//
//  PARAMETERS:
//    hwnd     - The window handling the command.
//    wCommand - IDM_FILENEW (unused)
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT CmdFileNew(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
	 char rgch[15];

	 wsprintf(rgch, "Untitled%d", cUntitled);

    CreateMDIChild(rgch);

    RTF_ShowCharAttributes();

    return 0;
}


//
//  FUNCTION: CmdFileClose(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To close the active mdi child window.
//
//  PARAMETERS:
//    hwnd     - The window handling the command.
//    wCommand - IDM_FILECLOSE (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT CmdFileClose(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    HWND hwndT;

    hwndT = GetActiveMDIChild();
    if (hwndT != NULL)
        SendMessage(hwndT, WM_CLOSE, 0, 0L);

    return 0;
}


//
//  FUNCTION: CmdClip(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Handle clipboard commands.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_CUT, IDM_COPY, IDM_CLEAR IDM_PASTE, IDM_UNDO
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//    Translate the commands into messages and send them to the edit control.
//

#pragma argsused
LRESULT CmdClip(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
	 WORD wMessage = 0;

	 switch (wCommand)
	 {
		  case IDM_EDITCUT:   wMessage = WM_CUT;   break;
        case IDM_EDITCOPY:  wMessage = WM_COPY;  break;
        case IDM_EDITPASTE: wMessage = WM_PASTE; break;
        case IDM_EDITCLEAR: wMessage = WM_CLEAR; break;
        case IDM_EDITUNDO:  wMessage = EM_UNDO;  break;
    }

    SendMessage(GetEditWindow(NULL), wMessage, 0, 0L);
    return 0;
}


//
//  FUNCTION: CmdWindowTile(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To tile the mdi child windows.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - IDM_WINDOWTILE (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT CmdWindowTile(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
	 SendMessage(hwndMDIClient, WM_MDITILE, 0, 0L);

	 return 0;
}


//
//  FUNCTION: CmdWindowCascade(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To cascade the mdi child windows.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - IDM_WINDOWCASCADE (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT CmdWindowCascade(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    SendMessage(hwndMDIClient, WM_MDICASCADE, 0, 0L);

    return 0;
}


//
//  FUNCTION: CmdWindowIcons(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To arrage the mdi child icons.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - IDM_WINDOWICONS (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT CmdWindowIcons(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    SendMessage(hwndMDIClient, WM_MDIICONARRANGE, 0, 0L);

    return 0;
}


//
//  FUNCTION: CmdWindowCloseAll(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To close all of the mdi child windows.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - IDM_WINDOWCLOSEALL (unused).
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

#pragma warn -pia
#pragma argsused
LRESULT CmdWindowCloseAll(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
	 HWND hwndT;
	 BOOL bCancel = 0;

	 // As long as the MDI client has a child, destroy it
	 while (!bCancel && (hwndT = GetWindow(hwndMDIClient, GW_CHILD)))
	 {
		  // Skip the icon and title windows
        while (hwndT && GetWindow(hwndT, GW_OWNER))
				hwndT = GetWindow(hwndT, GW_HWNDNEXT);

        if (hwndT)
        {
				if (!QuerySaveFile(hwnd))
               bCancel=TRUE;
            else
               SendMessage(hwndMDIClient, WM_MDIDESTROY, (WPARAM)hwndT, 0L);
        }
        else
            break;
    }

    return bCancel;
}
#pragma warn .pia

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
    BOOL bCancel;

    bCancel = SendMessage(hwnd, WM_COMMAND, IDM_WINDOWCLOSEALL, 0L);

    if (!bCancel)
       DestroyWindow(hwnd);
    return 0;
}

//
//  FUNCTIONS: CmdBold(HWND, WORD, WORD, HWND)
//             CmdItalic(HWND, WORD, WORD, HWND)
//             CmdUnderline(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Apply/Remove Bold, Italic, or underline attributes
//           to selected text.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_EDITBOLD, IDM_EDITITALIC, IDM_EDITUNDERLINE
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
LRESULT CmdBold(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
	 RTF_ChangeCharAttribute(GetEditWindow(NULL), CFM_BOLD, CFE_BOLD);
	 RTF_ShowCharAttributes();
	 return 0L;
}

#pragma argsused
LRESULT CmdItalic(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
	 RTF_ChangeCharAttribute(GetEditWindow(NULL), CFM_ITALIC, CFE_ITALIC);
	 RTF_ShowCharAttributes();
	 return 0L;
}

#pragma argsused
LRESULT CmdUnderline(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
	 RTF_ChangeCharAttribute(GetEditWindow(NULL), CFM_UNDERLINE, CFE_UNDERLINE);
	 RTF_ShowCharAttributes();
	 return 0L;
}

//
//  FUNCTION: CmdFontDialog(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Bring up Common FOnt Dialog to let User change
//           any attributes they want on the selected text's font.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_EDITFONTDIALOG
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
LRESULT CmdFontDialog(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
	 RTF_ChangeFont(GetParent(GetEditWindow(NULL)), GetEditWindow(NULL));
	 RTF_ShowCharAttributes();
    return 0L;
}

//
//  FUNCTIONS: CmdIncreaseFont(HWND, WORD, WORD, HWND)
//             CmdDecreaseFont(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Increase or Decrease the selected text's font size
//           by 2 points.  
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_EDITINCREASEFONT, IDM_EDITDECREASEFONT
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
LRESULT CmdIncreaseFont(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    RTF_ChangeSizeAttribute(GetEditWindow(NULL),2); // Increase 2 points
    return 0L;
}

#pragma argsused
LRESULT CmdDecreaseFont(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    RTF_ChangeSizeAttribute(GetEditWindow(NULL),-2); // Decrease 2 points
    return 0L;
}

//
//  FUNCTION: GetFName(VOID)
//
//  PURPOSE: Get the current file name.
//
//  PARAMETERS:
//    NONE
//
//  RETURN VALUE:
//    The full path name of the current file.
//
//  COMMENTS:
//
//

char *GetFName(VOID)
{
	 return szFName;
}

