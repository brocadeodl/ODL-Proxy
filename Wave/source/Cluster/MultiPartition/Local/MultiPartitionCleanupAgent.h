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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayaraman Iyer                                               *
 ***************************************************************************/

#ifndef MULTIPARTITIONCLEANUPAGENT_H
#define MULTIPARTITIONCLEANUPAGENT_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class MultiPartitionCleanupAgentContext;

class MultiPartitionCleanupAgent : protected WaveWorker
{
    private :
    protected :
    public :
                            MultiPartitionCleanupAgent            (WaveObjectManager *pWaveObjectManager, string &partitionName, UI32 senderServiceId , ObjectId &ownerPartitionManagedObjectId);
        virtual            ~MultiPartitionCleanupAgent            ();
        virtual ResourceId  execute                             ();

                ResourceId  getListOfEnabledServicesStep        (MultiPartitionCleanupAgentContext *pMultiPartitionCleanupAgentContext);
                ResourceId  sendMultiPartitionCleanupStep         (MultiPartitionCleanupAgentContext *pMultiPartitionCleanupAgentContext);

                bool        requiresMultiPartitionCleanupNotification (const PrismServiceId &prismServiceId);
                ObjectId    getOwnerPartitionManagedObjectId    () const ;
                bool        getIsPartialCleanup                 () const;

                void        setPartialCleanupTag                (const ResourceId &partialCleanupTag);
                ResourceId  getPartialCleanupTag                ();


    // Now the data members

    private :
                string     m_partitionName;
                UI32       m_senderServiceId;
                ObjectId   m_ownerPartitionManagedObjectId; 
                bool       m_isPartialCleanup;
                ResourceId m_partialCleanupTag;

    protected :
    public :
};

}

#endif // SLOTFAILOVERAGENT_H
