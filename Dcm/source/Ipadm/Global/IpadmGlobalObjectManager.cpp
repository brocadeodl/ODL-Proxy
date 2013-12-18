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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                             *
 *   Author : Chintan Shah		                                      *
 ***************************************************************************/

#include "DcmShell/DcmShell.h"
#include "DcmCore/DcmToolKit.h"
#include "Shell/ShellDebug.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Ipadm/Global/IpadmGlobalObjectManager.h"
#include "Ipadm/Global/IpadmGlobalManagedObject.h"
#include "Ipadm/Local/IpadmLocalManagedObject.h"
#include "Ipadm/Local/IpadmTypes.h"
#include "Framework/Types/IpVxAddress.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "DcmResourceIds.h"
#include "DcmResourceIdEnums.h"
//#include "ipadm.h"
#include "vcs.h"
namespace DcmNs
{

IpadmGlobalObjectManager::IpadmGlobalObjectManager ()
    : WaveObjectManager (getClassName ())
{
    PrismFrameworkObjectManager::excludeServiceForClusterValidationPhase (getServiceId ());

    IpadmGlobalNSManagedObject IpadmNameSrv (this);
    IpadmNameSrv.setupOrm ();
    addManagedClass (IpadmGlobalNSManagedObject::getClassName ());

    IpadmGlobalDNManagedObject IpadmDomainName (this);
    IpadmDomainName.setupOrm ();
    addManagedClass (IpadmGlobalDNManagedObject::getClassName ());


    // register to receive IPADMDNSCONFIG msg
    addOperationMap (IPADMDNSCONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmGlobalObjectManager::IpadmClusterConfigMsgHandler));

}

IpadmGlobalObjectManager::~IpadmGlobalObjectManager ()
{

}

string IpadmGlobalObjectManager::getClassName ()
{
    return ("IpadmGlobalObjectManager");
}

IpadmGlobalObjectManager *IpadmGlobalObjectManager::getInstance ()
{
    static IpadmGlobalObjectManager *pIpadmGlobalObjectManager = NULL;

    if (NULL == pIpadmGlobalObjectManager)
    {
        pIpadmGlobalObjectManager = new IpadmGlobalObjectManager ();
        WaveNs::prismAssert (NULL != pIpadmGlobalObjectManager, __FILE__, __LINE__);
    }

    return (pIpadmGlobalObjectManager);
}

PrismServiceId IpadmGlobalObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void IpadmGlobalObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
    vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, string ("IpadmGlobalObjectManager::failover failover reason") + FrameworkToolKit::localize(failoverReason));

    if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == failoverReason)
    {
        UI32 noOfFailedLocations = failedLocationIds.size ();


        for (UI32 i = 0; i < noOfFailedLocations; i++)
        {
            vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], IpadmLocalManagedObject::getClassName ());

            trace (TRACE_LEVEL_INFO, string ("IpadmGlobalObjectManager::failover for Location Id ") + failedLocationIds[i]);

            startTransaction ();

            UI32 sizeOfManagedObjects = pWaveManagedObjects->size ();

            for (UI32 j = 0; j < sizeOfManagedObjects; j++)
            {
                delete (*pWaveManagedObjects)[j];
            }   

            status = commitTransaction ();

            if (FRAMEWORK_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "IpadmGlobalObjectManager::failover can not commit local managed object delete to database ");
                prismAssert (false, __FILE__, __LINE__);
            }

            pWaveManagedObjects->clear ();

            delete pWaveManagedObjects;
        }
    }
    pFailoverAsynchronousContext->setCompletionStatus (status);
    pFailoverAsynchronousContext->callback ();
}

vector<WaveManagedObject *> *IpadmGlobalObjectManager::DoSynchronousQuery (const string className)
{
        vector<WaveManagedObject *> *pResults = querySynchronously (className);
        prismAssert (NULL != pResults, __FILE__, __LINE__);
        return pResults;
}

WaveManagedObject *IpadmGlobalObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;
    if (IpadmGlobalNSManagedObject::getClassName() == managedClassName) {
        pWaveManagedObject = new IpadmGlobalNSManagedObject (this);

    } else if (IpadmGlobalDNManagedObject::getClassName() == managedClassName) {
        pWaveManagedObject = new IpadmGlobalDNManagedObject (this);
    } else {
        trace (TRACE_LEVEL_ERROR, "IpadmGlobalObjectManager::createManagedObjectInstance : unsupported managed class name : " + managedClassName);
    }

    return pWaveManagedObject;
}

void IpadmGlobalObjectManager::install(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    /****************************************************************************
     * Create managed object and commit to database
     ***************************************************************************/
    trace (TRACE_LEVEL_INFO, "IpadmGlobalObjectManager::install: Installing IPADM GMO DB");

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    WaveNs::trace (TRACE_LEVEL_INFO, string ("IpadmGlobalObjectManager::install:boot=") + bootReason);

    if ((WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason) ||
        (WAVE_BOOT_FIRST_TIME_BOOT == bootReason)) {

#if 0   /* as of now nothing to do here */
        startTransaction ();

        status = commitTransaction ();
#endif
        status = FRAMEWORK_SUCCESS;

        if (FRAMEWORK_SUCCESS == status) {
            trace (TRACE_LEVEL_INFO, "IpadmGlobalObjectManager::install: Installing IPADM DB complete");
            status = WAVE_MESSAGE_SUCCESS;

        } else {
            trace (TRACE_LEVEL_FATAL, "IpadmGlobalObjectManager::install: Failed to install IPADM DB");
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    if (WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason) {
        trace(TRACE_LEVEL_DEBUG, "IpadmGlobalObjectManager::install: secondary configure boot");
        status = WAVE_MESSAGE_SUCCESS;
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

PrismMessage *IpadmGlobalObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

   tracePrintf(TRACE_LEVEL_DEBUG,
                  "IPADMGOM::createMessageInstance opcode=%d", operationCode);

    switch (operationCode)
    {
        case IPADMDNSCONFIG:
            trace (TRACE_LEVEL_DEBUG, "IPADMObjectManager::createMessageInstance:: IPADMDNSCONFIG");
            pPrismMessage = new IpadmDnsConfigMessage ();
            break;
        default :
            tracePrintf(TRACE_LEVEL_INFO,
                  "IPADMGObjectManager::createMessageInstance Invalid opcode=%d", operationCode);
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void IpadmGlobalObjectManager::IpadmClusterConfigMsgHandler (IpadmDnsConfigMessage *pIpadmDnsConfigMessage)
{   

    trace (TRACE_LEVEL_DEBUG, "IpadmGlobalObjectManager::IpadmClusterConfigMsgHandler : Entering ...");
    
     PrismLinearSequencerStep sequencerSteps[] =
    {

        reinterpret_cast<PrismLinearSequencerStep> (&IpadmGlobalObjectManager::cfgMessageHandler),
        reinterpret_cast<PrismLinearSequencerStep> (&IpadmGlobalObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&IpadmGlobalObjectManager::prismLinearSequencerFailedStep),
    };
    
    IpadmMessagingContext *pIpadmMessagingContext = new IpadmMessagingContext (pIpadmDnsConfigMessage, this,
                            sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pIpadmMessagingContext->setPMessage (pIpadmDnsConfigMessage);

    pIpadmMessagingContext->start ();
}

void IpadmGlobalObjectManager::cfgMessageHandler(IpadmMessagingContext *pIpadmMessagingContext)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, "IpadmGlobalObjectManager::cfgMessageHandler: Entering");
    ResourceId status = WAVE_MESSAGE_ERROR;
    ResourceId messageStatus = WAVE_MESSAGE_ERROR;
    IpadmLocalDnsCfgMessage *pMessage = NULL;
    UI32 i = 0, j = 0, NSindex = 0;
    int found = 0;
    bool addOp = false, delOp = false, modOp = false;
    IpVxAddress ns_ip;
    string domainName;

    IpadmGlobalNSManagedObject *pIpadmGlobalNSManagedObject = NULL;
    IpadmGlobalDNManagedObject *pIpadmGlobalDNManagedObject = NULL;
    IpadmGlobalDNManagedObject *pIpadmGlobalDNMagObj = NULL;

    pIpadmMessagingContext->setNumberOfFailures (0);
    ++(*pIpadmMessagingContext);

    IpadmDnsConfigMessage *pIpadmDnsConfigMessage = dynamic_cast<IpadmDnsConfigMessage *> (pIpadmMessagingContext->getPMessage ());
    prismAssert (pIpadmDnsConfigMessage, __FILE__, __LINE__);

    UI32 xmlTag = pIpadmDnsConfigMessage->getWyserUserTag();

    if (xmlTag == ip_administration_name_server) {
        ns_ip = pIpadmDnsConfigMessage->getNameSrvIp();
    } else if (xmlTag == ip_administration_domain_name) {
        domainName = pIpadmDnsConfigMessage->getDomainName();
    }

    vector<WaveManagedObject *> *pResults = NULL;
    if (xmlTag == ip_administration_name_server)
        pResults = querySynchronously (IpadmGlobalNSManagedObject::getClassName());
    else if (xmlTag == ip_administration_domain_name)
        pResults = querySynchronously (IpadmGlobalDNManagedObject::getClassName());
    prismAssert (pResults, __FILE__, __LINE__);

    vector<WaveManagedObject *> *pRemove = NULL;
    vector<WaveManagedObject *> *pMod = NULL;
    WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
    if (xmlTag == ip_administration_name_server)
        synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(IpadmGlobalNSManagedObject::getClassName());
    else if (xmlTag == ip_administration_domain_name)
        synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(IpadmGlobalDNManagedObject::getClassName());

    for (j = 0; j < pResults->size (); j++) {
      if (xmlTag == ip_administration_name_server) {
        pIpadmGlobalNSManagedObject = dynamic_cast<IpadmGlobalNSManagedObject *> ((*pResults)[j]);
        if (ns_ip == pIpadmGlobalNSManagedObject->getNameSrvIp())
                found = 1;
        else
                NSindex = pIpadmGlobalNSManagedObject->getIndex();
      } else if (xmlTag == ip_administration_domain_name) {
        pIpadmGlobalDNManagedObject = dynamic_cast<IpadmGlobalDNManagedObject *> ((*pResults)[j]);
	if (pIpadmGlobalDNManagedObject != NULL) {
                found = 1;
	}
      }
    }

    tracePrintf(TRACE_LEVEL_INFO, "IpadmGlobalObjectManager::cfgMessageHandler sz=%d found=%d",
        pResults->size(), found);

    if ((strcmp((pIpadmDnsConfigMessage->getOpr()).c_str(),"delete") == 0) && found) {
      if (xmlTag == ip_administration_name_server) {
        tracePrintf(TRACE_LEVEL_INFO, "IpadmGlobalObjectManager::cfgMessageHandler delete_ns=%s",
        	(ns_ip).toString().c_str());
        delOp = true;
        synchronousQueryContext->addAndAttribute (new AttributeIpVxAddress (ns_ip, "ipaddress"));
      } else if (xmlTag == ip_administration_domain_name) {
        tracePrintf(TRACE_LEVEL_INFO, "IpadmGlobalObjectManager::cfgMessageHandler delete_dn=%s",
		domainName.c_str());
        delOp = true;
      }
      pRemove = querySynchronously (synchronousQueryContext);
      if ((NULL == pRemove) || (pRemove->size() == 0))
      {
            if (xmlTag == ip_administration_name_server)
                trace (TRACE_LEVEL_FATAL, "name server ip not found");
            else if (xmlTag == ip_administration_domain_name)
                trace (TRACE_LEVEL_FATAL, "domain name not found");
            if (pRemove != NULL)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pRemove);
            if (pResults)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            delete synchronousQueryContext;
            --(*pIpadmMessagingContext);
            pIpadmMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
            return;
      }
    } else if ((strcmp((pIpadmDnsConfigMessage->getOpr()).c_str(),"add") == 0) && !found) {
        if (xmlTag == ip_administration_name_server)
           tracePrintf(TRACE_LEVEL_INFO, "IpadmGlobalObjectManager::cfgMessageHandler add_ns=%s",
                ns_ip.toString().c_str());
        else if (xmlTag == ip_administration_domain_name)
           tracePrintf(TRACE_LEVEL_INFO, "IpadmGlobalObjectManager::cfgMessageHandler add_dn=%s",
		domainName.c_str());
        addOp = true;
    } else if ((strcmp((pIpadmDnsConfigMessage->getOpr()).c_str(),"update") == 0)) {
	if ((xmlTag == ip_administration_domain_name)) {
           tracePrintf(TRACE_LEVEL_INFO, "IpadmGlobalObjectManager::cfgMessageHandler mod_dn=%s",
		domainName.c_str());
	}
	modOp = true;
    }

    /* if we find DN lets use pMod to update */
    if ((pResults->size () != 0) && (xmlTag == ip_administration_domain_name) && modOp) {
	pMod = querySynchronously (synchronousQueryContext);
    }

    startTransaction ();
    if (delOp) {
        if (xmlTag == ip_administration_name_server) {
                pIpadmGlobalNSManagedObject = dynamic_cast<IpadmGlobalNSManagedObject *> ((*pRemove)[0]);
                delete pIpadmGlobalNSManagedObject;
        } else if (xmlTag == ip_administration_domain_name) {
                pIpadmGlobalDNManagedObject = dynamic_cast<IpadmGlobalDNManagedObject *> ((*pRemove)[0]);
                delete pIpadmGlobalDNManagedObject;
        }
    } else if (addOp || modOp) {
        if (xmlTag == ip_administration_name_server) {
           pIpadmGlobalNSManagedObject = new IpadmGlobalNSManagedObject (this); 
           pIpadmGlobalNSManagedObject->setNameSrvIp(ns_ip);
           /*Setting index value for name server so as to keep them in sequence of its addition in the db*/
           pIpadmGlobalNSManagedObject->setIndex(NSindex+1);
           updateWaveManagedObject (pIpadmGlobalNSManagedObject);
        } else if (xmlTag == ip_administration_domain_name) {
	   /* if entry is present update else create new */
	   if (pResults->size() != 0) {
	   	pIpadmGlobalDNMagObj = dynamic_cast<IpadmGlobalDNManagedObject *> ((*pMod)[0]);
		/* incase of update don't delete the obj */
	   } else {
                pIpadmGlobalDNMagObj = new IpadmGlobalDNManagedObject (this);
		/* if new delete obj later */
	        addOp = true;
	   }
           updateWaveManagedObject (pIpadmGlobalDNMagObj);
           pIpadmGlobalDNMagObj->setDomainName(domainName);
        }
    }
    status = commitTransaction ();

    if (pResults)
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    if (pMod)
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pMod);
    if (delOp)
        delete pRemove;
    if (addOp) {
      if (xmlTag == ip_administration_name_server) {
	if (pIpadmGlobalNSManagedObject)
        delete pIpadmGlobalNSManagedObject;
      }
      if (xmlTag == ip_administration_domain_name) {
	if (pIpadmGlobalDNManagedObject)
          delete pIpadmGlobalDNManagedObject;
	if (pIpadmGlobalDNMagObj)
	   delete pIpadmGlobalDNMagObj;
      }
    }

    if(modOp) {
	/* don't delete obj in case of update */
    }           
        
    if (FRAMEWORK_SUCCESS == status)
    {   
        trace (TRACE_LEVEL_DEBUG, "IPADMGOM:cfgMessageHandler:Successfully Cluster-Wide committed the Interface Configuration");
        status = WAVE_MESSAGE_SUCCESS;
    }       
    else
    {       
        trace (TRACE_LEVEL_FATAL, "IPADMGOM:cfgMessageHandler: Commiting the Interface configuration failed.  Status : "
                + FrameworkToolKit::localize (status));
    }
            
    // get cluser info vector
    vector<WaveManagedObject *> *pResultsClusterInfo = DoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
    prismAssert (NULL != pResultsClusterInfo, __FILE__, __LINE__);
    UI32 nodesInCluster = pResultsClusterInfo->size ();
    WaveNs::trace (TRACE_LEVEL_DEBUG, string("IPADMGObjectManager::getClusterInfo: Getting information for : ")+ nodesInCluster + " switches");

    UI32 tag = 0;
    for (i = 0; i < nodesInCluster; i++) {
         WaveNs::trace (TRACE_LEVEL_DEBUG, "IPADMGOM::cfgMessageHandler:Sending message to SW IPADMLOM");
         VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject  =
                 (dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[i]));
         const UI32 mappedId = pVcsNodeLocalManagedObject->getMappedId ();
         LocationId locationId = DcmToolKit::getLocationIdFromMappedId (mappedId);
         ObjectId pOID = DcmToolKit::getVcsNodeObjectIdFromMappedId(mappedId);
         WaveNs::tracePrintf(TRACE_LEVEL_DEBUG, "IPADMGOM:cfgMessageHandler: Sending message to MID=%d LID=0x%x",
                             mappedId, locationId);

        tag = pIpadmDnsConfigMessage->getWyserUserTag();
        if (xmlTag == ip_administration_name_server)
                pMessage = new IpadmLocalDnsCfgMessage (pOID, tag);
        else if (xmlTag == ip_administration_domain_name)
                pMessage = new IpadmLocalDnsCfgMessage (pOID, domainName ,tag);
        pMessage->setMappedId(mappedId);

        if (delOp)
                pMessage->setOpr("delete");
        else if (addOp)
                pMessage->setOpr("add");
        else if (modOp)
                pMessage->setOpr("modify");

        if (xmlTag == ip_administration_name_server)
                pMessage->setNameSrvIp(pIpadmDnsConfigMessage->getNameSrvIp());
        else if (xmlTag == ip_administration_domain_name)
                pMessage->setDomainName(pIpadmDnsConfigMessage->getDomainName());
        status = WAVE_MESSAGE_SUCCESS;

        if (WAVE_MESSAGE_SUCCESS == status) {
            pMessage->setWaveClientOriginatingLocationId(pIpadmDnsConfigMessage->getWaveClientOriginatingLocationId ());
            pMessage->setWaveNativeClientId(pIpadmDnsConfigMessage->getWaveNativeClientId ());
            pMessage->setWaveUserClientId(pIpadmDnsConfigMessage->getWaveUserClientId());
            pMessage->setCompletionStatus(status); 
    
            WaveNs::trace (TRACE_LEVEL_DEBUG, "IPADMGOM::cfgMessageHandler:Sending message : IpadmGlobalObjectManager::cfgMessageCallback");
            status = send (pMessage,
                reinterpret_cast<PrismMessageResponseHandler> (&IpadmGlobalObjectManager::cfgMessageCallback),
            pIpadmMessagingContext, 10000, locationId);
    
            WaveNs::trace (TRACE_LEVEL_DEBUG, "IPADMGOM::cfgMessageHandler:Sending message : IpadmGlobalObjectManager::cfgMessageCallback and back...");
            if (WAVE_MESSAGE_SUCCESS != status) {
                pIpadmMessagingContext->incrementNumberOfFailures ();
                trace (TRACE_LEVEL_DEBUG, string ("IpadmGlobalObjectManager::cfgMessageHandler: Sending a message to [") 
                        + PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
                trace (TRACE_LEVEL_ERROR, "IpadmGlobalObjectManager::cfgMessageHandler: Error in sending message to get this node  : Status : \""
                        + FrameworkToolKit::localize (messageStatus) + "\"");
                delete pMessage;
            } else {
                ++(*pIpadmMessagingContext);
            }
        }
    }         
         
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsClusterInfo);
         
    --(*pIpadmMessagingContext);
    WaveNs::trace (TRACE_LEVEL_DEBUG, "IPADMGOM::cfgMessageHandler: execute next step ");
    pIpadmMessagingContext->executeNextStep (((pIpadmMessagingContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
    WaveNs::trace (TRACE_LEVEL_DEBUG, "IPADMGOM::cfgMessageHandler: execute next step DONE ");
}

void  IpadmGlobalObjectManager::cfgMessageCallback (FrameworkStatus frameworkStatus,
   IpadmLocalDnsCfgMessage *pMessage, IpadmMessagingContext *pIpadmMessagingContext)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, "IpadmGlobalObjectManager::cfgMessageCallback: Entering");
    if((pMessage == NULL) || (pIpadmMessagingContext == NULL)) {
	WaveNs::trace (TRACE_LEVEL_ERROR, "IPADMGOM:: cfgMessageCallback: either pMessage or pIpadmMessagingContext is NULL ");
	return;
    }
    --(*pIpadmMessagingContext);

    // get Original request msg
    IpadmDnsConfigMessage *pIpadmDnsConfigMessage = dynamic_cast<IpadmDnsConfigMessage *> (pIpadmMessagingContext->getPMessage ());
    ResourceId completionStatus = pMessage->getCompletionStatus();
    
    if (FRAMEWORK_SUCCESS == frameworkStatus) {
        prismAssert (pMessage, __FILE__, __LINE__);

        WaveNs::trace (TRACE_LEVEL_DEBUG, string ("IpadmGlobalObjectManager::cfgMessageCallback: MID=") + pMessage->getMappedId());

        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ())) {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("IpadmGlobalObjectManager::cfgMessageCallback: MID=") +
                           pMessage->getMappedId() + string ("Completion Failure"));
            trace (TRACE_LEVEL_FATAL, "IPADMGOM : cfgMessageCallback   Status :"+ FrameworkToolKit::localize (pMessage->getCompletionStatus()));
            // pIpadmDnsConfigMessage->setData("Failure");
            pIpadmMessagingContext->incrementNumberOfFailures ();

        } else {
            // add returned result
            WaveNs::trace (TRACE_LEVEL_DEBUG, string ("IpadmGlobalObjectManager::cfgMessageCallback: MID=") +
                           pMessage->getMappedId() + string (" Success"));

            // pIpadmDnsConfigMessage->setData(string ("IpadmGlobalObjectManager::cfgMessageCallback: switchid=") +
            //     pMessage->getMappedId() + string (" Success\n"));

            pIpadmDnsConfigMessage->setData(pMessage->getData());
        }
    } else {

        WaveNs::trace (TRACE_LEVEL_ERROR, string ("IPADMGOM::cfgMessageCallback FRAMEWROK Error") + frameworkStatus);

        pIpadmMessagingContext->incrementNumberOfFailures ();
    }
    if (NULL != pMessage)
        delete pMessage;

    tracePrintf(TRACE_LEVEL_DEBUG,"IPADMGOM::cfgMessageCallback 33 ** =%d", pIpadmMessagingContext->getNumberOfFailures ());
    if (IPADM_NS_UNDEFINED_WARNING == completionStatus) {
        pIpadmMessagingContext->executeNextStep (((pIpadmMessagingContext->getNumberOfFailures ()) > 0) ? (WaveNs::WaveMessageStatus)IPADM_NS_UNDEFINED_WARNING : WAVE_MESSAGE_SUCCESS);
    }
    else if (IPADM_DN_UNDEFINED_WARNING == completionStatus) {
        pIpadmMessagingContext->executeNextStep (((pIpadmMessagingContext->getNumberOfFailures ()) > 0) ? (WaveNs::WaveMessageStatus)IPADM_DN_UNDEFINED_WARNING : WAVE_MESSAGE_SUCCESS); 
    }
    else if (IPADM_DNS_CONFIG_FAIL_ERROR == completionStatus){
         pIpadmMessagingContext->executeNextStep (((pIpadmMessagingContext->getNumberOfFailures ()) > 0) ? (WaveNs::WaveMessageStatus)IPADM_DNS_CONFIG_FAIL_ERROR : WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        pIpadmMessagingContext->executeNextStep (((pIpadmMessagingContext->getNumberOfFailures ()) > 0) ? (WaveNs::WaveMessageStatus)IPADM_CONFIG_FAILED_ON_ALL_NODES_IN_CLUSTER : WAVE_MESSAGE_SUCCESS);
    }
}

}
