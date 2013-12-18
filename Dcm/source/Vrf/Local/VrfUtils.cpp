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
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Types/Types.h"
#include "VrfUtils.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Vrf/Local/VrfLocalManagedObject.h"
#include "ClientInterface/Nsm/nsmVrfIntf.h"
#include "ClientInterface/Nsm/NsmClientConfigVRFMessage.h"
#include "Vrf/Local/VrfLocalManagedObject.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"
#include "ClientInterface/Rtm/RtmClientConfigMessage.h"
#include "ClientInterface/Rtm/RtmClientInterface.h"
#include "Vrf/Local/VrfAfIpv4UcastManagedObject.h"
/*#include "rtm_vrf_util.h"*/
#include "DcmResourceIds.h"

namespace DcmNs
{

    ResourceId VrfUtils::sendVrfInstance(const string &vrfName, uint32_t opCode)
{
    ResourceId returnStatus = WAVE_MESSAGE_SUCCESS;
    string func_name = "DcmToolKit::sendVrfInstance() ";
    NsmClientConfigVRFMessage *m = new NsmClientConfigVRFMessage();
    m ->setOpcode(opCode);
    m->setVrfName(vrfName);

    trace (TRACE_LEVEL_INFO, func_name +  "Message to BACKEND" +
                " VrfName: " + m->getVrfName() +
                " OP-CODE: " + m->getOpcode() +
                "");

    /* Send the message to back end daemon */
    returnStatus = sendSynchronouslyToWaveClient ("nsm", m);
    if (returnStatus == WAVE_MESSAGE_SUCCESS) {
        returnStatus = m->getCompletionStatus();
        if (returnStatus != WAVE_MESSAGE_SUCCESS) {
                trace (TRACE_LEVEL_INFO, func_name +
                   " vrfName: " + vrfName + " OpCode=" + opCode +
                   " (NSM) getCompletionStatus() FAILED" +
                   " status = " + returnStatus);
        }
        else if (m->getClientStatus()) {
                int err = m->getClientStatus();

            returnStatus = NsmUtils::NsmBackendErrorMapToDcmStatus(err);
            trace (TRACE_LEVEL_INFO, func_name +
                  " vrfName: " + vrfName + " OpCode=" + opCode +
                  " (NSM) getClientStatus() FAILED" +
                  " NSM Error = " + err + " Dcmd Status = " + returnStatus);
            }
        }
    delete m;
    return (returnStatus);
}

    ResourceId VrfUtils::backendVRFCreate(const string &vrfName)
    {
        // TODO::  The BACKEND DAEMON RESPONSIBLE FOR VRF MANAGEMENT WILL HAVE TO IMPLEMENT
        // THE FUNCTIONALITY HERE.
        //  - send message to backend daemon to create vrf.
        ResourceId returnStatus = WAVE_MESSAGE_SUCCESS;
        string func_name = "DcmToolKit::backendVRFCreate() ";

	trace (TRACE_LEVEL_INFO, func_name + "Entering ...");
    	trace (TRACE_LEVEL_INFO, func_name + " vrfName: " + vrfName);

    	returnStatus = sendVrfInstance (vrfName, NSM_MSG_VRF);
        return (returnStatus);
    }
    void VrfUtils::restoreVrfRtmConfig(VrfLocalManagedObject *pMo)
    {
        restoreRd(pMo);
        restoreVrfAfIpv4(pMo->getVrfAfIpv4(), pMo->getVrfName());
        restoreRouterId(pMo);
        restoreVrfRt(pMo->getRt());
    }
    void VrfUtils::restoreVrfAfIpv4(VrfAfIpv4UcastManagedObject *mo, const  string &vrfName)
    {
        if(NULL == mo)
            return ;

        VrfLocalAddVrfMessage  m;
        m.setVrfName(vrfName);
        m.setCmdCode(RTM_MSG_AF_IPV4_SET);
        m.setVrfIpv4Flag(true);
        m.setMaxRoutes(mo->getMaxRoutes());
        backendVRFUpdate(&m);

        //Resote RT  Config
        restoreVrfAfIpv4Rt(mo->getRt());
    }

    void VrfUtils:: restoreVrfAfIpv4Rt(vector<WaveManagedObjectPointer<VrfRouteTargetAfManagedObject> > rt)
    {
        VrfRouteTargetAfManagedObject *mo;
        VrfLocalAddVrfMessage  m;
        for(unsigned int i = 0; i < rt.size(); i++) 
        {
            mo = rt[i].operator->();
            m.setVrfName(mo->getVrfName());
            m.setCmdCode(RTM_MSG_CONFIG_VRF_AF_IPV4_MODE_RT_SET);
            m.setRt(mo->getRt());
            m.setRtType(mo->getRtType());
            m.setVrfIpv4Flag(true);
            backendVRFUpdate(&m);
        }
    }
    void VrfUtils:: restoreVrfRt(vector<WaveManagedObjectPointer<VrfRouteTargetManagedObject> > rt)
    {
        VrfRouteTargetManagedObject *mo;
        VrfLocalAddVrfMessage  m;
        for(unsigned int i = 0; i < rt.size(); i++) 
        {
            mo = rt[i].operator->();
            m.setVrfName(mo->getVrfName());
            m.setCmdCode(RTM_MSG_CONFIG_VRF_MODE_RT_SET);
            m.setRt(mo->getRt());
            m.setRtType(mo->getRtType());
            m.setVrfIpv4Flag(false);
            backendVRFUpdate(&m);
        }
    }
    void VrfUtils::restoreRd(VrfLocalManagedObject *pMo)
    {
        VrfLocalAddVrfMessage m;
        m.setVrfName(pMo->getVrfName());
        if(pMo->getRd() != "") {
            m.setCmdCode(RTM_MSG_RD_SET);
            m.setRd(pMo->getRd());
            backendVRFUpdate(&m);
        }
    }
    void VrfUtils::restoreRouterId(VrfLocalManagedObject *pMo)
    {
        VrfLocalAddVrfMessage m;
        m.setVrfName(pMo->getVrfName());
        if(pMo->getRouterId().getIsUserConfigured()) {
             m.setCmdCode(RTM_MSG_CONFIG_ROUTER_ID_SET);
             m.setRouterId(pMo->getRouterId().getIpV4AddressValue());
             backendVRFUpdate(&m);
        }
    }


    ResourceId VrfUtils::backendVRFUpdate (VrfLocalAddVrfMessage *m)
    {
        ResourceId returnStatus = WAVE_MESSAGE_SUCCESS;
        trace(TRACE_LEVEL_INFO, string("VrfUtils::backendVRFUpdate vrfName = ") + m->getVrfName());
        RtmClientConfigMessage *msg = new RtmClientConfigMessage(m);
        returnStatus = sendToBackEnd(msg, "rtm");
        return returnStatus;
    }

    /*This function deletes input message*/
    ResourceId VrfUtils::sendToBackEnd(DcmManagementInterfaceMessage *m, const char *client)
    {
        ResourceId returnStatus = WAVE_MESSAGE_ERROR;
        if(!m | !client) return WAVE_MESSAGE_ERROR;
        returnStatus = sendSynchronouslyToWaveClient(client, m);
        if (returnStatus == WAVE_MESSAGE_SUCCESS) {
            returnStatus = m->getCompletionStatus();
            if (returnStatus != WAVE_MESSAGE_SUCCESS) {
            }
            else if (m->getClientStatus()) {
                int err = m->getClientStatus();

                returnStatus = ::mapRtmBackEndErrorsToDcmdErrors(err);
            }
        }
        delete m;
        return returnStatus;
    }
    

    ResourceId VrfUtils::backendVRFDelete(const string &vrfName)
    {
        // TODO::  The BACKEND DAEMON RESPONSIBLE FOR VRF MANAGEMENT WILL HAVE TO IMPLEMENT
        // THE FUNCTIONALITY HERE.
        //  - send message to backend daemon to delete vrf.
        ResourceId returnStatus = WAVE_MESSAGE_SUCCESS;
        string func_name = "DcmToolKit::backendVRFDelete() ";

    	trace (TRACE_LEVEL_INFO, func_name + "Entering ...");
    	trace (TRACE_LEVEL_INFO, func_name + " vrfName: " + vrfName);

    	returnStatus = sendVrfInstance (vrfName, NSM_MSG_NO_VRF);
        return (returnStatus);
    }

    ResourceId VrfUtils::validateVRFBinding(const ObjectId &oid, LocationId location)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeObjectId(oid, "objectId"));
        syncQueryCtxt.addSelectField ("rd");
        syncQueryCtxt.addSelectField ("vrfAfIpv4");
        syncQueryCtxt.addSelectField ("vrfName");
        syncQueryCtxt.setLoadOneToManyRelationships(false);
        if (location == 0) {
            location = FrameworkToolKit::getThisLocationId();
        }
        syncQueryCtxt.setLocationIdFilter(location);

        vector<WaveManagedObject *> *pResultsPMO = querySynchronously(&syncQueryCtxt);
        if (NULL != pResultsPMO) {
            UI32 numberOfResults = pResultsPMO->size ();
            if (0 == numberOfResults) {
               status = WRC_VRF_RD_ADDR_FAMILY_NOT_CONFIGURED;
            } else if (1 == numberOfResults) {
                VrfLocalManagedObject *vrfLocalMO = dynamic_cast <VrfLocalManagedObject *>((*pResultsPMO)[0]);
                if ((NULL == vrfLocalMO->getVrfAfIpv4()) || (vrfLocalMO->getRd() == "")) {
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
                    return WRC_VRF_RD_ADDR_FAMILY_NOT_CONFIGURED;
                }
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
        }
        return (status);


    }

    
    ResourceId VrfUtils::getObjectIdByVrfName(const string &vrfName, ObjectId &oid, LocationId location)
    {
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeString(vrfName, "vrfName"));
        syncQueryCtxt.addSelectField ("objectId");
        if (location == 0) {
            location = FrameworkToolKit::getThisLocationId();
        }
        syncQueryCtxt.setLocationIdFilter(location);
        vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);

        if (pResults && pResults->size() == 1 ) {
            VrfLocalManagedObject *pVrfLocalManagedObject=  dynamic_cast<VrfLocalManagedObject*>((*pResults)[0]);
            oid = pVrfLocalManagedObject->getObjectId();
            pResults->clear();
            delete pResults;
            return WAVE_MESSAGE_SUCCESS;
        }
        return WAVE_MESSAGE_ERROR;
    }
    ResourceId VrfUtils::getVrfNameByObjectId(const ObjectId &oid, string &vrfName)
    {
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeObjectId(oid, "objectId"));
        syncQueryCtxt.addSelectField ("vrfName");
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() == 1 ) {
            VrfLocalManagedObject *pVrfLocalManagedObject=  dynamic_cast<VrfLocalManagedObject*>((*pResults)[0]);
            vrfName = pVrfLocalManagedObject->getVrfName();
            pResults->clear();
            delete pResults;
            return WAVE_MESSAGE_SUCCESS;
        }
        return WAVE_MESSAGE_ERROR;
    }
    bool VrfUtils::isVrfExists(const string &vrfName, LocationId location)
    {
        UI32 count = 0;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeString(vrfName, "vrfName"));
        if (location == 0) {
            location = FrameworkToolKit::getThisLocationId();
        }
        syncQueryCtxt.setLocationIdFilter(location);
        ResourceId queryStatus = querySynchronouslyForCount(&syncQueryCtxt, count);

        if (WAVE_MESSAGE_SUCCESS == queryStatus) {
            return (count > 0);
        } else {
            return false;
        }

    }
    string VrfUtils::getVrfNameByObjectId(const ObjectId objid)
    {
        string vrfName;
        WaveManagedObject   *pWaveManagedObject = NULL;

        pWaveManagedObject = WaveObjectManagerToolKit::queryManagedObject(objid);
        if (NULL != pWaveManagedObject) {
            VrfLocalManagedObject *vrfLocalMO = dynamic_cast<VrfLocalManagedObject *>(pWaveManagedObject);
            vrfName = vrfLocalMO->getVrfName();
            delete vrfLocalMO;
        }

        return (vrfName);
    }

	bool VrfUtils::isRdExists(const string &rd, LocationId location)
	{
		bool ret = false;
		UI32 count = 0;
		
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfLocalManagedObject::getClassName ());
		syncQueryCtxt.addAndAttribute (new AttributeString (rd, "rd"));
        if (location == 0) {
            location = FrameworkToolKit::getThisLocationId();
        }
		syncQueryCtxt.setLocationIdFilter(location);
		ResourceId queryStatus = querySynchronouslyForCount(&syncQueryCtxt, count);

		if (WAVE_MESSAGE_SUCCESS == queryStatus) {
			if (count > 0) {
				ret = true;
			}
		}
		return (ret);
		

	}

    ResourceId VrfUtils::getVrfObjectIdByName(string vrfName, ObjectId &objid, const bool validate, LocationId location)
    {

        ResourceId status = WAVE_MESSAGE_ERROR;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfLocalManagedObject::getClassName ());
        syncQueryCtxt.addAndAttribute (new AttributeString (&vrfName, "vrfName"));
        syncQueryCtxt.addSelectField ("rd");
        syncQueryCtxt.addSelectFieldForObjectId("objectId");
        syncQueryCtxt.setLoadOneToManyRelationships(false);
        if (location == 0) {
            location = FrameworkToolKit::getThisLocationId();
        }
        syncQueryCtxt.setLocationIdFilter(location);
        vector<WaveManagedObject *> *pResultsPMO = querySynchronously(&syncQueryCtxt);
        if (NULL != pResultsPMO) {
            UI32 numberOfResults = pResultsPMO->size ();
            if (0 == numberOfResults) {
               status = WRC_VRF_RD_ADDR_FAMILY_NOT_CONFIGURED;
            } else if (1 == numberOfResults) {
                VrfLocalManagedObject *vrfLocalMO = dynamic_cast <VrfLocalManagedObject *>((*pResultsPMO)[0]);
                if (validate == true && (vrfLocalMO->getRd() == "")) {
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
                    return WRC_VRF_RD_ADDR_FAMILY_NOT_CONFIGURED;
                }
                objid = vrfLocalMO->getObjectId();
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
        }
        return (status);
    }

    /*Caller of this function should release memory*/
    ResourceId VrfUtils::getVrfMo(string vrfName, VrfLocalManagedObject *&mo, LocationId location)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfLocalManagedObject::getClassName ());
        syncQueryCtxt.addAndAttribute (new AttributeString (&vrfName, "vrfName"));
        if (location == 0) {
            location = FrameworkToolKit::getThisLocationId();
        }
        syncQueryCtxt.setLocationIdFilter(location);
        vector<WaveManagedObject *> *pResultsPMO = querySynchronously(&syncQueryCtxt);
        if (NULL != pResultsPMO) {
            UI32 numberOfResults = pResultsPMO->size ();
            if (0 == numberOfResults) {
               // FixMe: Error code
               status = WAVE_MESSAGE_ERROR;
            } else if (1 == numberOfResults) {
                VrfLocalManagedObject *vrfLocalMO = dynamic_cast <VrfLocalManagedObject *>((*pResultsPMO)[0]);
                mo = vrfLocalMO;
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
        }
        delete pResultsPMO;
        return (status);

    }   

    ResourceId VrfUtils::getVrfAfIpv4Mo(string vrfName, VrfAfIpv4UcastManagedObject *&mo, LocationId location)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfAfIpv4UcastManagedObject::getClassName ());
        syncQueryCtxt.addAndAttribute (new AttributeString (&vrfName, "vrfName"));
        if (location == 0) {
            location = FrameworkToolKit::getThisLocationId();
        }
        syncQueryCtxt.setLocationIdFilter(location);
        vector<WaveManagedObject *> *pResultsPMO = querySynchronously(&syncQueryCtxt);
        if (NULL != pResultsPMO) {
            UI32 numberOfResults = pResultsPMO->size ();
            if (0 == numberOfResults) {
               // FixMe: Error code
               status = WAVE_MESSAGE_ERROR;
            } else if (1 == numberOfResults) {
                mo = dynamic_cast <VrfAfIpv4UcastManagedObject*>((*pResultsPMO)[0]);
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
        }
        delete pResultsPMO;
        return (status);
    }

    ResourceId VrfUtils::getVrfAfRtMo(string vrfName, RtmMessageConfigRtType rtType, string rt, VrfRouteTargetAfManagedObject *mo[], LocationId location)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfRouteTargetAfManagedObject::getClassName ());
        if (location == 0) {
            location = FrameworkToolKit::getThisLocationId();
        }
        syncQueryCtxt.setLocationIdFilter(location);
        syncQueryCtxt.addAndAttribute (new AttributeString (&vrfName, "vrfName"));
        syncQueryCtxt.addAndAttribute (new AttributeString (&rt, "vrfRouteTarget"));
        if(RTM_MSG_CONFIG_RT_TYPE_BOTH != rtType )
            syncQueryCtxt.addAndAttribute (new AttributeEnum(rtType, "vrfRtType"));
        vector<WaveManagedObject *> *pResultsPMO = querySynchronously(&syncQueryCtxt);
        if (NULL != pResultsPMO) {
            UI32 numberOfResults = pResultsPMO->size ();
            if (0 == numberOfResults) {
               // FixMe: Error code
               status = WAVE_MESSAGE_ERROR;
            } else if (1 == numberOfResults) {
                mo[0] = dynamic_cast <VrfRouteTargetAfManagedObject*>((*pResultsPMO)[0]);
                status = WAVE_MESSAGE_SUCCESS;
            } else if(2 == numberOfResults) {
                mo[0] = dynamic_cast <VrfRouteTargetAfManagedObject*>((*pResultsPMO)[0]);
                mo[1] = dynamic_cast <VrfRouteTargetAfManagedObject*>((*pResultsPMO)[1]);
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
        }
        delete pResultsPMO;
        return (status);
    }

    ResourceId VrfUtils::getVrfRtMo(string vrfName, RtmMessageConfigRtType rtType, string rt, VrfRouteTargetManagedObject *mo[], LocationId location)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfRouteTargetManagedObject::getClassName ());
        syncQueryCtxt.addAndAttribute (new AttributeString (&vrfName, "vrfName"));
        syncQueryCtxt.addAndAttribute (new AttributeString (&rt, "vrfRouteTarget"));
        if (location == 0) {
            location = FrameworkToolKit::getThisLocationId();
        }
        syncQueryCtxt.setLocationIdFilter(location);
        if(RTM_MSG_CONFIG_RT_TYPE_BOTH != rtType )
            syncQueryCtxt.addAndAttribute (new AttributeEnum(rtType, "vrfRtType"));
        vector<WaveManagedObject *> *pResultsPMO = querySynchronously(&syncQueryCtxt);
        if (NULL != pResultsPMO) {
            UI32 numberOfResults = pResultsPMO->size ();
            if (0 == numberOfResults) {
               // FixMe: Error code
               status = WAVE_MESSAGE_ERROR;
            } else if (1 == numberOfResults ) {
                trace (TRACE_LEVEL_INFO, string("VrfUtils::getVrfRtMo") +  "one result found ");
                mo[0] = dynamic_cast <VrfRouteTargetManagedObject*>((*pResultsPMO)[0]);
                status = WAVE_MESSAGE_SUCCESS;
            }else if(2 == numberOfResults) {
                trace (TRACE_LEVEL_INFO, string("VrfUtils::getVrfRtMo") +  "two result found ");
                mo[0] = dynamic_cast <VrfRouteTargetManagedObject*>((*pResultsPMO)[0]);
                mo[1] = dynamic_cast <VrfRouteTargetManagedObject*>((*pResultsPMO)[1]);
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
        }
        delete pResultsPMO;
        return (status);
    }
    ResourceId VrfUtils::getVrfAfRtMo(VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage, VrfRouteTargetAfManagedObject *mo[], LocationId location)
    {
        return getVrfAfRtMo(pVrfLocalAddVrfMessage->getVrfName(), pVrfLocalAddVrfMessage->getRtType(), pVrfLocalAddVrfMessage->getRt(), mo, location);
    }
    ResourceId VrfUtils::getVrfRtMo(VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage, VrfRouteTargetManagedObject *mo[], LocationId location)
    {
        return getVrfRtMo(pVrfLocalAddVrfMessage->getVrfName(), pVrfLocalAddVrfMessage->getRtType(), pVrfLocalAddVrfMessage->getRt(), mo, location);
    }

    ObjectId VrfUtils::getVrfMoObjIdByPartitionMoObjId(ObjectId &ownerPartitionManagedObjectId)
    {
        ObjectId vrfObjectId = ObjectId::NullObjectId;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfLocalManagedObject::getClassName ());
        syncQueryCtxt.addAndAttribute (new AttributeObjectId (ownerPartitionManagedObjectId, "ownerPartitionManagedObjectId"));
        syncQueryCtxt.addSelectFieldForObjectId("objectId");
        syncQueryCtxt.setLoadOneToManyRelationships(false);
        LocationId location = FrameworkToolKit::getThisLocationId();
        syncQueryCtxt.setLocationIdFilter(location);

        vector<WaveManagedObject *> *pResultsPMO = querySynchronously(&syncQueryCtxt);
        if (NULL != pResultsPMO) {
            UI32 numberOfResults = pResultsPMO->size ();
            if (1 == numberOfResults) {
                VrfLocalManagedObject *vrfLocalMO = dynamic_cast <VrfLocalManagedObject *>((*pResultsPMO)[0]);
                vrfObjectId = vrfLocalMO->getObjectId();
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
        }
        return (vrfObjectId);

    }

    ResourceId VrfUtils::getVrfObjectIdByNameAndValidate(string vrfName, ObjectId &objid, LocationId locId)
    {

        ResourceId status = WAVE_MESSAGE_ERROR;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfLocalManagedObject::getClassName ());
        syncQueryCtxt.addAndAttribute (new AttributeString (&vrfName, "vrfName"));
        syncQueryCtxt.addSelectField ("rd");
        syncQueryCtxt.addSelectField ("vrfAfIpv4");
        syncQueryCtxt.addSelectFieldForObjectId("objectId");
        syncQueryCtxt.setLoadOneToManyRelationships(false);
        if(locId == 0) {
            locId = FrameworkToolKit::getThisLocationId();
        }
        syncQueryCtxt.setLocationIdFilter(locId);

        vector<WaveManagedObject *> *pResultsPMO = WaveObjectManagerToolKit::querySynchronously (&syncQueryCtxt);
        if (NULL != pResultsPMO) {
            UI32 numberOfResults = pResultsPMO->size ();
            if (0 == numberOfResults) {
               status = WRC_VRF_DOES_NOT_EXIST;
            } else if (1 == numberOfResults) {
                VrfLocalManagedObject *vrfLocalMO = dynamic_cast <VrfLocalManagedObject *>((*pResultsPMO)[0]);
                if (vrfLocalMO->getRd() == "") {
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
                    return WRC_VRF_RD_NOT_CONFIGURED;
                }
                if((vrfLocalMO->getVrfAfIpv4() == NULL)) {
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
                    return WRC_VRF_ADDR_FAMILY_NOT_CONFIGURED;

                }
                objid = vrfLocalMO->getObjectId();
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
        }
        return (status);
    }
    RT_ACTION_T VrfUtils::recalculateRtType(const string &className, const string &vrfName, const string & rt,RtmMessageConfigRtType &rtType, LocationId location)
    {
        if(RTM_MSG_CONFIG_RT_TYPE_BOTH != rtType)
            return RT_PROCEED;
        if(className == "VrfRouteTargetAf") {
            VrfRouteTargetAfManagedObject *mo[2] = {NULL, NULL};
            getVrfAfRtMo(vrfName, rtType, rt, mo, location);
            if(mo[0])
            {
                if(mo[1])
                {
                    delete mo[0];
                    delete mo[1];
                    return RT_EXISTS;
                }
                if(RTM_MSG_CONFIG_RT_TYPE_EXPORT == mo[0]->getRtType())
                {
                    rtType = RTM_MSG_CONFIG_RT_TYPE_IMPORT;
                }
                else if(RTM_MSG_CONFIG_RT_TYPE_IMPORT == mo[0]->getRtType())
                {
                    rtType = RTM_MSG_CONFIG_RT_TYPE_EXPORT;
                }
                delete mo[0];
                return RT_PROCEED;
            }
            return RT_PROCEED;
        }
        else if(className == "VrfRouteTarget") {
            VrfRouteTargetManagedObject *mo[2] = { NULL, NULL};
            getVrfRtMo(vrfName, rtType, rt, mo, location);
            if(mo[0])
            {
                if(mo[1])
                {
                    delete mo[0];
                    delete mo[1];
                    return RT_EXISTS;
                }
                if(RTM_MSG_CONFIG_RT_TYPE_EXPORT == mo[0]->getRtType())
                {
                    rtType = RTM_MSG_CONFIG_RT_TYPE_IMPORT;
                }
                else if(RTM_MSG_CONFIG_RT_TYPE_IMPORT == mo[0]->getRtType())
                {
                    rtType = RTM_MSG_CONFIG_RT_TYPE_EXPORT;
                }
                delete mo[0];
                return RT_PROCEED;
            }
        }
        return RT_PROCEED;
    }

/*input to this function should owner partiotion object id */
    string VrfUtils::getVrfNameFromOwnerPartitionManagedObjectId(const ObjectId & oid) {
        string vrfName = "";
        if( oid == ObjectId::NullObjectId)
           return "";
        /*TODO Need to request framework team to Provide 
         * vector<WaveManagedObject *> *WaveManagedObject::querySynchronouslyLocalManagedObjectsForLocationId 
         * (const LocationId &locationId, WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext)
         */

        /*This is performance issue will fix once framework provides above api*/

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeObjectId(oid, "ownerPartitionManagedObjectId"));
        syncQueryCtxt.addSelectField("vrfName");
        syncQueryCtxt.addLimitForQueryResults(1);
        vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);

        if (pResults && pResults->size() >= 1 ) {
            VrfLocalManagedObject *pVrfLocalManagedObject=  dynamic_cast<VrfLocalManagedObject*>((*pResults)[0]);
            vrfName = pVrfLocalManagedObject->getVrfName();
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
        return vrfName; 
    }

    void VrfUtils::setPartitionName(PrismMessage *pPrismMessage, const string  & partitionName) {
        pPrismMessage->setPartitionName (partitionName);
        pPrismMessage->setPartitionLocationIdForPropagation (FrameworkToolKit::getThisLocationId ());
        pPrismMessage->setIsPartitionContextPropagated (true);
    }

    MultiPartitionCleanup::MultiPartitionCleanup (const string objectManagerName,vector<string> managedClass)
        : WaveLocalObjectManager  (objectManagerName),m_managedClasses(managedClass)
    {
    }


    MultiPartitionCleanup::~MultiPartitionCleanup ()
    {

    }

    void MultiPartitionCleanup::multiPartitionPartialCleanup (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup)
    {
        tracePrintf (TRACE_LEVEL_DEVEL, false, false,  "VrfUtils::multiPartitionPartialCleanup : Entering ... ");

        ObjectId ownerPartitionManagedObjectId (pWaveAsynchronousContextForMultiPartitionCleanup->getOwnerPartitionManagedObjectId ());

        startTransaction();


        string managedClassName;

        for (unsigned int i = 0; i < m_managedClasses.size(); i++)
        {
            managedClassName = m_managedClasses[i];
            WaveManagedObjectSynchronousQueryContextForDeletion *waveManagedObjectSynchronousQueryContextForDeletion = new WaveManagedObjectSynchronousQueryContextForDeletion (managedClassName);
            waveManagedObjectSynchronousQueryContextForDeletion->addAndAttribute (new AttributeObjectId (ownerPartitionManagedObjectId, "ownerPartitionManagedObjectId"));
            waveManagedObjectSynchronousQueryContextForDeletion->setLocationIdFilter (FrameworkToolKit::getThisLocationId ());

            deleteMultipleWaveManagedObjects (waveManagedObjectSynchronousQueryContextForDeletion);
        }

        ResourceId status =  commitTransaction();

        if (FRAMEWORK_SUCCESS == status)
        {
            pWaveAsynchronousContextForMultiPartitionCleanup->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        }
        else
        {
            pWaveAsynchronousContextForMultiPartitionCleanup->setCompletionStatus (status);
        }

        pWaveAsynchronousContextForMultiPartitionCleanup->callback ();
    }

}
