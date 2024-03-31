//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1997 by Borland International, All Rights Reserved
//
//$Revision:   10.13  $
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
// Actually 512 is the absolute guaranteed minimum value.  The WSAData
// structure has the actual value (>=512).
//
int TDatagramSocket::MaxPacketSendSize = 512;

//
// Does nothing. Relies on TSocket to do all the work.
//
TDatagramSocket::TDatagramSocket()
:
  TSocket()
{
}

//
// Does nothing. Relies on TSocket to do all the work.
//
TDatagramSocket::TDatagramSocket(SOCKET& src)
:
  TSocket(src)
{
}

//
// Does nothing. Relies on TSocket to do all the work.
//
TDatagramSocket::TDatagramSocket(TSocketAddress& socketAddress, int addressFormat,
                                 int type, int protocol)
:
  TSocket(socketAddress, addressFormat, type, protocol)
{
}

//
// Reads the Chars into the chData buffer, and removes the data from the queue.
// chData is a pointer to a destination buffer for the data.
// nCharsToRead should be set to the maximum desired read size, which needs to be
//  equal or less to the size of chData.
// The sAddress parameter will get filled with the address of the sender.
// Returns WINSOCK_ERROR if there was an error, WINSOCK_NOERROR otherwise.
// This Read will get the data from the next buffered packet and delete the packet from
//  memory when finished.  Thus if this function is called with a 'nCharsToRead' that
//  is less than the size of the next packet, then only part of the packet will be
//  read and the rest will be lost.
// Upon return, nCharsToRead will be set to the actual number that were read.  If
//  nCharsToRead returns as 0, then no data was read.  If the function return value
//  is WINSOCK_ERROR, then there was a Winsock error, otherwise, the call was
//  successful, even though no data may have been read.
//
int TDatagramSocket::Read(char* data, int& charsToRead, TSocketAddress& address)
{
  // Simply call recvfrom() and return if some kind of error is encountered.
  //
  int addressSize = sizeof(sockaddr);
  int charsReceived = TWinSockDll::recvfrom(Handle, data, charsToRead, 0,
                                              &address, &addressSize);

  if (charsReceived == SOCKET_ERROR) {
    // It is entirely possible to get a blocking error here.  For example, a
    //  call to recv() could have been made right after the message was
    //  posted by the driver, but before this notification function got called.
    //
    charsToRead = 0;  // This will be available for the caller to examine.
    LastError = TWinSockDll::WSAGetLastError();
    if (LastError == WSAEWOULDBLOCK)
      return WINSOCK_NOERROR;  // This is not considered an "error" under Winsock.
    // Some other error occurred, return from this function.
    //
    return WINSOCK_ERROR;
  }
  charsToRead = charsReceived;
  return WINSOCK_NOERROR;
}

//
// Puts the data in the queue and attempts to actually write out the first item in the queue.
// At the end of the function, an attempt to write out the queue is made.  If it fails, well, the
//  data will get sent later, after the system has notified us it is ready.
// Returns WINSOCK_ERROR or WINSOCK_NOERROR.
//
int TDatagramSocket::Write(char* data, int& charsToWrite, TSocketAddress& outSocketAddress,
                           bool /*becomeOwnerOfData*/, bool /*copyData*/)
{
  // Note that bBecomeOwnerOfdata and bCopyData are ignored if we are using DataQueues.
  // Note that thus function may block here if blocking is enabled.
  //
  int charsSent = TWinSockDll::sendto(Handle, data, charsToWrite, 0,
                                   &outSocketAddress, sizeof(sockaddr));
  if (charsSent == SOCKET_ERROR) {
    //It is entirely possible to get a blocking error here.  For example, a call to
    //  send() could have been made right after the message was posted by the driver,
    //  but before this notification function got called.
    //
    charsToWrite = 0;
    LastError = TWinSockDll::WSAGetLastError();
    if (LastError == WSAEWOULDBLOCK)
      return WINSOCK_NOERROR;
    return WINSOCK_ERROR;
  }
  charsToWrite = charsSent;
  return WINSOCK_NOERROR;
}

//
// Simply calls the other Write() function with our latest address.
// Arguments and return values are the same.
//
int TDatagramSocket::Write(char* data, int& charsToWrite, bool becomeOwnerOfData, bool copyData)
{
  return Write(data, charsToWrite, PeerSocketAddress, becomeOwnerOfData, copyData);
}

//
// This function gets called whenever the socket gets a read notification.  This means
//  that data on the port is ready to be read.
//
// This function doesn't do much with the nError parameter.  It simply doesn't do the
//  read if there is an error value.  According to Winsock documentation, this
//  error may be "any error in winsock.h".
//
TDatagramSocket::DoReadNotification(const SOCKET& /*socket*/, int error)
{
  if (error) {
    LastError = error; // TWinSockDll::WSAGetLastError();
    return WINSOCK_ERROR;
  }

  // This function dan't do anything on its own without data queues.  Without
  //  DataQueues, you can do two things:  1) subclass TDatagramSocket and write
  //  your own DoReadNotification() or 2) Have the socket redirect the FD_READ
  //  notification to the window of your choice with Socket::SetNotificationSet()
  //  and SetNotificationWindow().
  //
  return WINSOCK_NOERROR;
}

//
// This function gets called whenever the socket gets a write notification.
//
TDatagramSocket::DoWriteNotification(const SOCKET& /*s*/, int error)
{
  if (error) {
    LastError = error; // TWinSockDll::WSAGetLastError();
    return WINSOCK_ERROR;
  }

  // This function dan't do anything on its own without data queues.  Without
  //  DataQueues, you can do two things:  1) subclass TDatagramSocket and write
  //  your own DoWriteNotification() or 2) Have the socket redirect the FD_WRITE
  //  notification to the window of your choice with Socket::SetNotificationSet()
  //  and SetNotificationWindow().
  //
  return WINSOCK_NOERROR;
}

//----------------------------------------------------------------------------

//
// Calls TSocket constructor and initializes the state of the connection
// to not connected.
//
TStreamSocket::TStreamSocket()
:
  TSocket(),
  ConnectStatus(NotConnected)
{
}

//
// Alias constructor.
//
TStreamSocket::TStreamSocket(SOCKET& src)
:
  TSocket(src)
{
  //  what the ConnectStatus is, given just a SOCKET descriptor.
  // Needs to be set somehow:
  // nConnectStatus = ???;
}

//
// Constructor for a protocol defined by an int.
//
TStreamSocket::TStreamSocket(TSocketAddress& socketAddress, int addressFormat, int type, int protocol)
:
  TSocket(socketAddress, addressFormat, type, protocol),
  ConnectStatus(NotConnected)
{
}

//
// Copies the socket connection information.
//
TStreamSocket& TStreamSocket::operator =(TStreamSocket& src)  
{
  TSocket::operator =(src);
  return *this;
}

//
// This reads from the already received and queued data.  This data has already
//  been received from the socket driver.
//
int TStreamSocket::Read(char* data, int& charsToRead)
{
  // Try to receive the characters.
  //
  int charsReceived = TWinSockDll::recv(Handle, data, charsToRead, 0); 
  if (charsReceived == SOCKET_ERROR) {
    // It is entirely possible to get a blocking error here.  For example, a call to
    //  recv() could have been made right after the message was posted by the driver,
    //  but before this notification function got called.
    //
    charsToRead = 0; // This will be available for the caller to examine.
    LastError = TWinSockDll::WSAGetLastError();
    if (LastError == WSAEWOULDBLOCK)
      return WINSOCK_NOERROR;
    // Some other error occurred.
    //
    return WINSOCK_ERROR;
  }
  charsToRead = charsReceived;
  return WINSOCK_NOERROR;
}

//
// Write the buffer into the stream.
//
int TStreamSocket::Write(char* data, int& charsToWrite, int flags,
                bool /*becomeOwnerOfData*/, bool /*copyData*/)
{
  int charsSent = TWinSockDll::send(Handle, data, charsToWrite, flags);
  if (charsSent == SOCKET_ERROR) {
    // It is entirely possible to get a blocking error here.  For example, a call to
    //  send() could have been made right after the message was posted by the driver,
    //  but before this notification function got called.
    //
    charsToWrite = 0;
    LastError = TWinSockDll::WSAGetLastError();
    if (LastError == WSAEWOULDBLOCK) {
      return WINSOCK_NOERROR;
    }
    return WINSOCK_ERROR;
  }
  charsToWrite = charsSent;
  return WINSOCK_NOERROR;
}

//
// Works just like the Read() function but it works on the OOB queue.
//
int TStreamSocket::ReadOOB(char* data, int& charsToRead)
{
  // Try to receive the characters.
  //
  int charsReceived = TWinSockDll::recv(Handle, data, charsToRead, MSG_OOB);
  if (charsReceived == SOCKET_ERROR) {
    // It is entirely possible to get a blocking error here.  For example, a call to
    //  recv() could have been made right after the message was posted by the driver,
    //  but before this notification function got called.
    //
    charsToRead = 0;
    LastError = TWinSockDll::WSAGetLastError();
    if (LastError == WSAEWOULDBLOCK)
      return WINSOCK_NOERROR;
    // Some other error occurred.
    return WINSOCK_ERROR;
  }
  charsToRead = charsReceived;
  return WINSOCK_NOERROR;
}

//
// Perform write operation using out-of-band data (i.e. via a logically
// independent tranmission channel between the connected sockets).
// NOTE: For 'non urgent data', you may used TStreamSocket::Write() instead.
//
int TStreamSocket::WriteOOB(char* data, int& charsToWrite, int flags,
                            bool /*becomeOwnerOfData*/, bool /*copyData*/)
{
  flags |= MSG_OOB;
  int charsSent = TWinSockDll::send(Handle, data, charsToWrite, flags);
  if (charsSent == SOCKET_ERROR) {
    // It is entirely possible to get a blocking error here.  For example, a call to
    //  send() could have been made right after the message was posted by the driver,
    //  but before this notification function got called.
    //
    charsToWrite = 0;
    LastError = TWinSockDll::WSAGetLastError();
    if (LastError == WSAEWOULDBLOCK) {
      return WINSOCK_NOERROR;
    }
    return WINSOCK_ERROR;
  }
  charsToWrite = charsSent;
  return WINSOCK_NOERROR;
}

//
// This function puts this socket into a passive listening mode.
//
TStreamSocket::Listen(int maxQueuedConnections)
{
  if (TWinSockDll::listen(Handle, maxQueuedConnections)) {
    LastError = TWinSockDll::WSAGetLastError();
    return WINSOCK_ERROR;
  }
  ConnectStatus = Listening;
  return WINSOCK_NOERROR;
}

//
// This function sets myPeerSocketAddress to sAddressToConnectTo then calls Connect().
// See Connect() for more details on it.
//
TStreamSocket::Connect(TSocketAddress& addressToConnectTo)
{
  SetPeerSocketAddress(addressToConnectTo);
  return Connect();
}

//
// This function uses 'myPeerSocketAddress'; it needs to be set before callign this function.
// The connection attempt (and this function) should return right away, without blocking.
// When we are actually connected, we get a notification from the driver at our
//  'DoConnectNotification()' function.  Upon receiving that notification, the nConnectStatus
//  will get set to 'nConnected'.
//
// Technically, it is true that a datagram socket can call connect; doing this sets the default address
//  for future send()/recv() calls tha the datagram socket might use.
//  We put it in the TStreamSocket class for simplicity, and because our TDatagramSocket class
//  already supports its own default address system.
//
TStreamSocket::Connect()
{
  if (TWinSockDll::connect(Handle, &PeerSocketAddress, sizeof(sockaddr))) {
    LastError = TWinSockDll::WSAGetLastError();
    return WINSOCK_ERROR;
  }
  // The following code assumes that the socket is in non-blocking mode.  If the socket was in
  //  blocking mode, then nConnectStatus would be nConnected.  Thus, this code is slightly
  //  bugged and can be patched with a call to select() to determine whether the socket is
  //  really connected or not.  Looked at another way, a blocking socket can be said to be
  //  connected if nConnectStatus is either nConnected or nConnectPending.
  //
  ConnectStatus = ConnectPending;
  return WINSOCK_NOERROR;
}

//
// This function will try to accept a connection with the first connecting peer that
//  is waiting in the queue.  If successful, the TStreamSocket& 'socket' will have a
//  valid and connected socket, a proper status of nConnected, and the correct
//  peer socket address.
//
// The caller will usually call this function in response to an Accept notification.
//  The caller merely needs to create a new TStreamSocket and pass it to this
//  function. The default constructor for TStreamSocket can be used, as this function
//  fixes up the missing parts.
//
TStreamSocket::Accept(TStreamSocket& socket)
{
  if (Accept(socket.Handle, socket.PeerSocketAddress) == WINSOCK_NOERROR) {
    socket.ConnectStatus = Connected;
    socket.SetSocketStyle(Family, Type, Protocol); //Copy our Family, etc. just to make sure.
    return WINSOCK_NOERROR;
  }
  return WINSOCK_ERROR;
}

//
// This function will try to accept a connection with the first connecting peer that is
//  waiting in the queue.  If successful, the 'socket' reference argument will be set to
//  a new connected socket.  The sAddress reference argument will get set to the address
//  of the connecting peer.  The caller of this function may immediately use the
//  new socket with data sends, etc.  Note that the caller may want to flag the socket
//  as connected.  If the 'socket' belongs to a 'StreanSocket', then you can set its
//  nConnectionStatus as nConnected.
//
// The return value is either WINSOCK_ERROR or WINSOCK_NOERROR.  If there is an error,
//  then nLastError will be set with the appropriate error.
//
// Note that it is possible that this call could be made when there are no pending
//  socket connecions in the queue.  If this is the case, the call will block
//  if the socket is marked as blocking, and will return WINSOCK_ERROR with
//  WSAEWOULDBLOCK if the socket is marked as non-blocking.
//
// This function is usually called in response to an accept notification.  A socket is
//  set up as a stream socket and listen() is called.  When a connection is ready, the
//  driver notifies the listening socket with a DoAcceptNotification() call.  See the
//  DoAcceptNotification() call for what to do with it.  You will want to call this Accept()
//  function as a result.
//
TStreamSocket::Accept(SOCKET& socket, sockaddr& address)
{
  int addressLength = sizeof(sockaddr);
  socket = TWinSockDll::accept(Handle, &address, &addressLength);

  if (socket == INVALID_SOCKET) {
    LastError = TWinSockDll::WSAGetLastError();
    return WINSOCK_ERROR;
  }
  return WINSOCK_NOERROR;
}

//
// This function gets called whenever the socket gets a read notification.  This means
//  that data on the port is ready to be read.
//
// This function doesn't do much with the nError parameter.  It simply doesn't do the
//  read if there is an error value.  According to Winsock documentation, this
//  error may be "any error in winsock.h".
//
TStreamSocket::DoReadNotification(const SOCKET& /*socket*/, int error)
{
  if (error) {
    LastError = error; // TWinSockDll::WSAGetLastError();
    return WINSOCK_ERROR;
  }
  return WINSOCK_NOERROR;
}

//
// This function gets called whenever the socket gets a write notification.
// This means that data on the port is ready to be written.
//
TStreamSocket::DoWriteNotification(const SOCKET& /*s*/, int error)
{
  if (error) {
    LastError = error;  
    return WINSOCK_ERROR;
  }

  // This function dan't do anything on its own without data queues.  Without
  //  DataQueues, you can do two things:  1) subclass TStreamSocket and write
  //  your own DoWriteNotification() or 2) Have the socket redirect the FD_WRITE
  //  notification to the window of your choice with Socket::SetNotificationSet()
  //  and SetNotificationWindow().
  //
  return WINSOCK_NOERROR;
}

//
// We get this notification when OOB data is ready to be received on the socket port.
//
TStreamSocket::DoOOBNotification(const SOCKET& /*s*/, int error)
{
  if (error) {
    LastError = error; // TWinSockDll::WSAGetLastError();
    return WINSOCK_ERROR;
  }

  // This function dan't do anything on its own without data queues.  Without
  //  DataQueues, you can do two things:  1) subclass TStreamSocket and write
  //  your own DoOOBNotification() or 2) Have the socket redirect the FD_OOB
  //  notification to the window of your choice with Socket::SetNotificationSet()
  //  and SetNotificationWindow().
  //
  return WINSOCK_NOERROR;
}

//
// We get this notification when a client socket on the network is attempting to connect to us.
//Code needs to be written to intercept this notification.
//
TStreamSocket::DoAcceptNotification(const SOCKET& /*s*/, int /*error*/)
{
  return 0; // We don't do anything.  We let the pending acceptance sit there.
}

//
// This means that the connection that we attempted with a server on the network has completed.
// This function gets called sometime after this object makes a connect() attempt.  If the
//  connect attempt was non-blocking, then a notification is posted and we end up here.
// When this function gets called, we our nConnectStatus should be 'nConnecting'.
//
TStreamSocket::DoConnectNotification(const SOCKET& /*s*/, int error)
{
  if (error) {
    // There was an error, and we cannot connect; this may due to a number of reasons.
    // We turn off our 'nConnectStatus' to 'nNotConnected'.  This is becuase our attempt
    //  to connect failed.
    //
    ConnectStatus = NotConnected;
    LastError = error;   
    return WINSOCK_ERROR;
  }
  ConnectStatus = Connected;
  return WINSOCK_NOERROR;
}

//
// This notification gets called when the socket has been closed.
// We mark the socket as not connected, so that the user may know about it.
// It is important that we read any data that may be waiting in the queue before
// changing the status of the connection and doing any notification.
//
TStreamSocket::DoCloseNotification(const SOCKET& /*s*/, int error)
{
  if (error) {
    LastError = error;  
    return WINSOCK_ERROR;
  }
  return WINSOCK_NOERROR;
}
