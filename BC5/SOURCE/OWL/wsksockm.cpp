//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1997 by Borland International, All Rights Reserved
//
//$Revision:   10.11  $
//
// Winsock for OWL subsystem.
// Based on work by Paul Pedriana, 70541.3223@compuserve.com
//----------------------------------------------------------------------------
#include <owl/pch.h>
#if !defined(OWL_WINSOCK_H)
# include <owl/winsock.h>
#endif

OWL_DIAGINFO;

//
// Default constructor. Initializes all data members to 0.
//
TSocketInfo::TSocketInfo()
{
  wVersion = 0;
  wHighVersion = 0;
  szDescription[0] = 0;
  szSystemStatus[0] = 0;
  iMaxSockets = 0;
  iMaxUdpDg = 0;
  lpVendorInfo = 0;
}

//----------------------------------------------------------------------------

//
// The SocketManager constructor takes a major and minor version as parameters.
//  These version parameters are the Winsock versions that you request, as
//  in WSAStartup() calls.
//
TSocketManager::TSocketManager(short versionMajor, short versionMinor, bool autoStartup)
{
  TWinSock::IsAvailable();  

  StartupCount = 0;
  Init(versionMajor, versionMinor);
  if (autoStartup)
     Startup();
}

//
// The TSocketManager destructor cleans up after itself.  For every time you
//  called Startup() (same as WSAStartup()), this destructor will call
//  ShutDown (same as WSACleanup()).
//
TSocketManager::~TSocketManager()
{
  while (StartupCount)
    ShutDown();  // Call ShutDown() enough times to equal times Startup() was called.
}

//
// The ITSocketManager function does some ititialization for the TSocketManager.
//  This function is separate from the constructor so that you can call it at
//  any time in order to re-specify the desired major and minor versions before
//  a call to Startup().
// nNewVersionMajor and nNewVersionMinor will be the values specified in the
//  Winsock WSAStartup() call that TSocketManager::Startup() makes.
// The return value is TRUE or FALSE.
//
void TSocketManager::Init(short versionMajor, short versionMinor)
{
  LastError    = 0;
  VersionMajor = versionMajor;
  VersionMinor = versionMinor;
}

//
// The Startup() function is equivalent to the Winsock WSAStartup() function.
//
// This function fills out the TSocketManager's SocketInfo structure (same as a
//  WSAData structure) with the return information.
//
// You can then call the GetMaxSocketsAvailable(), GetVendorInfo(), Available(),
//  GetMajorVersion(), GetMinorVersion(), or Information() functions to get the
//  results of the call.
//
int TSocketManager::Startup()
{
   uint16 packedValues = (uint16)(((uint16)VersionMinor) << 8) |
                         (uint16)VersionMajor;
   LastError = TWinSockDll::WSAStartup(packedValues, &SocketInfo);
   if (LastError)
     return WINSOCK_ERROR;

   StartupCount++;
   VersionMajor = HiUint8(SocketInfo.wVersion);
   VersionMinor = LoUint8(SocketInfo.wVersion);

   return WINSOCK_NOERROR;
}

//
// The ShutDown() function is the equivalent to the Winsock WSACleanup() function.
// You can call this function if you want, but in its destructor, the TSocketManager
//  will automatically call it once for every time Startup() was called.  Of course,
//  the TSocketManager cannot know about any independent WSAStartup() direct calls
//  that you may make.
//
int TSocketManager::ShutDown()
{
  if (TWinSockDll::WSACleanup()) {
    LastError = TWinSockDll::WSAGetLastError();
    return WINSOCK_ERROR;
  }
  StartupCount--;
  return WINSOCK_NOERROR;
}
