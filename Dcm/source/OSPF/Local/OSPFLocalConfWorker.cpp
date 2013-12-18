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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : kverma                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFLocalConfWorker.h"
#include "OSPF/Local/OSPFRouterConfigLocalMessage.h"
#include "OSPF/Local/OSPFPermitDenyRedistributeLocalMessage.h"
#include "OSPF/Local/OSPFNeighborIPAddressLocalManagedObject.h"
#include "OSPF/Local/OSPFSummaryAddressLocalManagedObject.h"
#include "OSPF/Local/OSPFDistanceLocalManagedObject.h"
#include "OSPF/Local/OSPFAreaRangeLocalManagedObject.h"
#include "OSPF/Local/OSPFVirtualLinkLocalManagedObject.h"
#include "OSPF/Local/OSPFAreaLocalManagedObject.h"
#include "OSPF/Local/OSPFPermitDenyRedistributeLocalManagedObject.h"
#include "OSPF/Local/OSPFPermitRedistributeLocalManagedObject.h"
#include "OSPF/Local/OSPFDenyRedistributeLocalManagedObject.h"
#include "OSPF/Local/OSPFConfigLocalManagedObject.h"
#include "ClientInterface/OSPF/OSPFRouterConfigMessage.h"
#include "ClientInterface/OSPF/OSPFPermitDenyRedistributeMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "OSPF/Local/OSPFTypes.h"
#include "Nsm/Local/OSPF/OSPFInterfaceLocalMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Utils/Layer3LicenseUtil.h"
#include "DcmCStatus.h"
#include "Vrf/Local/VrfLocalManagedObject.h"
#include "Vrf/Local/VrfUtils.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "DcmCore/DcmToolKit.h"

namespace DcmNs
{

    OSPFLocalConfWorker::OSPFLocalConfWorker ( OSPFLocalObjectManager *pOSPFLocalObjectManager)
        : WaveWorker  (pOSPFLocalObjectManager)
    {
        OSPFNeighborIPAddressLocalManagedObject    OSPFNeighborIPAddressLocalManagedObject    (pOSPFLocalObjectManager);
        OSPFSummaryAddressLocalManagedObject    OSPFSummaryAddressLocalManagedObject    (pOSPFLocalObjectManager);
        OSPFDistanceLocalManagedObject    OSPFDistanceLocalManagedObject    (pOSPFLocalObjectManager);
        OSPFAreaRangeLocalManagedObject    OSPFAreaRangeLocalManagedObject    (pOSPFLocalObjectManager);
        OSPFVirtualLinkLocalManagedObject    OSPFVirtualLinkLocalManagedObject    (pOSPFLocalObjectManager);
        OSPFAreaLocalManagedObject    OSPFAreaLocalManagedObject    (pOSPFLocalObjectManager);
        OSPFPermitDenyRedistributeLocalManagedObject    OSPFPermitDenyRedistributeLocalManagedObject    (pOSPFLocalObjectManager);
        OSPFPermitRedistributeLocalManagedObject    OSPFPermitRedistributeLocalManagedObject    (pOSPFLocalObjectManager);
        OSPFDenyRedistributeLocalManagedObject    OSPFDenyRedistributeLocalManagedObject    (pOSPFLocalObjectManager);
        OSPFConfigLocalManagedObject    OSPFConfigLocalManagedObject    (pOSPFLocalObjectManager);
        OSPFNeighborIPAddressLocalManagedObject.setupOrm ();
        addManagedClass (OSPFNeighborIPAddressLocalManagedObject::getClassName (), this);
        OSPFSummaryAddressLocalManagedObject.setupOrm ();
        addManagedClass (OSPFSummaryAddressLocalManagedObject::getClassName (), this);
        OSPFDistanceLocalManagedObject.setupOrm ();
        addManagedClass (OSPFDistanceLocalManagedObject::getClassName (), this);
        OSPFAreaRangeLocalManagedObject.setupOrm ();
        addManagedClass (OSPFAreaRangeLocalManagedObject::getClassName (), this);
        OSPFVirtualLinkLocalManagedObject.setupOrm ();
        addManagedClass (OSPFVirtualLinkLocalManagedObject::getClassName (), this);
        OSPFAreaLocalManagedObject.setupOrm ();
        addManagedClass (OSPFAreaLocalManagedObject::getClassName (), this);
        OSPFPermitDenyRedistributeLocalManagedObject.setupOrm ();
        addManagedClass (OSPFPermitDenyRedistributeLocalManagedObject::getClassName (), this);
        OSPFPermitRedistributeLocalManagedObject.setupOrm ();
        addManagedClass (OSPFPermitRedistributeLocalManagedObject::getClassName (), this);
        OSPFDenyRedistributeLocalManagedObject.setupOrm ();
        addManagedClass (OSPFDenyRedistributeLocalManagedObject::getClassName (), this);
        OSPFConfigLocalManagedObject.setupOrm ();
        addManagedClass (OSPFConfigLocalManagedObject::getClassName (), this);
        addOperationMap (OSPFROUTERCONFIGLOCAL, reinterpret_cast<PrismMessageHandler> (&OSPFLocalConfWorker::OSPFRouterConfigLocalMessageHandler));
        addOperationMap (OSPFAREALOCAL, reinterpret_cast<PrismMessageHandler> (&OSPFLocalConfWorker::OSPFAreaLocalMessageHandler));
        addOperationMap (OSPFAREARANGELOCAL, reinterpret_cast<PrismMessageHandler> (&OSPFLocalConfWorker::OSPFAreaRangeLocalMessageHandler));
        addOperationMap (OSPFPERMITDENYCONFIGLOCAL, reinterpret_cast<PrismMessageHandler> (&OSPFLocalConfWorker::OSPFPermitDenyRedistributeLocalMessageHandler));
        addOperationMap (OSPFVIRTUALLINKLOCAL, reinterpret_cast<PrismMessageHandler> (&OSPFLocalConfWorker::OSPFVirtualLinkLocalMessageHandler));
        addOperationMap (OSPFLOCALSHOW,reinterpret_cast<PrismMessageHandler> (&OSPFLocalConfWorker::OSPFLocalShowMessageHandler));
        addOperationMap (OSPFLOCALCLEAR,reinterpret_cast<PrismMessageHandler> (&OSPFLocalConfWorker::OSPFLocalClearMessageHandler));
    }

    OSPFLocalConfWorker::~OSPFLocalConfWorker ()
    {
    }

    PrismMessage  *OSPFLocalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case OSPFROUTERCONFIGLOCAL :
                pPrismMessage = new OSPFRouterConfigLocalMessage ();
                break;
            case OSPFAREALOCAL :
                pPrismMessage = new OSPFAreaLocalMessage ();
                break;
			case OSPFPERMITDENYCONFIGLOCAL:
				pPrismMessage = new OSPFPermitDenyRedistributeLocalMessage ();
				break;
            case OSPFVIRTUALLINKLOCAL:
                pPrismMessage = new OSPFVirtualLinkLocalMessage ();
                break;
                
		case OSPFAREARANGELOCAL :
			pPrismMessage = new OSPFAreaRangeLocalMessage ();
			break;
		case OSPFLOCALSHOW:
			pPrismMessage = new OSPFLocalShowMessage();
			break;
		case OSPFLOCALCLEAR:
			pPrismMessage = new OSPFLocalClearMessage();
			break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

	WaveManagedObject *OSPFLocalConfWorker::createLocalManagedObjectFromId(OSPFManagedObjectId managedObjectId,
			DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
        WaveManagedObject *pWaveManagedObject = NULL;
		OSPFConfigLocalManagedObject *pOSPFConfigMO = NULL;

		switch (managedObjectId)
		{
			case OSPF_CONFIG_MANAGED_OBJECT:
				pOSPFConfigMO = new OSPFConfigLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
				updateWaveManagedObject(pOSPFConfigMO);
				pWaveManagedObject = pOSPFConfigMO;
				break;
			case OSPF_DISTANCE_MANAGED_OBJECT:
			{
				OSPFDistanceLocalManagedObject *pMO = new OSPFDistanceLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
				pWaveManagedObject = pMO;
				pOSPFConfigMO = getOSPFConfigManagedObject(pPrismSynchronousLinearSequencerContext);
				prismAssert(NULL != pOSPFConfigMO, __FILE__, __LINE__);
				pOSPFConfigMO->addDistanceManagedObject(pMO);
				updateWaveManagedObject(pOSPFConfigMO);
			}
				break;
			case OSPF_SUMMARY_ADDRESS_MANAGED_OBJECT:
			{
				OSPFSummaryAddressLocalManagedObject *pMO = new OSPFSummaryAddressLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
				pWaveManagedObject = pMO;
				pOSPFConfigMO = getOSPFConfigManagedObject(pPrismSynchronousLinearSequencerContext);
				prismAssert(NULL != pOSPFConfigMO, __FILE__, __LINE__);
				pOSPFConfigMO->addSummaryAddressManagedObject(pMO);
				updateWaveManagedObject(pMO);
				updateWaveManagedObject(pOSPFConfigMO);
			}
				break;
			case OSPF_NEIGHBOR_IP_ADDRESS_MANAGED_OBJECT:
			{
				OSPFNeighborIPAddressLocalManagedObject *pMO = new OSPFNeighborIPAddressLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
				pWaveManagedObject = pMO;
				pOSPFConfigMO = getOSPFConfigManagedObject(pPrismSynchronousLinearSequencerContext);
				prismAssert(NULL != pOSPFConfigMO, __FILE__, __LINE__);
				pOSPFConfigMO->addNeighborIPAddressManagedObject(pMO);
				updateWaveManagedObject(pMO);
				updateWaveManagedObject(pOSPFConfigMO);
			}
				break;
			default:
				trace (TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::createManagedObjectInstanceFromId : Unknown Managed Class Id : " + managedObjectId);
				return NULL;
		}
		return pWaveManagedObject;
	}

    WaveManagedObject  *OSPFLocalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((OSPFNeighborIPAddressLocalManagedObject::getClassName ()) == managedClassName)
        {
			pWaveManagedObject = new OSPFNeighborIPAddressLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((OSPFSummaryAddressLocalManagedObject::getClassName ()) == managedClassName)
        {
			pWaveManagedObject = new OSPFSummaryAddressLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((OSPFDistanceLocalManagedObject::getClassName ()) == managedClassName)
        {
			pWaveManagedObject = new OSPFDistanceLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((OSPFAreaRangeLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new OSPFAreaRangeLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((OSPFVirtualLinkLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new OSPFVirtualLinkLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((OSPFAreaLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new OSPFAreaLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((OSPFPermitDenyRedistributeLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new OSPFPermitDenyRedistributeLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((OSPFPermitRedistributeLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new OSPFPermitRedistributeLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((OSPFDenyRedistributeLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new OSPFDenyRedistributeLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
		}
        else if ((OSPFConfigLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new OSPFConfigLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    string  OSPFLocalConfWorker::getManagedObjectClassNameFromId(const OSPFManagedObjectId &managedObjectId)
	{
		switch (managedObjectId)
		{
			case OSPF_CONFIG_MANAGED_OBJECT:
				return OSPFConfigLocalManagedObject::getClassName();
			case OSPF_DISTANCE_MANAGED_OBJECT:
				return OSPFDistanceLocalManagedObject::getClassName();
			case OSPF_SUMMARY_ADDRESS_MANAGED_OBJECT:
				return OSPFSummaryAddressLocalManagedObject::getClassName();
			case OSPF_NEIGHBOR_IP_ADDRESS_MANAGED_OBJECT:
				return OSPFNeighborIPAddressLocalManagedObject::getClassName();
			default:
				trace (TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::createManagedObjectInstanceFromId : Unknown Managed Class Id : " + managedObjectId);
				return string("");
		}
	}

	void OSPFLocalConfWorker::updateManagedObjectFromLocalMessage(WaveManagedObject *pMO, OSPFRouterConfigLocalMessage *pMsg)
	{
		trace (TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::updateManagedObjectFromLocalMessage : MO Id : ") + pMsg->getManagedObjectId());

		switch (pMsg->getManagedObjectId())
		{
			case OSPF_CONFIG_MANAGED_OBJECT:
				return (dynamic_cast<OSPFConfigLocalManagedObject*>(pMO))->updateMOFromLocalMessage(pMsg);
			case OSPF_DISTANCE_MANAGED_OBJECT:
				return (dynamic_cast<OSPFDistanceLocalManagedObject*>(pMO))->updateMOFromLocalMessage(pMsg);
			case OSPF_SUMMARY_ADDRESS_MANAGED_OBJECT:
				return (dynamic_cast<OSPFSummaryAddressLocalManagedObject*>(pMO))->updateMOFromLocalMessage(pMsg);
			case OSPF_NEIGHBOR_IP_ADDRESS_MANAGED_OBJECT:
				return (dynamic_cast<OSPFNeighborIPAddressLocalManagedObject*>(pMO))->updateMOFromLocalMessage(pMsg);
			default:
				trace (TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::updateManagedObjectFromLocalMessage : Unknown Managed Class Id : " + pMsg->getManagedObjectId());
				return ;
		}
	}

   ResourceId OSPFLocalConfWorker::validateVRF(OSPFRouterConfigLocalMessage *pMsg)
   {
        ObjectId vrfObjectId;
	if(pMsg) {
		string vrfName = pMsg->getPartitionName();
		string defaultVrfName = DEFAULT_VRF_NAME;
		int defVrfNameSize = defaultVrfName.length();
		NSM_PLUG_DBG_S(string("Default VRF Name:")+defaultVrfName+":Length of default VRF Name:"+defVrfNameSize);
		//Need to see if the non-default VRF is created before configuring ospf
		if(strncmp(vrfName.c_str(), defaultVrfName.c_str(), defVrfNameSize)) {
			ResourceId queryStatus=VrfUtils::getVrfObjectIdByNameAndValidate(vrfName,vrfObjectId,FrameworkToolKit::getThisLocationId());
			return queryStatus;
		}
		else {
			   NSM_PLUG_DBG_S(string("Configuring ospf for default VRF. No need to check whether the VRF MO exists"));
		}
	}
	return WAVE_MESSAGE_SUCCESS;
   }

	ResourceId OSPFLocalConfWorker::ospfRouterConfigStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
		NSM_PLUG_DBG_S(string("Entering:"));
		ResourceId status = WAVE_MESSAGE_SUCCESS;
                OSPFConfigLocalManagedObject *pOSPFConfigLocalMO = NULL;
        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();

		OSPFRouterConfigLocalMessage *pOSPFRouterConfigLocalMessage =
			dynamic_cast<OSPFRouterConfigLocalMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

		status = validateVRF(pOSPFRouterConfigLocalMessage);

		if(status != WAVE_MESSAGE_SUCCESS) {
			NSM_PLUG_DBG_S(string("Validations failed:"));
			return status;
		}
                if(pOSPFRouterConfigLocalMessage->getVRF() !=DEFAULT_VRF_NAME){
                    if(pOSPFRouterConfigLocalMessage->getIsMaxMetricOnStartupWaitForBgp()==true) {
		         trace(TRACE_LEVEL_ERROR, "OSPFLocalConfWorker::Wait for bgp can be configured only on default-vrf");
                         return WRC_OSPF_ERR_WAIT_FOR_BGP_NON_DEF_VRF;
                   }
                }
                if((pOSPFRouterConfigLocalMessage->getIsMaxMetricOnStartupWaitForBgp())&& 
                  (pOSPFRouterConfigLocalMessage->getMaxMetricOnStartupTime()!=0))
                {
	           trace(TRACE_LEVEL_ERROR, "OSPFLocalConfWorker:Both On startup time and wait-for-bgp cannot be configured ");
                   return WRC_OSPF_ERR_ONSTARTUP_TIME_AND_WAIT_FOR_BGP_CONFIGURED;
                }
                if(pOSPFRouterConfigLocalMessage->getCmdCode() == OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA) 
                {
                         pOSPFConfigLocalMO = getOSPFConfigManagedObject (pPrismSynchronousLinearSequencerContext);
	        if(pOSPFRouterConfigLocalMessage->getOnStartup())
                {
                     if((!pOSPFRouterConfigLocalMessage->getIsMaxMetricOnStartupWaitForBgp())
                        &&(!pOSPFRouterConfigLocalMessage->getMaxMetricOnStartupTime())
                        &&((pOSPFConfigLocalMO !=NULL)&&(!pOSPFConfigLocalMO->getMaxMetricOnStartupTime())&&(!pOSPFConfigLocalMO->getMaxMetricOnStartupWaitForBgp())))
                         {    if((pOSPFRouterConfigLocalMessage->getOpCode()!=OSPF_REMOVE)&&(pOSPFRouterConfigLocalMessage->getCmdType() != OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP)&&
                              (pOSPFRouterConfigLocalMessage->getCmdType() !=OSPF_SET_MAX_METRIC_STARTUP_TIME))
                              {
				  trace(TRACE_LEVEL_ERROR, "OSPFLocalConfWorker:On startup time /wait-for-bgp not configured hence command not supported");
                                  return WRC_OSPF_ERR_ONSTARTUP_TIME_OR_WAIT_FOR_BGP_NOT_CONFIGURED;
                              }

                          }
                  }
                 }	
		OSPFRouterConfigMessage *pOSPFRouterConfigMessage = new OSPFRouterConfigMessage();
		pOSPFRouterConfigMessage->copyFromLocalMessage(pOSPFRouterConfigLocalMessage);
                  //Edit pOSPFRouter ConfigMessgae To append al MaxMetricLink
                if(pOSPFRouterConfigLocalMessage->getCmdCode() == OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA) {
                if(/*(pOSPFRouterConfigLocalMessage->getMaxMetricLinkType())&&*/(pOSPFRouterConfigLocalMessage->getOpCode()!=OSPF_REMOVE))
               {
                  bool linkTypeAppended =0;
                       if((!pOSPFRouterConfigLocalMessage->getLinkAll())&&(pOSPFConfigLocalMO !=NULL))
                       {
                             trace(TRACE_LEVEL_DEVEL,string("Value of transit link onStartup")+pOSPFConfigLocalMO->getIsOnStartupLinkTransit());
                             trace(TRACE_LEVEL_DEVEL,string("Value of transit link")+pOSPFConfigLocalMO->getIsLinkTransit());
	                     if(!pOSPFRouterConfigLocalMessage->getOnStartup())
                             {
                              if((!pOSPFRouterConfigLocalMessage->getLinkPtp())&&((pOSPFConfigLocalMO->getIsLinkPtp())/*||(pOSPFConfigLocalMO->getIsOnStartupLinkPtp())*/))
                              {
                                  pOSPFRouterConfigMessage->setMaxMetricLinkType(pOSPFRouterConfigMessage->getMaxMetricLinkType()|OSPF_MAX_METRIC_LINK_TYPE_PTP);
                                  linkTypeAppended =1;
                              }
                              if((!pOSPFRouterConfigLocalMessage->getLinkTransit())&&((pOSPFConfigLocalMO->getIsLinkTransit())/*||(pOSPFConfigLocalMO->getIsOnStartupLinkTransit())*/))
                              {
                                  pOSPFRouterConfigMessage->setMaxMetricLinkType(pOSPFRouterConfigMessage->getMaxMetricLinkType()|OSPF_MAX_METRIC_LINK_TYPE_TRANSIT);
                                  linkTypeAppended =1;
                              }
                              if((!pOSPFRouterConfigLocalMessage->getLinkStub())&&((pOSPFConfigLocalMO->getIsLinkStub())/*||(pOSPFConfigLocalMO->getIsOnStartupLinkStub())*/))
                              {
                                  pOSPFRouterConfigMessage->setMaxMetricLinkType(pOSPFRouterConfigMessage->getMaxMetricLinkType()|OSPF_MAX_METRIC_LINK_TYPE_STUB);
                                  linkTypeAppended =1;
                              }
                            }
                            else 
                            {
                              if((!pOSPFRouterConfigLocalMessage->getLinkPtp())&&(/*(pOSPFConfigLocalMO->getIsLinkPtp())||*/(pOSPFConfigLocalMO->getIsOnStartupLinkPtp())))
                              {
                                  pOSPFRouterConfigMessage->setMaxMetricLinkType(pOSPFRouterConfigMessage->getMaxMetricLinkType()|OSPF_MAX_METRIC_LINK_TYPE_PTP);
                                  linkTypeAppended =1;
                              }
                              if((!pOSPFRouterConfigLocalMessage->getLinkTransit())&&(/*(pOSPFConfigLocalMO->getIsLinkTransit())||*/(pOSPFConfigLocalMO->getIsOnStartupLinkTransit())))
                              {
                                  pOSPFRouterConfigMessage->setMaxMetricLinkType(pOSPFRouterConfigMessage->getMaxMetricLinkType()|OSPF_MAX_METRIC_LINK_TYPE_TRANSIT);
                                  linkTypeAppended =1;
                              }
                              if((!pOSPFRouterConfigLocalMessage->getLinkStub())&&(/*(pOSPFConfigLocalMO->getIsLinkStub())||*/(pOSPFConfigLocalMO->getIsOnStartupLinkStub())))
                              {
                                  pOSPFRouterConfigMessage->setMaxMetricLinkType(pOSPFRouterConfigMessage->getMaxMetricLinkType()|OSPF_MAX_METRIC_LINK_TYPE_STUB);
                                  linkTypeAppended =1;
                              }

                            }
                               if((linkTypeAppended) &&(!(pOSPFRouterConfigLocalMessage->getCmdType()& OSPF_SET_MAX_METRIC_LINK_TYPE)))
                                 pOSPFRouterConfigMessage->setCmdType(pOSPFRouterConfigMessage->getCmdType()|OSPF_SET_MAX_METRIC_LINK_TYPE);
                       }

               }
//Update cmdType before sending to backend so taht backend can recognize it is an onstartup time or waitforBgp comamnd
	        if(pOSPFRouterConfigLocalMessage->getOnStartup())
                {
                     if((!pOSPFRouterConfigLocalMessage->getIsMaxMetricOnStartupWaitForBgp())
                        &&(!pOSPFRouterConfigLocalMessage->getMaxMetricOnStartupTime()));
                     {
                        if(pOSPFConfigLocalMO !=NULL)
                        {
                            if(pOSPFConfigLocalMO->getMaxMetricOnStartupTime())
                               pOSPFRouterConfigMessage->setCmdType(OSPF_SET_MAX_METRIC_STARTUP_TIME|pOSPFRouterConfigMessage->getCmdType());
                            if(pOSPFConfigLocalMO->getMaxMetricOnStartupWaitForBgp())
                               pOSPFRouterConfigMessage->setCmdType(OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP|pOSPFRouterConfigMessage->getCmdType());
                        }
                     }
               }
              }
		NSM_PLUG_DBG_S(string("ospfRouterConfigStep ospf router-config Local message recived for VRF:")+pOSPFRouterConfigLocalMessage->getPartitionName());
             if (pOSPFConfigLocalMO == NULL)
                 pOSPFConfigLocalMO = getOSPFConfigManagedObject (pPrismSynchronousLinearSequencerContext);
             if( pOSPFConfigLocalMO )
             {
            	  trace (TRACE_LEVEL_DEVEL, string("OSPFConfigLocalManagedObject is not null ") );
		  pPrismSynchronousLinearSequencerContext->getCache()->put(OSPFConfigLocalManagedObject::getClassName(), pOSPFConfigLocalMO);
             }      
		if (pOSPFRouterConfigMessage->getOpCode() == OSPF_REMOVE)
		{
			NSM_PLUG_INFO_S(string("Operation Delete:"));
			prismAssert(NULL != pOSPFConfigLocalMO, __FILE__, __LINE__);

			switch(pOSPFRouterConfigMessage->getCmdCode())
			{
				case OSPF_ROUTER_CMD_REDISTRIBUTION_CONNECTED_MAP:
					pOSPFRouterConfigMessage->setRedistributeConnectedRouteMap(pOSPFConfigLocalMO->getRedistributeConnectedMapName());
					trace (TRACE_LEVEL_INFO, string("Connected Map Name: ") + pOSPFRouterConfigMessage->getRedistributeConnectedRouteMap());
					break;

				case OSPF_ROUTER_CMD_REDISTRIBUTION_BGP_MAP:
					pOSPFRouterConfigMessage->setRedistributeBgpRouteMap(pOSPFConfigLocalMO->getRedistributeBgpMapName());
					trace (TRACE_LEVEL_INFO, string("Bgp Map Name: ") + pOSPFRouterConfigMessage->getRedistributeBgpRouteMap());
					break;

				case OSPF_ROUTER_CMD_REDISTRIBUTION_STATIC_MAP:
					pOSPFRouterConfigMessage->setRedistributeStaticRouteMap(pOSPFConfigLocalMO->getRedistributeStaticMapName());
					trace (TRACE_LEVEL_INFO, string("Static Map Name: ") + pOSPFRouterConfigMessage->getRedistributeStaticRouteMap());
					break;

				case OSPF_ROUTER_CMD_DEF_INFO_ORIGINATE:
					if (pOSPFRouterConfigLocalMessage->getDefInfoOrigCmd() & OSPF_CONFIG_DEF_INFO_ORIG_METRIC)
						pOSPFRouterConfigMessage->setDefInfoOrigMetric(pOSPFConfigLocalMO->getDefInfoOrigMetric());

					if (pOSPFRouterConfigLocalMessage->getDefInfoOrigCmd() & OSPF_CONFIG_DEF_INFO_ORIG_METRIC_TYPE)
						pOSPFRouterConfigMessage->setDefInfoOrigMetricType(pOSPFConfigLocalMO->getDefInfoOrigMetricType());

                default:
                    break;
			}
		}
        else
        {   
            if( pOSPFConfigLocalMO )
            {
            	trace (TRACE_LEVEL_DEVEL, string("OSPFConfigLocalManagedObject is not null ") );
                switch(pOSPFRouterConfigMessage->getCmdCode())
                {
                    case OSPF_ROUTER_CMD_TIMER_THROTTLE:
                        if((pOSPFRouterConfigMessage->getTimerInitDelay()) == OSPF_TIMER_SPF_INVALID)
                        {
                            pOSPFRouterConfigMessage->setTimerInitDelay(pOSPFConfigLocalMO->getTimerInitDelay());
                            pOSPFRouterConfigLocalMessage->setTimerInitDelay(pOSPFConfigLocalMO->getTimerInitDelay());
                        }    
                        if((pOSPFRouterConfigMessage->getTimerHoldTime()) == OSPF_TIMER_SPF_INVALID)
                        {
                            pOSPFRouterConfigMessage->setTimerHoldTime(pOSPFConfigLocalMO->getTimerHoldTime());
                            pOSPFRouterConfigLocalMessage->setTimerHoldTime(pOSPFConfigLocalMO->getTimerHoldTime());
                        }    
                        if((pOSPFRouterConfigMessage->getTimerMaxHoldTime()) == OSPF_TIMER_SPF_INVALID)
                        {
                            pOSPFRouterConfigMessage->setTimerMaxHoldTime(pOSPFConfigLocalMO->getTimerMaxHoldTime());
                            pOSPFRouterConfigLocalMessage->setTimerMaxHoldTime(pOSPFConfigLocalMO->getTimerMaxHoldTime());
                        }    

                        if((pOSPFRouterConfigMessage->getTimerMaxHoldTime()) < (pOSPFRouterConfigMessage->getTimerHoldTime()))
                        {
                            status = WRC_OSPF_CLI_ERR_HLD_GT_MAXHLD;
                            pOSPFRouterConfigLocalMessage->setCompletionStatus(status);
                            delete (pOSPFRouterConfigMessage);
                            return (status);
                        }

                        break;    

                    default:
                        break;

                }
            }
        }


        status = sendSynchronouslyToWaveClient("ospf",pOSPFRouterConfigMessage);

		if(status == WAVE_MESSAGE_SUCCESS)
		{
			status = pOSPFRouterConfigMessage->getClientStatus();
	        trace (TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorker::ospfRouterConfigStep ospf router-config Local message sent to OSPF backend, client status is : ") + FrameworkToolKit::localize(status));
		}
		else
	        trace (TRACE_LEVEL_ERROR, "OSPFLocalConfWorker::ospfRouterConfigStep ospf router-config Local message not sent to OSPF backend");

		pOSPFRouterConfigLocalMessage->setCompletionStatus(status);
        delete (pOSPFRouterConfigMessage);
        return (status);
	}

	ResourceId OSPFLocalConfWorker::deleteOSPFIntfConfig(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
		NSM_PLUG_DBG_S(string("Entering:"));
		ResourceId status = WAVE_MESSAGE_SUCCESS;
        
		OSPFRouterConfigLocalMessage *pOSPFRouterConfigLocalMessage =
			dynamic_cast<OSPFRouterConfigLocalMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
		
		if(pOSPFRouterConfigLocalMessage->getOpCode()!= OSPF_REMOVE || 
		   pOSPFRouterConfigLocalMessage->getCmdCode() != OSPF_ROUTER_CMD_GLOBAL_CONFIG)
			return(WAVE_MESSAGE_SUCCESS);

		OSPFInterfaceLocalMessage *pOSPFInterfaceLocalMessage = new OSPFInterfaceLocalMessage();
			
		pOSPFInterfaceLocalMessage->setOpCode(OSPF_REMOVE);
		pOSPFInterfaceLocalMessage->setCmdCode(OSPF_INTF_DEL);
		NSM_PLUG_DBG_S(string("VRF Name from the message:")+pOSPFRouterConfigLocalMessage->getVRF());
		pOSPFInterfaceLocalMessage->setPartitionName(pOSPFRouterConfigLocalMessage->getVRF());

		if ((status = sendSynchronously(pOSPFInterfaceLocalMessage)) == WAVE_MESSAGE_SUCCESS)
		{
			trace (TRACE_LEVEL_DEVEL, "OSPFLocalConfWorker::deleteOSPFIntfConfig ospf interface-config Local message sent successfully");
			status = pOSPFInterfaceLocalMessage->getCompletionStatus();
		}

		pOSPFRouterConfigLocalMessage->setCompletionStatus(status);
        delete (pOSPFInterfaceLocalMessage);
        return (status);
	}

	vector<WaveManagedObject *> *OSPFLocalConfWorker::queryManagedObject(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
        OSPFRouterConfigLocalMessage *pMsg =
            dynamic_cast<OSPFRouterConfigLocalMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
		int route_type = 0;
		LocationId locId =0;
		IpV4Address address, mask;
		trace(TRACE_LEVEL_INFO,string("ManagedObjectId") +pMsg->getManagedObjectId());
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(getManagedObjectClassNameFromId(pMsg->getManagedObjectId()));
        string vrfName = pMsg->getPartitionName();
        //string ownerkeystring =
        trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::queryManagedObject VRF Name:")+vrfName);

        OSPFConfigLocalManagedObject *ospfMOForVRF = dynamic_cast<OSPFConfigLocalManagedObject *> (pPrismSynchronousLinearSequencerContext->
        		getCache()->getWaveManagedObject(OSPFConfigLocalManagedObject::getClassName()));

        if(ospfMOForVRF) {
        	ObjectId objectId = ospfMOForVRF->getOwnerPartitionManagedObjectId();
        	trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::queryManagedObject OwnerPartitionManagedObjectId:")+objectId.toString());
        	syncQueryCtxt.addAndAttribute(new AttributeObjectId(&objectId, "ownerPartitionManagedObjectId"));
        }
        else {
        	trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::queryManagedObject OSPFConfigLocalManagedObject from cache is NULL:"));
        }

		switch(pMsg->getManagedObjectId())
		{
			case OSPF_CONFIG_MANAGED_OBJECT:
			{
				syncQueryCtxt.addAndAttribute (new AttributeString(&vrfName, "vrf"));
				break;
			}
			case OSPF_DISTANCE_MANAGED_OBJECT:
				switch (pMsg->getCmdCode())
				{
					case OSPF_ROUTER_CMD_DISTANCE_EXTERNAL:
						route_type = OSPF_ROUTE_TYPE_EXTERNAL;
						break;
					case OSPF_ROUTER_CMD_DISTANCE_INTER_AREA:
						route_type = OSPF_ROUTE_TYPE_INTER_AREA;
						break;
					case OSPF_ROUTER_CMD_DISTANCE_INTRA_AREA:
						route_type = OSPF_ROUTE_TYPE_INTRA_AREA;
						break;
				}
				syncQueryCtxt.addAndAttribute(new AttributeEnum((UI32*)&route_type, "distanceType"));
				break;
			case OSPF_SUMMARY_ADDRESS_MANAGED_OBJECT:
				address = pMsg->getSummaryAddress();
				mask = pMsg->getSummaryMask();
				syncQueryCtxt.addAndAttribute(new AttributeIpV4Address(&address, "summaryAddress"));
				/*New value of Mask should overwrite the entry. So search based on Dest alone*/
				//syncQueryCtxt.addAndAttribute(new AttributeIpV4Address(&mask, "summaryMask"));
				break;
			case OSPF_NEIGHBOR_IP_ADDRESS_MANAGED_OBJECT:
				address = pMsg->getNeighborAddress();
				syncQueryCtxt.addAndAttribute(new AttributeIpV4Address(&address, "neighborAddress"));
				break;
			default:
				return NULL;
		}
		vector<WaveManagedObject *> *pResults = NULL;
		locId = FrameworkToolKit::getThisLocationId();
		pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId,&syncQueryCtxt);
		return pResults;
	}

	OSPFConfigLocalManagedObject *OSPFLocalConfWorker::getOSPFConfigManagedObject(
			DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
		trace(TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::getOSPFConfigManagedObject Entering"));
		OSPFConfigLocalManagedObject *pMO = NULL;
		string vrf = pPrismSynchronousLinearSequencerContext->getPPrismMessage()->getPartitionName();
		LocationId locId = 0;
		locId = FrameworkToolKit::getThisLocationId();
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(OSPFConfigLocalManagedObject::getClassName());
        	trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::getOSPFConfigManagedObject VRF Name-")+vrf);
		syncQueryCtxt.addAndAttribute (new AttributeString(&vrf, "vrf"));
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId,&syncQueryCtxt);
		
		if (NULL != pResults)
		{
			trace(TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorker::OSPFConfigLocalManagedObject: NULL != pResults "));
			UI32 numberOfResults = pResults->size ();
			trace(TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorker::OSPFConfigLocalManagedObject: numberOfResults=")+numberOfResults);
			if (1 == numberOfResults) {
				pMO = dynamic_cast<OSPFConfigLocalManagedObject *>((*pResults)[0]);
				NSM_PLUG_DBG_S(string("Found OSPFConfigLocalManagedObject"));
			} else {
				pMO = NULL;
			}
			NSM_PLUG_DBG_S(string("Add the pResults for garbage collection"));
			pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			pResults->clear();
			delete pResults;
		}
		else {
			trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFConfigLocalManagedObject does not exist"));
		}
		trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::getOSPFConfigManagedObject Exiting"));
		return pMO;
	}
	OSPFConfigLocalManagedObject *OSPFLocalConfWorker::getOSPFConfigManagedObject(string vrf)
	{
		trace(TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorke-Linear clear::getOSPFConfigManagedObject Entering"));
		OSPFConfigLocalManagedObject *pMO = NULL;
		LocationId locId = 0;
		locId = FrameworkToolKit::getThisLocationId();
                vector<string> selectFields;
                 selectFields.push_back ("routerLsa");
                 selectFields.push_back ("summMaxMetric");
                 selectFields.push_back ("extMaxMetric");
                 selectFields.push_back ("summMaxMetricLsa");
                 selectFields.push_back ("extMaxMetricLsa");
                 selectFields.push_back ("linkPtp");
                 selectFields.push_back ("linkStub");
                 selectFields.push_back ("linkTransit");
                 selectFields.push_back ("maxMetricOnStartup");
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(OSPFConfigLocalManagedObject::getClassName());
        	trace(TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::getOSPFConfigManagedObject for clear VRF Name-")+vrf);
		syncQueryCtxt.addAndAttribute (new AttributeString(&vrf, "vrf"));
                syncQueryCtxt.addSelectFields(selectFields);
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId,&syncQueryCtxt);
		
		if (NULL != pResults)
		{
			trace(TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorker::OSPFConfigLocalManagedObject: NULL != pResults "));
			UI32 numberOfResults = pResults->size ();
			trace(TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorker::OSPFConfigLocalManagedObject: numberOfResults=")+numberOfResults);
			if (1 == numberOfResults) {
				pMO = dynamic_cast<OSPFConfigLocalManagedObject *>((*pResults)[0]);
				NSM_PLUG_DBG_S(string("Found OSPFConfigLocalManagedObject"));
			} else {
				pMO = NULL;
			}
			NSM_PLUG_DBG_S(string("Add the pResults for garbage collection"));
			pResults->clear();
			delete pResults;
		}
		else {
			trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorke-clear::OSPFConfigLocalManagedObject does not exist"));
		}
		trace(TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::getOSPFConfigManagedObject Exiting"));
		return pMO;
	}

	void OSPFLocalConfWorker::deleteManagedObject(WaveManagedObject *pMO, OSPFRouterConfigLocalMessage *pMsg, 
			DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
		OSPFConfigLocalManagedObject *pOSPFConfigMO = getOSPFConfigManagedObject(pPrismSynchronousLinearSequencerContext);
		OSPFSummaryAddressLocalManagedObject *pOSPFSummaryAddressMO = NULL;

		if (pOSPFConfigMO == NULL)
		{
			trace(TRACE_LEVEL_ERROR, "OSPFLocalConfWorker::deleteManagedObject OSPFConfigMO not found.");
			return ;
		}

		switch (pMsg->getManagedObjectId())
		{
			case OSPF_CONFIG_MANAGED_OBJECT:
				if (pMsg->getCmdCode() == OSPF_ROUTER_CMD_GLOBAL_CONFIG) {
					delete (pMO);
				}
				else
				{
					updateMoForAllVrf(pMO, pMsg,pPrismSynchronousLinearSequencerContext);
				}
				break;
			case OSPF_DISTANCE_MANAGED_OBJECT:
				pOSPFConfigMO->deleteDistanceManagedObject((dynamic_cast<OSPFDistanceLocalManagedObject*>(pMO))->getDistanceType());
				updateWaveManagedObject(pOSPFConfigMO);
				break;
			case OSPF_SUMMARY_ADDRESS_MANAGED_OBJECT:
				pOSPFSummaryAddressMO = dynamic_cast<OSPFSummaryAddressLocalManagedObject*>(pMO);
				pOSPFConfigMO->deleteSummaryAddressManagedObject(pOSPFSummaryAddressMO->getSummaryAddress(), pOSPFSummaryAddressMO->getSummaryMask());
				updateWaveManagedObject(pOSPFConfigMO);
				break;
			case OSPF_NEIGHBOR_IP_ADDRESS_MANAGED_OBJECT:
				pOSPFConfigMO->deleteNeighborIPAddressManagedObject((dynamic_cast<OSPFNeighborIPAddressLocalManagedObject*>(pMO))->getNeighborAddress());
				updateWaveManagedObject(pOSPFConfigMO);
				break;
			default:
				return ;
		}
	}

	void OSPFLocalConfWorker::updateMoForAllVrf(WaveManagedObject *pMO, OSPFRouterConfigLocalMessage *pMsg,DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext) 
	{
		OSPFConfigLocalManagedObject  *pMOAllVrf = NULL;
		if(!pMsg->getIsAllVrfSet())
		{
			updateManagedObjectFromLocalMessage(pMO, pMsg);
			updateWaveManagedObject(pMO);
		}
		else
		{
			LocationId locId = FrameworkToolKit::getThisLocationId();
			WaveManagedObjectSynchronousQueryContext syncQueryCtxt(OSPFConfigLocalManagedObject::getClassName());
			vector<WaveManagedObject *> *pResultsAllVrf = querySynchronouslyLocalManagedObjectsForLocationId (locId,&syncQueryCtxt);
			if (NULL != pResultsAllVrf)
			{
				trace(TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::OSPFConfigLocalManagedObject:bbdel NULL != pResults "));
				UI32 numberOfResultsAllVrf = pResultsAllVrf->size ();
				trace(TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorker::OSPFConfigLocalManagedObject: bbdel numberOfResults=")+numberOfResultsAllVrf);
				if (1 == numberOfResultsAllVrf)
				{
					pMOAllVrf = dynamic_cast<OSPFConfigLocalManagedObject *>((*pResultsAllVrf)[0]);
					NSM_PLUG_DBG_S(string("Found OSPFConfigLocalManagedObject"));
					if(pMOAllVrf !=NULL)
					{
						updateManagedObjectFromLocalMessage(pMOAllVrf, pMsg);
						updateWaveManagedObject(pMOAllVrf);
					}
				}
				else if ( numberOfResultsAllVrf > 1)
				{
					for(UI32 i=0;i<numberOfResultsAllVrf ;i++)
					{
						pMOAllVrf= dynamic_cast<OSPFConfigLocalManagedObject *>((*pResultsAllVrf)[i]);
						if(pMOAllVrf !=NULL)
						{
                                                            if((pMsg->getOnStartup())&&(((pMsg->getCmdType())&OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP)||
                                                              ((pMOAllVrf->getMaxMetricOnStartupWaitForBgp())&&(!((pMsg->getCmdType())&OSPF_SET_MAX_METRIC_STARTUP_TIME)))))
                                                                      if(pMOAllVrf->getVRF() != DEFAULT_VRF_NAME)
                                                                         continue;
							pMsg->setVRF(pMOAllVrf->getVRF());
							pMsg->setPartitionName(pMOAllVrf->getUserSpecifiedPartitionName());
							trace(TRACE_LEVEL_DEBUG,string("Localconfworker-VRFname")+ pMsg->getVRF() +string("partitionName")+ pMsg->getPartitionName());

							updateManagedObjectFromLocalMessage(pMOAllVrf, pMsg);
							updateWaveManagedObject(pMOAllVrf);
						}
					}
				}
				else if (0==numberOfResultsAllVrf)
				{
					pMOAllVrf = NULL;
				}
				NSM_PLUG_DBG_S(string("Add the pResults for garbage collection"));
				pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsAllVrf);
				pResultsAllVrf->clear();
				delete pResultsAllVrf;
			}
		}
	}






	void OSPFLocalConfWorker::deletePermitDenyLocalManagedObject(OSPFPermitDenyRedistributeLocalManagedObject *pMO, OSPFPermitDenyRedistributeLocalMessage *pMsg,DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
		OSPFConfigLocalManagedObject *pOSPFConfigMO = getOSPFConfigManagedObject(pPrismSynchronousLinearSequencerContext);

		trace (TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorker::deletePermitDenyLocalManagedObject delete permit/deny = ") + pMsg->getPermitDenyOption());
		if (pOSPFConfigMO == NULL) 
		{
			trace(TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::deletePermitDenyLocalManagedObject OSPFConfigMO not found.");
			return ;
		}

		pOSPFConfigMO->deletePermitDenyManagedObject(pMO->getIndex());
		updateWaveManagedObject(pOSPFConfigMO);
	}

    ResourceId OSPFLocalConfWorker::ospfRouterConfigMOStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
		WaveManagedObject  *pMO = NULL;
		UI32 numberOfResults = 0;

        OSPFRouterConfigLocalMessage *pOSPFRouterConfigLocalMessage =
            dynamic_cast<OSPFRouterConfigLocalMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

		vector<WaveManagedObject *> *pResults = NULL;
		pResults = queryManagedObject(pPrismSynchronousLinearSequencerContext);

		if (NULL != pResults) 
		{
			numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_DEVEL, "OSPFLocalConfWorker::ospfRouterConfigMOStep : Could not" 
					" find the OSPF config MO");
			}else if (1 == numberOfResults) {
				pMO = dynamic_cast<WaveManagedObject *>((*pResults)[0]);
				trace (TRACE_LEVEL_DEVEL, "OSPFLocalConfWorker::ospfRouterConfigMOStep : Found the" 
					" OSPF config MO");
			} else {
				trace (TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::ospfRouterConfigMOStep :" 
					" Two or more OSPF config MO's exists");

				pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
				delete (pResults);
				return WAVE_MESSAGE_ERROR;
			}
		}

		trace (TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorker::ospfRouterConfigMOStep MO ") +
				getManagedObjectClassNameFromId(pOSPFRouterConfigLocalMessage->getManagedObjectId()) +
				" no. of results=" + numberOfResults + " opcode " + pOSPFRouterConfigLocalMessage->getOpCode());

		if (pMO == NULL) 
		{
            if(pOSPFRouterConfigLocalMessage->getOpCode() != OSPF_CREATE)
            {
				if (pResults !=NULL)
				{
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
					delete (pResults);
				}
				return WAVE_MESSAGE_ERROR;
            }
			else
			{
				/* Create the OSPF config managed object*/
		            
				pMO = createLocalManagedObjectFromId(pOSPFRouterConfigLocalMessage->getManagedObjectId(),
					  pPrismSynchronousLinearSequencerContext);
				if (pMO == NULL) {
					trace(TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::ospfRouterConfigMOStep MO creation failed.");
					if (pResults !=NULL)
					{
						pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
						delete (pResults);
					}
					return WAVE_MESSAGE_ERROR;
				}
				trace(TRACE_LEVEL_DEVEL, "OSPFLocalConfWorker::ospfRouterConfigMOStep created OSPF Config MO");

				updateManagedObjectFromLocalMessage(pMO, pOSPFRouterConfigLocalMessage);

				if(pOSPFRouterConfigLocalMessage->getManagedObjectId() == OSPF_CONFIG_MANAGED_OBJECT)
					pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection (pMO);
                     }	
		}
		
		else
		{
            if(pOSPFRouterConfigLocalMessage->getOpCode() == OSPF_CREATE)
            {
				trace(TRACE_LEVEL_DEBUG, "OSPFLocalConfWorker::ospfRouterConfigMOStep updating OSPFMO");
				updateMoForAllVrf(pMO, pOSPFRouterConfigLocalMessage, pPrismSynchronousLinearSequencerContext);
            }
			else if(pOSPFRouterConfigLocalMessage->getOpCode() == OSPF_REMOVE)
			{
				deleteManagedObject(pMO, pOSPFRouterConfigLocalMessage,pPrismSynchronousLinearSequencerContext);

				if ((pResults !=NULL) &&(pOSPFRouterConfigLocalMessage->getCmdCode() != OSPF_ROUTER_CMD_GLOBAL_CONFIG))
				{
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
					pResults->clear();
					delete (pResults);
				}
				return pOSPFRouterConfigLocalMessage->getCompletionStatus ();
			}
		}

		if(pOSPFRouterConfigLocalMessage->getOpCode() == OSPF_SET_ELEM)
		{
			updateManagedObjectFromLocalMessage(pMO, pOSPFRouterConfigLocalMessage);
			updateWaveManagedObject(pMO);
		}

		if ((pResults !=NULL) && numberOfResults) 
		{
			trace(TRACE_LEVEL_DEVEL, "OSPFLocalConfWorker::ospfRouterConfigMOStep pResults is not NULL, so deleting");
			pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			delete (pResults);
        }
		return pOSPFRouterConfigLocalMessage->getCompletionStatus ();
	}

	ResourceId OSPFLocalConfWorker::OSPFPermitDenyRedistributeStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();

		OSPFPermitDenyRedistributeLocalMessage *pOSPFPermitDenyLocalMessage =
			dynamic_cast<OSPFPermitDenyRedistributeLocalMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

		OSPFPermitDenyRedistributeMessage *pOSPFPermitDenyMessage = new OSPFPermitDenyRedistributeMessage ();
			
		pOSPFPermitDenyMessage->updateFromLocalMessage(pOSPFPermitDenyLocalMessage);

		trace (TRACE_LEVEL_DEVEL, "OSPFLocalConfWorker::OSPFPermitDenyRedistributeStep message recived");

		status = sendSynchronouslyToWaveClient("ospf",pOSPFPermitDenyMessage);

		if(status == WAVE_MESSAGE_SUCCESS)
		{
			status = pOSPFPermitDenyMessage->getClientStatus();
			trace (TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorker::OSPFPermitDenyRedistributeStep message sent to OSPF backend, client status is : ") + FrameworkToolKit::localize(status));
		} else {
			trace (TRACE_LEVEL_ERROR, "OSPFLocalConfWorker::OSPFPermitDenyRedistributeStep message not sent to OSPF backend");
		}

		pOSPFPermitDenyLocalMessage->setCompletionStatus(status);
		delete (pOSPFPermitDenyMessage);
		return (status);
	}

	OSPFPermitDenyRedistributeLocalManagedObject * OSPFLocalConfWorker::createPermitDenyManagedObject (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext,
																										OSPFPermitDenyRedistributeLocalMessage *pMsg)
	{
		OSPFPermitRedistributeLocalManagedObject *pOSPFPermitLocalManagedObject = NULL;
		OSPFDenyRedistributeLocalManagedObject *pOSPFDenyLocalManagedObject = NULL;
		OSPFPermitDenyRedistributeLocalManagedObject *pOSPFPermitDenyLocalManagedObject = NULL;
		OSPFConfigLocalManagedObject *pOSPFConfigMO = getOSPFConfigManagedObject(pPrismSynchronousLinearSequencerContext);
		prismAssert(NULL != pOSPFConfigMO, __FILE__, __LINE__);

		switch (pMsg->getPermitDenyOption())
		{
			case OSPF_PERMIT_DENY_OPTION_PERMIT:
				pOSPFPermitLocalManagedObject = new OSPFPermitRedistributeLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
				pOSPFPermitDenyLocalManagedObject = pOSPFPermitLocalManagedObject;
				if (pOSPFPermitDenyLocalManagedObject == NULL)
				{
					trace(TRACE_LEVEL_ERROR, "OSPFLocalConfWorker::createPermitDenyManagedObject not enough mem to create Permit MO");
					return NULL;
				}
				pOSPFConfigMO->addPermitRedistributeManagedObject(pOSPFPermitLocalManagedObject);
				break;
			case OSPF_PERMIT_DENY_OPTION_DENY:
				pOSPFDenyLocalManagedObject = new OSPFDenyRedistributeLocalManagedObject(dynamic_cast<OSPFLocalObjectManager *>(getPWaveObjectManager()));
				pOSPFPermitDenyLocalManagedObject = pOSPFDenyLocalManagedObject;
				if (pOSPFPermitDenyLocalManagedObject == NULL)
				{
					trace(TRACE_LEVEL_ERROR, "OSPFLocalConfWorker::createPermitDenyManagedObject not enough mem to create Deny MO");
					return NULL;
				}
				pOSPFConfigMO->addDenyRedistributeManagedObject(pOSPFDenyLocalManagedObject);
				break;
			default:
				trace(TRACE_LEVEL_FATAL, string("OSPFLocalConfWorker::createPermitDenyManagedObject invalid option") + pMsg->getPermitDenyOption());
				return NULL;
		}

		trace(TRACE_LEVEL_DEVEL, "OSPFLocalConfWorker::createPermitDenyManagedObject MO created");

		pOSPFPermitDenyLocalManagedObject->updateFromMessage(pMsg);
		updateWaveManagedObject(pOSPFConfigMO);

		return pOSPFPermitDenyLocalManagedObject;
	}

	void OSPFLocalConfWorker::updateOSPFPermitDenyManagedObject(OSPFPermitDenyRedistributeLocalManagedObject *pOSPFPermitDenyLocalManagedObject)
	{

		OSPFPermitRedistributeLocalManagedObject *pOSPFPermitMO = NULL;
		OSPFDenyRedistributeLocalManagedObject *pOSPFDenyMO  = NULL;

		switch (pOSPFPermitDenyLocalManagedObject->getPermitDenyOption()){
			case OSPF_PERMIT_DENY_OPTION_PERMIT:
				trace (TRACE_LEVEL_DEVEL, "OSPFLocalConfWorker::updateOSPFPermitDenyManagedObject : updating permit MO");
				pOSPFPermitMO = dynamic_cast<OSPFPermitRedistributeLocalManagedObject*>(pOSPFPermitDenyLocalManagedObject);
				if (pOSPFPermitMO == NULL) {
					trace (TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::updateOSPFPermitDenyManagedObject : base to permitMO yielded null");
					return ;
				}
				updateWaveManagedObject(pOSPFPermitMO);
				break;
			case OSPF_PERMIT_DENY_OPTION_DENY:
				trace (TRACE_LEVEL_DEVEL, "OSPFLocalConfWorker::updateOSPFPermitDenyManagedObject : updating deny MO");
				pOSPFDenyMO = dynamic_cast<OSPFDenyRedistributeLocalManagedObject*>(pOSPFPermitDenyLocalManagedObject);
				if (pOSPFDenyMO == NULL) {
					trace (TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::updateOSPFPermitDenyManagedObject : base to denyMO yielded null");
					return ;
				}
				updateWaveManagedObject(pOSPFDenyMO);
				break;
			default:
				trace (TRACE_LEVEL_FATAL, string("OSPFLocalConfWorker::updateOSPFPermitDenyManagedObject : invalid permit/deny option ") + pOSPFPermitDenyLocalManagedObject->getPermitDenyOption());
				;
		}
	}

	ResourceId OSPFLocalConfWorker::OSPFPermitDenyRedistributeMOStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
		vector<WaveManagedObject *> *pResults = NULL;
		vector<WaveManagedObject *> *pResults1 = NULL;
		LocationId locId =0;
		OSPFPermitDenyRedistributeLocalManagedObject *pOSPFPermitDenyLocalManagedObject = NULL;
		OSPFPermitDenyRedistributeLocalMessage *pOSPFPermitDenyLocalMessage =
			dynamic_cast<OSPFPermitDenyRedistributeLocalMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
		UI32 index = pOSPFPermitDenyLocalMessage->getIndex();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(OSPFPermitDenyRedistributeLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeUI32(&index, "index"));
		locId = FrameworkToolKit::getThisLocationId();
		pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt);

		if (NULL != pResults) 
		{
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_DEVEL, "OSPFLocalConfWorker::OSPFPermitDenyRedistributeMOStep : MO not found");
			}else if (1 == numberOfResults) {
				pOSPFPermitDenyLocalManagedObject = dynamic_cast<OSPFPermitDenyRedistributeLocalManagedObject *>((*pResults)[0]);
				/*
				 * We've got the base class pointer, but we need the derived class MO to do the operation
				 */
				switch (pOSPFPermitDenyLocalManagedObject->getPermitDenyOption()) {
					case OSPF_PERMIT_DENY_OPTION_PERMIT:
					{
						pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
						delete(pResults);
						WaveManagedObjectSynchronousQueryContext syncQueryCtxt2(OSPFPermitRedistributeLocalManagedObject::getClassName());
						syncQueryCtxt2.addAndAttribute (new AttributeUI32(&index, "index"));
						pResults1 = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt2);
						if (NULL == pResults1) {
							trace (TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::OSPFPermitDenyRedistributeMOStep : PermitDenyMO exist, permitMO not found");
							return WAVE_MESSAGE_ERROR;
						}

						UI32 numberOfResults2 = pResults1->size ();
						if (numberOfResults2 != 1) {
							trace (TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::OSPFPermitDenyRedistributeMOStep : multiple denyMO exist");
							return WAVE_MESSAGE_ERROR;
						}

						pOSPFPermitDenyLocalManagedObject = dynamic_cast<OSPFPermitDenyRedistributeLocalManagedObject *>((*pResults1)[0]);
					}
						break;
					case OSPF_PERMIT_DENY_OPTION_DENY:
					{
						pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
						delete(pResults);
						WaveManagedObjectSynchronousQueryContext syncQueryCtxt2(OSPFDenyRedistributeLocalManagedObject::getClassName());
						syncQueryCtxt2.addAndAttribute (new AttributeUI32(&index, "index"));
						pResults1 = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt2);
						if (NULL == pResults1) {
							trace (TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::OSPFPermitDenyRedistributeMOStep : PermitDenyMO exist, denyMO not found");
							return WAVE_MESSAGE_ERROR;
						}

						UI32 numberOfResults2 = pResults1->size ();
						if (numberOfResults2 != 1) {
							trace (TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::OSPFPermitDenyRedistributeMOStep : multiple permitMO exist");
							return WAVE_MESSAGE_ERROR;
						}

						pOSPFPermitDenyLocalManagedObject = dynamic_cast<OSPFPermitDenyRedistributeLocalManagedObject *>((*pResults1)[0]);
					}
						break;
					default:
						pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
						delete(pResults);
						trace (TRACE_LEVEL_FATAL, string("OSPFLocalConfWorker::OSPFPermitDenyRedistributeMOStep : invalid permit/deny MO") +
													pOSPFPermitDenyLocalManagedObject->getPermitDenyOption());
						return WAVE_MESSAGE_ERROR;
				}

				trace (TRACE_LEVEL_DEVEL, "OSPFLocalConfWorker::OSPFPermitDenyRedistributeMOStep : MO found");
			} else {
				trace (TRACE_LEVEL_FATAL, "OSPFLocalConfWorker::OSPFPermitDenyRedistributeMOStep : multiple MO's exist");

				pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
				delete (pResults);
				return WAVE_MESSAGE_ERROR;
			}
		}

		if (pOSPFPermitDenyLocalManagedObject == NULL) 
		{
			if(pOSPFPermitDenyLocalMessage->getOpCode() != OSPF_CREATE)
			{
				if (pResults !=NULL)
				{
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
					delete (pResults);
				}
				return WAVE_MESSAGE_ERROR;
			} else {
				startTransaction();
				pOSPFPermitDenyLocalManagedObject = createPermitDenyManagedObject(pPrismSynchronousLinearSequencerContext, pOSPFPermitDenyLocalMessage);
				commitTransaction();
				if (pOSPFPermitDenyLocalManagedObject == NULL) {
					if (pResults !=NULL)
					{
						pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
						delete (pResults);
					}
					return WAVE_MESSAGE_ERROR;
				}
			}
		}
		else
		{
			if(pOSPFPermitDenyLocalMessage->getOpCode() == OSPF_CREATE)
			{
				if (pOSPFPermitDenyLocalManagedObject->getPermitDenyOption() != pOSPFPermitDenyLocalMessage->getPermitDenyOption())
				{
					/*
					 * Remove the MO, and create a new one
					 */
					startTransaction();
					deletePermitDenyLocalManagedObject(pOSPFPermitDenyLocalManagedObject, pOSPFPermitDenyLocalMessage,pPrismSynchronousLinearSequencerContext);
					commitTransaction();

					startTransaction();
					pOSPFPermitDenyLocalManagedObject = createPermitDenyManagedObject(pPrismSynchronousLinearSequencerContext, pOSPFPermitDenyLocalMessage);
					commitTransaction();
				} else {

					pOSPFPermitDenyLocalManagedObject->updateFromMessage(pOSPFPermitDenyLocalMessage);
					startTransaction();
					updateOSPFPermitDenyManagedObject(pOSPFPermitDenyLocalManagedObject);
					commitTransaction();
				}

				/*
				if (pResults !=NULL)
				{
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
					delete (pResults);
				}
				return WAVE_MESSAGE_ERROR;
				*/
			} else if(pOSPFPermitDenyLocalMessage->getOpCode() == OSPF_REMOVE)
			{
				startTransaction();
				deletePermitDenyLocalManagedObject(pOSPFPermitDenyLocalManagedObject, pOSPFPermitDenyLocalMessage,pPrismSynchronousLinearSequencerContext);
				commitTransaction();

				if (pResults1 !=NULL)
				{
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults1);
					pResults1->clear();
					delete (pResults1);
				}
				return pOSPFPermitDenyLocalMessage->getCompletionStatus ();
			}
		}

		if(pOSPFPermitDenyLocalMessage->getOpCode() == OSPF_SET_ELEM)
		{
			trace (TRACE_LEVEL_DEVEL, "OSPFLocalConfWorker::OSPFPermitDenyRedistributeMOStep : set elem");
			pOSPFPermitDenyLocalMessage->dumpMessage();
			pOSPFPermitDenyLocalManagedObject->updateFromMessage(pOSPFPermitDenyLocalMessage);
			startTransaction();
			updateOSPFPermitDenyManagedObject(pOSPFPermitDenyLocalManagedObject);
			commitTransaction();
		}

		if (pResults1 !=NULL) 
		{
			pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults1);
			delete (pResults1);
		}
		return pOSPFPermitDenyLocalMessage->getCompletionStatus ();
	}	

    ResourceId OSPFLocalConfWorker::OSPFUpdateAreaStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
    {
		trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateAreaStep: Entered....");
		LocationId locId = 0;
        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();
        OSPFAreaLocalMessage *pMsg = dynamic_cast<OSPFAreaLocalMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        if (!pMsg){
		trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateAreaStep: pMsg is NULL returning WAVE_MESSAGE_ERROR....");
            return WAVE_MESSAGE_ERROR;
	}	
        int status = WAVE_MESSAGE_SUCCESS;

        OSPFAreaMessage *m = new OSPFAreaMessage();
        m->setOpCode(pMsg->getOpCode());
		m->setCmdCode(pMsg->getCmd());
        m->setAreaId(pMsg->getAreaId());
        m->setVRF(pMsg->getPartitionName());
        trace (TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateAreaStep: Area ID:") + m->getAreaId() );
        trace (TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateAreaStep: VRF:") + m->getVRF() );
        OSPFConfigLocalManagedObject *pOSPFConfigMO = getOSPFConfigManagedObject(pPrismSynchronousLinearSequencerContext);
        if(pOSPFConfigMO) {
        	pPrismSynchronousLinearSequencerContext->getCache()->put(OSPFConfigLocalManagedObject::getClassName(), pOSPFConfigMO);
        }
        if( (m->getOpCode() == OSPF_CREATE ||
			 m->getOpCode() == OSPF_SET_ELEM) &&
			(pMsg->getCmd() >= OSPF_AREA_CONFIG &&
			 pMsg->getCmd() <= OSPF_AREA_CONFIG_STUB_NO_SUMMARY) )
		{

			if (!pOSPFConfigMO)
			{
				trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateAreaStep: pOSPFConfigMO is NULL returning WAVE_MESSAGE_ERROR....");
				delete (m);
				return WRC_OSPF_CLI_ERR_NO_OSPF_INST;
			}

			if ((pOSPFConfigMO->getAreaLocalManagedObject(pMsg->getAreaId()) == false)
				&& (pOSPFConfigMO->getAreaCount() >= OSPF_MAX_AREA_COUNT))
			{
				trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateAreaStep: Area Count exceeded...");
				delete (m);
				return WRC_OSPF_CLI_ERR_EXCEEDED_MAX_AREA_COUNT;
			}
		}

		if ( (pMsg->getCmd()) )
		{
			m->setNssa(pMsg->getNssa());
			m->setStub(pMsg->getStub());

			//Retrieve the stub/nssa value from the database to pass it to the backend in case of no nssa/ no stub cmd
			unsigned int cmd = pMsg->getCmd();
			if( (cmd == OSPF_AREA_CONFIG_NSSA) || (cmd == OSPF_AREA_CONFIG_NSSA_NO_SUMMARY) ||
					(cmd == OSPF_AREA_CONFIG_STUB) || (cmd == OSPF_AREA_CONFIG_STUB_NO_SUMMARY) )
			{
				// search areaMO's column 'idNumber' for getAreaId()
				locId = FrameworkToolKit::getThisLocationId();
				WaveManagedObjectSynchronousQueryContext syncQueryCtxt(OSPFAreaLocalManagedObject::getClassName());
				string areaId = pMsg->getAreaId();
				//string vrf = pMsg->getPartitionName();

		        if(pOSPFConfigMO) {
		        	ObjectId objectId = pOSPFConfigMO->getOwnerPartitionManagedObjectId();
		        	trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateAreaStep OwnerPartitionManagedObjectId:")+objectId.toString());
		        	syncQueryCtxt.addAndAttribute(new AttributeObjectId(&objectId, "ownerPartitionManagedObjectId"));
		        }
		        else {
		        	trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateAreaStep OSPFConfigLocalManagedObject is NULL:"));
		        }

				syncQueryCtxt.addAndAttribute (new AttributeString(&areaId, "areaId"));
				//syncQueryCtxt.addAndAttribute (new AttributeString(&vrf, "vrf"));
				syncQueryCtxt.addSelectField("areaId");
				syncQueryCtxt.addSelectField("NSSAMetric");
				syncQueryCtxt.addSelectField("stubMetric");
				vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId,&syncQueryCtxt);
				OSPFAreaLocalManagedObject *pAreaLocalMO = NULL;
				if (NULL != pResults) 
				{
					UI32 numberOfResults = pResults->size ();                    
					if (1 == numberOfResults) {
						pAreaLocalMO  = dynamic_cast<OSPFAreaLocalManagedObject*>((*pResults)[0]);
						NSM_PLUG_ASSERT_PTR (pAreaLocalMO);
						trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateAreaStep: One match found");
					}
					else {
						trace (TRACE_LEVEL_ERROR, "OSPFLocalConfWorker::OSPFUpdateAreaStep: numberOfResults != 1");
					}
				}    

				if(pAreaLocalMO)
				{
					if (!m->getNssa())
						m->setNssa(pAreaLocalMO->getNSSAMetric());
					if (!m->getStub())
						m->setStub(pAreaLocalMO->getStubMetric());
				}

				if (pResults) {
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
					trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateAreaStep: deleted pResults");
				}
			}

			trace (TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateAreaStep: Nssa:") + m->getNssa()); 

			trace (TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateAreaStep: Stub:") + m->getStub() );
		}


        status = sendSynchronouslyToWaveClient ("ospf", m);

		if(status == WAVE_MESSAGE_SUCCESS)
		{
			status = m->getClientStatus();
	        trace (TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorker::OSPFUpdateAreaStep ospf area config Local message sent to OSPF backend, client status is : ") + FrameworkToolKit::localize(status));
		}
		else
	        trace (TRACE_LEVEL_ERROR, "OSPFLocalConfWorker::OSPFUpdateAreaStep ospf area config Local message not sent to OSPF backend");

		pMsg->setCompletionStatus(status);
        delete (m);
        return (status);
    }

    string OSPFLocalConfWorker::OSPFGetOtherAreaFormatString(string area)
    {
        char buf[INET_ADDRSTRLEN];                                 
        buf[0] = '\0';
        uint32_t ipad1, ipad2, ipad3, ipad4;
        strcpy(buf, area.c_str());
        if(strchr (buf, '.'))                               
        {                                                          
            sscanf(buf, "%u.%u.%u.%u", (uint32_t *)&ipad1, (uint32_t *)&ipad2, (uint32_t *)&ipad3, (uint32_t *)&ipad4);
            ipad4 |= (ipad1 << 24) | (ipad2 << 16) | (ipad3 << 8);
            sprintf(buf, "%u", (ipad4));                     
            trace (TRACE_LEVEL_INFO, string("OSPFGetOtherAreaFormatString: OrigArea3: ") + string(buf));    
        }   
        else if( area.length() )
        {
            sscanf(buf, "%u", (uint32_t *)&(ipad4));
            ipad1 = ipad4 & 0xFF000000;
            ipad2 = ipad4 & 0x00FF0000;
            ipad3 = ipad4 & 0x0000FF00;
            ipad4 = ipad4 & 0x000000FF;
            sprintf(buf, "%u.%u.%u.%u",(ipad1 >> 24), (ipad2 >> 16), (ipad3 >> 8), ipad4); 
        }    

        trace (TRACE_LEVEL_INFO, string("OSPFGetOtherAreaFormatString: ") + string(buf));
        return string(buf);
    }



    ResourceId OSPFLocalConfWorker::OSPFUpdateAreaMOStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
		trace (TRACE_LEVEL_DEBUG, "OSPFLocalConfWorker::OSPFUpdateAreaMOStep: Entered....");
		int createFlag = 1, status = 0;
		string areaId;
		LocationId locId = 0;
		OSPFAreaLocalManagedObject *pAreaLocalMO = NULL;
		OSPFConfigLocalManagedObject *pConfigLocalMO = NULL;
		OSPFAreaLocalMessage *pAreaLocalMsg = dynamic_cast<OSPFAreaLocalMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        NSM_PLUG_ASSERT_PTR (pAreaLocalMsg);
        string vrf = pAreaLocalMsg->getPartitionName();
        trace (TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::OSPFUpdateAreaMOStep: Updating area for VRF : ")+vrf);
		locId = FrameworkToolKit::getThisLocationId();

		//Retrieve the OSPFLocalConfigMO from the cache which is put in the OSPFUpdateAreaStep
		pConfigLocalMO = dynamic_cast<OSPFConfigLocalManagedObject *> (pPrismSynchronousLinearSequencerContext->
        		getCache()->getWaveManagedObject(OSPFConfigLocalManagedObject::getClassName()));

		// search areaMO's column 'idNumber' for getAreaId()
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(OSPFAreaLocalManagedObject::getClassName());
		areaId = pAreaLocalMsg->getAreaId();

        //Decide which areaId to query
        if( pConfigLocalMO )
        {
            //Do not update any parameters of the area whose format is inconsistant 
            string otherAreaId = OSPFGetOtherAreaFormatString(areaId);
            if( (pConfigLocalMO->getAreaLocalManagedObject(otherAreaId)) )
            {
                return WRC_OSPF_CLI_ERR_INVALID_AREA_FORMAT;
            }    
        }
        
        syncQueryCtxt.addAndAttribute (new AttributeString(&areaId, "areaId"));
        //syncQueryCtxt.addAndAttribute (new AttributeString(&vrf, "vrf"));

        if(pConfigLocalMO) {
        	ObjectId objectId = pConfigLocalMO->getOwnerPartitionManagedObjectId();
        	trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateAreaMOStep OwnerPartitionManagedObjectId:")+objectId.toString());
        	syncQueryCtxt.addAndAttribute(new AttributeObjectId(&objectId, "ownerPartitionManagedObjectId"));
        }
        else {
        	trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateAreaMOStep OSPFConfigLocalManagedObject is NULL:"));
        }
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt);

		if (NULL != pResults) {

			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_INFO, "No match found");
				createFlag = 1;
				status = WAVE_MESSAGE_SUCCESS;
			}

			else if (1 == numberOfResults) {
				pAreaLocalMO  = dynamic_cast<OSPFAreaLocalManagedObject*>((*pResults)[0]);
                NSM_PLUG_ASSERT_PTR (pAreaLocalMO);

				trace (TRACE_LEVEL_INFO, "One match found");
				createFlag = 0;
				status = WAVE_MESSAGE_SUCCESS;
			}

			else {
				trace (TRACE_LEVEL_ERROR, string("OSPFLocalConfWorker::OSPFUpdateAreaMOStep: Error! numberOfResults = ") + numberOfResults);
				return (WAVE_MESSAGE_ERROR);
			}
		}

		else {
			trace (TRACE_LEVEL_ERROR, "OSPFLocalConfWorker::OSPFUpdateAreaMOStep: ERROR querying database!");
			return (WAVE_MESSAGE_ERROR);
		}

        if(createFlag)
        {
            trace (TRACE_LEVEL_INFO, "createFlag = 1");
            pAreaLocalMO = new OSPFAreaLocalManagedObject(dynamic_cast<OSPFLocalObjectManager*>(getPWaveObjectManager()));
            prismAssert (NULL != pAreaLocalMO, __FILE__, __LINE__);

            if( OSPF_AREA_CONFIG == pAreaLocalMsg->getCmd() )
                pAreaLocalMO->setNormal(true, true);
        } 

		pAreaLocalMO->updateMOFromLocalMessage(pAreaLocalMsg);
        //Updating NSSA para 
        if (createFlag == 0) {
            trace (TRACE_LEVEL_INFO, "createFlag = 0");
            pAreaLocalMO->setOwnerManagedObjectId(pConfigLocalMO->getObjectId());
            updateWaveManagedObject (pAreaLocalMO);
            prismAssert (NULL != pAreaLocalMO, __FILE__, __LINE__);
        }

        pAreaLocalMO->setAreaId(pAreaLocalMsg->getAreaId());

        trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateAreaMOStep: ip set to ")
                + pAreaLocalMO->getAreaId()
                + " from "
                + pAreaLocalMsg->getAreaId());

        if (createFlag) {
        	trace (TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateAreaMOStep: creating area for VRF:")+pConfigLocalMO->getVRF());
        	trace (TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateAreaMOStep: creating area for OwnerPartitionObjectID:")+pConfigLocalMO->getOwnerPartitionManagedObjectId().toString());
            pConfigLocalMO->setAreaLocalMO(pAreaLocalMO);

            updateWaveManagedObject(pConfigLocalMO);
			trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateAreaMOStep: createFlag was set");
        }

        else if((OSPF_REMOVE == pAreaLocalMsg->getOpCode()) && ((OSPF_AREA_CONFIG == pAreaLocalMsg->getCmd()) ||
				(OSPF_AREA_CONFIG_NSSA == pAreaLocalMsg->getCmd()) ||
				(OSPF_AREA_CONFIG_NSSA_NO_SUMMARY == pAreaLocalMsg->getCmd()) ||
				(OSPF_AREA_CONFIG_STUB == pAreaLocalMsg->getCmd()) ||
				(OSPF_AREA_CONFIG_STUB_NO_SUMMARY == pAreaLocalMsg->getCmd())))
        {
            trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateAreaMOStep: deleteAreaLocalMO from ConfigLocalMO");
            pConfigLocalMO->deleteAreaLocalMO(pAreaLocalMO);
            updateWaveManagedObject(pConfigLocalMO);
        }    

        if(!createFlag) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateAreaMOStep: deleted pResults");
            delete pResults;
        }

        trace (TRACE_LEVEL_DEBUG, "OSPFLocalConfWorker::OSPFUpdateAreaMOStep: exiting");
        return status;
    }

    void  OSPFLocalConfWorker::OSPFAreaLocalMessageHandler(OSPFAreaLocalMessage *pOSPFAreaLocalMessage)
    {
        // Validations and Queries should go here
        trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFAreaLocalMessageHandler: Entered....");

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::OSPFUpdateAreaStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::OSPFUpdateAreaMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pOSPFAreaLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }


 ResourceId OSPFLocalConfWorker::OSPFUpdateAreaRangeStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
    {

		trace (TRACE_LEVEL_DEBUG, "OSPFLocalConfWorker::OSPFUpdateAreaRangeStep: Entered....");
        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();
        OSPFAreaRangeLocalMessage *pMsg = dynamic_cast<OSPFAreaRangeLocalMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        if (!pMsg)
            return WAVE_MESSAGE_ERROR;

        trace (TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorker::OSPFUpdateAreaRangeStep ospf area range Local message recived for VRF:")
        		+pMsg->getPartitionName());

        int status = WAVE_MESSAGE_SUCCESS;

        LocationId locId = 0;
		OSPFAreaRangeLocalManagedObject *pAreaRangeLocalMO = NULL;
		OSPFAreaLocalManagedObject *pAreaLocalMO = 0;
        OSPFConfigLocalManagedObject *ospfMOForVRF = getOSPFConfigManagedObject(pPrismSynchronousLinearSequencerContext);
		// get ptr to parent area MO
		WaveManagedObjectSynchronousQueryContext syncQueryCtxtParent(OSPFAreaLocalManagedObject::getClassName());
		string areaId = pMsg->getAreaId();
		string vrf = pMsg->getPartitionName();
		NSM_PLUG_DBG_S(string("VRF:")+vrf);
        syncQueryCtxtParent.addAndAttribute (new AttributeString(&areaId, "areaId"));
        if(ospfMOForVRF) {
        	ObjectId objectId = ospfMOForVRF->getOwnerPartitionManagedObjectId();
        	NSM_PLUG_DBG_S(string("OwnerPartitionManagedObjectId:")+objectId.toString());
        	syncQueryCtxtParent.addAndAttribute(new AttributeObjectId(&objectId, "ownerPartitionManagedObjectId"));
        }
        else {
        	NSM_PLUG_DBG_S(string("OSPFConfigLocalManagedObject from cache is NULL:"));
        }
		NSM_PLUG_DBG_S(string("AreaId = ")+areaId+" ,Range Effect = "+pMsg->getRangeEffect()+" ,IP Address = "+pMsg->getIpAddr().toString());
		vector<WaveManagedObject *> *pResultsParent = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxtParent);
		if (!pResultsParent){
			trace(TRACE_LEVEL_INFO, "No area MO found!");
		}
		else {
			// we only get 1 result
			pAreaLocalMO = dynamic_cast<OSPFAreaLocalManagedObject *>((*pResultsParent)[0]);
			if (pAreaLocalMO) {
				pPrismSynchronousLinearSequencerContext->getCache()->put(OSPFAreaLocalManagedObject::getClassName(), pAreaLocalMO);
				pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pAreaLocalMO);
				// See if we have this range configured on this area already
				pAreaRangeLocalMO = pAreaLocalMO->getAreaRangeLocalMO(pMsg->getIpAddr(),pMsg->getIpMask());
				if(pAreaRangeLocalMO) {
					NSM_PLUG_DBG_S(string("Add the area range to the cache"));
					pPrismSynchronousLinearSequencerContext->getCache()->put(OSPFAreaRangeLocalManagedObject::getClassName(), pAreaRangeLocalMO);
				}
				else {
					NSM_PLUG_DBG_S(string("Area Range does not exist. Create operation"));
				}
			}
			else {
				NSM_PLUG_DBG_S(string("ERROR: Area is not Configured"));
			}
		}



        OSPFAreaRangeMessage *m = new OSPFAreaRangeMessage();

		m->setCmdCode(pMsg->getCmdCode());
		m->setOpCode(pMsg->getOpCode());
		m->setAreaId(pMsg->getAreaId());
		m->setIpAddr(pMsg->getIpAddr());
		m->setIpMask(pMsg->getIpMask());

		if(pMsg->isRangeEffectModified()) {
			NSM_PLUG_DBG_S(string("Range Effect Modified"));
			m->setRangeEffect(pMsg->getRangeEffect());
		}
		else if(pAreaRangeLocalMO) {
			NSM_PLUG_DBG_S(string("Range Effect Not Modified. Get the value from the MO"));
			m->setRangeEffect(pAreaRangeLocalMO->getRangeAdvertise());
			//pMsg has the default value. So update it also for setting it in the MO in the updateMO step
			pMsg->setRangeEffect(m->getRangeEffect());
		}
		else {
			NSM_PLUG_DBG_S(string("Set the default range effect from the message"));
			m->setRangeEffect(pMsg->getRangeEffect());
		}
		if(pMsg->isRangeCostModified()) {
			NSM_PLUG_DBG_S(string("Range Cost Modified"));
			m->setRangeCost(pMsg->getRangeCost());
		}
		else if(pAreaRangeLocalMO) {
			NSM_PLUG_DBG_S(string("Range Cost Not Modified. Get the value from the MO"));
			m->setRangeCost(pAreaRangeLocalMO->getRangeCost());
			//pMsg has the default value. So update it also for setting it in the MO in the updateMO step
			pMsg->setRangeCost(m->getRangeCost());
		}
		else {
			NSM_PLUG_DBG_S(string("Set the default cost from the message"));
			m->setRangeCost(pMsg->getRangeCost());
		}

		vrf = pMsg->getPartitionName();
		trace (TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::OSPFUpdateAreaRangeStep: VRF :")+vrf);
		m->setVRF(vrf);

        status = sendSynchronouslyToWaveClient ("ospf", m);
		if(status == WAVE_MESSAGE_SUCCESS)
		{
			status = m->getClientStatus();
	        trace (TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorker::OSPFUpdateAreaRangeStep ospf area-range config Local message sent to OSPF backend, client status is : ") + FrameworkToolKit::localize(status));
		}
		else
	        trace (TRACE_LEVEL_ERROR, "OSPFLocalConfWorker::OSPFUpdateAreaRangeStep ospf area-range config Local message not sent to OSPF backend");

		pMsg->setCompletionStatus(status);
        delete (m);
        return (status);
    }

    ResourceId OSPFLocalConfWorker::OSPFUpdateAreaRangeMOStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
		int createFlag = 1, status = WAVE_MESSAGE_SUCCESS;
		string areaId;
		//LocationId locId = 0;
		IpV4Address rangeAddress;
		OSPFAreaRangeLocalManagedObject *pAreaRangeLocalMO = NULL;
		pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pAreaRangeLocalMO);
		OSPFAreaLocalManagedObject *pAreaLocalMO = 0;
		//locId = FrameworkToolKit::getThisLocationId();

		trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateAreaRangeMOStep: Entered....");
		OSPFAreaRangeLocalMessage *pAreaRangeLocalMsg = dynamic_cast<OSPFAreaRangeLocalMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        NSM_PLUG_ASSERT_PTR (pAreaRangeLocalMsg);
        string vrf = pAreaRangeLocalMsg->getPartitionName();

        pAreaLocalMO = dynamic_cast<OSPFAreaLocalManagedObject *> (pPrismSynchronousLinearSequencerContext->
        		getCache()->getWaveManagedObject(OSPFAreaLocalManagedObject::getClassName()));
        if(pAreaLocalMO) {
        	// See if we have this range configured on this area already
        	pAreaRangeLocalMO = pAreaLocalMO->getAreaRangeLocalMO(pAreaRangeLocalMsg->getIpAddr(),
															  pAreaRangeLocalMsg->getIpMask());
        }
        else {
        	trace (TRACE_LEVEL_INFO,"OSPFLocalConfWorker::OSPFUpdateAreaRangeMOStep:Not possible case. Backend validates the area existence before configuring range");
        	return WAVE_MESSAGE_ERROR;
        }

		if (pAreaRangeLocalMO) {
			// update range MO
			trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateAreaRangeMOStep: Found range MO for "
				   + pAreaRangeLocalMsg->getIpAddr().toString() + " / "
				   + pAreaRangeLocalMsg->getIpMask().toString() + " in area "
				   + pAreaLocalMO->getAreaId());
			createFlag = 0;

			if (pAreaRangeLocalMsg->getOpCode() == OSPF_REMOVE) {
				// delete range MO from area MO
				pAreaLocalMO->deleteAreaRangeMO(pAreaRangeLocalMO);

				// since we deleted a range MO from the area MO, update the latter
				updateWaveManagedObject(pAreaLocalMO);
				return (WAVE_MESSAGE_SUCCESS);
			}
		}

		// create new range MO
		else {
			trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateAreaRangeMOStep: Creating new range MO for "
				   + pAreaRangeLocalMsg->getIpAddr().toString() + " / "
				   + pAreaRangeLocalMsg->getIpMask().toString() + " in area "
				   + pAreaLocalMO->getAreaId());
			createFlag = 1;
			pAreaRangeLocalMO = new OSPFAreaRangeLocalManagedObject(dynamic_cast<OSPFLocalObjectManager*>(getPWaveObjectManager()));
			prismAssert (NULL != pAreaRangeLocalMO, __FILE__, __LINE__);
		}

		// config new range MO, or update existing one
		pAreaRangeLocalMO->setRangeAddress(pAreaRangeLocalMsg->getIpAddr());
		pAreaRangeLocalMO->setRangeMask(pAreaRangeLocalMsg->getIpMask());
		if(createFlag == 1 || pAreaRangeLocalMsg->isRangeEffectModified()) {
			pAreaRangeLocalMO->setRangeAdvertise((OSPFAreaRangeEffect) pAreaRangeLocalMsg->getRangeEffect());
		}
		if(createFlag == 1 || pAreaRangeLocalMsg->isRangeCostModified()) {
			pAreaRangeLocalMO->setRangeCost(pAreaRangeLocalMsg->getRangeCost());
		}
		pAreaRangeLocalMO->setUserSpecifiedPartitionName(vrf);
		pAreaRangeLocalMO->setOwnerManagedObjectId(pAreaLocalMO->getObjectId());

		trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateAreaRangeMOStep: vars are ")
			  + pAreaRangeLocalMO->getRangeAddress().toString()
			  + " "
			  + pAreaRangeLocalMO->getRangeMask().toString()
			  + " "
			  + pAreaRangeLocalMO->getRangeAdvertise());

		// now save the range MO
		updateWaveManagedObject (pAreaRangeLocalMO);
		prismAssert (NULL != pAreaRangeLocalMO, __FILE__, __LINE__);

        if (createFlag) {
			// add new range MO to vector in parent area MO
			pAreaLocalMO->setAreaRangeLocalMO(pAreaRangeLocalMO);

			// since we added a range MO to the area MO, update the latter
			updateWaveManagedObject(pAreaLocalMO);

			// VSFIXME better confim by looking up area mo and then printing out ranges
			trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateAreaRangeMOStep: range MO added to parent area MO");
			for (unsigned int k = 0; k < pAreaLocalMO->getRange().size(); ++k)
				trace (TRACE_LEVEL_INFO, "Range: " + (pAreaLocalMO->getRange()[k])->m_rangeAddress.toString());
        }

		trace (TRACE_LEVEL_DEBUG, "OSPFLocalConfWorker::OSPFUpdateAreaRangeMOStep: exiting");
		return status;
	}


    void  OSPFLocalConfWorker::OSPFAreaRangeLocalMessageHandler(OSPFAreaRangeLocalMessage *pOSPFAreaRangeLocalMessage)
    {

// Validations and Queries should go here
		trace (TRACE_LEVEL_DEBUG, "OSPFLocalConfWorker::OSPFAreaRangeLocalMessageHandler: Entered....");

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::OSPFUpdateAreaRangeStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::OSPFUpdateAreaRangeMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pOSPFAreaRangeLocalMessage, this, sequencerSteps, sizeof
(sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
		trace (TRACE_LEVEL_DEBUG, "OSPFLocalConfWorker::OSPFAreaRangeLocalMessageHandler: Exiting....");
    }


    OSPFVirtualLinkLocalManagedObject *OSPFLocalConfWorker::OSPFGetVirtualLinkLocalManagedObject(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
    {
        trace (TRACE_LEVEL_DEBUG, "OSPFLocalConfWorker::OSPFGetVirtualLinkLocalManagedObject: Entered....");
        LocationId locId = 0;
        OSPFVirtualLinkLocalMessage *pMsg = dynamic_cast<OSPFVirtualLinkLocalMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        string vrf = pMsg->getPartitionName();
        trace (TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::OSPFGetVirtualLinkLocalManagedObject: VRF: ")+vrf);
        OSPFVirtualLinkLocalManagedObject *pVirtualLinkLocalMO = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxtParent(OSPFAreaLocalManagedObject::getClassName());
        string areaId = pMsg->getAreaId();
        syncQueryCtxtParent.addAndAttribute (new AttributeString(&areaId, "areaId"));
	locId = FrameworkToolKit::getThisLocationId();
        syncQueryCtxtParent.addAndAttribute (new AttributeString(&vrf, "vrf"));
        vector<WaveManagedObject *> *pResultsParent = querySynchronouslyLocalManagedObjectsForLocationId (locId,&syncQueryCtxtParent);
        
        OSPFAreaLocalManagedObject *pAreaLocalMO = dynamic_cast<OSPFAreaLocalManagedObject *>((*pResultsParent)[0]);
        if (!pAreaLocalMO)
            trace (TRACE_LEVEL_INFO, "No Area MO found");
        else
        {
            trace (TRACE_LEVEL_INFO, string("Area MO found with area id ") + (pAreaLocalMO->getAreaId()) );

            IpV4Address virtualLinkRouterId = pMsg->getVirtualLinkRouterId();
            pVirtualLinkLocalMO = pAreaLocalMO->getVirtualLinkLocalMO(virtualLinkRouterId);
        }

        if (pResultsParent) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsParent);
            trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFGetVirtualLinkLocalManagedObject: deleted pResultsParent");
        }

        trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFGetVirtualLinkLocalManagedObject: Exiting");
        return pVirtualLinkLocalMO;
    }    

    void OSPFLocalConfWorker::ospfUpdateLocalMessage(OSPFVirtualLinkLocalMessage *pOSPFVirtualLinkLocalMessage, OSPFAreaMessage *pOSPFAreaMessage)
    {
        trace (TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::ospfUpdateLocalMessage : Entering") );
        UI32 size = 0;
        void *data;
        ospf_area_config_out_msg_t *out_msg;

        pOSPFAreaMessage->getOutputBuffer(OSPFAREA, size, data);
        string vrf = pOSPFAreaMessage->getPartitionName();
        trace (TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::ospfUpdateLocalMessage : VRF:")+vrf );

        out_msg = (ospf_area_config_out_msg_t*) data;
        if (out_msg) {
            //Store the result
            trace (TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::ospfUpdateLocalMessage: RET: ") + out_msg->encrypted_key );
            if (pOSPFVirtualLinkLocalMessage->getOpCode() != OSPF_REMOVE)
            {
                switch(pOSPFVirtualLinkLocalMessage->getCmdCode())
                {
                    case OSPF_AREA_CONFIG_VIRTUAL_LINK_ENCRYPTTYPE:
                    case OSPF_AREA_CONFIG_VIRTUAL_LINK_AUTH_KEY:
                        {
                            if( (pOSPFAreaMessage->getEncryptionType()) == OSPF_GLOBAL_ENCR)
                            {
                                trace (TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::ospfUpdateLocalMessage: Updating AuthKey "));
                                if( OSPF_SIMPLE_ENCR == (OSPFEncrypType)out_msg->auth_type )
                                {
                                    pOSPFVirtualLinkLocalMessage->setEncryptionType((OSPFEncrypType)out_msg->auth_type);
                                    pOSPFVirtualLinkLocalMessage->setAuthKey(string(out_msg->encrypted_key));
                                }    
                            }
                            break;
                        }    

                    case OSPF_AREA_CONFIG_VIRTUAL_LINK_KEY_ID:
                    case OSPF_AREA_CONFIG_VIRTUAL_LINK_SWITCH_KEY:
                    case OSPF_AREA_CONFIG_VIRTUAL_LINK_MD5_AUTH_KEY:
                        {
                            if( (pOSPFAreaMessage->getAuthType()) == OSPF_GLOBAL_ENCR)
                            {
                                trace (TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::ospfUpdateLocalMessage: Updating MD5Key "));
                                if( OSPF_SIMPLE_ENCR == (OSPFEncrypType)out_msg->auth_type )
                                {
                                    pOSPFVirtualLinkLocalMessage->setAuthType((OSPFEncrypType)out_msg->auth_type);
                                    pOSPFVirtualLinkLocalMessage->setMD5Key(string(out_msg->encrypted_key));
                                }    
                            }
                            break;
                        }

                    default:
                        break;
                } //switch
            }//if
        }
    }

    OSPFVirtualLinkLocalManagedObject *OSPFLocalConfWorker::OSPFGetVirtualLinkMO(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
    {
        trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFGetVirtualLinkMO: Entered....");
        IpV4Address virtualLinkRouterId;
        string areaId;
        OSPFVirtualLinkLocalManagedObject *pVirtualLinkLocalMO = NULL;
        OSPFAreaLocalManagedObject *pAreaLocalMO = NULL;
        OSPFVirtualLinkLocalMessage *pVirtualLinkLocalMsg = dynamic_cast<OSPFVirtualLinkLocalMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        NSM_PLUG_ASSERT_PTR (pVirtualLinkLocalMsg);		
        LocationId locId = 0;
        OSPFConfigLocalManagedObject *ospfMOForVRF = getOSPFConfigManagedObject(pPrismSynchronousLinearSequencerContext);
        // get ptr to parent MO
        WaveManagedObjectSynchronousQueryContext syncQueryCtxtParent(OSPFAreaLocalManagedObject::getClassName());
        areaId = pVirtualLinkLocalMsg->getAreaId();
        string vrf = pVirtualLinkLocalMsg->getPartitionName();
        trace (TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFGetVirtualLinkMO: VRF :")+vrf);
        syncQueryCtxtParent.addAndAttribute (new AttributeString(&areaId, "areaId"));
	locId = FrameworkToolKit::getThisLocationId();
        if(ospfMOForVRF) {
        	ObjectId objectId = ospfMOForVRF->getOwnerPartitionManagedObjectId();
        	trace(TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::OSPFGetVirtualLinkMO OwnerPartitionManagedObjectId:")+objectId.toString());
        	syncQueryCtxtParent.addAndAttribute(new AttributeObjectId(&objectId, "ownerPartitionManagedObjectId"));
        }
        else {
        	trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFGetVirtualLinkMO OSPFConfigLocalManagedObject from cache is NULL:"));
        }

        vector<WaveManagedObject *> *pResultsParent = querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxtParent);
        // assume we only get 1 result for now, since it is supposed to be a singleton
        pAreaLocalMO = dynamic_cast<OSPFAreaLocalManagedObject *>((*pResultsParent)[0]);
        
        if (pAreaLocalMO)
        {
        	trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFGetVirtualLinkMO : Found AreaMO"));
            virtualLinkRouterId = pVirtualLinkLocalMsg->getVirtualLinkRouterId();
            pVirtualLinkLocalMO = pAreaLocalMO->getVirtualLinkLocalMO(virtualLinkRouterId);
            NSM_PLUG_DBG_S(string("Found OSPFVirtualLinkLocalManagedObject"));
            if(pVirtualLinkLocalMO)
            {

            }
        }    

        if (pResultsParent) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsParent);
            trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep: deleted pResultsParent");
        }

        return pVirtualLinkLocalMO;
    }

    ResourceId OSPFLocalConfWorker::validateKeyLength(OSPFVirtualLinkLocalMessage *pMsg)
    {
        switch(pMsg->getCmdCode())
        {
            case OSPF_AREA_CONFIG_VIRTUAL_LINK_ENCRYPTTYPE:
            case OSPF_AREA_CONFIG_VIRTUAL_LINK_AUTH_KEY:
                if(pMsg->getEncryptionType() == OSPF_SIMPLE_ENCR)
                {
                    if( ((pMsg->getAuthKey()).length()) > OSPF_AUTH_KEY_VALID_SIZE )
                        return WRC_OSPF_CLI_ERR_EXCEEDED_AUTH_KEY_SIZE;
                }
                else
                    if( ((pMsg->getAuthKey()).length()) > OSPF_AUTH_KEY_VALID_SIZE_8 )
                        return WRC_OSPF_CLI_ERR_EXCEEDED_AUTH_KEY_SIZE_8;
                break;

            case OSPF_AREA_CONFIG_VIRTUAL_LINK_KEY_ID:
            case OSPF_AREA_CONFIG_VIRTUAL_LINK_SWITCH_KEY:
            case OSPF_AREA_CONFIG_VIRTUAL_LINK_MD5_AUTH_KEY:
                if(pMsg->getAuthType() == OSPF_SIMPLE_ENCR)
                {
                    if( ((pMsg->getMD5Key()).length()) > OSPF_MD5_AUTH_KEY_VALID_SIZE )
                        return WRC_OSPF_CLI_ERR_EXCEEDED_MD5_KEY_SIZE;
                }
                else
                    if( ((pMsg->getMD5Key()).length()) > OSPF_MD5_AUTH_KEY_VALID_SIZE_16 )
                        return WRC_OSPF_CLI_ERR_EXCEEDED_MD5_KEY_SIZE_16;
                break;

            default:
                break;
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId OSPFLocalConfWorker::OSPFUpdateVirtualLinkStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
    {
		trace (TRACE_LEVEL_DEBUG, "OSPFLocalConfWorker::OSPFUpdateVirtualLinkStep: Entered....");
        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();
        OSPFVirtualLinkLocalMessage *pMsg = dynamic_cast<OSPFVirtualLinkLocalMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        if (!pMsg)
            return WAVE_MESSAGE_ERROR;

        int status = WAVE_MESSAGE_SUCCESS;

        OSPFVirtualLinkLocalManagedObject *pVirtualLinkLocalMO = OSPFGetVirtualLinkMO(pPrismSynchronousLinearSequencerContext);
        if(pVirtualLinkLocalMO)
            pVirtualLinkLocalMO->updateLocalMessageFromMO(pMsg);
        string vrf = pMsg->getPartitionName();
        trace (TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateVirtualLinkStep: VRF:")+vrf);
        status = validateKeyLength(pMsg);
        if(status != WAVE_MESSAGE_SUCCESS)
        {
            pMsg->setCompletionStatus(status);
            return status;
        }
        
        OSPFAreaMessage *m = new OSPFAreaMessage();
        m->setVRF(vrf);
        m->setOpCode(pMsg->getOpCode());
        m->setCmdCode(pMsg->getCmdCode());
        m->setAreaId(pMsg->getAreaId());
        m->setVirtualLinkRouterId(pMsg->getVirtualLinkRouterId());

        m->setHelloInterval(pMsg->getHelloInterval());
        m->setRouterDeadInterval(pMsg->getRouterDeadInterval());
        m->setRetransmitInterval(pMsg->getRetransmitInterval());
        m->setTransDelay(pMsg->getTransDelay());
        m->setAuthType(pMsg->getAuthType());
        m->setEncryptionType(pMsg->getEncryptionType());
        m->setAuthKey(pMsg->getAuthKey());
        m->setMD5KeyActWaitTime(pMsg->getMD5KeyActWaitTime());
        m->setMD5KeyId(pMsg->getMD5KeyId());
        m->setMD5Key(pMsg->getMD5Key());

        trace (TRACE_LEVEL_DEBUG, string("EncryptionType: ") + m->getEncryptionType() + string(" AuthKey: ") + m->getAuthKey());
        trace (TRACE_LEVEL_DEBUG, string("KeyId: ") + m->getMD5KeyId() + string(" AuthType: ") + m->getAuthType() + string(" MD5Key: ") + m->getMD5Key());

        status = sendSynchronouslyToWaveClient ("ospf", m);
		if(status == WAVE_MESSAGE_SUCCESS)
		{
			status = m->getClientStatus();
	        trace (TRACE_LEVEL_DEVEL, string("OSPFLocalConfWorker::OSPFUpdateVirtualLinkStep ospf virtual-link config Local message sent to OSPF backend, client status is : ") + FrameworkToolKit::localize(status));

            /*Update with any backend received values(e.g. encrypted passwd)*/
            ospfUpdateLocalMessage(pMsg, m);
		}
		else
	        trace (TRACE_LEVEL_ERROR, "OSPFLocalConfWorker::OSPFUpdateVirtualLinkStep ospf virtual-link config Local message not sent to OSPF backend");

		pMsg->setCompletionStatus(status);
        delete (m);
        return (status);
    }

    ResourceId OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
	{
		trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep: Entered....");
		int createFlag = 0, status = WAVE_MESSAGE_SUCCESS;
		IpV4Address virtualLinkRouterId;
        string areaId;
        OSPFVirtualLinkLocalManagedObject *pVirtualLinkLocalMO = NULL;
		OSPFAreaLocalManagedObject *pAreaLocalMO = NULL;
		OSPFVirtualLinkLocalMessage *pVirtualLinkLocalMsg = dynamic_cast<OSPFVirtualLinkLocalMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        NSM_PLUG_ASSERT_PTR (pVirtualLinkLocalMsg);
	LocationId locId =0;
        OSPFConfigLocalManagedObject *ospfMOForVRF = getOSPFConfigManagedObject(pPrismSynchronousLinearSequencerContext);
		// get ptr to parent MO
		WaveManagedObjectSynchronousQueryContext syncQueryCtxtParent(OSPFAreaLocalManagedObject::getClassName());
		areaId = pVirtualLinkLocalMsg->getAreaId();
		string vrf = pVirtualLinkLocalMsg->getPartitionName();
		trace (TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep: VRF:")+vrf);
        syncQueryCtxtParent.addAndAttribute (new AttributeString(&areaId, "areaId"));
	locId = FrameworkToolKit::getThisLocationId();
        if(ospfMOForVRF) {
        	ObjectId objectId = ospfMOForVRF->getOwnerPartitionManagedObjectId();
        	trace(TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep OwnerPartitionManagedObjectId:")+objectId.toString());
        	syncQueryCtxtParent.addAndAttribute(new AttributeObjectId(&objectId, "ownerPartitionManagedObjectId"));
        }
        else {
        	trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep OSPFConfigLocalManagedObject from cache is NULL:"));
        }

		vector<WaveManagedObject *> *pResultsParent = querySynchronouslyLocalManagedObjectsForLocationId (locId,&syncQueryCtxtParent);
		// assume we only get 1 result for now, since it is supposed to be a singleton
        pAreaLocalMO = dynamic_cast<OSPFAreaLocalManagedObject *>((*pResultsParent)[0]);
		if (!pAreaLocalMO)
			trace (TRACE_LEVEL_INFO, "No Area MO found");
		else
			trace (TRACE_LEVEL_INFO, string("Area MO found with area id ") + (pAreaLocalMO->getAreaId()) );

		virtualLinkRouterId = pVirtualLinkLocalMsg->getVirtualLinkRouterId();
        pVirtualLinkLocalMO = pAreaLocalMO->getVirtualLinkLocalMO(virtualLinkRouterId);

        if(!pVirtualLinkLocalMO)
            createFlag = 1;

        //no area <> virtual-link <>
        if ( (OSPF_AREA_CONFIG_VIRTUAL_LINK == (pVirtualLinkLocalMsg->getCmdCode())) 
                && (OSPF_REMOVE == pVirtualLinkLocalMsg->getOpCode()) )
        {
            if(pVirtualLinkLocalMO)
            {
                pAreaLocalMO->deleteVirtualLinkLocalMO(pVirtualLinkLocalMO);

                updateWaveManagedObject(pAreaLocalMO);
                //pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection (pVirtualLinkLocalMO);
                trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep: removed pVirtualLinkLocalMO");
                
                //delete pVirtualLinkLocalMO;
            }

            if (pResultsParent) {
                pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsParent); 
                trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep: deleted pResultsParent");
            }

            return WAVE_MESSAGE_SUCCESS;
        }    

        if(createFlag)
        {
            trace (TRACE_LEVEL_INFO, "createFlag = 1");
            pVirtualLinkLocalMO = new OSPFVirtualLinkLocalManagedObject(dynamic_cast<OSPFLocalObjectManager*>(getPWaveObjectManager()));
            prismAssert (NULL != pVirtualLinkLocalMO, __FILE__, __LINE__);
        }    

        //Updating param
        if( (OSPF_AREA_CONFIG_VIRTUAL_LINK <= (pVirtualLinkLocalMsg->getCmdCode())) &&  
            (OSPF_AREA_CONFIG_VIRTUAL_LINK_MD5_AUTH_KEY >=(pVirtualLinkLocalMsg->getCmdCode())) )
        {
			pVirtualLinkLocalMO->updateMOFromLocalMessage(pVirtualLinkLocalMsg);
        }

        if (createFlag == 0) {
            trace (TRACE_LEVEL_INFO, "createFlag = 0");

            updateWaveManagedObject (pVirtualLinkLocalMO);
            prismAssert (NULL != pVirtualLinkLocalMO, __FILE__, __LINE__);
        }

        pVirtualLinkLocalMO->setVirtualLinkRouterId(pVirtualLinkLocalMsg->getVirtualLinkRouterId());

        trace(TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep: virtuallink set ")
                + string(" to ")
                + pVirtualLinkLocalMsg->getAreaId());

        if (createFlag) {
            pAreaLocalMO->setVirtualLinkLocalMO(pVirtualLinkLocalMO);

            updateWaveManagedObject(pAreaLocalMO);

            //pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection (pVirtualLinkLocalMO);
            trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep: createFlag was set");
        }

        if (pResultsParent) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsParent);
            trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep: deleted pResultsParent");
            //delete pResultsParent;
        }    

        trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep: exiting");
        return status;
    }


    void  OSPFLocalConfWorker::OSPFVirtualLinkLocalMessageHandler(OSPFVirtualLinkLocalMessage *pOSPFVirtualLinkLocalMessage)
    {
        // Validations and Queries should go here
        trace (TRACE_LEVEL_INFO, "OSPFLocalConfWorker::OSPFVirtualLinkLocalMessageHandler: Entered....");

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::OSPFUpdateVirtualLinkStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        //PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pOSPFVirtualLinkLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pOSPFVirtualLinkLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }


    void  OSPFLocalConfWorker::OSPFRouterConfigLocalMessageHandler( OSPFRouterConfigLocalMessage *pOSPFRouterConfigLocalMessage)
    {
        // Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::ospfRouterConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::deleteOSPFIntfConfig),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::ospfRouterConfigMOStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        //PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pOSPFRouterConfigLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pOSPFRouterConfigLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pDceSynchronousLinearSeqContext->execute ();
    }

    void OSPFLocalConfWorker::OSPFPermitDenyRedistributeLocalMessageHandler (OSPFPermitDenyRedistributeLocalMessage *pOSPFPermitDenyLocalMessage)
    {
        // Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::OSPFPermitDenyRedistributeStep),
            //reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::OSPFPermitDenyRedistributeMOStep),

            //reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pOSPFPermitDenyLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  OSPFLocalConfWorker::OSPFLocalShowMessageHandler(OSPFLocalShowMessage *pMsg)
    {
        trace(TRACE_LEVEL_DEBUG,string("Entering OSPFLocalShowMessageHandler"));
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
             reinterpret_cast<PrismSynchronousLinearSequencerStep> (&OSPFLocalConfWorker::sendShowToOSPFDaemon),
             reinterpret_cast<PrismSynchronousLinearSequencerStep> (&OSPFLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
             reinterpret_cast<PrismSynchronousLinearSequencerStep> (&OSPFLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext *pSynchronousLinearSeqContext = new PrismSynchronousLinearSequencerContext(pMsg, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pSynchronousLinearSeqContext->execute ();

        trace(TRACE_LEVEL_DEBUG,string(" Leaving OSPFLocalShowMessageHandler"));
    }

    ResourceId OSPFLocalConfWorker::sendShowToOSPFDaemon(PrismSynchronousLinearSequencerContext *ctx_p) 
    {
         OSPFShowMessage *pShowOSPFMsg = NULL;
         ResourceId status = WAVE_MESSAGE_ERROR;
         OSPFLocalShowMessage *pMsg = NULL;
         bool firstReq = true;
         trace(TRACE_LEVEL_DEBUG,string("Entering SendShowToOSPFDaemon"));
         if(NULL==ctx_p->getPPrismMessage())
         {
              return status;
         }
         pMsg = dynamic_cast<OSPFLocalShowMessage *> (ctx_p->getPPrismMessage ());
         
         SI32 mappedId = DcmToolKit::getLocalMappedId();
         trace(TRACE_LEVEL_DEBUG,string("mappedId in localConfWorker") + mappedId);
         if(mappedId == -1) {
             mappedId = 0;
             trace(TRACE_LEVEL_DEBUG,string("mappedId in localConfWorker:1") + mappedId);
         }

         pShowOSPFMsg = new OSPFShowMessage();
         if(pShowOSPFMsg) 
         { 
             pShowOSPFMsg->setCmdCode(pMsg->getCmdCode());
             pShowOSPFMsg->setIpAddr1((pMsg->getIpAddr1()).toString());
             pShowOSPFMsg->setIpAddr2((pMsg->getIpAddr2()).toString());
             pShowOSPFMsg->setIntVal(toString(pMsg->getIntVal()));
             pShowOSPFMsg->setStrVal(pMsg->getStrVal());
             pShowOSPFMsg->setIfname(pMsg->getIfname());
             pShowOSPFMsg->setAreaId(pMsg->getAreaId());
             pShowOSPFMsg->setVRF(pMsg->getVRF());
             pShowOSPFMsg->setMoreDataFlag(pMsg->getMoreDataFlag());

             
             if(pMsg->getMoreDataFlag())
             {
                 firstReq = false;
                 trace(TRACE_LEVEL_DEBUG,string("Invoking copyAllBuffers") );
                 pShowOSPFMsg->copyAllBuffers(*pMsg);
                 pShowOSPFMsg->setMoreDataFlag(pMsg->getMoreDataFlag());
             }
             else
             {
                 firstReq = true;
             }
             trace(TRACE_LEVEL_DEBUG,string("cmdCode in Local handler:ShowMessage") +pShowOSPFMsg->getCmdCode());
             trace(TRACE_LEVEL_DEBUG,string("cmdCode in Local Message") +pMsg->getCmdCode());
             trace(TRACE_LEVEL_DEBUG,string("vrfNAme in Local Message") +pMsg->getVRF());
             trace(TRACE_LEVEL_DEBUG,string("vrfNAme showMessage") +pShowOSPFMsg->getVRF());
             trace(TRACE_LEVEL_DEBUG,string(" More Data Flag in showMessage") +pShowOSPFMsg->getMoreDataFlag());
             status = sendSynchronouslyToWaveClient("ospf",pShowOSPFMsg);

             if (status == WAVE_MESSAGE_SUCCESS)
             {
                 if (pShowOSPFMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
                 {
                     trace(TRACE_LEVEL_DEBUG, string("Resp: Client Completion Error MsgId: ") + pShowOSPFMsg->getMessageId() + FrameworkToolKit::localize(pShowOSPFMsg->getCompletionStatus()));
                     pMsg->setCompletionStatus(pShowOSPFMsg->getCompletionStatus());
                     status = pMsg->getCompletionStatus();
                     delete pShowOSPFMsg;
                     return status;
                 }
                 trace(TRACE_LEVEL_DEBUG,string("pShowOSPFMsg->getClientStatus()") + pShowOSPFMsg->getClientStatus());
                 if (pShowOSPFMsg->getClientStatus() != 0)
                 {
                     trace(TRACE_LEVEL_DEBUG, string("Resp:  Client Error MsgId: ") + pShowOSPFMsg->getMessageId() + FrameworkToolKit::localize(pShowOSPFMsg->getClientStatus()));
                     pMsg->setCompletionStatus(pShowOSPFMsg->getClientStatus());
                     status = pMsg->getCompletionStatus();
                     delete pShowOSPFMsg;
                     return status;
                 }
             }
             if (status != WAVE_MESSAGE_SUCCESS)
             {
                 trace(TRACE_LEVEL_DEBUG, string("Client Message sending failed for MsgId: ") + pShowOSPFMsg->getMessageId() + string("reason:") +  FrameworkToolKit::localize(status));
                 delete pShowOSPFMsg;
                 return status;
             }
             trace(TRACE_LEVEL_DEBUG, string("Client Message processing successful for MsgId: ") + pShowOSPFMsg->getMessageId());
             if(!firstReq)
             {
                 pShowOSPFMsg->removeBuffer(SHOW_OSPF_HANDLE_REQ_TAG);
             }
             pMsg->copyAllBuffers(*pShowOSPFMsg);
             status = pShowOSPFMsg->getCompletionStatus();
             delete pShowOSPFMsg;
        }
        return status;

    }


void OSPFLocalConfWorker::OSPFLocalClearMessageHandler( OSPFLocalClearMessage *pOSPFLocalClearMessage)
    {
        if (pOSPFLocalClearMessage->getisDistributionRequired())
        {
           PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep> (&OSPFLocalConfWorker::sendOSPFClearCommandToCluster),
                reinterpret_cast<PrismLinearSequencerStep> (&OSPFLocalConfWorker::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&OSPFLocalConfWorker::prismLinearSequencerFailedStep)
            };

            PrismLinearSequencerContext *pLinearSeqContext = new  PrismLinearSequencerContext( pOSPFLocalClearMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pLinearSeqContext->start();
            return;
        } else {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&OSPFLocalConfWorker::sendOSPFClearCommandToOSPFDaemon),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&OSPFLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&OSPFLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
            };

            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext(pOSPFLocalClearMessage , this, sequencerSteps,
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }


void OSPFLocalConfWorker::sendOSPFClearCommandToCluster(PrismLinearSequencerContext *seq_ctx_p)
    {
        NSM_PLUG_DBG_S("Entering ...");

        OSPFLocalClearMessage *msg_p = dynamic_cast<OSPFLocalClearMessage*> (seq_ctx_p->getPPrismMessage ());
        NSM_PLUG_ASSERT_PTR(msg_p);

        OSPFLocalClearMessage *loc_msg_p = new OSPFLocalClearMessage(msg_p);
        NSM_PLUG_ASSERT_PTR(loc_msg_p);
        loc_msg_p->setisDistributionRequired(false);

        WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&OSPFLocalConfWorker::OSPFClearCmdClusterCallback), seq_ctx_p);
        NSM_PLUG_ASSERT_PTR(send_ctx_p);

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        sendToWaveCluster(send_ctx_p);
        return;
    }

ResourceId OSPFLocalConfWorker::sendOSPFClearCommandToOSPFDaemon(DceSynchronousLinearSeqContext *seq_ctx_p)
    {
        NSM_PLUG_DBG_S(string("Entering ..."));
        OSPFLocalClearMessage *msg_p = dynamic_cast<OSPFLocalClearMessage*> (seq_ctx_p->getPPrismMessage ());
        OSPFClearMessage  *m = new OSPFClearMessage(msg_p);
        ResourceId status = sendSynchronouslyToWaveClient ("ospf",m);

        if(WAVE_MESSAGE_SUCCESS != status)
        {
            NSM_PLUG_DBG_S(string("sendSynchronouslyToWaveClient Failed") + status);
            delete m;
            return status;
        }
        else
        {

              if(msg_p->getCmdCode()==CLEAR_IP_OSPF_ALL)
              {
                   OSPFConfigLocalManagedObject *pOSPFConfigLocalMo = getOSPFConfigManagedObject( msg_p->getVRF());
                   trace(TRACE_LEVEL_INFO,"cleardaemon after getOSPFConfigManagedObject");
                   if(pOSPFConfigLocalMo != NULL)
                   {
                         if (pOSPFConfigLocalMo->getExtMaxMetric()||pOSPFConfigLocalMo->getSummMaxMetric()||
                             pOSPFConfigLocalMo->getIsLinkPtp()||pOSPFConfigLocalMo->getIsLinkStub()||pOSPFConfigLocalMo->getIsLinkTransit()/*||getRouterLsa()*/)
                         {
                               trace(TRACE_LEVEL_DEVEL," clear ip ospf all dbUpdateRequired\n");
                               startTransaction();
                               if(pOSPFConfigLocalMo->getExtMaxMetric())
                               {
                                    pOSPFConfigLocalMo->setExtMaxMetric(OSPF_DEFAULT_MAX_METRIC_EXT_METRIC);
                                    pOSPFConfigLocalMo->setExtMaxMetricLsa(false);
                                    pOSPFConfigLocalMo->addAttributeToBeUpdated("extMaxMetric");
                                    pOSPFConfigLocalMo->addAttributeToBeUpdated("extMaxMetricLsa");
                               }
                               if(pOSPFConfigLocalMo->getSummMaxMetric())
                               {
                                    pOSPFConfigLocalMo->setSummMaxMetric(OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC);
                                    pOSPFConfigLocalMo->setSummMaxMetricLsa(false);
                                    pOSPFConfigLocalMo->addAttributeToBeUpdated("summMaxMetric");
                                    pOSPFConfigLocalMo->addAttributeToBeUpdated("summMaxMetricLsa");
                               }
                               if(pOSPFConfigLocalMo->getIsLinkPtp())
                               {
                                    pOSPFConfigLocalMo->setIsLinkPtp(false);
                                    pOSPFConfigLocalMo->addAttributeToBeUpdated("linkPtp");
                               }
                               if(pOSPFConfigLocalMo->getIsLinkStub())
                               {
                                    pOSPFConfigLocalMo->setIsLinkStub(false);
                                    pOSPFConfigLocalMo->addAttributeToBeUpdated("linkStub");
                               }
                               if(pOSPFConfigLocalMo->getIsLinkTransit())
                               {
                                    pOSPFConfigLocalMo->setIsLinkTransit(false);
                                    pOSPFConfigLocalMo->addAttributeToBeUpdated("linkTransit");
                               }
                               if(!pOSPFConfigLocalMo->getMaxMetricOnStartup())
                               {
                                    pOSPFConfigLocalMo->setRouterLsa(false);
                                    pOSPFConfigLocalMo->addAttributeToBeUpdated("routerLsa");
                               }
                               updateWaveManagedObject(pOSPFConfigLocalMo);
                               commitTransaction();
                         }
                         delete (pOSPFConfigLocalMo);
                    }       

              }                                                             
             
        }
        if(m){
            delete m;
        }
        return status;
    }



void OSPFLocalConfWorker::OSPFClearCmdClusterCallback(WaveSendToClusterContext *ctx_p)
    {
        NSM_PLUG_DBG_S("Entering...");
        PrismLinearSequencerContext            *seq_ctx_p = NULL;
        OSPFLocalClearMessage *req_msg_p = NULL;
        OSPFLocalClearMessage *resp_msg_p = NULL;
        OSPFLocalClearMessage *sw_resp_p = NULL;


        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());

        NSM_PLUG_ASSERT_PTR(seq_ctx_p);

        req_msg_p = dynamic_cast<OSPFLocalClearMessage*> (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<OSPFLocalClearMessage*> (ctx_p->getPPrismMessageForPhase1());

        NSM_PLUG_ASSERT_PTR(req_msg_p);
        NSM_PLUG_ASSERT_PTR(resp_msg_p);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        NSM_PLUG_DBG_S(string("Response processing: Num resp:") + locations.size());


        for (i = 0; i < locations.size(); i++)
        {
            sw_resp_p = dynamic_cast<OSPFLocalClearMessage*> (ctx_p->getResultingMessageForPhase1(locations[i]));
            NSM_PLUG_ASSERT_PTR(sw_resp_p);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
            {
               NSM_PLUG_DBG_S((string("Response processing: The location: ") + locations[i] ) );
            }
            else
            {
                NSM_PLUG_DBG_S((string("Response processing: The location: ") + locations[i] ) );
                NSM_PLUG_DBG_S (string("returned error: ")  + FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
            }
        }
        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;
        return;

}
}

