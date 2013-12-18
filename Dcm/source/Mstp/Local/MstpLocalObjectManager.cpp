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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : nsong                                                     *
 **************************************************************************/

#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Mstp/Global/XstpBridgeConfigManagedObject.h"
#include "Mstp/Global/StpBridgeConfigManagedObject.h"
#include "Mstp/Global/RstpBridgeConfigManagedObject.h"
#include "Mstp/Global/MstpBridgeConfigManagedObject.h"
#include "Mstp/Global/MstpInstanceConfigManagedObject.h"
#include "Mstp/Global/InstanceBaseManagedObject.h"
#include "Mstp/Global/VlanBridgeStpConfigManagedObject.h"
#include "Mstp/Local/MstpLocalObjectManager.h"
#include "Mstp/Local/MstpLocalWorker.h"
#include "ClientInterface/Mstp/MstpBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpVlanBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpInstanceConfigMessage.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "L2sys/Global/L2sysAgingTimeoutManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Utils/mo_iterator.h"

namespace DcmNs
{

    MstpLocalObjectManager::MstpLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pMstpLocalWorker = new MstpLocalWorker (this);
        prismAssert (NULL != m_pMstpLocalWorker, __FILE__, __LINE__);
    }

    MstpLocalObjectManager::~MstpLocalObjectManager ()
    {
    }

    MstpLocalObjectManager  *MstpLocalObjectManager::getInstance()
    {
        static MstpLocalObjectManager *pMstpLocalObjectManager = new MstpLocalObjectManager ();

        WaveNs::prismAssert (NULL != pMstpLocalObjectManager, __FILE__, __LINE__);

        return (pMstpLocalObjectManager);
    }

    string  MstpLocalObjectManager::getClassName()
    {
        return ("Mstp");
    }

    PrismServiceId  MstpLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *MstpLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *MstpLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

     void MstpLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        string name = pWaveAsynchronousContextForPostBootPhase->getPassName();
        trace (TRACE_LEVEL_INFO, string("MstpLocalObjectManager::postboot Entered with pass:") + name);

        if (name == "DCM_POSTBOOT_GLOBAL_STAGE2") {
            configReplayXstpBridgeConfig();
            configReplayMstpInstanceConfig();
            configReplayVlanBridgeConfig();
        }
        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();
    }

    void MstpLocalObjectManager::configReplayVlanBridgeConfig()
    {

        vector<WaveManagedObject*> *pMos = querySynchronously(VlanBridgeStpConfigManagedObject::getClassName());
        configReplayVlanBridgeConfig(pMos);
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pMos);
        trace (TRACE_LEVEL_DEBUG, "MstpLocalObjectManager::configReplayVlanBridgeConfig: Exit");

    }


    void MstpLocalObjectManager::configReplayVlanBridgeConfig(UI32Range &vlanRange) {
        if (vlanRange.isEmpty()) {
            return;
        }

        mo_criteria criteria(VlanBridgeStpConfigManagedObject::getClassName());
        criteria.getQueryContext()->setAttributeRangeForQuery(
                new AttributeUI32Range(vlanRange, "vlan"));

        vector<WaveManagedObject *>* pVlanBridgeMOs = criteria.query();

        configReplayVlanBridgeConfig(pVlanBridgeMOs);

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pVlanBridgeMOs);
        WaveNs::trace (TRACE_LEVEL_DEBUG, "MstpLocalObjectManager::configReplayVlanBridgeConfig(vlangRange): Exit");

    }

    void MstpLocalObjectManager::configReplayVlanBridgeConfig(vector<WaveManagedObject*> *pVlanBridgeStpConfigMOs) {
        UI32 size = pVlanBridgeStpConfigMOs == NULL ? 0 : pVlanBridgeStpConfigMOs->size();

        WaveNs::tracePrintf(TRACE_LEVEL_DEBUG, "MstpLocalObjectManager::configReplayVlanBridgeConfig: found %u VlanBridgeStpConfigManagedObject entries", size);

        ResourceId status;
        VlanBridgeStpConfigManagedObject *pVlanMo;
        for (UI32 i=0; i < size; i++) {
            MstpVlanBridgeConfigMessage *pMsg = new MstpVlanBridgeConfigMessage;
            pMsg->setCmdCode(MSTP_MSG_POST_BOOT_BR_VLAN_CONFIG);

            pVlanMo =  dynamic_cast<VlanBridgeStpConfigManagedObject *>(pVlanBridgeStpConfigMOs->at(i));
            pVlanMo->getAttributeToMessage(pMsg);
            status = MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, pMsg);
            WaveNs::tracePrintf(TRACE_LEVEL_DEBUG, "MstpLocalObjectManager::configReplayVlanBridgeConfig Client status : %d ", pMsg->getClientStatus());
            if (status == WAVE_MESSAGE_SUCCESS) {
                WaveNs::trace(TRACE_LEVEL_SUCCESS, string("XstpBridgeConfigManagedObject: configured "));
            } else {
                WaveNs::trace(TRACE_LEVEL_ERROR, string("XstpBridgeConfigManagedObject: configuration failed"));
            }
            delete pMsg;
        }

    }

    void MstpLocalObjectManager::configReplayMstpInstanceConfig()
    {
	    MstpInstanceConfigManagedObject *pMstpInstMo = NULL;
	    ResourceId status = WAVE_MESSAGE_ERROR;

	    vector<WaveManagedObject *> *pMos = querySynchronously(MstpInstanceConfigManagedObject::getClassName());
	    UI32 size = pMos == NULL ? 0 : pMos->size();

	    tracePrintf(TRACE_LEVEL_DEBUG, "MstpLocalObjectManager::configReplayMstpInstanceConfig : found %u MSTI objects", size);

	    for (UI32 i = 0; i < size; i++) {
		    pMstpInstMo = dynamic_cast<MstpInstanceConfigManagedObject *>(pMos->at(i));

		    vector<UI32> instVlans = MstpToolKit::getVlanIdsForVlanObjectIds(pMstpInstMo->m_instanceVlans);

		    MstpInstanceConfigMessage *pMsg = new MstpInstanceConfigMessage;
		    pMsg->setCmdCode (MSTP_MSG_POST_BOOT_BR_INST_MSTP_CONFIG);
		    pMsg->setInstanceId(pMstpInstMo->getInstanceId());
            pMsg->setVlanidList(instVlans);
		    pMsg->setPriority(pMstpInstMo->getPriority());

		    status = sendSynchronouslyToWaveClient ("mstp", pMsg);
		    tracePrintf(TRACE_LEVEL_DEBUG, "MstpLocalObjectManager::configReplayMstpInstanceConfig Client status : %d", pMsg->getClientStatus());

		    if (status == WAVE_MESSAGE_SUCCESS) {
			    trace(TRACE_LEVEL_SUCCESS, string("XstpBridgeConfigManagedObject: configured "));
		    } else {
			    trace(TRACE_LEVEL_ERROR, string("XstpBridgeConfigManagedObject: configuration failed"));
		    }
			delete pMsg;
	    }

	    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pMos);
    }


    void MstpLocalObjectManager::configReplayXstpBridgeConfig()
    {
	    XstpBridgeConfigManagedObject *pXstpBridgeConfigMO = NULL;
	    ResourceId status = WAVE_MESSAGE_ERROR;
	    int mode = 0;
		bool isSend = false;
		UI32 aging = MSTP_DCM_DEFAULT_AGING_TIME;

		/* get bridge mac aging */
        vector<WaveManagedObject *> *pResults = querySynchronously(L2sysAgingTimeoutManagedObject::getClassName());
        if (pResults && pResults->size() > 0) {
            L2sysAgingTimeoutManagedObject *pAgingMo = dynamic_cast<L2sysAgingTimeoutManagedObject*>((*pResults)[0]);
			aging = pAgingMo->getAgingTimeout();
        }

        if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }


		MstpBridgeConfigMessage *pMsg = new MstpBridgeConfigMessage;
		if ((status = MstpToolKit::getBridgeConfigMO(pXstpBridgeConfigMO, mode)) != WAVE_MESSAGE_SUCCESS )
		{
		    tracePrintf(TRACE_LEVEL_ERROR, "MstpLocalObjectManager::configReplayXstpBridgeConfig: Could not query bridge MO!!, err=%u", status);
		}
		else if (mode != MSTP_DCM_BRIDGE_NONE) {
				tracePrintf(TRACE_LEVEL_DEBUG, "MstpLocalObjectManager::configReplayXstpBridgeConfig:  mode=%d", mode);

			    pMsg->setCmdCode (MSTP_MSG_POST_BOOT_BR_XSTP_CONFIG);
			    pMsg->setMode (mode);
				pXstpBridgeConfigMO->getAttributeToMessage(pMsg);
				pMsg->setAgingTime(aging);

				if (MstpToolKit::isVcsEnabled()) {
                    MacAddress brMac;
                    status = MstpToolKit::allocateVcsBridgeMac(brMac);

                    if (status == WAVE_MESSAGE_SUCCESS) {
                        pMsg->setVcsBridgeMac(brMac);
                    }
                    else if (pXstpBridgeConfigMO->getBridgeMacByte() != 0) {
                        UI8 brMacByte = pXstpBridgeConfigMO->getBridgeMacByte();
                        brMac = MstpToolKit::getVcsBridgeMac(brMacByte);
                        tracePrintf(TRACE_LEVEL_WARN, true, false, "MstpLocalObjectManager::configReplayXstpBridgeConfig : "
                                "Cannot allocate bridge Id; err=%u ( %s ); using fallback value %s", 
                                status, FrameworkToolKit::localizeToSourceCodeEnum(status).c_str(), brMac.toString2().c_str());
                        pMsg->setVcsBridgeMac(brMac);
                    }
                    else {
                        MstpToolKit::logBridgeMacAllocError(status, "postboot");
                        //pMsg->setStpDisable(true);
                    }
				}

                isSend = true;
		    } else {
			    trace(TRACE_LEVEL_SUCCESS, string("XstpBridgeConfigManagedObject: no config present "));
			    // pMsg->setCmdCode (MSTP_MSG_BR_NONE);
				// Do we need to send if there has no configure???
		    }

			if (isSend) {
			    status = MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, pMsg);

				if (status == WAVE_MESSAGE_SUCCESS) { 
					trace(TRACE_LEVEL_SUCCESS, string("XstpBridgeConfigManagedObject: configured "));
				} else {
					tracePrintf(TRACE_LEVEL_ERROR, "XstpBridgeConfigManagedObject: configuration failed; error=%u", status);
				}
			}

			delete pXstpBridgeConfigMO;
			delete pMsg;
		}

}
