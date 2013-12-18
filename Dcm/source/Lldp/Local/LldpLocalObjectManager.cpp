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
 *   Author : mmohan                                                     *
 **************************************************************************/

#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Lldp/Global/LldpGlobalScalarsConfManagedObject.h"
#include "Lldp/Global/CdpGlobalConfManagedObject.h"
#include "Lldp/Global/LldpGlobalProfileConfManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/LldpIntfConfLocalManagedObject.h"
#include "Lldp/Local/LldpLocalObjectManager.h"
#include "Lldp/Local/LldpLocalConfWorker.h"
#include "Lldp/Local/CdpLocalConfWorker.h"
#include "Lldp/Local/LldpTypes.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/Lldp/LldpClientScalarsConfSetMessage.h"
#include "ClientInterface/Lldp/LldpClientProfileConfSetMessage.h"
#include "ClientInterface/Lldp/NsmLldpClientIntfConfSetMessage.h"
#include "ClientInterface/Lldp/CdpClientConfSetMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"

namespace DcmNs
{

    LldpLocalObjectManager::LldpLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pLldpLocalConfWorker = new LldpLocalConfWorker (this);
        prismAssert (NULL != m_pLldpLocalConfWorker, __FILE__, __LINE__);
        
        m_pCdpLocalConfWorker = new CdpLocalConfWorker (this);
        prismAssert (NULL != m_pCdpLocalConfWorker, __FILE__, __LINE__);
    }

    LldpLocalObjectManager::~LldpLocalObjectManager ()
    {
    }

    LldpLocalObjectManager  *LldpLocalObjectManager::getInstance()
    {
        static LldpLocalObjectManager *pLldpLocalObjectManager = new LldpLocalObjectManager ();

        WaveNs::prismAssert (NULL != pLldpLocalObjectManager, __FILE__, __LINE__);

        return (pLldpLocalObjectManager);
    }

    string  LldpLocalObjectManager::getClassName()
    {
        return ("Lldp");
    }

    PrismServiceId  LldpLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *LldpLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LldpLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    UI32 LldpLocalObjectManager::setGlobalConfigForReplay (LldpGlobalScalarsConfManagedObject
                                        *pLldpGlobalScalarsConfMO, LldpClientScalarsConfSetMessage *m)
    {
        UI32 cmdcode = 0x00000000;
		bool  isVcsEnabled = false;

		/* check if vcs enabled */
		if (DcmToolKit::isVcsEnabled () != WAVE_PERSISTENCE_CLUSTER_DISABLED) { 
			isVcsEnabled = true;
		}

		m->setDescription (pLldpGlobalScalarsConfMO->getDescription());
		cmdcode |= LLDP_GLOBAL_CONF_DESC;

		m->setHello (pLldpGlobalScalarsConfMO->getHello());
		cmdcode |= LLDP_GLOBAL_CONF_HELLO;

		m->setMode (pLldpGlobalScalarsConfMO->getMode());
		cmdcode |= LLDP_GLOBAL_CONF_MODE;

		m->setMultiplier (pLldpGlobalScalarsConfMO->getMultiplier());
		cmdcode |= LLDP_GLOBAL_CONF_MULTIPLIER;

		/* fcoe tlv are not advertising in standalone 
		 * hence don't replay fcoe related config present in MO.
		 */
		if(isVcsEnabled) {
			m->setDcbxFcoeAppTlv (pLldpGlobalScalarsConfMO->getDcbxFcoeAppTlv());
			cmdcode |= LLDP_GLOBAL_CONF_DCBXFCOEAPPTLV;
        }
		/* fcoe tlv are not advertising in standalone 
		 * hence don't replay fcoe related config present in MO.
		 */
		if(isVcsEnabled) {
			m->setDcbxFcoeLogicalLinkTlv (pLldpGlobalScalarsConfMO->getDcbxFcoeLogicalLinkTlv());
			cmdcode |= LLDP_GLOBAL_CONF_DCBXFCOELOGICALLINKTLV;
        }

		m->setDcbxTlv (pLldpGlobalScalarsConfMO->getDcbxTlv());
		cmdcode |= LLDP_GLOBAL_CONF_DCBXTLV;

		m->setDot1xTlv (pLldpGlobalScalarsConfMO->getDot1xTlv());
		cmdcode |= LLDP_GLOBAL_CONF_DOT1XTLV;

		m->setDot3xTlv (pLldpGlobalScalarsConfMO->getDot3xTlv());
		cmdcode |= LLDP_GLOBAL_CONF_DOT3XTLV;

		m->setMgmtAddrTlv (pLldpGlobalScalarsConfMO->getMgmtAddrTlv());
		cmdcode |= LLDP_GLOBAL_CONF_MGMTADDRTLV;

		m->setPortDescrTlv (pLldpGlobalScalarsConfMO->getPortDescrTlv());
		cmdcode |= LLDP_GLOBAL_CONF_PORTDESCRTLV;

		m->setSysCapTlv (pLldpGlobalScalarsConfMO->getSysCapTlv());
		cmdcode |= LLDP_GLOBAL_CONF_SYSCAPTLV;

		m->setSysDescrTlv (pLldpGlobalScalarsConfMO->getSysDescrTlv());
		cmdcode |= LLDP_GLOBAL_CONF_SYSDESCRTLV;

		m->setSysNameTlv (pLldpGlobalScalarsConfMO->getSysNameTlv());
		cmdcode |= LLDP_GLOBAL_CONF_SYSNAMETLV;

		m->setSysName (pLldpGlobalScalarsConfMO->getSysName());
		cmdcode |= LLDP_GLOBAL_CONF_SYSNAME;

		m->setSysDescr (pLldpGlobalScalarsConfMO->getSysDescr());
		cmdcode |= LLDP_GLOBAL_CONF_SYSDESCR;

		m->setDisable (pLldpGlobalScalarsConfMO->getDisable());
		cmdcode |= LLDP_GLOBAL_CONF_DISABLE;

		m->setDcbxIscsiAppTlv (pLldpGlobalScalarsConfMO->getDcbxIscsiAppTlv());
		cmdcode |= LLDP_GLOBAL_CONF_DCBXISCSIAPPTLV;

		m->setIscsiPriority (pLldpGlobalScalarsConfMO->getIscsiPriority());
		cmdcode |= LLDP_GLOBAL_CONF_ISCSIPRIORITY;

		if(cmdcode == 0)
			return 0;

		m->setCmdcode (cmdcode);
		return 1;

	}
				
	void LldpLocalObjectManager::ConfigReplayLldpGlobalScalars()
	{
		LldpGlobalScalarsConfManagedObject *pLldpGlobalScalarsConfMO = NULL;
		ResourceId status = WAVE_MESSAGE_ERROR;

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(LldpGlobalScalarsConfManagedObject::getClassName());
		vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);

		if (NULL != pResultsPMO) {
			UI32 numberOfResults = pResultsPMO->size ();
			trace(TRACE_LEVEL_DEVEL, string("LldpGlobalScalarsConfManagedObject: NumResults - ")+numberOfResults);
			if (numberOfResults == 1) {
    			LldpClientScalarsConfSetMessage *m = new LldpClientScalarsConfSetMessage();
				pLldpGlobalScalarsConfMO = dynamic_cast<LldpGlobalScalarsConfManagedObject *>((*pResultsPMO)[0]);
		
				status = setGlobalConfigForReplay (pLldpGlobalScalarsConfMO, m);
		
				if (status != 0) {
	        		status = sendSynchronouslyToWaveClient (LLDP_DCM_CLIENT_NAME, m);
					if (status == WAVE_MESSAGE_SUCCESS) {
						trace(TRACE_LEVEL_SUCCESS, string("LldpGlobalScalarsConfManagedObject: configured "));
					} else {
						trace(TRACE_LEVEL_ERROR, string("LldpGlobalScalarsConfManagedObject: configured"));
					}
				} else {
					trace(TRACE_LEVEL_ERROR, string("LldpGlobalScalarsConfManagedObject: default config"));
				}
				delete m;
			} else {
				trace(TRACE_LEVEL_SUCCESS, string("LldpGlobalScalarsConfManagedObject: no config present "));
			}
            //Sanjeev: Cleanup the query results.
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
		}

	}
	
	void LldpLocalObjectManager::ConfigReplayLldpGlobalProfile()
	{
		LldpGlobalProfileConfManagedObject *profileMo = NULL;
        UI32 cmdcode;
		ResourceId status = WAVE_MESSAGE_ERROR;

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(LldpGlobalProfileConfManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL == pResults)
			return;
		
		UI32 numberOfResults = pResults->size ();
		if (numberOfResults > 0) {
			trace(TRACE_LEVEL_DEVEL, string("updateNonUserProfileConfigs: NumResults - ") + numberOfResults);
			for (UI32 i=0; i<numberOfResults; i++) {
				profileMo = dynamic_cast<LldpGlobalProfileConfManagedObject *>((*pResults)[i]);
    			LldpClientProfileConfSetMessage *m = new LldpClientProfileConfSetMessage();
				
				m->setMsgType (LLDP_CREATE_CONF);
				
				m->setProfileName (profileMo->getProfileName());
				cmdcode = LLDP_GLOBAL_CONF_PROFILE;

				StringUC  descriptionUC = profileMo->getDescription();
				if (descriptionUC.getIsUserConfigured ()) {
					m->setDescription (descriptionUC.getStringValue());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_DESC;
				}
				
				UI32UC  helloUC = profileMo->getHello();
				if (helloUC.getIsUserConfigured ()) {
					m->setHello (helloUC.getUI32Value());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_HELLO;
				}
				
				EnumUC modeUC = profileMo->getMode();
				if (modeUC.getIsUserConfigured ()) {
					m->setMode (modeUC.getUI32Value());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_MODE;
				}
				
				UI32UC multiplierUC = profileMo->getMultiplier();
				if (multiplierUC.getIsUserConfigured ()) {
					m->setMultiplier (multiplierUC.getUI32Value());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_MULTIPLIER;
				}
				
				BoolUC dcbxFcoeAppTlvUC = profileMo->getDcbxFcoeAppTlv();
				if (dcbxFcoeAppTlvUC.getIsUserConfigured ()) {
					m->setDcbxFcoeAppTlv (dcbxFcoeAppTlvUC.getBoolValue());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_DCBXFCOEAPPTLV;
				}
				
				BoolUC dcbxFcoeLogicalLinkTlvUC = profileMo->getDcbxFcoeLogicalLinkTlv();
				if (dcbxFcoeLogicalLinkTlvUC.getIsUserConfigured ()) {
					m->setDcbxFcoeLogicalLinkTlv (dcbxFcoeLogicalLinkTlvUC.getBoolValue());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_DCBXFCOELOGICALLINKTLV;
				}
				
				BoolUC dcbxIscsiAppTlvUC = profileMo->getDcbxIscsiAppTlv();
				if (dcbxIscsiAppTlvUC.getIsUserConfigured ()) {
                    m->setDcbxIscsiAppTlv (dcbxIscsiAppTlvUC.getBoolValue());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_DCBXISCSIAPPTLV;
				}
				
				BoolUC dcbxTlvUC = profileMo->getDcbxTlv();
				if (dcbxTlvUC.getIsUserConfigured ()) {
					m->setDcbxTlv (dcbxTlvUC.getBoolValue());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_DCBXTLV;
				}
					
				BoolUC dot1xTlvUC = profileMo->getDot1xTlv();
				if (dot1xTlvUC.getIsUserConfigured ()) {
					m->setDot1xTlv (dot1xTlvUC.getBoolValue());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_DOT1XTLV;
				}
				
				BoolUC dot3xTlvUC = profileMo->getDot3xTlv();
				if (dot3xTlvUC.getIsUserConfigured ()) {
					m->setDot3xTlv (dot3xTlvUC.getBoolValue());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_DOT3XTLV;
				}
					
				BoolUC mgmtAddrTlvUC = profileMo->getMgmtAddrTlv();
				if (mgmtAddrTlvUC.getIsUserConfigured ()) {
					m->setMgmtAddrTlv (mgmtAddrTlvUC.getBoolValue());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_MGMTADDRTLV;
				}
				
				BoolUC portDescrTlvUC = profileMo->getPortDescrTlv();
				if (portDescrTlvUC.getIsUserConfigured ()) {
					m->setPortDescrTlv (portDescrTlvUC.getBoolValue());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_PORTDESCRTLV;
				}
				
				BoolUC sysCapTlvUC = profileMo->getSysCapTlv();
				if (sysCapTlvUC.getIsUserConfigured ()) {
					m->setSysCapTlv (sysCapTlvUC.getBoolValue());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_SYSCAPTLV;
				}
				
				BoolUC sysDescrTlvUC = profileMo->getSysDescrTlv();
				if (sysDescrTlvUC.getIsUserConfigured ()) {
					m->setSysDescrTlv (sysDescrTlvUC.getBoolValue());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_SYSDESCRTLV;
				}
				
				BoolUC sysNameTlvUC = profileMo->getSysNameTlv();
				if (sysNameTlvUC.getIsUserConfigured ()) {
					m->setSysNameTlv (sysNameTlvUC.getBoolValue());
					cmdcode |= LLDP_GLOBAL_PROFILE_CONF_SYSNAMETLV;
				}

				m->setCmdcode (cmdcode);
				status = sendSynchronouslyToWaveClient (LLDP_DCM_CLIENT_NAME, m);
				if (status == WAVE_MESSAGE_SUCCESS) {
					trace(TRACE_LEVEL_SUCCESS, string("LldpGlobalProfileConfManagedObject: configured "));
				} else {
					trace(TRACE_LEVEL_ERROR, string("LldpGlobalProfileConfManagedObject: configured"));
				}
				delete m;
			}
		} else {
			trace(TRACE_LEVEL_SUCCESS, string("LldpGlobalProfileConfManagedObject: no config present "));
		}
		
		//Sanjeev: Cleanup the query results.
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	}
	
    void LldpLocalObjectManager::ConfigReplayCdpGlobal()
	{
		CdpGlobalConfManagedObject *pCdpGlobalConfMO = NULL;
		ResourceId status = WAVE_MESSAGE_ERROR;

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CdpGlobalConfManagedObject::getClassName());
		vector<WaveManagedObject *> *pResultsPMO = querySynchronously (&syncQueryCtxt);

		if (NULL != pResultsPMO) {
			UI32 numberOfResults = pResultsPMO->size ();
			trace(TRACE_LEVEL_DEVEL, string("CdpGlobalConfManagedObject: NumResults - ")+numberOfResults);
			if (numberOfResults == 1) {
    			CdpClientConfSetMessage *m = new CdpClientConfSetMessage();
				pCdpGlobalConfMO = dynamic_cast<CdpGlobalConfManagedObject *>((*pResultsPMO)[0]);
                m->setCmdcode (CDP_GLOBAL_CONF_PROTO_STATUS);
                m->setProtoEnable (pCdpGlobalConfMO->getProtoEnable());
                
                status = sendSynchronouslyToWaveClient (LLDP_DCM_CLIENT_NAME, m);
                if (status == WAVE_MESSAGE_SUCCESS) {
                    trace(TRACE_LEVEL_SUCCESS, string("ConfigReplayCdpGlobal: configured "));
                } else {
                    trace(TRACE_LEVEL_ERROR, string("ConfigReplayCdpGlobal: Failed config"));
                }
                delete m;
			} else {
				trace(TRACE_LEVEL_SUCCESS, string("ConfigReplayCdpGlobal: no config present "));
			}
            // Cleanup the query results.
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsPMO);
		}

	}
    
	void LldpLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
	{
		string name= pWaveAsynchronousContextForPostBootPhase->getPassName();
		
		trace (TRACE_LEVEL_INFO, string("LldpLocalObjectManager::postboot Entered with pass:") + name);

		if (name == "DCM_POSTBOOT_GLOBAL_STAGE1") {
				trace (TRACE_LEVEL_DEVEL, "ConfigReplayLldpGlobalScalars:: Replaying global config");
				ConfigReplayLldpGlobalScalars();
				trace (TRACE_LEVEL_DEVEL, "ConfigReplayLldpGlobalProfile:: Replaying global profile config");
				ConfigReplayLldpGlobalProfile();
				trace (TRACE_LEVEL_DEVEL, "ConfigReplayCdpGlobal:: Replaying cdp global config");
                ConfigReplayCdpGlobal();
		}
		pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForPostBootPhase->callback ();
	}

}
