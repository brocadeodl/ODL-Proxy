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

/**
 *@file ClusterLocalSetHardwareSynchronizationMessage.h
 * Copyright (C) 2011 BrocadeCommunications Systems,Inc.
 * All rights reserved. 
 *
 * Description: This file declares the message class handled
 *              by ClusterLocalOM which is used for updating
 *              the hardware synchronization state attribute
 *              in the WaveNode Managed Object. 
 *
 * Author :     Brian Adaniya
 * Date :       05/05/2011
 */

#ifndef CLUSTERLOCALSETHARDWARDSYNCHRONIZATIONMESSAGE_H 
#define CLUSTERLOCALSETHARDWARDSYNCHRONIZATIONMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

namespace WaveNs
{

class ClusterLocalSetHardwareSynchronizationStateMessage : public PrismMessage
{
    private :
                void                setupAttributesForSerialization                     ();

    protected :
    public :
                                    ClusterLocalSetHardwareSynchronizationStateMessage  ();
                                    ClusterLocalSetHardwareSynchronizationStateMessage  (ResourceId hardwareSynchronizationState);
        virtual                    ~ClusterLocalSetHardwareSynchronizationStateMessage  ();

                ResourceId          getHardwareSynchronizationState                     () const;
                void                setHardwareSynchronizationState                     (ResourceId hardwareSynchronizationState);

    // Now the data members

    private :
                ResourceId          m_hardwareSynchronizationState;

    protected :
    public :
};

}

#endif // CLUSTERLOCALSETHARDWARDSYNCHRONIZATIONMESSAGE_H

