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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef UNIXDOMAINSTREAMINGSOCKET_H
#define UNIXDOMAINSTREAMINGSOCKET_H

#include "Framework/Utils/StreamingSocketBase.h"
#include <sys/types.h>
#include <string>

namespace WaveNs
{

class UnixDomainStreamingSocket : public StreamingSocketBase
{
    private :
    protected :
    public :
                       UnixDomainStreamingSocket          (const SI32 maximumNumberOfConnections);
                       UnixDomainStreamingSocket          ();
        virtual       ~UnixDomainStreamingSocket          ();

        virtual bool   bind                               (const SI32 port);
        virtual bool   accept                             (StreamingSocketBase &newStreamingSocketBase);
        virtual bool   connect                            (const string &host, const SI32 port);

/*
        virtual string getSourceIpAddress                 ();
        virtual string getPeerIpAddress                   ();
        virtual SI32   getPeerPort                        ();
        static  string getAPublicIpAddressForThisMachine  ();
        static  void   getPublicIpAddressesForThisMachine (vector<string> &ipAddressesVector);
        static  string getIpAddressForInterface           (const string &interfaceName);
*/
    // Now the data members

    private :
        sockaddr_un m_socketAddress;

        static const string serverPathPrefix;

    protected :
    public :
};

}

#endif // UNIXDOMAINSTREAMINGSOCKET_H
