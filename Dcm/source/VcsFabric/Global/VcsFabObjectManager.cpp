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

/**
 *@file VcsFabObjectManager.cpp  
 * Copyright (C) 2005-2010 BrocadeCommunications Systems,Inc. 
 * All rights reserved. 
 * Description: This file defines the implementation of the
 *              VcsFabObjectManager class which represents a
 *              Global Plugin responsible for all cluster
 *              related operations like add, rejoin and
 *              delete
 * Author : Jitendra Singh,Aashish Akhouri
 */
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include <sstream>
#include <iostream>
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsClusterManagedObject.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodePrincipalSelectionWorker.h"
#include "VcsFabric/Local/VcsNodeFCPrincipalTrackingWorker.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmShell/DcmShell.h"
#include "Shell/ShellPrism.h"
#include "Shell/ShellDebug.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "Cluster/ClusterMessages.h"
#include "Cluster/Local/WaveNode.h"
#include "Cluster/Local/ClusterLocalSetThisNodeIpAddressMessage.h"
#include "Framework/Messaging/Local/PrismEvent.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Persistence/PersistenceObjectManagerMessages.h"
#include "fabric/fabric.h" 
#include "VcsFabric/Global/VcsFabAddRejoinNodeWorker.h"
#include "VcsFabric/Global/VcsFabRemoveSegmentNodeWorker.h"
#include "VcsFabric/Global/VcsFabDeleteClusterWorker.h"
#include "VcsFabric/Global/VcsFabVcsModeWorker.h"
#include "VcsFabric/Global/VirtualIpGlobalWorker.h"
#include "VcsFabric/Global/VcsClusterConnectionQueryWorker.h"
#include "VcsFabric/Global/VcsFabConfigDistributionWorker.h"
#include "VcsFabric/Global/VcsFabConfigSnapshotWorker.h"
#include "VcsFabric/Local/VcsFabricLocalMessages.h"
#include "VcsFabric/Local/VcsFabricLocalSerializableObject.h"

extern "C"
{
    #include "raslog/raslog.h"
    #include "raslog/raslog_vcs.h"
}
#include "VcsFabric/Global/VcsFabVcsModeWorker.h"
#include "Framework/Failover/FailoverAsynchronousContext.h"

namespace DcmNs
{

string  getIpAddressFromDomainId        (UI32 domainId);
UI32    getMgmtPort                     (UI32 domainId);

/** 
 *Name: VcsFabObjectManager 
 *Description: Constructor 
 *@return: none  
 */
VcsFabObjectManager::VcsFabObjectManager () : WaveObjectManager (getClassName ()),
    m_bootComplete (false), m_becomePrincipalFlag (false),m_timerHandle (0), m_timerRunningFlag (false) 
                                              
{
    PrismFrameworkObjectManager::excludeServiceForClusterValidationPhase (getServiceId ());

    VcsClusterManagedObject vcsClusterManagedObject (this);
    vcsClusterManagedObject.setupOrm ();
    addManagedClass (VcsClusterManagedObject::getClassName ());

    //All the Message Handlers

    addOperationMap (VCS_FABRIC_GET_CLUSTER_INFO, reinterpret_cast<PrismMessageHandler> (&VcsFabObjectManager::clusterInfoMessageHandler));

    m_pVcsFabAddRejoinNodeWorker        = new VcsFabAddRejoinNodeWorker (this);
    m_pVcsFabRemoveSegmentNodeWorker    = new VcsFabRemoveSegmentNodeWorker (this);
    m_pVcsFabDeleteClusterWorker        = new VcsFabDeleteClusterWorker (this);
    m_pVcsFabVcsModeWorker              = new VcsFabVcsModeWorker (this);
    m_pVirtualIpGlobalWorker            = new VirtualIpGlobalWorker (this);
    m_pVcsClusterConnectionQueryWorker  = new VcsClusterConnectionQueryWorker (this);
    m_pVcsFabConfigDistributionWorker   = new VcsFabConfigDistributionWorker (this);
    m_pVcsFabConfigSnapshotWorker       = new VcsFabConfigSnapshotWorker (this);
    m_clusterGUID                       = vcsClusterManagedObject.getClusterGUID();
}

/** 
 *Name: ~VcsFabObjectManager 
 *Description: Destructor
 *@return: none  
 */
VcsFabObjectManager::~VcsFabObjectManager ()
{
    if(m_pVcsFabAddRejoinNodeWorker) 
    {
        delete (m_pVcsFabAddRejoinNodeWorker);
    }

    if (m_pVcsFabRemoveSegmentNodeWorker) 
    {
        delete (m_pVcsFabRemoveSegmentNodeWorker);
    }

    if (m_pVcsFabDeleteClusterWorker) 
    {
        delete (m_pVcsFabDeleteClusterWorker); 
    }

    if (m_pVcsFabVcsModeWorker)
    {
        delete (m_pVcsFabVcsModeWorker);
    }
    
    if (m_pVirtualIpGlobalWorker)
    {
        delete (m_pVirtualIpGlobalWorker);
    }

    if (m_pVcsClusterConnectionQueryWorker)
    {
        delete (m_pVcsClusterConnectionQueryWorker);
    }
    
    if (m_pVcsFabConfigDistributionWorker)
    {
        delete (m_pVcsFabConfigDistributionWorker);
    }

    if (m_pVcsFabConfigSnapshotWorker)
    {   
        delete (m_pVcsFabConfigSnapshotWorker);
    }
}
/*
string VcsFabObjectManager::getClassName ()
{
    return ("VcsFabObjectManager");
}
*/

VcsFabObjectManager *VcsFabObjectManager::getInstance ()
{
    static VcsFabObjectManager *pVcsFabObjectManager = new VcsFabObjectManager ();

    WaveNs::prismAssert (NULL != pVcsFabObjectManager, __FILE__, __LINE__);

    return (pVcsFabObjectManager);
}

PrismServiceId VcsFabObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void VcsFabObjectManager::setBitMapSize (BitMap &bitMap)
{
        bitMap.resize (max_domain_list -1);
}

void VcsFabObjectManager::setSizeForDomainBitMaps (scnDomainsStruct & domainBitMapStruct)
{
        setBitMapSize (domainBitMapStruct.dcaDomainsBitMap);
        setBitMapSize (domainBitMapStruct.reachableDomainsBitMap);
        setBitMapSize (domainBitMapStruct.remoteKnownDCABitMap);
        setBitMapSize (domainBitMapStruct.switchInfoReceivedBitMap);
}

vector<WaveManagedObject *> *VcsFabObjectManager::fabOMDoSynchronousQuery (const string className)
{
        vector<WaveManagedObject *> *pResults = querySynchronously (className);
        prismAssert (NULL != pResults, __FILE__, __LINE__);
        return pResults;
}

vector<VcsNodeLocalManagedObject *> *VcsFabObjectManager::getNodeLocalManagedObjects ()
{
        vector<WaveManagedObject *> *pResults = fabOMDoSynchronousQuery (VcsNodeLocalManagedObject::getClassName());
        return (reinterpret_cast <vector<VcsNodeLocalManagedObject *> *> (pResults));
}

VcsClusterManagedObject *VcsFabObjectManager::getVcsClusterManagedObject()
{
        vector<WaveManagedObject *> *pResults = fabOMDoSynchronousQuery (VcsClusterManagedObject::getClassName());
    VcsClusterManagedObject *pVcsClusterManagedObject = NULL;
    prismAssert (1 >= pResults->size (), __FILE__, __LINE__);

        if (1 == pResults->size ())
        {
        pVcsClusterManagedObject = dynamic_cast<VcsClusterManagedObject *> ((*pResults)[0]);
                delete pResults;
                return pVcsClusterManagedObject;
        }
        // pResults->size () must be 0
        delete pResults;
        return NULL;    
}

WaveManagedObject *VcsFabObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;
    if (VcsClusterManagedObject::getClassName() == managedClassName)
    {
        pWaveManagedObject = new VcsClusterManagedObject (this);

    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::createManagedObjectInstance : unsupported managed class name : " + managedClassName);

    }
    return pWaveManagedObject;
}

PrismMessage *VcsFabObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case VCS_FABRIC_CREATE_CLUSTER :
            pPrismMessage = new VcsCreateClusterMessage ();
            break;
        case VCS_FABRIC_ADD_SWITCH :
            pPrismMessage = new VcsAddSwitchMessage     ();
            break;
        case VCS_FABRIC_REJOIN_SWITCH :
            pPrismMessage = new VcsRejoinSwitchMessage  ();
            break;
        case VCS_FABRIC_DELETE_CLUSTER :
            pPrismMessage = new VcsDeleteClusterMessage ();
            break;
        case VCS_FABRIC_REMOVE_SWITCH :
            pPrismMessage = new VcsRemoveSwitchMessage  ();
            break;
        case VCS_FABRIC_MAKE_PRINCIPAL :
            pPrismMessage = new VcsMakePrincipalMessage ();
            break;
        case VCS_FABRIC_GET_CLUSTER_INFO :
            pPrismMessage = new VcsClusterInfoMessage ();
            break;
        case VCS_FABRIC_SEGMENT_SWITCH :
            pPrismMessage = new VcsSegmentSwitchMessage ();
            break;
        case VCS_FABRIC_IS_IP_REACHABLE_FROM_CLUSTER :
            pPrismMessage = new VcsIsIpReachableFromClusterMessage ();
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void VcsFabObjectManager::deleteObjects (vector<WaveManagedObject *> *pResults)
{
    int i;
        if (NULL == pResults) return;
        int count = pResults->size ();

        for (i = 0; i < count; i++)
        {
            if (NULL != ((*pResults)[i])) delete ((*pResults)[i]);
        }
    delete pResults;
}

UI32 VcsFabObjectManager::getLocalSwitchPort ()
{
    return  FrameworkToolKit::getThisLocationPort();
}

string VcsFabObjectManager::getLocalSwitchIpAddress ()
{
    return  FrameworkToolKit::getThisLocationIpAddress();
}

string VcsFabObjectManager::getIpAddressFromDomainId (UI32 domainId)
{
        return VcsNodeFCPrincipalTrackingWorker::getIpAddressFromDomainId(domainId);
}

UI32 VcsFabObjectManager::getPortFromDomainId(UI32 domainId)
{
    return FrameworkToolKit::getThisLocationPort();
}

bool VcsFabObjectManager::validIpV4SubString (const string &ipSubString)
{
        UI32 val;
        val = atoi (ipSubString.c_str ());
        if ((255 >= val) && (0 <= val))
        {
                return true;
        }
        else
        {
                return false;
        }
}

UI32 VcsFabObjectManager::getMgmtPort(UI32 domainId)
{
        return VcsNodeFCPrincipalTrackingWorker::getMgmtPort(domainId);
}

bool VcsFabObjectManager::validDomainId (UI32 domainId)
{
    if ((0 < domainId) && ((max_domain_list -1) > domainId))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool VcsFabObjectManager::validIpV4Address (const string &ipAddress)
{
        size_t pos = 0;
        size_t lastPos = 0;
        UI32 i;
        string ipSubString;

        for (i = 0; i <= 3; i++)
        {
                pos = ipAddress.find ('.',lastPos);
                if ((pos == string::npos) && (3 > i))
                {
                        trace (TRACE_LEVEL_ERROR, string("VcsFabObjectManager::validIpV4Address : i = ") + i + " pos = " + pos);
                        return false;
                }
                else if ((pos != string::npos) && (i == 3))
                {
                        trace (TRACE_LEVEL_ERROR, string("VcsFabObjectManager::validIpV4Address : extra '.' ") + " lastpos = "+ lastPos + " pos = " + pos);
                        return false;
                }
                ipSubString = ipAddress.substr (lastPos, (pos - lastPos));
                if (!validIpV4SubString (ipSubString))
                {
                        trace (TRACE_LEVEL_ERROR, string("VcsFabObjectManager::validIpV4Address : i = ") + i + " pos = " + pos + " substr = " + ipSubString);
                        return false;
                }
                //trace (TRACE_LEVEL_INFO, string("VcsFabObjectManager::validIpV4Address : i = ") + i + " pos = " + pos + " lastpos = "+ lastPos + " substr = " + ipSubString);
                lastPos = pos+1;
        }
        return true;
}

UI32 VcsFabObjectManager::getIpAddressFromInputString (const string &ipAddressAndPort, string &ipAddress)
{
        size_t pos = 0;
        trace (TRACE_LEVEL_INFO, "VcsFabObjectManager::getIpAddressFromInputString : ipAddressAndPort = " + ipAddressAndPort);

        pos = ipAddressAndPort.find (':');

        ipAddress = ipAddressAndPort.substr (0,pos);
        if (validIpV4Address (ipAddress))
        {
                return VCS_SUCCESS;
        }
        else
        {
                trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::getIpAddressFromInputString : bad ipAddress (syntax: ipAddress:Port): " + ipAddressAndPort);
                return PARAMETER_OUT_OF_RANGE;
        }
}

UI32 VcsFabObjectManager::getPortFromInputString (const string &ipAddressAndPort, UI32 &port)
{
        size_t pos = 0;
        trace (TRACE_LEVEL_INFO, "VcsFabObjectManager::getPortFromInputString : ipAddressAndPort = " + ipAddressAndPort);

        string portString;
        pos = ipAddressAndPort.find (':');
        if (pos >= ipAddressAndPort.length ())
        {
                port = default_mgmt_port;
                return VCS_SUCCESS;
        }
        portString = ipAddressAndPort.substr (pos+1,ipAddressAndPort.size ());
        if (0 >= portString.size ())
        {
                port = default_mgmt_port;
                trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::getPortFromInputString : using default port = ") + port);
        }
        else
        {
                port = atoi (portString.c_str ());
        }
        return VCS_SUCCESS;
}


UI32 VcsFabObjectManager::getPrincipalSwitchObjectId (ObjectId &principalSwitchObjectId)
{
    VcsClusterManagedObject *pVcsClusterManagedObject = getVcsClusterManagedObject ();
    if (NULL == pVcsClusterManagedObject)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::getPrincipalSwitchObjectId : VCS cluster object not in database");
            return CLUSTER_OBJECT_NOT_FOUND;
        }
        principalSwitchObjectId =  pVcsClusterManagedObject->getPrincipalSwitchObjectId ();
        delete pVcsClusterManagedObject;
        return VCS_SUCCESS;
}

VcsNodeLocalManagedObject * VcsFabObjectManager::getSwitchObjectFromObjectId(const ObjectId &switchObjectId)
{
        vector<ObjectId> switchObjectIds;
        switchObjectIds.insert(switchObjectIds.begin(), 1, switchObjectId);
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;

        vector<WaveManagedObject *> *pResults = querySynchronously (VcsNodeLocalManagedObject::getClassName (), switchObjectIds);
        if (NULL == pResults)
        {
                trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::getSwitchObjectFromObjectId: pResults for LocalNode query by ObjectId returned NULL");
                prismAssert (false, __FILE__, __LINE__);
        }
        if (1 < pResults->size ())
        {
                delete pResults;
                trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::getSwitchObjectFromObjectId: multiple objects with same object ID found");
                prismAssert (false, __FILE__, __LINE__);
        }
        if (0 == pResults->size ())
        {
                delete pResults;
                return NULL;    
        }
        pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *>((*pResults)[0]);
        delete pResults;
        return pVcsNodeLocalManagedObject;
}

VcsNodeLocalManagedObject * VcsFabObjectManager::getSwitchObjectFromDomainId (UI32 domainId)
{
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (domainId, "domainId"));
        vector<WaveManagedObject *> *pResultsLocalNodes = querySynchronously (&synchronousQueryContext);
        if (NULL == pResultsLocalNodes)
        {
                trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::getSwitchObjectFromDomainId: results pointer from querySynchronously is NULL");
                prismAssert (false, __FILE__, __LINE__);
        }

        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;

        string ipAddress = "";
        if (1 < pResultsLocalNodes->size ())
        {
          trace (TRACE_LEVEL_ERROR, string ("VcsFabObjectManager::getSwitchObjectFromDomainId: multiple objects with same domain ID found domainId = ")+domainId + " numObjects = "+pResultsLocalNodes->size ());
          UI32 nodeDomainId = 0;
          for (UI32 i = 0; i < pResultsLocalNodes->size (); i++)
          {
              pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *>((*pResultsLocalNodes)[i]);
              prismAssert(NULL!=pVcsNodeLocalManagedObject, __FILE__, __LINE__);
              nodeDomainId = pVcsNodeLocalManagedObject->getDomainId ();
              ipAddress = (pVcsNodeLocalManagedObject->getPublicIpAddress ())[0];
              trace (TRACE_LEVEL_ERROR, string ("VcsFabObjectManager::getSwitchObjectFromDomainId: object list: domainId = ")+domainId + " domainId from the MO" +nodeDomainId + " pubIpAddress = "+ipAddress);
          }
          prismAssert (false, __FILE__, __LINE__);
        }
        if (0 == pResultsLocalNodes->size ())
        {
                delete pResultsLocalNodes;
                return NULL;
        }
        pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *>((*pResultsLocalNodes)[0]);
        prismAssert (NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__); 
        delete pResultsLocalNodes;
        return pVcsNodeLocalManagedObject;
}

VcsNodeLocalManagedObject *VcsFabObjectManager::getNodeLocalManagedObjectForThisNode ()
{
        ObjectId localSwitchObjectId;
        getLocalSwitchObjectId (localSwitchObjectId);
        return getSwitchObjectFromObjectId (localSwitchObjectId);
}

bool VcsFabObjectManager::vcsModeEnabled ()
{
    bool vcsMode;

    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = getNodeLocalManagedObjectForThisNode ();

    if (NULL == pVcsNodeLocalManagedObject)
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
        return 0;
    }
    else
    {
        vcsMode = pVcsNodeLocalManagedObject->getVcsMode ();
        delete pVcsNodeLocalManagedObject;
        return vcsMode;
    }
}

void VcsFabObjectManager::getLocalSwitchObjectId (ObjectId &localSwitchObjectId)
{
    localSwitchObjectId = (VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeObjectId ();
}

UI32 VcsFabObjectManager::getLocalNodeVcsId ()
{
    const ObjectId  localSwitchObjectId = (VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeObjectId ();
    VcsNodeLocalManagedObject * pVcsNodeLocalManagedObject = getSwitchObjectFromObjectId(localSwitchObjectId);
    prismAssert (NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__);
    UI32 vcsId = pVcsNodeLocalManagedObject->getVcsId ();
    delete pVcsNodeLocalManagedObject;
    return vcsId;
}


void VcsFabObjectManager::printLocalNodesList ()
{
        vector<WaveManagedObject *> *pResultsAllLocalNodes = fabOMDoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
        UI32 totalnodes = pResultsAllLocalNodes->size ();
    trace (TRACE_LEVEL_INFO, string("VcsFabObjectManager::printLocalNodesList : total local nodes found = ")+ totalnodes);
    UI32 i;
    VcsNodeLocalManagedObject    *pVcsNodeLocalManagedObject = NULL;
    ObjectId objectId;
    string serialNumber;
    for (i = 0; i < (*pResultsAllLocalNodes).size (); i++)
    {
        pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsAllLocalNodes)[i]);
        objectId = pVcsNodeLocalManagedObject->getObjectId ();
        serialNumber = pVcsNodeLocalManagedObject->getSerialNumber ();
        trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::printLocalNodesList: serial # = ")+serialNumber+" objectId = "+objectId.toString ());
    }
        deleteObjects (pResultsAllLocalNodes);
}


UI32 VcsFabObjectManager::getRemoteNodeObjectIdFromLocalNodesDB (string remoteSerialNumber, ObjectId &remoteNodeObjectId)
{
    //printLocalNodesList ();
    VcsNodeLocalManagedObject    *pVcsNodeLocalManagedObject = NULL;

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeString (remoteSerialNumber, "serialnumber"));


    vector<WaveManagedObject *> *pResultsLocalNodes = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsLocalNodes, __FILE__, __LINE__);
    UI32    nodeCount = pResultsLocalNodes->size ();
    prismAssert (1 >=  nodeCount, __FILE__, __LINE__);

    if (1 == nodeCount)
    {
         pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsLocalNodes)[0]);
         remoteNodeObjectId = pVcsNodeLocalManagedObject->getObjectId ();
         deleteObjects (pResultsLocalNodes);
         return VCS_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string("VcsFabObjectManager::getRemoteNodeObjectIdFromLocalNodesDB : local node not found for SerialNumber = ")+ remoteSerialNumber);
                deleteObjects (pResultsLocalNodes);
        return SWITCH_DOMAIN_ID_NOT_FOUND;
    }
}

UI32 VcsFabObjectManager::getRemoteNodeObjectIdFromDomainId (u_short domainId, ObjectId &remoteNodeObjectId)
{
 
    vector<WaveManagedObject *> *pResultsAllLocalNodes = fabOMDoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
    UI32 totalnodes = pResultsAllLocalNodes->size ();
    trace (TRACE_LEVEL_INFO, string("VcsFabObjectManager::getRemoteNodeObjectIdFromDomainId : total local nodes found = ")+ totalnodes);
        deleteObjects (pResultsAllLocalNodes);


        WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (domainId, "domainId"));

    vector<WaveManagedObject *> *pResultsLocalNodes = querySynchronously (&synchronousQueryContext);
    prismAssert (NULL != pResultsLocalNodes, __FILE__, __LINE__);
    UI32    nodeCount = pResultsLocalNodes->size ();
    prismAssert (1 >=  nodeCount, __FILE__, __LINE__);

    if (1 == nodeCount)
    {
                VcsNodeLocalManagedObject        *pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsLocalNodes)[0]);
        remoteNodeObjectId = pVcsNodeLocalManagedObject->getObjectId ();
                deleteObjects (pResultsLocalNodes);
                return VCS_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string("VcsFabObjectManager::getRemoteNodeObjectIdFromDomainId : local node not found for domainId = ")+ domainId);
                deleteObjects (pResultsLocalNodes);
        return SWITCH_DOMAIN_ID_NOT_FOUND;
    }
}

UI32 VcsFabObjectManager::getRemoteNodeDomainIdFromObjectId (const ObjectId &objectId, UI32 &domainId)
{
    VcsNodeLocalManagedObject   *pVcsNodeLocalManagedObject = getSwitchObjectFromObjectId       (objectId);
    if (NULL != pVcsNodeLocalManagedObject)
    {
        domainId = pVcsNodeLocalManagedObject->getDomainId ();
        delete pVcsNodeLocalManagedObject;
        return VCS_SUCCESS;
    }
    else
    {
        domainId = 0;
        return SWITCH_DOMAIN_ID_NOT_FOUND;
    }
}


bool VcsFabObjectManager::getBecomePrincipalFlag ()
{
    return  m_becomePrincipalFlag ;
}

void VcsFabObjectManager::setBecomePrincipalFlag (bool flag)
{
        m_becomePrincipalFlag = flag;
}

UI32 VcsFabObjectManager::getNumSwitchesInCluster ()
{
        vector<VcsNodeLocalManagedObject *> *pVcsLocalNodes = getNodeLocalManagedObjects ();
        UI32 numNodes = pVcsLocalNodes->size ();
        deleteObjects (reinterpret_cast <vector<WaveManagedObject *> *> (pVcsLocalNodes));
        return numNodes;        
}

UI32 VcsFabObjectManager::getDomainsInCluster (BitMap &domainsIncluster)
{
        vector<WaveManagedObject *> *pVcsLocalNodes = fabOMDoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;
        UI32 numNodes = pVcsLocalNodes->size ();
        UI32 i;
        UI32 count = 0;
        for (i = 0; i < numNodes; i++)
        {
                pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pVcsLocalNodes)[i]);
                if (NULL != pVcsNodeLocalManagedObject)
                {
                        domainsIncluster.set (pVcsNodeLocalManagedObject->getDomainId ());
                        count++;
                }
        }
        deleteObjects (pVcsLocalNodes); 
        return count;
}


UI32 VcsFabObjectManager::deleteWaveNodeObject (const ObjectId &switchObjectId)
{
    // Delete WaveNode object
    string ipAddress;
    UI32 portId;
    UI32 numberOfNode = 0;
    UI32 switchDomainId = 0;
    UI32 retCode = getRemoteNodeDomainIdFromObjectId (switchObjectId, switchDomainId);

        if (VCS_SUCCESS != retCode)
        {
                return retCode;
        }
    else
    {
        ipAddress =  getIpAddressFromDomainId (switchDomainId);
        portId = getMgmtPort(switchDomainId);
    }

    ClusterObjectManagerDeleteNodeMessage *message = new  ClusterObjectManagerDeleteNodeMessage ();
    message->setSecondaryNodeNameAndPort (ipAddress, portId);
    numberOfNode = 1;
    message->setNNodesToDelete(numberOfNode);

    WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabObjectManager::deleteWaveNodeObject WaveNode to remove : IpAddr = ") +ipAddress+ " Port = " +portId);

    ResourceId status = WAVE_MESSAGE_ERROR;
    ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;
    status = sendSynchronously (message);
    completionStatus = message->getCompletionStatus ();

    delete message;

    if (WAVE_MESSAGE_SUCCESS == status)
    {
                if (WAVE_MESSAGE_SUCCESS == completionStatus)
                {
                        trace (TRACE_LEVEL_SUCCESS, "VcsFabObjectManager::deleteWaveNodeObject : Successfully deleted Wave node from Database."+FrameworkToolKit::localize (status));
                        return VCS_SUCCESS;
                }
                else
                {
                        trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::deleteWaveNodeObject : Failed to delete Wave node from Database."+FrameworkToolKit::localize (status));
                        return GENERIC_ERROR;
                }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::deleteWaveNodeObject : SendSynchronous message failed."+FrameworkToolKit::localize (status));
        return GENERIC_ERROR;
    }
}

string VcsFabObjectManager::getLocalSwitchSerialNumber ()
{
    ObjectId localSwitchObjectId = (VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeObjectId ();
    VcsNodeLocalManagedObject * pVcsNodeLocalManagedObject = getSwitchObjectFromObjectId(localSwitchObjectId);
    prismAssert (NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__);
        string serialNumber = pVcsNodeLocalManagedObject->getSerialNumber ();
        delete pVcsNodeLocalManagedObject;
        return serialNumber;
}

UI32 VcsFabObjectManager::getVcsClusterId (bool &clusterExists)
{
    VcsClusterManagedObject *pVcsClusterManagedObject = NULL;
    UI32 vcsClusterId = 0;
    clusterExists = false;

    vector<WaveManagedObject *> *pResultsCluster = fabOMDoSynchronousQuery (VcsClusterManagedObject::getClassName ());
    prismAssert (1 >= pResultsCluster->size (), __FILE__, __LINE__);

    if (1 == (*pResultsCluster).size())
    {
        clusterExists = true;
        pVcsClusterManagedObject = dynamic_cast<VcsClusterManagedObject *>((*pResultsCluster)[0]);
        vcsClusterId = pVcsClusterManagedObject->getVcsClusterId ();
        delete pVcsClusterManagedObject;
        delete pResultsCluster;
    }
    else
    {
        vcsClusterId = getLocalNodeVcsId ();
                delete pResultsCluster;
    }

    return vcsClusterId;
}

bool VcsFabObjectManager::isSwitchInVcsClusterManagedObject (string responderSerialNumber)
{
    VcsClusterManagedObject *pVcsClusterManagedObject = getVcsClusterManagedObject();

    if (NULL == pVcsClusterManagedObject) // no VCS cluster formed
    {
        return false;
    }
    else
    {
        ObjectId remoteNodeObjectId;
        UI32 retCode = getRemoteNodeObjectIdFromLocalNodesDB (responderSerialNumber, remoteNodeObjectId);
        if (VCS_SUCCESS == retCode)
        {
            if (pVcsClusterManagedObject->isObjectIdInClusterMO     (remoteNodeObjectId))
            {
                WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabObjectManager::isSwitchInVcsClusterManagedObject : switch object found in VCS Cluster MO serialNumber = " +responderSerialNumber);
                delete pVcsClusterManagedObject;
                return true;
            }
            else
            {
                WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabObjectManager::isSwitchInVcsClusterManagedObject : switch object NOT found in VCS Cluster MO serialNumber = " +responderSerialNumber);
                delete pVcsClusterManagedObject;
                return false;
            }
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "VcsFabObjectManager::isSwitchInVcsClusterManagedObject : unable to retrieve ObjectId from local nodes database using serialNumber = " +responderSerialNumber);
            delete pVcsClusterManagedObject;
            return false;
        }
    }
}

bool VcsFabObjectManager::isSwitchInVcsClusterManagedObject (const ObjectId &switchOid)
{
        VcsClusterManagedObject *pVcsClusterManagedObject = getVcsClusterManagedObject();
        if (NULL == pVcsClusterManagedObject) // no VCS cluster formed
        {
                return false;
        }
        else
        {
                if (pVcsClusterManagedObject->isObjectIdInClusterMO     (switchOid))
                {
                        delete pVcsClusterManagedObject;
                        return true;
                }
                else
                {
                        delete pVcsClusterManagedObject;
                        return false;
                }
        }       
}

UI32 VcsFabObjectManager::deleteObjectIdFromVcsCluster (ObjectId &removeSwitchObjectId, UI32 &nSwitchesInCluster)
{
        // now get the VCS cluster object from the database
        vector<WaveManagedObject *> *pResultsCluster = fabOMDoSynchronousQuery (VcsClusterManagedObject::getClassName ());
        if (0 == pResultsCluster->size ())
        {
                trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::deleteObjectIdFromVcsCluster: VCS Cluster object not created or deleted");
                delete pResultsCluster;
                return CLUSTER_OBJECT_NOT_FOUND;
        }
        if (1 != pResultsCluster->size ())
        {
                trace (TRACE_LEVEL_ERROR, string ("VcsFabObjectManager::deleteObjectIdFromVcsCluster: number of VCS cluster objects = ")+pResultsCluster->size ()+" should be = 1");
                prismAssert (false, __FILE__, __LINE__); //dcmd will crash here
        }

        VcsClusterManagedObject *pVcsClusterManagedObject = dynamic_cast<VcsClusterManagedObject *>((*pResultsCluster)[0]);
        nSwitchesInCluster = pVcsClusterManagedObject->getSwitchCount ();

        // update the cluster object after deleting the switch reference with the given object id

        UI32 retCode = pVcsClusterManagedObject->removeSwitchObjectId (removeSwitchObjectId);
        if (VCS_SUCCESS != retCode)
        {
                WaveNs::trace (TRACE_LEVEL_ERROR, string("VcsFabObjectManager::deleteObjectIdFromVcsCluster : Failed to remove switch from VCS Cluster.")+ " retCode = "+retCode);
        return retCode;
        }

        startTransaction ();

        updateWaveManagedObject (pVcsClusterManagedObject);

        ResourceId status = commitTransaction ();

        deleteObjects (pResultsCluster); // pResults and pVcsClusterManagedObject deleted
        if (FRAMEWORK_SUCCESS == status)
        {
                nSwitchesInCluster--;
                WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsFabObjectManager::deleteObjectIdFromVcsCluster : Successfully removed switch from VCS Cluster.");
                return VCS_SUCCESS;
        }
        else
        {
                WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::deleteObjectIdFromVcsCluster : Failed to remove switch from VCS Cluster.");
                return GENERIC_ERROR;
        }
}

WaveNode *VcsFabObjectManager::getWaveNode (VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject)
{
        UI32 locationId = pVcsNodeLocalManagedObject->getOwnerLocationId ();
        WaveNode *pWaveNode;

        WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (WaveNode::getClassName ());
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (locationId, "locationId"));

        vector<WaveManagedObject *> *pResultsWaveNodes = querySynchronously (&synchronousQueryContext);
        if (NULL == pResultsWaveNodes)
        {
                trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::getWaveNode: results pointer from querySynchronously is NULL");
                prismAssert (false, __FILE__, __LINE__);
        }
        prismAssert (1 >= pResultsWaveNodes->size (), __FILE__, __LINE__);
        pWaveNode = dynamic_cast<WaveNode *>((*pResultsWaveNodes)[0]);
        delete pResultsWaveNodes;
        return pWaveNode;
}

ResourceId VcsFabObjectManager::getNodeStatus (VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject, UI32 &specificStatus, UI32 &genericStatus)
{
    WaveNode *pWaveNode = getWaveNode (pVcsNodeLocalManagedObject);

    if (NULL != pWaveNode)
    {
        specificStatus = pWaveNode->getSpecificStatus ();
        genericStatus = pWaveNode->getGenericStatus ();
        delete pWaveNode;
    }

    return genericStatus;
}


ResourceId VcsFabObjectManager::getClusterInfoForNodeInWaitForRejonState(VcsFabMessagingContext *pVcsFabMessagingContext)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabObjectManager::getClusterInfoForNodeInWaitForRejonState: Entering");

    VcsClusterInfoMessage *pVcsClusterInfoMessage = dynamic_cast<VcsClusterInfoMessage *> (pVcsFabMessagingContext->getPMessage ());

    prismAssert(NULL != pVcsClusterInfoMessage, __FILE__, __LINE__);

    UI32 startDomainId      = pVcsClusterInfoMessage->getStartDomainId ();
    UI32 endDomainId        = pVcsClusterInfoMessage->getEndDomainId ();

    if (((int)endDomainId < (int)startDomainId) || ((int)startDomainId < 0) || ((int)endDomainId < 0))
    {
        pVcsClusterInfoMessage->setSwitchesInCluster (0);
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::getClusterInfoForNodeInWaitForRejonState: end DomainId must be >= start DomainId both must be > 0");

        return WAVE_MESSAGE_ERROR;
    }

    pVcsClusterInfoMessage->setSwitchesInCluster (1); // only local switch - which must always be present
    pVcsClusterInfoMessage->setClusterCreated(0);

    string clusterFormationTime = "None";
            //return WAVE_MESSAGE_ERROR;

    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = getNodeLocalManagedObjectForThisNode ();

    if (NULL == pVcsNodeLocalManagedObject)
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
        return 0;
    }

    string ipAddress = "";
    WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabObjectManager::getClusterInfoForNodeInWaitForRejonState: Getting information for switch waiting for rejoin"));

    if ((0 == startDomainId) || ((pVcsNodeLocalManagedObject->getDomainId () >= startDomainId) && (pVcsNodeLocalManagedObject->getDomainId () <= endDomainId)))
    {
        pVcsClusterInfoMessage->addVcsMode (pVcsNodeLocalManagedObject->getVcsMode());
        pVcsClusterInfoMessage->addDomainId (pVcsNodeLocalManagedObject->getDomainId ());
        pVcsClusterInfoMessage->addSerialNumber (pVcsNodeLocalManagedObject->getSerialNumber ());

        pVcsClusterInfoMessage->addFcfMac (pVcsNodeLocalManagedObject->getFcfMac ());
        pVcsClusterInfoMessage->addSwitchMac (pVcsNodeLocalManagedObject->getSwitchMac ());
        pVcsClusterInfoMessage->addInVcs (pVcsNodeLocalManagedObject->getInVcs ());
        pVcsClusterInfoMessage->addIsPrincipal (pVcsNodeLocalManagedObject->getIsPrincipal ());
        pVcsClusterInfoMessage->addVcsId (pVcsNodeLocalManagedObject->getVcsId ());
        if (validDomainId (pVcsNodeLocalManagedObject->getDomainId ()))
        {
            pVcsClusterInfoMessage->addIpAddress (getIpAddressFromDomainId (pVcsNodeLocalManagedObject->getDomainId ()));
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::getClusterInfoForNodeInWaitForRejonState: domainId is invalid so internal ipAddress is not set");
            pVcsClusterInfoMessage->addIpAddress ("Not set");
        }

        pVcsClusterInfoMessage->addPublicIpAddress (pVcsNodeLocalManagedObject->getPublicIpAddress ());

               
        pVcsClusterInfoMessage->addSpecificStatus (pVcsNodeLocalManagedObject->getSpecificStatus());
        pVcsClusterInfoMessage->addGenericStatus  (pVcsNodeLocalManagedObject->getGenericStatus());
        pVcsClusterInfoMessage->addLocationId (pVcsNodeLocalManagedObject->getOwnerLocationId ());
    }
        
    pVcsClusterInfoMessage->addClusterFormTime (clusterFormationTime);
    delete pVcsNodeLocalManagedObject;
    return WAVE_MESSAGE_SUCCESS;

}

void VcsFabObjectManager::getClusterInfo (VcsFabMessagingContext *pVcsFabMessagingContext)
{

        WaveNs::trace (TRACE_LEVEL_DEBUG, "VcsFabObjectManager::getClusterInfo: Entering");

        if(LOCATION_SECONDARY_UNCONFIRMED == FrameworkToolKit::getThisLocationRole()) 
        {            
            ResourceId status = getClusterInfoForNodeInWaitForRejonState(pVcsFabMessagingContext);            
            pVcsFabMessagingContext->executeNextStep (status);
            return;
        }


        VcsClusterInfoMessage *pVcsClusterInfoMessage = dynamic_cast<VcsClusterInfoMessage *> (pVcsFabMessagingContext->getPMessage ());
        if (NULL == pVcsClusterInfoMessage)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::getClusterInfo: PMessage is NULL");
        
            pVcsFabMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);
            return;
        }

       string clusterFormationTime = "";

        VcsClusterManagedObject *pVcsClusterManagedObject = getVcsClusterManagedObject();
        if (NULL == pVcsClusterManagedObject)
        {
                pVcsClusterInfoMessage->setSwitchesInCluster (1); // only local switch - which must always be present
                pVcsClusterInfoMessage->setClusterCreated(0);

                WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabObjectManager::getClusterInfo: VCS Cluster not created");
                clusterFormationTime = "None";
                //return WAVE_MESSAGE_ERROR;
        }
        else
        {
          clusterFormationTime = pVcsClusterManagedObject->getClusterFormationTime ();
          pVcsClusterInfoMessage->setClusterCreated(1);
        }

        UI32 startDomainId      = pVcsClusterInfoMessage->getStartDomainId ();
        UI32 endDomainId        = pVcsClusterInfoMessage->getEndDomainId ();
        if (((int)endDomainId < (int)startDomainId) || ((int)startDomainId < 0) || ((int)endDomainId < 0))
        {
                pVcsClusterInfoMessage->setSwitchesInCluster (0);
                WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::getClusterInfo: end DomainId must be >= start DomainId both must be > 0");
         
                pVcsFabMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);
                if( NULL != pVcsClusterManagedObject ) 
                {
                    delete (pVcsClusterManagedObject );
                }
                return;
        }

         //Get the status of the cluster managed object 
        if(pVcsClusterManagedObject) 
        {
            pVcsClusterInfoMessage->setClusterGenericStatus(pVcsClusterManagedObject->getGenericStatus());
            pVcsClusterInfoMessage->setClusterSpecificStatus(pVcsClusterManagedObject->getSpecificStatus());                                    
        }

        vector<WaveManagedObject *> *pResultsClusterInfo = fabOMDoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
        UI32 nodesInCluster = pResultsClusterInfo->size ();

        pVcsClusterInfoMessage->setSwitchesInCluster (nodesInCluster);
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;
        UI32 i;
        UI32 nodesListed =  0;
        string ipAddress = "";

        WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabObjectManager::getClusterInfo: Getting information for : ")+ nodesInCluster + " switches");

        for (i = 0; i < nodesInCluster; i++)
        {
            pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[i]);

            if ((0 == startDomainId) || ((pVcsNodeLocalManagedObject->getDomainId () >= startDomainId) && (pVcsNodeLocalManagedObject->getDomainId () <= endDomainId)))
            {
                pVcsClusterInfoMessage->addVcsMode (pVcsNodeLocalManagedObject->getVcsMode());
                pVcsClusterInfoMessage->addDomainId (pVcsNodeLocalManagedObject->getDomainId ());
                pVcsClusterInfoMessage->addSerialNumber (pVcsNodeLocalManagedObject->getSerialNumber ());

                pVcsClusterInfoMessage->addFcfMac (pVcsNodeLocalManagedObject->getFcfMac ());
                pVcsClusterInfoMessage->addSwitchMac (pVcsNodeLocalManagedObject->getSwitchMac ());
                pVcsClusterInfoMessage->addInVcs (pVcsNodeLocalManagedObject->getInVcs ());
                pVcsClusterInfoMessage->addIsPrincipal (pVcsNodeLocalManagedObject->getIsPrincipal ());
                pVcsClusterInfoMessage->addVcsId (pVcsNodeLocalManagedObject->getVcsId ());
                if (validDomainId (pVcsNodeLocalManagedObject->getDomainId ()))
                {
                    pVcsClusterInfoMessage->addIpAddress (getIpAddressFromDomainId (pVcsNodeLocalManagedObject->getDomainId ()));
                }
                else
                {
                    WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::getClusterInfo: domainId is invalid so internal ipAddress is not set");
                    pVcsClusterInfoMessage->addIpAddress ("Not set");
                }
                pVcsClusterInfoMessage->addPublicIpAddress (pVcsNodeLocalManagedObject->getPublicIpAddress ());                        
                pVcsClusterInfoMessage->addSpecificStatus (pVcsNodeLocalManagedObject->getSpecificStatus());
                pVcsClusterInfoMessage->addGenericStatus  (pVcsNodeLocalManagedObject->getGenericStatus());

                pVcsClusterInfoMessage->addLocationId (pVcsNodeLocalManagedObject->getOwnerLocationId ());

                nodesListed++;
            }
        }

        pVcsClusterInfoMessage->setSwitchesInCluster (nodesListed);
        pVcsClusterInfoMessage->addClusterFormTime (clusterFormationTime);
        vector<LocationId> connectedLocationsVector;
        FrameworkToolKit::getConnectedLocations(connectedLocationsVector);
        UI32 numberOfConnectedNodes = connectedLocationsVector.size();
        if(nodesListed>0) 
        {
           pVcsClusterInfoMessage->setNumberOfDisconnectedSwitches(nodesListed-1-numberOfConnectedNodes);
        }
        
        
        deleteObjects (pResultsClusterInfo);
        delete pVcsClusterManagedObject;
    
        pVcsFabMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
}

bool VcsFabObjectManager::inFailedList (VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject, vector<LocationId> failedLocationIds)
{
    LocationId locationId = pVcsNodeLocalManagedObject->getLocationId ();

    UI32 i;
    for (i = 0; i < failedLocationIds.size (); i++)
    {
        trace (TRACE_LEVEL_DEVEL, string ("VcsFabObjectManager::inFailedList: failed location id = ")+ failedLocationIds[i]+ " nodeLocationId = "+locationId);
        if (locationId == failedLocationIds[i])
        {
            return true;
        }
    }
    return false;
}

ResourceId VcsFabObjectManager::deleteClusterManagedObject ()
{
    ResourceId status =  deleteVcsClusterManagedObject (NULL);
    if (WAVE_MESSAGE_ERROR == status)
    {
       trace (TRACE_LEVEL_ERROR, string ("VcsFabObjectManager::deleteClusterManagedObject: Error deleting VcsClusterMO"));
       return FRAMEWORK_ERROR;
    }
    trace (TRACE_LEVEL_SUCCESS, string ("VcsFabObjectManager::deleteClusterManagedObject: successfully deleted both VcsClusterMO and SNDBMO"));
    return FRAMEWORK_SUCCESS;
}

void VcsFabObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
    vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();
    bool                                    isPrincipalChangedWithThisFailover  = pFailoverAsynchronousContext->getIsPrincipalChangedWithThisFailover ();

    trace (TRACE_LEVEL_DEBUG, string ("VcsFabObjectManager::failover process : Entry"));

    if (true == isPrincipalChangedWithThisFailover)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::failover : isPrincipalChangedWithThisFailover = true"));
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::failover : isPrincipalChangedWithThisFailover = false"));
    }
    
    vector<WaveManagedObject *> *pResultsLocalNodes = fabOMDoSynchronousQuery (VcsNodeLocalManagedObject::getClassName());
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;
    UI32 myDomainId = getMyDomainId ();
    UI32 nSwitches = pResultsLocalNodes->size ();
    UI32 numToRemove = failedLocationIds.size ();
    UI32 removeCount = 0;
    bool isPrincipal = false;
    VcsFabricLocalObjectManager* pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance ();

    if (! validDomainId (myDomainId))
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsFabObjectManager::failover: invalid myDomainId returned, myDomainId = ")+myDomainId);
        deleteObjects (pResultsLocalNodes);
        return;
    }

    if ((getInstance()->m_becomePrincipalFlag))
    {
        trace(TRACE_LEVEL_INFO, string("VcsFabObjectManager::failover become principal = ") + getInstance()->m_becomePrincipalFlag);
        setPrincipalFlag (true);
        // Reset m_becomePrincipalFlag to false since this node is now the principal
        setBecomePrincipalFlag(false);
        isPrincipal = true;
        
    }
    else
    {
        isPrincipal = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectIsPrincipal ();
    }

    LocationRole thisLocationRole = FrameworkToolKit::getThisLocationRole ();

    if (LOCATION_SECONDARY != thisLocationRole)
    {
        isPrincipal = true;
    }

    bool clusterDeleted = false;

    if (LOCATION_STAND_ALONE == thisLocationRole)
    {
        clusterDeleted = true;
    }

    ResourceId status = WAVE_MESSAGE_ERROR;
    ObjectId objectId;
    bool myVcsMode = false;
    UI32 i;
    // Remove the VcsCluster Managed Objects 
    switch (failoverReason)
    {
        case FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED:
        {

            VcsClusterManagedObject* pVcsClusterManagedObject =  NULL ;
            if (isPrincipal)
            {
                trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::failover: Node is principal # in locationId list = ")+numToRemove);
                if (VcsFabricLocalObjectManager::getInstance ()->m_noVcsEnablePrincipalFlag) 
                // principal is being removed from cluster
                {
                   // for the principal a no vcs enable already deletes the cluster so there are no SNDB or VcsCluster MO, nothing to do here just a place holder
                   trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::failover: No Vcs Enable Processing :Failover for Principal VcsClusterMO should already be removed, domainId = ")+myDomainId);
                }
                else
                {
                    if (LOCATION_STAND_ALONE == thisLocationRole)
                    {
                        status = deleteClusterManagedObject ();
                        pVcsClusterManagedObject = NULL;
                        
                        // update the specific status to standalone as cluster has been deleted and locationRole is standalone.
                        startTransaction ();
                        pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectStatus (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD, VCS_FABRIC_LOCAL_SPECIFIC_STATUS_STAND_ALONE);
                        updateWaveManagedObject(pVcsFabricLocalObjectManager->m_pVcsNodeLocalManagedObject );
                        status = commitTransaction ();
                        if (FRAMEWORK_SUCCESS != status)
                        {
                            trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::failover : Could not update the specific status of a local node to standalone.");
                        }
                        else
                        {
                            trace (TRACE_LEVEL_INFO, "VcsFabObjectManager::failover : updated the specific status of a local node to standalone.");
                        }
                    }
                    trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::failover:Not a case of NoVcsEnable:  domainId = ")+myDomainId);
                }
            }
            else 
            {
                if (LOCATION_STAND_ALONE == FrameworkToolKit::getThisLocationRole())
                {
                    status = deleteClusterManagedObject ();

                    if (FRAMEWORK_SUCCESS != status)
                    {
                        trace (TRACE_LEVEL_ERROR, string ("VcsFabObjectManager::failover: error deleting VcsClusterMO on secondary switch"));
                    
                        UI32 myCount = 0;

                        startTransaction ();

                        for (i = 0; i < nSwitches; i++)
                        {
                            pVcsNodeLocalManagedObject =  dynamic_cast <VcsNodeLocalManagedObject *> ((*pResultsLocalNodes)[i]);

                            if ((pVcsNodeLocalManagedObject->getDomainId () != myDomainId) && (inFailedList (pVcsNodeLocalManagedObject, failedLocationIds)))
                            {
                                string filename("");
                                VcsFabConfigSnapshotWorker::getRestoreTriggeredConfigSnapshotFilenameForDomainId ( pVcsNodeLocalManagedObject->getDomainId (), filename );

                                if ( true == filename.empty() )
                                {
                                    //restore flag is not set. Delete all files for the failed domain and sync
                                    m_pVcsFabConfigSnapshotWorker->deleteAllConfigSnapshotFilesForDomainId( pVcsNodeLocalManagedObject->getDomainId (), true);
                                }
                                delete pVcsNodeLocalManagedObject;
                            }
                            else
                            {
                                myCount = i;
                            }
                        }

                        status = commitTransaction ();

                        if (FRAMEWORK_SUCCESS != status)
                        {
                            trace (TRACE_LEVEL_FATAL, "VcsFabObjectManager::failover Not Able to Delete VcsNodeLocalMO When Cluster MO doesnot exist in DB but VcsNodeLocalMO exist for other nodes");
                            prismAssert (false, __FILE__, __LINE__);
                        }
                        else
                        {
                            trace (TRACE_LEVEL_INFO, "VcsFabObjectManager::failover clean up the vcs node local managed object for other nodes");
                        }

                        delete ((*pResultsLocalNodes)[myCount]);

                        delete pResultsLocalNodes;

                        pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_ERROR);
                        pFailoverAsynchronousContext->callback ();
                        return;
                    }
                }
            }

            vector<string> principalPublicIpAddress = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectPublicIpAddress ();
            pVcsClusterManagedObject = getVcsClusterManagedObject ();
            startTransaction ();
    
            for (i = 0; i < nSwitches; i++)
            {
                pVcsNodeLocalManagedObject =  dynamic_cast <VcsNodeLocalManagedObject *> ((*pResultsLocalNodes)[i]);
                if (pVcsNodeLocalManagedObject->getDomainId () != myDomainId)
                {
                    if ((!isPrincipal) && (inFailedList (pVcsNodeLocalManagedObject, failedLocationIds)))
                    {
                        trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::failover: Secondary deleting node domainId = ") + pVcsNodeLocalManagedObject->getDomainId ());

                        string filename("");
                        string filenameOnCurrentNode("");

                        VcsFabConfigSnapshotWorker::getRestoreTriggeredConfigSnapshotFilenameForDomainId ( pVcsNodeLocalManagedObject->getDomainId (), filename );
                        VcsFabConfigSnapshotWorker::getRestoreTriggeredConfigSnapshotFilenameForDomainId ( myDomainId, filenameOnCurrentNode );

                        //Check if restore is happening on current node or on the disconnected node
                        //If the failover is because of restore on current/disconnected node, dont delete the snapshot files, else delete the snapshot for disconnected node
                        if ( true == filenameOnCurrentNode.empty() && true == filename.empty() )
                        {   
                            //restore flag is not set. Delete all files for failed domain and sync
                            m_pVcsFabConfigSnapshotWorker->deleteAllConfigSnapshotFilesForDomainId( pVcsNodeLocalManagedObject->getDomainId (), true );
                        }

                        delete pVcsNodeLocalManagedObject;
                        (*pResultsLocalNodes)[i] = NULL;
                        removeCount++;
                    }
                    else
                    {
                        if (inFailedList (pVcsNodeLocalManagedObject, failedLocationIds))
                        {
                            trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::failover: Principal deleting node domainId = ") + pVcsNodeLocalManagedObject->getDomainId ());

                            if(pVcsClusterManagedObject) 
                            {
                                updateWaveManagedObject(pVcsClusterManagedObject);
                                pVcsClusterManagedObject->removeSwitchObjectId(pVcsNodeLocalManagedObject->getObjectId ());
                            }

                            // Add cluster wide RASLOG for node remove

                            vector<string> switchPublicIpAddress = pVcsNodeLocalManagedObject->getPublicIpAddress ();
                            UI32 vcsId = pVcsNodeLocalManagedObject->getVcsId ();
                            UI32 domainId = pVcsNodeLocalManagedObject->getDomainId ();

                            string filename("");
                            string filenameOnCurrentNode("");
 
                            VcsFabConfigSnapshotWorker::getRestoreTriggeredConfigSnapshotFilenameForDomainId ( pVcsNodeLocalManagedObject->getDomainId (), filename );
                            VcsFabConfigSnapshotWorker::getRestoreTriggeredConfigSnapshotFilenameForDomainId ( myDomainId, filenameOnCurrentNode );
                                
                            //Check if restore is happening on current node or on the disconnected node
                            //If the failover is because of restore on current/disconnected node, dont delete the snapshot files, else delete the snapshot for disconnected node
                            if ( true == filenameOnCurrentNode.empty() && true == filename.empty() )
                            {   
                                //restore flag is not set. Delete all files for failed domain and sync
                                m_pVcsFabConfigSnapshotWorker->deleteAllConfigSnapshotFilesForDomainId( pVcsNodeLocalManagedObject->getDomainId (), true );
                            }

                            if ( ( VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && ( DCM_CCM == DcmToolKit::getDistributionMode ()) )
                            {    
                                raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, VCS_1007, principalPublicIpAddress[0].c_str (), vcsId, domainId, switchPublicIpAddress[0].c_str ());
                            }

                            delete pVcsNodeLocalManagedObject;
                            (*pResultsLocalNodes)[i] = NULL;    
                            removeCount++;
                        }
                    }
                }
                else // local switch
                {
                    trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::failover: updating local node object for self, myDomainId = ")+myDomainId);

                    if (isPrincipal && (clusterDeleted)) 
                    {
                        if (true == VcsFabricLocalObjectManager::getInstance ()->m_noVcsEnablePrincipalFlag)
                        {
                            trace(TRACE_LEVEL_INFO, "VcsFabObjectManager::failover: VcsFabricLocalObjectManager::m_noVcsEnablePrincipalFlag is true.");
                        }

                        trace(TRACE_LEVEL_INFO,"VcsFabObjectManager::failover:IsPrincipal And Cluster Deleted: setting IsPrincipal to false");
                        pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectIsPrincipal (false); 
                        pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectInVcs (false);
                    }
                    else if (! isPrincipal)
                    {
                       trace(TRACE_LEVEL_INFO,"VcsFabObjectManager::failover:setting only InVcs to false");
                       pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectInVcs (false);
                    }
                    else
                    {
                        trace (TRACE_LEVEL_INFO, "VcsFabObjectManager::failover: isPrincipal = true, clusterDeleted = false");
                    }

                    updateWaveManagedObject(pVcsFabricLocalObjectManager->m_pVcsNodeLocalManagedObject );
                }
            }

            status = commitTransaction ();

            // update the cluster MO status
            trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::failover: Updating the cluster MO status"));
            updateClusterStatus();

            // resync the in memory object with what's in the DB after the update
            myVcsMode = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectVcsMode ();

            // only delete objects not already deleted viz the nodes not in failed list
            deleteObjects (pResultsLocalNodes);

            if (NULL != pVcsClusterManagedObject)
            {
                delete pVcsClusterManagedObject;
            }

            if (FRAMEWORK_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "VcsFabObjectManager::failover: Error deleting LocalNode managed objects from DB" + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::failover : Number of LocalNode managed objects successfully removed = ") + removeCount);
                //  handle case where node is removed as part of "no vcs enable" processing
                if (VcsFabricLocalObjectManager::getInstance ()->m_noVcsEnableInProgressFlag)
                {
                    // remove all config snapshots
                    trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::failover: No Vcs Enable Processing :removing all snapshots, domainId = ")+myDomainId);
                    m_pVcsFabConfigSnapshotWorker->deleteAllConfigSnapshotFiles ();

                    VcsFabricLocalObjectManager::getInstance ()->copyDefaultToStartAtLocalNode (myVcsMode);
                }
            }

            break;
        }  // end case FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED

     case FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED:
     {
            bool isUnconfirmedLocationRole = ((thisLocationRole == LOCATION_SECONDARY_UNCONFIRMED)  || (thisLocationRole == LOCATION_PRIMARY_UNCONFIRMED));

            if (LOCATION_PRIMARY == thisLocationRole || true == isUnconfirmedLocationRole)
            {
                trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::uncontrol_failover: Node is principal , number of remove nodes = ")+numToRemove);
                        /* Update the status for the failed cluster node - Optimization can be done by commit once time and remove commit from deActivateSwitchDomainIdFromSNDB  */
                VcsClusterManagedObject *pVcsClusterManagedObject = getVcsClusterManagedObject ();

                vector<string> principalPublicIpAddress = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectPublicIpAddress ();
      
                for (i = 0; i < nSwitches; i++)
                {
                  pVcsNodeLocalManagedObject =  dynamic_cast <VcsNodeLocalManagedObject *> ((*pResultsLocalNodes)[i]);
                  if (inFailedList (pVcsNodeLocalManagedObject, failedLocationIds))
                  {

                       /* We just need to update database only */
                        trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::uncontrol_failover: Principal update node domainId = ")+pVcsNodeLocalManagedObject->getDomainId ());
                        startTransaction ();
                        pVcsNodeLocalManagedObject->setInVcs(false);
                        pVcsNodeLocalManagedObject->setIsPrincipal(false);
                        //pVcsNodeLocalManagedObject->setGenericStatus(WAVE_MANAGED_OBJECT_GENERIC_STATUS_FAILED);
                        pVcsNodeLocalManagedObject->setGenericStatus(WAVE_MANAGED_OBJECT_GENERIC_STATUS_OFFLINE);
                        pVcsNodeLocalManagedObject->setSpecificStatus (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_DISCONNECTED_FROM_CLUSTER);

                        //Update the cluster managed object status
                        if(pVcsClusterManagedObject) 
                        {
                          pVcsClusterManagedObject->setGenericStatus(WAVE_MANAGED_OBJECT_GENERIC_STATUS_DEGRADED);
                          pVcsClusterManagedObject->setSpecificStatus(VCS_CLUSTER_SPECIFIC_STATUS_NODES_DISCONNECTED_FROM_CLUSTER);
                          updateWaveManagedObject(pVcsClusterManagedObject);

                        }
                        updateWaveManagedObject(pVcsNodeLocalManagedObject);
                        status = commitTransaction ();
                        
                        if(pVcsClusterManagedObject)
                        {
                           delete pVcsClusterManagedObject;
                           pVcsClusterManagedObject = 0;
                        }


                        // Add cluster wide RASLOG for node disconnect

                        vector<string> switchPublicIpAddress = pVcsNodeLocalManagedObject->getPublicIpAddress ();
                        UI32 vcsId = pVcsNodeLocalManagedObject->getVcsId ();
                        UI32 domainId = pVcsNodeLocalManagedObject->getDomainId ();

                        if ( ( VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && ( DCM_CCM == DcmToolKit::getDistributionMode ()) )
                        {
                            raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, VCS_1009, principalPublicIpAddress[0].c_str (), vcsId, domainId, switchPublicIpAddress[0].c_str ());
                        }
                    }
                }

                if(pVcsClusterManagedObject)
                {
                    delete pVcsClusterManagedObject;
                    pVcsClusterManagedObject = 0;
                }

            }
            else 
            {
                trace (TRACE_LEVEL_ERROR, string ("VcsFabObjectManager::uncontrol_failover: Node is not principal myDomainId = ")+myDomainId+ " # local nodes to update status = "+ (nSwitches-1));
            }

        // Delete the query result
        deleteObjects(pResultsLocalNodes);
        break;
    }
        default:
                prismAssert (false, __FILE__, __LINE__);
        break;
    }

    pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pFailoverAsynchronousContext->callback ();
}

ResourceId VcsFabObjectManager::deleteVcsClusterManagedObject (VcsFabMessagingContext *pMessagingContext)
{
        trace (TRACE_LEVEL_DEBUG, string("VcsFabObjectManager::deleteVcsClusterManagedObject : Entering"));

        VcsClusterManagedObject *pVcsClusterManagedObject = getVcsClusterManagedObject ();

    if (NULL == pVcsClusterManagedObject)
    {
        trace (TRACE_LEVEL_ERROR, string("VcsFabObjectManager::deleteVcsClusterManagedObject : VCS cluster not in database"));
                return WAVE_MESSAGE_ERROR;
    }
    else
    {
                ResourceId status = FRAMEWORK_ERROR;
        startTransaction ();

        delete pVcsClusterManagedObject ;
        clearClusterGUID();

        status = commitTransaction ();

        if (FRAMEWORK_SUCCESS == status)
        {
            trace (TRACE_LEVEL_SUCCESS, "VcsFabObjectManager::deleteVcsClusterManagedObject : Successfully deleted VCS cluster from Database.");
                        return WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::deleteVcsClusterManagedObject : Failed to delete VCS cluster from Database.");
            return WAVE_MESSAGE_ERROR;
        }
    }
}

bool VcsFabObjectManager::vcsClusterMOExists ()
{
    VcsClusterManagedObject *pVcsClusterManagedObject = getVcsClusterManagedObject ();
        if (NULL != pVcsClusterManagedObject)
        {
                delete pVcsClusterManagedObject;
                return true;
        }
        else
        {
                return false;
        }
}

Uuid VcsFabObjectManager::getClusterGUID ()
{
	return m_clusterGUID;
}

void VcsFabObjectManager::setClusterGUID (const Uuid &clusterGUID)
{
	m_clusterGUID = clusterGUID;
}

void VcsFabObjectManager::clearClusterGUID ()
{
	m_clusterGUID.clear();
}

void VcsFabObjectManager::updateClusterStatus()
{

  trace(TRACE_LEVEL_INFO, "VcsFabObjectManager::updateClusterStatus: entering..");
  VcsClusterManagedObject *pVcsClusterManagedObject = getVcsClusterManagedObject();

  if(!pVcsClusterManagedObject) 
  {
      return;
  }
  vector<LocationId> connectedLocationsVector;
  FrameworkToolKit::getConnectedLocations(connectedLocationsVector);
  UI32 numberOfConnectedNodes = connectedLocationsVector.size ();

  tracePrintf(TRACE_LEVEL_INFO, "VcsFabObjectManager::updateClusterStatus: Number of connected Locations: %d",numberOfConnectedNodes);
  

   vector<WaveManagedObject *> *pResults = fabOMDoSynchronousQuery (VcsNodeLocalManagedObject::getClassName());

   UI32 numNodesInTheDatabase = 0;

   if(pResults) 
   {
     numNodesInTheDatabase = pResults->size();
     WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
     tracePrintf(TRACE_LEVEL_INFO, "VcsFabObjectManager::updateClusterStatus: Number of Nodes in the DB: %d",numNodesInTheDatabase);

   }
   if((numberOfConnectedNodes == (numNodesInTheDatabase-1))) 
   {

       if(!(pVcsClusterManagedObject->getGenericStatus() == WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD && pVcsClusterManagedObject->getSpecificStatus() == VCS_CLUSTER_SPECIFIC_STATUS_ALL_NODES_IN_CLUSTER)) 
       {       
        startTransaction();

        pVcsClusterManagedObject->setGenericStatus (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD);
        pVcsClusterManagedObject->setSpecificStatus(VCS_CLUSTER_SPECIFIC_STATUS_ALL_NODES_IN_CLUSTER);
        updateWaveManagedObject(pVcsClusterManagedObject);
        ResourceId status = commitTransaction();
        
        if (FRAMEWORK_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::updateClusterStatus : commitTransaction failed.");
            prismAssert (false, __FILE__, __LINE__);
        }

       }
   }



  delete pVcsClusterManagedObject;
  return;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DCM Message Handlers ////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void VcsFabObjectManager::makePrincipalMessageHandler (VcsMakePrincipalMessage *pMakePrincipalMessage)
{
    trace (TRACE_LEVEL_DEBUG, "VcsFabObjectManager::VcsMakePrincipalMessageHandler : Entering ...");
    trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::VcsMakePrincipalMessageHandler : This interface is not supported");
    pMakePrincipalMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
    reply (pMakePrincipalMessage);
    return;
    /*
        const UI32 domainId = pMakePrincipalMessage->GetSwitchDomainId ();
    // retrieve swithc object with this domainId and set isPrincipal = true

    trace (TRACE_LEVEL_INFO, string("VcsFabObjectManager::makePrincipalMessageHandler principal domainId = ")+ domainId);
    pMakePrincipalMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMakePrincipalMessage);
    */
}

void VcsFabObjectManager::clusterInfoMessageHandler (VcsClusterInfoMessage *pClusterInfoMessage)
{
    trace (TRACE_LEVEL_DEBUG, "VcsFabObjectManager::clusterInfoMessageHandler : Entering ...");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabObjectManager::getClusterInfo),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabObjectManager::prismLinearSequencerFailedStep)
    };

    VcsFabMessagingContext *pVcsFabMessagingContext = new VcsFabMessagingContext (reinterpret_cast<PrismMessage *> (pClusterInfoMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pVcsFabMessagingContext->setPMessage (pClusterInfoMessage);
    pVcsFabMessagingContext->start();
}

void VcsFabObjectManager::printNodeSpecificStatus (ResourceId nodeStatus)
{
        switch (nodeStatus)
        {
                case WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_STAND_ALONE:
                        cout.width (20);        cout    << left <<      "Standalone";
                        break;
                case WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_PRIMARY:
                        cout.width (20);        cout    << left <<      "Principal";
                        break;
                case WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_SECONDARY_CONNECTED_TO_CLUSTER:
                        cout.width (20);        cout    << left <<      "Secondary-InCluster";
                        break;
                case WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_SECONDARY_NOT_CONNECTED_TO_CLUSTER:
                        cout.width (20);        cout    << left <<      "Secondary-NotConnected";
                        break;
                default:
                        cout.width (20);        cout    << left <<      "Not Available";
        }
}

void VcsFabObjectManager::printNodeGenericStatus (ResourceId nodeStatus)
{
        switch (nodeStatus)
        {
        case WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD:
                cout.width (20);        cout    << left <<      "Status: Good";
                break;
        case WAVE_MANAGED_OBJECT_GENERIC_STATUS_DEGRADED:
                cout.width (20);        cout    << left <<      "Status: Degraded";
                break;
        case WAVE_MANAGED_OBJECT_GENERIC_STATUS_FAILED:
                cout.width (20);        cout    << left <<      "Status: Failed";
                break;
        case WAVE_MANAGED_OBJECT_GENERIC_STATUS_OFFLINE:
                cout.width (20);        cout    << left <<      "Status: Offline";
                break;
        default:
                cout.width (20);        cout    << left <<      "Not Available";
        }
}

extern "C" 
{
// This is a wrapper function so it can be called from a 'c' module getid.h without having to include the vcsfabobjectmanager.h file
UI32 getMyDomainIdFromVcsFabObjectManager()
{
    return VcsFabObjectManager::getInstance()->getMyDomainId ();
}
}


UI32 VcsFabObjectManager::getMyDomainId ()
{
        UI32 myDomainId;
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = getNodeLocalManagedObjectForThisNode ();
        if (NULL == pVcsNodeLocalManagedObject)
        {
        prismAssert (false, __FILE__, __LINE__);
        return 0;
        }
        else
        {
                myDomainId = pVcsNodeLocalManagedObject->getDomainId ();
        if (! validDomainId (myDomainId))
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabObjectManager::getMyDomainId: VcsNodeLocalManagedObject retruned invalid myDomainId = ")+myDomainId);
        }
                delete pVcsNodeLocalManagedObject;
                return myDomainId;
        }
}



void VcsFabObjectManager::setSwitchInfoRetrievedBit (const u_short &infoRetrievedDomain)
{
        m_knownDomainsList.switchInfoReceivedBitMap.set (infoRetrievedDomain);
}

UI32 VcsFabObjectManager::setPrincipalFlag (bool principalFlag)
{
        UI32 localSwitchDomainId = getMyDomainId ();
        VcsNodeSetIsPrincipalMessage *pSetIsPrincipalMessage = new VcsNodeSetIsPrincipalMessage (principalFlag);
        ResourceId status = sendSynchronously (pSetIsPrincipalMessage);
        if (WAVE_MESSAGE_ERROR == status)
        {
            trace(TRACE_LEVEL_ERROR, string("VcsFabObjectManager::setPrincipalFlag: error sending setIsPrincipal message ")+ FrameworkToolKit::localize (status));
            delete pSetIsPrincipalMessage;
            return GENERIC_ERROR;
        }
        else
        {
            trace(TRACE_LEVEL_INFO, string("VcsFabObjectManager::setPrincipalFlag: isPrincipal set for switch with domainId = ")+ localSwitchDomainId);
        }

        delete pSetIsPrincipalMessage;
        return VCS_SUCCESS;
}

void VcsFabObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), BOOT_COMPLETE_FOR_THIS_LOCATION, reinterpret_cast<PrismEventHandler> (&VcsFabObjectManager::bootCompleteEventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

UI32 VcsFabObjectManager::wereAllSwitchesAdded (BitMap &switchesNotInCluster)
{
        BitMap domainsInCluster;
        setBitMapSize (domainsInCluster);
        UI32 numNodes = getDomainsInCluster (domainsInCluster);

        UI32 i;
        UI32 nNotInCluster = 0;
    switchesNotInCluster.reset ();

        for (i = 1; i < (max_domain_list - 1); i++)
        {
                if (m_knownDomainsList.dcaDomainsBitMap[i])
                {
                        if (! domainsInCluster[i])
                        {
                                switchesNotInCluster.set (i);
                                nNotInCluster++;
                        }
                }
        }
        
        printf ("wereAllSwitchesAdded: Num switches in cluster = %d Num Not in cluster = %d\n\r",numNodes,nNotInCluster);
        return nNotInCluster;
}

void VcsFabObjectManager::bootCompleteEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent)
{

        static UI32 numberOfBootCompleteEvents = 0;
        trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::bootCompleteEventHandler: BootComplete event received count = ")+numberOfBootCompleteEvents);
        numberOfBootCompleteEvents++;
        m_bootComplete = true;
        setSizeForDomainBitMaps (m_knownDomainsList);
        reply (reinterpret_cast<const PrismEvent *&> (pBootCompleteForThisLocationEvent));
}

void VcsFabObjectManager::disable (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEBUG, "VcsFabObjectManager::disable : Entering ...");

        if (m_timerRunningFlag)
        {
                if (FRAMEWORK_SUCCESS != deleteTimer (VcsFabObjectManager::m_timerHandle))
                {
                        trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::disable: delete timer FAILED.");
                        return;
                }
                else
                {
                        trace (TRACE_LEVEL_SUCCESS, "VcsFabObjectManager::disable: delete timer SUCCEEDED.");
                        m_timerRunningFlag = false;
                }
        }
        else
        {
                        trace (TRACE_LEVEL_INFO, "VcsFabObjectManager::disable: delete timer not running.");
        }
    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void VcsFabObjectManager::shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    
    trace (TRACE_LEVEL_DEBUG, "VcsFabObjectManager::shutdown : Entering ...");

        if (m_timerRunningFlag)
        {
                if (FRAMEWORK_SUCCESS != deleteTimer (VcsFabObjectManager::m_timerHandle))
                {
                        trace (TRACE_LEVEL_ERROR, "VcsFabObjectManager::shutdown: delete timer FAILED.");
                        return;
                }
                else
                {
                        trace (TRACE_LEVEL_SUCCESS, "VcsFabObjectManager::shutdown delete timer SUCCEEDED.");
                        m_timerRunningFlag = false;
                }
        }
        else
        {
                        trace (TRACE_LEVEL_INFO, "VcsFabObjectManager::shutdown: delete timer not running.");
        }
    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

// TODO : Handle hainstall for global services ?

void VcsFabObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::install : WaveBootReason [") + FrameworkToolKit::localizeToSourceCodeEnum (bootReason) + string ("]"));

    if ((WAVE_BOOT_FIRST_TIME_BOOT == bootReason) || (WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason))
    {
        ResourceId currentDistributionMode = DcmToolKit::getDistributionMode ();

        if ((DCM_ONLY == currentDistributionMode) || (DCM_CCM == currentDistributionMode))
        {
            registerFeatureGroupsAndXPaths ();
        }
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

// Note about boot and haboot -
//     In case of firmware download, 'previous firmware' / 'firmware on active' may have different set of XPaths.
//     Install may not be called, if schema is same.
//     Only in such cases, all existing registrations may be deleted, and new set of XPaths (from current version) be registered.
//
//     i.e. Need to detect if 'WAVE_BOOT_PERSISTENT_BOOT/WAVE_BOOT_HASTANDBY_BOOT' is after getting database from a 'different firmware version'
//
//     However, currently, keeping old XPaths, and adding new.
//     Duplicate registrations are ignored by Wave Framework. So, registerFeatureGroupsAndXPaths can be called even if same set of XPath is present.

void VcsFabObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::boot : WaveBootReason [") + FrameworkToolKit::localizeToSourceCodeEnum (bootReason) + string ("]"));

    if (WAVE_BOOT_PERSISTENT_BOOT == bootReason)
    {
        ResourceId currentDistributionMode = DcmToolKit::getDistributionMode ();

        if ((DCM_ONLY == currentDistributionMode) || (DCM_CCM == currentDistributionMode))
        {
            registerFeatureGroupsAndXPaths ();
        }
    }
    else if (WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason)
    {
        // ResourceId currentDistributionMode = DcmToolKit::getDistributionMode ();
        // if ((DCM_CCM == currentDistributionMode) && (true == DcmToolKit::isVcsClusterEnabled ()))
        {
            PersistenceObjectManagerResetXPathStringsTimestampsMessage persistenceObjectManagerResetXPathStringsTimestampsMessage;

            ResourceId sendStatus = sendSynchronously (&persistenceObjectManagerResetXPathStringsTimestampsMessage);

            if (WAVE_MESSAGE_SUCCESS != sendStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("VcsFabObjectManager::boot : could not send PersistenceObjectManagerResetXPathStringsTimestampsMessage. sendStatus [") + (FrameworkToolKit::localizeToSourceCodeEnum (sendStatus)) + string ("]"));
            }
            else
            {
                ResourceId completionStatus = persistenceObjectManagerResetXPathStringsTimestampsMessage.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != completionStatus)
                {
                    trace (TRACE_LEVEL_ERROR, string ("VcsFabObjectManager::boot : PersistenceObjectManagerResetXPathStringsTimestampsMessage processing failed. completionStatus [") + (FrameworkToolKit::localizeToSourceCodeEnum (completionStatus)) + string ("]"));
                }
                else
                {
                    trace (TRACE_LEVEL_SUCCESS, string ("VcsFabObjectManager::boot : PersistenceObjectManagerResetXPathStringsTimestampsMessage processing is successful."));
                }
            }
        }
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VcsFabObjectManager::haboot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    trace (TRACE_LEVEL_INFO, string ("VcsFabObjectManager::haboot : WaveBootReason [") + FrameworkToolKit::localizeToSourceCodeEnum (bootReason) + string ("]"));

    if (WAVE_BOOT_HASTANDBY_BOOT == bootReason)
    {
        ResourceId currentDistributionMode = DcmToolKit::getDistributionMode ();

        if ((DCM_ONLY == currentDistributionMode) || (DCM_CCM == currentDistributionMode))
        {
            registerFeatureGroupsAndXPaths ();
        }
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VcsFabObjectManager::registerFeatureGroupsAndXPaths ()
{
    vector<string> xPathStrings;

    // Special XPath used for "get-last-config-update-time"
    xPathStrings.push_back ("/");

    // XPaths for Netconf RPC "get-last-config-update-time-for-xpaths"
    xPathStrings.push_back ("/cee-map");
    xPathStrings.push_back ("/interface/fortygigabitethernet/channel-group");
    xPathStrings.push_back ("/interface/gigabitethernet/channel-group");
    xPathStrings.push_back ("/interface/tengigabitethernet/channel-group");
    xPathStrings.push_back ("/fcoe/fcoe-fabric-map");
    xPathStrings.push_back ("/interface-vlan/interface/ve");
    xPathStrings.push_back ("/interface/fc-port");
    xPathStrings.push_back ("/interface/fortygigabitethernet");
    xPathStrings.push_back ("/interface/gigabitethernet");
    xPathStrings.push_back ("/interface/tengigabitethernet");
    xPathStrings.push_back ("/snmp-server/agtconfig");
    xPathStrings.push_back ("/port-profile");
    xPathStrings.push_back ("/port-profile-global/port-profile");
    xPathStrings.push_back ("/mac/access-list");
    xPathStrings.push_back ("/interface/port-channel");
    xPathStrings.push_back ("/interface/fortygigabitethernet/switchport");
    xPathStrings.push_back ("/interface/fortygigabitethernet/vlan");
    xPathStrings.push_back ("/interface/gigabitethernet/switchport");
    xPathStrings.push_back ("/interface/gigabitethernet/vlan");
    xPathStrings.push_back ("/interface/port-channel/switchport");
    xPathStrings.push_back ("/interface/port-channel/vlan");
    xPathStrings.push_back ("/interface/tengigabitethernet/switchport");
    xPathStrings.push_back ("/interface/tengigabitethernet/vlan");
    xPathStrings.push_back ("/interface-vlan");
    xPathStrings.push_back ("/interface-vlan/interface/vlan"); // Is this required ?
    xPathStrings.push_back ("/protocol/spanning-tree");
    xPathStrings.push_back ("/rbridge-id/interface/ve");

    ResourceId status = FrameworkToolKit::addXPathStrings (xPathStrings);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        tracePrintf (TRACE_LEVEL_INFO, "VcsFabObjectManager::registerFeatureGroupsAndXPaths : Registered [%u] XPaths with Framework.", xPathStrings.size ());
    }
    else
    {
        tracePrintf (TRACE_LEVEL_ERROR, "VcsFabObjectManager::registerFeatureGroupsAndXPaths : FrameworkToolKit::addXPathStrings returned [%s]", (FrameworkToolKit::localizeToSourceCodeEnum (status)).c_str ());
    }
}

}

