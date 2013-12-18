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
 *   Author : kverma, bgangise, asharma                                    *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForUpgradePhase.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Types/BitMap.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalConfWorker.h"
#include "APPM/Global/APPMGlobalAPPMProfileMessage.h"
#include "APPM/Global/APPMGlobalProfileDomainMessage.h"
#include "APPM/Global/APPMGlobalAPPMProfileBulkMessage.h"
#include "APPM/Global/APPMGlobalAPPMVlanMessage.h"
#include "APPM/Global/APPMGlobalAPPMQosMessage.h"
#include "APPM/Global/APPMGlobalAPPMFcoeMessage.h"
#include "APPM/Global/APPMGlobalAPPMSecurityMessage.h"
#include "APPM/Global/APPMNativeVlanManagedObject.h"
#include "APPM/Local/APPMLocalAPPMProfileMessage.h"
#include "APPM/Local/APPMLocalProfileDomainMessage.h"
#include "APPM/Local/APPMLocalAPPMProfileBulkMessage.h"
#include "APPM/Local/APPMLocalAPPMVlanMessage.h"
#include "APPM/Local/APPMLocalAPPMQosMessage.h"
#include "APPM/Local/APPMLocalAPPMFcoeMessage.h"
#include "APPM/Local/APPMLocalAPPMSecurityMessage.h"
#include "APPM/Global/AppmEnableManagedObject.h"
#include "APPM/Global/SubProfileManagedObject.h"
#include "Qos/Global/CeeMapManagedObject.h"
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "APPM/Global/ProfileManagedObject.h"
#include "APPM/Global/ProfileDomainManagedObject.h"
#include "APPM/Global/ProfileDomainCompositeManagedObject.h"
#include "APPM/Global/VlanProfileCfgManagedObject.h"
#include "APPM/Global/ProfileMacManagedObject.h"
#include "APPM/Global/APPMVlanClassifierManagedObject.h"
#include "APPM/Global/APPMVlanManagedObject.h"
#include "APPM/Global/FcoeProfileCfgManagedObject.h"
#include "APPM/Global/PfcRxTxMapManagedObject.h"
#include "APPM/Global/QosProfileCfgManagedObject.h"
#include "APPM/Global/SecProfileCfgManagedObject.h"
#include "APPM/Utils/APPMUtils.h"
#include "Utils/ProvisioningUtil.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Global/QosGlobalCreateCosToCosMutationMapMessage.h"
#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "Qos/Global/QosGlobalCreateCosToTrafficClassMapMessage.h"
#include "Qos/Global/CeeMapManagedObject.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "DcmCStatus.h"
#include "Qos/Global/PriorityMapManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Common/QosUtils.h"
#include "ClientInterface/Ssm/qoscommon.h"

#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Utils/DceClusterUtils.h"
#include <raslog/raslog.h>
#include <raslog/raslog_nsm.h>
#include "APPM/Global/VcenterGetAllPortProfileListMessage.h"
#include "Framework/Attributes/AttributeSqlIn.h"
#include <algorithm>

namespace DcmNs
{
	BitMap g_pProfileId(MAX_NO_PROFILES) ;

    APPMGlobalConfWorker::APPMGlobalConfWorker ( APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : WaveWorker  (pAPPMGlobalObjectManager)
    {
        AppmEnableManagedObject    AppmEnableManagedObject    (pAPPMGlobalObjectManager);
        SubProfileManagedObject    SubProfileManagedObject    (pAPPMGlobalObjectManager);
        ProfileManagedObject    ProfileManagedObject    (pAPPMGlobalObjectManager);
        ProfileDomainManagedObject ProfileDomainManagedObject (pAPPMGlobalObjectManager);
        ProfileDomainCompositeManagedObject ProfileDomainCompositeManagedObject (pAPPMGlobalObjectManager);
        VlanProfileCfgManagedObject    VlanProfileCfgManagedObject    (pAPPMGlobalObjectManager);
        ProfileMacManagedObject    ProfileMacManagedObject    (pAPPMGlobalObjectManager);
        APPMVlanClassifierManagedObject    APPMVlanClassifierManagedObject    (pAPPMGlobalObjectManager);
        APPMVlanManagedObject    APPMVlanManagedObject    (pAPPMGlobalObjectManager);
        FcoeProfileCfgManagedObject    FcoeProfileCfgManagedObject    (pAPPMGlobalObjectManager);
        PfcRxTxMapManagedObject    PfcRxTxMapManagedObject    (pAPPMGlobalObjectManager);
        QosProfileCfgManagedObject    QosProfileCfgManagedObject    (pAPPMGlobalObjectManager);
        SecProfileCfgManagedObject    SecProfileCfgManagedObject    (pAPPMGlobalObjectManager);
		APPMNativeVlanManagedObject    APPMNativeVlanManagedObject    (pAPPMGlobalObjectManager);
        AppmEnableManagedObject.setupOrm ();
        addManagedClass (AppmEnableManagedObject::getClassName (), this);
        SubProfileManagedObject.setupOrm ();
        addManagedClass (SubProfileManagedObject::getClassName (), this);
        ProfileManagedObject.setupOrm ();
        addManagedClass (ProfileManagedObject::getClassName (), this);
        ProfileDomainManagedObject.setupOrm ();
        addManagedClass (ProfileDomainManagedObject::getClassName (), this);
        ProfileDomainCompositeManagedObject.setupOrm ();
        addManagedClass (ProfileDomainCompositeManagedObject::getClassName (), this);
        VlanProfileCfgManagedObject.setupOrm ();
        addManagedClass (VlanProfileCfgManagedObject::getClassName (), this);
        ProfileMacManagedObject.setupOrm ();
        addManagedClass (ProfileMacManagedObject::getClassName (), this);
        APPMVlanClassifierManagedObject.setupOrm ();
        addManagedClass (APPMVlanClassifierManagedObject::getClassName (), this);
        APPMVlanManagedObject.setupOrm ();
        addManagedClass (APPMVlanManagedObject::getClassName (), this);
        FcoeProfileCfgManagedObject.setupOrm ();
        addManagedClass (FcoeProfileCfgManagedObject::getClassName (), this);
        PfcRxTxMapManagedObject.setupOrm ();
        addManagedClass (PfcRxTxMapManagedObject::getClassName (), this);
        QosProfileCfgManagedObject.setupOrm ();
        addManagedClass (QosProfileCfgManagedObject::getClassName (), this);
        SecProfileCfgManagedObject.setupOrm ();
        addManagedClass (SecProfileCfgManagedObject::getClassName (), this);
		APPMNativeVlanManagedObject.setupOrm ();
        addManagedClass (APPMNativeVlanManagedObject::getClassName (), this);

        APPMVlanClassificationMO APPMVlanClassificationMO (pAPPMGlobalObjectManager);
        APPMVlanClassificationMO.setupOrm ();
        addManagedClass (APPMVlanClassificationMO::getClassName (), this);

        APPMCtagVlanClassificationMO APPMCtagVlanClassificationMO (pAPPMGlobalObjectManager);
        APPMCtagVlanClassificationMO.setupOrm ();
        addManagedClass (APPMCtagVlanClassificationMO::getClassName (), this);

        APPMCtagVlanClassificationRemoveMO APPMCtagVlanClassificationRemoveMO (pAPPMGlobalObjectManager);
        APPMCtagVlanClassificationRemoveMO.setupOrm ();
        addManagedClass (APPMCtagVlanClassificationRemoveMO::getClassName (), this);

        APPMMacAddressVlanClassificationMO APPMMacAddressVlanClassificationMO (pAPPMGlobalObjectManager);
        APPMMacAddressVlanClassificationMO.setupOrm ();
        addManagedClass (APPMMacAddressVlanClassificationMO::getClassName (), this);

        APPMMacGroupVlanClassificationMO APPMMacGroupVlanClassificationMO (pAPPMGlobalObjectManager);
        APPMMacGroupVlanClassificationMO.setupOrm ();
        addManagedClass (APPMMacGroupVlanClassificationMO::getClassName (), this);

        addOperationMap (APPMGLOBALAPPMPROFILE, reinterpret_cast<PrismMessageHandler> (&APPMGlobalConfWorker::APPMGlobalAPPMProfileMessageHandler));
        addOperationMap (APPMGLOBALPORTPROFILEDOMAIN, reinterpret_cast<PrismMessageHandler> (&APPMGlobalConfWorker::APPMGlobalPortProfileDomainMessageHandler));
        addOperationMap (APPMGLOBALAPPMVLAN, reinterpret_cast<PrismMessageHandler> (&APPMGlobalConfWorker::APPMGlobalAPPMVlanMessageHandler));
        addOperationMap (APPMGLOBALAPPMQOS, reinterpret_cast<PrismMessageHandler> (&APPMGlobalConfWorker::APPMGlobalAPPMQosMessageHandler));
        addOperationMap (APPMGLOBALAPPMFCOE, reinterpret_cast<PrismMessageHandler> (&APPMGlobalConfWorker::APPMGlobalAPPMFcoeMessageHandler));
        addOperationMap (APPMGLOBALAPPMSECURITY, reinterpret_cast<PrismMessageHandler> (&APPMGlobalConfWorker::APPMGlobalAPPMSecurityMessageHandler));
		addOperationMap (APPMGLOBALAPPMPROFILEBULK, reinterpret_cast<PrismMessageHandler>  (&APPMGlobalConfWorker::APPMGlobalAPPMProfileBulkMessageHandler));
		addOperationMap (VCENTERGETPORTPROFILE, reinterpret_cast<PrismMessageHandler> (&APPMGlobalConfWorker::PortProfileObjectMessageHandler));
		addOperationMap (VCENTERGETALLPORTPROFILES,reinterpret_cast<PrismMessageHandler> (&APPMGlobalConfWorker::VcenterGetAllPortProfileListMessageHandler));
        g_pProfileId.reset();
    }
	
    APPMGlobalConfWorker::~APPMGlobalConfWorker ()
    {
    }

    PrismMessage  *APPMGlobalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case APPMGLOBALAPPMPROFILE :
                pPrismMessage = new APPMGlobalAPPMProfileMessage ();
                break;
            case APPMGLOBALPORTPROFILEDOMAIN :
                pPrismMessage = new APPMGlobalProfileDomainMessage ();
                break;
            case APPMGLOBALAPPMVLAN :
                pPrismMessage = new APPMGlobalAPPMVlanMessage ();
                break;
            case APPMGLOBALAPPMQOS :
                pPrismMessage = new APPMGlobalAPPMQosMessage ();
                break;
            case APPMGLOBALAPPMFCOE :
                pPrismMessage = new APPMGlobalAPPMFcoeMessage ();
                break;
            case APPMGLOBALAPPMSECURITY :
                pPrismMessage = new APPMGlobalAPPMSecurityMessage ();
                break;
            case APPMGLOBALAPPMPROFILEBULK :
                pPrismMessage = new APPMGlobalAPPMProfileBulkMessage ();
                break;
            case VCENTERGETPORTPROFILE:
                pPrismMessage = new PortProfileObject (); //object modified to  be used as a message
                break;
            case VCENTERGETALLPORTPROFILES:
                pPrismMessage = new VcenterGetAllPortProfileListMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *APPMGlobalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((AppmEnableManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new AppmEnableManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((SubProfileManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new SubProfileManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((ProfileManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ProfileManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((ProfileDomainManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ProfileDomainManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((ProfileDomainCompositeManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ProfileDomainCompositeManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        }

        else if ((VlanProfileCfgManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new VlanProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((ProfileMacManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ProfileMacManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((APPMVlanClassifierManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new APPMVlanClassifierManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((APPMVlanManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new APPMVlanManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((FcoeProfileCfgManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FcoeProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((PfcRxTxMapManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PfcRxTxMapManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((QosProfileCfgManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new QosProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((SecProfileCfgManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new SecProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ( (APPMNativeVlanManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new APPMNativeVlanManagedObject(dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
        }
        else if ( (APPMVlanClassificationMO::getClassName ()) == managedClassName) {
            pWaveManagedObject = new APPMVlanClassificationMO(dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
        }
        else if ( (APPMCtagVlanClassificationMO ::getClassName ()) == managedClassName) {
            pWaveManagedObject = new APPMCtagVlanClassificationMO(dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
        }
        else if ( (APPMCtagVlanClassificationRemoveMO ::getClassName ()) == managedClassName) {
            pWaveManagedObject = new APPMCtagVlanClassificationRemoveMO(dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
        }
        else if ( (APPMMacAddressVlanClassificationMO ::getClassName ()) == managedClassName) {
            pWaveManagedObject = new APPMMacAddressVlanClassificationMO(dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
        }
        else if ( (APPMMacGroupVlanClassificationMO ::getClassName ()) == managedClassName) {
            pWaveManagedObject = new APPMMacGroupVlanClassificationMO(dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
        }
        else
        {
			APPM_PLUG_TRACE(TRACE_LEVEL_FATAL, string("Unknown Managed Class Name : ") + managedClassName);            
        }
        return (pWaveManagedObject);
    }

    UI32 getNextPPid()
	{
        BitMap tempBitMap = g_pProfileId;
        UI32 i = 1;
        while ( ((tempBitMap.getValue(i)) != 0) && (i<(MAX_NO_PROFILES -1)) )
        {
            i++;
        }
        return (i);
	}
    
    void setGlobalPPid(UI32 ppId)
	{
       g_pProfileId.set(ppId);
	}

    void unSetGlobalPPid(UI32 ppId)
    {
        g_pProfileId.reset(ppId); 
    }

    void APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback (WaveSendToClusterContext  *ctx_p) {
        
        PrismLinearSequencerContext             *seq_ctx_p = NULL; 
        vector<LocationId>                      locationIdsInCluster;
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
        
        if ( WAVE_MESSAGE_SUCCESS == sendToClusterCompletionStatus ) {
            APPM_PLUG_TRACE (TRACE_LEVEL_INFO, "APPM Global Command succeeded on all the nodes.");
        } else {
            APPM_PLUG_TRACE (TRACE_LEVEL_INFO, "APPM Global Command failed on few or all the nodes.");
            locationIdsInCluster = ctx_p->getLocationsToSendToForPhase1();
            
            for (UI32 i = 0; i < locationIdsInCluster.size(); i++) {
                ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locationIdsInCluster[i]);
                if(WAVE_MESSAGE_SUCCESS != statusPerLocationId) {
                    APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, string("APPM config command failed on the node<")
                            + locationIdsInCluster[i] + ">, with an error: " + FrameworkToolKit::localize(statusPerLocationId));
                }
            }
        }
        
        delete ctx_p  -> getPPrismMessageForPhase1();
        delete ctx_p;
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        return;
        
    }

    void APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackForBulk (WaveSendToClusterContext  *ctx_p)
        {
            PrismLinearSequencerContext             *seq_ctx_p;
			WaveSendToClusterContext                *send_ctx_p = NULL;
			LocationId                              locationid;
            ClusterGetFirstFailureLocation(ctx_p, locationid);
            ResourceId sendToClusterCompletionStatus = ctx_p->getCompletionStatusForPhase1(locationid);
			APPMGlobalAPPMProfileBulkMessage *req_msg_p = NULL;
			APPMLocalAPPMProfileBulkMessage *sw_resp_p = NULL;
			APPMLocalAPPMProfileBulkMessage *resp_msg_p = NULL;

            seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());

			req_msg_p = dynamic_cast<APPMGlobalAPPMProfileBulkMessage *> (seq_ctx_p->getPPrismMessage ());			
			resp_msg_p = dynamic_cast<APPMLocalAPPMProfileBulkMessage *> (ctx_p->getPPrismMessageForPhase1());
			prismAssert ((resp_msg_p) != NULL, __FILE__, __LINE__);

			vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
            UI32    i = 0;
            UI32 size = 0;
            UI32 num_of_locations = 0;
            void *data = NULL;
            UI32 bufferIndex = 0;
            vector <string> mac_vectors;
            UI32 mac_count = 0;
            vector<UI32> bufferVector;
            portProfileBulkMsg_t *pMsgOutpt = NULL;
            unsigned char successFlag = 1;			

            APPM_PLUG_TRACE (TRACE_LEVEL_INFO, string("location size = ") + locations.size() );

            num_of_locations = locations.size() >0 ?1:0;


            for (i = 0; i < num_of_locations; i++)
            {
				sw_resp_p = dynamic_cast<APPMLocalAPPMProfileBulkMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
				prismAssert ((sw_resp_p) != NULL, __FILE__, __LINE__);
                if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
                {
					req_msg_p->copyAllBuffers(*sw_resp_p);
                }
                else
                {
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("Response processing: The location: ") + locations[i] );
                }

			}

        	UI32 numberOfBuffers = req_msg_p->getNumberOfBuffers();
	        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("numberOfBuffers received from backend= ") + numberOfBuffers);
			req_msg_p->getBufferTags(bufferVector);
			
			for (bufferIndex = 0; bufferIndex < numberOfBuffers; bufferIndex++) 
			{
				req_msg_p->getOutputBuffer(bufferVector[bufferIndex], size, data);
				if (size == 0) {
    	          trace(TRACE_LEVEL_ERROR,  string("Did n't receive the data for buffer num ") + bufferVector[bufferIndex]);
			              continue;
				}

			pMsgOutpt = (portProfileBulkMsg_t *)data;
			successFlag = pMsgOutpt->allSuccessFlag;
			trace(TRACE_LEVEL_INFO,	string("Bulk Mac association status from backend= (1 = success) " ) + successFlag );
			}

			if (successFlag != 1)  //atleast one mac failed at backend
			{
				for (i = 0; i <  pMsgOutpt->macCount; i++) {
			  		if(pMsgOutpt->macListAndFlags[i].macErrorFlag != 1)
		  			{
						mac_vectors.push_back(pMsgOutpt->macListAndFlags[i].profileMacsStr);
						mac_count++;
		  			}
					else
					  trace(TRACE_LEVEL_INFO, string("Error: Mac not associated: " )+ pMsgOutpt->macListAndFlags[i].profileMacsStr);
				}
				/* Reset the mac vector containing only successfully associated macs */
				req_msg_p->setProfileMacs (mac_vectors);
				req_msg_p->setMacCount    (mac_count);
			}

            delete ctx_p;
			delete send_ctx_p;
			delete resp_msg_p;
            seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
            return;
        }

	void APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackWithoutNextstep (WaveSendToClusterContext  *ctx_p)
          {
              //PrismLinearSequencerContext             *seq_ctx_p;
              LocationId                              locationid;
              ClusterGetFirstFailureLocation(ctx_p, locationid);
              vector<LocationId>  locationIdFailedNodes;
              ResourceId  status = WAVE_MESSAGE_SUCCESS;


              //seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());

              locationIdFailedNodes = ctx_p->getLocationsToSendToForPhase1();
              for (UI32 i = 0; i < locationIdFailedNodes.size(); i++) {
                  status = ctx_p->getCompletionStatusForPhase1(locationIdFailedNodes[i]);
                  if(status != WAVE_MESSAGE_SUCCESS)
                  {
                      APPM_PLUG_TRACE (TRACE_LEVEL_INFO, string("getCompletionStatus: ") + status + (" for location: ") + locationIdFailedNodes[i]);
                  }
              }

              delete ctx_p  -> getPPrismMessageForPhase1();
              delete ctx_p;
              return;
          }

    void APPMGlobalConfWorker::ClusterGetFirstFailureLocation(WaveSendToClusterContext  *ctx_p , LocationId &locationid)
    {
        vector<LocationId>  locationsToSendToForPhase1 = ctx_p -> getLocationsToSendToForPhase1 ();

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        for(unsigned int i = 0; i < locationsToSendToForPhase1.size(); i++)
        {
            locationid = locationsToSendToForPhase1[i];
            status = ctx_p -> getCompletionStatusForPhase1(locationid);
            if (status != WAVE_MESSAGE_SUCCESS) {
                return;
            }

        }
    }

    void APPMGlobalConfWorker::fcoeMessageStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
	    WaveSendToClusterContext    *send_ctx_p;
	    bool flag=true;
        APPMGlobalAPPMFcoeMessage *pAPPMGlobalAPPMFcoeMessage = 
            dynamic_cast<APPMGlobalAPPMFcoeMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        APPMLocalAPPMFcoeMessage *m = new APPMLocalAPPMFcoeMessage();

        m->setProfileName(pAPPMGlobalAPPMFcoeMessage->getProfileName());
        m->setOpCode(pAPPMGlobalAPPMFcoeMessage->getOpCode());
        m->setCmdCode(pAPPMGlobalAPPMFcoeMessage->getCmdCode());
        m->setFcoePort(pAPPMGlobalAPPMFcoeMessage->getFcoePort());

        send_ctx_p = new WaveSendToClusterContext(this,
			reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback), pPrismLinearSequencerContext);
        prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);
        send_ctx_p->setPartialSuccessFlag(flag);
   	    send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag); 
        sendToWaveCluster(send_ctx_p);
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEVEL);
        return;
    }

    void APPMGlobalConfWorker::fcoeMessageMoStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        APPMGlobalAPPMFcoeMessage *pAPPMGlobalAPPMFcoeMessage =
            dynamic_cast<APPMGlobalAPPMFcoeMessage *> (pPrismLinearSequencerContext->getPPrismMessage());
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
		ProfileManagedObject *pProfileManagedObject = NULL;
		int ret = APPM_SUCCESS;
		int retval;
		UI8 cmdCode = 0;
		cmdCode = pAPPMGlobalAPPMFcoeMessage->getCmdCode();
        string profileName = pAPPMGlobalAPPMFcoeMessage->getProfileName();
		syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
        //Load only the sub profile vector.
        syncQueryCtxt.addSelectField("subProfileCfg");
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		unsigned int fcoeMoCount = 0;
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				APPM_PLUG_TRACE (TRACE_LEVEL_ERROR, " SET_ELEM: Could not" 
					" find the profile MO with key: " + profileName);
	            status = WAVE_MESSAGE_ERROR;
				goto last;
			} else if (1 == numberOfResults) {
				pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
				APPM_PLUG_TRACE (TRACE_LEVEL_DEVEL, " SET_ELEM: Found the" 
					" profile MO with key: " + profileName);
			} else {
				APPM_PLUG_TRACE (TRACE_LEVEL_ERROR, " SET_ELEM:" 
					" Two or more profile MO's exists with key: " + profileName);
	            status = WAVE_MESSAGE_ERROR;
				goto last;
			}
		}

		if (pProfileManagedObject == NULL) {
			prismAssert (NULL != pProfileManagedObject, __FILE__, __LINE__);
            goto last;
		}
		
        if ((pAPPMGlobalAPPMFcoeMessage->getOpCode() == APPM_REMOVE) && (pAPPMGlobalAPPMFcoeMessage->getCmdCode() == FCOE_PROFILE_DELETE)){
            retval = pProfileManagedObject->delSubProfileFromVector("fcoe-profile");
            if (retval == 1) {
                APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " deleted "
                    "the fcoe sub-profile");
            } else {
                APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " fcoe"
                    "sub-profile is not found");
				status = WAVE_MESSAGE_ERROR;
	            goto last;
            }
        }
	
		else {

            if(pAPPMGlobalAPPMFcoeMessage->getOpCode() == APPM_CREATE) {
                WaveManagedObjectSynchronousQueryContext validateSyncQueryCtxt(FcoeProfileCfgManagedObject::getClassName());
                validateSyncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
                status = querySynchronouslyForCount (&validateSyncQueryCtxt, fcoeMoCount);

                    if (WAVE_MESSAGE_SUCCESS == status) {
                        if (0 == fcoeMoCount) {
                            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " Validation successful " 
                                " No FcoeProfile Managed object exists with key: " + profileName);
                        } else  {
                            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation failure " 
                                "Already existing FcoeProfile Managed Object with key: " + profileName);
                            goto last;
                        }
                    } else {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation not completed " 
                            "  Quering DB is failed");
                        goto last;
                    }
            }
		ret = pProfileManagedObject->updateFcoeSubProfileInfo(pPrismLinearSequencerContext, pAPPMGlobalAPPMFcoeMessage);
		if (ret != APPM_SUCCESS) {
            status = WAVE_MESSAGE_ERROR;
            goto last;
		}
		}
		cmdCode = pAPPMGlobalAPPMFcoeMessage->getCmdCode();

		if ((cmdCode == FCOE_PROFILE_CREATE) || (cmdCode == FCOE_PROFILE_DELETE)) {

            pProfileManagedObject->addAttributeToBeUpdated("subProfileCfg");
			updateWaveManagedObject(pProfileManagedObject);
		}

last:
		if (pResults != NULL) {
			pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			delete (pResults);
        }

        pPrismLinearSequencerContext->executeNextStep(status);
        return;
	}

	bool APPMGlobalConfWorker::isActiveProfilePresent() {

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute(new AttributeBool(true, "isActivated"));
		UI32 count = 0;
        bool activeProfilePresent = false;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

		status = querySynchronouslyForCount(&syncQueryCtxt, count);
        
        if ( ( WAVE_MESSAGE_SUCCESS == status ) && ( count >= 1 )  ) {
            
            activeProfilePresent = true ;
			APPM_PLUG_TRACE (TRACE_LEVEL_DEVEL, string (" "
					" API() active profiles: ") +  count );
		}
		return activeProfilePresent;
	}
	
	bool APPMGlobalConfWorker::isFcoeEnabledInDefaultProfile(
		PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext,
		PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		return ((APPMGlobalObjectManager::getInstance())->isFcoeEnabledInDefaultProfile(pPrismSynchronousLinearSequencerContext,
				pPrismLinearSequencerContext));
	}

    ResourceId APPMGlobalConfWorker::fcoeProfileValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		APPMGlobalAPPMFcoeMessage *pAPPMGlobalAPPMFcoeMessage =
			dynamic_cast<APPMGlobalAPPMFcoeMessage *> (pPrismLinearSequencerContext->getPPrismMessage());
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		WaveManagedObjectSynchronousQueryContext intfsyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = NULL;
		UI32 i;
		bool isprofiled = true;
		string profileName("\0");
		string fcoemapName("\0");
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEVEL);
		UI32 cmdCode = pAPPMGlobalAPPMFcoeMessage->getCmdCode();

		profileName = pAPPMGlobalAPPMFcoeMessage->getProfileName();
		fcoemapName = pAPPMGlobalAPPMFcoeMessage->getMapName();
		if (NsmUtils::isFcoeConfiguredOnAnyProfiledPort()) {
			status = APPM_ERR_FCOE_PROFILE_MODIFY;
			goto last;
		}

		switch (cmdCode) {
		case FCOE_PROFILE_CREATE:
			break;
		case FCOE_PROFILE_DELETE:
			if (isActiveProfilePresent()) {
				status = APPM_ERR_ACTIVE_PROFILES_PRESENT;
			}	
			break;
		case FCOE_PROFILE_FCOEPORT_ADD:
			intfsyncQueryCtxt.addAndAttribute (new AttributeBool(isprofiled,"isportprofiled"));
			intfsyncQueryCtxt.addSelectField ("phyType");
			pResults = querySynchronously (&intfsyncQueryCtxt);

			if (NULL != pResults)
			{
				UI32 numberOfResults = pResults->size ();
				if (0 == numberOfResults) {
					goto last;
				}
				else {
					for (i=0; i <numberOfResults ; i++){

						pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject  *>((*pResults)[i]);

                        if ( pPhyIntfLocalManagedObject->getPhyType() == IF_TYPE_GI ){
							NSM_PLUG_DBG_ERR("1G Profiled Ports  on carme");
							status = WRC_NSM_ERR_PORT_PROFILED_CARME;
							NSM_PLUG_DBG_S("1G Profiled Ports on carme " + FrameworkToolKit::localize(status));
							break;
						}	
					//break;
					} 

				}
				if (status != WAVE_MESSAGE_SUCCESS){
					goto last;
				}
			}
			if (isActiveProfilePresent()) {
				status = APPM_ERR_ACTIVE_PROFILES_PRESENT;
			}
			if(fcoemapName != "default") {
				status = WRC_APPM_ERR_INVALID_FCOEMAP_NAME;
			}	
			break;
		case FCOE_PROFILE_FCOEPORT_DEL:
			if (isActiveProfilePresent()) {
			status = APPM_ERR_ACTIVE_PROFILES_PRESENT;
			}
			break;
		default:
			break;
		}
last:
		if (pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
		pAPPMGlobalAPPMFcoeMessage->setCompletionStatus(status);
		pPrismLinearSequencerContext->executeNextStep(status);
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEVEL);
		return (status);
	}

    void  APPMGlobalConfWorker::APPMGlobalAPPMFcoeMessageHandler( APPMGlobalAPPMFcoeMessage *pAPPMGlobalAPPMFcoeMessage)
    {
		// Validations and Queries should go here
		PrismLinearSequencerStep sequencerSteps[] =
		{	
			reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::fcoeProfileValidationStep),
			reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::fcoeMessageStep),
			reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerStartTransactionStep),
			// Your configuration change code goes here
			reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::fcoeMessageMoStep),
			reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
			// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext =
			new PrismLinearSequencerContext (pAPPMGlobalAPPMFcoeMessage, this,
			sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
     pPrismLinearSequencerContext->holdAll();	
     pPrismLinearSequencerContext->start ();
    }

    ResourceId APPMGlobalConfWorker::defaultProfileValidation(APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileMessage)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		string profileName("\0");
		profileName = pAPPMGlobalAPPMProfileMessage->getProfileName();

		if (profileName.compare(DEFAULT_PORT_PROFILE_NAME) == 0) {
			switch (pAPPMGlobalAPPMProfileMessage->getCmdCode()) {
			case PORT_PROFILE_DELETE:
				status = APPM_ERR_DEFAULT_PROFILE_REMOVE;
				break;
			case PORT_PROFILE_ACTIVATE:
			case VCENTER_PORT_PROFILE_ACTIVATE:
				status = APPM_ERR_DEFAULT_PROFILE_ACTIVATE;
				break;
			case PORT_PROFILE_DEACTIVATE:
				status = APPM_ERR_DEFAULT_PROFILE_DEACTIVATE;
				break;
			case PORT_PROFILE_ASSOCIATE:
			case VCENTER_ASSOC_MAC:
				status = APPM_ERR_DEFAULT_PROFILE_ASSOCIATE;
				break;
			case PORT_PROFILE_DEASSOCIATE:
			case VCENTER_DEASSOC_MAC:
				status = APPM_ERR_DEFAULT_PROFILE_DEASSOCIATE;
				break;
			}
		}
		return (status);
	}

    ResourceId APPMGlobalConfWorker::validateProfileAlreadyCreatedOrAlreadyDeleted
            ( APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileMessage ) {

                ResourceId status = WAVE_MESSAGE_SUCCESS;
                UI32 count = 0;
                WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
                syncQueryCtxt.addAndAttribute (new AttributeString (pAPPMGlobalAPPMProfileMessage->getProfileName(), "profileName"));
                status = querySynchronouslyForCount(&syncQueryCtxt,count);

                if ( (WAVE_MESSAGE_SUCCESS == status) ) {
                    switch(pAPPMGlobalAPPMProfileMessage->getCmdCode()) {
                        case PORT_PROFILE_CREATE:
                        case VCENTER_PORT_PROFILE_CREATE: {
                            if (count >=1) {
                                status = AMPP_ERR_PROFILE_ALREADY_CREATED ;
                                tracePrintf(TRACE_LEVEL_INFO, "port-profile <%s> is already created in the database.", 
                                            pAPPMGlobalAPPMProfileMessage->getProfileName().c_str());
                            }
                            break;
                        }
                        case PORT_PROFILE_DELETE:
                        case VCENTER_PORT_PROFILE_DELETE: {
                            if (count == 0 ) {
                                status = AMPP_ERR_PROFILE_ALREADY_DELETED;
                                tracePrintf(TRACE_LEVEL_INFO, "port-profile <%s> is already deleted from the database.", 
                                            pAPPMGlobalAPPMProfileMessage->getProfileName().c_str());

                            }
                            break;
                        }
                    }
                } 
                return status;
    }            



    ResourceId APPMGlobalConfWorker::profileAssocDeassocValidation(APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileMessage, DceLinearSeqContext *dceLinearSeqContext)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 count = 0;
		string macAddr = pAPPMGlobalAPPMProfileMessage->getProfileMac();
		vector<WaveManagedObject *> *pResults = NULL;

		switch (pAPPMGlobalAPPMProfileMessage->getCmdCode()) {
		case PORT_PROFILE_ASSOCIATE:
		case VCENTER_ASSOC_MAC:
			{
				string profileName = pAPPMGlobalAPPMProfileMessage->getProfileName();
				/* check whether profile is activate state or not */
                if (isProfileActivated(profileName, status,dceLinearSeqContext) != true) {
                    status = APPM_ERR_PORT_PROFILE_IN_ACTIVE;
                    tracePrintf(TRACE_LEVEL_INFO, ">>> Profile is not activated <<<");
                    goto last;
                } else {                
					/* check whether the MAC is associated to any other profile or not */
                    tracePrintf(TRACE_LEVEL_INFO, ">>> Profile is activated <<<");
					WaveManagedObjectSynchronousQueryContext syncMacQueryCtxt(ProfileMacManagedObject::getClassName());

					syncMacQueryCtxt.addAndAttribute(new AttributeString(macAddr, "mac"));
                    status = querySynchronouslyForCount(&syncMacQueryCtxt,count);

				    if( status == WAVE_MESSAGE_SUCCESS ) {
                        if ( 0 != count ) {
                        APPM_PLUG_TRACE (TRACE_LEVEL_ERROR, " Mac is already associated to other profile.");
                        raslog_ext(__FILE__,__FUNCTION__,__LINE__,RASLOG_OID_INVALID,
							RASLOG_FRCLOG,NSM_2036,macAddr.c_str(),
							"Mac is already associated to other profile");
                        status = NSM_ERR_DCM_APPM_MAC_ASSOC;
                        } else {
                        goto last;
                        }
                    } else {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation not completed "
                            "  Quering DB is failed");
                        goto last;
                    }
                
                }
			}
			break;

		case PORT_PROFILE_DEASSOCIATE:
		case VCENTER_DEASSOC_MAC:
			{
				WaveManagedObjectSynchronousQueryContext syncMacQueryCtxt(ProfileMacManagedObject::getClassName());
				syncMacQueryCtxt.addAndAttribute(new AttributeString(macAddr, "mac"));
				status = querySynchronouslyForCount(&syncMacQueryCtxt,count);
				
				if ( WAVE_MESSAGE_SUCCESS == status ) {
					if( 0  == count  ){
						APPM_PLUG_TRACE (TRACE_LEVEL_ERROR, " MAC does not exist");
						status = APPM_ERR_MAC_NO_ASSOC;
					} else {
						goto last;
					}
				} else {
					APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation not completed "
							"  Quering DB is failed");
					goto last;
				}                                                                                                           
			 }
			break;
			
			
		default:
			break;
		}
last:
		if (pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
		
		return (status);
	}

    ResourceId APPMGlobalConfWorker::profileBulkIntraValidation(APPMGlobalAPPMProfileBulkMessage *pAPPMGlobalAPPMProfileBulkMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 numberOfResults = 0;
        ProfileManagedObject *pProfileManagedObject = NULL;
        string profileName = pAPPMGlobalAPPMProfileBulkMessage->getProfileName();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults) {
            numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                APPM_PLUG_TRACE (TRACE_LEVEL_ERROR, " is not found for " + profileName);
                status = WRC_NSM_ERR_DCM_APPM_PORT_PROFILE_EXIST;
                goto last;
            }else if (1 == numberOfResults) {
                pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
                APPM_PLUG_TRACE (TRACE_LEVEL_DEVEL, " Found MO for " + profileName);
            } else {
                APPM_PLUG_TRACE (TRACE_LEVEL_ERROR, ": Two or more objects exists");
                goto last;
            }

            if (pProfileManagedObject == NULL) {
                status = WAVE_MESSAGE_ERROR;
                goto last;
            }

            status = pProfileManagedObject->ProfileActivationValidation();
        }
last:
        if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
        return (status);
    }

    ResourceId APPMGlobalConfWorker::profileBulkAssocValidation(APPMGlobalAPPMProfileBulkMessage *pAPPMGlobalAPPMProfileBulkMessage, DceLinearSeqContext *dceLinearSeqContext)
            {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<string>::iterator it;
        vector<string> macAddList = pAPPMGlobalAPPMProfileBulkMessage->getProfileMacs();
        vector<WaveManagedObject *> *pResults = NULL;

        // remove duplicates in mac vector and reset the macCount
        std::sort(pAPPMGlobalAPPMProfileBulkMessage->m_profileMacs.begin(), pAPPMGlobalAPPMProfileBulkMessage->m_profileMacs.end());
        it = std::unique(pAPPMGlobalAPPMProfileBulkMessage->m_profileMacs.begin(),pAPPMGlobalAPPMProfileBulkMessage->m_profileMacs.end());
        pAPPMGlobalAPPMProfileBulkMessage->m_profileMacs.resize(std::distance(pAPPMGlobalAPPMProfileBulkMessage->m_profileMacs.begin(),it));
        pAPPMGlobalAPPMProfileBulkMessage->m_macCount = pAPPMGlobalAPPMProfileBulkMessage->m_profileMacs.size();

        switch (pAPPMGlobalAPPMProfileBulkMessage->getCmdCode()) {
            case PORT_PROFILE_ASSOCIATE:
            case VCENTER_ASSOC_MAC: {
                 string  profileName = pAPPMGlobalAPPMProfileBulkMessage->getProfileName();

                 {
                    //Query the list of macs from DCMD Database to see if there are duplicates.
                    WaveManagedObjectSynchronousQueryContext syncMacQueryCtxt(
                            ProfileMacManagedObject::getClassName());

                    vector<Attribute *> macAttributes;
                    for (size_t i = 0; i < macAddList.size(); ++i) {
                        macAttributes.push_back(
                                new AttributeString(macAddList[i], "mac"));
                    }
                    syncMacQueryCtxt.addAndAttribute(
                            new AttributeSqlIn(macAttributes),
                              WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);

                    syncMacQueryCtxt.addSelectField("mac");

                    pResults = querySynchronously(&syncMacQueryCtxt);

                    if (NULL != pResults) {
                        UI32 numberOfResults = pResults->size();
                        if (0 == numberOfResults) {
                            //Macs don't exist, so we can continue
                            goto last;
                        }
                        else {
                            for (size_t i = 0; i < numberOfResults; ++i) {
                                ProfileMacManagedObject *pProfileMacManagedObject =
                                        dynamic_cast<ProfileMacManagedObject *>((*pResults)[i]);
                                tracePrintf(
                                        TRACE_LEVEL_INFO,
                                        true,
                                        false,
                                        ">>> Duplicate Mac %s found for profile %s <<<",
                                        pProfileMacManagedObject->getMac().c_str(),
                                        profileName.c_str());

                                //Remove the duplicated macs from the mesasage for further processing
                                vector<string>::iterator itr;
                                itr = std::find(
                                                pAPPMGlobalAPPMProfileBulkMessage->m_profileMacs.begin(),
                                                pAPPMGlobalAPPMProfileBulkMessage->m_profileMacs.end(),
                                                pProfileMacManagedObject->getMac());
                                if (itr != pAPPMGlobalAPPMProfileBulkMessage->m_profileMacs.end()) {
                                    pAPPMGlobalAPPMProfileBulkMessage->m_profileMacs.erase(
                                            itr);
                                }
                            }
                            if (pAPPMGlobalAPPMProfileBulkMessage->m_profileMacs.size()
                                    == 0) {
                                tracePrintf(
                                        TRACE_LEVEL_INFO,
                                        true,
                                        false,
                                        ">>> Duplicate Macs found for profile %s,Setting Error <<<",
                                        profileName.c_str());

                                status = NSM_ERR_DCM_APPM_MAC_ASSOC;
                            }
                            // reset the macCount
                            pAPPMGlobalAPPMProfileBulkMessage->m_macCount = pAPPMGlobalAPPMProfileBulkMessage->m_profileMacs.size();
                        }
                    }
                    else {
                        //Macs don't exist.
                        goto last;
                    }
                }
            }
                break;



            default:
                break;
        }
        last: if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

        return (status);
    }

    ResourceId APPMGlobalConfWorker::profileBulkValidationStep(DceLinearSeqContext *pPrismLinearSequencerContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        APPMGlobalAPPMProfileBulkMessage *pAPPMGlobalAPPMProfileBulkMessage =
            dynamic_cast<APPMGlobalAPPMProfileBulkMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

        APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEVEL);
        string profileName("\0");
        size_t pos;
        profileName = pAPPMGlobalAPPMProfileBulkMessage->getProfileName();
        pos = profileName.find('%');
        if (pos == string::npos) {
            trace (TRACE_LEVEL_INFO,string("percentage character not found"));
        }
        else {
            status = WRC_NSM_ERR_DCM_APPM_INVALID_PORT_PROFILE_NAME;
            goto last;
        }

        /* default profile deletion/association validations */
		if (profileName.compare(DEFAULT_PORT_PROFILE_NAME) == 0) {
				switch (pAPPMGlobalAPPMProfileBulkMessage->getCmdCode()) {
		            case PORT_PROFILE_ASSOCIATE:
        		    case VCENTER_ASSOC_MAC:
						status = APPM_ERR_DEFAULT_PROFILE_ASSOCIATE;
						break;
		            case PORT_PROFILE_DEASSOCIATE:
        		    case VCENTER_DEASSOC_MAC:
                		status = APPM_ERR_DEFAULT_PROFILE_DEASSOCIATE;
                		break;
			}
		}

        if (status != WAVE_MESSAGE_SUCCESS) {
            goto last;
        }

        switch (pAPPMGlobalAPPMProfileBulkMessage->getCmdCode()) {
        /* assoc/deassoc validations */
        case PORT_PROFILE_ASSOCIATE:
        case VCENTER_ASSOC_MAC:

            status = profileBulkAssocValidation(pAPPMGlobalAPPMProfileBulkMessage, pPrismLinearSequencerContext );

            break;
        default:
            break;
        }
last:
        pAPPMGlobalAPPMProfileBulkMessage->setCompletionStatus(status);
        pPrismLinearSequencerContext->executeNextStep(status);
        APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEVEL);
        return (status);
    }

    void APPMGlobalConfWorker::profileBulkMessageStep(DceLinearSeqContext *pPrismLinearSequencerContext)
    {
        WaveSendToClusterContext    *send_ctx_p;
        bool flag=true;
        APPMGlobalAPPMProfileBulkMessage *pAPPMGlobalAPPMProfileBulkMessage =
            dynamic_cast<APPMGlobalAPPMProfileBulkMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

        APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEVEL);
        APPMLocalAPPMProfileBulkMessage *m = new APPMLocalAPPMProfileBulkMessage();

        if (pAPPMGlobalAPPMProfileBulkMessage->getOpCode() == APPM_CREATE) {
            m->setProfileId(getNextPPid());
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string (" :: PPID ") + m->getProfileId());
        }

        if(pAPPMGlobalAPPMProfileBulkMessage->getOpCode() == APPM_VCENTER)
            {
            if(pAPPMGlobalAPPMProfileBulkMessage->getCmdCode() == VCENTER_ASSOC_MAC)
            {
                m->setOpCode(APPM_SET_ELEM);
                m->setCmdCode(PORT_PROFILE_ASSOCIATE);
            }
            else if (pAPPMGlobalAPPMProfileBulkMessage->getCmdCode() == VCENTER_DEASSOC_MAC)
            {
                m->setOpCode(APPM_SET_ELEM);
                m->setCmdCode(PORT_PROFILE_DEASSOCIATE);
            }
        }
        else
        {
            m->setOpCode(pAPPMGlobalAPPMProfileBulkMessage->getOpCode());
            m->setCmdCode(pAPPMGlobalAPPMProfileBulkMessage->getCmdCode());
        }

        m->setProfileName(pAPPMGlobalAPPMProfileBulkMessage->getProfileName());
        m->setIsActivated(pAPPMGlobalAPPMProfileBulkMessage->getIsActivated());
        m->setProfileMacs(pAPPMGlobalAPPMProfileBulkMessage->getProfileMacs());
        m->setIsAllow(pAPPMGlobalAPPMProfileBulkMessage->getIsAllow());
        m->setMacCount(pAPPMGlobalAPPMProfileBulkMessage->getMacCount());

        send_ctx_p = new WaveSendToClusterContext(this,
            reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackForBulk),
            pPrismLinearSequencerContext);

        prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);

        send_ctx_p->setPPrismMessageForPhase1(m);
        send_ctx_p->setPartialSuccessFlag(flag);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag);
        sendToWaveCluster(send_ctx_p);
		m->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        return;
        APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEVEL);
    }

    void APPMGlobalConfWorker::profileBulkMessageMoStep(DceLinearSeqContext *pPrismLinearSequencerContext)
    {

        APPMGlobalAPPMProfileBulkMessage *pAPPMGlobalAPPMProfileBulkMessage =
            dynamic_cast<APPMGlobalAPPMProfileBulkMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

        ProfileManagedObject *pProfileManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        //AppmReturnCode ret = APPM_SUCCESS;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        string profileName = pAPPMGlobalAPPMProfileBulkMessage->getProfileName();
		APPM_PLUG_TRACE(TRACE_LEVEL_INFO, "profileName = " + profileName);
		APPM_PLUG_TRACE(TRACE_LEVEL_INFO, "ID= " + pAPPMGlobalAPPMProfileBulkMessage->getProfileId());
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
        switch(pAPPMGlobalAPPMProfileBulkMessage->getOpCode())
        {
            case APPM_SET_ELEM:
            case APPM_VCENTER:
                syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
                syncQueryCtxt.setLoadOneToManyRelationships(false);
                syncQueryCtxt.addSelectField("profileId");

                pResults = querySynchronously (&syncQueryCtxt);

                if (NULL != pResults) {
                    UI32 numberOfResults = pResults->size ();
                    if (0 == numberOfResults) {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " SET_ELEM: Could not"
                            " find the profile Managed object with key: " + profileName);
                        goto last;
                    } else if (1 == numberOfResults) {
                        pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
                        APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " SET_ELEM: Found the"
                            " Managed Object with key: " + profileName);
                    } else {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " SET_ELEM:"
                            " Two or more objects exists with key: " + profileName);
                        goto last;
                    }
                } else {
                    APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " SET_ELEM:"
                        " pResults ptr in NULL, Quering DB is failed");
			        goto last;
                }

                if (pProfileManagedObject == NULL) {
				  APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "pProfileManagedObject is NULL");
                    goto last;
                }

                switch (pAPPMGlobalAPPMProfileBulkMessage->getCmdCode()) {
				  
                case PORT_PROFILE_ASSOCIATE:
                case VCENTER_ASSOC_MAC:
					APPM_PLUG_TRACE(TRACE_LEVEL_INFO,"In case VCENTER_ASSOC_MAC:");
                    /* Update the Association information in the MO */
					pPrismLinearSequencerContext->setWaveManagedObjectId(pProfileManagedObject->getObjectId());
                    updateBulkAssociateInfo(pPrismLinearSequencerContext, pAPPMGlobalAPPMProfileBulkMessage);
                    break;
                case PORT_PROFILE_DEASSOCIATE:
                case VCENTER_DEASSOC_MAC:
                    /* Update the Association information in the MO */
                    pPrismLinearSequencerContext->setWaveManagedObjectId(pProfileManagedObject->getObjectId());
                    updateBulkAssociateInfo(pPrismLinearSequencerContext, pAPPMGlobalAPPMProfileBulkMessage);
                    break;
                default:
                    break;
                }
                break;
        }
last:
        if (pResults) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete (pResults);
        }

        pPrismLinearSequencerContext->executeNextStep(status);
        return;
    }

    AppmReturnCode APPMGlobalConfWorker::updateBulkAssociateInfo(PrismLinearSequencerContext *pPrismLinearSequencerContext, APPMGlobalAPPMProfileBulkMessage *pMsg)
        {

		  APPM_PLUG_TRACE (TRACE_LEVEL_DEVEL, "In updateBulkAssociateInfo");
            vector<WaveManagedObject *> *pResults = NULL;
			ObjectId profileManagedObjectId = ObjectId::NullObjectId; 
			string profileName = pMsg->getProfileName();
            vector<string> profileMacs = pMsg->getProfileMacs();
            UI32 totalMacs = pMsg->getMacCount();

            //Retrieve the object id of the ProfileManagedObject from the DceLinearSeqContext.
			profileManagedObjectId = dynamic_cast<DceLinearSeqContext *> (pPrismLinearSequencerContext)->getWaveManagedObjectId();
			trace (TRACE_LEVEL_DEVEL, string("profileManagedObjectId") + profileManagedObjectId.toString ());
            if (pMsg->getCmdCode() == PORT_PROFILE_ASSOCIATE || pMsg->getCmdCode() == VCENTER_ASSOC_MAC) {
                    APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " set_elem: new entry creation profilemacmanagedobject");
                    /* Allocate, push it to ProfileMacManagedMO and insert the MAC into vector*/
					APPM_PLUG_TRACE(TRACE_LEVEL_INFO,"TotalMacs Received : " + totalMacs);
					for (unsigned int i = 0; i < totalMacs; ++i) {
					  	ProfileMacManagedObject *pProfileMacManagedObject = NULL;
					  	APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " PROFILE ID : " + pMsg->getProfileId());

						pProfileMacManagedObject = new ProfileMacManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
						pProfileMacManagedObject->setMac(profileMacs[i]);
						pProfileMacManagedObject->setProfileName(pMsg->getProfileName());
						pProfileMacManagedObject->setProfileId(pMsg->getProfileId());
						pProfileMacManagedObject->setOwnerManagedObjectId(profileManagedObjectId);
						APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL,"Mac: " + profileMacs[i]);
						trace (TRACE_LEVEL_DEVEL, string("profileManagedObjectId") + profileManagedObjectId.toString ());
						trace (TRACE_LEVEL_DEVEL, string("pProfileMacManagedObject->getObjectId()=") + pProfileMacManagedObject->getObjectId().toString ());

						addToComposition(ProfileManagedObject::getClassName(),ProfileMacManagedObject::getClassName(),"assocMacs", profileManagedObjectId, pProfileMacManagedObject->getObjectId());

			            if (pProfileMacManagedObject) {
            			    pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pProfileMacManagedObject);
            			}
					}
            } else if (pMsg->getCmdCode() == PORT_PROFILE_DEASSOCIATE || pMsg->getCmdCode() == VCENTER_DEASSOC_MAC ) {
                /* Handling deassociation part */
                    APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " set_elem: profile mac entry found in ProfileMacManagedMO");
					for (unsigned int i = 0; i < totalMacs; ++i) {
	                    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileMacManagedObject::getClassName());
    	                syncQueryCtxt.addAndAttribute(new AttributeString(profileMacs[i], "mac"));
        	            pResults = querySynchronously(&syncQueryCtxt);
            	        ProfileMacManagedObject *pProfileMacMORslt = NULL;

                	    if (NULL != pResults) {
                    	    UI32 numberOfResults = pResults->size();
                        	if (1 == numberOfResults) {
                            	pProfileMacMORslt = dynamic_cast<ProfileMacManagedObject *>((*pResults)[0]);
                            	APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " Mac is found in ProfileMacMO DB");
                            	deleteFromComposition(ProfileManagedObject::getClassName(), ProfileMacManagedObject::getClassName(),"assocMacs", profileManagedObjectId, pProfileMacMORslt->getObjectId());
                        	}
                    	} else {
                        	APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " pResults ptr in NULL. Quering DB is failed");
                        	goto last;
                    	}

					}
            }

    last:
            if (pResults) {
                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                delete (pResults);
            }
			profileMacs.clear();
			profileMacs.reserve(0); //to free memory held by profileMacs
            return (APPM_SUCCESS);
        }


	ResourceId APPMGlobalConfWorker::profileIntraValidation(APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileMessage)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 numberOfResults = 0;
		ProfileManagedObject *pProfileManagedObject = NULL;
		string profileName = pAPPMGlobalAPPMProfileMessage->getProfileName();
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {
			numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				APPM_PLUG_TRACE (TRACE_LEVEL_ERROR, " is not found for " + profileName);
				status = WRC_NSM_ERR_DCM_APPM_PORT_PROFILE_EXIST;
				goto last;	
			}else if (1 == numberOfResults) {
				pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
				APPM_PLUG_TRACE (TRACE_LEVEL_DEVEL, " Found MO for " + profileName);
			} else {
				APPM_PLUG_TRACE (TRACE_LEVEL_ERROR, ": Two or more objects exists");
				goto last;
			}

			if (pProfileManagedObject == NULL) {
				status = WAVE_MESSAGE_ERROR;
				goto last;
			}

			status = pProfileManagedObject->ProfileActivationValidation();
		}
last:
		if (pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
		return (status);

	}

    ResourceId APPMGlobalConfWorker::profileValidationStep(DceLinearSeqContext *pPrismLinearSequencerContext)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileMessage =
			dynamic_cast<APPMGlobalAPPMProfileMessage *> (pPrismLinearSequencerContext->getPPrismMessage());
		
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEVEL);
		string profileName("\0");
		size_t pos;
        profileName = pAPPMGlobalAPPMProfileMessage->getProfileName();
    	pos = profileName.find('%');		
		if (pos == string::npos) {
			trace (TRACE_LEVEL_INFO,string("percentage character not found"));
		}
		else {
			status = WRC_NSM_ERR_DCM_APPM_INVALID_PORT_PROFILE_NAME;
			goto last;
		}

		/* default profile deletion/activation/association validations */
		status = defaultProfileValidation(pAPPMGlobalAPPMProfileMessage);
		
		if (status != WAVE_MESSAGE_SUCCESS) {
			goto last;
		}

		switch (pAPPMGlobalAPPMProfileMessage->getCmdCode()) {
            /* Idempotent validations */
            case PORT_PROFILE_CREATE : 
            case VCENTER_PORT_PROFILE_CREATE: {
                status = validateProfileAlreadyCreatedOrAlreadyDeleted(pAPPMGlobalAPPMProfileMessage);
                if ( AMPP_ERR_PROFILE_ALREADY_CREATED == status ) {
                    pPrismLinearSequencerContext->executeSuccessStep();
                    return WAVE_MESSAGE_SUCCESS;
                }
                break;
            }
            case PORT_PROFILE_DELETE : 
            case VCENTER_PORT_PROFILE_DELETE : {
                status = validateProfileAlreadyCreatedOrAlreadyDeleted(pAPPMGlobalAPPMProfileMessage);
                if ( AMPP_ERR_PROFILE_ALREADY_DELETED == status ) {
                    pPrismLinearSequencerContext->executeSuccessStep();
                    return WAVE_MESSAGE_SUCCESS;
                }
                /*Deletion of a port-profile, when associated with an interface is blocked.*/
                string portProfileName = pAPPMGlobalAPPMProfileMessage->getProfileName();
                if(ProvisioningUtil::isGvlanEnabled()) {
                    status = APPMUtils::isPortProfileActiveOnAnInterface(portProfileName);
                    if( WAVE_MESSAGE_SUCCESS != status ) {
                        break;
                    } 
                    status = APPMUtils::isPortProfileActiveOnADomain(portProfileName);
                    if( WAVE_MESSAGE_SUCCESS != status ) {
                        break;
                    }
                }
                break;
            }
		/* assoc/deassoc validations */
		case PORT_PROFILE_ASSOCIATE:
		case VCENTER_ASSOC_MAC:
		case PORT_PROFILE_DEASSOCIATE:
		case VCENTER_DEASSOC_MAC:
			status = profileAssocDeassocValidation(pAPPMGlobalAPPMProfileMessage, pPrismLinearSequencerContext );
			break;
		/* Intra profile validation */
		case PORT_PROFILE_ACTIVATE:
		case VCENTER_PORT_PROFILE_ACTIVATE:
			status = profileIntraValidation(pAPPMGlobalAPPMProfileMessage);
			break;
		default:
			break;
		}
last:
		pAPPMGlobalAPPMProfileMessage->setCompletionStatus(status);
		pPrismLinearSequencerContext->executeNextStep(status);
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEVEL);
		return (status);
	}

    void APPMGlobalConfWorker::profileMessageStep(DceLinearSeqContext *pPrismLinearSequencerContext)
	{
		WaveSendToClusterContext    *send_ctx_p;
		bool flag=true;
		APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileMessage =
			dynamic_cast<APPMGlobalAPPMProfileMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEVEL);
		APPMLocalAPPMProfileMessage *m = new APPMLocalAPPMProfileMessage();

		if (pAPPMGlobalAPPMProfileMessage->getOpCode() == APPM_CREATE) {
			m->setProfileId(getNextPPid());
			APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string (" :: PPID ") + m->getProfileId());
		}

		if(pAPPMGlobalAPPMProfileMessage->getOpCode() == APPM_VCENTER)
            {
            if(pAPPMGlobalAPPMProfileMessage->getCmdCode() == VCENTER_ASSOC_MAC)
            {
                m->setOpCode(APPM_SET_ELEM);
                m->setCmdCode(PORT_PROFILE_ASSOCIATE);
            }
            else if (pAPPMGlobalAPPMProfileMessage->getCmdCode() == VCENTER_DEASSOC_MAC)
            {
                m->setOpCode(APPM_SET_ELEM);
                m->setCmdCode(PORT_PROFILE_DEASSOCIATE);
            }
            else if (pAPPMGlobalAPPMProfileMessage->getCmdCode() == VCENTER_PORT_PROFILE_ACTIVATE)
            {
                m->setOpCode(APPM_SET_ELEM);
                m->setCmdCode(PORT_PROFILE_ACTIVATE);
            }
            else if (pAPPMGlobalAPPMProfileMessage->getCmdCode() == VCENTER_PORT_PROFILE_DEACTIVATE)
            {
                m->setOpCode(APPM_SET_ELEM);
                m->setCmdCode(PORT_PROFILE_DEACTIVATE);
            }
            else if (pAPPMGlobalAPPMProfileMessage->getCmdCode() == VCENTER_PORT_PROFILE_CREATE)
            {
                m->setOpCode(APPM_CREATE);
                m->setCmdCode(PORT_PROFILE_CREATE);
            }
            else if (pAPPMGlobalAPPMProfileMessage->getCmdCode() == VCENTER_PORT_PROFILE_DELETE)
            {
                m->setOpCode(APPM_REMOVE);
                m->setCmdCode(PORT_PROFILE_DELETE);
            }
        }
        else
        {
            m->setOpCode(pAPPMGlobalAPPMProfileMessage->getOpCode());
            m->setCmdCode(pAPPMGlobalAPPMProfileMessage->getCmdCode());
        }

		m->setProfileName(pAPPMGlobalAPPMProfileMessage->getProfileName());
		m->setIsActivated(pAPPMGlobalAPPMProfileMessage->getIsActivated());
		m->setProfileMac(pAPPMGlobalAPPMProfileMessage->getProfileMac());
		m->setIsAllow(pAPPMGlobalAPPMProfileMessage->getIsAllow());
		m->setIsFloodingRestricted(pAPPMGlobalAPPMProfileMessage->getIsFloodingRestricted());

        send_ctx_p = new WaveSendToClusterContext(this, 
			reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback),
			pPrismLinearSequencerContext);
		
        prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
        
		send_ctx_p->setPPrismMessageForPhase1(m);
	    send_ctx_p->setPartialSuccessFlag(flag);
	    send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag); 
	    sendToWaveCluster(send_ctx_p);
		return;
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEVEL);
	}

    void APPMGlobalConfWorker::profileMessageMoStep(DceLinearSeqContext *pPrismLinearSequencerContext)
    {

        APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileMessage =
            dynamic_cast<APPMGlobalAPPMProfileMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

        ProfileManagedObject *pProfileManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
		string macAddr("\0");
		//AppmReturnCode ret = APPM_SUCCESS;
		UI32 PPID = getNextPPid();
		UI32 count = 0;
		ResourceId status = WAVE_MESSAGE_SUCCESS;

        string profileName = pAPPMGlobalAPPMProfileMessage->getProfileName();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
        switch(pAPPMGlobalAPPMProfileMessage->getOpCode())
        {
            case APPM_CREATE:
                {
                    profileName = pAPPMGlobalAPPMProfileMessage->getProfileName();
                    syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
                    status = querySynchronouslyForCount (&syncQueryCtxt, count);
                    if (WAVE_MESSAGE_SUCCESS == status) {
                        if (0 == count) {
                            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " Validation successful " 
                                    " No Managed object exists with key: " + profileName);
                        } else  {
                            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation failure " 
                                    "Already existing  Managed Object with key: " + profileName);
                            goto last;
                        } 
                    } else {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation not completed " 
                                "  Quering DB is failed");
                        goto last;
                    }

                    pProfileManagedObject =
                        new ProfileManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

                    APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " Creating port-profile Managed Object");

                    prismAssert (NULL != pProfileManagedObject, __FILE__, __LINE__);

                    pProfileManagedObject->setProfileName(pAPPMGlobalAPPMProfileMessage->getProfileName());
                    pProfileManagedObject->setProfileId(PPID);
                    setGlobalPPid(PPID);
                    /* GC the MO */
                    pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pProfileManagedObject);
                    //When gvlan is disabled, add the created port-profile to the default port-profile-domain.
                    string portProfileName = pAPPMGlobalAPPMProfileMessage->getProfileName();
                    addRemoveProfilesToDefaultDomainWhenVirtualFabricIsDisabled
                        (portProfileName, APPM_CREATE, pPrismLinearSequencerContext);
                    break;
                }

            case APPM_REMOVE:
                {
                    syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
                    pResults = querySynchronously (&syncQueryCtxt);

                    if (NULL != pResults) 
                    {
                        UI32 numberOfResults = pResults->size ();
                        if (0 == numberOfResults) {
                            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Deleting port-profile Managed object : Can't find the policy object");
                            status = WAVE_MESSAGE_ERROR;
                            goto last;
                        } else if (1 == numberOfResults) {
                            pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
                            APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Deleting port-profile Managed object : Found Managed Object");
                        } else {
                            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Deleting port-profile Managed object : Two or more objects exists");
                            status = WAVE_MESSAGE_ERROR;
                            goto last;
                        }
                    }

                    if (pProfileManagedObject == NULL) {
                        status = WAVE_MESSAGE_ERROR;
                        goto last;
                    } else {

                        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, "Deleting port-profile Managed object, withouting deleting sub profiles individulaly :");
                        unSetGlobalPPid(pProfileManagedObject->getProfileId());
                        delete pProfileManagedObject;
                        pResults->clear();
                        delete pResults;
                        //When gvlan is disabled, remove the deleted port-profile from the default port-profile-domain.
                        addRemoveProfilesToDefaultDomainWhenVirtualFabricIsDisabled
                            (profileName, APPM_REMOVE, pPrismLinearSequencerContext);
                        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
                        return;
                    }
                }
				break;

			case APPM_SET_ELEM:
            case APPM_VCENTER:
//In case of mac association , do not load the ProfileManagedObject. The ProfileMacManagedObject is added as composition [ hence we will be needing only the object id of ProfileManagedObject, which can be retrived from the DceLinearSeqContext ].
            if ( (PORT_PROFILE_ASSOCIATE != pAPPMGlobalAPPMProfileMessage->getOpCode())) {
				syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
                syncQueryCtxt.setLoadOneToManyRelationships(false);
                syncQueryCtxt.addSelectField("profileId");

				pResults = querySynchronously (&syncQueryCtxt);
				
                if (NULL != pResults) {
					UI32 numberOfResults = pResults->size ();
					if (0 == numberOfResults) {
						APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " SET_ELEM: Could not" 
							" find the profile Managed object with key: " + profileName);
						goto last;
					} else if (1 == numberOfResults) {
						pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
						APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " SET_ELEM: Found the" 
							" Managed Object with key: " + profileName);
					} else {
						APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " SET_ELEM:" 
							" Two or more objects exists with key: " + profileName);
						goto last;
					}
				} else {
					APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " SET_ELEM:" 
						" pResults ptr in NULL, Quering DB is failed");
					goto last;
				}

				if (pProfileManagedObject == NULL) {
					goto last;	
				}
            }

				switch (pAPPMGlobalAPPMProfileMessage->getCmdCode()) {
				case PORT_PROFILE_ACTIVATE:
				case PORT_PROFILE_DEACTIVATE:
				case VCENTER_PORT_PROFILE_ACTIVATE:
					/* Update the Activate information in the MO */
					pProfileManagedObject->updateActivateInfo(pAPPMGlobalAPPMProfileMessage);
                    pProfileManagedObject->addAttributeToBeUpdated("isActivated");
					updateWaveManagedObject(pProfileManagedObject);
					break;

				case PORT_PROFILE_ASSOCIATE:
				case VCENTER_ASSOC_MAC:
					/* Set the profile id info too */
                    pProfileManagedObject = dynamic_cast<ProfileManagedObject *> (dynamic_cast<DceLinearSeqContext *> (pPrismLinearSequencerContext)->getPWaveManagedObject());
					pAPPMGlobalAPPMProfileMessage->setProfileId(pProfileManagedObject->getProfileId());
					/* Update the Association information in the MO */
					updateAssociateInfo(pPrismLinearSequencerContext, pAPPMGlobalAPPMProfileMessage);
					break;
                case PORT_PROFILE_DEASSOCIATE:
				case VCENTER_DEASSOC_MAC:
                    pAPPMGlobalAPPMProfileMessage->setProfileId(pProfileManagedObject->getProfileId()); 
                    /* Update the Association information in the MO */
                    pPrismLinearSequencerContext->setWaveManagedObjectId(pProfileManagedObject->getObjectId());
                    updateAssociateInfo(pPrismLinearSequencerContext, pAPPMGlobalAPPMProfileMessage);
                    break;
				case PORT_PROFILE_ALLOW_NONPROFILEDMACS:
                    trace(TRACE_LEVEL_DEBUG,"APPMGlobalConfWorker:updatewaveMO:allow mac");
                    pProfileManagedObject->setIsAllow(pAPPMGlobalAPPMProfileMessage->getIsAllow());
                    pProfileManagedObject->addAttributeToBeUpdated("isAllow");
                    updateWaveManagedObject(pProfileManagedObject);
                    break;
				case PORT_PROFILE_RESTRICT_FLOODING:
                    trace(TRACE_LEVEL_DEBUG,"APPMGlobalConfWorker:updatewaveMO:restrict flooding");
                    pProfileManagedObject->setIsFloodingRestricted(pAPPMGlobalAPPMProfileMessage->getIsFloodingRestricted());
                    pProfileManagedObject->addAttributeToBeUpdated("isFloodingRestricted");
                    updateWaveManagedObject(pProfileManagedObject);
                    break;

				default:
					break;
				}

				break;
		}
last:
		if (pResults) {
			pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			delete (pResults);
		}
		
        pPrismLinearSequencerContext->executeNextStep(status);
        return;
    }


    /*When gvlan (virtual fabric) is disabled, all the port-profiles must be part of default port-profile-domain.
      Hence any created port-profile needs to be added to the default domain 
      and any deleted port-profile needs to be removed from the default domain.*/
    ResourceId APPMGlobalConfWorker::addRemoveProfilesToDefaultDomainWhenVirtualFabricIsDisabled
        (string &portProfileName, AppmOpCode cmdCode, DceLinearSeqContext* pPrismLinearSequencerContext) {

            ResourceId status = WAVE_MESSAGE_SUCCESS;
            ProfileDomainCompositeManagedObject* pProfileDomainCompositeManagedObject = NULL;

            if( !(ProvisioningUtil::isGvlanEnabled()) ) {

                WaveManagedObjectSynchronousQueryContext ppDomainSyncQueryCtxt(ProfileDomainManagedObject::getClassName());
                vector<WaveManagedObject *> *ppDomainResults;
                ProfileDomainManagedObject* pProfileDomainManagedObject = NULL;

                ppDomainSyncQueryCtxt.addAndAttribute (new AttributeString (DEFAULT_PORT_PROFILE_DOMAIN_NAME, "portProfileDomainName"));
                ppDomainResults = querySynchronously (&ppDomainSyncQueryCtxt);

                if (NULL != ppDomainResults) {
                    UI32 numberOfResults = ppDomainResults->size ();
                    if (0 == numberOfResults) {
                        tracePrintf(TRACE_LEVEL_INFO, "profiledomain <default> is not found");
                        status = WAVE_MESSAGE_ERROR;
                        goto last;
                    }else if (1 == numberOfResults) {
                        pProfileDomainManagedObject = dynamic_cast<ProfileDomainManagedObject *>((*ppDomainResults)[0]);
                        tracePrintf(TRACE_LEVEL_INFO, "profiledomain <default> is found");
                    } else {
                        status = WAVE_MESSAGE_ERROR;
                        tracePrintf(TRACE_LEVEL_ERROR, "more than one profiledomain <default> found");
                        goto last;
                    }
                }

                if(NULL == pProfileDomainManagedObject) {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }

                switch(cmdCode) 
                {
                    case APPM_CREATE :
                        {
                            pProfileDomainCompositeManagedObject
                                = new ProfileDomainCompositeManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
                            pProfileDomainCompositeManagedObject->setPortProfileName(portProfileName);
                            pProfileDomainManagedObject->addPortProfileToPortProfileDomain
                                (pProfileDomainCompositeManagedObject);
                            updateWaveManagedObject(pProfileDomainManagedObject);
                            break;
                        }
                    case APPM_REMOVE : 
                        {
                            pProfileDomainManagedObject->removePortProfileFromPortProfileDomain(portProfileName);
                            updateWaveManagedObject(pProfileDomainManagedObject);
                            break;
                        }
                    default : 
                        {
                            break;
                        }

                        if (ppDomainResults) {
                            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*ppDomainResults);
                            delete (ppDomainResults);
                        }
                }        
            }
last: 
            return status;
        }


    void  APPMGlobalConfWorker::APPMGlobalAPPMProfileMessageHandler( APPMGlobalAPPMProfileMessage *pAPPMGlobalAPPMProfileMessage)
    {
// Validations and Queries should go here

        DceLinearSeqContext *dceLinearSeqContext = NULL;
        PrismLinearSequencerStep sequencerSteps[] =
        {
		    reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::profileValidationStep),
		    reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::profileMessageStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::profileMessageMoStep),

            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerFailedStep)
        };
//Using DceLinearSeqContext to save the object id of ProfileManagedObject during mac association validation step. The saved ObjectId will be used during updateMO Step for mac association cases.
        dceLinearSeqContext =
			new DceLinearSeqContext (pAPPMGlobalAPPMProfileMessage, this,
			sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

       dceLinearSeqContext->holdAll();
       dceLinearSeqContext->start ();
    }

    ResourceId APPMGlobalConfWorker::portProfileDomainValidationStep(DceLinearSeqContext *pPrismLinearSequencerContext) {

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        APPMGlobalProfileDomainMessage *pAPPMGlobalProfileDomainMessage =
            dynamic_cast<APPMGlobalProfileDomainMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

        APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEVEL);
        string profileDomainName("\0");
        size_t pos;
        profileDomainName = pAPPMGlobalProfileDomainMessage->getPortProfileDomainName();
		string profileName = pAPPMGlobalProfileDomainMessage->getPortProfileName();
        pos = profileDomainName.find('%');        
        if (pos == string::npos) {
            trace (TRACE_LEVEL_INFO,string("percentage character not found"));
        }
        else {
            status = WRC_NSM_ERR_DCM_APPM_INVALID_PORT_PROFILE_NAME;
            goto last;
        }

        switch (pAPPMGlobalProfileDomainMessage->getCmdCode()) 
		{
			case PORT_PROFILE_DOMAIN_CREATE :
			{
                if(!(ProvisioningUtil::isGvlanEnabled())) {
                    status = WRC_APPM_PP_DOMAINS_CANNOT_BE_CREATED_WHEN_VF_IS_DISABLED;
                }
                break;
			}
			case PORT_PROFILE_DOMAIN_DELETE :
			{
				/* Deletion of "default" port-profile-domain is not allowed. */
				if (DEFAULT_PORT_PROFILE_DOMAIN_NAME == profileDomainName) {
					status = WRC_APPM_DEFAULT_PORT_PROFILE_DOMAIN_CANNOT_BE_DELETED;
					goto last;
				}
                if(WAVE_MESSAGE_SUCCESS != APPMUtils::isPortProfileDomainActiveOnAnInterface(profileDomainName)) {
                    status = WRC_APPM_PORT_PROFILE_DOMAIN_IS_ASSOCIATED_WITH_AN_INTERFACE;
                    goto last;
                }
                break;
			}
			case PORT_PROFILE_ADD_TO_PORT_PROFILE_DOMAIN :
			{
				if(WAVE_MESSAGE_SUCCESS != APPMUtils::isPortProfileConflictingWithOtherProfilesOfTheDomain
					(profileName, profileDomainName)) {
					status = WRC_APPM_PORT_PROFILE_CONFLICTS_WITH_OTHER_PROFILES_OF_THE_DOMAIN;
					goto last;
				}
                if(WAVE_MESSAGE_SUCCESS != (APPMUtils::doesPortProfileExist(profileName))) {
                    status = WRC_APPM_PORT_PROFILE_DOESNT_EXIST;
                    goto last;
                }
                if( (DEFAULT_PORT_PROFILE_DOMAIN_NAME == profileDomainName) && (!(ProvisioningUtil::isGvlanEnabled())) ) {
                    status = WRC_APPM_DEFAULT_PORT_PROFILE_DOMAIN_CANNOT_BE_EDITED;
                    goto last;
                }
                if(WAVE_MESSAGE_SUCCESS != APPMUtils::isPortProfileDomainActiveOnAnInterface(profileDomainName)) {
                    status = WRC_APPM_PORT_PROFILE_DOMAIN_IS_ASSOCIATED_WITH_AN_INTERFACE;
                    goto last;
                }
				break;
			}
			case PORT_PROFILE_DELETE_FROM_PORT_PROFILE_DOMAIN :
			{
				if(WAVE_MESSAGE_SUCCESS != APPMUtils::isPortProfileDomainActiveOnAnInterface(profileDomainName)) {
					status = WRC_APPM_PORT_PROFILE_DOMAIN_IS_ASSOCIATED_WITH_AN_INTERFACE;
					goto last;
                }
                if( (DEFAULT_PORT_PROFILE_DOMAIN_NAME == profileDomainName) && (!(ProvisioningUtil::isGvlanEnabled())) ) {
                    status = WRC_APPM_DEFAULT_PORT_PROFILE_DOMAIN_CANNOT_BE_EDITED;
                    goto last;
                }
				break;
			}
			default : 
			{
				break;
			}
        }
last:
        pAPPMGlobalProfileDomainMessage->setCompletionStatus(status);
        pPrismLinearSequencerContext->executeNextStep(status);
        APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEVEL);
        return (status);
    }

    void APPMGlobalConfWorker::portProfileDomainSendToClusterStep(DceLinearSeqContext *pPrismLinearSequencerContext) {

        WaveSendToClusterContext    *send_ctx_p;
        bool flag=true;
        APPMGlobalProfileDomainMessage *pAPPMGlobalProfileDomainMessage =
            dynamic_cast<APPMGlobalProfileDomainMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

        APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEVEL);
        APPMLocalProfileDomainMessage* m = new APPMLocalProfileDomainMessage();

        m->setOpCode(pAPPMGlobalProfileDomainMessage->getOpCode());
        m->setCmdCode(pAPPMGlobalProfileDomainMessage->getCmdCode());
        m->setPortProfileDomainName(pAPPMGlobalProfileDomainMessage->getPortProfileDomainName());
        m->setPortProfileName(pAPPMGlobalProfileDomainMessage->getPortProfileName());

        send_ctx_p = new WaveSendToClusterContext(this, 
                reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalPortProfileDomainSendToClusterCallback),
                pPrismLinearSequencerContext);

        prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);

        send_ctx_p->setPPrismMessageForPhase1(m);
        send_ctx_p->setPartialSuccessFlag(flag);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag); 

        tracePrintf(TRACE_LEVEL_INFO, "Sending APPMGlobalProfileDomainMessage to all the nodes in cluster :"
            "Opcode<%d> CmdCode<%d> port-profile-domain-name<%s> port-profile-name<%s>.",
                m->getOpCode(), m->getCmdCode(), m->getPortProfileDomainName().c_str(), m->getPortProfileName().c_str());
        sendToWaveCluster(send_ctx_p);
        return;
        APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEVEL);
    }

    void APPMGlobalConfWorker::APPMGlobalPortProfileDomainSendToClusterCallback (WaveSendToClusterContext  *ctx_p) {

        PrismLinearSequencerContext             *seq_ctx_p = NULL;
        vector<LocationId>                      locationIdsInCluster;
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());

        if ( WAVE_MESSAGE_SUCCESS == sendToClusterCompletionStatus ) {
            APPM_PLUG_TRACE (TRACE_LEVEL_INFO, "Port profile domain config command succeeded on all the nodes.");
        } else {
            APPM_PLUG_TRACE (TRACE_LEVEL_INFO, "Port profile domain config command failed on few or all the nodes.");
            locationIdsInCluster = ctx_p->getLocationsToSendToForPhase1();

            for (UI32 i = 0; i < locationIdsInCluster.size(); i++) {
                ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locationIdsInCluster[i]);
                if(WAVE_MESSAGE_SUCCESS != statusPerLocationId) {
                    APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, string("Port profile domain config command failed on the node<")
                            + locationIdsInCluster[i] + ">, with an error: " + FrameworkToolKit::localize(statusPerLocationId));
                }
            }
        }

        delete ctx_p  -> getPPrismMessageForPhase1();
        delete ctx_p;
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        return;

    }


    void APPMGlobalConfWorker::portProfileDomainMoUpdateStep(DceLinearSeqContext *pPrismLinearSequencerContext) {

        APPMGlobalProfileDomainMessage *pAPPMGlobalProfileDomainMessage =
            dynamic_cast<APPMGlobalProfileDomainMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

        ProfileDomainManagedObject *pProfileDomainManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        string profileDomainName = pAPPMGlobalProfileDomainMessage->getPortProfileDomainName();

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileDomainManagedObject::getClassName());

        switch(pAPPMGlobalProfileDomainMessage->getCmdCode()) 
        {
            case PORT_PROFILE_DOMAIN_CREATE:
                {
                    syncQueryCtxt.addAndAttribute (new AttributeString (profileDomainName, "portProfileDomainName"));
                    UI32 count = 0;
                    status = querySynchronouslyForCount (&syncQueryCtxt, count);
                    if (WAVE_MESSAGE_SUCCESS == status) {
                        if (0 == count) {
                            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " Validation successful " 
                                    " No Managed object exists with key: " + profileDomainName);
                        } else  {
                            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation failure " 
                                    "Already existing  Managed Object with key: " + profileDomainName);
                            goto last;
                        } 
                    } else {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation not completed " 
                                "  Quering DB is failed");
                        goto last;
                    }

                    pProfileDomainManagedObject =
                        new ProfileDomainManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

                    APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " Creating port-profile Domain Managed Object");

                    prismAssert (NULL != pProfileDomainManagedObject, __FILE__, __LINE__);

                    pProfileDomainManagedObject->setPortProfileDomainName(pAPPMGlobalProfileDomainMessage->getPortProfileDomainName());
                    pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pProfileDomainManagedObject);
                    break;
                }

            case PORT_PROFILE_DOMAIN_DELETE: 
                {
                    syncQueryCtxt.addAndAttribute (new AttributeString (profileDomainName, "portProfileDomainName"));
                    pResults = querySynchronously (&syncQueryCtxt);

                    if (NULL != pResults) 
                    {
                        UI32 numberOfResults = pResults->size ();
                        if (0 == numberOfResults) {
                            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Deleting port-profile-domain Managed object : Can't find the object");
                            status = WAVE_MESSAGE_ERROR;
                            goto last;
                        } else if (1 == numberOfResults) {
                            pProfileDomainManagedObject = dynamic_cast<ProfileDomainManagedObject *>((*pResults)[0]);
                            APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Deleting port-profile-domain Managed object : Found Managed Object");
                        } else {
                            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Deleting port-profile-domain Managed object : Two or more objects exists");
                            status = WAVE_MESSAGE_ERROR;
                            goto last;
                        }
                    }

                    if (pProfileDomainManagedObject == NULL) {
                        status = WAVE_MESSAGE_ERROR;
                        goto last;
                    } else {

                        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, "Deleting port-profile-domain Managed object.");
                        delete pProfileDomainManagedObject;
                        pResults->clear();
                        delete pResults;
                        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
                        return;
                    }
                    break;
                }

            case PORT_PROFILE_ADD_TO_PORT_PROFILE_DOMAIN:
                {
                    syncQueryCtxt.addAndAttribute (new AttributeString (profileDomainName, "portProfileDomainName"));
                    pResults = querySynchronously (&syncQueryCtxt);

                    if (NULL != pResults)
                    {
                        UI32 numberOfResults = pResults->size ();
                        if (0 == numberOfResults) {
                            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Adding port-profile to the port-profile-domain Managed object : Can't find the object");
                            status = WAVE_MESSAGE_ERROR;
                            goto last;
                        } else if (1 == numberOfResults) {
                            pProfileDomainManagedObject = dynamic_cast<ProfileDomainManagedObject *>((*pResults)[0]);
                            APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Adding port-profile to the port-profile-domain Managed object : Found Managed Object");
                        } else {
                            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Adding port-profile to the port-profile-domain Managed object : Two or more objects exists");
                            status = WAVE_MESSAGE_ERROR;
                            goto last;
                        }
                    }

                    if (pProfileDomainManagedObject == NULL) {
                        status = WAVE_MESSAGE_ERROR;
                        goto last;
                    } else {

                        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, "Adding port-profile to the port-profile-domain Managed object.");
                        updateWaveManagedObject(pProfileDomainManagedObject);
                        ProfileDomainCompositeManagedObject* pProfileDomainCompositeManagedObject 
                            = new ProfileDomainCompositeManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
                        pProfileDomainCompositeManagedObject->setPortProfileName(pAPPMGlobalProfileDomainMessage->getPortProfileName());
                        pProfileDomainManagedObject->addPortProfileToPortProfileDomain
                            (pProfileDomainCompositeManagedObject);
                        pResults->clear();
                        delete pResults;
                        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
                        return;
                    }
                    break;
                }
            case PORT_PROFILE_DELETE_FROM_PORT_PROFILE_DOMAIN:
                {
                    syncQueryCtxt.addAndAttribute (new AttributeString (profileDomainName, "portProfileDomainName"));
                    pResults = querySynchronously (&syncQueryCtxt);

                    if (NULL != pResults)
                    {
                        UI32 numberOfResults = pResults->size ();
                        if (0 == numberOfResults) {
                            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Deleting port-profile  from the pp domain Managed object : Can't find the policy object");
                            status = WAVE_MESSAGE_ERROR;
                            goto last;
                        } else if (1 == numberOfResults) {
                            pProfileDomainManagedObject = dynamic_cast<ProfileDomainManagedObject *>((*pResults)[0]);
                            APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Deleting port-profile from the pp domain Managed object : Found Managed Object");
                        } else {
                            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Deleting port-profile  from the pp domain Managed object: Two or more objects exists");
                            status = WAVE_MESSAGE_ERROR;
                            goto last;
                        }
                    }

                    if (pProfileDomainManagedObject == NULL) {
                        status = WAVE_MESSAGE_ERROR;
                        goto last;
                    } else {

                        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, "Deleting port-profile from the port-profile-domain Managed object.");
                        updateWaveManagedObject(pProfileDomainManagedObject);
                        pProfileDomainManagedObject->removePortProfileFromPortProfileDomain
                            (pAPPMGlobalProfileDomainMessage->getPortProfileName());
                        pResults->clear();
                        delete pResults;
                        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
                        return;
                    }

                    break;
                }
            default : {
                          break;
                      }
        }
last:
        if (pResults) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete (pResults);
        }

        pPrismLinearSequencerContext->executeNextStep(status);
        return;
    }


    void APPMGlobalConfWorker::APPMGlobalPortProfileDomainMessageHandler ( APPMGlobalProfileDomainMessage* pAPPMGlobalProfileDomainMessage) {
        DceLinearSeqContext *dceLinearSeqContext = NULL;
        PrismLinearSequencerStep sequencerSteps[] = {
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::portProfileDomainValidationStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::portProfileDomainSendToClusterStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::portProfileDomainMoUpdateStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerFailedStep)
        };
        dceLinearSeqContext =
            new DceLinearSeqContext (pAPPMGlobalProfileDomainMessage, this,
                    sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        dceLinearSeqContext->holdAll();
        dceLinearSeqContext->start ();

    }

//Ankit
    void  APPMGlobalConfWorker::APPMGlobalAPPMProfileBulkMessageHandler( APPMGlobalAPPMProfileBulkMessage *pAPPMGlobalAPPMProfileBulkMessage)
    {
// Validations and Queries should go here

        DceLinearSeqContext *dceLinearSeqContext = NULL;
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::profileBulkValidationStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::profileBulkMessageStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::profileBulkMessageMoStep),

            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerFailedStep)
        };
//Using DceLinearSeqContext to save the object id of ProfileManagedObject during mac association validation step. The saved ObjectId will be used during updateMO Step for mac association cases.
        dceLinearSeqContext =
            new DceLinearSeqContext (pAPPMGlobalAPPMProfileBulkMessage, this,
            sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

       dceLinearSeqContext->holdAll();
       dceLinearSeqContext->start ();
    }

    ResourceId APPMGlobalConfWorker::qosProfileValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		APPMGlobalAPPMQosMessage *pAPPMGlobalAPPMQosMessage =
			dynamic_cast<APPMGlobalAPPMQosMessage *> (pPrismLinearSequencerContext->getPPrismMessage());
		string profileName("\0");
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEVEL);

        /*
         * For Inter Plugin Messages, no need to do any validation.
         */
        if ((pAPPMGlobalAPPMQosMessage->getOpCode()) == APPM_QOS_MSG)
        {
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return status;
        }

		profileName = pAPPMGlobalAPPMQosMessage->getProfileName();
		/* default qos sub-profile validations */
		if (profileName.compare(DEFAULT_PORT_PROFILE_NAME) == 0) {
			status = APPM_ERR_DEFAULT_QOS_PROFILE_MODIFY;
		}
		/* check whether profile is activated */
		status = isProfileActivated(profileName, status, pPrismLinearSequencerContext) ? APPM_ERR_PROFILE_ACTIVE : status;

		pAPPMGlobalAPPMQosMessage->setCompletionStatus(status);
		pPrismLinearSequencerContext->executeNextStep(status);
		return (status);
	}

    void APPMGlobalConfWorker::qosMessageStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		APPMGlobalAPPMQosMessage *pAPPMGlobalAPPMQosMessage = 
			dynamic_cast<APPMGlobalAPPMQosMessage *> (pPrismLinearSequencerContext->getPPrismMessage());
        ResourceId sanityStatus = WAVE_MESSAGE_SUCCESS;
		ProfileManagedObject *pProfileManagedObject = NULL;
        WaveSendToClusterContext    *send_ctx_p;
		bool flag=true;
		bool isQosVerified = true;
		ResourceId status = WAVE_MESSAGE_SUCCESS;

        /*
                * For Inter Plugin Messages, no need to do any validation.
                */
        if ((pAPPMGlobalAPPMQosMessage->getOpCode()) == APPM_QOS_MSG)
        {
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }

		APPMLocalAPPMQosMessage *m = new APPMLocalAPPMQosMessage();
		string profileName = pAPPMGlobalAPPMQosMessage->getProfileName();
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
        syncQueryCtxt.addSelectField ("profileId" );
        syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        QosProfileCfgManagedObject *pQosProfileCfgManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQosQueryCtxt(QosProfileCfgManagedObject::getClassName());
        //The below statement is to load the QosProfileCfgManagedObject with only an attribute.
        syncQosQueryCtxt.addSelectField("defaultCosValue");
        syncQosQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
        vector<WaveManagedObject *> *pQosResults = querySynchronously (&syncQosQueryCtxt);

		if (NULL != pResults) 
		{
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Trying to set Profile Id in Qos Local Message : Can't find profile Managed object for Profile ");
			}else if (1 == numberOfResults) {
				pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
				APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Trying to set Profile Id in Qos Local Message : Found Managed object");
			} else {
				APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Trying to set Profile Id in Qos Local Message : Two or more objects exists");
			}
		}

		if (pProfileManagedObject == NULL) {
			status = WAVE_MESSAGE_ERROR;
            goto last;
        } else {
            prismAssert (NULL != pProfileManagedObject, __FILE__, __LINE__);
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ( profileName ));
            m->setProfileId(pProfileManagedObject->getProfileId());
        }

        if (pQosResults != NULL && pQosResults->size()) {
            pQosProfileCfgManagedObject = dynamic_cast<QosProfileCfgManagedObject *>((*pQosResults)[0]);
        } else if (pQosResults == NULL) {
			status = WAVE_MESSAGE_ERROR;
            goto last;
        }

        switch ( pAPPMGlobalAPPMQosMessage->getCmdCode() )
        {
        case QOS_COS_TO_COS_MUTATION:
	        if (!pQosProfileCfgManagedObject) 
    	    {
				status = WAVE_MESSAGE_ERROR;
            	goto last;
	        }
		
            isQosVerified = pQosProfileCfgManagedObject->verifyCosMutMap 
                (pAPPMGlobalAPPMQosMessage->getCosToCosMutationMap());
            break;

        case QOS_COS_TO_TRAFFIC_MAP:
	        if (!pQosProfileCfgManagedObject) 
    	    {
				status = WAVE_MESSAGE_ERROR;
            	goto last;
	        }
		
            isQosVerified = pQosProfileCfgManagedObject->verifyCosTcMap
                (pAPPMGlobalAPPMQosMessage->getCosToTrafficClassMap());
            break;

        case QOS_PROFILE_CEE_MAP:
	        if (!pQosProfileCfgManagedObject) 
    	    {
				status = WAVE_MESSAGE_ERROR;
            	goto last;
	        }
		
            isQosVerified = pQosProfileCfgManagedObject->verifyCeeMap
                (pAPPMGlobalAPPMQosMessage->getCeeMapName());
            break;

        default:
            break;
        }

        if (!isQosVerified)
        {
            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR,string("APPM:: MAP Doesn't Exist\n"));
            pAPPMGlobalAPPMQosMessage->setCompletionStatus(WRC_SSM_QOS_MAP_NOT_FOUND);
            status = WRC_SSM_QOS_MAP_NOT_FOUND;
            goto last;
        }

        sanityStatus = appmQosVcsSanity(
            pAPPMGlobalAPPMQosMessage->getProfileName(),
            pAPPMGlobalAPPMQosMessage->getCmdCode(),
            pAPPMGlobalAPPMQosMessage->getCeeMapName(),
            pAPPMGlobalAPPMQosMessage->getCosToCosMutationMap(),
            pAPPMGlobalAPPMQosMessage->getDefaultCosValue());

        if (sanityStatus != WAVE_MESSAGE_SUCCESS)
        {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, "sanityStatus is not success");
            pAPPMGlobalAPPMQosMessage->setCompletionStatus(sanityStatus);
            status = sanityStatus;
            goto last;
        }

        m->setProfileName(pAPPMGlobalAPPMQosMessage->getProfileName());
        m->setOpCode(pAPPMGlobalAPPMQosMessage->getOpCode());
        m->setCmdCode(pAPPMGlobalAPPMQosMessage->getCmdCode());
        m->setDefaultCosValue(pAPPMGlobalAPPMQosMessage->getDefaultCosValue());
        m->setQosTrust(pAPPMGlobalAPPMQosMessage->getQosTrust());
        m->setFlowControlTxEnable(pAPPMGlobalAPPMQosMessage->getFlowControlTxEnable());
        m->setFlowControlRxEnable(pAPPMGlobalAPPMQosMessage->getFlowControlRxEnable());
        m->setPfcValue(pAPPMGlobalAPPMQosMessage->getPfcValue());
        m->setTxValue(pAPPMGlobalAPPMQosMessage->getTxValue());
        m->setRxValue(pAPPMGlobalAPPMQosMessage->getRxValue());
        m->setCeeMapName(pAPPMGlobalAPPMQosMessage->getCeeMapName());
        m->setCosToCosMutationMap(pAPPMGlobalAPPMQosMessage->getCosToCosMutationMap());
        m->setCosToTrafficClassMap(pAPPMGlobalAPPMQosMessage->getCosToTrafficClassMap());

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback), pPrismLinearSequencerContext);
        prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);
	    send_ctx_p->setPartialSuccessFlag(flag);
	    send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag); 
		sendToWaveCluster(send_ctx_p);

last:
        if(NULL != pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

        if(NULL != pQosResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pQosResults);
        }

		if (status != WAVE_MESSAGE_SUCCESS) {
	        pPrismLinearSequencerContext->executeNextStep(status);
			delete m;
		}
		return;
	}

    void APPMGlobalConfWorker::qosMessageMoStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        APPMGlobalAPPMQosMessage *pAPPMGlobalAPPMQosMessage =
            dynamic_cast<APPMGlobalAPPMQosMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

	    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(QosProfileCfgManagedObject::getClassName());

        vector<WaveManagedObject *> *pResults = NULL;
        vector<WaveManagedObject *> *pQosResults = NULL;
        QosProfileCfgManagedObject *pQosProfileCfgManagedObject = NULL;
        string profileName = pAPPMGlobalAPPMQosMessage->getProfileName();
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		unsigned int qosMoCount = 0;
		int retval;
        syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));

		switch(pAPPMGlobalAPPMQosMessage->getOpCode())
		{
		case APPM_CREATE:
			{

				WaveManagedObjectSynchronousQueryContext validateSyncQueryCtxt(QosProfileCfgManagedObject::getClassName());
				validateSyncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
				status = querySynchronouslyForCount (&validateSyncQueryCtxt, qosMoCount);

				if (WAVE_MESSAGE_SUCCESS == status) {
					if (0 == qosMoCount) {
						APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " Validation successful " 
							" No QosProfile Managed object exists with key: " + profileName);
					} else  {
						APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation failure " 
							"Already existing QosProfile Managed Object with key: " + profileName);
						goto last;
					}
				} else {
					APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation not completed " 
						"  Quering DB is failed");
					goto last;
				}


               ProfileManagedObject *pProfileManagedObject = NULL;
               WaveManagedObjectSynchronousQueryContext profileQueryCtxt
                   (ProfileManagedObject::getClassName());
               profileQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
               
               // In case of QOS Profile creation, load only the sub profile vector of ProfileManagedObject, update the vector and partially update the DB.
               profileQueryCtxt.addSelectField("subProfileCfg");
               pResults = querySynchronously (&profileQueryCtxt);

               if (pResults == NULL || !(pResults->size()))
               {
                   status = WAVE_MESSAGE_ERROR;
                   goto last;
               }

               pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
		
                pQosProfileCfgManagedObject =
                    new QosProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

                prismAssert (NULL != pQosProfileCfgManagedObject, __FILE__, __LINE__);

			    pQosProfileCfgManagedObject->setDescription(pAPPMGlobalAPPMQosMessage->getProfileName());
                pQosProfileCfgManagedObject->setDefaultCosValue(pAPPMGlobalAPPMQosMessage->getDefaultCosValue());
				pQosProfileCfgManagedObject->setQosTrust(pAPPMGlobalAPPMQosMessage->getQosTrust());
                pQosProfileCfgManagedObject->setCeeMapId(ObjectId::NullObjectId);
                pQosProfileCfgManagedObject->setCosTrafficClass(ObjectId::NullObjectId);
                pQosProfileCfgManagedObject->setCosMutation(ObjectId::NullObjectId);
                pQosProfileCfgManagedObject->setName(string("qos-profile"));
                pQosProfileCfgManagedObject->setDefaultFlowControlVals();
                pProfileManagedObject->addSubProfileToVector
                    ((SubProfileManagedObject *) pQosProfileCfgManagedObject);

                pProfileManagedObject->addAttributeToBeUpdated("subProfileCfg");
                updateWaveManagedObject(pProfileManagedObject);
                }
                break;

			case APPM_REMOVE: 
				{
					ProfileManagedObject *pProfileManagedObject = NULL;
					WaveManagedObjectSynchronousQueryContext profileQueryCtxt
						(ProfileManagedObject::getClassName());
					profileQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));

                    //In case of QOS Profile delete, load only the sub profile vector of the ProfileManagedObject.
                    profileQueryCtxt.addSelectField("subProfileCfg");
					pResults = querySynchronously (&profileQueryCtxt);

					if (pResults == NULL || !(pResults->size()))
					{
						status = WAVE_MESSAGE_ERROR;
						goto last;
					}

					pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
					if ( pAPPMGlobalAPPMQosMessage->getCmdCode() == QOS_PROFILE_DEL){
						retval = pProfileManagedObject->delSubProfileFromVector("qos-profile");
						if (retval == 1) {
							APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " deleted "
								"the qos sub-profile");
						} else {
							APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " qos "
								"sub-profile is not found");
							status = WAVE_MESSAGE_ERROR;
							goto last;
						}	
					}
                    
                    //Since only the QOS profile is deleted from the sub profile vector, perform partial DB commit, to delete a QOSProfileObject entry.
                    pProfileManagedObject->addAttributeToBeUpdated("subProfileCfg");

					updateWaveManagedObject(pProfileManagedObject);	
				}
				break;
            case APPM_SET_ELEM:
				{
					pResults = querySynchronously (&syncQueryCtxt);
					if (NULL != pResults) 
					{
						UI32 numberOfResults = pResults->size ();
						if (0 == numberOfResults) {
							APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating qos-profile Managed object : Can't find the policy object");
						}else if (1 == numberOfResults) {
							pQosProfileCfgManagedObject = dynamic_cast<QosProfileCfgManagedObject *>((*pResults)[0]);
							APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Updating qos-profile Managed object : Found Managed Object");
						} else {
							APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating qos-profile Managed object : Two or more objects exists");
						}
					}

					if (pQosProfileCfgManagedObject != NULL) 
					{
						updateWaveManagedObject(pQosProfileCfgManagedObject);

						prismAssert (NULL != pQosProfileCfgManagedObject, __FILE__, __LINE__);

						switch(pAPPMGlobalAPPMQosMessage->getCmdCode())
						{
                        case QOS_NO_DEFAULT_COS:
						case QOS_DEFAULT_COS:
							pQosProfileCfgManagedObject->setDefaultCosValue(pAPPMGlobalAPPMQosMessage->getDefaultCosValue());
                            pQosProfileCfgManagedObject->addAttributeToBeUpdated("defaultCosValue");
                            break;
                        case QOS_NO_TRUST:
                        case QOS_TRUST:
							pQosProfileCfgManagedObject->setQosTrust(pAPPMGlobalAPPMQosMessage->getQosTrust());
                            pQosProfileCfgManagedObject->addAttributeToBeUpdated("qosTrust");
                            break;
                        case QOS_NO_FLOW_CTRL_PFC:
                            {
                                unsigned int iter;
                                vector<WaveManagedObjectPointer<PfcRxTxMapManagedObject> >  tempFlowControlPfcRxTxMapVector;

                                tempFlowControlPfcRxTxMapVector = pQosProfileCfgManagedObject->getFlowControlPfcRxTxMap();
                                pQosProfileCfgManagedObject->addAttributeToBeUpdated("flowControlPfcRxTxMap");
                                for (iter=0 ; iter < tempFlowControlPfcRxTxMapVector.size(); iter++ )
                                {
                                    if(tempFlowControlPfcRxTxMapVector[iter]->m_pfcValue ==
                                        pAPPMGlobalAPPMQosMessage->getPfcValue())
                                        break;
                                }
                                
                                if (iter != tempFlowControlPfcRxTxMapVector.size())
                                 pQosProfileCfgManagedObject->deletePfcTxRxToVector(iter);

                                break;
                            }
                        case QOS_FLOW_CTRL_PFC:
							{
								unsigned int iter;
                                /*
                                UI32 txValue = pAPPMGlobalAPPMQosMessage->getTxValue();
                                UI32 rxValue = pAPPMGlobalAPPMQosMessage->getRxValue();
                                bool flowcontrol_val = false;
                                */
								vector<WaveManagedObjectPointer<PfcRxTxMapManagedObject> >  tempFlowControlPfcRxTxMapVector;
								tempFlowControlPfcRxTxMapVector = pQosProfileCfgManagedObject->getFlowControlPfcRxTxMap();
                                pQosProfileCfgManagedObject->addAttributeToBeUpdated("flowControlPfcRxTxMap");

                                for (iter=0 ; iter < tempFlowControlPfcRxTxMapVector.size(); iter++ )
                                {
                                    if(tempFlowControlPfcRxTxMapVector[iter]->m_pfcValue ==
                                        pAPPMGlobalAPPMQosMessage->getPfcValue())
                                    {
                                        /*update case*/
                                        /*
                                         * Both values are off, i.e should be deleted
                                         * from the MO.
                                         */
                                        pQosProfileCfgManagedObject->deletePfcTxRxToVector(iter);

                                        break;
                                    }
                                }
                                
                                PfcRxTxMapManagedObject *pPfcRxTxMapManagedObject =
                                    new PfcRxTxMapManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

                                pPfcRxTxMapManagedObject->setPfcValue(pAPPMGlobalAPPMQosMessage->getPfcValue());
                                pPfcRxTxMapManagedObject->setTxValue(pAPPMGlobalAPPMQosMessage->getTxValue());
                                pPfcRxTxMapManagedObject->setRxValue(pAPPMGlobalAPPMQosMessage->getRxValue());

                                pQosProfileCfgManagedObject->addPfcTxRxToVector(pPfcRxTxMapManagedObject);

#if 0
                                for (iter=0 ; iter < tempFlowControlPfcRxTxMapVector.size(); iter++ )
								{
									if(tempFlowControlPfcRxTxMapVector[iter]->m_pfcValue ==
										pAPPMGlobalAPPMQosMessage->getPfcValue())
									{/*update case*/

                                        if (txValue || rxValue)
                                        {

                                            (tempFlowControlPfcRxTxMapVector[iter])->
                                                setTxValue(txValue);
                                            (tempFlowControlPfcRxTxMapVector[iter])->
                                                setRxValue(rxValue);
                                        } else {
                                        /*
                                         * Both values are off, i.e should be deleted 
                                         * from the MO.
                                         */
                                            pQosProfileCfgManagedObject->deletePfcTxRxToVector(iter);
                                        }

                                        break;
									}
								}


								if (iter == tempFlowControlPfcRxTxMapVector.size())
								{/*Create a new Mo and insert it in qos pfc map vector*/
									PfcRxTxMapManagedObject *pPfcRxTxMapManagedObject = 
										new PfcRxTxMapManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager())); 

									pPfcRxTxMapManagedObject->setPfcValue(pAPPMGlobalAPPMQosMessage->getPfcValue());
									pPfcRxTxMapManagedObject->setTxValue(pAPPMGlobalAPPMQosMessage->getTxValue());
									pPfcRxTxMapManagedObject->setRxValue(pAPPMGlobalAPPMQosMessage->getRxValue());

                                    pQosProfileCfgManagedObject->addPfcTxRxToVector(pPfcRxTxMapManagedObject);
								}
#endif
							}

										break;

                        case QOS_NO_FLOW_CTRL_ALL:
						case QOS_FLOW_CTRL_ALL:
                                        
                            if (pAPPMGlobalAPPMQosMessage->getFlowControlTxEnable() ||
                                 pAPPMGlobalAPPMQosMessage->getFlowControlRxEnable())
                            {
							pQosProfileCfgManagedObject->setFlowControlTxEnable(pAPPMGlobalAPPMQosMessage->getFlowControlTxEnable());
							pQosProfileCfgManagedObject->setFlowControlRxEnable(pAPPMGlobalAPPMQosMessage->getFlowControlRxEnable());
                            } else {
                            /*
                             * Reseting it to default value,
                             * so that it is not shown in the running-config.
                             */
                            pQosProfileCfgManagedObject->setDefaultFlowControlVals();
                            }
                            pQosProfileCfgManagedObject->addAttributeToBeUpdated("flowControlTxEnable");
                            pQosProfileCfgManagedObject->addAttributeToBeUpdated("flowControlRxEnable");
							break;
                        case QOS_PROFILE_NO_CEE_MAP:
                            pQosProfileCfgManagedObject->setCeeMapId(ObjectId::NullObjectId);
                            pQosProfileCfgManagedObject->addAttributeToBeUpdated("ceeMapId");
                            break;
						case QOS_PROFILE_CEE_MAP:
							{
								string ceeMapName;
								ObjectId ceeMapObjId = ObjectId::NullObjectId;
								CeeMapManagedObject *pCeeMapManagedObject = NULL;
								WaveManagedObjectSynchronousQueryContext ceeSyncQueryCtxt(CeeMapManagedObject::getClassName());
								ceeMapName = pAPPMGlobalAPPMQosMessage->getCeeMapName();


								ceeSyncQueryCtxt.addAndAttribute (new AttributeString (ceeMapName, "ceeMapName"));
                                ceeSyncQueryCtxt.addSelectField("objectId");
								pQosResults = querySynchronously (&ceeSyncQueryCtxt);

								if (NULL != pQosResults) 
								{
									UI32 numberOfResults = pQosResults->size ();
									if (0 == numberOfResults) {
										APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating cee_map objectId in qos-profile Managed object : Can't find object");
									}else if (1 == numberOfResults) {
										pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject *>((*pQosResults)[0]);
										APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Updating cee_map objectId in qos-profile Managed object : Found Managed Object");
									} else {
										APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating cee_map objectId in qos-profile Managed object : Two or more objects exists");
									}
								}

                                if (NULL != pCeeMapManagedObject)
                                {
								ceeMapObjId = pCeeMapManagedObject->getObjectId();
								pQosProfileCfgManagedObject->setCeeMapId(ceeMapObjId);
        	                        /* Flushing off Qos Config other than Cos.  */
                                pQosProfileCfgManagedObject->applyCeeMap();
                                }
							}
                            break;
                        case QOS_NO_COS_TO_COS_MUTATION:
                            pQosProfileCfgManagedObject->setCosMutation(ObjectId::NullObjectId);
                            pQosProfileCfgManagedObject->addAttributeToBeUpdated("cosMutation");
                            break;
						case QOS_COS_TO_COS_MUTATION:
							{
                                APPM_PLUG_TRACE(TRACE_LEVEL_INFO, "APPM_SET_ELEM QOS_COS_TO_COS_MUTATION :: ENTER");
								string cosToCosMutationMapName;
								CosToCosMutationMapManagedObject *pCosToCosMutationMapManagedObject = NULL;
								ObjectId cosToCosMutationMapObjId = ObjectId::NullObjectId;
								WaveManagedObjectSynchronousQueryContext mutationSyncQueryCtxt(CosToCosMutationMapManagedObject::getClassName());
                                cosToCosMutationMapName = pAPPMGlobalAPPMQosMessage->getCosToCosMutationMap();

								mutationSyncQueryCtxt.addAndAttribute (new AttributeString (cosToCosMutationMapName, "cosToCosMutationMapName"));
                                mutationSyncQueryCtxt.addSelectField("objectId");
								pQosResults = querySynchronously (&mutationSyncQueryCtxt);

								if (NULL != pQosResults) 
								{
									UI32 numberOfResults = pQosResults->size ();
									if (0 == numberOfResults) {
										APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating cos_to_cos_mutation_map objectId in qos-profile Managed object : Can't find object");
									}else if (1 == numberOfResults) {
										pCosToCosMutationMapManagedObject = dynamic_cast<CosToCosMutationMapManagedObject *>((*pQosResults)[0]);
										APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Updating cos_to_cos_mutation_map objectId in qos-profile Managed object : Found Managed Object");
									} else {
										APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating cos_to_cos_mutation_map objectId in qos-profile Managed object : Two or more objects exists");
									}
								}

								if (NULL != pCosToCosMutationMapManagedObject) {
									cosToCosMutationMapObjId = pCosToCosMutationMapManagedObject->getObjectId();
									pQosProfileCfgManagedObject->setCosMutation(cosToCosMutationMapObjId);
								}
                                pQosProfileCfgManagedObject->addAttributeToBeUpdated("cosMutation");
							}
							break;

                        case QOS_NO_COS_TO_TRAFFIC_MAP:
                            pQosProfileCfgManagedObject->setCosTrafficClass(ObjectId::NullObjectId);
                            pQosProfileCfgManagedObject->addAttributeToBeUpdated("cosTrafficClass");
                            break;
						case QOS_COS_TO_TRAFFIC_MAP:
							{
								string cosToTrafficClassMapName;
								ObjectId cosToTrafficClassMapObjId = ObjectId::NullObjectId;
								CosToTrafficClassMapManagedObject *pCosToTrafficClassMapManagedObject = NULL;
								WaveManagedObjectSynchronousQueryContext trafficSyncQueryCtxt(CosToTrafficClassMapManagedObject::getClassName());

                                cosToTrafficClassMapName = pAPPMGlobalAPPMQosMessage->getCosToTrafficClassMap(); 

								trafficSyncQueryCtxt.addAndAttribute (new AttributeString (cosToTrafficClassMapName, "cosToTrafficClassMapName"));
                                trafficSyncQueryCtxt.addSelectField("objectId");
								pQosResults = querySynchronously (&trafficSyncQueryCtxt);

								if (NULL != pQosResults) 
								{
									UI32 numberOfResults = pQosResults->size ();
									if (0 == numberOfResults) {
										APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating cos_to_traffic_map objectId in qos-profile Managed object : Can't find object");
									}else if (1 == numberOfResults) {
										pCosToTrafficClassMapManagedObject = dynamic_cast<CosToTrafficClassMapManagedObject *>((*pQosResults)[0]);
										APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Updating cos_to_traffic_map objectId in qos-profile Managed object : Found Managed Object");
									} else {
										APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating cos_to_traffic_map objectId in qos-profile Managed object : Two or more objects exists");
									}
								}

								if (NULL != pCosToTrafficClassMapManagedObject) {
									cosToTrafficClassMapObjId = pCosToTrafficClassMapManagedObject->getObjectId();
									pQosProfileCfgManagedObject->setCosTrafficClass(cosToTrafficClassMapObjId);
								}
                                pQosProfileCfgManagedObject->addAttributeToBeUpdated("cosTrafficClass");
								break;
							}
						}
					}
				}
                break;

            case APPM_QOS_MSG:
                {
                    vector<WaveManagedObject *> *pQosProfileResults = NULL;
                    WaveManagedObjectSynchronousQueryContext QosProfilesyncQueryCtxt(QosProfileCfgManagedObject::getClassName());
                    unsigned int iter = 0;

                    switch(pAPPMGlobalAPPMQosMessage->getCmdCode())
                    {
                    case QOS_NO_COS_TO_TRAFFIC_MAP:
                        {
                            string cosToTrafficClassMapName;
                            vector<WaveManagedObject *> *pQosMapResults = NULL;
                            ObjectId cosToTrafficClassMapObjId = ObjectId::NullObjectId;
                            CosToTrafficClassMapManagedObject *pCosToTrafficClassMapManagedObject = NULL;
                            WaveManagedObjectSynchronousQueryContext qosTrafficSyncQueryCtxt(CosToTrafficClassMapManagedObject::getClassName());
                            cosToTrafficClassMapName = pAPPMGlobalAPPMQosMessage->getCosToTrafficClassMap(); 
                            qosTrafficSyncQueryCtxt.addAndAttribute (new AttributeString (cosToTrafficClassMapName, "cosToTrafficClassMapName"));
                            qosTrafficSyncQueryCtxt.addSelectField("objectId");
                            pQosMapResults = querySynchronously (&qosTrafficSyncQueryCtxt);
                            if (pQosMapResults && (pQosMapResults->size() > 0))
                            {
                                pCosToTrafficClassMapManagedObject = dynamic_cast<CosToTrafficClassMapManagedObject *>((*pQosMapResults)[0]);
                                prismAssert (NULL != pCosToTrafficClassMapManagedObject, __FILE__, __LINE__);
                                cosToTrafficClassMapObjId = pCosToTrafficClassMapManagedObject->getObjectId();

                                QosProfilesyncQueryCtxt.addAndAttribute (new AttributeObjectId (cosToTrafficClassMapObjId, "cosTrafficClass"));
                                QosProfilesyncQueryCtxt.addSelectField("cosTrafficClass");
                                pQosProfileResults = querySynchronously (&QosProfilesyncQueryCtxt);

                                for (iter = 0 ; iter < pQosProfileResults->size(); iter++)
                                {
                                    pQosProfileCfgManagedObject = dynamic_cast<QosProfileCfgManagedObject *>((*pQosProfileResults)[iter]);
                                    pQosProfileCfgManagedObject->setCosTrafficClass(ObjectId::NullObjectId);
                                    pQosProfileCfgManagedObject->addAttributeToBeUpdated("cosTrafficClass");
                                    updateWaveManagedObject(pQosProfileCfgManagedObject);
                                }

                                if (pQosProfileResults)
                                {
                                    pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pQosProfileResults);
                                    delete pQosProfileResults;
                                }

                                if (pQosMapResults)
                                {
                                    pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pQosMapResults);
                                    delete pQosMapResults;

                                }

                            } else {
                                if (pQosMapResults)
                                    delete pQosMapResults;
                            }
                        }
                        
                        break;
                        
                    case QOS_NO_COS_TO_COS_MUTATION:

                        {
                            string cosToCosMutationMapName;
                            vector<WaveManagedObject *> *pQosMapResults = NULL;
                            CosToCosMutationMapManagedObject *pCosToCosMutationMapManagedObject = NULL;
                            ObjectId cosToCosMutationMapObjId = ObjectId::NullObjectId;
                            WaveManagedObjectSynchronousQueryContext mutationSyncQueryCtxt(CosToCosMutationMapManagedObject::getClassName());
                            cosToCosMutationMapName = pAPPMGlobalAPPMQosMessage->getCosToCosMutationMap();
                            mutationSyncQueryCtxt.addAndAttribute (new AttributeString (cosToCosMutationMapName, "cosToCosMutationMapName"));
                            mutationSyncQueryCtxt.addSelectField("objectId");
                            pQosMapResults = querySynchronously (&mutationSyncQueryCtxt);
                            if (pQosMapResults && (pQosMapResults->size() > 0))
                            {
                                pCosToCosMutationMapManagedObject = dynamic_cast<CosToCosMutationMapManagedObject *>((*pQosMapResults)[0]);
                                prismAssert (NULL != pCosToCosMutationMapManagedObject, __FILE__, __LINE__);
                                cosToCosMutationMapObjId = pCosToCosMutationMapManagedObject->getObjectId();

                                QosProfilesyncQueryCtxt.addAndAttribute (new AttributeObjectId (cosToCosMutationMapObjId, "cosMutation"));
                                QosProfilesyncQueryCtxt.addSelectField("cosMutation");
                                pQosProfileResults = querySynchronously (&QosProfilesyncQueryCtxt);

                                for (iter = 0 ; iter < pQosProfileResults->size(); iter++)
                                {
                                    pQosProfileCfgManagedObject = dynamic_cast<QosProfileCfgManagedObject *>((*pQosProfileResults)[iter]);
                                    pQosProfileCfgManagedObject->setCosMutation(ObjectId::NullObjectId);
                                    pQosProfileCfgManagedObject->addAttributeToBeUpdated("cosMutation");
                                    updateWaveManagedObject(pQosProfileCfgManagedObject);
                                }

                                if (pQosProfileResults)
                                {
                                    pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pQosProfileResults);
                                    delete pQosProfileResults;
                                }

                                if (pQosMapResults)
                                {
                                    pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pQosMapResults);
                                    delete pQosMapResults;
                                }

                            } else {
                                if (pQosMapResults)
                                    delete pQosMapResults;
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                break;
        }

last:
        if (pResults != NULL) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }
        if (pQosResults != NULL) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pQosResults);
            delete pQosResults;
        }

        pPrismLinearSequencerContext->executeNextStep(status);
        return;
	}

    void  APPMGlobalConfWorker::APPMGlobalAPPMQosMessageHandler( APPMGlobalAPPMQosMessage *pAPPMGlobalAPPMQosMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::qosProfileValidationStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::qosMessageStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::qosMessageMoStep),

            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pAPPMGlobalAPPMQosMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

       pPrismLinearSequencerContext->holdAll();
       pPrismLinearSequencerContext->start ();

    }

    ResourceId APPMGlobalConfWorker::securityProfileValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		APPMGlobalAPPMSecurityMessage *pAPPMGlobalAPPMSecurityMessage =
			dynamic_cast<APPMGlobalAPPMSecurityMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

		string profileName("\0");
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEVEL);

        /* For Inter Plugin Messages, no need to do any validation.  */
        if ((pAPPMGlobalAPPMSecurityMessage->getOpCode()) == APPM_SSM_MSG)
        {
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return status;
        }

		profileName = pAPPMGlobalAPPMSecurityMessage->getProfileName();
		/* default security sub-profile validations */
		if (profileName.compare(DEFAULT_PORT_PROFILE_NAME) == 0) {
			status = APPM_ERR_DEFAULT_SEC_PROFILE_MODIFY;
		}
		/* check whether profile is activated */
		status = isProfileActivated(profileName, status, pPrismLinearSequencerContext) ? APPM_ERR_PROFILE_ACTIVE : status;

		pAPPMGlobalAPPMSecurityMessage->setCompletionStatus(status);
		pPrismLinearSequencerContext->executeNextStep(status);
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEVEL);
		return (status);
	}

    void APPMGlobalConfWorker::securityMessageStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		WaveSendToClusterContext    *send_ctx_p;
		bool flag=true;
		APPMGlobalAPPMSecurityMessage *pAPPMGlobalAPPMSecurityMessage = 
            dynamic_cast<APPMGlobalAPPMSecurityMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEVEL);

		ProfileManagedObject *pProfileManagedObject = NULL;
		string profileName = pAPPMGlobalAPPMSecurityMessage->getProfileName();
        string aclName;
        AppmReturnCode aclNameFetchStatus = APPM_SUCCESS;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
        
        /*
         * For Inter Plugin Messages, no need to goto backend,
         * as message sent by the calling plugin would have taken care of it.
         */
        if ((pAPPMGlobalAPPMSecurityMessage->getOpCode()) == APPM_SSM_MSG)
        {
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }

        APPMLocalAPPMSecurityMessage *pMsg = new APPMLocalAPPMSecurityMessage();
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
        syncQueryCtxt.addSelectField("profileId");
		syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) 
		{
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " profile MO is not found for " + profileName);
			}else if (1 == numberOfResults) {
				pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
				APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " Found profile MO for " + profileName);
			} else {
				APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Two or more objects exists");
			}
		}

		if (pProfileManagedObject == NULL) {
            status = WAVE_MESSAGE_ERROR;
			goto last;
		} else {
			prismAssert (NULL != pProfileManagedObject, __FILE__, __LINE__);
			pMsg->setProfileId(pProfileManagedObject->getProfileId());
		}

        if (pAPPMGlobalAPPMSecurityMessage->getCmdCode() == 
            MAC_ACCESS_GRP_DEL) {

            aclNameFetchStatus = pProfileManagedObject->getAclName( aclName, profileName );

            if (aclNameFetchStatus != APPM_SUCCESS)
            {
                pAPPMGlobalAPPMSecurityMessage->setCompletionStatus
                    (WRC_SSM_DCM_ERR_SEC_PROFILE_ACL_NOT_APPLIED);
                status = WRC_SSM_DCM_ERR_SEC_PROFILE_ACL_NOT_APPLIED;
                goto last;
            }
        } else {
            aclName = pAPPMGlobalAPPMSecurityMessage->getAccessGroup();
        }

        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " APPM SecurityProfile Message Step Acl: " + aclName);

        pMsg->setOpCode(pAPPMGlobalAPPMSecurityMessage->getOpCode());
        pMsg->setCmdCode(pAPPMGlobalAPPMSecurityMessage->getCmdCode());
		pMsg->setProfileName(pAPPMGlobalAPPMSecurityMessage->getProfileName());
		pMsg->setAccessGroup(aclName);

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback), pPrismLinearSequencerContext);
        prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(pMsg);
	    send_ctx_p->setPartialSuccessFlag(flag);
	    send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag); 
	    sendToWaveCluster(send_ctx_p);
last:
        if (pResults != NULL) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
		if (status != WAVE_MESSAGE_SUCCESS)
		{
	        pPrismLinearSequencerContext->executeNextStep(status);
			delete pMsg;
		}
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEVEL);
        return;
    }

    void APPMGlobalConfWorker::securityMessageMoStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        APPMGlobalAPPMSecurityMessage *pAPPMGlobalAPPMSecurityMessage =
            dynamic_cast<APPMGlobalAPPMSecurityMessage *> (pPrismLinearSequencerContext->getPPrismMessage());
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
		ProfileManagedObject *pProfileManagedObject = NULL;
		int ret = APPM_SUCCESS;
		UI8 cmdCode = 0;
		ResourceId status = WAVE_MESSAGE_SUCCESS;

        /*
         * For inter plugin messages only query & clean up has to be done.
         */
        if ((pAPPMGlobalAPPMSecurityMessage->getOpCode()) == APPM_SSM_MSG)
        {
            ResourceId removeAclStatus = WAVE_MESSAGE_SUCCESS; 
            removeAclStatus = secProfileRemoveAcl(pPrismLinearSequencerContext);
            pPrismLinearSequencerContext->executeNextStep(removeAclStatus);
            return;
        }

        string profileName = pAPPMGlobalAPPMSecurityMessage->getProfileName();
		syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));

        //Load only the sub profile vector field of ProfileManagedObject.
        syncQueryCtxt.addSelectField("subProfileCfg");

		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) 
		{
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " SET_ELEM: Could not" 
					" find the profile MO with key: " + profileName);
	            status = WAVE_MESSAGE_ERROR;
				goto last;
			}else if (1 == numberOfResults) {
				pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
				APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " SET_ELEM: Found the" 
					" profile MO with key: " + profileName);
			} else {
				APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " SET_ELEM:" 
					" Two or more profile MO's exists with key: " + profileName);
	            status = WAVE_MESSAGE_ERROR;
				goto last;
			}
		}

		if (pProfileManagedObject == NULL) {
			goto last;
		}

		ret = pProfileManagedObject->updateSecSubProfileInfo(pPrismLinearSequencerContext, pAPPMGlobalAPPMSecurityMessage);
		if (ret != APPM_SUCCESS) {
            status = WAVE_MESSAGE_ERROR;
			goto last;
		}
		cmdCode = pAPPMGlobalAPPMSecurityMessage->getCmdCode();
        
        // In case of addition and deletion of security profile from ProfileManagedObject, update the sub profile vector of ProfileManagedObject and update the ProfileManagedObject in DB.
        if ((cmdCode == SEC_PROFILE_CREATE) || (cmdCode == SEC_PROFILE_DELETE)) {

            pProfileManagedObject->addAttributeToBeUpdated("subProfileCfg");
			updateWaveManagedObject(pProfileManagedObject);
		}

        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " EXIT");
last:
        if(pResults != NULL) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			delete (pResults);
        }

        pPrismLinearSequencerContext->executeNextStep(status);
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEVEL);
        return;
	}
    
    ResourceId APPMGlobalConfWorker::secProfileRemoveAcl(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        APPMGlobalAPPMSecurityMessage *pAPPMGlobalAPPMSecurityMessage =
            dynamic_cast<APPMGlobalAPPMSecurityMessage *> (pPrismLinearSequencerContext->getPPrismMessage());
        
        string aclName = pAPPMGlobalAPPMSecurityMessage->getAccessGroup();
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObject   *pWaveManagedObject = NULL;
        SecProfileCfgManagedObject *pSecProfileCfgManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SecProfileCfgManagedObject::getClassName());
        //Partial Query to load only the needed attribute.
        syncQueryCtxt.addSelectField("ownerManagedObjectId");

        ProfileManagedObject *pProfileManagedObject = NULL;
        ObjectId ProfileObjId = ObjectId::NullObjectId;
        unsigned iter = 0;
        UI32 retval = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        syncQueryCtxt.addAndAttribute (new AttributeString (aclName, "accessGroup"));
        pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size())
        {
            for(iter = 0; iter < pResults->size(); iter++)
            {

                pSecProfileCfgManagedObject = dynamic_cast<SecProfileCfgManagedObject *>
                    ((*pResults)[iter]);
                prismAssert (NULL != pSecProfileCfgManagedObject, __FILE__, __LINE__);

                /*
                 * Get the corresponding port-profile managed object.
                 */

                ProfileObjId = pSecProfileCfgManagedObject->getOwnerManagedObjectId();
                pWaveManagedObject = queryManagedObject (ProfileObjId);
                pProfileManagedObject = dynamic_cast<ProfileManagedObject *>(pWaveManagedObject);

                //Delete the security profile from the sub-profile vector and update the profile managed object.
                retval = pProfileManagedObject->delSubProfileFromVector("security-profile");
                if(0 == retval) {
                    APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "security sub-profile is not found");
                    pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pWaveManagedObject);
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }
                pProfileManagedObject->addAttributeToBeUpdated("subProfileCfg");
                updateWaveManagedObject(pProfileManagedObject);

                pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pWaveManagedObject);
            }
        }

last:	if (pResults)
        {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }

        return status;
    }

    void  APPMGlobalConfWorker::APPMGlobalAPPMSecurityMessageHandler( APPMGlobalAPPMSecurityMessage *pAPPMGlobalAPPMSecurityMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
		    reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::securityProfileValidationStep),
		    reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::securityMessageStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here
		    reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::securityMessageMoStep),

            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext
			(pAPPMGlobalAPPMSecurityMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

       pPrismLinearSequencerContext->holdAll();
       pPrismLinearSequencerContext->start ();
    }

	bool APPMGlobalConfWorker::isProfileActivated(string profileName, ResourceId &status, PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
        bool isProfileActive = false;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
        syncQueryCtxt.addSelectField("isActivated");

        syncQueryCtxt.addSelectField("profileId");

		syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		ProfileManagedObject *pProfileManagedObject = NULL;

		if (NULL != pResults) 
		{
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " MO is not found for " + profileName);
			}else if (1 == numberOfResults) {
				pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
				APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " Found MO for " + profileName);
			} else {
				APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Two or more objects exists");
			}
		}

		if (pProfileManagedObject == NULL) {
			status = WAVE_MESSAGE_ERROR;
			return (false);
		}

		isProfileActive = pProfileManagedObject->getIsActivated();
       
        DceLinearSeqContext *dceLinearContext = dynamic_cast<DceLinearSeqContext *> (pPrismLinearSequencerContext);
        //Save the ProfileManagedObject in DceLinearContext.
        if ( NULL != dceLinearContext ) {
            dceLinearContext->setWaveManagedObjectId(pProfileManagedObject->getObjectId());
            dceLinearContext->setPWaveManagedObject(pProfileManagedObject);
        }

        if((NULL != pPrismLinearSequencerContext) && ( pResults != NULL)) {
            //Retain the object till the end of sequencer steps, because the same object is used in Mac association MO step.
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }
        return isProfileActive;
	}


    ResourceId APPMGlobalConfWorker::vlanProfileValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext) {

		ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 vlanId;
        bool vlanClassificationNeeded = false;
        UI32 macGroupId = 0;

		APPMGlobalAPPMVlanMessage *pAPPMGlobalAPPMVlanMessage =
			dynamic_cast<APPMGlobalAPPMVlanMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

		string profileName("\0");
		profileName = pAPPMGlobalAPPMVlanMessage->getProfileName();
        macGroupId = pAPPMGlobalAPPMVlanMessage->getMacGroup();
		/* default vlan sub-profile validations */
		if (profileName.compare(DEFAULT_PORT_PROFILE_NAME) == 0) {
			switch(pAPPMGlobalAPPMVlanMessage->getOpCode()) {
			case APPM_CREATE:
				break;
			case APPM_REMOVE:
				status = APPM_ERR_DEFAULT_VLAN_PROFILE_MODIFY;
				break;
			case APPM_SET_ELEM:
				switch(pAPPMGlobalAPPMVlanMessage->getCmdCode()) {
				case VLAN_SW_MODE:
					/* Should be changed when native vlan support in place */
					if (pAPPMGlobalAPPMVlanMessage->getSwMode() != VLAN_SW_MODE_TRUNK) {
						status = APPM_ERR_DEFAULT_VLAN_PROFILE_MODIFY;
					}
					break;
				case VLAN_SW_MODE_FLAG_DEL:
					status = APPM_ERR_DEFAULT_VLAN_PROFILE_MODIFY;
					break;
				case VLAN_IS_ALLOWED_VLAN_NONE:
				case VLAN_IS_ALLOWED_VLAN_ADD:
				case VLAN_IS_ALLOWED_VLAN_REMOVE: 
				case VLAN_IS_ALLOWED_VLAN_EXCEPT:
				case VLAN_GROUP_ID:
				case VLAN_ACCESS_VLAN:
				case VLAN_NATIVE_ID_ADD:
                case APPM_VLAN_CLASSIFICATION_SET_MAC_ADDR:
                case APPM_VLAN_CLASSIFICATION_DEL_MAC_ADDR:
                case APPM_VLAN_CLASSIFICATION_SET_MAC_GROUP:
                case APPM_VLAN_CLASSIFICATION_DEL_MAC_GROUP:
                case APPM_VLAN_CLASSIFICATION_SET_CTAG:
                case APPM_VLAN_CLASSIFICATION_DEL_CTAG:
                case APPM_VLAN_CLASSIFICATION_SET_NATIVE_CTAG:
                case APPM_VLAN_CLASSIFICATION_DEL_NATIVE_CTAG: {
					status = APPM_ERR_DEFAULT_VLAN_PROFILE_MODIFY;
					break;
                }
				}
				break;
			}
			/* Skip further validations in case of default profile */
			goto executeNext; 
		}

		APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, 
			   string(" pAPPMGlobalAPPMVlanMessage->getIsChangeVlanList() is : ") + 
			   pAPPMGlobalAPPMVlanMessage->getIsChangeVlanList());

		/* check whether profile is activated */
		if(pAPPMGlobalAPPMVlanMessage->getCmdCode() != VLAN_NSM_INTF_DELETE)
			status = isProfileActivated(profileName, status, pPrismLinearSequencerContext) ? APPM_ERR_PROFILE_ACTIVE : status;

		 vlanId = pAPPMGlobalAPPMVlanMessage->getNativeVlanId();
		/*Warning : Allowing vlan 0 to passthrough so that native vlan can be disabled need toRevisit*/

		if(WAVE_MESSAGE_SUCCESS == status 
            && (VLAN_NATIVE_ID_ADD == pAPPMGlobalAPPMVlanMessage->getCmdCode() 
                    || VCENTER_TRUNK_VLAN_PROFILE_NATIVE_VLAN == pAPPMGlobalAPPMVlanMessage->getCmdCode()) 
            && (0 != vlanId) ) {

            UI32 count = 0;
            WaveManagedObjectSynchronousQueryContext vlan_syncQueryCtxt(VlanIntfManagedObject::getClassName());
			vlan_syncQueryCtxt.addAndAttribute (new AttributeUI32(vlanId,"id"));
            status = querySynchronouslyForCount (&vlan_syncQueryCtxt,count);

			if ( count != 1)
				status = WRC_NSM_API_ERR_VLAN_NOT_CONFIGURED;
			/*check sw mode*/
			WaveManagedObjectSynchronousQueryContext appm_syncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
            appm_syncQueryCtxt.addSelectField("swModeFlag");
            appm_syncQueryCtxt.addSelectField("swMode");
            appm_syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
			vector<WaveManagedObject *> *pResults1 = querySynchronously (&appm_syncQueryCtxt);
            
            if(pResults1)
			{	
				if(1 == pResults1->size())
				{
					VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject*>((*pResults1)[0]);
					if(false == pVlanProfileCfgManagedObject->getSwModeFlag())
					{
						APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string("port-profile ") + profileName + string("not in sw mode ") );
						status = WRC_NSM_ERR_DCM_APPM_VLAN_PROFILE_MODE_INVALID;

					}
					else if(VLAN_SW_MODE_TRUNK != pVlanProfileCfgManagedObject->getSwMode())
					{
						APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string("port-profile ") + profileName + string("not in trunk mode") );
						status =  NSM_ERR_DCM_APPM_VLAN_PROFILE_TRUNK_MODE;
					}		
				}
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults1);
			}	
		}

        vlanId = APPM_INVALID_VLAN;

        switch(pAPPMGlobalAPPMVlanMessage->getCmdCode())
        {
            case VLAN_ACCESS_VLAN:
                {
                    vlanId = pAPPMGlobalAPPMVlanMessage->getAccessVlan();
                    if( (!(ProvisioningUtil::isGvlanEnabled())) && (vlanId > APPM_MAX_CTAG_ID) ) {
                        status = WRC_APPM_NOT_DOT1Q_VLANS_CANNOT_BE_USED_WHEN_VF_IS_DISABLED;
                        goto executeNext;
                    }
                    break;
                }
            case VLAN_NATIVE_ID_ADD:
                {
                    vlanId = pAPPMGlobalAPPMVlanMessage->getNativeVlanId();
                    if(vlanId > APPM_MAX_CTAG_ID) {
                        status = WRC_APPM_VLAN_NEEDS_CLASSIFICATION;
                        goto executeNext;
                    }
                    break;
                }

            //Validate the existence of the vlan 
            case APPM_VLAN_CLASSIFICATION_SET_MAC_ADDR:
            {
                if( !(ProvisioningUtil::isGvlanEnabled()) ) {
                    status = WRC_APPM_SWITCHPORT_VLAN_CLASSIFICATIONS_NOT_SUPPORTED_WHEN_VF_DISABLED;
                    goto executeNext;
                }
                vlanId = pAPPMGlobalAPPMVlanMessage->getAccessVlan();
                //mac based classification can be present on the access vlans < 4k and also > 4k.
                break;
            }
            case APPM_VLAN_CLASSIFICATION_SET_MAC_GROUP:
            {
                if( !(ProvisioningUtil::isGvlanEnabled()) ) {
                    status = WRC_APPM_SWITCHPORT_VLAN_CLASSIFICATIONS_NOT_SUPPORTED_WHEN_VF_DISABLED;
                    goto executeNext;
                }
                vlanId = pAPPMGlobalAPPMVlanMessage->getAccessVlan();
                //mac based classification can be present on the access vlans < 4k and also > 4k.
                status = APPMUtils::isMacGroupPresent(macGroupId);
                if(WAVE_MESSAGE_SUCCESS != status) {
                    goto executeNext;
                }
                break;
            }
            case APPM_VLAN_CLASSIFICATION_SET_CTAG:
            {
                if( !(ProvisioningUtil::isGvlanEnabled()) ) {
                    status = WRC_APPM_SWITCHPORT_VLAN_CLASSIFICATIONS_NOT_SUPPORTED_WHEN_VF_DISABLED;
                    goto executeNext;
                }
                UI32 trunkVlanId = pAPPMGlobalAPPMVlanMessage->getTrunkVlanId();
                UI32 ctagId = pAPPMGlobalAPPMVlanMessage->getCtag();
                status = APPMUtils::doesCtagOrTrunkVlanAlreadyExist(profileName, trunkVlanId, ctagId);

                if(WAVE_MESSAGE_SUCCESS != status) {
                    goto executeNext;
                }

                vlanId = pAPPMGlobalAPPMVlanMessage->getTrunkVlanId();
                vlanClassificationNeeded = true;
                break;
            }
            case APPM_VLAN_CLASSIFICATION_SET_NATIVE_CTAG:
            {
                if( !(ProvisioningUtil::isGvlanEnabled()) ) {
                    status = WRC_APPM_SWITCHPORT_VLAN_CLASSIFICATIONS_NOT_SUPPORTED_WHEN_VF_DISABLED;
                    goto executeNext;
                }
                vlanId = pAPPMGlobalAPPMVlanMessage->getNativeVlanId();
                vlanClassificationNeeded = true;
                break;
            }
            default :
            {
                break;
            }
        }

        if (APPM_INVALID_VLAN != vlanId) {
            status = APPMUtils::isVlanPresent(vlanId);
            if (WAVE_MESSAGE_SUCCESS != status) {
                goto executeNext;
            }
            if(vlanClassificationNeeded) {
                status = APPMUtils::isVlanValidForClassification(vlanId);
                if (WAVE_MESSAGE_SUCCESS != status) {
                    goto executeNext;
                }
            }
        }

executeNext:

		pAPPMGlobalAPPMVlanMessage->setCompletionStatus(status);
		pPrismLinearSequencerContext->executeNextStep(status);
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEVEL);
		return (status);
	}

    void APPMGlobalConfWorker::vlanMessageStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		WaveSendToClusterContext    *send_ctx_p;
        WaveSendToClusterContext    *send_ctx_p2;
        WaveSendToClusterContext    *send_ctx_p3;
        WaveSendToClusterContext    *send_ctx_p4;

		bool flag=true;
		string profileName("\0");
		APPMGlobalAPPMVlanMessage *pAPPMGlobalAPPMVlanMessage = 
            dynamic_cast<APPMGlobalAPPMVlanMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

		profileName = pAPPMGlobalAPPMVlanMessage->getProfileName();
		/* allowed vlan all, can fall in to this. Custom CLI */
		if (profileName.compare(DEFAULT_PORT_PROFILE_NAME) == 0) {
			APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("default profile, opCode: ") + 
				pAPPMGlobalAPPMVlanMessage->getOpCode() + string (" return success"));
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
			return;
		}

        APPMLocalAPPMVlanMessage *m = new APPMLocalAPPMVlanMessage();
		if(pAPPMGlobalAPPMVlanMessage->getOpCode() != APPM_VCENTER)
        {
        m->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
        m->setOpCode(pAPPMGlobalAPPMVlanMessage->getOpCode());
        m->setCmdCode(pAPPMGlobalAPPMVlanMessage->getCmdCode());
		m->setSwModeFlag(pAPPMGlobalAPPMVlanMessage->getSwModeFlag());
		m->setSwMode((VlanSwMode)pAPPMGlobalAPPMVlanMessage->getSwMode());
		m->setIsAllowedVlanAll(pAPPMGlobalAPPMVlanMessage->getIsAllowedVlanAll());
		m->setIsAllowedVlanNone(pAPPMGlobalAPPMVlanMessage->getIsAllowedVlanNone());
		m->setAccessVlan(pAPPMGlobalAPPMVlanMessage->getAccessVlan());
		m->setVlanRange(pAPPMGlobalAPPMVlanMessage->getVlanRange());
		m->setVlanClassifierGroupId(pAPPMGlobalAPPMVlanMessage->getVlanClassifierGroupId());
		m->setVlanClassifierVlan(pAPPMGlobalAPPMVlanMessage->getVlanClassifierVlan());
        m->setNativeVlanId(pAPPMGlobalAPPMVlanMessage->getNativeVlanId());
        m->setTrunkVlanId(pAPPMGlobalAPPMVlanMessage->getTrunkVlanId());
        m->setVlanClassificationType(pAPPMGlobalAPPMVlanMessage->getVlanClassificationType());
        m->setCtag(pAPPMGlobalAPPMVlanMessage->getCtag());
        m->setMac(pAPPMGlobalAPPMVlanMessage->getMac());
        m->setMacGroup(pAPPMGlobalAPPMVlanMessage->getMacGroup());
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("\t Native Vlan Id :: ") + pAPPMGlobalAPPMVlanMessage->getNativeVlanId());

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback), pPrismLinearSequencerContext);
        prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);
	    send_ctx_p->setPartialSuccessFlag(flag);
	    send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag); 
	    sendToWaveCluster(send_ctx_p);
		return;
    }
	else
	{
		trace (TRACE_LEVEL_INFO, "OPCODE: APPM_VCENTER....." );
          switch (pAPPMGlobalAPPMVlanMessage->getCmdCode())
            {
              case VCENTER_ACCESS_VLAN_PROFILE_CREATE:
                {

                  trace (TRACE_LEVEL_INFO, "Command CODE: VCENTER_ACCESS_VLAN_PROFILE_CREATE....." );
                m->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m->setOpCode(APPM_CREATE);
                m->setCmdCode(VLAN_PROFILE_CREATE);
                send_ctx_p = new WaveSendToClusterContext(this,reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackWithoutNextstep), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
                send_ctx_p->setPPrismMessageForPhase1(m);
                send_ctx_p->setPartialSuccessFlag(flag);
                send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                sendToWaveCluster(send_ctx_p);

                APPMLocalAPPMVlanMessage *m2 = new APPMLocalAPPMVlanMessage();
                m2->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m2->setOpCode(APPM_SET_ELEM);
                m2->setCmdCode(VLAN_SW_MODE_FLAG);
                m2->setSwModeFlag(true);
                m2->setSwMode(VLAN_SW_MODE_ACCESS);
                send_ctx_p2 = new WaveSendToClusterContext(this,reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackWithoutNextstep), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p2, __FILE__, __LINE__);
                send_ctx_p2->setPPrismMessageForPhase1(m2);
                send_ctx_p2->setPartialSuccessFlag(flag);
                send_ctx_p2->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                sendToWaveCluster(send_ctx_p2);

                APPMLocalAPPMVlanMessage *m3 = new APPMLocalAPPMVlanMessage();
                m3->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m3->setOpCode(APPM_SET_ELEM);
                m3->setCmdCode(VLAN_ACCESS_VLAN);
                m3->setAccessVlan(pAPPMGlobalAPPMVlanMessage->getAccessVlan());
                trace (TRACE_LEVEL_INFO, "sendToWaveCluster....for created AccessVlan" );
                send_ctx_p3 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p3, __FILE__, __LINE__);
                send_ctx_p3->setPPrismMessageForPhase1(m3);
                send_ctx_p3->setPartialSuccessFlag(flag);
                send_ctx_p3->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                sendToWaveCluster(send_ctx_p3);

                return;
            }
            case VCENTER_ACCESS_VLAN_PROFILE_UPDATE:
            {
            //  APPMLocalAPPMVlanMessage *m = new APPMLocalAPPMVlanMessage();
                m->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m->setOpCode(APPM_SET_ELEM);
                m->setCmdCode(VLAN_ACCESS_VLAN);
                m->setAccessVlan(pAPPMGlobalAPPMVlanMessage->getAccessVlan());
				                send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
                send_ctx_p->setPPrismMessageForPhase1(m);
                send_ctx_p->setPartialSuccessFlag(flag);
                send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                sendToWaveCluster(send_ctx_p);
                return;
            }
			break;
			case VCENTER_TRUNK_VLAN_PROFILE_NATIVE_VLAN:
			{
                m->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m->setOpCode(APPM_SET_ELEM);
                m->setCmdCode(VLAN_NATIVE_ID_ADD);
				m->setNativeVlanId(pAPPMGlobalAPPMVlanMessage->getNativeVlanId());
                send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
                send_ctx_p->setPPrismMessageForPhase1(m);
                send_ctx_p->setPartialSuccessFlag(flag);
                send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                trace (TRACE_LEVEL_INFO, "VCENTER_TRUNK_VLAN_PROFILE_NATIVE_VLAN: sendToWaveCluster message" );
                sendToWaveCluster(send_ctx_p);
                return;

			}
			break;
            case VCENTER_TRUNK_VLAN_PROFILE_CREATE:
            {
                //APPMLocalAPPMVlanMessage *m = new APPMLocalAPPMVlanMessage();
                m->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m->setOpCode(APPM_CREATE);
                m->setCmdCode(VLAN_PROFILE_CREATE);
                send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackWithoutNextstep), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
                send_ctx_p->setPPrismMessageForPhase1(m);
                send_ctx_p->setPartialSuccessFlag(flag);
                send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag);
				trace (TRACE_LEVEL_INFO, "VCENTER_TRUNK_VLAN_PROFILE_CREATE: sendToWaveCluster message1" );
                sendToWaveCluster(send_ctx_p);

                APPMLocalAPPMVlanMessage *m2 = new APPMLocalAPPMVlanMessage();
                m2->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m2->setOpCode(APPM_SET_ELEM);
                m2->setCmdCode(VLAN_SW_MODE_FLAG);
                m2->setSwModeFlag(true);
                m2->setSwMode(VLAN_SW_MODE_TRUNK);
                send_ctx_p2 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackWithoutNextstep), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p2, __FILE__, __LINE__);
                send_ctx_p2->setPPrismMessageForPhase1(m2);
                send_ctx_p2->setPartialSuccessFlag(flag);
                send_ctx_p2->setTreatFailureOnFailingOverAsSuccessFlag (flag);
				trace (TRACE_LEVEL_INFO, "VCENTER_TRUNK_VLAN_PROFILE_CREATE: sendToWaveCluster message2" );
                sendToWaveCluster(send_ctx_p2);

				APPMLocalAPPMVlanMessage *m3 = new APPMLocalAPPMVlanMessage();
				m3->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
				m3->setOpCode(APPM_SET_ELEM);
				m3->setCmdCode(VLAN_SW_MODE);
				m3->setSwModeFlag(true);
				m3->setSwMode(VLAN_SW_MODE_TRUNK);
				send_ctx_p3 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackWithoutNextstep), pPrismLinearSequencerContext);
				prismAssert(NULL != send_ctx_p3, __FILE__, __LINE__);
                send_ctx_p3->setPPrismMessageForPhase1(m3);
                send_ctx_p3->setPartialSuccessFlag(flag);
                send_ctx_p3->setTreatFailureOnFailingOverAsSuccessFlag (flag);
				trace (TRACE_LEVEL_INFO, "VCENTER_TRUNK_VLAN_PROFILE_CREATE: sendToWaveCluster message3" );
				sendToWaveCluster(send_ctx_p3);

                APPMLocalAPPMVlanMessage *m4 = new APPMLocalAPPMVlanMessage();
                m4->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m4->setOpCode(APPM_SET_ELEM);
                m4->setCmdCode(VLAN_IS_ALLOWED_VLAN_ADD);
                m4->setVlanRange(pAPPMGlobalAPPMVlanMessage->getVlanRange());
                send_ctx_p4 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p4, __FILE__, __LINE__);
                send_ctx_p4->setPPrismMessageForPhase1(m4);
                send_ctx_p4->setPartialSuccessFlag(flag);
                send_ctx_p4->setTreatFailureOnFailingOverAsSuccessFlag (flag);
				trace (TRACE_LEVEL_INFO, "VCENTER_TRUNK_VLAN_PROFILE_CREATE: sendToWaveCluster message4" );
                sendToWaveCluster(send_ctx_p4);
                return;
            }
			break;
            case VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_ALL:
            {
                m->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m->setOpCode(APPM_CREATE);
                m->setCmdCode(VLAN_PROFILE_CREATE);
                send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackWithoutNextstep), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
                send_ctx_p->setPPrismMessageForPhase1(m);
                send_ctx_p->setPartialSuccessFlag(flag);
                send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                trace (TRACE_LEVEL_INFO, "VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_ALL: sendToWaveCluster message1" );
                sendToWaveCluster(send_ctx_p);

                APPMLocalAPPMVlanMessage *m2 = new APPMLocalAPPMVlanMessage();
                m2->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m2->setOpCode(APPM_SET_ELEM);
                m2->setCmdCode(VLAN_SW_MODE_FLAG);
                m2->setSwModeFlag(true);
                m2->setSwMode(VLAN_SW_MODE_TRUNK);
                send_ctx_p2 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackWithoutNextstep), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p2, __FILE__, __LINE__);
                send_ctx_p2->setPPrismMessageForPhase1(m2);
                send_ctx_p2->setPartialSuccessFlag(flag);
                send_ctx_p2->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                trace (TRACE_LEVEL_INFO, "VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_ALL: sendToWaveCluster message2" );
                sendToWaveCluster(send_ctx_p2);

                APPMLocalAPPMVlanMessage *m3 = new APPMLocalAPPMVlanMessage();
                m3->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m3->setOpCode(APPM_SET_ELEM);
                m3->setCmdCode(VLAN_SW_MODE);
                m3->setSwModeFlag(true);
                m3->setSwMode(VLAN_SW_MODE_TRUNK);
                send_ctx_p3 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackWithoutNextstep), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p3, __FILE__, __LINE__);
                send_ctx_p3->setPPrismMessageForPhase1(m3);
                send_ctx_p3->setPartialSuccessFlag(flag);
                send_ctx_p3->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                trace (TRACE_LEVEL_INFO, "VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_ALL: sendToWaveCluster message3" );
                sendToWaveCluster(send_ctx_p3);

                APPMLocalAPPMVlanMessage *m4 = new APPMLocalAPPMVlanMessage();
                m4->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m4->setOpCode(APPM_SET_ELEM);
                m4->setCmdCode(VLAN_IS_ALLOWED_VLAN_ALL);
				m4->setIsAllowedVlanAll(pAPPMGlobalAPPMVlanMessage->getIsAllowedVlanAll());
                send_ctx_p4 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p4, __FILE__, __LINE__);
                send_ctx_p4->setPPrismMessageForPhase1(m4);
                send_ctx_p4->setPartialSuccessFlag(flag);
                send_ctx_p4->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                trace (TRACE_LEVEL_INFO, "VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_ALL: sendToWaveCluster message4" );
                sendToWaveCluster(send_ctx_p4);
                return;
            }
			break;
            case VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_NONE:
            {
                m->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m->setOpCode(APPM_CREATE);
                m->setCmdCode(VLAN_PROFILE_CREATE);
                send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackWithoutNextstep), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
                send_ctx_p->setPPrismMessageForPhase1(m);
                send_ctx_p->setPartialSuccessFlag(flag);
                send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                trace (TRACE_LEVEL_INFO, "VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_NONE: sendToWaveCluster message1" );
                sendToWaveCluster(send_ctx_p);

                APPMLocalAPPMVlanMessage *m2 = new APPMLocalAPPMVlanMessage();
                m2->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m2->setOpCode(APPM_SET_ELEM);
                m2->setCmdCode(VLAN_SW_MODE_FLAG);
                m2->setSwModeFlag(true);
                m2->setSwMode(VLAN_SW_MODE_TRUNK);
                send_ctx_p2 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackWithoutNextstep), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p2, __FILE__, __LINE__);
                send_ctx_p2->setPPrismMessageForPhase1(m2);
                send_ctx_p2->setPartialSuccessFlag(flag);
                send_ctx_p2->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                trace (TRACE_LEVEL_INFO, "VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_NONE: sendToWaveCluster message2" );
                sendToWaveCluster(send_ctx_p2);

                APPMLocalAPPMVlanMessage *m3 = new APPMLocalAPPMVlanMessage();
                m3->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m3->setOpCode(APPM_SET_ELEM);
                m3->setCmdCode(VLAN_SW_MODE);
                m3->setSwModeFlag(true);
                m3->setSwMode(VLAN_SW_MODE_TRUNK);
                send_ctx_p3 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallbackWithoutNextstep), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p3, __FILE__, __LINE__);
                send_ctx_p3->setPPrismMessageForPhase1(m3);
                send_ctx_p3->setPartialSuccessFlag(flag);
                send_ctx_p3->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                trace (TRACE_LEVEL_INFO, "VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_NONE: sendToWaveCluster message3" );
                sendToWaveCluster(send_ctx_p3);

                APPMLocalAPPMVlanMessage *m4 = new APPMLocalAPPMVlanMessage();
                m4->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m4->setOpCode(APPM_SET_ELEM);
                m4->setCmdCode(VLAN_IS_ALLOWED_VLAN_NONE);
                m4->setIsAllowedVlanNone(pAPPMGlobalAPPMVlanMessage->getIsAllowedVlanNone());
                send_ctx_p4 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p4, __FILE__, __LINE__);
                send_ctx_p4->setPPrismMessageForPhase1(m4);
                send_ctx_p4->setPartialSuccessFlag(flag);
                send_ctx_p4->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                trace (TRACE_LEVEL_INFO, "VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_NONE: sendToWaveCluster message4" );
                sendToWaveCluster(send_ctx_p4);
                return;
            }
            break;
            case VCENTER_TRUNK_VLAN_PROFILE_UPDATE:
            {
                APPMLocalAPPMVlanMessage *m3 = new APPMLocalAPPMVlanMessage();
                m3->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m3->setOpCode(APPM_SET_ELEM);
                m3->setCmdCode(VLAN_IS_ALLOWED_VLAN_ADD);
				m3->setVlanRange(pAPPMGlobalAPPMVlanMessage->getVlanRange());
                send_ctx_p3 = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p3, __FILE__, __LINE__);
                send_ctx_p3->setPPrismMessageForPhase1(m3);
                send_ctx_p3->setPartialSuccessFlag(flag);
                send_ctx_p3->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                sendToWaveCluster(send_ctx_p3);
                return;
            }
			break;
            case VCENTER_VLAN_PROFILE_DELETE:
            {
                //APPMLocalAPPMVlanMessage *m = new APPMLocalAPPMVlanMessage();
                trace (TRACE_LEVEL_INFO, "OPCODE: VCENTER_VLAN_PROFILE_DELETE....." );
                m->setProfileName(pAPPMGlobalAPPMVlanMessage->getProfileName());
                m->setOpCode(APPM_SET_ELEM);
                m->setCmdCode(VLAN_PROFILE_DEL);
                send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&APPMGlobalConfWorker::APPMGlobalConfSendToClusterCallback), pPrismLinearSequencerContext);
                prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
                send_ctx_p->setPPrismMessageForPhase1(m);
                send_ctx_p->setPartialSuccessFlag(flag);
                send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (flag);
                sendToWaveCluster(send_ctx_p);
                return;
            }
          }

	}
}


    void APPMGlobalConfWorker::vlanMessageMoStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{

		APPMGlobalAPPMVlanMessage *pAPPMGlobalAPPMVlanMessage =
			dynamic_cast<APPMGlobalAPPMVlanMessage *> (pPrismLinearSequencerContext->getPPrismMessage());
        AppmReturnCode ret = APPM_SUCCESS;
        vector<WaveManagedObject *> *pResults = NULL;
		vector<WaveManagedObject *> *pResultsprofile = NULL;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		string profileName("\0");
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEVEL);
		int retval;
		unsigned int vlanMoCount = 0;


		profileName = pAPPMGlobalAPPMVlanMessage->getProfileName();
		/* allowed vlan all, can fall in to this. Custom CLI */
		if (profileName.compare(DEFAULT_PORT_PROFILE_NAME) == 0) {
			APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("default profile, opCode: ") + 
				pAPPMGlobalAPPMVlanMessage->getOpCode() + string (" return success"));
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
			return;
		}

		switch(pAPPMGlobalAPPMVlanMessage->getOpCode())
		{
		case APPM_CREATE:
			{

				WaveManagedObjectSynchronousQueryContext validateSyncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
				validateSyncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
				status = querySynchronouslyForCount (&validateSyncQueryCtxt, vlanMoCount);

				if (WAVE_MESSAGE_SUCCESS == status) {
					if (0 == vlanMoCount) {
						APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " Validation successful " 
							" No VlanProfile Managed object exists with key: " + profileName);
					} else  {
						APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation failure " 
							"Already existing VlanProfile Managed Object with key: " + profileName);
						goto last;
					}
				} else {
					APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation not completed " 
						"  Quering DB is failed");
					goto last;
				}

				
                ProfileManagedObject *pProfileManagedObject = NULL;
                WaveManagedObjectSynchronousQueryContext profileQueryCtxt
                    (ProfileManagedObject::getClassName());
                profileQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));

                // Query the existing ProfileManagedObject partially to load the subprofile vector and add the VlanProfileManagedObject to the subprofile vector. And update the ProfileManagedObject in DB.

                profileQueryCtxt.addSelectField("subProfileCfg");
                pResults = querySynchronously (&profileQueryCtxt);

                if ( pResults && (pResults->size())) {
                    pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
                } else {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }

                if( NULL == pProfileManagedObject ) {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }

				VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject =
					new VlanProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

				prismAssert (NULL != pVlanProfileCfgManagedObject, __FILE__, __LINE__);

                APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Creating vlan-profile Managed Object");
				pVlanProfileCfgManagedObject->setDescription(pAPPMGlobalAPPMVlanMessage->getProfileName());
				pVlanProfileCfgManagedObject->setSwModeFlag(false);
                pVlanProfileCfgManagedObject->setIsAllowedVlanAll(false);
                pVlanProfileCfgManagedObject->setIsAllowedVlanAllV2(false);
				pVlanProfileCfgManagedObject->setIsAllowedVlanNone(false);
				pVlanProfileCfgManagedObject->setName(string("vlan-profile"));
				APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("\t Native Vlan Id :: ") + pAPPMGlobalAPPMVlanMessage->getNativeVlanId());
				APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject  = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
				pAPPMNativeVlanManagedObject->setNativeVlanId(APPM_NSM_NATIVE_VLAN_DEFAULT);
				pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                pProfileManagedObject->addSubProfileToVector 
                    ((SubProfileManagedObject *) pVlanProfileCfgManagedObject);
                pProfileManagedObject->addAttributeToBeUpdated("subProfileCfg");
				updateWaveManagedObject(pProfileManagedObject);
				break;
			}
		        case APPM_VCENTER:
            {
            trace (TRACE_LEVEL_INFO, "vlanMessageMoStep OPCODE: APPM_VCENTER" );
              switch(pAPPMGlobalAPPMVlanMessage->getCmdCode())
              {
                case VCENTER_ACCESS_VLAN_PROFILE_CREATE:
                {
                trace (TRACE_LEVEL_INFO, "vlanMessageMoStep CMCODE: VCENTER_ACCESS_VLAN_PROFILE_CREATE" );
                WaveManagedObjectSynchronousQueryContext validateSyncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
                validateSyncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
                status = querySynchronouslyForCount (&validateSyncQueryCtxt, vlanMoCount);

                if (WAVE_MESSAGE_SUCCESS == status) {
                    if (0 == vlanMoCount) {
                        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " Validation successful "
                            " No VlanProfile Managed object exists with key: " + profileName);
                    } else  {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation failure "
                            "Already existing VlanProfile Managed Object with key: " + profileName);
                        goto last;
                    }
                } else {
                    APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation not completed "
                        "  Quering DB is failed");
                    goto last;
                }


                ProfileManagedObject *pProfileManagedObject = NULL;
                WaveManagedObjectSynchronousQueryContext profileQueryCtxt
                    (ProfileManagedObject::getClassName());
                profileQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));

                // Query the existing ProfileManagedObject partially to load the subprofile vector and add the VlanProfileManagedObject to the subprofile vector. And update the ProfileManagedObject in DB.

                profileQueryCtxt.addSelectField("subProfileCfg");
                pResults = querySynchronously (&profileQueryCtxt);

                if ( pResults && (pResults->size())) {
                    pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
                } else {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }

                if( NULL == pProfileManagedObject ) {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }

                VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject =
                    new VlanProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

                prismAssert (NULL != pVlanProfileCfgManagedObject, __FILE__, __LINE__);

                APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Creating vlan-profile Managed Object");
                pVlanProfileCfgManagedObject->setDescription(pAPPMGlobalAPPMVlanMessage->getProfileName());
                //pVlanProfileCfgManagedObject->setSwModeFlag(false);
                pVlanProfileCfgManagedObject->setIsAllowedVlanAll(false);
                pVlanProfileCfgManagedObject->setIsAllowedVlanAllV2(false);
                pVlanProfileCfgManagedObject->setIsAllowedVlanNone(false);
                pVlanProfileCfgManagedObject->setName(string("vlan-profile"));
                pVlanProfileCfgManagedObject->setSwMode(VLAN_SW_MODE_ACCESS);
                pVlanProfileCfgManagedObject->setAccessVlan(pAPPMGlobalAPPMVlanMessage->getAccessVlan());
                pVlanProfileCfgManagedObject->setVlanRangeAdd(UI32Range(""));
                pVlanProfileCfgManagedObject->setVlanRangeRemove(UI32Range(""));
                pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));
                pVlanProfileCfgManagedObject->setSwModeFlag(true);
                APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("\t Native Vlan Id :: ") + pAPPMGlobalAPPMVlanMessage->getNativeVlanId());
                APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject  = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
                pAPPMNativeVlanManagedObject->setNativeVlanId(APPM_NSM_NATIVE_VLAN_DEFAULT);
                pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                pProfileManagedObject->addSubProfileToVector
                    ((SubProfileManagedObject *) pVlanProfileCfgManagedObject);
                pProfileManagedObject->addAttributeToBeUpdated("subProfileCfg");
                updateWaveManagedObject(pProfileManagedObject);
                }
                break;
                case VCENTER_ACCESS_VLAN_PROFILE_UPDATE:
                {

                    VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = NULL;

                if( pAPPMGlobalAPPMVlanMessage->getCmdCode() != VLAN_NSM_INTF_DELETE ) {

                WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanProfileCfgManagedObject::getClassName());

                syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
                pResults = querySynchronously (&syncQueryCtxt);

                if (NULL != pResults)
                {
                    UI32 numberOfResults = pResults->size ();
                    if (0 == numberOfResults) {
                        APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "Updating Vlan-profile Managed object : Can't find the vlan-profile object");
                    }else if (1 == numberOfResults) {
                        pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject *>((*pResults)[0]);
                        APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Updating Vlan-profile Managed object : Found Managed Object");
                    } else {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating Vlan-profile Managed object : Two or more objects exists");
                        goto last;
                    }
                }
                }

                    pVlanProfileCfgManagedObject->setAccessVlan(pAPPMGlobalAPPMVlanMessage->getAccessVlan());
                    pVlanProfileCfgManagedObject->addAttributeToBeUpdated("accessVlan");
                    updateWaveManagedObject(pVlanProfileCfgManagedObject);
                    break;
                }
                case VCENTER_TRUNK_VLAN_PROFILE_CREATE:
                    {
                    WaveManagedObjectSynchronousQueryContext validateSyncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
                validateSyncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
                status = querySynchronouslyForCount (&validateSyncQueryCtxt, vlanMoCount);

                if (WAVE_MESSAGE_SUCCESS == status) {
                    if (0 == vlanMoCount) {
                        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " Validation successful "
                            " No VlanProfile Managed object exists with key: " + profileName);
                    } else  {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation failure "
                            "Already existing VlanProfile Managed Object with key: " + profileName);
                        goto last;
                    }
                } else {
                    APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation not completed "
                        "  Quering DB is failed");
                    goto last;
                }

                ProfileManagedObject *pProfileManagedObject = NULL;
                WaveManagedObjectSynchronousQueryContext profileQueryCtxt
                    (ProfileManagedObject::getClassName());
                profileQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));

                // Query the existing ProfileManagedObject partially to load the subprofile vector and add the VlanProfileManagedObject to the subprofile vector. And update the ProfileManagedObject in DB.

                profileQueryCtxt.addSelectField("subProfileCfg");
                pResults = querySynchronously (&profileQueryCtxt);

                if ( pResults && (pResults->size())) {
                    pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
                } else {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }

                if( NULL == pProfileManagedObject ) {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }

                VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject =
                    new VlanProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

                prismAssert (NULL != pVlanProfileCfgManagedObject, __FILE__, __LINE__);

                /* vLan Range Add */
                vector<UI32>::iterator msgiter;
                vector <UI32> msgVlanRangeVector;
                UI32Range msgVlanrange = UI32Range("");
                msgVlanrange = pAPPMGlobalAPPMVlanMessage->getVlanRange();
                //msgVlanrange.getUI32RangeVector(msgVlanRangeVector);
                //msgVlanrange = UI32Range("");

 /* vLan Range Add end*/

                APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Creating vlan-profile Managed Object");
                pVlanProfileCfgManagedObject->setDescription(pAPPMGlobalAPPMVlanMessage->getProfileName());
                pVlanProfileCfgManagedObject->setSwModeFlag(true);
                pVlanProfileCfgManagedObject->setIsAllowedVlanAll(false);
                pVlanProfileCfgManagedObject->setIsAllowedVlanAllV2(false);
                pVlanProfileCfgManagedObject->setIsAllowedVlanNone(false);
                pVlanProfileCfgManagedObject->setName(string("vlan-profile"));

                pVlanProfileCfgManagedObject->setSwMode(VLAN_SW_MODE_TRUNK);
                pVlanProfileCfgManagedObject->setVlanRangeAdd(msgVlanrange);
                pVlanProfileCfgManagedObject->setVlanRangeRemove(UI32Range(""));
                pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));

                APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("\t Native Vlan Id :: ") + pAPPMGlobalAPPMVlanMessage->getNativeVlanId());
                APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject  = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
                pAPPMNativeVlanManagedObject->setNativeVlanId(APPM_NSM_NATIVE_VLAN_DEFAULT);
                pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                pProfileManagedObject->addSubProfileToVector
                    ((SubProfileManagedObject *) pVlanProfileCfgManagedObject);
                pProfileManagedObject->addAttributeToBeUpdated("subProfileCfg");
                updateWaveManagedObject(pProfileManagedObject);
                break;
                    }
               case VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_ALL:
                {

				trace (TRACE_LEVEL_INFO, "vlanMessageMoStep CMCODE: VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_ALL" );
                WaveManagedObjectSynchronousQueryContext validateSyncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
                validateSyncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
                status = querySynchronouslyForCount (&validateSyncQueryCtxt, vlanMoCount);

                if (WAVE_MESSAGE_SUCCESS == status) {
                    if (0 == vlanMoCount) {
                        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " Validation successful "
                            " No VlanProfile Managed object exists with key: " + profileName);
                    } else  {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation failure "
                            "Already existing VlanProfile Managed Object with key: " + profileName);
                        goto last;
                    }
                } else {
                    APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation not completed "
                        "  Quering DB is failed");
                    goto last;
                }

                ProfileManagedObject *pProfileManagedObject = NULL;
                WaveManagedObjectSynchronousQueryContext profileQueryCtxt
                    (ProfileManagedObject::getClassName());
                profileQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));

                // Query the existing ProfileManagedObject partially to load the subprofile vector and add the VlanProfileManagedObject to the subprofile vector. And update the ProfileManagedObject in DB.

                profileQueryCtxt.addSelectField("subProfileCfg");
                pResults = querySynchronously (&profileQueryCtxt);

                if ( pResults && (pResults->size())) {
                    pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
                } else {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }

                if( NULL == pProfileManagedObject ) {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }

                VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject =
                    new VlanProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

                prismAssert (NULL != pVlanProfileCfgManagedObject, __FILE__, __LINE__);

                APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Creating vlan-profile Managed Object");
                pVlanProfileCfgManagedObject->setDescription(pAPPMGlobalAPPMVlanMessage->getProfileName());
                pVlanProfileCfgManagedObject->setSwModeFlag(true);
                pVlanProfileCfgManagedObject->setIsAllowedVlanAll(true);
                pVlanProfileCfgManagedObject->setIsAllowedVlanAllV2(true);
                pVlanProfileCfgManagedObject->setIsAllowedVlanNone(false);
                pVlanProfileCfgManagedObject->setName(string("vlan-profile"));

                pVlanProfileCfgManagedObject->setSwMode(VLAN_SW_MODE_TRUNK);
                pVlanProfileCfgManagedObject->setVlanRangeAdd(UI32Range(""));
                pVlanProfileCfgManagedObject->setVlanRangeRemove(UI32Range(""));
                pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));

                APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("\t Native Vlan Id :: ") + pAPPMGlobalAPPMVlanMessage->getNativeVlanId());
                APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject  = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
                pAPPMNativeVlanManagedObject->setNativeVlanId(1);
                pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                pProfileManagedObject->addSubProfileToVector
                    ((SubProfileManagedObject *) pVlanProfileCfgManagedObject);
                pProfileManagedObject->addAttributeToBeUpdated("subProfileCfg");
                updateWaveManagedObject(pProfileManagedObject);
                }
                break;
               case VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_NONE:
                {

                trace (TRACE_LEVEL_INFO, "vlanMessageMoStep CMCODE: VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_NONE" );
                WaveManagedObjectSynchronousQueryContext validateSyncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
                validateSyncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
                status = querySynchronouslyForCount (&validateSyncQueryCtxt, vlanMoCount);

                if (WAVE_MESSAGE_SUCCESS == status) {
                    if (0 == vlanMoCount) {
                        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " Validation successful "
                            " No VlanProfile Managed object exists with key: " + profileName);
                    } else  {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation failure "
                            "Already existing VlanProfile Managed Object with key: " + profileName);
                        goto last;
                    }
                } else {
                    APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation not completed "
                        "  Quering DB is failed");
                    goto last;
                }

                ProfileManagedObject *pProfileManagedObject = NULL;
                WaveManagedObjectSynchronousQueryContext profileQueryCtxt
                    (ProfileManagedObject::getClassName());
                profileQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));

                // Query the existing ProfileManagedObject partially to load the subprofile vector and add the VlanProfileManagedObject to the subprofile vector. And update the ProfileManagedObject in DB.

                profileQueryCtxt.addSelectField("subProfileCfg");
                pResults = querySynchronously (&profileQueryCtxt);

                if ( pResults && (pResults->size())) {
                    pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[0]);
                } else {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }

                if( NULL == pProfileManagedObject ) {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }

                VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject =
                    new VlanProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

                prismAssert (NULL != pVlanProfileCfgManagedObject, __FILE__, __LINE__);

                APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Creating vlan-profile Managed Object");
                pVlanProfileCfgManagedObject->setDescription(pAPPMGlobalAPPMVlanMessage->getProfileName());
                pVlanProfileCfgManagedObject->setSwModeFlag(true);
                pVlanProfileCfgManagedObject->setIsAllowedVlanAll(false);
                pVlanProfileCfgManagedObject->setIsAllowedVlanAllV2(false);
                pVlanProfileCfgManagedObject->setIsAllowedVlanNone(true);
                pVlanProfileCfgManagedObject->setName(string("vlan-profile"));

                pVlanProfileCfgManagedObject->setSwMode(VLAN_SW_MODE_TRUNK);
                pVlanProfileCfgManagedObject->setVlanRangeAdd(UI32Range(""));
                pVlanProfileCfgManagedObject->setVlanRangeRemove(UI32Range(""));
                pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));

                APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("\t Native Vlan Id :: ") + pAPPMGlobalAPPMVlanMessage->getNativeVlanId());
                APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject  = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
                pAPPMNativeVlanManagedObject->setNativeVlanId(1);
                pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                pProfileManagedObject->addSubProfileToVector
                    ((SubProfileManagedObject *) pVlanProfileCfgManagedObject);
                pProfileManagedObject->addAttributeToBeUpdated("subProfileCfg");
                updateWaveManagedObject(pProfileManagedObject);
                }
                break;
                case VCENTER_TRUNK_VLAN_PROFILE_UPDATE:
                    {
                    VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = NULL;

                if( pAPPMGlobalAPPMVlanMessage->getCmdCode() != VLAN_NSM_INTF_DELETE ) {

                WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanProfileCfgManagedObject::getClassName());

                syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
                pResults = querySynchronously (&syncQueryCtxt);

                if (NULL != pResults)
                {
                    UI32 numberOfResults = pResults->size ();
                    if (0 == numberOfResults) {
                        APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "Updating Vlan-profile Managed object : Can't find the vlan-profile object");
                    }else if (1 == numberOfResults) {
                        pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject *>((*pResults)[0]);
                        APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Updating Vlan-profile Managed object : Found Managed Object");
                    } else {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating Vlan-profile Managed object : Two or more objects exists");
                        goto last;
                    }
                }
                }

                vector<UI32>::iterator msgiter;
                            vector <UI32> msgVlanRangeVector;
                            UI32Range msgVlanrange = UI32Range("");


                            msgVlanrange = pAPPMGlobalAPPMVlanMessage->getVlanRange();
                            msgVlanrange.getUI32RangeVector(msgVlanRangeVector);
                            msgVlanrange = UI32Range("");

                            if(pVlanProfileCfgManagedObject->getIsAllowedVlanAllV2())
                            {/*Check the remove list*/
                                UI32Range moRemoveVlanrange = UI32Range("");
                                vector <UI32> moRemoveVlanRangeVector;
                                vector<UI32>::iterator moremiter;

                                moRemoveVlanrange = pVlanProfileCfgManagedObject->getVlanRangeRemove();
                                moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);
                                moRemoveVlanrange = UI32Range("");

                                for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
                                {
                                    for (moremiter=moRemoveVlanRangeVector.begin(); moremiter != moRemoveVlanRangeVector.end(); moremiter++)
                                    {
                                        if(*moremiter == *msgiter)
                                        {/*Found in remove list, so now erase the element from the remove vector*/
                                            moRemoveVlanRangeVector.erase(moremiter);
                                            break;
                                        }

                                    }

                                }
                                string vlanRemoveRangeStr = UI32Range::getUI32RangeStringFromVector(moRemoveVlanRangeVector);
                                moRemoveVlanrange.fromString(vlanRemoveRangeStr);
                                pVlanProfileCfgManagedObject->setVlanRangeRemove(moRemoveVlanrange);
                                pVlanProfileCfgManagedObject->setVlanRangeAdd(UI32Range(""));
                                pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));

                                //Create a vector of all the attributes to be updated.
                                vector<string> listOfAttributes;
                                listOfAttributes.push_back("vlanRangeRemove");
                                listOfAttributes.push_back("vlanRangeAdd");
                                listOfAttributes.push_back("vlanRangeExcept");
                                pVlanProfileCfgManagedObject->setAttributesToBeUpdated(listOfAttributes);
                            }
                            else
                            {
                                vector <UI32> moVlanRangeVector;
                                UI32Range moVlanrange = UI32Range("");

                                moVlanrange = pVlanProfileCfgManagedObject->getVlanRangeAdd();
                                moVlanrange.getUI32RangeVector(moVlanRangeVector);
                                moVlanrange = UI32Range("");

                                vector<UI32>::iterator moiter;
                                vector<UI32>::iterator moindex = moVlanRangeVector.begin();
                                bool isVlanIdPresent = true;

                                for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
                                {
                                    for (moiter=moVlanRangeVector.begin(); moiter != moVlanRangeVector.end(); moiter++)
                                    {
                                        if(*moiter == *msgiter)
                                        {
                                            break;
                                        }
                                        else if(*moiter > *msgiter)/*Since this is a sorted vector, and we have already exceeded the required
                                                                     VLAN ID, we cannot find it in further parses, so break*/
                                        {
                                            moindex = moVlanRangeVector.begin();
                                            isVlanIdPresent = false;
                                            break;
                                        }
                                    }

                                    if(!isVlanIdPresent)
                                        while(*moindex < *msgiter)
                                            moindex++;

                                    if(moiter == moVlanRangeVector.end())
                                    {
                                        isVlanIdPresent = false;
                                        moindex = moVlanRangeVector.end();
                                    }

                                    if(!isVlanIdPresent)/*Vlan not found in MO, add it at appropriate position to keep the vector sorted*/
                                        moVlanRangeVector.insert(moindex, *msgiter);

                                    moindex = moVlanRangeVector.begin();
                                    isVlanIdPresent = true;
                                }
                                string vlanRangeStr = UI32Range::getUI32RangeStringFromVector(moVlanRangeVector);
                                moVlanrange.fromString(vlanRangeStr);
                                pVlanProfileCfgManagedObject->setVlanRangeAdd(moVlanrange);
                                pVlanProfileCfgManagedObject->setVlanRangeRemove(UI32Range(""));
                                pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));
                                pVlanProfileCfgManagedObject->setIsAllowedVlanNone(false);

                                //Create a vector of all the attributes to be updated.
                                vector<string> listOfAttributes;
                                listOfAttributes.push_back("vlanRangeAdd");
                                listOfAttributes.push_back("vlanRangeRemove");
                                listOfAttributes.push_back("vlanRangeExcept");
                                listOfAttributes.push_back("isAllowedVlanNone");
                                pVlanProfileCfgManagedObject->setAttributesToBeUpdated(listOfAttributes);
                            }
                            updateWaveManagedObject(pVlanProfileCfgManagedObject);
                            break;
                    }
                    case VCENTER_VLAN_PROFILE_DELETE:
                    {
                        trace (TRACE_LEVEL_INFO, "APPM_REMOVE" );
                    ProfileManagedObject *pProfileManagedObject = NULL;
                    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
					WaveManagedObjectSynchronousQueryContext profileQueryCtxt (ProfileManagedObject::getClassName());
                    profileQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
                    profileQueryCtxt.addSelectField("subProfileCfg");
                    pResultsprofile = querySynchronously (&profileQueryCtxt);

                    if (pResultsprofile == NULL || !(pResultsprofile->size()))
                    {
                        status = WAVE_MESSAGE_ERROR;
                        goto last;
                    }

                    pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResultsprofile)[0]);

                    if ( NULL == pProfileManagedObject ) {
                        status = WAVE_MESSAGE_ERROR;
                        goto last;
                    }

                    syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
                    pResults = querySynchronously (&syncQueryCtxt);
                    VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = NULL;

                    if (NULL != pResults) {
                        UI32 numberOfResults = pResults->size();
                    if (0 == numberOfResults) {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Deleting Vlan-profile Managed object : Can't find the policy object");
                        goto last;
                    }else if (1 == numberOfResults) {
                        pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject *>((*pResults)[0]);
                        APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " APPM_REMOVE Found Managed Object");
                    } else {
                        APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Two or more objects exists");
                        goto last;
                    }
                    }

                    if ( NULL == pVlanProfileCfgManagedObject ) {
                        status = WAVE_MESSAGE_ERROR;
                        goto last;
                    }

                    retval = pProfileManagedObject->delSubProfileFromVector("vlan-profile");
                    if (retval == 1) {
                        APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " deleted vlan profile");
                    } else {
                        APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL,  "vlan  sub-profile is not found");
                        status = WAVE_MESSAGE_ERROR;
                        goto last;
                    }
                    pProfileManagedObject->addAttributeToBeUpdated("subProfileCfg");
                    updateWaveManagedObject(pProfileManagedObject);
                    break;
                    }
                case VCENTER_TRUNK_VLAN_PROFILE_NATIVE_VLAN:
                    {


                    VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = NULL;

                    if( pAPPMGlobalAPPMVlanMessage->getCmdCode() != VLAN_NSM_INTF_DELETE ) {

                    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanProfileCfgManagedObject::getClassName());

                    syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
                    pResults = querySynchronously (&syncQueryCtxt);

                    if (NULL != pResults)
                    {
                        UI32 numberOfResults = pResults->size ();
                        if (0 == numberOfResults) {
                            APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "Updating Vlan-profile Managed object : Can't find the vlan-profile object");
                        }else if (1 == numberOfResults) {
                            pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject *>((*pResults)[0]);
                            APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Updating Vlan-profile Managed object : Found Managed Object");
                        } else {
                            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating Vlan-profile Managed object : Two or more objects exists");
                            goto last;
                        }
                        }
                    }

                        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("\t Native Vlan Id :: ") + pAPPMGlobalAPPMVlanMessage->getNativeVlanId());
                        APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject  = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
                        pAPPMNativeVlanManagedObject->setNativeVlanId(pAPPMGlobalAPPMVlanMessage->getNativeVlanId());
                        pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                        pVlanProfileCfgManagedObject->addAttributeToBeUpdated("nativeVlanIntfConfig");
                        updateWaveManagedObject(pVlanProfileCfgManagedObject);
                        break;

                        }

                default:
				    APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "(APPM_VCENTER) unknown operation code");
                    break;
              }
            break;
            }
		case APPM_REMOVE:
			{
				ProfileManagedObject *pProfileManagedObject = NULL;
				WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
				WaveManagedObjectSynchronousQueryContext profileQueryCtxt (ProfileManagedObject::getClassName());
				profileQueryCtxt.addAndAttribute (new AttributeString (profileName, "profileName"));
                profileQueryCtxt.addSelectField("subProfileCfg");
				pResultsprofile = querySynchronously (&profileQueryCtxt);

				if (pResultsprofile == NULL || !(pResultsprofile->size()))
				{
					status = WAVE_MESSAGE_ERROR;
					goto last;
				}

				pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResultsprofile)[0]);

                if ( NULL == pProfileManagedObject ) {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }

				syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
				pResults = querySynchronously (&syncQueryCtxt);
				VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = NULL;

				if (NULL != pResults) {
					UI32 numberOfResults = pResults->size();
					if (0 == numberOfResults) {
						APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Deleting Vlan-profile Managed object : Can't find the policy object");
						goto last;
					}else if (1 == numberOfResults) {
						pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject *>((*pResults)[0]);
						APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " APPM_REMOVE Found Managed Object");
					} else {
						APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Two or more objects exists");
						goto last;
					}
				}

                if ( NULL == pVlanProfileCfgManagedObject ) {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }

                APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject = NULL; 
                
				switch (pAPPMGlobalAPPMVlanMessage->getCmdCode()) {

				case VLAN_GROUP_ID:
					APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " removing vlan classifier activation");
					ret = pVlanProfileCfgManagedObject->updateVlanClassifierInfo(pAPPMGlobalAPPMVlanMessage, false);
					if (ret != APPM_SUCCESS) {
						status = WAVE_MESSAGE_ERROR;
                        goto last;
					}
                    pVlanProfileCfgManagedObject->addAttributeToBeUpdated("vlanClassifierInfo");
                    updateWaveManagedObject(pVlanProfileCfgManagedObject);
					break;

				case VLAN_ACCESS_VLAN_DEL:
                {
					APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " removing access vlan configuration");
					pVlanProfileCfgManagedObject->setAccessVlan(APPM_DEFAULT_VLAN);
                    pVlanProfileCfgManagedObject->addAttributeToBeUpdated("accessVlan");
                    updateWaveManagedObject(pVlanProfileCfgManagedObject);
					break;
                }

				case VLAN_SW_MODE_FLAG_DEL:
                {

					if(pAPPMGlobalAPPMVlanMessage->getSwModeFlag() == 
						pVlanProfileCfgManagedObject->getSwModeFlag())
						break;

					pVlanProfileCfgManagedObject->setSwMode(VLAN_SW_MODE_INVALID);
                    pVlanProfileCfgManagedObject->setIsAllowedVlanAll(false);
                    pVlanProfileCfgManagedObject->setIsAllowedVlanAllV2(false);
					pVlanProfileCfgManagedObject->setIsAllowedVlanNone(false);
					pVlanProfileCfgManagedObject->setAccessVlan(0);

					pVlanProfileCfgManagedObject->setVlanRangeAdd(UI32Range(""));
					pVlanProfileCfgManagedObject->setVlanRangeRemove(UI32Range(""));
					pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));
					pVlanProfileCfgManagedObject->setSwModeFlag(false);
					pAPPMNativeVlanManagedObject  = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
                    pAPPMNativeVlanManagedObject->setNativeVlanId(APPM_NSM_NATIVE_VLAN_DEFAULT);
                    pAPPMNativeVlanManagedObject->setCtag(APPM_NSM_NATIVE_VLAN_DEFAULT);
                    pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                    pVlanProfileCfgManagedObject->deleteAllMacVlanClassifications();
                    pVlanProfileCfgManagedObject->deleteAllMacGroupVlanClassifications();
                    pVlanProfileCfgManagedObject->deleteAllCtagVlanClassifications();
                    updateWaveManagedObject(pVlanProfileCfgManagedObject);
					break;
                }

				case  VLAN_NATIVE_ID_DEL:
					pAPPMNativeVlanManagedObject  = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
					pAPPMNativeVlanManagedObject->setNativeVlanId(APPM_NSM_NATIVE_VLAN_DEFAULT);
                    pAPPMNativeVlanManagedObject->setCtag(APPM_NSM_NATIVE_VLAN_DEFAULT);
					pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                    pVlanProfileCfgManagedObject->addAttributeToBeUpdated("nativeVlanIntfConfig");
                    updateWaveManagedObject(pVlanProfileCfgManagedObject);
                    break;

                case APPM_VLAN_CLASSIFICATION_DEL_NATIVE_CTAG :
                    {
                        pAPPMNativeVlanManagedObject
                            = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
                        pAPPMNativeVlanManagedObject->setNativeVlanId(APPM_NSM_NATIVE_VLAN_DEFAULT);
                        pAPPMNativeVlanManagedObject->setCtag(APPM_NSM_NATIVE_VLAN_DEFAULT);
                        pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                        pVlanProfileCfgManagedObject->addAttributeToBeUpdated("nativeVlanIntfConfig");
                        updateWaveManagedObject(pVlanProfileCfgManagedObject);
                        break;
                    }

				case VLAN_PROFILE_DEL:
					retval = pProfileManagedObject->delSubProfileFromVector("vlan-profile");
					if (retval == 1) {
						APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " deleted vlan profile");
					} else {
						APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL,  "vlan  sub-profile is not found");
						status = WAVE_MESSAGE_ERROR;
						goto last;
					}
                    pProfileManagedObject->addAttributeToBeUpdated("subProfileCfg");
					updateWaveManagedObject(pProfileManagedObject);
					break;

				default:
					APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "(APPM_REMOVE) unknown operation code");
					break;
				}
				break;
			}

		case APPM_SET_ELEM:
			{
				VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = NULL;

				if( pAPPMGlobalAPPMVlanMessage->getCmdCode() != VLAN_NSM_INTF_DELETE ) {

				WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanProfileCfgManagedObject::getClassName());

				syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
				pResults = querySynchronously (&syncQueryCtxt);

				if (NULL != pResults) 
				{
					UI32 numberOfResults = pResults->size ();
					if (0 == numberOfResults) {
						APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "Updating Vlan-profile Managed object : Can't find the vlan-profile object");
					}else if (1 == numberOfResults) {
						pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject *>((*pResults)[0]);
						APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Updating Vlan-profile Managed object : Found Managed Object");
					} else {
						APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating Vlan-profile Managed object : Two or more objects exists");
						goto last;
					}
				}
				}

				if (pVlanProfileCfgManagedObject != NULL || pAPPMGlobalAPPMVlanMessage->getCmdCode() == VLAN_NSM_INTF_DELETE) 
				{

					switch(pAPPMGlobalAPPMVlanMessage->getCmdCode())
					{

					case VLAN_SW_MODE: 
						{
							//Create a vector of all the attributes to be updated.
                            vector<string> listOfAttributes;

                            if ((pAPPMGlobalAPPMVlanMessage->getSwMode() == VLAN_SW_MODE_ACCESS)&& (pVlanProfileCfgManagedObject->getSwMode() != VLAN_SW_MODE_ACCESS) ) {	
                                
                                /* reset the trunk vlan info when the switch mode is changed from trunk to access mode.*/
                                pVlanProfileCfgManagedObject->setIsAllowedVlanAll(false);
                                pVlanProfileCfgManagedObject->setIsAllowedVlanAllV2(false);
								pVlanProfileCfgManagedObject->setIsAllowedVlanNone(false);
							    pVlanProfileCfgManagedObject->setVlanRangeAdd(UI32Range(""));
							    pVlanProfileCfgManagedObject->setVlanRangeRemove(UI32Range(""));
							    pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));
								pVlanProfileCfgManagedObject->setAccessVlan(APPM_DEFAULT_VLAN);
                                pVlanProfileCfgManagedObject->deleteAllCtagVlanClassifications();

                                //Partial DB update.
                                listOfAttributes.push_back("isAllowedVlanAll");
                                listOfAttributes.push_back("isAllowedVlanAllV2");
                                listOfAttributes.push_back("isAllowedVlanNone");
                                listOfAttributes.push_back("vlanRangeAdd");
                                listOfAttributes.push_back("vlanRangeRemove");
                                listOfAttributes.push_back("vlanRangeExcept");
                                listOfAttributes.push_back("accessVlan");
                                listOfAttributes.push_back("ctagVlanClassifications");

							} else {

                                pVlanProfileCfgManagedObject->deleteAllMacVlanClassifications();
                                pVlanProfileCfgManagedObject->deleteAllMacGroupVlanClassifications();

                                pVlanProfileCfgManagedObject->setAccessVlan(APPM_INVALID_VLAN);
                                listOfAttributes.push_back("accessVlan");
                                listOfAttributes.push_back("macVlanClassifications");
                                listOfAttributes.push_back("macGroupVlanClassifications");
							}

							pVlanProfileCfgManagedObject->setSwMode((VlanSwMode)pAPPMGlobalAPPMVlanMessage->getSwMode());
                            listOfAttributes.push_back("swMode");

							if(VLAN_SW_MODE_ACCESS ==  (VlanSwMode)pAPPMGlobalAPPMVlanMessage->getSwMode())
							{
								APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject  = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
								pAPPMNativeVlanManagedObject->setNativeVlanId(APPM_NSM_NATIVE_VLAN_DEFAULT);
								pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                                listOfAttributes.push_back("nativeVlanIntfConfig");
							}
                            pVlanProfileCfgManagedObject->setAttributesToBeUpdated(listOfAttributes);

                            updateWaveManagedObject(pVlanProfileCfgManagedObject);
							break;
						}
					case VLAN_NSM_INTF_DELETE:
					{
						APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string("APPMGlobalConfWorker::vlanMessageMoStep changeVlanList is : !!") 
														+ pAPPMGlobalAPPMVlanMessage->getIsChangeVlanList());
						if(pAPPMGlobalAPPMVlanMessage->getIsChangeVlanList())
						{
							/*At this point we know that none of the port profiles where this vlan exists as access or trunk vlan 
							are activated. So we can go ahead with the vlan deletion. But we also need to remove the access or trunk 
							association of this vlan from all the port profiles*/

            				UI32 vlan_id = pAPPMGlobalAPPMVlanMessage->getAccessVlan(); 			
            				VlanProfileCfgManagedObject *pVlanProfileCfgMO = NULL;

							WaveManagedObjectSynchronousQueryContext vlanSyncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
							vlanSyncQueryCtxt.addAndAttribute(new AttributeString("default", "description"),WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);
							vlanSyncQueryCtxt.addAndAttribute(new AttributeBool(false, "isAllowedVlanNone"));

							vector<WaveManagedObject *> *pVlanResults = NULL;

							pVlanResults = querySynchronously (&vlanSyncQueryCtxt);

							if (NULL != pVlanResults)
							{
								UI32 numberOfResults = pVlanResults->size ();

								if (0 == numberOfResults) {
									APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, 
										" No MO in VlanProfileCfgManagedObject found");
								}else {
									UI32 count = 0;
									while(count != numberOfResults)
									{
										pVlanProfileCfgMO = dynamic_cast<VlanProfileCfgManagedObject *>
																					((*pVlanResults)[count]);
										/*string profName("\0");

										profName = pVlanProfileCfgMO->getDescription();
										We do not make any changes to the default profile

										if (profName.compare("default") == 0)
										{
											count++;
											continue;
										}*/

										//if(!(pVlanProfileCfgMO->getIsAllowedVlanNone()))
										//{
											if(pVlanProfileCfgMO->getIsAllowedVlanAllV2())
											{
												vector<UI32>::iterator moremoveiter;	
												UI32Range moRemoveVlanrange = UI32Range("");
												vector <UI32> moRemoveVlanRangeVector;
												moRemoveVlanrange = pVlanProfileCfgMO->getVlanRangeRemove();
												moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);     
												moRemoveVlanrange = UI32Range("");

												for (moremoveiter = moRemoveVlanRangeVector.begin(); 
													 moremoveiter != moRemoveVlanRangeVector.end();moremoveiter++)
												{
													if(*moremoveiter == vlan_id)
													{
														moRemoveVlanRangeVector.erase(moremoveiter);
														string vlanRangeStr = 
															UI32Range::getUI32RangeStringFromVector(moRemoveVlanRangeVector);

														moRemoveVlanrange.fromString(vlanRangeStr);
														pVlanProfileCfgMO->setVlanRangeRemove(moRemoveVlanrange);
                                                        pVlanProfileCfgMO->addAttributeToBeUpdated("vlanRangeRemove");
														updateWaveManagedObject(pVlanProfileCfgMO);
														break;
													}
												}
											}

											else
											{/*Search if it is a part of trunk or access vlan*/
												if(pVlanProfileCfgMO->getSwMode() == VLAN_SW_MODE_ACCESS)
												{
                                                    bool moChanged = false;
                                                    //If the vlan to be deleted is asssociated with a vlan profile in access mode, fall back to default vlan association.
                                                    if(pVlanProfileCfgMO->getAccessVlan() == vlan_id)
                                                    {
                                                        pVlanProfileCfgMO->setAccessVlan(APPM_DEFAULT_VLAN);
                                                        pVlanProfileCfgMO->addAttributeToBeUpdated("accessVlan");
                                                        moChanged = true;
                                                    }

                                                    if(pVlanProfileCfgMO->doesVlanContainMacClassification(vlan_id)) {
                                                        pVlanProfileCfgMO->deleteMacVlanClassificationsOfVlan(vlan_id);
                                                        pVlanProfileCfgMO->deleteMacGroupVlanClassificationsOfVlan(vlan_id);
                                                        pVlanProfileCfgMO->addAttributeToBeUpdated("macVlanClassifications");
                                                        pVlanProfileCfgMO->addAttributeToBeUpdated("macGroupVlanClassifications");
                                                        moChanged = true;
                                                    }
                                                    if(moChanged) {
                                                        updateWaveManagedObject(pVlanProfileCfgMO);
                                                    }
												}
												else if (pVlanProfileCfgMO->getSwMode() == VLAN_SW_MODE_TRUNK)
												{
													WaveManagedObjectPointer<APPMNativeVlanManagedObject> pNativeVlanMo = 
														pVlanProfileCfgMO->getNativeVlanIntfConfig();

													APPMNativeVlanManagedObject *nativeTmp = pNativeVlanMo.operator-> (); 
													if(nativeTmp->getNativeVlanId() == vlan_id)
													{ 
														APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject = 
															new APPMNativeVlanManagedObject( 
																dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

														pAPPMNativeVlanManagedObject->setNativeVlanId(APPM_NSM_NATIVE_VLAN_DEFAULT);

														pVlanProfileCfgMO->setNativeVlanIntfConfig(
															WaveManagedObjectPointer<APPMNativeVlanManagedObject> (
																pAPPMNativeVlanManagedObject));
                                                        pVlanProfileCfgMO->addAttributeToBeUpdated("nativeVlanIntfConfig");
                                                        updateWaveManagedObject(pVlanProfileCfgMO);
														APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, 
														"setting the native Vlan ID to 0");
													}
													else
													{
														vector <UI32> moAddVlanRangeVector;
														UI32Range moAddVlanrange = UI32Range("");
														vector<UI32>::iterator moadditer;

														moAddVlanrange = pVlanProfileCfgMO->getVlanRangeAdd();
														moAddVlanrange.getUI32RangeVector(moAddVlanRangeVector);
														moAddVlanrange = UI32Range("");

														moadditer = moAddVlanRangeVector.begin();

														for (moadditer=moAddVlanRangeVector.begin(); 
															moadditer != moAddVlanRangeVector.end(); moadditer++)
														{
															if(*moadditer == vlan_id)
															{/*Vlan Id exists, remove from vlan list*/
																moAddVlanRangeVector.erase(moadditer);
																break;
															}
														}

														string vlanRangeStr = 
															UI32Range::getUI32RangeStringFromVector(moAddVlanRangeVector);

														moAddVlanrange.fromString(vlanRangeStr);
                                                        pVlanProfileCfgMO->setVlanRangeAdd(moAddVlanrange);
                                                        pVlanProfileCfgMO->deleteCtagVlanClassificationsOfVlan(vlan_id);
                                                        pVlanProfileCfgMO->addAttributeToBeUpdated("vlanRangeAdd");
                                                        pVlanProfileCfgMO->addAttributeToBeUpdated("ctagVlanClassifications");
                                                        updateWaveManagedObject(pVlanProfileCfgMO);
													}
												}
											}
										//}
										count++;
									}
								}
								pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pVlanResults);
								delete pVlanResults;
							}
						}
						break;
					}
					case VLAN_IS_ALLOWED_VLAN_ALL: 
						{
                            pVlanProfileCfgManagedObject->setIsAllowedVlanAll(pAPPMGlobalAPPMVlanMessage->getIsAllowedVlanAll());
                            pVlanProfileCfgManagedObject->setIsAllowedVlanAllV2(pAPPMGlobalAPPMVlanMessage->getIsAllowedVlanAll());
						    pVlanProfileCfgManagedObject->setIsAllowedVlanNone(false);
							pVlanProfileCfgManagedObject->setVlanRangeAdd(UI32Range(""));
							pVlanProfileCfgManagedObject->setVlanRangeRemove(UI32Range(""));
							pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));
							pVlanProfileCfgManagedObject->setAccessVlan(0);

                            //Create a vector of all the attributes to be updated.
                            vector<string> listOfAttributes;
                            listOfAttributes.push_back("isAllowedVlanAll");
                            listOfAttributes.push_back("isAllowedVlanAllV2");
                            listOfAttributes.push_back("isAllowedVlanNone");
                            listOfAttributes.push_back("vlanRangeAdd");
                            listOfAttributes.push_back("vlanRangeRemove");
                            listOfAttributes.push_back("vlanRangeExcept");
                            listOfAttributes.push_back("accessVlan");
                            pVlanProfileCfgManagedObject->setAttributesToBeUpdated(listOfAttributes);
                            updateWaveManagedObject(pVlanProfileCfgManagedObject);

							break;
						}
					case VLAN_SW_MODE_FLAG:
						{
                            //Below condition, if breaks, it is made sure the pVlanProfileCfgManagedObject doesnt get updated.
							if(pAPPMGlobalAPPMVlanMessage->getSwModeFlag() == 
								pVlanProfileCfgManagedObject->getSwModeFlag())
								break;
							pVlanProfileCfgManagedObject->setSwMode(VLAN_SW_MODE_ACCESS);
							pVlanProfileCfgManagedObject->setAccessVlan(APPM_DEFAULT_VLAN);
							pVlanProfileCfgManagedObject->setVlanRangeAdd(UI32Range(""));
							pVlanProfileCfgManagedObject->setVlanRangeRemove(UI32Range(""));
							pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));
							pVlanProfileCfgManagedObject->setSwModeFlag(true);
							APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject  = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
							pAPPMNativeVlanManagedObject->setNativeVlanId(APPM_NSM_NATIVE_VLAN_DEFAULT);
							pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                            
                            //Create a vector of all the attributes to be updated.
                            vector<string> listOfAttributes;
                            listOfAttributes.push_back("swMode");
                            listOfAttributes.push_back("accessVlan");
                            listOfAttributes.push_back("vlanRangeAdd");
                            listOfAttributes.push_back("vlanRangeRemove");
                            listOfAttributes.push_back("vlanRangeExcept");
                            listOfAttributes.push_back("swModeFlag");
                            listOfAttributes.push_back("nativeVlanIntfConfig");
                            pVlanProfileCfgManagedObject->setAttributesToBeUpdated(listOfAttributes);
                            updateWaveManagedObject(pVlanProfileCfgManagedObject);

							break;
						}
					case VLAN_IS_ALLOWED_VLAN_NONE: 
						{
						    pVlanProfileCfgManagedObject->setIsAllowedVlanNone(pAPPMGlobalAPPMVlanMessage->getIsAllowedVlanNone());
                            pVlanProfileCfgManagedObject->setIsAllowedVlanAll(false);
                            pVlanProfileCfgManagedObject->setIsAllowedVlanAllV2(false);
							pVlanProfileCfgManagedObject->setVlanRangeAdd(UI32Range(""));
							pVlanProfileCfgManagedObject->setVlanRangeRemove(UI32Range(""));
							pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));

                            //Create a vector of all the attributes to be updated.
                            vector<string> listOfAttributes;
                            listOfAttributes.push_back("isAllowedVlanNone");
                            listOfAttributes.push_back("isAllowedVlanAll");
                            listOfAttributes.push_back("isAllowedVlanAllV2");
                            listOfAttributes.push_back("vlanRangeAdd");
                            listOfAttributes.push_back("vlanRangeRemove");
                            listOfAttributes.push_back("vlanRangeExcept");
                            pVlanProfileCfgManagedObject->setAttributesToBeUpdated(listOfAttributes);
                            updateWaveManagedObject(pVlanProfileCfgManagedObject);
							break;
						}

                    case VLAN_ACCESS_VLAN:
							pVlanProfileCfgManagedObject->setAccessVlan(pAPPMGlobalAPPMVlanMessage->getAccessVlan());
                            pVlanProfileCfgManagedObject->addAttributeToBeUpdated("accessVlan");
                            updateWaveManagedObject(pVlanProfileCfgManagedObject);
							break;

					case VLAN_IS_ALLOWED_VLAN_ADD:
						{
							vector<UI32>::iterator msgiter;
							vector <UI32> msgVlanRangeVector;
							UI32Range msgVlanrange = UI32Range("");
							
							msgVlanrange = pAPPMGlobalAPPMVlanMessage->getVlanRange();
							msgVlanrange.getUI32RangeVector(msgVlanRangeVector);
							msgVlanrange = UI32Range("");

							if(pVlanProfileCfgManagedObject->getIsAllowedVlanAllV2())
							{/*Check the remove list*/
								UI32Range moRemoveVlanrange = UI32Range("");
						    	vector <UI32> moRemoveVlanRangeVector;
								vector<UI32>::iterator moremiter;

								moRemoveVlanrange = pVlanProfileCfgManagedObject->getVlanRangeRemove();
								moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);
								moRemoveVlanrange = UI32Range("");
								
                            	for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
								{
							    	for (moremiter=moRemoveVlanRangeVector.begin(); moremiter != moRemoveVlanRangeVector.end(); moremiter++)
									{
										if(*moremiter == *msgiter)
										{/*Found in remove list, so now erase the element from the remove vector*/
									    	moRemoveVlanRangeVector.erase(moremiter);
											break;
										}
										
									}
								
								}
						    	string vlanRemoveRangeStr = UI32Range::getUI32RangeStringFromVector(moRemoveVlanRangeVector);
								moRemoveVlanrange.fromString(vlanRemoveRangeStr); 
						    	pVlanProfileCfgManagedObject->setVlanRangeRemove(moRemoveVlanrange);
						    	pVlanProfileCfgManagedObject->setVlanRangeAdd(UI32Range(""));
						    	pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));

                                //Create a vector of all the attributes to be updated.
                                vector<string> listOfAttributes;
                                listOfAttributes.push_back("vlanRangeRemove");
                                listOfAttributes.push_back("vlanRangeAdd");
                                listOfAttributes.push_back("vlanRangeExcept");
                                pVlanProfileCfgManagedObject->setAttributesToBeUpdated(listOfAttributes);
							}
							else
							{
								vector <UI32> moVlanRangeVector;
								UI32Range moVlanrange = UI32Range("");
								
								moVlanrange = pVlanProfileCfgManagedObject->getVlanRangeAdd();
								moVlanrange.getUI32RangeVector(moVlanRangeVector);
								moVlanrange = UI32Range("");

								vector<UI32>::iterator moiter;
								vector<UI32>::iterator moindex = moVlanRangeVector.begin();
								bool isVlanIdPresent = true;

								for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
								{
									for (moiter=moVlanRangeVector.begin(); moiter != moVlanRangeVector.end(); moiter++)
									{
										if(*moiter == *msgiter)
										{
											break;
										}
										else if(*moiter > *msgiter)/*Since this is a sorted vector, and we have already exceeded the required 
																	 VLAN ID, we cannot find it in further parses, so break*/
										{
											moindex = moVlanRangeVector.begin();
											isVlanIdPresent = false;
											break;
										}
									}

									if(!isVlanIdPresent)
										while(*moindex < *msgiter)
											moindex++;

									if(moiter == moVlanRangeVector.end())
									{
										isVlanIdPresent = false;
										moindex = moVlanRangeVector.end();
									}

									if(!isVlanIdPresent)/*Vlan not found in MO, add it at appropriate position to keep the vector sorted*/
										moVlanRangeVector.insert(moindex, *msgiter);

									moindex = moVlanRangeVector.begin();
									isVlanIdPresent = true;
								}
								string vlanRangeStr = UI32Range::getUI32RangeStringFromVector(moVlanRangeVector);
								moVlanrange.fromString(vlanRangeStr); 
								pVlanProfileCfgManagedObject->setVlanRangeAdd(moVlanrange);
								pVlanProfileCfgManagedObject->setVlanRangeRemove(UI32Range(""));
								pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));
								pVlanProfileCfgManagedObject->setIsAllowedVlanNone(false);
                                
                                //Create a vector of all the attributes to be updated.
                                vector<string> listOfAttributes;
                                listOfAttributes.push_back("vlanRangeAdd");
                                listOfAttributes.push_back("vlanRangeRemove");
                                listOfAttributes.push_back("vlanRangeExcept");
                                listOfAttributes.push_back("isAllowedVlanNone");
                                pVlanProfileCfgManagedObject->setAttributesToBeUpdated(listOfAttributes);
							}
                            updateWaveManagedObject(pVlanProfileCfgManagedObject);
							break;
						}

					case VLAN_IS_ALLOWED_VLAN_REMOVE: 
						{
							vector<UI32>::iterator moindex;
							vector<UI32>::iterator msgiter;
							vector <UI32> msgVlanRangeVector;
							UI32Range msgVlanrange = UI32Range("");
							bool isVlanIdPresent = true;

							msgVlanrange = pAPPMGlobalAPPMVlanMessage->getVlanRange();
							msgVlanrange.getUI32RangeVector(msgVlanRangeVector);
							msgVlanrange = UI32Range("");
							
							if(pVlanProfileCfgManagedObject->getIsAllowedVlanAllV2())
							{/*Check the remove list*/
								UI32Range moRemoveVlanrange = UI32Range("");
						    	vector <UI32> moRemoveVlanRangeVector;
								vector<UI32>::iterator moremiter;

								moindex = moRemoveVlanRangeVector.begin();
								moRemoveVlanrange = pVlanProfileCfgManagedObject->getVlanRangeRemove();
								moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);
								moRemoveVlanrange = UI32Range("");
								
                            	for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
								{
							    	for (moremiter=moRemoveVlanRangeVector.begin(); moremiter != moRemoveVlanRangeVector.end(); moremiter++)
									{
										if(*moremiter == *msgiter)
										{/*Found in remove list, so do nothing*/
											break;
										}

										else if(*moremiter > *msgiter)/*Since this is a sorted vector, and we have already exceeded the required 																		VLAN ID, we cannot find it in further parses, so break*/
										{
											moindex = moRemoveVlanRangeVector.begin();
											isVlanIdPresent = false;
											break;
										}
									}

									if(!isVlanIdPresent)
										while(*moindex < *msgiter)
											moindex++;

									if(moremiter == moRemoveVlanRangeVector.end())
									{
										isVlanIdPresent = false;
										moindex = moRemoveVlanRangeVector.end();
									}

									if(!isVlanIdPresent)/*Vlan not found in MO, add it at appropriate position to keep the vector sorted*/
										moRemoveVlanRangeVector.insert(moindex, *msgiter);

									moindex = moRemoveVlanRangeVector.begin();
									isVlanIdPresent = true;
								}

						    	string vlanRemoveRangeStr = UI32Range::getUI32RangeStringFromVector(moRemoveVlanRangeVector);
								moRemoveVlanrange.fromString(vlanRemoveRangeStr); 
						    	pVlanProfileCfgManagedObject->setVlanRangeRemove(moRemoveVlanrange);
						    	pVlanProfileCfgManagedObject->setVlanRangeAdd(UI32Range(""));
						    	pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));


                                //Construct a vector of attributes to be updated in DB.
                                vector<string> listofAttributes;
                                listofAttributes.push_back("vlanRangeRemove");
                                listofAttributes.push_back("vlanRangeAdd");
                                listofAttributes.push_back("vlanRangeExcept");
                                pVlanProfileCfgManagedObject->setAttributesToBeUpdated(listofAttributes);

							}
							else
							{
								vector <UI32> moVlanRangeVector;
								UI32Range moVlanrange = UI32Range("");

								moVlanrange = pVlanProfileCfgManagedObject->getVlanRangeAdd();
								moVlanrange.getUI32RangeVector(moVlanRangeVector);
								moVlanrange = UI32Range("");

								vector<UI32>::iterator moiter;
								moindex = moVlanRangeVector.begin();

								for (msgiter=msgVlanRangeVector.begin(); msgiter != msgVlanRangeVector.end(); msgiter++)
								{
									for (moiter=moVlanRangeVector.begin(); moiter != moVlanRangeVector.end(); moiter++)
									{
										if(*moiter == *msgiter)
										{/*Vlan Id exists, remove from vlan list*/
											moVlanRangeVector.erase(moiter); 
											break;
										}
									}
								}

								string vlanRangeStr = UI32Range::getUI32RangeStringFromVector(moVlanRangeVector);

								moVlanrange.fromString(vlanRangeStr); 
								pVlanProfileCfgManagedObject->setVlanRangeAdd(moVlanrange);
								pVlanProfileCfgManagedObject->setVlanRangeRemove(UI32Range(""));
								pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));
                                
                                //Create a vector of all the attributes to be updated.
                                vector<string> listOfAttributes;
                                listOfAttributes.push_back("vlanRangeAdd");
                                listOfAttributes.push_back("vlanRangeRemove");
                                listOfAttributes.push_back("vlanRangeExcept");
                                pVlanProfileCfgManagedObject->setAttributesToBeUpdated(listOfAttributes);
							}

                            updateWaveManagedObject(pVlanProfileCfgManagedObject);
							break;
						}
					case VLAN_IS_ALLOWED_VLAN_EXCEPT:
						{
							UI32Range msgVlanrange = UI32Range("");
							UI32Range moVlanrange = UI32Range("");
							
							msgVlanrange = pAPPMGlobalAPPMVlanMessage->getVlanRange();
							
                            pVlanProfileCfgManagedObject->setIsAllowedVlanAll(true);
                            pVlanProfileCfgManagedObject->setIsAllowedVlanAllV2(true);
							pVlanProfileCfgManagedObject->setIsAllowedVlanNone(false);
						    pVlanProfileCfgManagedObject->setVlanRangeRemove(msgVlanrange);
						    pVlanProfileCfgManagedObject->setVlanRangeAdd(UI32Range(""));
						    pVlanProfileCfgManagedObject->setVlanRangeExcept(UI32Range(""));

                            //Create a vector of all the attributes to be updated.
                            vector<string> listOfAttributes;
                            listOfAttributes.push_back("isAllowedVlanAll");
                            listOfAttributes.push_back("isAllowedVlanAllV2");
                            listOfAttributes.push_back("isAllowedVlanNone");
                            listOfAttributes.push_back("vlanRangeRemove");
                            listOfAttributes.push_back("vlanRangeAdd");
                            listOfAttributes.push_back("vlanRangeExcept");

                            pVlanProfileCfgManagedObject->setAttributesToBeUpdated(listOfAttributes);
                            updateWaveManagedObject(pVlanProfileCfgManagedObject);
							break;
						}
					case VLAN_GROUP_ID:
						{
                            ret = pVlanProfileCfgManagedObject->updateVlanClassifierInfo(pAPPMGlobalAPPMVlanMessage, true);
                            pVlanProfileCfgManagedObject->addAttributeToBeUpdated("vlanClassifierInfo");
                            
                            if (ret != APPM_SUCCESS) {
                                status = WAVE_MESSAGE_ERROR;
                                goto last;
                            } else {
                                //update the DB only when updateVlanClassifierInfo succeeds.
                                updateWaveManagedObject(pVlanProfileCfgManagedObject);
                            }
							break;
						}
                    case VLAN_NATIVE_ID_ADD:
					case VCENTER_TRUNK_VLAN_PROFILE_NATIVE_VLAN:
						{
							APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("\t Native Vlan Id :: ") + pAPPMGlobalAPPMVlanMessage->getNativeVlanId());
							APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject  = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
							pAPPMNativeVlanManagedObject->setNativeVlanId(pAPPMGlobalAPPMVlanMessage->getNativeVlanId());
							pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                            pVlanProfileCfgManagedObject->addAttributeToBeUpdated("nativeVlanIntfConfig");
                            updateWaveManagedObject(pVlanProfileCfgManagedObject);
                            break;
                            
                        } 
                    case APPM_VLAN_CLASSIFICATION_SET_MAC_ADDR :
                        {
                            APPMMacAddressVlanClassificationMO *pMO 
                                = new APPMMacAddressVlanClassificationMO (dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
                            pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pMO);
                            pMO->setOwnerManagedObjectId(pVlanProfileCfgManagedObject->getObjectId());
                            pMO->setAccessVlanId(pAPPMGlobalAPPMVlanMessage->getAccessVlan());
                            pMO->setMac(pAPPMGlobalAPPMVlanMessage->getMac());
                            addToComposition(VlanProfileCfgManagedObject::getClassName(), APPMMacAddressVlanClassificationMO::getClassName(), 
                                    "macVlanClassifications", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
                            break;
                        }
                    case APPM_VLAN_CLASSIFICATION_DEL_MAC_ADDR :
                        {
                            WaveManagedObjectSynchronousQueryContext accessPortSyncQueryCtxt (APPMMacAddressVlanClassificationMO::getClassName ());
                            accessPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (pAPPMGlobalAPPMVlanMessage->getAccessVlan(), "vlanId"));
                            accessPortSyncQueryCtxt.addAndAttribute (new AttributeString (pAPPMGlobalAPPMVlanMessage->getMac(), "mac"));
                            accessPortSyncQueryCtxt.addAndAttribute (new AttributeObjectId (pVlanProfileCfgManagedObject->getObjectId(), "ownerManagedObjectId"));
                            vector<WaveManagedObject *> *pAccessResults = querySynchronously (&accessPortSyncQueryCtxt);

                            if((pAccessResults != NULL) && pAccessResults->size() > 0)
                            {
                                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pAccessResults);
                                APPMMacAddressVlanClassificationMO *pMO =  dynamic_cast<APPMMacAddressVlanClassificationMO*>((*pAccessResults)[0]);
                                deleteFromComposition (VlanProfileCfgManagedObject::getClassName(), APPMMacAddressVlanClassificationMO::getClassName(), 
                                        "macVlanClassifications", pMO->getOwnerManagedObjectId(), pMO->getObjectId());

                                pAccessResults->clear();
                                delete pAccessResults;
                            }

                            break;
                        }
                    case APPM_VLAN_CLASSIFICATION_SET_MAC_GROUP :
                        {
                            APPMMacGroupVlanClassificationMO *pMO = new APPMMacGroupVlanClassificationMO (dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
                            pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pMO);
                            pMO->setOwnerManagedObjectId(pVlanProfileCfgManagedObject->getObjectId());
                            pMO->setVlanId(pAPPMGlobalAPPMVlanMessage->getAccessVlan());
                            pMO->setMacGroupId(pAPPMGlobalAPPMVlanMessage->getMacGroup());
                            addToComposition(VlanProfileCfgManagedObject::getClassName(), APPMMacGroupVlanClassificationMO::getClassName(), 
                                    "macGroupVlanClassifications", pMO->getOwnerManagedObjectId(), pMO->getObjectId());

                            break;
                        }
                    case APPM_VLAN_CLASSIFICATION_DEL_MAC_GROUP :
                        {
                            WaveManagedObjectSynchronousQueryContext macGroupQueryCtxt (APPMMacGroupVlanClassificationMO::getClassName ());
                            macGroupQueryCtxt.addAndAttribute (new AttributeUI32 (pAPPMGlobalAPPMVlanMessage->getAccessVlan(), "vlanId"));
                            macGroupQueryCtxt.addAndAttribute (new AttributeUI32 (pAPPMGlobalAPPMVlanMessage->getMacGroup(), "macGroupId"));
                            macGroupQueryCtxt.addAndAttribute (new AttributeObjectId (pVlanProfileCfgManagedObject->getObjectId(), "ownerManagedObjectId"));
                            vector<WaveManagedObject *> *pMacGroupResults = querySynchronously (&macGroupQueryCtxt);

                            if((pMacGroupResults != NULL) && pMacGroupResults->size() > 0)
                            {
                                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pMacGroupResults);
                                APPMMacGroupVlanClassificationMO *pMO =  dynamic_cast<APPMMacGroupVlanClassificationMO*>((*pMacGroupResults)[0]);
                                deleteFromComposition (VlanProfileCfgManagedObject::getClassName(), APPMMacGroupVlanClassificationMO::getClassName(), 
                                        "macGroupVlanClassifications", pMO->getOwnerManagedObjectId(), pMO->getObjectId());

                                pMacGroupResults->clear();
                                delete pMacGroupResults;
                            }

                            break;
                        }
                    case APPM_VLAN_CLASSIFICATION_SET_CTAG :
                        {
                            APPMCtagVlanClassificationMO *pMO = new APPMCtagVlanClassificationMO (dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
                            pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pMO);
                            pMO->setOwnerManagedObjectId(pVlanProfileCfgManagedObject->getObjectId());
                            pMO->setVlanId(pAPPMGlobalAPPMVlanMessage->getTrunkVlanId());
                            pMO->setCtagId(pAPPMGlobalAPPMVlanMessage->getCtag());
                            addToComposition(VlanProfileCfgManagedObject::getClassName(), APPMCtagVlanClassificationMO::getClassName(), 
                                    "ctagVlanClassifications", pMO->getOwnerManagedObjectId(), pMO->getObjectId());
                            break;
                        }
                    case APPM_VLAN_CLASSIFICATION_DEL_CTAG :
                        {
                            WaveManagedObjectSynchronousQueryContext trunkPortSyncQueryCtxt (APPMCtagVlanClassificationMO::getClassName ());
                            trunkPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (pAPPMGlobalAPPMVlanMessage->getTrunkVlanId(), "vlanId"));
                            trunkPortSyncQueryCtxt.addAndAttribute (new AttributeUI32 (pAPPMGlobalAPPMVlanMessage->getCtag(), "ctagId"));
                            trunkPortSyncQueryCtxt.addAndAttribute (new AttributeObjectId (pVlanProfileCfgManagedObject->getObjectId(), "ownerManagedObjectId"));
                            vector<WaveManagedObject *> *pTrunkResults = querySynchronously (&trunkPortSyncQueryCtxt);

                            if((pTrunkResults != NULL) && (pTrunkResults->size() > 0))
                            {
                                pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pTrunkResults);
                                APPMCtagVlanClassificationMO *pMO =  dynamic_cast<APPMCtagVlanClassificationMO*>((*pTrunkResults)[0]);
                                deleteFromComposition (VlanProfileCfgManagedObject::getClassName(), APPMCtagVlanClassificationMO::getClassName(), 
                                        "ctagVlanClassifications", pMO->getOwnerManagedObjectId(), pMO->getObjectId());

                                pTrunkResults->clear();
                                delete pTrunkResults;
                            }

                            break;
                        }
                    case APPM_VLAN_CLASSIFICATION_SET_NATIVE_CTAG :
                    {
                        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("APPM_VLAN_CLASSIFICATION_SET_NATIVE_CTAG Native Vlan Id<") 
                            + pAPPMGlobalAPPMVlanMessage->getNativeVlanId() + ">");

                        APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject  
                            = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
                        pAPPMNativeVlanManagedObject->setNativeVlanId(pAPPMGlobalAPPMVlanMessage->getNativeVlanId());
                        pAPPMNativeVlanManagedObject->setCtag(pAPPMGlobalAPPMVlanMessage->getCtag());
                        pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                        pVlanProfileCfgManagedObject->addAttributeToBeUpdated("nativeVlanIntfConfig");
                        updateWaveManagedObject(pVlanProfileCfgManagedObject);
                        break;
                    }
                    case APPM_VLAN_CLASSIFICATION_DEL_NATIVE_CTAG :
                    {
                        APPMNativeVlanManagedObject* pAPPMNativeVlanManagedObject  
                            = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
                        pAPPMNativeVlanManagedObject->setNativeVlanId(APPM_NSM_NATIVE_VLAN_DEFAULT);
                        pAPPMNativeVlanManagedObject->setCtag(APPM_NSM_NATIVE_VLAN_DEFAULT);
                        pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));
                        pVlanProfileCfgManagedObject->addAttributeToBeUpdated("nativeVlanIntfConfig");
                        updateWaveManagedObject(pVlanProfileCfgManagedObject);
                        break;
                    }

					}

				}
			}

		}
last:
		if(pResults != NULL) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }

			if(pResultsprofile != NULL) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsprofile);
            delete pResultsprofile;
        }
		
        pPrismLinearSequencerContext->executeNextStep(status);
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEVEL);
	}
    void  APPMGlobalConfWorker::APPMGlobalAPPMVlanMessageHandler( APPMGlobalAPPMVlanMessage *pAPPMGlobalAPPMVlanMessage)
    {
// Validations and Queries should go here
    	if(pAPPMGlobalAPPMVlanMessage->getCmdCode() != VLAN_NSM_INTF_DELETE){
		PrismLinearSequencerStep sequencerSteps[] =
		{
			reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::vlanProfileValidationStep),
			reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::vlanMessageStep),
			reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerStartTransactionStep),
			// Your configuration change code goes here
			reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::vlanMessageMoStep),

			reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
			// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pAPPMGlobalAPPMVlanMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

      pPrismLinearSequencerContext->holdAll();
      pPrismLinearSequencerContext->start ();
      } else {


    	  PrismLinearSequencerStep sequencerSteps[] =
    	  		{
    	  			reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerStartTransactionStep),
    	  			// Your configuration change code goes here
    	  			reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::vlanMessageMoStep),

    	  			reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
    	  			// Programming Protocol Daemons goes here

    	              reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerSucceededStep),
    	              reinterpret_cast<PrismLinearSequencerStep>(&APPMGlobalConfWorker::prismLinearSequencerFailedStep)
    	          };

    	          PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pAPPMGlobalAPPMVlanMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    	        pPrismLinearSequencerContext->holdAll();
    	        pPrismLinearSequencerContext->start ();
      }
    }

    string  APPMGlobalConfWorker::getClassName()
    {
        return ("APPMGlobalConfWorker");
    }

    ResourceId APPMGlobalConfWorker::appmQosVcsSanity(string profile_name, 
        UI8 cmdCode, string CeeMapName, string CosMutationMapName, SI32 Cos)
    {
        
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    string mapName;

    /*
     * Basic Check whether Vcs is Enabled or not.
     */
    if (DcmToolKit::isVcsEnabled () != WAVE_PERSISTENCE_CLUSTER_ENABLED) {
        return (WAVE_MESSAGE_SUCCESS);
    }

    switch ( cmdCode )
    {
    case QOS_COS_TO_COS_MUTATION: {
        status = QosUtils::vcsQosSanity(QOS_INTERFACE_COS_MUTATION, CosMutationMapName,true,0,0,0);
        break;
    }
    
    //The below 2 defects contradict each other.
    //TR000407684 [FC: Change in behavior of "default CoS" from Hercules+ to Hydra.] => Added below case to fix thos.
    //and TR000454577 FC: PFC enable should be allowed on default-CoS to support end-to-end legacy pause.  => Commenting the below conde, to fix this.

    /*case QOS_DEFAULT_COS: {
        status = QosUtils::vcsQosSanity(QOS_INTERFACE_DEFAULT_COS, "", true, 0, 0, Cos);
        break;
    }
    case QOS_NO_DEFAULT_COS: {
        status = QosUtils::vcsQosSanity(QOS_INTERFACE_DEFAULT_COS, "", true, 0, 0, Cos);
        break;
    }*/
    case QOS_NO_COS_TO_COS_MUTATION: {
        break;
    }
    case QOS_TRUST: {
        return (WRC_SSM_DCM_ERR_APPM_QOS_TRUST_COS_IN_VCS_NON_CEE);
        break;
    }
    case QOS_NO_TRUST: {
        return (WRC_SSM_DCM_ERR_APPM_REMOVE_QOS_TRUST_COS_IN_VCS_NON_CEE);
        break;
    }
/*
    case QOS_FLOW_CTRL_ALL:
    case QOS_NO_FLOW_CTRL_ALL:
        return (WRC_SSM_DCM_ERR_APPM_QOS_FLOWCONTROL_ENABLE_IN_VCS);
        break;
*/
    default:
        return WAVE_MESSAGE_SUCCESS;

    }

    return status;
    }


    void  APPMGlobalConfWorker::PortProfileObjectMessageHandler (PortProfileObject *pProfile)
    {

        vector<WaveManagedObject *> *pResults1 = NULL;
        QosProfileCfgManagedObject *pQosProfileCfgManagedObject = NULL;
        VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = NULL;
        unsigned int iter = 0;
        UI32 pvlan = 0;
        APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject = NULL;
        string VlanRangeString;
        UI32Range VlanRange = UI32Range("");
        UI32Range VlanRange2 = UI32Range("0");
        ProfileManagedObject *pProfileManagedObject = NULL;
        string strPname = pProfile->getProfileName();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt1(ProfileManagedObject::getClassName());
        vector<WaveManagedObjectPointer<ProfileMacManagedObject> > configuredAssocMacs;
        syncQueryCtxt1.addAndAttribute (new AttributeString (strPname, "profileName"));
        pResults1 = querySynchronously(&syncQueryCtxt1);
        if ( pResults1 && (pResults1->size())) {
        pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults1)[0]);
        }
        configuredAssocMacs = pProfileManagedObject->getAssocMacs();
        vector<string> assoc_macs;

        vector<WaveManagedObjectPointer<SubProfileManagedObject> > subProfileVector = pProfileManagedObject->getSubProfileCfg();

        for (unsigned int iterator = 0; iterator < subProfileVector.size();
                iterator++) {

            if (NULL != dynamic_cast<VlanProfileCfgManagedObject *>(subProfileVector[iterator].operator ->())) {
                APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "VLAN Profile Exists.");
                pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject *>(subProfileVector[iterator].operator ->());
            } else if (NULL != dynamic_cast<QosProfileCfgManagedObject *>(subProfileVector[iterator].operator ->())) {
                APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "QOS Profile Exists.");
                pQosProfileCfgManagedObject = dynamic_cast<QosProfileCfgManagedObject *>(subProfileVector[iterator].operator ->());
            } else {
                APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "NO Sub Profile Exists.");
            }
        }

        if ( (subProfileVector.size() > 0) && (pVlanProfileCfgManagedObject != NULL))
        {
        pProfile->setVlanRange(VlanRange2);  //initializing it to 0
        if(pVlanProfileCfgManagedObject->getSwModeFlag())
          pProfile->setSwMode(true);
        else
          pProfile->setSwMode(false);

        for (iter = 0; iter < configuredAssocMacs.size(); iter++) {
            assoc_macs.push_back(configuredAssocMacs[iter]->m_mac);
        }
        sort(assoc_macs.begin(), assoc_macs.end());
        pProfile->setProfileMacs(assoc_macs);
        pProfile->setMacCount(assoc_macs.size());

        if(pVlanProfileCfgManagedObject->getSwModeFlag())
        {
        switch (pVlanProfileCfgManagedObject->getSwMode())
        {
        case VLAN_SW_MODE_ACCESS:
            pProfile->setSwMode(true);
            pProfile->setisAccessMode(true);
            if ((pvlan = pVlanProfileCfgManagedObject->getAccessVlan()))
                pProfile->setAccessVlan(pvlan);
            break;
        case VLAN_SW_MODE_TRUNK:
            {
            pProfile->setSwMode(true);
            pProfile->setisAccessMode(false);
            pAPPMNativeVlanManagedObject = ((pVlanProfileCfgManagedObject->getNativeVlanIntfConfig()).operator->());

        if (pVlanProfileCfgManagedObject->getIsAllowedVlanNone())
        {
            // In this case only possibility is for native vlan to exist
            if(pAPPMNativeVlanManagedObject == NULL){
                break;
            }
            if(pAPPMNativeVlanManagedObject->getNativeVlanId())
                pProfile->setNativeVlanId(pAPPMNativeVlanManagedObject->getNativeVlanId());
            break;
        }
        if (pVlanProfileCfgManagedObject->getIsAllowedVlanNone())
            pProfile->setIsAllowedVlanNone(true);
        if (pVlanProfileCfgManagedObject->getIsAllowedVlanAllV2())
            pProfile->setIsAllowedVlanAll(true);
          else {
            VlanRange = pVlanProfileCfgManagedObject->getVlanRangeAdd();
            VlanRangeString = VlanRange.toString();
            if (VlanRangeString != "")
                pProfile->setVlanRange(VlanRange);
        }

        if(pAPPMNativeVlanManagedObject->getNativeVlanId())
            pProfile->setNativeVlanId(pAPPMNativeVlanManagedObject->getNativeVlanId());
            }
        break;

        default:
            break;
            }
        }
    }

        // get QoS value from mo
        if ( (subProfileVector.size() > 0) && (pQosProfileCfgManagedObject != NULL))
        {
            SI32 qosVal= pQosProfileCfgManagedObject->getDefaultCosValue();
            stringstream ss;
            ss << qosVal;
            pProfile->setQos(ss.str());
        }
        if (pResults1)
          {
             WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults1);
          }
        pProfile->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply(pProfile);

    return;
    }


void  APPMGlobalConfWorker::VcenterGetAllPortProfileListMessageHandler ( VcenterGetAllPortProfileListMessage *pVcenterGetAllPortProfileListMessage)
    {

        ProfileManagedObject *pProfileManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults;
        string pattern = pVcenterGetAllPortProfileListMessage->getPattern();
        syncQueryCtxt.addAndAttribute (new AttributeString (pattern, "profileName"),WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
        string profile_name;
        vector<string> ppvector;
        UI32 i;
        syncQueryCtxt.addOrderField ("profileId", true);
        pResults = querySynchronously(&syncQueryCtxt);
        if (!pResults) {
            return;
        }

       for (i = 0; i < pResults->size() ; i++)
       {
           pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pResults)[i]);
           profile_name = pProfileManagedObject->getProfileName();
           ppvector.push_back(profile_name);
       }
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("Setting VcenterGetAllPortProfileListMessageHandler :: PPs:%d") + ppvector.size()
            );
       if(pResults)
		   {
		      WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		   }
        pVcenterGetAllPortProfileListMessage->setPortProfiles(ppvector);
        pVcenterGetAllPortProfileListMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply(pVcenterGetAllPortProfileListMessage);
        return;
    }


    void APPMGlobalConfWorker::install(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases) {

		APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, getClassName() + "::" + __FUNCTION__+" creating default profile MO");

		ResourceId status = WAVE_MESSAGE_SUCCESS;

		if ((pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_FIRST_TIME_BOOT) 
                && (pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT)) {

			pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
			pWaveAsynchronousContextForBootPhases->callback ();
			return;
		}

		ProfileManagedObject *pProfileManagedObject = NULL;
        ProfileDomainManagedObject *pProfileDomainManagedObject = NULL;
        VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = NULL;

		startTransaction();
		pProfileManagedObject =
			new ProfileManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
		prismAssert(NULL != pProfileManagedObject, __FILE__, __LINE__);

		pVlanProfileCfgManagedObject = 
			new VlanProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
		prismAssert(NULL != pVlanProfileCfgManagedObject, __FILE__, __LINE__);

		pProfileManagedObject->setProfileName(DEFAULT_PORT_PROFILE_NAME);
		pProfileManagedObject->setProfileId(0);

		pVlanProfileCfgManagedObject->setDescription(DEFAULT_PORT_PROFILE_NAME);
		pVlanProfileCfgManagedObject->setSwModeFlag(true);
		pVlanProfileCfgManagedObject->setSwMode(VLAN_SW_MODE_TRUNK);
        pVlanProfileCfgManagedObject->setIsAllowedVlanAll(true);
		pVlanProfileCfgManagedObject->setIsAllowedVlanAllV2(false);
		pVlanProfileCfgManagedObject->setIsAllowedVlanNone(false);
		pVlanProfileCfgManagedObject->setName(string("vlan-profile"));

		pProfileManagedObject->addSubProfileToVector 
			((SubProfileManagedObject *) pVlanProfileCfgManagedObject);
		APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject  = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
		prismAssert(NULL != pAPPMNativeVlanManagedObject, __FILE__, __LINE__);
		pAPPMNativeVlanManagedObject->setNativeVlanId(APPM_DEFAULT_VLAN);
		pVlanProfileCfgManagedObject->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));

		status = commitTransaction();
		if (status == FRAMEWORK_SUCCESS) {
			status = WAVE_MESSAGE_SUCCESS;
		}

        delete pProfileManagedObject;

        ProfileManagedObject *defaultVlanProfileMO = NULL;
        VlanProfileCfgManagedObject *defaultVlanSubProfileMO = NULL;

        startTransaction();

        defaultVlanProfileMO =
            new ProfileManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        prismAssert(NULL != defaultVlanProfileMO, __FILE__, __LINE__);
        defaultVlanSubProfileMO =
            new VlanProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        prismAssert(NULL != defaultVlanSubProfileMO, __FILE__, __LINE__);

        UI32 PPID = getNextPPid();
        defaultVlanProfileMO->setProfileName(DEFAULT_VLAN_PORT_PROFILE_NAME);
        defaultVlanProfileMO->setProfileId(PPID);
        setGlobalPPid(PPID);

        pAPPMNativeVlanManagedObject
            = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
        prismAssert(NULL != pAPPMNativeVlanManagedObject, __FILE__, __LINE__);
        pAPPMNativeVlanManagedObject->setNativeVlanId(APPM_NSM_NATIVE_VLAN_DEFAULT);
        pAPPMNativeVlanManagedObject->setCtag(APPM_NSM_NATIVE_VLAN_DEFAULT);
        defaultVlanSubProfileMO->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));


        defaultVlanSubProfileMO->setDescription(DEFAULT_VLAN_PORT_PROFILE_NAME);
        defaultVlanSubProfileMO->setSwModeFlag(true);
        defaultVlanSubProfileMO->setSwMode(VLAN_SW_MODE_TRUNK);
        defaultVlanSubProfileMO->setIsAllowedVlanAll(false);
        defaultVlanSubProfileMO->setIsAllowedVlanAllV2(true);
        defaultVlanSubProfileMO->setIsAllowedVlanNone(false);
        defaultVlanSubProfileMO->setName(string("vlan-profile"));

        defaultVlanProfileMO->addSubProfileToVector
            ((SubProfileManagedObject *) defaultVlanSubProfileMO);

        status = commitTransaction();
        if (status == FRAMEWORK_SUCCESS) {
            status = WAVE_MESSAGE_SUCCESS;
        }
        delete defaultVlanProfileMO;



        startTransaction();

        pProfileDomainManagedObject = new ProfileDomainManagedObject
            (dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        pProfileDomainManagedObject->setPortProfileDomainName(DEFAULT_PORT_PROFILE_DOMAIN_NAME);

        ProfileDomainCompositeManagedObject* pProfileDomainCompositeManagedObject
            = new ProfileDomainCompositeManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
        pProfileDomainCompositeManagedObject->setPortProfileName(DEFAULT_VLAN_PORT_PROFILE_NAME);
        pProfileDomainManagedObject->addPortProfileToPortProfileDomain
            (pProfileDomainCompositeManagedObject);

        status = commitTransaction();
        if (status == FRAMEWORK_SUCCESS) {
            status = WAVE_MESSAGE_SUCCESS;
        }       

        delete pProfileDomainManagedObject;

		pWaveAsynchronousContextForBootPhases->setCompletionStatus(status);
		pWaveAsynchronousContextForBootPhases->callback();
	}

    AppmReturnCode APPMGlobalConfWorker::updateAssociateInfo(PrismLinearSequencerContext *pPrismLinearSequencerContext,
    											APPMGlobalAPPMProfileMessage *pMsg)
    	{
    		string macAddr = pMsg->getProfileMac();
    		vector<WaveManagedObject *> *pResults = NULL;
            ProfileMacManagedObject *pProfileMacManagedObject = NULL;

            //Retrieve the object id of the ProfileManagedObject from the DceLinearSeqContext.
            ObjectId profileManagedObjectId = dynamic_cast<DceLinearSeqContext *> (pPrismLinearSequencerContext)->getWaveManagedObjectId();

    		if (pMsg->getCmdCode() == PORT_PROFILE_ASSOCIATE || pMsg->getCmdCode() == VCENTER_ASSOC_MAC) {
    				APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " set_elem:"
    					" new entry creation profilemacmanagedobject");
    				/* Allocate, push it to ProfileMacManagedMO and insert the MAC into vector*/
    				pProfileMacManagedObject =
    					new ProfileMacManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
                    
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " PROFILE ID : " + pMsg->getProfileId());

    				pProfileMacManagedObject->setMac(pMsg->getProfileMac());
    				pProfileMacManagedObject->setProfileName(pMsg->getProfileName());
    				pProfileMacManagedObject->setProfileId(pMsg->getProfileId());

    				pProfileMacManagedObject->setOwnerManagedObjectId(profileManagedObjectId);
    				addToComposition(ProfileManagedObject::getClassName(), ProfileMacManagedObject::getClassName(),
    					"assocMacs", profileManagedObjectId, pProfileMacManagedObject->getObjectId());
    		} else if (pMsg->getCmdCode() == PORT_PROFILE_DEASSOCIATE || pMsg->getCmdCode() == VCENTER_DEASSOC_MAC ) {
    			/* Handling deassociation part */
    				APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " set_elem:"
    					" profile mac entry found in ProfileMacManagedMO");
    				WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileMacManagedObject::getClassName());
    				syncQueryCtxt.addAndAttribute(new AttributeString(macAddr, "mac"));
    				pResults = querySynchronously(&syncQueryCtxt);
    				ProfileMacManagedObject *pProfileMacMORslt = NULL;

    				if (NULL != pResults) {
    					UI32 numberOfResults = pResults->size();
    					if (1 == numberOfResults) {
    						pProfileMacMORslt = dynamic_cast<ProfileMacManagedObject *>((*pResults)[0]);
    						APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " Mac is found in ProfileMacMO DB");
    						deleteFromComposition(ProfileManagedObject::getClassName(), ProfileMacManagedObject::getClassName(),
    							"assocMacs", profileManagedObjectId, pProfileMacMORslt->getObjectId());
    					}
    				} else {
    					APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " pResults ptr in NULL. Quering DB is failed");
    					goto last;
    				}

    		}

    last:
    		if (pResults) {
    			pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
    			delete (pResults);
    		}
    		if (pProfileMacManagedObject) {
    			pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pProfileMacManagedObject);
    		}

    		return (APPM_SUCCESS);
    	}



void APPMGlobalConfWorker::upgrade
(WaveAsynchronousContextForUpgradePhase *pWaveAsynchronousContextForUpgradePhase) {

    unsigned int it = 0;
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    vector<WaveManagedObject *> *ppDomainResults = NULL;
    vector<WaveManagedObject *> *ppResults = NULL;
    vector<WaveManagedObject *> *pResults = NULL;
    vector<WaveManagedObject *> *vlanPResults = NULL;

    WaveManagedObjectSynchronousQueryContext ppSyncQueryCtxt(ProfileManagedObject::getClassName());
    WaveManagedObjectSynchronousQueryContext vlanProfilesyncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
    WaveManagedObjectSynchronousQueryContext ppDomainSyncQueryCtxt(ProfileDomainManagedObject::getClassName());

    ProfileManagedObject *pProfileManagedObject = NULL;
    VlanProfileCfgManagedObject* pVlanProfileCfgManagedObject = NULL;
    APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject = NULL;
    ProfileManagedObject* defaultVlanProfileMO = NULL;
    VlanProfileCfgManagedObject* defaultVlanSubProfileMO = NULL;
    ProfileDomainManagedObject *pProfileDomainManagedObject = NULL;

    /*create a "default" port-profile-domain.*/
    startTransaction();

    pProfileDomainManagedObject = new ProfileDomainManagedObject
        (dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
    pProfileDomainManagedObject->setPortProfileDomainName(DEFAULT_PORT_PROFILE_DOMAIN_NAME);

    status = commitTransaction();
    if (status == FRAMEWORK_SUCCESS) {
        status = WAVE_MESSAGE_SUCCESS;
    }

    delete pProfileDomainManagedObject;
    pProfileDomainManagedObject = NULL;

    /* Remove "switchport trunk allowed vlan all" config from the "default" port-profile.*/
    startTransaction();
    vlanProfilesyncQueryCtxt.addAndAttribute (new AttributeString (DEFAULT_PORT_PROFILE_NAME, "description"));
    vlanPResults = querySynchronously (&vlanProfilesyncQueryCtxt);


    if (NULL != vlanPResults) {
        UI32 numberOfResults = vlanPResults->size ();
        if (0 == numberOfResults) {
            APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "Updating Vlan-profile Managed object : Can't find the vlan-profile object");
            status = WAVE_MESSAGE_ERROR;
            goto last;
        }else if (1 == numberOfResults) {
            pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject *>((*vlanPResults)[0]);
            APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Updating Vlan-profile Managed object : Found Managed Object");
        } else {
            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating Vlan-profile Managed object : Two or more objects exists");
            status = WAVE_MESSAGE_ERROR;
            goto last;
        }
    } else {
        status = WAVE_MESSAGE_ERROR;
        goto last;
    }

    if ( NULL == pVlanProfileCfgManagedObject ) {
        status = WAVE_MESSAGE_ERROR;
        goto last;
    }

    pVlanProfileCfgManagedObject->setIsAllowedVlanAll(true);
    pVlanProfileCfgManagedObject->setIsAllowedVlanAllV2(false);
    updateWaveManagedObject(pVlanProfileCfgManagedObject);
    status = commitTransaction();

    if(FRAMEWORK_SUCCESS == status) {
        status = WAVE_MESSAGE_SUCCESS;
    }

    /* Create a new port-profile named "UpgradedVlanProfile" and add "switchport trunk allowed vlan all" to it */
    syncQueryCtxt.addAndAttribute (new AttributeString (DEFAULT_VLAN_PORT_PROFILE_NAME, "profilename"));
    pResults = querySynchronously (&syncQueryCtxt);

    if (NULL != pResults) {
        UI32 numberOfResults = pResults->size ();
        if (0 == numberOfResults) {

            APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "Updating Vlan-profile Managed object : Can't find the vlan-profile object");
            startTransaction();

            defaultVlanProfileMO =
                new ProfileManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
            prismAssert(NULL != defaultVlanProfileMO, __FILE__, __LINE__);
            defaultVlanSubProfileMO =
                new VlanProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));
            prismAssert(NULL != defaultVlanSubProfileMO, __FILE__, __LINE__);

            UI32 PPID = getNextPPid();
            defaultVlanProfileMO->setProfileName(DEFAULT_VLAN_PORT_PROFILE_NAME);
            defaultVlanProfileMO->setProfileId(PPID);
            setGlobalPPid(PPID);

            pAPPMNativeVlanManagedObject
                = new APPMNativeVlanManagedObject( dynamic_cast<APPMGlobalObjectManager*>(getPWaveObjectManager()));
            prismAssert(NULL != pAPPMNativeVlanManagedObject, __FILE__, __LINE__);
            pAPPMNativeVlanManagedObject->setNativeVlanId(APPM_NSM_NATIVE_VLAN_DEFAULT);
            pAPPMNativeVlanManagedObject->setCtag(APPM_NSM_NATIVE_VLAN_DEFAULT);
            defaultVlanSubProfileMO->setNativeVlanIntfConfig(WaveManagedObjectPointer<APPMNativeVlanManagedObject> (pAPPMNativeVlanManagedObject));


            defaultVlanSubProfileMO->setDescription(DEFAULT_VLAN_PORT_PROFILE_NAME);
            defaultVlanSubProfileMO->setSwModeFlag(true);
            defaultVlanSubProfileMO->setSwMode(VLAN_SW_MODE_TRUNK);
            pVlanProfileCfgManagedObject->setIsAllowedVlanAll(false);
            defaultVlanSubProfileMO->setIsAllowedVlanAllV2(true);
            defaultVlanSubProfileMO->setIsAllowedVlanNone(false);
            defaultVlanSubProfileMO->setName(string("vlan-profile"));

            defaultVlanProfileMO->addSubProfileToVector
                ((SubProfileManagedObject *) defaultVlanSubProfileMO);

            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS) {
                status = WAVE_MESSAGE_SUCCESS;
            }
            delete defaultVlanProfileMO;

        }else if (1 == numberOfResults) {
            APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Updating Vlan-profile Managed object : Found Managed Object");
        } else {
            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Updating Vlan-profile Managed object : Two or more objects exists");
        }
    }

    /*Add all leo port-profiles to the default port-profile-domain of leo++*/
    ppDomainSyncQueryCtxt.addAndAttribute (new AttributeString (DEFAULT_PORT_PROFILE_DOMAIN_NAME, "portProfileDomainName"));
    ppDomainResults = querySynchronously (&ppDomainSyncQueryCtxt);

    if (NULL != ppDomainResults) {
        UI32 numberOfResults = ppDomainResults->size ();
        if (0 == numberOfResults) {
            tracePrintf(TRACE_LEVEL_INFO, "profiledomain <default> is not found");
            status = WAVE_MESSAGE_ERROR;
            goto last;
        }else if (1 == numberOfResults) {
            pProfileDomainManagedObject = dynamic_cast<ProfileDomainManagedObject *>((*ppDomainResults)[0]);
            tracePrintf(TRACE_LEVEL_INFO, "profiledomain <default> is found");
        } else {
            tracePrintf(TRACE_LEVEL_ERROR, "more than one profiledomain <default> found");
            status = WAVE_MESSAGE_ERROR;
            goto last;
        }
    } else {
        status = WAVE_MESSAGE_ERROR;
        goto last;
    }

    if(NULL == pProfileDomainManagedObject) {
        status = WAVE_MESSAGE_ERROR;
        goto last;
    }

    ppResults = querySynchronously(&ppSyncQueryCtxt);

    if (NULL != ppResults) {
        UI32 numberOfResults = ppResults->size ();
        if (0 == numberOfResults) {
            tracePrintf(TRACE_LEVEL_INFO, "No profiles found in the leo build.");
        }else {
            tracePrintf(TRACE_LEVEL_INFO, "Profiles found in the leo build.");
        }
    }

    for( it = 0 ; it < ppResults->size () ; it++) {

        pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*ppResults)[it]);

        // Verify whether the profile is already added to the domain, if yes, donot add, else add.
        UI32 count = 0;
        string portProfileName = pProfileManagedObject->getProfileName();
        WaveManagedObjectSynchronousQueryContext ppCompSyncQueryCtxt(ProfileDomainCompositeManagedObject::getClassName());
        ppCompSyncQueryCtxt.addAndAttribute(new AttributeString(portProfileName, "portprofilename"));
        status = querySynchronouslyForCount (&ppCompSyncQueryCtxt, count);

        if( (0 == count) && (DEFAULT_PORT_PROFILE_NAME != portProfileName) ) {
            startTransaction();

            ProfileDomainCompositeManagedObject* pProfileDomainCompositeManagedObject
                = new ProfileDomainCompositeManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

            pProfileDomainCompositeManagedObject->setPortProfileName(portProfileName);
            pProfileDomainManagedObject->addPortProfileToPortProfileDomain
                (pProfileDomainCompositeManagedObject);         
            updateWaveManagedObject(pProfileDomainManagedObject);

            status = commitTransaction();
            if (status == FRAMEWORK_SUCCESS) {
                status = WAVE_MESSAGE_SUCCESS;
            }
        }
    }

last:
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(ppDomainResults);
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(ppResults);
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(vlanPResults);

    pWaveAsynchronousContextForUpgradePhase->setCompletionStatus (status);
    pWaveAsynchronousContextForUpgradePhase->callback ();

}


}

