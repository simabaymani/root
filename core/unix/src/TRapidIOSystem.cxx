// @(#)root/unix:$Id: 887c618d89c4ed436e4034fc133f468fecad651b $
// Author: Fons Rademakers   15/09/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRapidIOSystem                                                       //
//                                                                      //
// Class providing an interface to the RapidIO framework.               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "RConfigure.h"
#include "RConfig.h"
#include "TUnixSystem.h"
#include "TRapidIOSystem.h"
#include "TROOT.h"
#include "TError.h"
#include "TOrdCollection.h"
#include "TRegexp.h"
#include "TPRegexp.h"
#include "TException.h"
#include "Demangle.h"
#include "TEnv.h"
#include "TSocket.h"
#include "Getline.h"
#include "TInterpreter.h"
#include "TApplication.h"
#include "TObjString.h"
#include "Riostream.h"
#include "TVirtualMutex.h"
#include "TObjArray.h"
#include <map>
#include <algorithm>
#include <atomic>

ClassImp(TRapidIOSystem)

////////////////////////////////////////////////////////////////////////////////

TRapidIOSystem::TRapidIOSystem() : TUnixSystem()
{
  ::Info("TRapidIOSystem::TRapidIOSystem", "Done.");
}

////////////////////////////////////////////////////////////////////////////////
/// Reset to original state.

TRapidIOSystem::~TRapidIOSystem()
{
  ::Info("TRapidIOSystem::~TRapidIOSystem", "Done.");
}

////////////////////////////////////////////////////////////////////////////////
/// Initialize RapidIO system interface.

Bool_t TRapidIOSystem::Init()
{
  ::Info("TRapidIOSystem::Init", "Done.");
   if (TUnixSystem::Init())
      return kTRUE;

   return kFALSE;
}


//---- RPC ---------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// Get Internet Protocol (IP) address of remote host and port #.

TInetAddress TRapidIOSystem::GetPeerName(int sock)
{
  ::Info("TRapidIOSystem::GetPeerName", "Done.");
  return TInetAddress();
}

////////////////////////////////////////////////////////////////////////////////
/// Connect to service servicename on server servername.

int TRapidIOSystem::ConnectService(const char *servername, int port,
                                int tcpwindowsize, const char *protocol)
{
  ::Info("TRapidIOSystem::ConnectService", "Done.");
  return RapidIOTcpConnect(servername, port, tcpwindowsize);
}

////////////////////////////////////////////////////////////////////////////////
/// Open a connection to a service on a server. Returns -1 in case
/// connection cannot be opened.
/// Use tcpwindowsize to specify the size of the receive buffer, it has
/// to be specified here to make sure the window scale option is set (for
/// tcpwindowsize > 65KB and for platforms supporting window scaling).
/// Is called via the TSocket constructor.

int TRapidIOSystem::OpenConnection(const char *server, int port, int tcpwindowsize, const char *protocol)
{
  ::Info("TRapidIOSystem::OpenConnection", "Done.");
  return ConnectService(server, port, tcpwindowsize, protocol);
}

////////////////////////////////////////////////////////////////////////////////
/// Announce TCP/IP service.
/// Open a socket, bind to it and start listening for TCP/IP connections
/// on the port. If reuse is true reuse the address, backlog specifies
/// how many sockets can be waiting to be accepted.
/// Use tcpwindowsize to specify the size of the receive buffer, it has
/// to be specified here to make sure the window scale option is set (for
/// tcpwindowsize > 65KB and for platforms supporting window scaling).
/// Returns socket fd or -1 if socket() failed, -2 if bind() failed
/// or -3 if listen() failed.
/* SIMA */
int TRapidIOSystem::AnnounceTcpService(int port, Bool_t reuse, int backlog,
                                    int tcpwindowsize)
{
  ::Info("TRapidIOSystem::AnnounceTcpService", "Done.");
   return RapidIOTcpService(port, reuse, backlog, tcpwindowsize);
}


////////////////////////////////////////////////////////////////////////////////
/// Accept a connection. In case of an error return -1. In case
/// non-blocking I/O is enabled and no connections are available
/// return -2.

int TRapidIOSystem::AcceptConnection(int sock)
{
  ::Info("TRapidIOSystem::AcceptConnection", "Done.");
  return 1;
  // int soc = -1;

   // while ((soc = ::accept(sock, 0, 0)) == -1 && GetErrno() == EINTR)
   //    ResetErrno();

   // if (soc == -1) {
   //    if (GetErrno() == EWOULDBLOCK)
   //       return -2;
   //    else {
   //       SysError("AcceptConnection", "accept");
   //       return -1;
   //    }
   // }

   // return soc;
}

////////////////////////////////////////////////////////////////////////////////
/// Close socket.

void TRapidIOSystem::CloseConnection(int sock, Bool_t force)
{
  ::Info("TRapidIOSystem::CloseConnection", "Done.");
  //    if (sock < 0) return;

// #if !defined(R__AIX) || defined(_AIX41) || defined(_AIX43)
//    if (force)
//       ::shutdown(sock, 2);   // will also close connection of parent
// #endif

//    while (::close(sock) == -1 && GetErrno() == EINTR)
//       ResetErrno();
}

////////////////////////////////////////////////////////////////////////////////
/// Receive a buffer headed by a length indicator. Length is the size of
/// the buffer. Returns the number of bytes received in buf or -1 in
/// case of error.

int TRapidIOSystem::RecvBuf(int sock, void *buf, int length)
{
  ::Info("TRapidIOSystem::RecvBuf", "Done.");
  return RapidIORecv(sock, buf, length, 0);
   // Int_t header;

   // if (RapidIORecv(sock, &header, sizeof(header), 0) > 0) {
   //    int count = ntohl(header);

   //    if (count > length) {
   //       Error("RecvBuf", "record header exceeds buffer size");
   //       return -1;
   //    } else if (count > 0) {
   //       if (RapidIORecv(sock, buf, count, 0) < 0) {
   //          Error("RecvBuf", "cannot receive buffer");
   //          return -1;
   //       }
   //    }
   //    return count;
   // }
   // return -1;
}

////////////////////////////////////////////////////////////////////////////////
/// Send a buffer headed by a length indicator. Returns length of sent buffer
/// or -1 in case of error.

int TRapidIOSystem::SendBuf(int sock, const void *buf, int length)
{
  ::Info("TRapidIOSystem::SendBuf", "Done.");
  return RapidIOSend(sock, buf, length, 0);
  // Int_t header = htonl(length);

   // if (RapidIOSend(sock, &header, sizeof(header), 0) < 0) {
   //    Error("SendBuf", "cannot send header");
   //    return -1;
   // }
   // if (length > 0) {
   //    if (RapidIOSend(sock, buf, length, 0) < 0) {
   //       Error("SendBuf", "cannot send buffer");
   //       return -1;
   //    }
   // }
   // return length;
}

////////////////////////////////////////////////////////////////////////////////
/// Receive exactly length bytes into buffer. Use opt to receive out-of-band
/// data or to have a peek at what is in the buffer (see TSocket). Buffer
/// must be able to store at least length bytes. Returns the number of
/// bytes received (can be 0 if other side of connection was closed) or -1
/// in case of error, -2 in case of MSG_OOB and errno == EWOULDBLOCK, -3
/// in case of MSG_OOB and errno == EINVAL and -4 in case of kNoBlock and
/// errno == EWOULDBLOCK. Returns -5 if pipe broken or reset by peer
/// (EPIPE || ECONNRESET).

int TRapidIOSystem::RecvRaw(int sock, void *buf, int length, int opt)
{
  ::Info("TRapidIOSystem::RecvRaw", "Done.");
  return RapidIORecv(sock, buf, length, opt);
   // int flag;

   // switch (opt) {
   // case kDefault:
   //    flag = 0;
   //    break;
   // case kOob:
   //    flag = MSG_OOB;
   //    break;
   // case kPeek:
   //    flag = MSG_PEEK;
   //    break;
   // case kDontBlock:
   //    flag = -1;
   //    break;
   // default:
   //    flag = 0;
   //    break;
   // }

   // int n;
   // if ((n = RapidIORecv(sock, buf, length, flag)) <= 0) {
   //    if (n == -1 && GetErrno() != EINTR)
   //       Error("RecvRaw", "cannot receive buffer");
   //    return n;
   // }
   // return n;
}

////////////////////////////////////////////////////////////////////////////////
/// Send exactly length bytes from buffer. Use opt to send out-of-band
/// data (see TSocket). Returns the number of bytes sent or -1 in case of
/// error. Returns -4 in case of kNoBlock and errno == EWOULDBLOCK.
/// Returns -5 if pipe broken or reset by peer (EPIPE || ECONNRESET).

int TRapidIOSystem::SendRaw(int sock, const void *buf, int length, int opt)
{
  ::Info("TRapidIOSystem::SendRaw", "Done.");
  return RapidIOSend(sock, buf, length, opt);
   // int flag;

   // switch (opt) {
   // case kDefault:
   //    flag = 0;
   //    break;
   // case kOob:
   //    flag = MSG_OOB;
   //    break;
   // case kDontBlock:
   //    flag = -1;
   //    break;
   // case kPeek:            // receive only option (see RecvRaw)
   // default:
   //    flag = 0;
   //    break;
   // }

   // int n;
   // if ((n = RapidIOSend(sock, buf, length, flag)) <= 0) {
   //    if (n == -1 && GetErrno() != EINTR)
   //       Error("SendRaw", "cannot send buffer");
   //    return n;
   // }
   // return n;
}



//---- RPC -------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// Open a TCP/IP connection to server and connect to a service (i.e. port).
/// Use tcpwindowsize to specify the size of the receive buffer, it has
/// to be specified here to make sure the window scale option is set (for
/// tcpwindowsize > 65KB and for platforms supporting window scaling).
/// Is called via the TSocket constructor. Returns -1 in case of error.

int TRapidIOSystem::RapidIOTcpConnect(const char *hostname, int port,
                                int tcpwindowsize)
{
  ::Info("TRapidIOSystem::RapidIOTcpConnect", "Done.");
  return 0;
  // short  sport;
   // struct servent *sp;

   // if ((sp = getservbyport(htons(port), kProtocolName)))
   //    sport = sp->s_port;
   // else
   //    sport = htons(port);

   // TInetAddress addr = gSystem->GetHostByName(hostname);
   // if (!addr.IsValid()) return -1;
   // UInt_t adr = htonl(addr.GetAddress());

   // struct sockaddr_in server;
   // memset(&server, 0, sizeof(server));
   // memcpy(&server.sin_addr, &adr, sizeof(adr));
   // server.sin_family = addr.GetFamily();
   // server.sin_port   = sport;

   // // Create socket
   // int sock;
   // if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
   //    ::SysError("TRapidIOSystem::UnixTcpConnect", "socket (%s:%d)",
   //               hostname, port);
   //    return -1;
   // }

   // if (tcpwindowsize > 0) {
   //    gSystem->SetSockOpt(sock, kRecvBuffer, tcpwindowsize);
   //    gSystem->SetSockOpt(sock, kSendBuffer, tcpwindowsize);
   // }

   // while (connect(sock, (struct sockaddr*) &server, sizeof(server)) == -1) {
   //    if (GetErrno() == EINTR)
   //       ResetErrno();
   //    else {
   //       ::SysError("TRapidIOSystem::UnixTcpConnect", "connect (%s:%d)",
   //                  hostname, port);
   //       close(sock);
   //       return -1;
   //    }
   // }
   // return sock;
}

////////////////////////////////////////////////////////////////////////////////
/// Open a socket, bind to it and start listening for TCP/IP connections
/// on the port. If reuse is true reuse the address, backlog specifies
/// how many sockets can be waiting to be accepted. If port is 0 a port
/// scan will be done to find a free port. This option is mutual exlusive
/// with the reuse option.
/// Use tcpwindowsize to specify the size of the receive buffer, it has
/// to be specified here to make sure the window scale option is set (for
/// tcpwindowsize > 65KB and for platforms supporting window scaling).
/// Returns socket fd or -1 if socket() failed, -2 if bind() failed
/// or -3 if listen() failed.

int TRapidIOSystem::RapidIOTcpService(int port, Bool_t reuse, int backlog,
                                int tcpwindowsize)
{
  ::Info("TRapidIOSystem::RapidIOTcpService", "Done.");
  return 0;

  // const short kSOCKET_MINPORT = 5000, kSOCKET_MAXPORT = 15000;
   // short  sport, tryport = kSOCKET_MINPORT;
   // struct servent *sp;

   // if (port == 0 && reuse) {
   //    ::Error("TRapidIOSystem::RapidIOTcpService", "cannot do a port scan while reuse is true");
   //    return -1;
   // }

   // if ((sp = getservbyport(htons(port), kProtocolName)))
   //    sport = sp->s_port;
   // else
   //    sport = htons(port);

   // // Create tcp socket
   // int sock;
   // if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
   //    ::SysError("TRapidIOSystem::RapidIOTcpService", "socket");
   //    return -1;
   // }

   // if (reuse)
   //    gSystem->SetSockOpt(sock, kReuseAddr, 1);

   // if (tcpwindowsize > 0) {
   //    gSystem->SetSockOpt(sock, kRecvBuffer, tcpwindowsize);
   //    gSystem->SetSockOpt(sock, kSendBuffer, tcpwindowsize);
   // }

   // struct sockaddr_in inserver;
   // memset(&inserver, 0, sizeof(inserver));
   // inserver.sin_family = AF_INET;
   // inserver.sin_addr.s_addr = htonl(INADDR_ANY);
   // inserver.sin_port = sport;

   // // Bind socket
   // if (port > 0) {
   //    if (::bind(sock, (struct sockaddr*) &inserver, sizeof(inserver))) {
   //       ::SysError("TRapidIOSystem::RapidIOTcpService", "bind");
   //       close(sock);
   //       return -2;
   //    }
   // } else {
   //    int bret;
   //    do {
   //       inserver.sin_port = htons(tryport++);
   //       bret = ::bind(sock, (struct sockaddr*) &inserver, sizeof(inserver));
   //    } while (bret < 0 && GetErrno() == EADDRINUSE && tryport < kSOCKET_MAXPORT);
   //    if (bret < 0) {
   //       ::SysError("TRapidIOSystem::RapidIOTcpService", "bind (port scan)");
   //       close(sock);
   //       return -2;
   //    }
   // }

   // // Start accepting connections
   // if (::listen(sock, backlog)) {
   //    ::SysError("TRapidIOSystem::RapidIOTcpService", "listen");
   //    close(sock);
   //    return -3;
   // }

   // return sock;
}


////////////////////////////////////////////////////////////////////////////////
/// Receive exactly length bytes into buffer. Returns number of bytes
/// received. Returns -1 in case of error, -2 in case of MSG_OOB
/// and errno == EWOULDBLOCK, -3 in case of MSG_OOB and errno == EINVAL
/// and -4 in case of kNoBlock and errno == EWOULDBLOCK.
/// Returns -5 if pipe broken or reset by peer (EPIPE || ECONNRESET).

int TRapidIOSystem::RapidIORecv(int sock, void *buffer, int length, int flag)
{
  ::Info("TRapidIOSystem::RapidIORecv", "Done.");
  return 0;
  // ResetErrno();

   // if (sock < 0) return -1;

   // int once = 0;
   // if (flag == -1) {
   //    flag = 0;
   //    once = 1;
   // }
   // if (flag == MSG_PEEK)
   //    once = 1;

   // int n, nrecv = 0;
   // char *buf = (char *)buffer;

   // for (n = 0; n < length; n += nrecv) {
   //    if ((nrecv = recv(sock, buf+n, length-n, flag)) <= 0) {
   //       if (nrecv == 0)
   //          break;        // EOF
   //       if (flag == MSG_OOB) {
   //          if (GetErrno() == EWOULDBLOCK)
   //             return -2;
   //          else if (GetErrno() == EINVAL)
   //             return -3;
   //       }
   //       if (GetErrno() == EWOULDBLOCK)
   //          return -4;
   //       else {
   //          if (GetErrno() != EINTR)
   //             ::SysError("TRapidIOSystem::RapidIORecv", "recv");
   //          if (GetErrno() == EPIPE || GetErrno() == ECONNRESET)
   //             return -5;
   //          else
   //             return -1;
   //       }
   //    }
   //    if (once)
   //       return nrecv;
   // }
   // return n;
}

////////////////////////////////////////////////////////////////////////////////
/// Send exactly length bytes from buffer. Returns -1 in case of error,
/// otherwise number of sent bytes. Returns -4 in case of kNoBlock and
/// errno == EWOULDBLOCK. Returns -5 if pipe broken or reset by peer
/// (EPIPE || ECONNRESET).

int TRapidIOSystem::RapidIOSend(int sock, const void *buffer, int length, int flag)
{
  ::Info("TRapidIOSystem::RapidIOSend", "Done.");
  return 0;
  // if (sock < 0) return -1;

   // int once = 0;
   // if (flag == -1) {
   //    flag = 0;
   //    once = 1;
   // }

   // int n, nsent = 0;
   // const char *buf = (const char *)buffer;

   // for (n = 0; n < length; n += nsent) {
   //    if ((nsent = send(sock, buf+n, length-n, flag)) <= 0) {
   //       if (nsent == 0)
   //          break;
   //       if (GetErrno() == EWOULDBLOCK)
   //          return -4;
   //       else {
   //          if (GetErrno() != EINTR)
   //             ::SysError("TRapidIOSystem::UnixSend", "send");
   //          if (GetErrno() == EPIPE || GetErrno() == ECONNRESET)
   //             return -5;
   //          else
   //             return -1;
   //       }
   //    }
   //    if (once)
   //       return nsent;
   // }
   // return n;
}

////////////////////////////////////////////////////////////////////////////////
/// Get socket option.

int TRapidIOSystem::GetSockOpt(int sock, int opt, int *val)
{
  *val = 1;
  ::Info("TRapidIOSystem::GetSockOpt", "Done.");
   return 0;
}
