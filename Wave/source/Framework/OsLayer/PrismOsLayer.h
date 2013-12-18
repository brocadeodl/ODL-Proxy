/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/***************************************************************************
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMOSLAYER_H
#define PRISMOSLAYER_H

#include <unistd.h>
#include <sys/time.h>
#include <syslog.h>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "Framework/Trace/TraceTypes.h"
#include "WaveResourceIdEnums.h"

using namespace std;

namespace WaveNs
{

unsigned int  prismSleep                 (unsigned int numberOfSecondsToSleep);
unsigned int  prismUSleep                (unsigned int numberOfMicroSecondsToSleep);
void          prismSysLog                (const char * const pStringToLog);
void          initializePrismSocketLayer ();
void          getIpAddressesForThisHost  (vector<string> &ipAddressesForThisHost);
string        getIpAddressForInterface   (const string &interfaceName);
int           prismCloseSocket           (int fd);
void          prismBlockSignals          ();
void          prismInstallSigIntHandler  ();
char         *prismGetCwd                (char *buf, size_t size);
void          prismCreateWaitableTimer   ();
void          prismWaitForAlarmSignal    ();
int           prismSetITimer             (struct itimerval *pItimerValue);
void          prismSetConsoleTextColor   (TraceLevel requestedTraceLevel);
void          prismResetConsoleTextColor ();
string        getIPv6AddressForInterface (const string &interfaceName);
string        getStateForInterface       (const string &interfaceName);

}

#endif // PRISMOSLAYER_H

