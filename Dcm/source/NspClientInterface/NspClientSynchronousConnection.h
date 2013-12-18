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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef NSPCLIENTSYNCHRONOUSCONNECTION_H
#define NSPCLIENTSYNCHRONOUSCONNECTION_H

#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "DcmResourceIdEnums.h"

using namespace WaveNs;
using namespace DcmNs;

namespace NspNs
{

class NspClientSynchronousConnection : public WaveClientSynchronousConnection
{
    private :
    protected :
    public :
                            NspClientSynchronousConnection (const string &nspClientName, const string &waveServerIpAddress, const UI32 &waveServerPort);

        virtual            ~NspClientSynchronousConnection ();

                ResourceId  createNetwork                  (const string &networkName);

        // Now the data members

    private :
    protected :
    public :
};

}

#endif // NSPCLIENTSYNCHRONOUSCONNECTION_H
