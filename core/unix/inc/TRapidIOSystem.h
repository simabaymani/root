// @(#)root/unix:$Id$
// Author: Fons Rademakers   15/09/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TRapidIOSystem
#define ROOT_TRapidIOSystem


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRapidIOSystem                                                       //
//                                                                      //
// Class providing an interface to the RapidIO framework.               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TSystem
#include "TSystem.h"
#endif
#ifndef ROOT_TUnixSystem
#include "TUnixSystem.h"
#endif
#ifndef ROOT_TSysEvtHandler
#include "TSysEvtHandler.h"
#endif
#ifndef ROOT_TTimer
#include "TTimer.h"
#endif


class TRapidIOSystem : public TUnixSystem {

protected:

   // static functions providing semi-low level interface to raw RapidIO
   static int          RapidIOTcpConnect(const char *hostname, int port, int tcpwindowsize);
   static int          RapidIOTcpService(int port, Bool_t reuse, int backlog,
                                      int tcpwindowsize);
   static int          RapidIORecv(int sock, void *buf, int len, int flag);
   static int          RapidIOSend(int sock, const void *buf, int len, int flag);

public:
   TRapidIOSystem();
   virtual ~TRapidIOSystem();

   Bool_t            Init();

   //---- RPC --------------------------------------------------
   int               ConnectService(const char *server, int port, int tcpwindowsize, const char *protocol = "tcp");
   int               OpenConnection(const char *server, int port, int tcpwindowsize = -1, const char *protocol = "tcp");
   int               AnnounceTcpService(int port, Bool_t reuse, int backlog, int tcpwindowsize = -1);
   int               AcceptConnection(int sock);
   void              CloseConnection(int sock, Bool_t force = kFALSE);
   int               RecvRaw(int sock, void *buffer, int length, int flag);
   int               SendRaw(int sock, const void *buffer, int length, int flag);
   int               RecvBuf(int sock, void *buffer, int length);
   int               SendBuf(int sock, const void *buffer, int length);

   ClassDef(TRapidIOSystem,0)  //Interface to RapidIO framework
};

#endif
