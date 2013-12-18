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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : justinh                                                      *
 **************************************************************************/

#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"

#include "Nsm/Local/Layer3/NsmNodePoManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfLoopbackManagedObject.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrConfigManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrManagedObject.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Utils/DceClusterUtils.h"
#include "DcmCore/DcmToolKit.h"

#define addOnlySelectedFields(sFields, syncQueryCtxt)\
    do {\
        if(sFields)\
        {\
            trace(TRACE_LEVEL_DEBUG, string(__FILE__) + __LINE__ + string("selective query"));\
            syncQueryCtxt.addSelectFields(*sFields);\
        }\
    }while(0)


namespace DcmNs
{
    PhyIntfLocalManagedObject* NsmL3Util::PhyIfNameGetPhyIfLocalMO(string ifName, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromLocal, const vector <string> *sFields)
    { 
        vector<WaveManagedObject *> *pResults;

        WaveManagedObjectSynchronousQueryContext 
            syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeString(&ifName, "ifName"));
        addOnlySelectedFields(sFields, syncQueryCtxt);

        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();

            if (1 == numberOfResults) {
                trace(TRACE_LEVEL_DEBUG, string(__FILE__) + __LINE__ +
					  string("Record found!"));

                pPhyIntfLocalManagedObject =
                    dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
            }
            else {
                trace(TRACE_LEVEL_DEBUG, string(__FILE__) + __LINE__ +
					  string("Record not found! (!= 1)"));
            }

            // user shoudld add it to GC
            // pContext->addManagedObjectsForGarbageCollection(*pResults);
            //pResults->clear ();
            delete pResults;
        }
        else {
            trace(TRACE_LEVEL_DEBUG, string(__FILE__) + __LINE__ +
				  string("Record not found! (NULL)"));
        }

        return pPhyIntfLocalManagedObject;
    }

    NsmL3ReadyManagedObject* NsmL3Util::PhyIfNameGetL3ReadyMO(string ifName, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromLocal, const vector <string> *sFields)
    {
        NsmL3ReadyManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmL3ReadyManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeString(ifName, 
            "interfaceName"));
        addOnlySelectedFields(sFields, syncQueryCtxt);

        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            return NULL;
        }
        pMo = dynamic_cast<NsmL3ReadyManagedObject*>((*pResults)[0]);

        delete pResults;
        return pMo;
    }

    NsmIpAddrConfigManagedObject* NsmL3Util::PhyIfNameGetIpAddrCfgMO(string ifName, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromLocal, const vector <string> *sFields)
    {
        NsmIpAddrConfigManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;
        vector<string> ipAddrColm;
        ipAddrColm.push_back("objectId");

        NsmL3ReadyManagedObject *pL3Mo = PhyIfNameGetL3ReadyMO(ifName, pContext, fromLocal, &ipAddrColm);  
        if (pL3Mo == NULL) {
            return NULL;
        }
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmIpAddrConfigManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeObjectId(pL3Mo->getObjectId(), 
            "ownerManagedObjectId"));
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            goto finish;
        }
        pMo = dynamic_cast<NsmIpAddrConfigManagedObject*>((*pResults)[0]);
finish:
        if(pContext)
        {
            pContext->addManagedObjectForGarbageCollection(pL3Mo);
        }
        else 
        {
            delete pL3Mo;
        }
        //Dont delete pMo as we returning it
        delete pResults;
        return pMo;
    }

    NsmIpConfigManagedObject* NsmL3Util::PhyIfNameGetIpConfigMO(string ifName, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromLocal)
    {
        vector<WaveManagedObject *> *pResults;
        NsmIpConfigManagedObject *pMo = NULL;
        vector<string> ipConfColm;
        ipConfColm.push_back("objectId");

        NsmL3ReadyManagedObject *pL3Mo = PhyIfNameGetL3ReadyMO(ifName, pContext, fromLocal, &ipConfColm);  
        if (pL3Mo == NULL) {
            return NULL;
        }
        
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmIpConfigManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeObjectId(pL3Mo->getObjectId(), 
            "ownerManagedObjectId"));
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            goto finish;
        }
        pMo = dynamic_cast<NsmIpConfigManagedObject*>((*pResults)[0]);

        // user shoudld add it to GC
        // pContext->addManagedObjectsForGarbageCollection(*pResults);
finish:
        delete pResults;

        if(pContext)
        {
            pContext->addManagedObjectForGarbageCollection(pL3Mo);
        }
        else 
        {
            delete pL3Mo;
        }
        //Dont delete pMo as we returning it

        return pMo;
    }

    bool NsmL3Util::PhyIfNameCheckL3Ready(string ifName, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromLocal)
    {
        string func_name = "NsmL3Util::PhyIfNameCheckL3Ready() ";
        UI32 count = 0;
        bool isReady = false;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmL3ReadyManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeString(ifName, 
            "interfaceName"));
        syncQueryCtxt.addAndAttribute(
                new AttributeEnum(IF_TYPE_PHY, "interfaceType"));

        ResourceId queryStatus = WaveObjectManagerToolKit::querySynchronouslyForCount(&syncQueryCtxt, count);

        if (WAVE_MESSAGE_SUCCESS == queryStatus) {
            PLUG_DBG_S(string("count = ") + count );
            return (count > 0);
        } else {
            PLUG_DBG_S(string("Query Failed : ") + FrameworkToolKit::localize(queryStatus));
            return false;
        }
        return isReady;
    }

    bool NsmL3Util::PhyIfNameCheckIpAddrReady(string ifName, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromLocal)
    {
        string func_name = "NsmL3Util::PhyIfNameCheckIpAddrReady() ";
        bool isReady = false;

        NsmIpAddrConfigManagedObject *pIpAddrCfgMo = 
            PhyIfNameGetIpAddrCfgMO(ifName, pContext, fromLocal);

        if (pIpAddrCfgMo != NULL) {
            isReady = true;
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("PhyIfName: ") + ifName + string(" is IpAddr ready!"));
            if (pContext) {
                pContext->addManagedObjectForGarbageCollection(pIpAddrCfgMo);
            }
            else delete pIpAddrCfgMo;
        }
        else {
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("PhyIfName: ") + ifName + string(" is NOT IpAddr ready!"));
        }
         
        return isReady;
    }

    NsmIpAddrManagedObject* NsmL3Util::PhyIfNameGetPrimaryIpAddrMO(string ifName, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromLocal)
    {
        NsmIpAddrManagedObject *pMo = NULL;

        NsmIpAddrConfigManagedObject *pIpAddrCfgMo = 
            PhyIfNameGetIpAddrCfgMO(ifName, pContext, fromLocal);

        if (pIpAddrCfgMo == NULL) {
            return NULL;
        }
         
        pMo = (WaveManagedObjectPointer<NsmIpAddrManagedObject> 
            (pIpAddrCfgMo->getPrimaryIpAddrMO())).operator->();


        if (pContext) {
            pContext->addManagedObjectForGarbageCollection(pIpAddrCfgMo);
        }
        else delete pIpAddrCfgMo;

    
        return pMo;
    }

    ////////////////////
    PoIntfManagedObject* NsmL3Util::PoIdGetPoIfMO(UI32 poId, const SI32 &mapId, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal, const vector <string> *sFields)
    {
        vector<WaveManagedObject *> *pResults;

        WaveManagedObjectSynchronousQueryContext 
            syncQueryCtxt(PoIntfManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId, "id"));
        addOnlySelectedFields(sFields, syncQueryCtxt);

        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        PoIntfManagedObject *pPoIntfManagedObject = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (1 == numberOfResults) {
                pPoIntfManagedObject = 
                    dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
            }

            // user shoudld add it to GC
            // pContext->addManagedObjectsForGarbageCollection(*pResults);
            // pResults->clear ();
            delete pResults;
        }

        return pPoIntfManagedObject;
    }

    NsmNodePoManagedObject* NsmL3Util::PoIdGetNodePoMO(UI32 poId, const SI32 &mapId, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal)
    {
        NsmNodePoManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;
        vector<string> poObjectId;
        poObjectId.push_back("objectId");

        PoIntfManagedObject* pPoIntfMo = PoIdGetPoIfMO(poId, mapId, pContext, fromGlobal, &poObjectId);

        if (pPoIntfMo == NULL) {
            return NULL;
        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmNodePoManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeObjectId(pPoIntfMo->getObjectId(),
            "ownerManagedObjectId"));
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        //Only add pPoIntfMo to GC
        if (pContext) {
            pContext->addManagedObjectForGarbageCollection(pPoIntfMo);
        }
        else delete pPoIntfMo;

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            return NULL;
        }
        pMo = dynamic_cast<NsmNodePoManagedObject*>((*pResults)[0]);

        // user shoudld add it to GC
        //
        // pContext->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
         
        return pMo;
    }

    NsmL3ReadyManagedObject* NsmL3Util::PoIdGetL3ReadyMO(UI32 poId,
            const SI32 &mapId, PrismSynchronousLinearSequencerContext *pContext,
            bool fromGlobal, const vector <string> *sFields) {

        NsmL3ReadyManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;
        string ifName = getString(poId);

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmL3ReadyManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeString(ifName, 
            "interfaceName"));
        syncQueryCtxt.addAndAttribute(
                new AttributeEnum(IF_TYPE_PO, "interfaceType"));
        addOnlySelectedFields(sFields, syncQueryCtxt);

        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            return NULL;
        }
        pMo = dynamic_cast<NsmL3ReadyManagedObject*>((*pResults)[0]);

        delete pResults;
        return pMo;

    }

    NsmL3ReadyManagedObject* NsmL3Util::PoMoGetL3ReadyMO(
            PoIntfManagedObject* poIntfMO,
            PrismSynchronousLinearSequencerContext *pContext) {
        NsmL3ReadyManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;

        NsmNodePoManagedObject *pNodePoMo = (WaveManagedObjectPointer<
                NsmNodePoManagedObject>(poIntfMO->getNodePo())).operator->();
        if (pNodePoMo == NULL) {
            return NULL;
        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
                NsmL3ReadyManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
                new AttributeObjectId(pNodePoMo->getObjectId(),
                        "ownerManagedObjectId"));
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        UI32 numberOfResults = pResults->size();
        if (numberOfResults == 1) {
            pMo = dynamic_cast<NsmL3ReadyManagedObject*>((*pResults)[0]);
            // user should add it to GC
            // pContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
          //  pMo->setIntfType(IF_TYPE_PO);
        } else {
            //TODO: Need to take care of cleaning up the results fetched
        }
        return pMo;
    }

    NsmIpAddrConfigManagedObject* NsmL3Util::PoIdGetIpAddrCfgMO(UI32 poId, const SI32 &mapId, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal, const vector <string> *sFields)
    {
        NsmIpAddrConfigManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;
        vector<string> ipAddrColm;
        ipAddrColm.push_back("objectId");

        NsmL3ReadyManagedObject *pL3Mo =
            PoIdGetL3ReadyMO(poId, mapId, pContext, fromGlobal, &ipAddrColm);

        if (pL3Mo == NULL) {
            return NULL;
        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmIpAddrConfigManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeObjectId(pL3Mo->getObjectId(),
            "ownerManagedObjectId"));
        addOnlySelectedFields(sFields, syncQueryCtxt);
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        if(pResults){
            //results cannt be more than one because we querying based objectid
            UI32 numberOfResults = pResults->size ();
            if (numberOfResults == 0) {
                delete pResults;
                return NULL;
            }
        }
        else {
            return NULL;
        }
        pMo = dynamic_cast<NsmIpAddrConfigManagedObject*>((*pResults)[0]);

        // user shoudld add it to GC
        if(pContext)
            pContext->addManagedObjectForGarbageCollection(pL3Mo);
        else
            delete pL3Mo;
        delete pResults;

        return pMo;
    }

    NsmIpConfigManagedObject* NsmL3Util::PoIdGetIpConfigMO(UI32 poId, 
        const SI32 &mapId, PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal, const vector <string> *sFields)
    {
        NsmIpConfigManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;
         string poIdStr = getString(poId);
         vector<string> ipConfColm;
         ipConfColm.push_back("objectId");


        NsmL3ReadyManagedObject *pL3Mo =
            PoIdGetL3ReadyMO(poId, mapId, pContext, fromGlobal, &ipConfColm);

        if (pL3Mo == NULL) {
            return NULL;
        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmIpConfigManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeObjectId(pL3Mo->getObjectId(),
            "ownerManagedObjectId"));
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        if(pResults){
            //results cannt be more than one because we querying based objectid
            UI32 numberOfResults = pResults->size ();
            if (numberOfResults == 0) {
                delete pResults;
                return NULL;
            }
        }
        else {
            return NULL;
        }
        pMo = dynamic_cast<NsmIpConfigManagedObject*>((*pResults)[0]);

        if(pContext)
            pContext->addManagedObjectForGarbageCollection(pL3Mo);
        else
            delete pL3Mo;
        delete pResults;

        return pMo;
    }

    bool NsmL3Util::PoIdCheckNodePo(UI32 poId, const SI32 &mapId,
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal)
    {
        string func_name = "NsmL3Util::PoIdCheckNodePo() ";
        bool isReady = false;

        NsmNodePoManagedObject *pNodePoMo = 
            PoIdGetNodePoMO(poId, mapId, pContext, fromGlobal);

        if (pNodePoMo != NULL) {
            isReady = true;
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("PoId: ") + poId + string(" is NodePo ready!"));
            if (pContext) {
                pContext->addManagedObjectForGarbageCollection(pNodePoMo);
            }
            else delete pNodePoMo;
        }
        else {
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("PoId: ") + poId + string(" is NOT NodePo ready!"));
        }
         
        return isReady;
    }

    bool NsmL3Util::PoIdCheckL3Ready(UI32 poId, const SI32 &mapId,
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal)
    {
        string func_name = "NsmL3Util::PoIdCheckL3Ready() ";
        bool isReady = false;

        NsmL3ReadyManagedObject *pL3ReadyMo = 
            PoIdGetL3ReadyMO(poId, mapId, pContext, fromGlobal);

        if (pL3ReadyMo != NULL) {
            isReady = true;
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("PoId: ") + poId + string(" is Layer3 ready!"));
            if (pContext) {
                pContext->addManagedObjectForGarbageCollection(pL3ReadyMo);
            }
            else delete pL3ReadyMo;
        }
        else {
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("PoId: ") + poId + string(" is NOT Layer3 ready!"));
        }
        
        return isReady;
    }

    bool NsmL3Util::PoIdCheckIpAddrReady(UI32 poId, const SI32 &mapId,
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal)
    {
        string func_name = "NsmL3Util::PoIdCheckIpAddrReady() ";
        bool isReady = false;

        NsmIpAddrConfigManagedObject *pIpAddrCfgMo = 
            PoIdGetIpAddrCfgMO(poId, mapId, pContext, fromGlobal);

        if (pIpAddrCfgMo != NULL) {
            isReady = true;
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("PoId: ") + poId + string(" is IpAddr ready!"));
            if (pContext) {
                pContext->addManagedObjectForGarbageCollection(pIpAddrCfgMo);
            }
            else delete pIpAddrCfgMo;
        }
        else {
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("PoId: ") + poId + string(" is NOT IpAddr ready!"));
        }
         
        return isReady;
    }

    NsmIpAddrManagedObject* NsmL3Util::PoIdGetPrimaryIpAddrMO(UI32 poId, const SI32 &mapId, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal)
    {
        NsmIpAddrManagedObject *pMo = NULL;

        NsmIpAddrConfigManagedObject *pIpAddrCfgMo = 
            PoIdGetIpAddrCfgMO(poId, mapId, pContext, fromGlobal);
        if (pIpAddrCfgMo == NULL) {
            return NULL;
        }

        pMo = (WaveManagedObjectPointer<NsmIpAddrManagedObject> 
            (pIpAddrCfgMo->getPrimaryIpAddrMO())).operator->();
    
        if (pContext) {
            pContext->addManagedObjectForGarbageCollection(pIpAddrCfgMo);
        }
        else delete pIpAddrCfgMo;

        return pMo;
    }

/////////

    VlanIntfManagedObject* NsmL3Util::VlanIdGetVlanIfMO(UI32 vlanId, const SI32 &mapId, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal,  const vector <string> *sFields)
    {
        vector<WaveManagedObject *> *pResults;

        WaveManagedObjectSynchronousQueryContext 
            syncQueryCtxt(VlanIntfManagedObject::getClassName());

        syncQueryCtxt.addAndAttribute(new AttributeUI32(&vlanId, "id"));
        addOnlySelectedFields(sFields, syncQueryCtxt);
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();

            if (1 == numberOfResults) {
                pVlanIntfManagedObject = 
                    dynamic_cast<VlanIntfManagedObject *>((*pResults)[0]);
            } 

            // user shoudld add it to GC
            // pContext->addManagedObjectsForGarbageCollection(*pResults);
            //pResults->clear();
            delete pResults;
        }

        return pVlanIntfManagedObject;
    }

    NsmIntfSviManagedObject* NsmL3Util::VlanIdGetIntfSviMO(UI32 vlanId, const SI32 &mapId, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal, const vector <string> *sFields)
    {
        NsmIntfSviManagedObject *pMo = NULL;
        //vector<WaveManagedObject *> *pResults;
        vector<string> vlanObjId ;
        vlanObjId.push_back("objectId");
        

        VlanIntfManagedObject* pVlanIntfMo = 
            VlanIdGetVlanIfMO(vlanId, mapId, pContext, fromGlobal, &vlanObjId);
 
        if (pVlanIntfMo == NULL) {
            return NULL;
        }

        /*********************************
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmIntfSviManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeObjectId(pVlanIntfMo->getObjectId(),
            "ownerManagedObjectId"));
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);
        *********************************/

        pMo = GetIntfSviMO(pVlanIntfMo, mapId, pContext, fromGlobal, sFields);

        //Only add pVlanIntfMo to GC
        if (pContext) {
            pContext->addManagedObjectForGarbageCollection(pVlanIntfMo);
        }
        else delete pVlanIntfMo;

        /******************************
        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            return NULL;
        }
        pMo = dynamic_cast<NsmIntfSviManagedObject*>((*pResults)[0]);

        // user shoudld add it to GC
        // pContext->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
        *****************************/

        return pMo;
    }

    NsmIntfSviManagedObject* NsmL3Util::GetIntfSviMO(VlanIntfManagedObject *pVlanIntfMo, const SI32 &mapId, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal, const vector <string> *sFields)
    {
        NsmIntfSviManagedObject *pMo = NULL;

        if (pVlanIntfMo == NULL) {
            return NULL;
        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NsmIntfSviManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeObjectId(pVlanIntfMo->getObjectId(), "vlan"));
        if (mapId > 0) {
            LocationId locId = DcmToolKit::getLocationIdFromMappedId(mapId);
            syncQueryCtxt.setLocationIdFilter(locId);
        }
        vector<WaveManagedObject *> *pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            return NULL;
        }
        pMo = dynamic_cast<NsmIntfSviManagedObject*>((*pResults)[0]);

        if (pResults) {
            delete (pResults);
        }

        return pMo;
    }

    NsmL3ReadyManagedObject* NsmL3Util::VlanIdGetL3ReadyMO(UI32 vlanId, const SI32 &mapId, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal,const vector<string> *sFields)
    {
        NsmL3ReadyManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;
        string ifName = getString(vlanId);

         WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmL3ReadyManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeString(ifName,
            "interfaceName"));
        syncQueryCtxt.addAndAttribute(
                new AttributeEnum(IF_TYPE_VL, "interfaceType"));
        if (mapId > 0) {
            LocationId locId = DcmToolKit::getLocationIdFromMappedId(mapId);
            syncQueryCtxt.setLocationIdFilter(locId);
        }
        addOnlySelectedFields(sFields, syncQueryCtxt);

        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            //TODO add mapid to l3ready MO 
            //as there can be many SVIs in 
            //case MC scenarios different 
            //rbridges refering same vlan
            return NULL;
        }
        pMo = dynamic_cast<NsmL3ReadyManagedObject*>((*pResults)[0]);

        delete pResults;
        return pMo;
    }

    NsmIpAddrConfigManagedObject* NsmL3Util::VlanIdGetIpAddrCfgMO(UI32 vlanId, const SI32 &mapId, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal)
    {
        NsmIpAddrConfigManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;
        vector<string> ipAddrColm;
        ipAddrColm.push_back("objectId");

        NsmL3ReadyManagedObject *pL3Mo = VlanIdGetL3ReadyMO(vlanId, mapId, pContext, fromGlobal, &ipAddrColm);
        if (pL3Mo == NULL) {
            return NULL;
        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmIpAddrConfigManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeObjectId(pL3Mo->getObjectId(),
            "ownerManagedObjectId"));
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            return NULL;
        }
        pMo = dynamic_cast<NsmIpAddrConfigManagedObject*>((*pResults)[0]);
        if(pContext)
            pContext->addManagedObjectForGarbageCollection(pL3Mo);
        else 
            delete pL3Mo;

        delete pResults;
 
        return pMo;
    }

    NsmIpConfigManagedObject* NsmL3Util::VlanIdGetIpConfigMO(UI32 vlanId, const SI32 &mapId, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal)
    {
        NsmIpConfigManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;
        vector<string> ipConfColm;
        ipConfColm.push_back("objectId");


        NsmL3ReadyManagedObject *pL3Mo = VlanIdGetL3ReadyMO(vlanId, mapId, pContext, fromGlobal, &ipConfColm);
        if (pL3Mo == NULL) {
            return NULL;
        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmIpConfigManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeObjectId(pL3Mo->getObjectId(),
            "ownerManagedObjectId"));
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            return NULL;
        }
        pMo = dynamic_cast<NsmIpConfigManagedObject*>((*pResults)[0]);
        if(pContext)
            pContext->addManagedObjectForGarbageCollection(pL3Mo);
        else 
            delete pL3Mo;

        delete pResults;
 
        return pMo;
    }

    bool NsmL3Util::VlanIdCheckIntfSvi(UI32 vlanId, const SI32 &mapId,
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal)
    {
        string func_name = "NsmL3Util::VlanIdCheckintfSvi() ";
        bool isReady = false;

        NsmIntfSviManagedObject *pIntfSviMo = 
            VlanIdGetIntfSviMO(vlanId, mapId, pContext, fromGlobal);

        if (pIntfSviMo != NULL) {
            isReady = true;
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("VlanId: ") + vlanId + string(" is IntfSvi ready!"));
            if (pContext) {
                pContext->addManagedObjectForGarbageCollection(pIntfSviMo);
            }
            else delete pIntfSviMo;
        }
        else {
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("vlanId: ") + vlanId + string(" is NOT IntfSvi ready!"));
        }
         
        return isReady;
    }

    bool NsmL3Util::VlanIdCheckL3Ready(UI32 vlanId, const SI32 &mapId,
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal)
    {
        string func_name = "NsmL3Util::VlanIdCheckL3Ready() ";
        bool isReady = false;

        NsmL3ReadyManagedObject *pL3ReadyMo = 
            VlanIdGetL3ReadyMO(vlanId, mapId, pContext, fromGlobal);

        if (pL3ReadyMo != NULL) {
            isReady = true;
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("VlanId: ") + vlanId + string(" is Layer3 ready!"));
            if (pContext) {
                pContext->addManagedObjectForGarbageCollection(pL3ReadyMo);
            }
            else delete pL3ReadyMo;
        }
        else {
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("vlanId: ") + vlanId + string(" is NOT Layer3 ready!"));
        }
         
        return isReady;
    }

    bool NsmL3Util::VlanIdCheckIpAddrReady(UI32 vlanId, const SI32 &mapId,
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal)
    {
        string func_name = "NsmL3Util::VlanIdCheckIpAddrReady() ";
        bool isReady = false;

        NsmIpAddrConfigManagedObject *pIpAddrCfgMo = 
            VlanIdGetIpAddrCfgMO(vlanId, mapId, pContext, fromGlobal);

        if (pIpAddrCfgMo != NULL) {
            isReady = true;
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("VlanId: ") + vlanId + string(" is IpAddr ready!"));
            if (pContext) {
                pContext->addManagedObjectForGarbageCollection(pIpAddrCfgMo);
            }
            else delete pIpAddrCfgMo;
        }
        else {
            trace(TRACE_LEVEL_INFO, string(__FILE__) + __LINE__ + func_name +
				  string("vlanId: ") + vlanId + string(" is NOT IpAddr ready!"));
        }
         
        return isReady;
    }

    NsmIpAddrManagedObject* NsmL3Util::VlanIdGetPrimaryIpAddrMO(UI32 vlanId, const SI32 &mapId, 
        PrismSynchronousLinearSequencerContext  *pContext, 
        bool fromGlobal)
    {
        NsmIpAddrManagedObject *pMo = NULL;

        NsmIpAddrConfigManagedObject *pIpAddrCfgMo = 
            VlanIdGetIpAddrCfgMO(vlanId, mapId, pContext, fromGlobal);
 
        if (pIpAddrCfgMo == NULL) {
            return NULL;
        }

        pMo = (WaveManagedObjectPointer<NsmIpAddrManagedObject> 
            (pIpAddrCfgMo->getPrimaryIpAddrMO())).operator->();
    
        if (pContext) {
            pContext->addManagedObjectForGarbageCollection(pIpAddrCfgMo);
        }
        else delete pIpAddrCfgMo;
         
        return pMo;
    }

#define copyVectorOfPointers(mos, pResults, type)\
    do {\
        UI32 numberOfResults = pResults->size();\
        for (UI32 i = 0; i < numberOfResults; ++i) {\
            mos.push_back(dynamic_cast<#type*> ((*pResults)[i]));\
        }\
        delete pResults;\
    }while(0)


/*caller need to release memory mos */
    void NsmL3Util::NodePoIfGetMOs(vector<NsmNodePoManagedObject*>& mos,
        const SI32 mapId)
    {
        vector<WaveManagedObject *> *pResults;

        WaveManagedObjectSynchronousQueryContext 
            syncQueryCtxt(NsmNodePoManagedObject::getClassName());
        //syncQueryCtxt.addAndAttribute (new AttributeUI32(&mapId, "mapId"));
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);
        UI32 numberOfResults = pResults->size();
        for (UI32 i = 0; i < numberOfResults; ++i) {
            mos.push_back(dynamic_cast<NsmNodePoManagedObject*> ((*pResults)[i]));
        }
        delete pResults;

        return;
    }

/*caller need to release memory mos */
    void NsmL3Util::NodeVlanIfGetMOs(vector<NsmIntfSviManagedObject*>& mos,
        const LocationId &locId)
    {
        vector<WaveManagedObject *> *pResults;

        WaveManagedObjectSynchronousQueryContext 
            syncQueryCtxt(NsmIntfSviManagedObject::getClassName());
        if (locId <= 0) {
            syncQueryCtxt.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
        } else {
            syncQueryCtxt.setLocationIdFilter(locId);
        }
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);
        UI32 numberOfResults = pResults->size();
        for (UI32 i = 0; i < numberOfResults; ++i) {
            mos.push_back(dynamic_cast<NsmIntfSviManagedObject*> ((*pResults)[i]));
        }
        delete pResults;

        return;
    }

/*caller need to release memory mos */
    void NsmL3Util::GetIntfLoopbackMOs ( 
        vector<NsmIntfLoopbackManagedObject*>& mos,
        const LocationId &locId)
    {
        vector<WaveManagedObject *> *pResults;

        WaveManagedObjectSynchronousQueryContext
            syncQueryCtxt(NsmIntfLoopbackManagedObject::getClassName());
        if (locId <= 0) {
            syncQueryCtxt.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
        } else {
            syncQueryCtxt.setLocationIdFilter(locId);
        }

		/*
		 * Quering the MOs in ascending order of interface name. This is to replay the
		 * interface configuration in correct order.
 		 */
		syncQueryCtxt.addOrderField("ifName", true);
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);
        UI32 numberOfResults = pResults->size();
        for (UI32 i = 0; i < numberOfResults; ++i) {
            mos.push_back(dynamic_cast<NsmIntfLoopbackManagedObject*> ((*pResults)[i]));
        }
        delete pResults;
        return;
    }

    NsmL3ReadyManagedObject* NsmL3Util::PhyIfGetL3ReadyMO(PhyIntfLocalManagedObject *pPhyIntfMo,
            PrismSynchronousLinearSequencerContext  *pContext,
            bool fromLocal)
        {
        trace(TRACE_LEVEL_DEBUG, string(__FILE__) + __LINE__ +
			  string("PhyIfGetL3ReadyMO : Entering: "));
            NsmL3ReadyManagedObject *pMo = NULL;
            vector<WaveManagedObject *> *pResults;
            InterfaceType intf_type;;

            if (pPhyIntfMo == NULL) {
                return NULL;
            }
            intf_type = pPhyIntfMo->getPhyType();
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
                NsmL3ReadyManagedObject::getClassName());
            syncQueryCtxt.addAndAttribute(
                new AttributeObjectId(pPhyIntfMo->getObjectId(),
                "ownerManagedObjectId"));
            pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);
            /*
            //Only add pPhyIntfMo to GC
            if (pContext) {
                pContext->addManagedObjectForGarbageCollection(pPhyIntfMo);
            }
            else delete pPhyIntfMo;
            */

            if(!pResults) {
            	return NULL;
            }
            UI32 numberOfResults = pResults->size ();
            if (numberOfResults == 0 || numberOfResults > 1) {
                return NULL;
            }
            pMo = dynamic_cast<NsmL3ReadyManagedObject*>((*pResults)[0]);

            // user shoudld add it to GC
            // pContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
            pMo->setIntfType(intf_type);
            return pMo;
        }

  NsmL3ReadyManagedObject* NsmL3Util::LoopbackIdGetL3ReadyMO(UI32 LoopbackId, const SI32 &mapId,
        PrismSynchronousLinearSequencerContext  *pContext)
    {
        NsmL3ReadyManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;
	string ifName = "lo";
        ifName = ifName +  getString(LoopbackId);

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmL3ReadyManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeString(ifName,
            "interfaceName"));
        syncQueryCtxt.addAndAttribute(
                new AttributeEnum(IF_TYPE_INTF_LOOPBACK, "interfaceType"));
        if (mapId > 0) {
            LocationId locId = DcmToolKit::getLocationIdFromMappedId(mapId);
            syncQueryCtxt.setLocationIdFilter(locId);
        }

        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            return NULL;
        }
        pMo = dynamic_cast<NsmL3ReadyManagedObject*>((*pResults)[0]);

        delete pResults;
        return pMo;
    }

    NsmIntfLoopbackManagedObject* NsmL3Util::loIdGetLoopbackIfMO(UI32 loId, const SI32 &mapId, const vector <string> *sFields)
    {
        vector<WaveManagedObject *> *pResults;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NsmIntfLoopbackManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeUI32(&loId, "ifId"));
        addOnlySelectedFields(sFields, syncQueryCtxt);
        if (mapId > 0) {
            LocationId locId = DcmToolKit::getLocationIdFromMappedId(mapId);
            syncQueryCtxt.setLocationIdFilter(locId);
        } else {
            syncQueryCtxt.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
		}
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        NsmIntfLoopbackManagedObject *pMO = NULL;
        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size ();
            if (1 == numberOfResults)
            {
                pMO = dynamic_cast<NsmIntfLoopbackManagedObject *>((*pResults)[0]);
            } 
            // user shoudld add it to GC
            delete pResults;
        }
        return pMO;
    }
} // DcmNs

