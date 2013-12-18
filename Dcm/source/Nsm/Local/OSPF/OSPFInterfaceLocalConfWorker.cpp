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
 *   Author : kverma                                                       *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/OSPF/OSPFInterfaceLocalConfWorker.h"
#include "Nsm/Local/OSPF/OSPFInterfaceLocalManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "OSPF/Local/OSPFAreaLocalManagedObject.h"
#include "ClientInterface/OSPF/OSPFInterfaceConfigMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "Nsm/Local/NsmTypes.h"

#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfLoopbackManagedObject.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "ClientInterface/InterfaceCommon.h"
#include "Utils/Layer3LicenseUtil.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Vrf/Local/VrfLocalManagedObject.h"

#define OSPF_PLUG_ASSERT_PTR(exp) \
            prismAssert ((exp) != NULL, __FILE__, __LINE__);
#define OSPF_DFLT_MAP_ID 0
            

namespace DcmNs
{

    OSPFInterfaceLocalConfWorker::OSPFInterfaceLocalConfWorker ( NsmLocalObjectManager *pNsmLocalObjectManager)
        : WaveWorker  (pNsmLocalObjectManager)
    {
	    OSPFInterfaceLocalManagedObject    OSPFInterfaceLocalManagedObject    (pNsmLocalObjectManager);
        OSPFInterfaceLocalManagedObject.setupOrm ();
        addManagedClass (OSPFInterfaceLocalManagedObject::getClassName (), this);

        addOperationMap (NSMLOCALOSPFINTERFACECONFIG, reinterpret_cast<PrismMessageHandler> (&OSPFInterfaceLocalConfWorker::NsmLocalOSPFInterfaceConfigMessageHandler));
    }

    OSPFInterfaceLocalConfWorker::~OSPFInterfaceLocalConfWorker ()
    {
    }

    PrismMessage  *OSPFInterfaceLocalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMLOCALOSPFINTERFACECONFIG :
                pPrismMessage = new OSPFInterfaceLocalMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *OSPFInterfaceLocalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((OSPFInterfaceLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new OSPFInterfaceLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "OSPFInterfaceLocalConfWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    ResourceId OSPFInterfaceLocalConfWorker::validateKeyLength(OSPFInterfaceConfigMessage *pOSPFInterfaceConfigMessage)
    {
        switch (pOSPFInterfaceConfigMessage->getCmdCode())
        {
            case OSPF_INTF_AUTH_KEY:
                if((pOSPFInterfaceConfigMessage->getEncryptionType()) == OSPF_SIMPLE_ENCR )
                {
                    if( ((pOSPFInterfaceConfigMessage->getAuthKey()).length()) > OSPF_AUTH_KEY_VALID_SIZE )
                        return WRC_OSPF_CLI_ERR_EXCEEDED_AUTH_KEY_SIZE;
                }
                else
                    if( ((pOSPFInterfaceConfigMessage->getAuthKey()).length()) > OSPF_AUTH_KEY_VALID_SIZE_8 )
                        return WRC_OSPF_CLI_ERR_EXCEEDED_AUTH_KEY_SIZE_8;
                break;
                
            case OSPF_INTF_MD5_AUTH_KEY:
                if((pOSPFInterfaceConfigMessage->getMD5EncryptionType()) == OSPF_SIMPLE_ENCR )
                {
                    if( ((pOSPFInterfaceConfigMessage->getMD5Key()).length()) > OSPF_MD5_AUTH_KEY_VALID_SIZE )
                        return WRC_OSPF_CLI_ERR_EXCEEDED_MD5_KEY_SIZE;
                }        
                else
                    if( ((pOSPFInterfaceConfigMessage->getMD5Key()).length()) > OSPF_MD5_AUTH_KEY_VALID_SIZE_16 )
                        return WRC_OSPF_CLI_ERR_EXCEEDED_MD5_KEY_SIZE_16;
                break;

            default:
                break;

        }
        
        return WAVE_MESSAGE_SUCCESS; 
    }

	ResourceId OSPFInterfaceLocalConfWorker::ospfInterfaceConfigStep(DceSynchronousLinearSeqContext  *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
        
		OSPFInterfaceLocalMessage *pOSPFInterfaceLocalMessage =
			dynamic_cast<OSPFInterfaceLocalMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

		trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigStep message Local plugin entering");
		
        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();

        NSM_PLUG_INFO_S(string("Check the IfcType and retrieve the InterfaceLocalMO Physical/Svi/Loopback"));
        OSPFInterfaceConfigMessage *pOSPFInterfaceConfigMessage = new OSPFInterfaceConfigMessage();
        UI64 ifIndex = 0;
        if( PHY_INT == (pOSPFInterfaceLocalMessage->getintfType()) )
        {
        	NSM_PLUG_INFO_S(string("Retrieve the PhyIntfLocalManagedObject for ")+pOSPFInterfaceLocalMessage->getintfName());
			PhyIntfLocalManagedObject *aPhyIntfLocalManagedObject = NsmL3Util::PhyIfNameGetPhyIfLocalMO
					(pOSPFInterfaceLocalMessage->getintfName(), pPrismSynchronousLinearSequencerContext);
			if(aPhyIntfLocalManagedObject) {
                //Keep in the cache so that subsequent steps need not query the database again
				pPrismSynchronousLinearSequencerContext->getCache()->put(PhyIntfLocalManagedObject::getClassName(), aPhyIntfLocalManagedObject);
				//Ensure that this cache object is garbage collected
				if(aPhyIntfLocalManagedObject) {
					pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(aPhyIntfLocalManagedObject);
					ifIndex = aPhyIntfLocalManagedObject->getIfIndex();
					NSM_PLUG_INFO_S(string("IfIndex of the PhysicalInterface=")+ifIndex);
				}
                if(aPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId) {
                	 NSM_PLUG_DBG_ERR_S(string("Cannot configure OSPF on a aggregator member"));
                     return WRC_NSM_ERR_SW_NOT_ALLOWEED;
                }
                if ((DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (aPhyIntfLocalManagedObject->getPortrole())) {
                	NSM_PLUG_DBG_ERR_S(string("Cannot configure OSPF on an ISL port "));
                    return WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
                }
			}
        }
        if( VLAN_INT == (pOSPFInterfaceLocalMessage->getintfType()) )
        {
        	UI32 vlanId = atoi(pOSPFInterfaceLocalMessage->getintfName().c_str());
        	NSM_PLUG_INFO_S(string("Configuring for Ve Interface for vlanId=")+vlanId);
        	NsmIntfSviManagedObject *sviMo = NsmL3Util :: VlanIdGetIntfSviMO(vlanId, DcmToolKit :: getLocalMappedId(), pPrismSynchronousLinearSequencerContext, false);
			if(sviMo) {
                //Keep in the cache so that subsequent steps need not query the database again
				pPrismSynchronousLinearSequencerContext->getCache()->put(NsmIntfSviManagedObject::getClassName(), sviMo);
				ifIndex = sviMo->getIfIndex();
				NSM_PLUG_INFO_S(string("IfIndex of the Ve Interface=")+ifIndex);
				//Ensure that this cache object is garbage collected
				pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(sviMo);
			}
         }
         if(LOOPBACK_INT == (pOSPFInterfaceLocalMessage->getintfType()) )
         {

        	UI32 loopbackId = atoi(pOSPFInterfaceLocalMessage->getintfName().c_str());
        	NSM_PLUG_INFO_S(string("Configuring for loopback Interface for loopbackId=")+loopbackId);
            NsmIntfLoopbackManagedObject *loopbackMo = NsmL3Util ::loIdGetLoopbackIfMO(loopbackId, DcmToolKit::getLocalMappedId ());
            if(loopbackMo) {
                pPrismSynchronousLinearSequencerContext->getCache()->put(NsmIntfLoopbackManagedObject::getClassName(), loopbackMo);
                ifIndex = loopbackMo->getIfIndex();
                NSM_PLUG_INFO_S(string("IfIndex of the loopback Interface=")+ifIndex);
                //Ensure that this cache object is garbage collected
                pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(loopbackMo);
        	}
         }
        pOSPFInterfaceConfigMessage->setIfindex(ifIndex);


        NsmL3ReadyManagedObject *pNsmL3ReadyManagedObject = NULL;

		if (pOSPFInterfaceLocalMessage->getCmdCode() != OSPF_INTF_DEL) {
			NSM_PLUG_INFO_S(string("CmdCode != OSPF_INTF_DEL. Retrieve the L3ReadyMO"));
			//Get Interface Object 
			pNsmL3ReadyManagedObject =
				ospfGetL3RdyMo(pPrismSynchronousLinearSequencerContext, pOSPFInterfaceLocalMessage);
			pPrismSynchronousLinearSequencerContext->getCache()->put(NsmL3ReadyManagedObject::getClassName(), pNsmL3ReadyManagedObject);
			//Ensure that this cache object is garbage collected
			if(pNsmL3ReadyManagedObject) {
				pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pNsmL3ReadyManagedObject);
			}
			if(!pNsmL3ReadyManagedObject)
			{
				trace (TRACE_LEVEL_ERROR, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigStep L3ReadyMO not found for this "
						"interface, returning");
				pOSPFInterfaceLocalMessage->setCompletionStatus(WRC_OSPF_CLI_ERR_L3_NOT_READY);
				return (WRC_OSPF_CLI_ERR_L3_NOT_READY);
			}
		}


		pOSPFInterfaceConfigMessage->setOpCode(pOSPFInterfaceLocalMessage->getOpCode());
		pOSPFInterfaceConfigMessage->setintfName(pOSPFInterfaceLocalMessage->getintfName());
        if( PHY_INT == (pOSPFInterfaceLocalMessage->getintfType()) )
        {
        	PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *> (pPrismSynchronousLinearSequencerContext->getCache()->getWaveManagedObject(PhyIntfLocalManagedObject::getClassName()));
        	/*
        	PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NsmL3Util::PhyIfNameGetPhyIfLocalMO
					(pOSPFInterfaceLocalMessage->getintfName(), pPrismSynchronousLinearSequencerContext);
			*/
            if(pPhyIntfLocalManagedObject)
                pOSPFInterfaceConfigMessage->setintfType(pPhyIntfLocalManagedObject->getPhyType());
            trace (TRACE_LEVEL_INFO, string("OSPFInterfaceLocalConfWorker::ospfInterfaceConfigStep: IfType ") + pOSPFInterfaceConfigMessage->getintfType());    
        }
        else
    		pOSPFInterfaceConfigMessage->setintfType(pOSPFInterfaceLocalMessage->getintfType());

		/*
		//vs
		printf("\n%s, %d: RECEIVED %d, %d", __FUNCTION__, __LINE__,
			   pOSPFInterfaceLocalMessage->getCmdCode(),
			   pOSPFInterfaceLocalMessage->getOpCode());*/

		if (pOSPFInterfaceLocalMessage->getOpCode() != OSPF_REMOVE)
		{/*fill in the set element code here*/
			NSM_PLUG_INFO_S(string("OpCode != OSPF_REMOVE. Set the client message from the local message"));
			pOSPFInterfaceConfigMessage->setCmdCode(pOSPFInterfaceLocalMessage->getCmdCode());
			switch(pOSPFInterfaceLocalMessage->getCmdCode())
			{
				case OSPF_INTF_AREA:
				{
					pOSPFInterfaceConfigMessage->setAreaId(pOSPFInterfaceLocalMessage->getAreaId());
					break;
				}
				case OSPF_INTF_HELLO:
				{
					pOSPFInterfaceConfigMessage->setHelloInterval(pOSPFInterfaceLocalMessage->getHelloInterval());
					break;
				}
				case OSPF_INTF_DEAD:
				{
					pOSPFInterfaceConfigMessage->setRouterDeadInterval(pOSPFInterfaceLocalMessage->getRouterDeadInterval());
					break;
				}
				case OSPF_INTF_RETRANSMIT:
				{
					pOSPFInterfaceConfigMessage->setRetransmitInterval(pOSPFInterfaceLocalMessage->getRetransmitInterval());
					break;
				}
				case OSPF_INTF_TRANSDELAY:
				{
					pOSPFInterfaceConfigMessage->setTransDelay(pOSPFInterfaceLocalMessage->getTransDelay());
					break;
				}
				case OSPF_INTF_COST:
				{
					pOSPFInterfaceConfigMessage->setCost(pOSPFInterfaceLocalMessage->getCost());
					break;
				}
				case OSPF_INTF_PRIORITY:
				{
					pOSPFInterfaceConfigMessage->setPriority(pOSPFInterfaceLocalMessage->getPriority());
					break;
				}
				case OSPF_INTF_AUTH_CHANGE_WAIT_TIME:
				{
					pOSPFInterfaceConfigMessage->setAuthChangeWaitTime(pOSPFInterfaceLocalMessage->getAuthChangeWaitTime());
					break;
				}
				case OSPF_INTF_AUTH_KEY:
				{
                    OSPFInterfaceLocalManagedObject *pOSPFInterfaceLocalManagedObject =
                        OSPFGetOSPFInterfaceLocalManagedObject(pPrismSynchronousLinearSequencerContext);

                    if(pOSPFInterfaceLocalManagedObject)
                    {
                        if(pOSPFInterfaceLocalMessage->getEncryptionType() != OSPF_INVALID_ENCR)
                            pOSPFInterfaceConfigMessage->setEncryptionType(pOSPFInterfaceLocalMessage->getEncryptionType());
                        else
                            pOSPFInterfaceConfigMessage->setEncryptionType(pOSPFInterfaceLocalManagedObject->getEncryptionType());
                        
                        if( (pOSPFInterfaceLocalMessage->getAuthKey()).length() )
                            pOSPFInterfaceConfigMessage->setAuthKey(pOSPFInterfaceLocalMessage->getAuthKey());
                        else
                             pOSPFInterfaceConfigMessage->setAuthKey(pOSPFInterfaceLocalManagedObject->getAuthKey());

                        //Validate-Key-Length
                        status = validateKeyLength(pOSPFInterfaceConfigMessage);
                        if( status != WAVE_MESSAGE_SUCCESS )
                        {
                            pOSPFInterfaceLocalMessage->setCompletionStatus(status);
                            delete (pOSPFInterfaceConfigMessage);
                            return status;
                        }
                    }
                    else
                    {
					    pOSPFInterfaceConfigMessage->setEncryptionType(pOSPFInterfaceLocalMessage->getEncryptionType());
    					pOSPFInterfaceConfigMessage->setAuthType(pOSPFInterfaceLocalMessage->getAuthType());
	    				pOSPFInterfaceConfigMessage->setAuthKey(pOSPFInterfaceLocalMessage->getAuthKey());
                    }    
					break;
				}
				case OSPF_INTF_MD5_KEY_ACTV_WAIT_TIME:
				{
					pOSPFInterfaceConfigMessage->setMD5KeyActWaitTime(pOSPFInterfaceLocalMessage->getMD5KeyActWaitTime());
					pOSPFInterfaceConfigMessage->setAuthType(pOSPFInterfaceLocalMessage->getAuthType());
					break;
				}
				case OSPF_INTF_MD5_AUTH_KEY:
				{
                    OSPFInterfaceLocalManagedObject *pOSPFInterfaceLocalManagedObject =
                        OSPFGetOSPFInterfaceLocalManagedObject(pPrismSynchronousLinearSequencerContext);

                    if(pOSPFInterfaceLocalManagedObject)
                    {
                        if( pOSPFInterfaceLocalMessage->getMD5EncryptionType() != OSPF_INVALID_ENCR ) 
    				        pOSPFInterfaceConfigMessage->setMD5EncryptionType(pOSPFInterfaceLocalMessage->getMD5EncryptionType());
                        else
                            pOSPFInterfaceConfigMessage->setMD5EncryptionType(pOSPFInterfaceLocalManagedObject->getMD5EncryptionType());

                        if( (pOSPFInterfaceLocalMessage->getMD5Key()).length() )
    					    pOSPFInterfaceConfigMessage->setMD5Key(pOSPFInterfaceLocalMessage->getMD5Key());
                        else
                            pOSPFInterfaceConfigMessage->setMD5Key(pOSPFInterfaceLocalManagedObject->getMD5Key());

                        if( (pOSPFInterfaceLocalMessage->getMD5KeyId()) < 256 )    
        					pOSPFInterfaceConfigMessage->setMD5KeyId(pOSPFInterfaceLocalMessage->getMD5KeyId());
                        else
                            pOSPFInterfaceConfigMessage->setMD5KeyId(pOSPFInterfaceLocalManagedObject->getMD5KeyId());
                            
                     }
                     else
                     {
                        pOSPFInterfaceConfigMessage->setMD5EncryptionType(pOSPFInterfaceLocalMessage->getMD5EncryptionType());
                        pOSPFInterfaceConfigMessage->setAuthType(pOSPFInterfaceLocalMessage->getAuthType());
                        pOSPFInterfaceConfigMessage->setMD5KeyId(pOSPFInterfaceLocalMessage->getMD5KeyId());
                     }

       				pOSPFInterfaceConfigMessage->setAuthType(pOSPFInterfaceLocalMessage->getAuthType());

                    //Validate-Key-Length
                    status = validateKeyLength(pOSPFInterfaceConfigMessage);
                    if( status != WAVE_MESSAGE_SUCCESS )
                    {
                        pOSPFInterfaceLocalMessage->setCompletionStatus(status);
                        delete (pOSPFInterfaceConfigMessage);
                        return status;
                    }    
					break;
				}
				// ip ospf database-filter all-out
				case OSPF_INTF_FILTER_ALL:
				{
					pOSPFInterfaceConfigMessage->setFilterAll(true);
                    break;
				}
				// ip ospf database-filter {all-external | all-summary-external} *
				// pOSPFInterfaceLocalMessage is filled in by the CPWorker
				case OSPF_INTF_FILTER_EXT:
				case OSPF_INTF_FILTER_SUMM_EXT:
				{
					pOSPFInterfaceConfigMessage->setFilterOption(pOSPFInterfaceLocalMessage->getFilterOption());
					break;
				}

				case OSPF_INTF_MTU_IGNORE:
				{
					pOSPFInterfaceConfigMessage->setmtuIgnore(pOSPFInterfaceLocalMessage->getmtuIgnore());
					break;
				}
				case OSPF_INTF_NETWORK:
				{
					pOSPFInterfaceConfigMessage->setNetworkType(pOSPFInterfaceLocalMessage->getNetworkType());
					break;
				}
				case OSPF_INTF_ACTIVE:
				{
					pOSPFInterfaceConfigMessage->setActive(pOSPFInterfaceLocalMessage->getActive());
					break;
				}
				case OSPF_INTF_PASSIVE:
				{
					pOSPFInterfaceConfigMessage->setPassive(pOSPFInterfaceLocalMessage->getPassive());
					break;
				}
				default:
				{
			        trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigStep default case: Invalid command code");
					break;
				}
			}
		}
		else
		{/*Fill in the remove case code here*/
			NSM_PLUG_INFO_S(string("OpCode == OSPF_REMOVE."));
			if(pOSPFInterfaceLocalMessage->getCmdCode() == OSPF_INTF_DEL)
			{
				NSM_PLUG_INFO_S(string("CmdCode == OSPF_INTF_DEL."));
				pOSPFInterfaceLocalMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        		delete (pOSPFInterfaceConfigMessage);
				return WAVE_MESSAGE_SUCCESS;
			}

			if (!pNsmL3ReadyManagedObject) {
                return WAVE_MESSAGE_ERROR; // This should not happen unless the code gets messed up
            }

            OSPFInterfaceLocalManagedObject *pOSPFInterfaceLocalManagedObject = 
                ((WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject> 
                  (pNsmL3ReadyManagedObject->getOSPFIntfConfig())).operator->());

            if(!pOSPFInterfaceLocalManagedObject) {
                // To prevent a request to delete a nonexistent config (which may occur with Brocade WyserEa read
                // optimizations) from causing any issues, we skip the rest of the sequence and cause it to return success.
                delete (pOSPFInterfaceConfigMessage);
                return WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
            }

			pOSPFInterfaceConfigMessage->setCmdCode(pOSPFInterfaceLocalMessage->getCmdCode());
			switch(pOSPFInterfaceLocalMessage->getCmdCode())
			{
				case OSPF_INTF_AREA:
				{
					NSM_PLUG_INFO_S(string("CmdCode == OSPF_INTF_AREA."));
                    //Retriving the area string from OSPFAreaLocalManagedObject.
                    OSPFAreaLocalManagedObject *pAreaLocalMO = NULL;    

                    if( (pOSPFInterfaceLocalManagedObject->getArea()) != ObjectId::NullObjectId )
                    {
                        pAreaLocalMO = dynamic_cast<OSPFAreaLocalManagedObject*>( queryManagedObject (pOSPFInterfaceLocalManagedObject->getArea()) );

                        if(!pAreaLocalMO) {
                            NSM_PLUG_DBG_ERR_S(string("Failed to retrieve OSPFAreaLocalManagedObject"));
                            return WAVE_MESSAGE_ERROR;
                        }
                    }
                    else
                        return WAVE_MESSAGE_ERROR;
                    
                    trace (TRACE_LEVEL_INFO, string("OSPFInterfaceLocalConfWorker::ospfInterfaceConfigStep : Area: ") + pAreaLocalMO->getAreaId());

                    pOSPFInterfaceConfigMessage->setAreaId(pAreaLocalMO->getAreaId());
                    pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pAreaLocalMO);
					break;
				}

				// For no ip ospf database-filter
				// The CPWorker only sends us a 'no', we gotta figure out the rest so we
				// can send the full CLI to the backend
				case OSPF_INTF_FILTER_ALL:
				{
					pOSPFInterfaceConfigMessage->setFilterAll(false);
                    break;
				}
                case OSPF_INTF_FILTER_EXT:
                {
					trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigStep cmd: OSPF_INTF_FILTER_EXT");
                    OSPFInterfaceLocalManagedObject *MO =
                        OSPFGetOSPFInterfaceLocalManagedObject(pPrismSynchronousLinearSequencerContext);

                    if (MO) 
					{
						pOSPFInterfaceConfigMessage->setFilterOption(MO->getFilterAllExt());
					}
					break;
                }
				case OSPF_INTF_FILTER_SUMM_EXT:
                {
					trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigStep cmd: OSPF_INTF_FILTER_SUMM_EXT");
                    OSPFInterfaceLocalManagedObject *MO =
                        OSPFGetOSPFInterfaceLocalManagedObject(pPrismSynchronousLinearSequencerContext);

                    if (MO) 
					{
						pOSPFInterfaceConfigMessage->setFilterOption(MO->getFilterAllSumExt());
					}
					break;
                }
				default:
				{
					break;
				}
			}
		}


		trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigStep ospf interface-config Local message recived");
		pOSPFInterfaceConfigMessage->dumpMessage();

        status = sendSynchronouslyToWaveClient("ospf",pOSPFInterfaceConfigMessage);

		if(status == WAVE_MESSAGE_SUCCESS)
		{
			status = pOSPFInterfaceConfigMessage->getClientStatus();
	        trace (TRACE_LEVEL_INFO, string("OSPFInterfaceLocalConfWorker::ospfInterfaceConfigStep ospf interface-config Local message sent to OSPF backend, client status is : ") + FrameworkToolKit::localize(status));
           
            /*Update with any backend received values(e.g. encrypted passwd)*/
            ospfUpdateLocalMessage(pOSPFInterfaceLocalMessage, pOSPFInterfaceConfigMessage);
        }
		else
	        trace (TRACE_LEVEL_ERROR, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigStep ospf interface-config Local message not sent to OSPF backend");

		pOSPFInterfaceLocalMessage->setCompletionStatus(status);


		/* //vs
		printf("\n%s, %d: DONE WTIH %d, %d", __FUNCTION__, __LINE__,
			   pOSPFInterfaceLocalMessage->getCmdCode(),
			   pOSPFInterfaceLocalMessage->getOpCode()); */

        delete (pOSPFInterfaceConfigMessage);
        return (status);
	}

    ResourceId OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep(DceSynchronousLinearSeqContext  *pPrismSynchronousLinearSequencerContext)
	{
        OSPFInterfaceLocalMessage *pOSPFInterfaceLocalMessage =
            dynamic_cast<OSPFInterfaceLocalMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
        string vrfName = pOSPFInterfaceLocalMessage->getPartitionName();
        NSM_PLUG_INFO_S(string("VRF Name:")+vrfName);

		OSPFInterfaceLocalManagedObject *pOSPFInterfaceLocalManagedObject = NULL;
		ObjectId vrfObjectIdOfIfc;
		ObjectId ownerPartitionManagedObjectId;

		//vector<WaveManagedObject *> *pResults = NULL;
		vector<WaveManagedObject *> *pAreaResults = NULL;
		vector<WaveManagedObject *> *pVrfResults = NULL;
        bool createFlag = false;
		LocationId locId = FrameworkToolKit::getThisLocationId();

        NsmL3ReadyManagedObject *pNsmL3ReadyManagedObject = NULL; 
		trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep Entering...");
		if((pOSPFInterfaceLocalMessage->getOpCode() != OSPF_REMOVE) || (pOSPFInterfaceLocalMessage->getCmdCode() != OSPF_INTF_DEL))
		{
			NSM_PLUG_INFO_S(string("OpCode != OSPF_REMOVE or CmdCode != OSPF_INTF_DEL. Retrieve L3ReadyMO"));
			//Get Interface Object
			/*
			pNsmL3ReadyManagedObject = 
				ospfGetL3RdyMo(pPrismSynchronousLinearSequencerContext, pOSPFInterfaceLocalMessage);
			*/
			pNsmL3ReadyManagedObject = dynamic_cast<NsmL3ReadyManagedObject *> (pPrismSynchronousLinearSequencerContext->getCache()->getWaveManagedObject(NsmL3ReadyManagedObject::getClassName()));
			OSPF_PLUG_ASSERT_PTR(pNsmL3ReadyManagedObject);

			UI32 interfaceType;
            if( PHY_INT == (pOSPFInterfaceLocalMessage->getintfType()) )
            {
            	NSM_PLUG_INFO_S(string("Configuring OSPF for Physical Interface:"));
            	PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *> (pPrismSynchronousLinearSequencerContext->getCache()->getWaveManagedObject(PhyIntfLocalManagedObject::getClassName()));
                if(pPhyIntfLocalManagedObject) {
                	interfaceType = pPhyIntfLocalManagedObject->getPhyType();
                    vrfObjectIdOfIfc = pPhyIntfLocalManagedObject->getVrfObjectId();
                    NSM_PLUG_INFO_S(string("VRF ObjectId of the Physical Interface:")+vrfObjectIdOfIfc.toString());
                }
            }
            else {
            	if(VLAN_INT == (pOSPFInterfaceLocalMessage->getintfType())) {
            		NSM_PLUG_INFO_S(string("Configuring OSPF for Ve Interface:"));
            		NsmIntfSviManagedObject *sviMo = dynamic_cast<NsmIntfSviManagedObject *> (pPrismSynchronousLinearSequencerContext->getCache()->getWaveManagedObject(NsmIntfSviManagedObject::getClassName()));
                    if(sviMo) {
                        vrfObjectIdOfIfc = sviMo->getVrfObjectId();
                        NSM_PLUG_INFO_S(string("VRF ObjectId of the Ve Interface:")+vrfObjectIdOfIfc.toString());
                    }
            	}
            	else if(LOOPBACK_INT == (pOSPFInterfaceLocalMessage->getintfType())) {
            		NSM_PLUG_INFO_S(string("Configuring OSPF for Loopback Interface:"));
            		NsmIntfLoopbackManagedObject *loopbackMo = dynamic_cast<NsmIntfLoopbackManagedObject *> (pPrismSynchronousLinearSequencerContext->getCache()->getWaveManagedObject(NsmIntfLoopbackManagedObject::getClassName()));
                    if(loopbackMo) {
                        vrfObjectIdOfIfc = loopbackMo->getVrfObjectId();
                        NSM_PLUG_INFO_S(string("VRF ObjectId of the Loopback Interface:")+vrfObjectIdOfIfc.toString());
                    }
            	}
            	interfaceType = pOSPFInterfaceLocalMessage->getintfType();
            }
            trace (TRACE_LEVEL_INFO, string("OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep: Interface Type ") + interfaceType);

			pOSPFInterfaceLocalManagedObject = 
				((WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject> 
				  (pNsmL3ReadyManagedObject->getOSPFIntfConfig())).operator->());

			if(!pOSPFInterfaceLocalManagedObject)
			{
				trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep: OSPFInterfaceLocalManagedObject not found ");

				pOSPFInterfaceLocalManagedObject = new OSPFInterfaceLocalManagedObject(
					dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
				pNsmL3ReadyManagedObject->setOSPFIntfConfig(pOSPFInterfaceLocalManagedObject);
				pOSPFInterfaceLocalManagedObject->setId(pOSPFInterfaceLocalMessage->getintfName());
				pOSPFInterfaceLocalManagedObject->setType(interfaceType);
				pOSPFInterfaceLocalManagedObject->setArea(ObjectId::NullObjectId);
				updateWaveManagedObject (pNsmL3ReadyManagedObject);
				createFlag = true;
			}    
			OSPF_PLUG_ASSERT_PTR(pOSPFInterfaceLocalManagedObject);
			prismAssert (NULL != pOSPFInterfaceLocalManagedObject, __FILE__, __LINE__);
		}
		if (pOSPFInterfaceLocalMessage->getOpCode() == OSPF_CREATE) 
		{
			trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep Creating OSPF interface Managed Object");

            //Creation and linking OSPFInterfaceLocalManagedObject already done by the time we reach here
			prismAssert (NULL != pOSPFInterfaceLocalManagedObject, __FILE__, __LINE__);
			
		}
		else
		{
			trace (TRACE_LEVEL_DEVEL, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep Found OSPFInterfaceLocalManagedObject");
		}

		NSM_PLUG_INFO_S(string("Retrieve ownerPartitionManagedObjectId of the VRF MO using the vrfObjectId:")+vrfObjectIdOfIfc.toString());
		VrfLocalManagedObject *vrfMo = NULL;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VrfLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute(new AttributeObjectId(&vrfObjectIdOfIfc, "objectid"));
		pVrfResults = querySynchronouslyLocalManagedObjectsForLocationId (locId,&syncQueryCtxt);
		pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pVrfResults);
		if (NULL != pVrfResults)
		{
			NSM_PLUG_INFO_S(string("NULL != pVrfResults"));
			UI32 numberOfVrfs = pVrfResults->size ();
			if(numberOfVrfs > 0) {
				NSM_PLUG_INFO_S(string("numberOfVrfs > 0"));
				vrfMo = dynamic_cast<VrfLocalManagedObject *>((*pVrfResults)[0]);
			}
		}
		if(vrfMo) {
			ownerPartitionManagedObjectId = vrfMo->getOwnerPartitionManagedObjectId();
			NSM_PLUG_INFO_S(string("ownerPartitionManagedObjectId of VRF MO :")+ownerPartitionManagedObjectId.toString());
		}
		else {
			NSM_PLUG_INFO_S(string("Failed to retrieve VRF MO. The config might be for an interface in default VRF"));
		}



		if(pOSPFInterfaceLocalMessage->getOpCode() != OSPF_REMOVE)
		{
			NSM_PLUG_INFO_S(string("OpCode != OSPF_REMOVE "));
			/*Update the relevant fields here*/
			switch(pOSPFInterfaceLocalMessage->getCmdCode())
			{
			    case OSPF_INTF_AREA:
				{
					OSPFAreaLocalManagedObject *pOSPFAreaLocalManagedObject = NULL;
					string areaId = pOSPFInterfaceLocalMessage->getAreaId();
					WaveManagedObjectSynchronousQueryContext areasyncQueryCtxt(OSPFAreaLocalManagedObject::getClassName());
					areasyncQueryCtxt.addAndAttribute (new AttributeString(&areaId,"areaId"));
		        	trace (TRACE_LEVEL_DEVEL,(string("OwnerPartitionManagedObjectId:")+ownerPartitionManagedObjectId.toString()));
		        	areasyncQueryCtxt.addAndAttribute(new AttributeObjectId(&ownerPartitionManagedObjectId, "ownerPartitionManagedObjectId"));

					pAreaResults = querySynchronouslyLocalManagedObjectsForLocationId (locId, &areasyncQueryCtxt);

					if (NULL != pAreaResults) 
					{
						trace (TRACE_LEVEL_DEVEL,(string("NULL != pAreaResults")));
						UI32 numberOfArea = pAreaResults->size ();
						if (0 == numberOfArea) {
							trace (TRACE_LEVEL_ERROR, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep area MO is not found");
						}else if (1 == numberOfArea) {
							pOSPFAreaLocalManagedObject = dynamic_cast<OSPFAreaLocalManagedObject *>((*pAreaResults)[0]);
							trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep Found areaMO");
						} else {
							trace (TRACE_LEVEL_ERROR, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep Two or more area MO objects exists");
						}
						if(1 != numberOfArea){
							pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pAreaResults);
							delete (pAreaResults);
							NSM_PLUG_DBG_ERR_S(string("Number of AreaMOs != 1? It should be an error"));
							/*
                            if(pNsmL3ReadyManagedObject)
                                pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pNsmL3ReadyManagedObject);
							*/
							return WAVE_MESSAGE_ERROR;
						}
					}
					pOSPFInterfaceLocalManagedObject->setArea(pOSPFAreaLocalManagedObject->getObjectId());
					break;
				}
                
			    case OSPF_INTF_HELLO:
				{
					pOSPFInterfaceLocalManagedObject->setHelloInterval(pOSPFInterfaceLocalMessage->getHelloInterval(), true);
					break;
				}
                
    			case OSPF_INTF_DEAD:
				{
					pOSPFInterfaceLocalManagedObject->setRouterDeadInterval(pOSPFInterfaceLocalMessage->getRouterDeadInterval(), true);
					break;
				}
                
    			case OSPF_INTF_RETRANSMIT:
				{
					pOSPFInterfaceLocalManagedObject->setRetransmitInterval(pOSPFInterfaceLocalMessage->getRetransmitInterval());
					break;
				}
                
    			case OSPF_INTF_TRANSDELAY:
				{
					pOSPFInterfaceLocalManagedObject->setTransDelay(pOSPFInterfaceLocalMessage->getTransDelay());
					break;
				}
                
    			case OSPF_INTF_COST:
				{
					pOSPFInterfaceLocalManagedObject->setCost(pOSPFInterfaceLocalMessage->getCost(),true);
					break;
				}
                
    			case OSPF_INTF_PRIORITY:
				{
					pOSPFInterfaceLocalManagedObject->setPriority(pOSPFInterfaceLocalMessage->getPriority());
					break;
				}
                
    			case OSPF_INTF_AUTH_CHANGE_WAIT_TIME:
				{
					pOSPFInterfaceLocalManagedObject->setAuthChangeWaitTime(pOSPFInterfaceLocalMessage->getAuthChangeWaitTime());
					break;
				}
                
    			case OSPF_INTF_AUTH_KEY:
				{
                    if(pOSPFInterfaceLocalMessage->getEncryptionType() != OSPF_INVALID_ENCR )
    					pOSPFInterfaceLocalManagedObject->setEncryptionType(pOSPFInterfaceLocalMessage->getEncryptionType(),true);
					pOSPFInterfaceLocalManagedObject->setAuthType(pOSPFInterfaceLocalMessage->getAuthType());

                    if( (pOSPFInterfaceLocalMessage->getAuthKey()).length() )
    					pOSPFInterfaceLocalManagedObject->setAuthKey(pOSPFInterfaceLocalMessage->getAuthKey(),true);
                     
                    /*Erase md5-authentication config if any*/ 
					pOSPFInterfaceLocalManagedObject->setMD5EncryptionType(OSPF_NO_ENCR, false);
					pOSPFInterfaceLocalManagedObject->setMD5KeyId(0, false);
					pOSPFInterfaceLocalManagedObject->setMD5Key(string(""), false);
					break;
				}
                
    			case OSPF_INTF_MD5_KEY_ACTV_WAIT_TIME:
				{
					pOSPFInterfaceLocalManagedObject->setMD5KeyActWaitTime(pOSPFInterfaceLocalMessage->getMD5KeyActWaitTime(), true);
					pOSPFInterfaceLocalManagedObject->setAuthType(pOSPFInterfaceLocalMessage->getAuthType());
					break;
				}
                
    			case OSPF_INTF_MD5_AUTH_KEY:
				{
                    if(pOSPFInterfaceLocalMessage->getMD5EncryptionType() != OSPF_INVALID_ENCR) 
    					pOSPFInterfaceLocalManagedObject->setMD5EncryptionType(pOSPFInterfaceLocalMessage->getMD5EncryptionType(),true);
                        
					pOSPFInterfaceLocalManagedObject->setAuthType(pOSPFInterfaceLocalMessage->getAuthType());

                    if( pOSPFInterfaceLocalMessage->getMD5KeyId() < 256 )
    					pOSPFInterfaceLocalManagedObject->setMD5KeyId(pOSPFInterfaceLocalMessage->getMD5KeyId(),true);
                        
                    if((pOSPFInterfaceLocalMessage->getMD5Key()).length())    
    					pOSPFInterfaceLocalManagedObject->setMD5Key(pOSPFInterfaceLocalMessage->getMD5Key(),true);
                    
                   /*Erase authentication config if any*/
					pOSPFInterfaceLocalManagedObject->setEncryptionType(OSPF_NO_ENCR, false);
					pOSPFInterfaceLocalManagedObject->setAuthKey(string(""),false);
                   
					break;
				}
				// ip ospf database-filter all-out
    			case OSPF_INTF_FILTER_ALL:
				{
					trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep ip ospf database-filter all-out");
                    pOSPFInterfaceLocalManagedObject->clearDatabaseFilterOptions();
					pOSPFInterfaceLocalManagedObject->setFilterAll(true);
					break;
				}
				
				// ip ospf database-filter all-external *
    			case OSPF_INTF_FILTER_EXT:
				{
					trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep ip ospf database-filter all-external *");
                    pOSPFInterfaceLocalManagedObject->clearDatabaseFilterOptions();
					pOSPFInterfaceLocalManagedObject->setFilterAllExt(pOSPFInterfaceLocalMessage->getFilterOption());
					break;
				}
				
				// ip ospf database-filter all-summary-external *
    			case OSPF_INTF_FILTER_SUMM_EXT:
				{
					trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep ip ospf database-filter all-summary-external *");
                    pOSPFInterfaceLocalManagedObject->clearDatabaseFilterOptions();
					pOSPFInterfaceLocalManagedObject->setFilterAllSumExt(pOSPFInterfaceLocalMessage->getFilterOption());
					break;
				}
                
    			case OSPF_INTF_MTU_IGNORE:
				{
					pOSPFInterfaceLocalManagedObject->setMtuIgnore(pOSPFInterfaceLocalMessage->getmtuIgnore());
					break;
				}
                
    			case OSPF_INTF_NETWORK:
				{
					pOSPFInterfaceLocalManagedObject->setNetworkType(pOSPFInterfaceLocalMessage->getNetworkType());
					break;
				}
				
				case OSPF_INTF_ACTIVE:
				{
					pOSPFInterfaceLocalManagedObject->setActive(pOSPFInterfaceLocalMessage->getActive());
					pOSPFInterfaceLocalManagedObject->setPassive(false);
					break;
				}
                
    			case OSPF_INTF_PASSIVE:
				{
					pOSPFInterfaceLocalManagedObject->setPassive(pOSPFInterfaceLocalMessage->getPassive());
					pOSPFInterfaceLocalManagedObject->setActive(false);
					break;
				}
                    
    			default:
				{
					trace (TRACE_LEVEL_DEVEL, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep default case: Invalid command code");
					break;
				}
			}

			if((pOSPFInterfaceLocalMessage->getOpCode() == OSPF_SET_ELEM) || (!createFlag)){
				trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep: update OSPFInterfaceLocalManagedObject");
				updateWaveManagedObject(pOSPFInterfaceLocalManagedObject);
			}
		}
		else
		{/*This section of the code caters no form of ospf interface commands - kunjan*/
			NSM_PLUG_INFO_S(string("OpCode == OSPF_REMOVE "));
			switch(pOSPFInterfaceLocalMessage->getCmdCode())
			{
			case OSPF_INTF_AREA:
				{
					ObjectId areaId = pOSPFInterfaceLocalManagedObject->getArea();
					if(areaId != ObjectId::NullObjectId)
					{
						//Get Interface Object 
						pNsmL3ReadyManagedObject = dynamic_cast<NsmL3ReadyManagedObject *>
						(pPrismSynchronousLinearSequencerContext->getCache()->getWaveManagedObject(NsmL3ReadyManagedObject::getClassName()));
						OSPF_PLUG_ASSERT_PTR(pNsmL3ReadyManagedObject);
						trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep: Del area from Intf: NSML3ReadyMO found");

						pNsmL3ReadyManagedObject->setOSPFIntfConfig(NULL);
						trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep: Deassociated OSPF intfMO from L3readyMO");
						//pOSPFInterfaceLocalManagedObject->setArea(ObjectId::NullObjectId);
						updateWaveManagedObject(pNsmL3ReadyManagedObject);
						//delete(pOSPFInterfaceLocalManagedObject);
						trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep: Deleted OSPF Intf MO");
					}
					break;
				}
			case OSPF_INTF_HELLO:
				{
					pOSPFInterfaceLocalManagedObject->setHelloInterval(OSPF_DEFAULT_HELLO_INTERVAL, false);
					break;
				}
			case OSPF_INTF_DEAD:
				{
					pOSPFInterfaceLocalManagedObject->setRouterDeadInterval(OSPF_DEFAULT_ROUTER_DEAD_INTERVAL, false);
					break;
				}
			case OSPF_INTF_RETRANSMIT:
				{
					pOSPFInterfaceLocalManagedObject->setRetransmitInterval(OSPF_DEFAULT_RETRANSMIT_INTERVAL);
					break;
				}
			case OSPF_INTF_TRANSDELAY:
				{
					pOSPFInterfaceLocalManagedObject->setTransDelay(OSPF_DEFAULT_TRANSMIT_DELAY);
					break;
				}
			case OSPF_INTF_COST:
				{
					pOSPFInterfaceLocalManagedObject->setCost(OSPF_DEFAULT_COST,false);
					break;
				}
			case OSPF_INTF_PRIORITY:
				{
					pOSPFInterfaceLocalManagedObject->setPriority(OSPF_DEFAULT_PRIORITY);
					break;
				}
			case OSPF_INTF_AUTH_CHANGE_WAIT_TIME:
				{
					pOSPFInterfaceLocalManagedObject->setAuthChangeWaitTime(OSPF_DEFAULT_AUTH_CHANGE_WAIT_TIME);
					break;
				}
			case OSPF_INTF_AUTH_KEY:
				{
					pOSPFInterfaceLocalManagedObject->setEncryptionType(OSPF_NO_ENCR, false);
					pOSPFInterfaceLocalManagedObject->setAuthType(OSPF_NONE);
					pOSPFInterfaceLocalManagedObject->setAuthKey(string(""),false);
					break;
				}
			case OSPF_INTF_MD5_KEY_ACTV_WAIT_TIME:
				{
					pOSPFInterfaceLocalManagedObject->setMD5KeyActWaitTime(0, false);
					break;
				}
			case OSPF_INTF_MD5_AUTH_KEY:
				{
					pOSPFInterfaceLocalManagedObject->setMD5EncryptionType(OSPF_NO_ENCR, false);
					pOSPFInterfaceLocalManagedObject->setAuthType(OSPF_NONE);
					pOSPFInterfaceLocalManagedObject->setMD5KeyId(0, false);
					pOSPFInterfaceLocalManagedObject->setMD5Key(string(""), false);
					break;
				}
			// no ip ospf database-filter *
			case OSPF_INTF_FILTER_ALL:
			case OSPF_INTF_FILTER_EXT:
			case OSPF_INTF_FILTER_SUMM_EXT:
				{
					trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep no ip ospf database-filter:");
                    pOSPFInterfaceLocalManagedObject->clearDatabaseFilterOptions();    
					break;
				}
			case OSPF_INTF_MTU_IGNORE:
				{
					pOSPFInterfaceLocalManagedObject->setMtuIgnore(false);
					break;
				}
			case OSPF_INTF_NETWORK:
				{
					pOSPFInterfaceLocalManagedObject->setNetworkType(OSPF_INTF_NETWORK_NONE);
					break;
				}
			case OSPF_INTF_ACTIVE:
				{
					trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::Inside active setting to false >>>>>>>>");
					pOSPFInterfaceLocalManagedObject->setActive(false);
					break;
				}
			case OSPF_INTF_PASSIVE:
				{
					pOSPFInterfaceLocalManagedObject->setPassive(false);
					break;
				}
			case OSPF_INTF_DEL:
				{
					NSM_PLUG_INFO_S(string("CmdCode == OSPF_INTF_DEL "));
					//When a 'no router ospf vrf <vrf_name>' is issued only those interfaces belonging to the <vrf_name> should
					//delete the OSPF config

					//Keep a list of L3ReadyMOs to be updated for the ospf interface config deletion
					vector<NsmL3ReadyManagedObject*> l3ReadyMOsTobeUpdated;

					//Use the dcm framework API to get the interfaces belonging to the default vrf, if the vrf name is 'default-vrf' using the IS NULL filter
					//For other VRFs, we retrieve the interfaces using the vrfobjectid

					string defaultVrfName = DEFAULT_VRF_NAME;
					int defVrfNameSize = defaultVrfName.length();
					bool isDefaultVrf = false;
					VrfLocalManagedObject *vrfLocalMo = NULL;
					ObjectId vrfObjectId = ObjectId::NullObjectId;
					if(strncmp(vrfName.c_str(), defaultVrfName.c_str(), defVrfNameSize)) {
						NSM_PLUG_INFO_S(string("Need to query the interfaces belonging to the non-default VRF ")+vrfName);
						//Step 1 : Get the VRFLocalMO and its ObjectId
						NSM_PLUG_INFO_S(string("Retrieve the VRFLocalMO and its objectId"));
						WaveManagedObjectSynchronousQueryContext vrfSyncQueryCtxt(VrfLocalManagedObject::getClassName());
						vrfSyncQueryCtxt.addAndAttribute(new AttributeString(&vrfName, "vrfname"));
						pVrfResults = querySynchronouslyLocalManagedObjectsForLocationId (locId,&vrfSyncQueryCtxt);
						pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pVrfResults);
						if (NULL != pVrfResults)
						{
							UI32 numberOfVrfs = pVrfResults->size ();
							if(numberOfVrfs > 0) {
								NSM_PLUG_INFO_S(string("numberOfVrfs =")+numberOfVrfs);
								vrfLocalMo = dynamic_cast<VrfLocalManagedObject *>((*pVrfResults)[0]);
							}
						}
						if(vrfLocalMo) {
							vrfObjectId = vrfLocalMo->getObjectId();
							NSM_PLUG_INFO_S(string("ObjectId of VRF MO :")+vrfObjectId.toString());
						}
						else {
							NSM_PLUG_INFO_S(string("Failed to retrieve VRF MO. The configuration might fail"));
						}
					}
					else {
						NSM_PLUG_INFO_S(string("Need to query the interfaces belonging to the default VRF"));
						isDefaultVrf = true;
					}



					UI32 resultCount = 0;
					UI32 index = 0;

					//Step 2: Get the PhyIntfLocalManagedObjects belonging to this VRF and get the L3ReadyMO and add to the vector
					NSM_PLUG_INFO_S(string("Retrieve the physical interfaces belonging to the VRF ")+vrfName);
					WaveManagedObjectSynchronousQueryContext phyIntfsyncQueryCtxt
						(PhyIntfLocalManagedObject::getClassName());
					if(true == isDefaultVrf) {
						phyIntfsyncQueryCtxt.addAndAttribute (new AttributeObjectIdAssociation (&vrfObjectId, "vrfid", VrfLocalManagedObject::getClassName()), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IS_NULL);
					}
					else {
						phyIntfsyncQueryCtxt.addAndAttribute(new AttributeObjectId(&vrfObjectId,"vrfid"));
					}
					vector<WaveManagedObject * >* pPhyIntfResults = querySynchronouslyLocalManagedObjectsForLocationId(locId, &phyIntfsyncQueryCtxt);
					resultCount = pPhyIntfResults->size();
					NSM_PLUG_INFO_S(string("Number of physical interfaces belonging to the VRF:")+resultCount);
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pPhyIntfResults);
		            for (index = 0; index < resultCount; index++) {
		                PhyIntfLocalManagedObject *phyIntfMO = dynamic_cast <PhyIntfLocalManagedObject*>((*pPhyIntfResults)[index]);
		                NsmL3ReadyManagedObject *l3ReadyMo = phyIntfMO->getL3ReadyMO();
		                //For interfaces with switchport configuration, the L3ReadyMO will be null
		                if(l3ReadyMo) {
		                	l3ReadyMOsTobeUpdated.push_back(l3ReadyMo);
		                	NSM_PLUG_INFO_S(string("Retrieved the L3ReadyMO for the interface :")+phyIntfMO->getIfName());
		                }
		                else {
		                	NSM_PLUG_INFO_S(string("NO L3ReadyMO for the interface :")+phyIntfMO->getIfName());
		                }
		            }

		            //Step 3: Get the NsmIntfSviManagedObjects belonging to this VRF and get the L3ReadyMO and add to the vector
		            NSM_PLUG_INFO_S(string("Retrieve the SVI interfaces belonging to the VRF ")+vrfName);
					WaveManagedObjectSynchronousQueryContext sviIntfsyncQueryCtxt
						(NsmIntfSviManagedObject::getClassName());
					if(true == isDefaultVrf) {
						sviIntfsyncQueryCtxt.addAndAttribute (new AttributeObjectIdAssociation (&vrfObjectId, "vrfid", VrfLocalManagedObject::getClassName()), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IS_NULL);
					}
					else {
						sviIntfsyncQueryCtxt.addAndAttribute(new AttributeObjectId(&vrfObjectId,"vrfid"));
					}
					vector<WaveManagedObject * >* pSviIntfResults = querySynchronouslyLocalManagedObjectsForLocationId(locId, &sviIntfsyncQueryCtxt);
					resultCount = pSviIntfResults->size();
					NSM_PLUG_INFO_S(string("Number of SVI interfaces belonging to the VRF:")+resultCount);
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pSviIntfResults);
		            for (index = 0; index < resultCount; index++) {
		            	NsmIntfSviManagedObject *sviIntfMO = dynamic_cast <NsmIntfSviManagedObject*>((*pSviIntfResults)[index]);
		                NsmL3ReadyManagedObject *l3ReadyMo = sviIntfMO->getL3ReadyMO();
		                if(l3ReadyMo) {
		                	l3ReadyMOsTobeUpdated.push_back(l3ReadyMo);
		                	NSM_PLUG_INFO_S(string("Retrieved the L3ReadyMO for the interface : Ve ")+sviIntfMO->getIfId());
		                }
		                else {
		                	NSM_PLUG_INFO_S(string("NO L3ReadyMO for the interface :")+sviIntfMO->getIfId());
		                }
		            }

		            //Step 4: Get the NsmIntfLoopbackManagedObjects belonging to this VRF and get the L3ReadyMO and add to the vector
		            NSM_PLUG_INFO_S(string("Retrieve the Loopback interfaces belonging to the VRF ")+vrfName);
					WaveManagedObjectSynchronousQueryContext loopbackIntfsyncQueryCtxt
						(NsmIntfLoopbackManagedObject::getClassName());
					if(true == isDefaultVrf) {
						loopbackIntfsyncQueryCtxt.addAndAttribute (new AttributeObjectIdAssociation (&vrfObjectId, "vrfid", VrfLocalManagedObject::getClassName()), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IS_NULL);
					}
					else {
						loopbackIntfsyncQueryCtxt.addAndAttribute(new AttributeObjectId(&vrfObjectId,"vrfid"));
					}
					vector<WaveManagedObject * >* pLoopbackIntfResults = querySynchronouslyLocalManagedObjectsForLocationId(locId, &loopbackIntfsyncQueryCtxt);
					resultCount = pLoopbackIntfResults->size();
					NSM_PLUG_INFO_S(string("Number of Loopback interfaces belonging to the VRF:")+resultCount);
					pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pLoopbackIntfResults);
		            for (index = 0; index < resultCount; index++) {
		            	NsmIntfLoopbackManagedObject *loopbackIntfMO = dynamic_cast <NsmIntfLoopbackManagedObject*>((*pLoopbackIntfResults)[index]);
		                NsmL3ReadyManagedObject *l3ReadyMo = loopbackIntfMO->getL3ReadyMO();
		                if(l3ReadyMo) {
		                	l3ReadyMOsTobeUpdated.push_back(l3ReadyMo);
		                	NSM_PLUG_INFO_S(string("Retrieved the L3ReadyMO for the interface :")+loopbackIntfMO->getIfName());
		                }
		                else {
		                	NSM_PLUG_INFO_S(string("NO L3ReadyMO for the interface :")+loopbackIntfMO->getIfName());
		                }
		            }

		            //Step 5: Unset the OSPF config on each of the selected NsmL3ReadyManagedObjects
		            NSM_PLUG_INFO_S(string("Unset the OSPF config from L3ReadyMOs. NUMBER OF L3ReadyMOs to be updated:")+l3ReadyMOsTobeUpdated.size());
		            for (index = 0; index < l3ReadyMOsTobeUpdated.size(); index++) {
		            	NsmL3ReadyManagedObject *pNsmL3ReadyMO = l3ReadyMOsTobeUpdated[index];

						pNsmL3ReadyMO->setOSPFIntfConfig(NULL);
						updateWaveManagedObject(pNsmL3ReadyMO);

						NSM_PLUG_INFO_S(string("Updated the L3ReadyMO for the interface :")+pNsmL3ReadyMO->getInterfaceName());
		            }
#if 0
		            NSM_PLUG_DBG_S(string("Unset the OSPF interface configs"));
		            /*Loop through L3ReadyMO and delete all the OSPFInterfaceMO's*/
					UI32 ospfInterfaceObjectIdClassId = 0;
					NsmL3ReadyManagedObject *pNsmL3ReadyMO = NULL;
					//OSPFInterfaceLocalManagedObject *pOSPFInterfaceLocalMO = NULL;
					WaveManagedObjectSynchronousQueryContext ospfIntfsyncQueryCtxt
						(OSPFInterfaceLocalManagedObject::getClassName());
					vector<WaveManagedObject * >* pOspfIntfResults = querySynchronouslyLocalManagedObjectsForLocationId(locId, &ospfIntfsyncQueryCtxt);
                                        trace(TRACE_LEVEL_INFO, string("num of ospf Interface objects")+pOspfIntfResults->size());
					if(pOspfIntfResults->size())
					{
						ospfInterfaceObjectIdClassId = (((*pOspfIntfResults)[0])->getObjectId()).getClassId();
					}

					WaveManagedObjectSynchronousQueryContext L3ReadysyncQueryCtxt(NsmL3ReadyManagedObject::getClassName());
					L3ReadysyncQueryCtxt.addAndAttribute(new AttributeUI32(ospfInterfaceObjectIdClassId, "ospfintfconfigclassid") );
					vector<WaveManagedObject * >* pL3ReadyResults = querySynchronouslyLocalManagedObjectsForLocationId(locId, &L3ReadysyncQueryCtxt);

                                        trace(TRACE_LEVEL_INFO, string("num of L3 Ready MO objects ")+pL3ReadyResults->size());
					if(pL3ReadyResults->size())
					{
						UI32 numberOfResults = pL3ReadyResults->size ();
						while(numberOfResults)
						{
                                                        trace(TRACE_LEVEL_INFO, string("while-current number of results")+numberOfResults);
							pNsmL3ReadyMO = dynamic_cast<NsmL3ReadyManagedObject *>((*pL3ReadyResults)[numberOfResults-1]);
							pNsmL3ReadyMO->setOSPFIntfConfig(NULL);
							updateWaveManagedObject(pNsmL3ReadyMO);
							numberOfResults--;
						}
						if(pPrismSynchronousLinearSequencerContext)
							pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pL3ReadyResults);
					}/*Now here the L3ReadyMo is unlinked from OSPFIntfMO. So it is safe now to go ahead and delete
					   OSPFIntfMO*/
#endif
					break;
				}
			default:
				{
					trace (TRACE_LEVEL_DEVEL, "OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep default case: Invalid command code");
					break;
				}
			}
			if(pOSPFInterfaceLocalMessage->getCmdCode() != OSPF_INTF_DEL && pOSPFInterfaceLocalMessage->getCmdCode() != OSPF_INTF_AREA) {
				NSM_PLUG_INFO_S(string("CmdCode != OSPF_INTF_DEL or OSPF_INTF_AREA. Update the MO:"));
				updateWaveManagedObject(pOSPFInterfaceLocalManagedObject);
			}
		}

        if (pAreaResults !=NULL)
        {
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pAreaResults);
            delete (pAreaResults);
        }
		return pOSPFInterfaceLocalMessage->getCompletionStatus ();
	}

    OSPFInterfaceLocalManagedObject *OSPFInterfaceLocalConfWorker::OSPFGetOSPFInterfaceLocalManagedObject(DceSynchronousLinearSeqContext  *pPrismSynchronousLinearSequencerContext)
    {
        trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::OSPFGetOSPFInterfaceLocalManagedObject Entering...");

        OSPFInterfaceLocalMessage *pOSPFInterfaceLocalMessage =
            dynamic_cast<OSPFInterfaceLocalMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

        OSPFInterfaceLocalManagedObject *pOSPFInterfaceLocalManagedObject = NULL;

        //Get Interface Object
        NsmL3ReadyManagedObject *pNsmL3ReadyManagedObject =
            ospfGetL3RdyMo(pPrismSynchronousLinearSequencerContext, pOSPFInterfaceLocalMessage);
        OSPF_PLUG_ASSERT_PTR(pNsmL3ReadyManagedObject);

        pOSPFInterfaceLocalManagedObject =
            ((WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject>
              (pNsmL3ReadyManagedObject->getOSPFIntfConfig())).operator->());

        if(pNsmL3ReadyManagedObject)
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pNsmL3ReadyManagedObject);

        trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalConfWorker::OSPFGetOSPFInterfaceLocalManagedObject Exiting");
        return pOSPFInterfaceLocalManagedObject;      

    }

    void  OSPFInterfaceLocalConfWorker::NsmLocalOSPFInterfaceConfigMessageHandler( OSPFInterfaceLocalMessage *pOSPFInterfaceLocalMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFInterfaceLocalConfWorker::ospfInterfaceConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFInterfaceLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFInterfaceLocalConfWorker::ospfInterfaceConfigMOStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFInterfaceLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFInterfaceLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFInterfaceLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pOSPFInterfaceLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pDceSynchronousLinearSeqContext->execute();
    }


    NsmL3ReadyManagedObject *OSPFInterfaceLocalConfWorker::ospfGetL3RdyMo(DceSynchronousLinearSeqContext  *pSeqCtx,
                                                                OSPFInterfaceLocalMessage *pMsg)
    {
        string ifName;
        UI8 ifType;
        NsmL3ReadyManagedObject *pL3RdyMo = NULL;
        SI32 mapId = DcmToolKit::getLocalMappedId();

        OSPF_PLUG_ASSERT_PTR(pMsg);

        ifName = pMsg->getintfName();
        ifType = pMsg->getintfType();

        trace(TRACE_LEVEL_INFO, string("OSPFInterfaceLocalConfWorker::ospfGetL3RdyMo:") +
                string("Searching for interface ") + ifType +
                string(" ") + ifName);
        switch(ifType) {
            case PHY_INT:
            case IF_TYPE_GI:
            case IF_TYPE_TE:
            case IF_TYPE_FO:
            {
            	//pL3RdyMo = NsmL3Util::PhyIfNameGetL3ReadyMO(ifName, pSeqCtx, true);
            	PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *> (pSeqCtx->
            			getCache()->getWaveManagedObject(PhyIntfLocalManagedObject::getClassName()));

                pL3RdyMo = NsmL3Util::PhyIfGetL3ReadyMO(pPhyIntfLocalManagedObject, pSeqCtx, true);
                break;
            }
            case VLAN_INT:
                pL3RdyMo = NsmL3Util::VlanIdGetL3ReadyMO(toUI32(ifName), mapId, pSeqCtx, false);
                break;
            case PO_INT:
                pL3RdyMo = NsmL3Util::PoIdGetL3ReadyMO(toUI32(ifName), mapId, pSeqCtx, false);
                break;
            case LOOPBACK_INT:
                pL3RdyMo = NsmL3Util::LoopbackIdGetL3ReadyMO(toUI32(ifName), mapId, pSeqCtx);
                break;
            default:
                trace(TRACE_LEVEL_INFO,
                        string("OSPFInterfaceLocalConfWorker::ospfGetL3RdyMo:") +
                        string("Unrecognized interface type (") + ifType +
                        string(")"));
                break;
        }

        return(pL3RdyMo);
    }

    void OSPFInterfaceLocalConfWorker::ospfUpdateLocalMessage(OSPFInterfaceLocalMessage *pOSPFInterfaceLocalMessage, OSPFInterfaceConfigMessage *pOSPFInterfaceConfigMessage)
    {
        trace (TRACE_LEVEL_DEBUG, string("OSPFInterfaceLocalConfWorker::ospfUpdateLocalMessage : Entering") );
        UI32 size = 0;
        void *data;
        ospf_intf_config_out_msg_t *out_msg;

        pOSPFInterfaceConfigMessage->getOutputBuffer(OSPFINTERFACECONFIG, size, data);

        out_msg = (ospf_intf_config_out_msg_t*) data;
        if (out_msg) {
            //Store the result
            trace (TRACE_LEVEL_DEBUG, string("OSPFInterfaceLocalConfWorker::ospfUpdateLocalMessage: RET: ") + out_msg->encrypted_key );
            if (pOSPFInterfaceLocalMessage->getOpCode() != OSPF_REMOVE)
            {
                switch(pOSPFInterfaceLocalMessage->getCmdCode())
                {
                    case OSPF_INTF_AUTH_KEY:
                        {
                            if( (pOSPFInterfaceConfigMessage->getEncryptionType()) == OSPF_GLOBAL_ENCR)
                            {
                                trace (TRACE_LEVEL_DEBUG, string("OSPFInterfaceLocalConfWorker::ospfUpdateLocalMessage: Updating AuthKey "));
                                if( OSPF_SIMPLE_ENCR == (OSPFEncrypType)out_msg->auth_type )
                                {
                                    pOSPFInterfaceLocalMessage->setEncryptionType((OSPFEncrypType)out_msg->auth_type);
                                    pOSPFInterfaceLocalMessage->setAuthKey(string(out_msg->encrypted_key));
                                }    
                            }
                            break;
                        }    

                    case OSPF_INTF_MD5_AUTH_KEY:
                        {
                            if( (pOSPFInterfaceConfigMessage->getMD5EncryptionType()) == OSPF_GLOBAL_ENCR)
                            {
                                trace (TRACE_LEVEL_DEBUG, string("OSPFInterfaceLocalConfWorker::ospfUpdateLocalMessage: Updating MD5Key "));
                                if( OSPF_SIMPLE_ENCR == (OSPFEncrypType)out_msg->auth_type )
                                {
                                    pOSPFInterfaceLocalMessage->setMD5EncryptionType((OSPFEncrypType)out_msg->auth_type);
                                    pOSPFInterfaceLocalMessage->setMD5Key(string(out_msg->encrypted_key));
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
}
