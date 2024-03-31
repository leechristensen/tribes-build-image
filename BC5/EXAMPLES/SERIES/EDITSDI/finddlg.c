// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: finddlg.c
//
//  PURPOSE: To show the use of the find and replace common dialogs.
//
//
//  FUNCTIONS:
//    InitFindReplace - Register the windows message for find/rep notifications.
//    CmdFindReplace - Opens the find or replace modeless dialog box.
//    MsgFindReplace - Handle the messages generated by the find/replace dialog.
//
//  COMMENTS:
//
//
//
//  SPECIAL INSTRUCTIONS: N/A
//

#include <windows.h>            // required for all Windows applications
#ifdef WIN16
#include <commdlg.h>
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

#define CCHMAXFIND 80
#define CCHMAXREPL 80

static HWND hdlg = NULL;        // handle identifying the find/rep dialog
static WORD wCommCur = 0;       // Current command - find or replace
static char szFind[CCHMAXFIND] = {'\0'}; // The string to find
static char szRepl[CCHMAXREPL] = {'\0'}; // The string to replace

// common dialog box structure
static FINDREPLACE fr = {0,0,0,FR_DOWN|FR_MATCHCASE,0};

//
//  FUNCTION: InitFindReplace(VOID)
//
//  PURPOSE: Register the windows message for find/replace notifications
//
//  PARAMETERS:
//    NONE.
//
//  RETURN VALUE:
//    TRUE - Initialization succeeded - the message was registered.
//    FALSE - Initialization failed - the message was not registered.
//
//  COMMENTS:
//
//

BOOL InitFindReplace(VOID) {
    msdiMain.rgmsd[0].uMessage = RegisterWindowMessage(FINDMSGSTRING);
    return msdiMain.rgmsd[0].uMessage != 0;
}

BOOL IsFindReplaceMsg(LPMSG lpmsg) {
    if (hdlg != NULL)
        return IsDialogMessage(hdlg, lpmsg);
    else
        return FALSE;
}

BOOL CanFind(VOID) {
    return szFind[0] != '\0';
}


//
//  FUNCTION: CmdFindReplace(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Opens the find or replace modeless dialog box.
//
//  PARAMETERS:
//    hwnd     - The window handle.
//    wCommand - IDM_FIND || IDM_REPLACE
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//    If a find or replace dialog already exists, this command just
//      brings it to the top.
//
//

#pragma argsused
LRESULT CmdFindReplace(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl) {
    if (hdlg != NULL) {
        if (wCommand == wCommCur) {
            BringWindowToTop(hdlg);
            return 0;
        }
        else {
            DestroyWindow(hdlg);
        }
    }
    wCommCur = wCommand;

    // Initialize the find/replace structure

    fr.lStructSize = sizeof(FINDREPLACE);
    fr.hwndOwner = hwnd;
    fr.lpstrFindWhat = szFind;
    fr.wFindWhatLen = sizeof(szFind);
    fr.lpstrReplaceWith = szRepl;
    fr.wReplaceWithLen = sizeof(szRepl);

    // Display the modeless Find/Replace dialog box.

    if (wCommand == IDM_FIND) {
        hdlg = FindText(&fr);
    }
    else {
        hdlg = ReplaceText(&fr);
    }

    return 0;
}

//
//  FUNCTION: MsgFindReplace(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the messages generated by the find/replace dialog.
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - LPFINDREPLACE
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    If FR_DIALOGTERM flag is set, set hdlg to NULL so that the CmdFindReplace
//    function can open another one when it is called.
//    Otherwise call the user defined FindReplace function with the information
//    about the find/replace to do the real work.
//

#pragma argsused
LRESULT MsgFindReplace(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam) {
	 LPFINDREPLACE lpfr;
	 FRT frt;

	 // Extract a pointer to the FINDREPLACE structure from lParam.

    lpfr = (FINDREPLACE FAR*) lparam;

    // If the system has set the FR_DIALOGTERM flag, invalidate the
    //  handle identifying the dialog box.

    if (lpfr->Flags & FR_DIALOGTERM){
        hdlg = NULL;
        lpfr->Flags &= ~FR_DIALOGTERM;
        return 0;
    }

	 // Determine if the request is a find, replace, or replace all.

    if (lpfr->Flags & FR_FINDNEXT) {
        frt = frtFind;
    }
    else if (lpfr->Flags & FR_REPLACE) {
        frt = frtReplace;
    }
    else if (lpfr->Flags & FR_REPLACEALL) {
        frt = frtRepAll;
    }
    else {
        return 0;
	 }

    // If the dialog box is still valid, call the application-defined
    //   search or replace routine

    FindReplace(
        szFind, szRepl,
        frt,
        MAKEBOOL(lpfr->Flags & FR_DOWN),
        MAKEBOOL(lpfr->Flags & FR_MATCHCASE),
        MAKEBOOL(lpfr->Flags & FR_WHOLEWORD)
    );

	 return 0;
}

//
//  FUNCTION: CmdFindNext(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Opens the find or replace modeless dialog box.
//
//  PARAMETERS:
//    hwnd     - The window handle.
//    wCommand - IDM_FINDPREV || IDM_FINDNEXT
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//    If a find or replace dialog already exists, this command just
//      brings it to the top.
//
//

#pragma argsused
LRESULT CmdFindNext(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl) {
    FindReplace(
        szFind, szRepl,
        frtFind,
        wCommand == IDM_FINDNEXT,
        MAKEBOOL(fr.Flags & FR_MATCHCASE),
        MAKEBOOL(fr.Flags & FR_WHOLEWORD)
    );

    return 0;
}
