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

#ifndef MULTIPARTITIONLOCALMESSAGES_H
#define MULTIPARTITIONLOCALMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

namespace WaveNs
{

class MultiPartitionDeleteLocalPartitionMessage : public ManagementInterfaceMessage
{
    private :
        void                    setupAttributesForSerialization  ();

    protected :
    public :
                                MultiPartitionDeleteLocalPartitionMessage (void);
                                MultiPartitionDeleteLocalPartitionMessage (const string &partitionName);
        virtual                ~MultiPartitionDeleteLocalPartitionMessage (void);

                string          getPartitionName                    (void) const;
                void            setPartitionName                    (const string &partitionName);

               void            setSenderServiceId               (const UI32 &senderServiceId);
                UI32            getSenderServiceId               (void) const ; 

                ObjectId        getOwnerPartitionManagedObjectId    () const ;
                void            setOwnerPartitionManagedObjectId    (const ObjectId &objId);

                bool            getIsPartialCleanup                 () const;

                void            setPartialCleanupTag                (const ResourceId &partialCleanupTag);
                ResourceId      getPartialCleanupTag                ();


    // Now the data members
    private :
                string          m_partitionName;
                UI32            m_senderServiceId;
                ObjectId        m_ownerPartitionManagedObjectId;
                bool            m_isPartialCleanup;
                ResourceId      m_partialCleanupTag;


    protected :
    public :
};
}

#endif // MULTIPARTITIONLOCALMESSAGES_H
