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
 *   Author : sholla                                                     *
 **************************************************************************/

#ifndef UIDSERVICEOBJECTMANAGER_H
#define UIDSERVICEOBJECTMANAGER_H

#include "UidServiceTypes.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Utils/DceSynchronousLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

    class UidServiceMessage;
    class UidServiceQueryMessage;


    typedef struct {
        const UI32  id_type;        // ID type ( protocol )
        const UI32  start_value;    // start ID
        const UI32  end_value;      // end ID
        const UI32  max_count;      // Max number if IDs
        const UI32  alloc_type;     // Monotonic or random??
    } id_type_info;


    class UidServiceObjectManager: public WaveObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

            void UidServiceMessageHandler(UidServiceMessage *pUidServiceMessage);

            void UidAllocHandler(UidServiceMessage *msg);
            ResourceId validateUidAssign(DceSynchronousLinearSeqContext *seqContext);
            ResourceId doUidAlloc(DceSynchronousLinearSeqContext *seqContext);

            void UidFreeHandler(UidServiceMessage *msg);
            void UidRemoveHandler(UidServiceMessage *msg);
            ResourceId doUidFree(DceSynchronousLinearSeqContext *seqContext);
            ResourceId doUidRemove(DceSynchronousLinearSeqContext *seqContext);

            void UidSetHandler(UidServiceMessage *msg);
            ResourceId doUidSet(DceSynchronousLinearSeqContext *seqContext);

            void UidQueryHandler (UidServiceQueryMessage *msg);
            ResourceId doUidQuery (DceSynchronousLinearSeqContext *seqContext);

            id_type_info *findMetadata(UI32 idType);
            UI32 calculateNextId(id_type_info *typeInfo, UI32 idCount);

        protected:
        public:
            UidServiceObjectManager ();
            virtual    ~UidServiceObjectManager ();
            static UidServiceObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

// Now the data members

        private:

            static id_type_info idTypeMetadata[];

        protected:
        public:
    };

}
#endif                                            //UIDSERVICEOBJECTMANAGER_H
