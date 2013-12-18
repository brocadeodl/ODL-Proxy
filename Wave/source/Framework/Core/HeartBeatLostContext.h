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

#ifndef HEARTBEATLOSTCONTEXT_H
#define HEARTBEATLOSTCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include <vector>
#include <string>

namespace WaveNs
{

class HeartBeatLostContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                      HeartBeatLostContext    (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual      ~HeartBeatLostContext    ();

                void   addNodeIpAddressAndPort (const string &nodeIpAddress, const UI32 &port);
                UI32   getNumberOfNodes        () const;
                string getNodeIpAddressAtIndex (const UI32 &index) const;
                UI32   getNodePortAtIndex      (const UI32 &index) const;

    // Now the data members

    private :
        vector<string> m_nodeIpAddresses;
        vector<UI32>   m_nodePorts;

    protected :
    public :
};

}

#endif // HEARTBEATLOSTCONTEXT_H
