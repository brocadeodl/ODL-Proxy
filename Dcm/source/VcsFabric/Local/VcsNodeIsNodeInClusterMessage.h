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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Pritee Ghosh                                                 *
 ***************************************************************************/

#ifndef VCSNODEISNODEINCLUSTERMESSAGE_H
#define VCSNODEISNODEINCLUSTERMESSAGE_H 

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeIsNodeInClusterMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        //PhysicalInterfaceCreateMessage ();
                        VcsNodeIsNodeInClusterMessage();
                        VcsNodeIsNodeInClusterMessage (const UI32 rbridgeId);
        virtual        ~VcsNodeIsNodeInClusterMessage ();

                UI32    getRBridgeId                     () const;
                void    setRBridgeId                     (UI32 rbridgeId);
                bool    getIsNodeInCluster               () const;
                void    setIsNodeInCluster               (bool isNodeInCluster); 

    // Now the data members

    private :
        UI32 m_rbridgeId;
        bool m_isNodeInCluster;

    protected :
    public :
};

}

#endif // VCSNODEISNODEINCLUSTERMESSAGE_H
