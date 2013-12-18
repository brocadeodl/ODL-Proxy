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

#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Framework/Utils/StringUtils.h"
#include "UidService/Global/UidServiceObjectManager.h"
#include "UidService/Global/UidManagedObject.h"
#include "UidService/Global/UidServiceMessage.h"
#include "UidService/Global/UidServiceQueryMessage.h"
#include "UidService/Global/UidServiceTypes.h"
#include "Utils/mo_iterator.h"
#include "DcmResourceIds.h"

namespace DcmNs
{

    typedef enum {
        ALLOC_MONOTONIC = 1,
        ALLOC_RANDOM
    } UIdAllocType;

    id_type_info UidServiceObjectManager::idTypeMetadata[] = {
            { IDTYPE_BRIDGE_ID_LSB,    1, 0xFF,    1,      ALLOC_RANDOM    },
            { IDTYPE_STP_PORT_ID,      1, 0xFF,    0xFF,   ALLOC_MONOTONIC },
            { IDTYPE_RSTP_PORT_ID,     1, 0xFFF,   0xFFF,  ALLOC_MONOTONIC },
            //{ IDTYPE_LACP_PID,         1, 0xFFFF,  0xFFFF, ALLOC_MONOTONIC },
    };


    UidServiceObjectManager::UidServiceObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        UidManagedObject    UidManagedObject    (this);
        UidManagedObject.setupOrm ();
        addManagedClass (UidManagedObject::getClassName ());

        addOperationMap (UID_ALLOC, reinterpret_cast<PrismMessageHandler> (&UidServiceObjectManager::UidAllocHandler));
        addOperationMap (UID_FREE, reinterpret_cast<PrismMessageHandler> (&UidServiceObjectManager::UidFreeHandler));
        addOperationMap (UID_FREE_BULK, reinterpret_cast<PrismMessageHandler> (&UidServiceObjectManager::UidFreeHandler));
        addOperationMap (UID_SET, reinterpret_cast<PrismMessageHandler> (&UidServiceObjectManager::UidSetHandler));
        addOperationMap (UID_REMOVE, reinterpret_cast<PrismMessageHandler> (&UidServiceObjectManager::UidRemoveHandler));

        addOperationMap (UID_QUERY, reinterpret_cast<PrismMessageHandler> (&UidServiceObjectManager::UidQueryHandler));
    }

    UidServiceObjectManager::~UidServiceObjectManager ()
    {
    }

    UidServiceObjectManager  *UidServiceObjectManager::getInstance()
    {
        static UidServiceObjectManager *pUidServiceObjectManager = new UidServiceObjectManager ();

        WaveNs::prismAssert (NULL != pUidServiceObjectManager, __FILE__, __LINE__);

        return (pUidServiceObjectManager);
    }

    string  UidServiceObjectManager::getClassName()
    {
        return ("UidService");
    }

    PrismServiceId  UidServiceObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *UidServiceObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case UID_ALLOC:
            case UID_FREE:
            case UID_SET:
            case UID_REMOVE:
                pPrismMessage = new UidServiceMessage (operationCode);
                break;
            case UID_QUERY:
            case UID_FREE_BULK:
                pPrismMessage = new UidServiceQueryMessage (operationCode);
                break;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *UidServiceObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((UidManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new UidManagedObject(this);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "UidServiceObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    id_type_info *UidServiceObjectManager::findMetadata(UI32 idType)
    {
        int size = sizeof idTypeMetadata / sizeof(id_type_info);
        for (int i = 0; i < size; ++i) {
            if (idTypeMetadata[i].id_type == idType)
                return &idTypeMetadata[i];
        }
        return NULL;
    }

    static inline UI32 nextRand(id_type_info *typeInfo) {
        return (((UI32) rand()) % (typeInfo->end_value - typeInfo->start_value + 1)) + typeInfo->start_value;
    }

    static inline bool isUnique(UI32 idType, UI32 id) {
        mo_criteria c (UidManagedObject::getClassName());
        return !c.And(UID_ATTR_ID_TYPE, idType).And(UID_ATTR_ID, id).queryExists();
    }

    UI32 UidServiceObjectManager::calculateNextId(id_type_info *typeInfo, UI32 idCount)
    {
        if (typeInfo->alloc_type == ALLOC_MONOTONIC) {
            return typeInfo->start_value + idCount;
        }

        // Random number allocator
        if (idCount == 0) {
            srand(time(NULL)); // seed only once
            return nextRand(typeInfo);
        }

        // Generate random numbers few times and see if it is unique
        for (UI32 i = typeInfo->end_value - typeInfo->start_value; i > 0; --i) {
            UI32 id = nextRand(typeInfo);
            if (isUnique(typeInfo->id_type, id)) return id;
        }

        // Search linearly for unique id
        for (UI32 id = typeInfo->start_value; id <= typeInfo->end_value; ++id) {
            if (isUnique(typeInfo->id_type, id)) return id;
        }

        return 0;   // should NEVER happen..
    }

    void UidServiceObjectManager::UidAllocHandler( UidServiceMessage *pUidServiceMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::validateUidAssign),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::doUidAlloc),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pUidServiceMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void UidServiceObjectManager::UidFreeHandler( UidServiceMessage *pUidServiceMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::doUidFree),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pUidServiceMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void UidServiceObjectManager::UidRemoveHandler( UidServiceMessage *pUidServiceMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::doUidRemove),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pUidServiceMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void UidServiceObjectManager::UidSetHandler( UidServiceMessage *pUidServiceMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::validateUidAssign),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::doUidSet),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pUidServiceMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void UidServiceObjectManager::UidQueryHandler (UidServiceQueryMessage *msg)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::doUidQuery),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&UidServiceObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *seqContext = new DceSynchronousLinearSeqContext (msg, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        seqContext->execute();
    }



    ResourceId UidServiceObjectManager::validateUidAssign(DceSynchronousLinearSeqContext *seqContext)
    {
        UidServiceMessage *msg = dynamic_cast<UidServiceMessage*>(seqContext->getPPrismMessage());

        if (msg == NULL) {
            tracePrintf(TRACE_LEVEL_ERROR, "UidServiceObjectManager::validateUidAssign : NULL message!!");
            return WRC_UIDSVC_INVALID_ARGS_ERROR;
        }

        if (!msg->isEntityNamePresent()) {
            tracePrintf(TRACE_LEVEL_ERROR, "UidServiceObjectManager::validateUidAssign : missing entityName");
            return WRC_UIDSVC_INVALID_ARGS_ERROR;
        }

        if (!msg->isIdTypePresent()) {
            tracePrintf(TRACE_LEVEL_ERROR, "UidServiceObjectManager::validateUidAssign : missing idType");
            return WRC_UIDSVC_INVALID_ARGS_ERROR;
        }

        // TODO consider caching the metadata in the sequencer context..
        if (findMetadata(msg->getIdType()) == NULL) {
            tracePrintf(TRACE_LEVEL_ERROR, "UidServiceObjectManager::validateUidAssign : Unknown idType %u", msg->getIdType());
            return WRC_UIDSVC_INVALID_ARGS_ERROR;
        }

        if (msg->getOperationCode() == UID_SET && !msg->isIdPresent()) {
            tracePrintf(TRACE_LEVEL_ERROR, "UidServiceObjectManager::validateUidAssign : missing id for UID_SET operation");
            return WRC_UIDSVC_INVALID_ARGS_ERROR;
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId UidServiceObjectManager::doUidAlloc(DceSynchronousLinearSeqContext *seqContext)
    {
        UidServiceMessage *msg = dynamic_cast<UidServiceMessage*>(seqContext->getPPrismMessage());
        const string &entityName = msg->getEntityName();
        const UI32 idType = msg->getIdType();

        tracePrintf(TRACE_LEVEL_DEBUG, true, false,
                "UidServiceObjectManager::doUidAlloc : Allocating uid for idType=%u, entity=%s",
                idType, entityName.c_str());

        mo_iterator<UidManagedObject> uidIter(seqContext);
        uidIter.criteria().And(UID_ATTR_ENTITY_NAME, entityName).And(UID_ATTR_ID_TYPE, idType);

        if (uidIter.hasNext()) {
            UidManagedObject *uidMo = uidIter.next();
            UI32 uid = uidMo->getId();

            if (uidMo->getActive()) {
                tracePrintf(TRACE_LEVEL_DEBUG, "UidServiceObjectManager::doUidAlloc : Found active entry %u", uid);
            } else {
                tracePrintf(TRACE_LEVEL_DEBUG, "UidServiceObjectManager::doUidAlloc : Found inactive entry %u; marking entry as active...", uid);
                uidMo->setActive(true);
                uidMo->addAttributeToBeUpdated(UID_ATTR_ACTIVE);
                updateWaveManagedObject(uidMo);
            }

            msg->setId(uid);
            return WAVE_MESSAGE_SUCCESS;
        }

        id_type_info *typeInfo = findMetadata(idType);
        mo_iterator<UidManagedObject> idTypeIter (seqContext);
        UI32 idCount = idTypeIter.criteria().And(UID_ATTR_ID_TYPE, idType).queryCount();

        if (idCount < typeInfo->max_count) {
            UI32 uid = calculateNextId (typeInfo, idCount);
            tracePrintf(TRACE_LEVEL_DEBUG, "UidServiceObjectManager::doUidAlloc : Allocated new id %u", uid);

            UidManagedObject *uidMo = new UidManagedObject(this);
            uidMo->setIdType(idType);
            uidMo->setId(uid);
            uidMo->setEntityName(entityName);
            uidMo->setActive(true);

            seqContext->addManagedObjectForGarbageCollection(uidMo);
            msg->setId(uid);
            return WAVE_MESSAGE_SUCCESS;
        }

        idTypeIter.criteria().And(UID_ATTR_ACTIVE, false).orderBy("lastModifiedTimeStamp").limit(1);
        if (idTypeIter.hasNext()) {
            UidManagedObject *uidMo = uidIter.next();

            tracePrintf(TRACE_LEVEL_DEBUG, true, false,
                    "UidServiceObjectManager::doUidAlloc : Re-allocating id %u, previously allocated to %s",
                    uidMo->getId(), uidMo->getEntityName().c_str());

            uidMo->setEntityName(entityName);
            uidMo->setActive(true);

            updateWaveManagedObject(uidMo);
            msg->setId(uidMo->getId());
            return WAVE_MESSAGE_SUCCESS;
        }

        tracePrintf(TRACE_LEVEL_ERROR, "UidServiceObjectManager::doUidAlloc : All the IDs in use; cannot allocate an ID for %s", entityName.c_str());
        return WRC_UIDSVC_ID_EXHAUSTED_ERROR;
    }

    static inline void addEntityNameCriteria (mo_criteria &criteria, const string &name)
    {
        if (name.find('*') != string::npos) {
            string x = name;
            std::replace(x.begin(), x.end(), '*', '%');
            criteria.Or(UID_ATTR_ENTITY_NAME, x, WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
        }
        else if (name.find('%') != string::npos) {
            criteria.Or(UID_ATTR_ENTITY_NAME, name, WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
        }
        else {
            criteria.Or(UID_ATTR_ENTITY_NAME, name);
        }
    }


    ResourceId UidServiceObjectManager::doUidFree(DceSynchronousLinearSeqContext *seqContext)
    {
        UidServiceMessage *msg = dynamic_cast<UidServiceMessage*>(seqContext->getPPrismMessage());

        // TODO use bulk update when it is available..
        mo_iterator<UidManagedObject> uidIter (seqContext);
        mo_criteria &c = uidIter.criteria();
        if (msg->isEntityNamePresent()) {
            //c.And(UID_ATTR_ENTITY_NAME, msg->getEntityName());
            addEntityNameCriteria(c, msg->getEntityName());
        }
        else if (msg->getOperationCode() == UID_FREE_BULK) { // needs redesign
            UidServiceQueryMessage *bulkMsg = dynamic_cast<UidServiceQueryMessage*>(msg);
            for (UI32 i = 0; i < bulkMsg->getCount(); ++i) {
                addEntityNameCriteria(c, bulkMsg->getEntityNameAt(i));
            }
        }

        if (msg->isIdTypePresent()) {
            c.And(UID_ATTR_ID_TYPE, msg->getIdType());
        }
        if (msg->isIdPresent()) {
            c.And(UID_ATTR_ID, msg->getId());
        }

        c.And(UID_ATTR_ACTIVE, true);

        while (uidIter.hasNext()) {
            UidManagedObject *uidMo = uidIter.next();
            tracePrintf(TRACE_LEVEL_DEBUG, "UidServiceObjectManager::doUidFree : Free entry <idType=%u, id=%u, entityName=%s>",
                    uidMo->getIdType(), uidMo->getId(), uidMo->getEntityName().c_str());

            uidMo->setActive(false);
            uidMo->addAttributeToBeUpdated(UID_ATTR_ACTIVE);
            updateWaveManagedObject(uidMo);
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId UidServiceObjectManager::doUidRemove(DceSynchronousLinearSeqContext *seqContext)
    {
        UidServiceMessage *msg = dynamic_cast<UidServiceMessage*>(seqContext->getPPrismMessage());
        string filter;

        WaveManagedObjectSynchronousQueryContextForDeletion qc (UidManagedObject::getClassName());
        if (msg->isEntityNamePresent()) {
            qc.addAndAttribute(new AttributeString(msg->getEntityName(), UID_ATTR_ENTITY_NAME));
            filter += (" idType=" + msg->getIdType());
        }
        if (msg->isIdTypePresent()) {
            qc.addAndAttribute(new AttributeUI32(msg->getIdType(), UID_ATTR_ID_TYPE));
            filter += (" entityName=" + msg->getEntityName());
        }

        if (filter.empty()) {
            tracePrintf(TRACE_LEVEL_DEBUG, "UidServiceObjectManager::doUidClear : Deleting all entries..");
        } else {
            tracePrintf(TRACE_LEVEL_DEBUG, "UidServiceObjectManager::doUidClear : Deleting entries <%s >", filter.c_str());
        }

        deleteWaveManagedObject(&qc);

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId UidServiceObjectManager::doUidSet(DceSynchronousLinearSeqContext *seqContext)
    {
        UidServiceMessage *msg = dynamic_cast<UidServiceMessage*>(seqContext->getPPrismMessage());
        UI32 idType = msg->getIdType();
        UI32 id     = msg->getId();
        string name = msg->getEntityName();

        mo_criteria otherEntityCriteria (UidManagedObject::getClassName());
        otherEntityCriteria.And(UID_ATTR_ID_TYPE, idType).And(UID_ATTR_ID, id);
        if (otherEntityCriteria.queryExists()) {
            tracePrintf(TRACE_LEVEL_ERROR, "UidServiceObjectManager::doUidSet : ID %u:%u is already allocated", idType, id);
            return WRC_UIDSVC_DUPLICATE_ID_ERROR;
        }

        mo_iterator<UidManagedObject> uidIter (seqContext);
        uidIter.criteria().And(UID_ATTR_ID_TYPE, idType).And(UID_ATTR_ENTITY_NAME, name);

        if (uidIter.hasNext()) {
            UidManagedObject *uidMo = uidIter.next();
            tracePrintf(TRACE_LEVEL_DEBUG, "UidServiceObjectManager::doUidSet : Changing id %u as %u; idType=%u, name=%u", uidMo->getId(), id, idType, name.c_str());
            uidMo->setId(id);
            uidMo->addAttributeToBeUpdated(UID_ATTR_ID);
            updateWaveManagedObject(uidMo);
        } else {
            // FIXME creating entry like this is not safe for monotonic IDs..
            // As per today's usecase, it is okay. Need to improve id allocation logic
            tracePrintf(TRACE_LEVEL_DEBUG, "UidServiceObjectManager::doUidSet : Creating new inactive entry - id=%u, idType=%u, name=%s", id, idType, name.c_str());
            UidManagedObject *uidMo = new UidManagedObject(this, name, idType, id, false);
            seqContext->addManagedObjectForGarbageCollection(uidMo);
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId UidServiceObjectManager::doUidQuery (DceSynchronousLinearSeqContext *seqContext)
    {
        UidServiceQueryMessage *msg = dynamic_cast<UidServiceQueryMessage*>(seqContext->getPPrismMessage());

        if (msg == NULL || !msg->isIdTypePresent()) {
            tracePrintf(TRACE_LEVEL_ERROR, "UidServiceObjectManager::doUidQuery : missing idType");
            return WRC_UIDSVC_INVALID_ARGS_ERROR;
        }

        mo_iterator<UidManagedObject> uidIter;
        uidIter.criteria().And(UID_ATTR_ID_TYPE, msg->getIdType()).orderBy(UID_ATTR_ID);
        if (msg->isEntityNamePresent()) {
            //uidIter.criteria().And(UID_ATTR_ENTITY_NAME, msg->getEntityName());
            addEntityNameCriteria(uidIter.criteria(), msg->getEntityName());
        }

        while (uidIter.hasNext()) {
            UidManagedObject *mo = uidIter.next();
            msg->addIdInfo(mo->getEntityName(), mo->getId(), mo->getActive());
        }

        msg->pack();

        return WAVE_MESSAGE_SUCCESS;
    }

}
