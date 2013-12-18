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

 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh                                               *
 ***************************************************************************/

#include <sstream>
#include "Framework/OsLayer/PrismOsLayer.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/ConfigFileManagementToolKit.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "VcsFabric/Local/VcsNodeFCPrincipalTrackingWorker.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosScnQueue.h"
#include "VcsFabric/Local/VcsEnqSwitchMessage.h"
#include "VcsFabric/Local/VcsAsyncClusterMergeMessage.h"
#include "Cluster/PrismCluster.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Cluster/Local/HeartBeat/HeartBeatObjectManager.h"
#include "Firmware/Global/FirmwareToolKit.h"
#include "Firmware/Global/NodeAutoUpgradeConfiguration.h"
#include "fabric/fabric.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "VirtualIp/Global/VirtualIpGlobalBindVipMessage.h"
#include "VirtualIp/Global/VirtualIpGlobalUnbindVipMessage.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Postboot/PrismPostPersistentBootMessages.h"
#include "Cluster/Local/ClusterLocalSetThisNodeIpAddressMessage.h"
#include "Framework/ObjectModel/WavePostBootEvent.h"
#include "VcsFabric/Local/VcsFabricLocalSerializableObject.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "VcsFabric/Local/VcsFabricLocalMessages.h"
#include "Framework/Core/PrismFrameworkReplayConfigMessage.h"
/*
 * ATTENTION:
 *	All new FOS includes should be added to this
 *	global header file.  This will help to ensure
 *	that libInit() has been called for any libraries
 *	that are required.
 */
#include "dcm/fos_headers.h"

extern "C"
{
module_t MODNAME(DCMVCS);
#define DCMLOG  &MODNAME(DCMVCS)
}
namespace DcmNs
{
    vector<UI32>     getx86DomainList      ();
    vector<string>   getx86ipAddresses     ();
    vector<UI32>     getx86ports           ();
    
    void             setx86DomainList       (vector<u_short> &x86DomainList);
    void             setx86ipAddresses      (vector<string> &x86iPAddress);
    void             setx86ports            (vector<UI32> &x86ports);
    
    void             addTox86DomainList         (UI32 &x86DomainId);
    void             addTox86IpAddressList  (string &x86IpAddress);
    void             addTox86PortList       (UI32 &x86Port);
    bool             isEnvx86               ();
    UI32             getMgmtPort                     (UI32 domainId);
    string           getIpAddressFromDomainId        (UI32 domainId);
    void             getx86ipAddressAndPort          (const UI32 &x86DomainId, string &x86ipAddress, UI32 &x86port);
    UI32             getx86DomainIdFromIpAddressAndMgmtPort (string ipAddress, UI32 mgmtPort);

    void            getKnownDomainList              (scnDomainsStruct &knownDomainList);
    bool            vcsModeEnabled                  ();

TimerHandle     VcsNodeFCPrincipalTrackingWorker::m_timerHandle;
bool            VcsNodeFCPrincipalTrackingWorker::m_timerRunningFlag;
    
const UI32            VcsNodeFCPrincipalTrackingWorker::PRINCIPAL_TRACKING_TIMEOUT_IN_MILLISECONDS_IN_UNCONFIRMED_STATE = 180000;

PrismMutex  VcsNodeFCPrincipalTrackingWorker::m_fabGetCosDomainListMutex;

static UI32 s_connectionSequenceNumber = 0;

VcsNodeFCPrincipalTrackingWorker::VcsNodeFCPrincipalTrackingWorker (VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager)
    : WaveWorker (pVcsFabricLocalObjectManager)
{
    setDomainValid (false);
    m_timerRunningFlag = false;
    m_clusterMergeFlag = false;
    m_remoteSwitchInfoList.clear ();
    setFCPrincipalDomainId(0);
    m_principalTrackingTimerRunInUnconfirmedState = false;
    m_dcaDomainListSize = 0;

    setBitMapSize (m_knownDomainsList.reachableDomainsBitMap);
    setBitMapSize (m_knownDomainsList.dcaDomainsBitMap);
    setBitMapSize (m_knownDomainsList.switchInfoReceivedBitMap);
    setBitMapSize (m_knownDomainsList.remoteKnownDCABitMap);

    m_knownDomainsList.reachableDomainsBitMap.reset ();
    m_knownDomainsList.dcaDomainsBitMap.reset ();   
    m_knownDomainsList.switchInfoReceivedBitMap.reset ();
    m_knownDomainsList.remoteKnownDCABitMap.reset ();

    m_isNeedToStartTimerForWarmRecovery = false;
	
    VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN);
}

VcsNodeFCPrincipalTrackingWorker::~VcsNodeFCPrincipalTrackingWorker ()
{
}

void VcsNodeFCPrincipalTrackingWorker::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

VcsNodeFCPrincipalTrackingWorker *VcsNodeFCPrincipalTrackingWorker::getInstance ()
{
    static VcsNodeFCPrincipalTrackingWorker *pVcsNodeFCPrincipalTrackingWorker = (VcsFabricLocalObjectManager::getInstance ())->m_fcPrincipalTrackingWorker;
    WaveNs::prismAssert (NULL != pVcsNodeFCPrincipalTrackingWorker, __FILE__, __LINE__);
    return pVcsNodeFCPrincipalTrackingWorker;
}

string VcsNodeFCPrincipalTrackingWorker::getClassName ()
{
    return ("VcsNodeFCPrincipalTrackingWorker");
}

void VcsNodeFCPrincipalTrackingWorker::deleteObjects (vector<WaveManagedObject *> *pResults)
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

void VcsNodeFCPrincipalTrackingWorker::initializeClusterGUIDs ()
{
        vector<string> clusterGUIDs = getVcsClusterGUIDsFromClusterMO ();
        if (0 == clusterGUIDs.size ())
        {
                m_vcsClusterGUIDs.push_back ("00000000000000000000000000000000"); // initialized to 32 zeros hex string = 128 bits binary 0s
        }
        else
        {
                m_vcsClusterGUIDs = clusterGUIDs;
        }
}

vector<string> VcsNodeFCPrincipalTrackingWorker::getClusterGUIDs ()
{
        return m_vcsClusterGUIDs;
}

void VcsNodeFCPrincipalTrackingWorker::setVcsClusterGUIDs (const vector<string> &vcsClusterGUIDs)
{
        m_vcsClusterGUIDs = vcsClusterGUIDs;
}

vector<string> VcsNodeFCPrincipalTrackingWorker::getVcsClusterGUIDsFromClusterMO ()
{
    VcsClusterManagedObject *pVcsClusterManagedObject = NULL;
        vector<string> vcsClusterGUIDs;
    vector<WaveManagedObject *> *pResultsCluster = fabOMDoSynchronousQuery (VcsClusterManagedObject::getClassName ());
        prismAssert (1 >= pResultsCluster->size (), __FILE__, __LINE__);

    if (1 == (*pResultsCluster).size())
    {
        if (NULL == (*pResultsCluster)[0])
        {
            WaveNs::trace (TRACE_LEVEL_FATAL, string ("VcsNodeFCPrincipalTrackingWorker::getVcsClusterGUIDsFromClusterMO: DB query returns one object but pointer to the object is NULL - this shouldn't happen so assert"));
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }
        pVcsClusterManagedObject = dynamic_cast<VcsClusterManagedObject *>((*pResultsCluster)[0]);
                vcsClusterGUIDs.push_back(pVcsClusterManagedObject->getClusterGUID ().toString());
                delete pVcsClusterManagedObject;
                delete pResultsCluster;
    }
        else
        {
                vcsClusterGUIDs.clear ();
                delete pResultsCluster;
        }
        return vcsClusterGUIDs;
}

bool VcsNodeFCPrincipalTrackingWorker::isEnvx86 ()
{
        if (0 < getInstance ()->m_x86domainList.size ())
        {
                return true;
        }
        else
        {
                return false;
        }
}

void VcsNodeFCPrincipalTrackingWorker::getx86ipAddressAndPort    (const UI32 &x86DomainId, string &x86ipAddress, UI32 &x86port)
{
        UI32 i;
        x86ipAddress = "";
        x86port = 0;

        for (i = 0; i < getInstance ()->m_x86domainList.size (); i++)
        {
                if ((getInstance ()->m_x86domainList)[i] == x86DomainId)
                {
                        x86ipAddress = (getInstance ()->m_x86ipAddresses)[i];
                        x86port = (getInstance ()->m_x86ports)[i];
                        return;
                }
        }
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::getx86ipAddressAndPort domainId not found: ")+ x86DomainId);
}

string VcsNodeFCPrincipalTrackingWorker::getIpAddressFromDomainId(UI32 domainId)
{
        UI32 port;
        string ipAddress;

        if (isEnvx86 ())
        {
                getx86ipAddressAndPort  (domainId, ipAddress, port);
                return ipAddress;
        }
        else
        {
                // get ipAddress in PPC fos environment
                return DcmToolKit::getIpAddressFromDomainId (domainId);
        }
}
extern "C" 
{
// This is a wrapper function so it can be called from a 'c' module without having to include the .h file for VcsNodeFCPrincipalTrackingWorker
vector<UI32> getx86DomainListFromFCPrincipalTrackingWorker ()
{
    return VcsNodeFCPrincipalTrackingWorker::getInstance ()->getx86DomainList ();
}
}

vector<UI32> VcsNodeFCPrincipalTrackingWorker::getx86DomainList ()
{
    return (getInstance ())->m_x86domainList;
}

vector<string> VcsNodeFCPrincipalTrackingWorker::getx86ipAddresses ()
{
    return (getInstance ())->m_x86ipAddresses;
}

vector<UI32> VcsNodeFCPrincipalTrackingWorker::getx86ports ()
{
    return (getInstance ())->m_x86ports;
}

void VcsNodeFCPrincipalTrackingWorker::setx86DomainList (vector<UI32> &x86DomainList)
{
    (getInstance ())->m_x86domainList = x86DomainList;
}

void VcsNodeFCPrincipalTrackingWorker::setx86ipAddresses (vector<string> &x86iPAddress)
{
    (getInstance ())->m_x86ipAddresses = x86iPAddress;
}

void VcsNodeFCPrincipalTrackingWorker::setx86ports (vector<UI32> &x86port)
{
    (getInstance ())->m_x86ports = x86port;
}

void VcsNodeFCPrincipalTrackingWorker::addTox86DomainList        (UI32 &x86DomainId)
{
    UI32 i;
    for (i = 0; i < (getInstance ())->m_x86domainList.size (); i++)
    {
        if (x86DomainId == getInstance ()->m_x86domainList[i])
        {
            ((getInstance ())->m_x86domainList).erase ((getInstance ())->m_x86domainList.begin ()+i);
            ((getInstance ())->m_x86ipAddresses).erase ((getInstance ())->m_x86ipAddresses.begin ()+i);
            ((getInstance ())->m_x86ports).erase ((getInstance ())->m_x86ports.begin ()+i);
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::addTox86DomainList: deleted previous domainId = ")+x86DomainId);
        }
    }
        getInstance ()->m_x86domainList.push_back (x86DomainId);
}

void VcsNodeFCPrincipalTrackingWorker::removeFromx86DomainList   (UI32 &x86DomainId)
{
    UI32 i;
    for (i = 0; i < (getInstance ())->m_x86domainList.size (); i++)
    {
        if (x86DomainId == getInstance ()->m_x86domainList[i])
        {
            ((getInstance ())->m_x86domainList).erase ((getInstance ())->m_x86domainList.begin ()+i);
            ((getInstance ())->m_x86ipAddresses).erase ((getInstance ())->m_x86ipAddresses.begin ()+i);
            ((getInstance ())->m_x86ports).erase ((getInstance ())->m_x86ports.begin ()+i);
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::removeFromx86DomainList: deleted domainId = ")+x86DomainId);
        }
    }
}


void    VcsNodeFCPrincipalTrackingWorker::addTox86IpAddressList (string &x86IpAddress)
{
        (getInstance ())->m_x86ipAddresses.push_back (x86IpAddress);
}

void    VcsNodeFCPrincipalTrackingWorker::addTox86PortList (UI32 &x86Port)
{
        (getInstance ())->m_x86ports.push_back (x86Port);
}

UI32 VcsNodeFCPrincipalTrackingWorker::getMgmtPort(UI32 domainId)
{       
        UI32 port;
        string ipAddress;

        if (isEnvx86 ())
        {
                getInstance ()->getx86ipAddressAndPort  (domainId, ipAddress, port);
                return port;
        }
        else
        {
            // get ipAddress in PPC fos environment

            if (CCM_ONLY == DcmToolKit::getDistributionMode ())
            {
                return default_ccmd_mgmt_port;
            }
            else if (DCM_NSP == DcmToolKit::getDistributionMode ())
            {
                return (defaultNspdManagementPort);
            }
            else
            {
                return default_mgmt_port; 
            }
        }
}

UI32 VcsNodeFCPrincipalTrackingWorker::getPortFromDomainId(UI32 domainId)
{
    return FrameworkToolKit::getThisLocationPort();
}

UI32 VcsNodeFCPrincipalTrackingWorker::getx86DomainIdFromIpAddressAndMgmtPort (string ipAddress, UI32 mgmtPort)
{

    UI32 i;
    string domainValStr;

    if (isEnvx86 ())
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::getx86DomainIdFromIpAddressAndMgmtPort: ipAddress = ")+ipAddress+" port = "+mgmtPort);
        for (i = 0; i < getInstance ()->m_x86domainList.size (); i++)
        {
            if (((getInstance ()->m_x86ipAddresses)[i] == ipAddress) && ((getInstance ()->m_x86ports)[i] == mgmtPort))
            {
                return (getInstance ()->m_x86domainList)[i];
            }
        }
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::getx86DomainIdFromIpAddressAndMgmtPort: ipAddress not found can't get domain id for: "+ipAddress);
        return 0;
    }
    else    // should never get here
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::getx86DomainIdFromIpAddressAndMgmtPort: this function should only be called for the PPC environment: ");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
        return 0;   // just to avoid compiler warning
    }
}

vector<WaveManagedObject *> *VcsNodeFCPrincipalTrackingWorker::fabOMDoSynchronousQuery (const string className)
{
        vector<WaveManagedObject *> *pResults = querySynchronously (className);
        prismAssert (NULL != pResults, __FILE__, __LINE__);
        return pResults;
}

vector<VcsNodeLocalManagedObject *>    *VcsNodeFCPrincipalTrackingWorker::getNodeLocalManagedObjects	()
{
        vector<WaveManagedObject *> *pResults = fabOMDoSynchronousQuery (VcsNodeLocalManagedObject::getClassName());
        return (reinterpret_cast <vector<VcsNodeLocalManagedObject *> *> (pResults));
}

ObjectId VcsNodeFCPrincipalTrackingWorker::getLocalSwitchObjectId ()
{
    return (VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeObjectId ();
}

UI32 VcsNodeFCPrincipalTrackingWorker::getMyDomainId ()
{
        UI32 myDomainId = VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeLocalManagedObjectDomainId ();
        return myDomainId;
}

VcsNodeLocalManagedObject *VcsNodeFCPrincipalTrackingWorker::getNodeLocalManagedObjectForThisNode ()
{
        ObjectId localSwitchObjectId = getLocalSwitchObjectId ();
        return getSwitchObjectFromObjectId (localSwitchObjectId);
}

VcsNodeLocalManagedObject * VcsNodeFCPrincipalTrackingWorker::getSwitchObjectFromObjectId(const ObjectId &switchObjectId)
{
        vector<ObjectId> switchObjectIds;
        switchObjectIds.insert(switchObjectIds.begin(), 1, switchObjectId);
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;

        vector<WaveManagedObject *> *pResults = querySynchronously (VcsNodeLocalManagedObject::getClassName (), switchObjectIds);
        if (NULL == pResults)
        {
                trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::getSwitchObjectFromObjectId: pResults for LocalNode query by ObjectId returned NULL");
                prismAssert (false, __FILE__, __LINE__);
        }
        if (1 < pResults->size ())
        {
                delete pResults;
                trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::getSwitchObjectFromObjectId: multiple objects with same object ID found");
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

void VcsNodeFCPrincipalTrackingWorker::addRemoteSwitchInfo (const VcsFabricLocalSwitchInfoObject &remoteSwitchInfo)
{
        m_remoteSwitchInfoList.push_back (remoteSwitchInfo);

        setSwitchInfoRetrievedBit (remoteSwitchInfo.getSwitchDomainId ());
}

void  VcsNodeFCPrincipalTrackingWorker::clearSwitchInfoLists ()
{
    getInstance ()->m_remoteSwitchInfoList.clear ();
}

void VcsNodeFCPrincipalTrackingWorker::getWWNAndSwbdForReplacementNodes (VcsFabricLocalSwitchInfoObject & switchInfoObject)
{

	ResourceId nodeSpecificStatus = VCS_FABRIC_LOCAL_SPECIFIC_STATUS_NODE_REPLACEMENT_IN_PROGRESS;

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeResourceId (nodeSpecificStatus, "specificStatus"));
    vector<WaveManagedObject *> *pResultsLocalNodes = querySynchronously (&synchronousQueryContext);
	
	prismAssert (NULL != pResultsLocalNodes, __FILE__, __LINE__);

	int numReplacementNodes = (int) (*pResultsLocalNodes).size ();
	
	if (0 >= numReplacementNodes)
	{
		trace (TRACE_LEVEL_DEBUG, string ("VcsNodeFCPrincipalTrackingWorker::getWWNAndSwbdForReplacementNodes: # ReplacementNodes = ")+ numReplacementNodes);
		
		deleteObjects (pResultsLocalNodes);
		return;
	}
	
	VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;
	WorldWideName wwn;
	SI32 swbd;
	
	for (int i=0; i < numReplacementNodes; i++)
	{

        pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *>((*pResultsLocalNodes)[0]);
		
		prismAssert (NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__);
		
		wwn = pVcsNodeLocalManagedObject->getSwitchWWN ();
		swbd = pVcsNodeLocalManagedObject->getSWBDModel ();
		
		trace (TRACE_LEVEL_DEBUG, string ("VcsNodeFCPrincipalTrackingWorker::getWWNAndSwbdForReplacementNodes: ReplaceNode WWN = ")+wwn.toString ()+ " Swbd = " + swbd) ;
		
		switchInfoObject.addReplacementSwitchSwbd   (swbd);
		switchInfoObject.addReplacementSwitchWWN	(wwn);
	}

	deleteObjects (pResultsLocalNodes);
}

void VcsNodeFCPrincipalTrackingWorker::getLocalSwitchInfo (VcsFabricLocalSwitchInfoObject &localSwitchInfo)
{
    UI32                         myDomainId                   = getMyDomainId ();
    VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance ();
    LocationRole                 locationRole                 = FrameworkToolKit::getThisLocationRole ();
    UI32                         primaryLocationId            = FrameworkToolKit::getClusterPrimaryLocationId ();
    bool                         isLocationUnconfirmRole;
    UI32                         principalNodeDomainId;

    if (NULL == VcsFabricLocalObjectManager::getInstance ()->m_pVcsNodeLocalManagedObject ) 
    {
        trace (TRACE_LEVEL_FATAL, "VcsNodeFCPrincipalTrackingWorker::getLocalSwitchInfo :This should not happen. Asserting to catch how this happend.");
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    if (0 != primaryLocationId)
    {
        principalNodeDomainId = DcmToolKit::getDomainIdFromLocationId (primaryLocationId);
    }
    else
    {
        principalNodeDomainId = 0;   // principal doesn't exist
    }

    if ((locationRole == LOCATION_PRIMARY_UNCONFIRMED) || (locationRole == LOCATION_SECONDARY_UNCONFIRMED))
    {
        isLocationUnconfirmRole = true;
    }
    else
    {
        isLocationUnconfirmRole = false;
    }
    
    localSwitchInfo.setSwitchDomainId                      (myDomainId);
    localSwitchInfo.setSwitchSerialNumber                  (pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectSerialNumber ());
    localSwitchInfo.setSwitchSWVersion                     (my_sw_version);  
    localSwitchInfo.setSwitchVcsId                         (pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectVcsId ());
    localSwitchInfo.setSwitchClusterManagementIpAddress    (FrameworkToolKit::getThisLocationIpAddress());
    localSwitchInfo.setSwitchClusterManagementPort         (FrameworkToolKit::getThisLocationPort ());
    localSwitchInfo.setSwitchPublicIpAddress               (pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectPublicIpAddress ());
    localSwitchInfo.setIsSwitchInCluster                   (pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectInVcs ());
    localSwitchInfo.setIsPrincipal                         (pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectIsPrincipal ());
    localSwitchInfo.setDcaBitMap                           (m_knownDomainsList.dcaDomainsBitMap);
    localSwitchInfo.setNodeState                           (pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectSpecificStatus ());
    localSwitchInfo.setLocationRole                        (locationRole);
    localSwitchInfo.setIsSwitchUnconfirmPrincipal          (isLocationUnconfirmRole);
    localSwitchInfo.setPrincipalSwitchDomainId             (principalNodeDomainId);
    localSwitchInfo.setSwitchLocationId                    (FrameworkToolKit::getThisLocationId ());
	getWWNAndSwbdForReplacementNodes 					   (localSwitchInfo); // info is for all replacement switches in cluster not the local switch
}

void VcsNodeFCPrincipalTrackingWorker::getKnownDomainList (scnDomainsStruct &knownDomainList)
{
    knownDomainList.reachableDomainsBitMap = getInstance ()->m_knownDomainsList.reachableDomainsBitMap;
    knownDomainList.dcaDomainsBitMap = getInstance ()->m_knownDomainsList.dcaDomainsBitMap;
    knownDomainList.switchInfoReceivedBitMap = getInstance ()->m_knownDomainsList.switchInfoReceivedBitMap;
    knownDomainList.remoteKnownDCABitMap = getInstance ()->m_knownDomainsList.remoteKnownDCABitMap;
}

UI32 VcsNodeFCPrincipalTrackingWorker::getDomainsInCluster (BitMap &domainsIncluster)
{
    vector<WaveManagedObject *> *pVcsLocalNodes                             = fabOMDoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
    VcsNodeLocalManagedObject   *pVcsNodeLocalManagedObject                 = NULL;
    UI32                         numNodes                                   = pVcsLocalNodes->size ();
    UI32                         i;
    UI32                         count                                      = 0;
    UI32                         domainId                                   = 0;
    ResourceId                   specificStatusOfVcsNodeLocalMangedObject;

    trace (TRACE_LEVEL_DEBUG, string ("VcsNodeFCPrincipalTrackingWorker::getDomainsInCluster num nodes in cluster = ")+ numNodes);
    
    for (i = 0; i < numNodes; i++)
    {
        pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pVcsLocalNodes)[i]);

        if (NULL != pVcsNodeLocalManagedObject)
        {
            domainId = pVcsNodeLocalManagedObject->getDomainId ();

            specificStatusOfVcsNodeLocalMangedObject = pVcsNodeLocalManagedObject->getSpecificStatus ();            

            trace (TRACE_LEVEL_DEBUG, string ("VcsNodeFCPrincipalTrackingWorker::getDomainsInCluster domainId = ")+ domainId+" node specific status = "+FrameworkToolKit::localize (specificStatusOfVcsNodeLocalMangedObject));
            domainsIncluster.set (pVcsNodeLocalManagedObject->getDomainId ());

            count++;
        }
    }

    deleteObjects (pVcsLocalNodes); 
    return count;
}

UI32 VcsNodeFCPrincipalTrackingWorker::getConnectedDomainsInCluster (BitMap &domainsIncluster)
{
    vector<WaveManagedObject *> *pVcsLocalNodes = fabOMDoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;
    UI32 numNodes = pVcsLocalNodes->size ();
    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::getConnectedDomainsInCluster num nodes in cluster = ")+ numNodes);
    UI32 i;
    UI32 count = 0;
    UI32 domainId = 0;
    ResourceId specificStatusOfVcsNodeLocalMangedObject;

    for (i = 0; i < numNodes; i++)
    {
        pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pVcsLocalNodes)[i]);
        if (NULL != pVcsNodeLocalManagedObject)
        {
            domainId = pVcsNodeLocalManagedObject->getDomainId ();
            specificStatusOfVcsNodeLocalMangedObject = pVcsNodeLocalManagedObject->getSpecificStatus ();
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker:getConnectedDomainsInCluster domainId = ")+ domainId+" node specific status = "+FrameworkToolKit::localize (specificStatusOfVcsNodeLocalMangedObject));
            if ((VCS_FABRIC_LOCAL_SPECIFIC_STATUS_PRIMARY == specificStatusOfVcsNodeLocalMangedObject) ||
                (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY == specificStatusOfVcsNodeLocalMangedObject) ||
                (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE2_STARTED == specificStatusOfVcsNodeLocalMangedObject) ||
                (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_REJOIN_PHASE2_STARTED == specificStatusOfVcsNodeLocalMangedObject) ||
                (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE2_COMPLETED == specificStatusOfVcsNodeLocalMangedObject) ||
                (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_REJOIN_PHASE2_COMPLETED == specificStatusOfVcsNodeLocalMangedObject) ||
                (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE3_STARTED == specificStatusOfVcsNodeLocalMangedObject) ||
                (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_REJOIN_PHASE3_STARTED == specificStatusOfVcsNodeLocalMangedObject))
            {
                domainsIncluster.set (pVcsNodeLocalManagedObject->getDomainId ());
                count++;
            }
        }
    }
    deleteObjects (pVcsLocalNodes);
    return count;
}

VcsNodeLocalManagedObject * VcsNodeFCPrincipalTrackingWorker::getSwitchObjectFromDomainId (UI32 domainId)
{
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (domainId, "domainId"));
    vector<WaveManagedObject *> *pResultsLocalNodes = querySynchronously (&synchronousQueryContext);
    if (NULL == pResultsLocalNodes)
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::getSwitchObjectFromDomainId: results pointer from querySynchronously is NULL");
        prismAssert (false, __FILE__, __LINE__);
    }

    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;
    string ipAddress = "";
    UI32 i;
    if (1 < pResultsLocalNodes->size ())
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::getSwitchObjectFromDomainId: multiple objects with same domain ID found domainId = ")+domainId + " numObjects = "+pResultsLocalNodes->size ());
        for (i = 0; i < pResultsLocalNodes->size (); i++)
        {
            if (NULL == (*pResultsLocalNodes)[i])
            {
                WaveNs::trace (TRACE_LEVEL_FATAL, string ("VcsNodeFCPrincipalTrackingWorker::getSwitchObjectFromDomainId: Ptr to DB object is null even though (*pResultsLocalNodes)[i] should be valid for i = ")+i);
                WaveNs::prismAssert (false, __FILE__, __LINE__);
            }
            pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *>((*pResultsLocalNodes)[i]);
            domainId = pVcsNodeLocalManagedObject->getDomainId ();
            ipAddress = (pVcsNodeLocalManagedObject->getPublicIpAddress ())[0];
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::getSwitchObjectFromDomainId: object list: domainId = ")+domainId + " pubIpAddress = "+ipAddress);
        }
        prismAssert (false, __FILE__, __LINE__);
    }
    if (0 == pResultsLocalNodes->size ())
    {
        delete pResultsLocalNodes;
        return NULL;
    }
    pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *>((*pResultsLocalNodes)[0]);
    delete pResultsLocalNodes;
    return pVcsNodeLocalManagedObject;
}

void VcsNodeFCPrincipalTrackingWorker::setBitMapSize (BitMap &bitMap)
{
    bitMap.resize (max_domain_list -1);
}

void VcsNodeFCPrincipalTrackingWorker::setSizeForDomainBitMaps (scnDomainsStruct & domainBitMapStruct)
{
    setBitMapSize (domainBitMapStruct.dcaDomainsBitMap);
    setBitMapSize (domainBitMapStruct.reachableDomainsBitMap);
    setBitMapSize (domainBitMapStruct.remoteKnownDCABitMap);
    setBitMapSize (domainBitMapStruct.switchInfoReceivedBitMap);
}

bool VcsNodeFCPrincipalTrackingWorker::validDomainId (UI32 domainId)
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

bool VcsNodeFCPrincipalTrackingWorker::vcsModeEnabled ()
{
    bool vcsMode;
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = getInstance ()->getNodeLocalManagedObjectForThisNode ();
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

void VcsNodeFCPrincipalTrackingWorker::prepareDomainListAfterHaRecovery (const bool &isWarmRecovery)
{
    if (true == isWarmRecovery)
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::prepareDomainListAfterHaRecovery Warm Recovery");

        UI32 principalDomainId = fabGetPrincipalSwitchDomain ();
        UI8 *domainList        = new UI8 [256];

        trace (TRACE_LEVEL_INFO, string ("FC Principal Domain Id ") + principalDomainId);

        setFCPrincipalDomainId (principalDomainId);
        setDomainValid (true);

        m_isNeedToStartTimerForWarmRecovery = true;

        getDomainLst((char *)domainList);

        trace (TRACE_LEVEL_INFO, string ("Number of domain in reachable list are ") + domainList[0]);

        for (UI32 i = 1; i <= domainList[0]; i++)
        {
            if (domainList [i] != getMyDomainId ())
            {
//            trace (TRACE_LEVEL_INFO, string ("setting DR bit for domain ") + domainList [i]);
                setReachableDomainBitForDomainId (domainList [i]);
            }
        }
    
        delete[] domainList;

        string  ethernetInterface                 = "";
        string  ipAddress                         = "";
        UI32    maxNumberOfRetriesForIpAddress    = 60;
        UI32    numberOfRetriesForIpAddress       = 0;
        string  interfaceState                    = "";

        ethernetInterface = PrismFrameworkObjectManager::getEthernetInterfaceForThisLocation ();

        if ("" == ethernetInterface)
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::prepareDomainListAfterHaRecovery : No specific Ethernet Interface Assigned");
        }
        else
        {
            ipAddress = FrameworkToolKit::getIpAddressForInterface (ethernetInterface);

            if (true == ((VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeLocalManagedObjectVcsMode()))
            {
                while ("" == ipAddress)
                {
                    trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::prepareDomainListAfterHaRecovery Retrying to fetch IP Address for Ethernet Interface \"" + ethernetInterface + "\"");

                    ipAddress = FrameworkToolKit::getIpAddressForInterface (ethernetInterface);

                    numberOfRetriesForIpAddress++;

                    if (numberOfRetriesForIpAddress > maxNumberOfRetriesForIpAddress)
                    {
                        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::prepareDomainListAfterHaRecovery Cound not Fetch IpAddress:");
                        break;
                    }
                    else
                    {
                        prismSleep(1);
                    }
                }

                if ("" == ipAddress)
                {
                    trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::prepareDomainListAfterHaRecovery : Not Able to Fetch Any IpAddress");
                    interfaceState = getStateForInterface(ethernetInterface);
                    trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::prepareDomainListAfterHaRecovery:getStateForInterface ") + ethernetInterface + string(" : ") + interfaceState);
                }

                if (FrameworkToolKit::getThisLocationIpAddress () == ipAddress)
                {
                    trace (TRACE_LEVEL_DEBUG, "VcsNodeFCPrincipalTrackingWorker::prepareDomainListAfterHaRecovery : IP Address unchanged for Ethernet Interface \"" + ethernetInterface + "\"");
                }
                else
                {
                    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::prepareDomainListAfterHaRecovery: Resetting IP Address to : ") + ipAddress + string (" for this node.") + FrameworkToolKit::getThisLocationId () + string ("for interface ") + ethernetInterface); 

                    ClusterLocalSetThisNodeIpAddressMessage message         (ipAddress);
                    ResourceId                              messageStatus = WAVE_MESSAGE_ERROR;

                    messageStatus = sendSynchronously (&message, FrameworkToolKit::getThisLocationId ()); 

                    prismAssert (WAVE_MESSAGE_SUCCESS == messageStatus, __FILE__, __LINE__);
    
                    if (WAVE_MESSAGE_SUCCESS != message.getCompletionStatus ())
                    {   
                        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::prepareDomainListAfterHaRecovery: Not able to set IP address") + FrameworkToolKit::localize (message.getCompletionStatus ()));
                    }       
                }
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "VcsNodeFabosInterfaceObjectManager::prepareDomainListAfterHaRecovery: Vcs Mode is Not Enabled ");
            }
        }

        if (CCM_ONLY == DcmToolKit::getDistributionMode ())
        {
             replaySCNQueueEvents();
        }

        if (principalDomainId == getMyDomainId ())
        {
            startPrincipalTrackingTimer (1);
        }
        else
        {
            m_isNeedToStartTimerForWarmRecovery = false;
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::prepareDomainListAfterHaRecovery Cold Recovery");

        if (CCM_ONLY == DcmToolKit::getDistributionMode ())
        {
            replaySCNQueueEvents();
        }
    }
}

void VcsNodeFCPrincipalTrackingWorker::resetDomainBitmap ()
{
    setFCPrincipalDomainId (0);

    initializeDomainList ();

    VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN);
}

void VcsNodeFCPrincipalTrackingWorker::initializeDomainList ()
{
    setBitMapSize (getInstance ()->m_knownDomainsList.reachableDomainsBitMap);
    setBitMapSize (getInstance ()->m_knownDomainsList.dcaDomainsBitMap);
    setBitMapSize (getInstance ()->m_knownDomainsList.switchInfoReceivedBitMap);
    setBitMapSize (getInstance ()->m_knownDomainsList.remoteKnownDCABitMap);

    getInstance ()->m_knownDomainsList.reachableDomainsBitMap.reset ();
    getInstance ()->m_knownDomainsList.dcaDomainsBitMap.reset ();   
    getInstance ()->m_knownDomainsList.switchInfoReceivedBitMap.reset ();
    getInstance ()->m_knownDomainsList.remoteKnownDCABitMap.reset ();

    UI32 myDomainId = getMyDomainId ();

    if (validDomainId (myDomainId))
    {
        getInstance ()->m_knownDomainsList.reachableDomainsBitMap.set (myDomainId);
        getInstance ()->m_knownDomainsList.dcaDomainsBitMap.set (myDomainId);
        m_dcaDomainListSize = 1;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::initializeDomainList: getMyDomainId () returned invalid myDomainId, domain list initialization did not succeed myDomainId = ")+myDomainId);
        m_dcaDomainListSize = 0;
    }
}

UI32 VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents ()
{
    trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents: Entering...");

    UI32 numberOfScnEventsReplayed = 0;

    //State Transition for VcsFabricLocalObjectManager

    if (VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN != VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    { 
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents SCNs are not blocked, so replay is not required"));
        return (numberOfScnEventsReplayed);
    }

    VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

    setSizeForDomainBitMaps (m_knownDomainsList);
    
    VcsNodeFabosScnQueueObject *pVcsNodeFabosScnQueueObject = NULL;
    
    VcsNodeDomainValidEvent                                 *pDVEvent                   = NULL;
    const VcsNodeDomainValidEvent                   *pDVEventCopy               = NULL;
    
    VcsNodeDomainCapabilitiesAvailableEvent             *pDCAEvent                  = NULL;
    const VcsNodeDomainCapabilitiesAvailableEvent       *pDCAEventCopy              = NULL;
    
    VcsNodeDomainInvalidEvent                           *pDIEvent                   = NULL;
    const VcsNodeDomainInvalidEvent                     *pDIEventCopy               = NULL;
    
    VcsNodeDomainReachableEvent                         *pDREvent                   = NULL;
    const VcsNodeDomainReachableEvent                   *pDREventCopy               = NULL;
    
    VcsNodeDomainUnreachableEvent                       *pDUEvent                   = NULL;
    const VcsNodeDomainUnreachableEvent                 *pDUEventCopy               = NULL;

    VcsNodeSwitchOfflineEvent                           *pSwitchOfflineEvent        = NULL;
    const VcsNodeSwitchOfflineEvent                     *pSwitchOfflineEventCopy    = NULL;

    VcsNodeSwitchReadyToAcceptCommandsEvent        *pSwitchReadyEvent          = NULL;
    const VcsNodeSwitchReadyToAcceptCommandsEvent  *pSwitchReadyEventCopy      = NULL;
    
    VcsNodeSwitchActivationCompleteEvent        *pSwitchActivationCompleteEvent          = NULL;
    const VcsNodeSwitchActivationCompleteEvent  *pSwitchActivationCompleteEventCopy      = NULL;

    UI32 eventType;
    UI32 domainId;
    UI32 principalDomainId;
    UI32 eventId;
    ResourceId status = WAVE_MESSAGE_ERROR;

    trace (TRACE_LEVEL_INFO,string ("VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents: SCNQsize = ")+ VcsNodeFabosInterfaceObjectManager::getInstance ()->scnQueueSize ());

    while (0 != VcsNodeFabosInterfaceObjectManager::getInstance ()->scnQueueSize ())
    {
        pVcsNodeFabosScnQueueObject = VcsNodeFabosInterfaceObjectManager::getInstance ()->getFrontScnQueueObject();
        domainId = pVcsNodeFabosScnQueueObject->getDomainId();
        principalDomainId = pVcsNodeFabosScnQueueObject->getPrincipalDomainId();
        eventType = pVcsNodeFabosScnQueueObject->getScnType();
        eventId = pVcsNodeFabosScnQueueObject->getScnObjectId ();

        switch (eventType)
        {
            case VCS_NODE_FABOS_DOMAIN_VALID:

                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents: Replaying Domain Valid event for domainId = ")+ domainId);
                
                pDVEvent = new VcsNodeDomainValidEvent ();

                pDVEvent->setDomainId (domainId);
                pDVEvent->setPrincipalDomainId (principalDomainId);
                pDVEvent->setEventId (eventId);

                pDVEventCopy = pDVEvent;

                status = DomainValidEventHandler (pDVEventCopy);
                
                if (WAVE_MESSAGE_SUCCESS == status)
                {
                    broadcast (pDVEvent);
                }
                else
                {
                    delete (pDVEvent);    
                }
                
                break;

            case VCS_NODE_FABOS_DOMAIN_CAPABILITIES_AVAILABLE:
                            
                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents: Replaying DCA event for domainId = ")+ domainId);
                
                pDCAEvent = new VcsNodeDomainCapabilitiesAvailableEvent ();
                pDCAEvent->setDomainId (domainId);
                pDCAEvent->setEventId (eventId);
                pDCAEventCopy = pDCAEvent;
                status = DomainCapabilitiesAvailableEventHandler (pDCAEventCopy);
                
                if (WAVE_MESSAGE_SUCCESS == status)
                {
                broadcast (pDCAEvent);
                }
                else
                {
                    delete (pDCAEvent);    
                }
                
                break;

            case VCS_NODE_FABOS_DOMAIN_INVALID:
                            
                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents: Replaying Domain Invalid event for domainId = ")+ domainId);
                
                pDIEvent = new VcsNodeDomainInvalidEvent ();
                pDIEvent->setDomainId (domainId);
                pDIEvent->setEventId (eventId);
                pDIEventCopy = pDIEvent;
                status = DomainInvalidEventHandler(pDIEventCopy);
                
                if (WAVE_MESSAGE_SUCCESS == status)
                {
                broadcast (pDIEvent);
                }
                else
                {
                    delete (pDIEvent);    
                }
                
                break;

            case VCS_NODE_FABOS_DOMAIN_REACHABLE:
                            
                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents: Replaying Domain Reachable event for domainId = ")+ domainId);
                
                pDREvent = new VcsNodeDomainReachableEvent ();
                pDREvent->setDomainId (domainId);
                pDREvent->setEventId (eventId);
                pDREventCopy = pDREvent;
                status = DomainReachableEventHandler(pDREventCopy);
                
                if (WAVE_MESSAGE_SUCCESS == status)
                {
                broadcast (pDREvent);
                }
                else
                {
                    delete (pDREvent);    
                }
                
                break;

            case VCS_NODE_FABOS_DOMAIN_UNREACHABLE:
                            
                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents: Replaying Domain UnReachable event for domainId = ")+ domainId);
                
                pDUEvent = new VcsNodeDomainUnreachableEvent ();
                pDUEvent->setDomainId (domainId);
                pDUEvent->setEventId (eventId);
                pDUEventCopy = pDUEvent;
                status = DomainUnReachableEventHandler(pDUEventCopy);
                
                if (WAVE_MESSAGE_SUCCESS == status)
                {
                broadcast (pDUEvent);
                }
                else
                {
                    delete (pDUEvent);    
                }
                
                break;

            case VCS_NODE_FABOS_SWITCH_OFFLINE:
                            
                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents: Replaying Switch Offline event for domainId"));
                
                pSwitchOfflineEvent = new VcsNodeSwitchOfflineEvent ();
                pSwitchOfflineEvent->setEventId (eventId);
                pSwitchOfflineEventCopy = pSwitchOfflineEvent;
                status = SwitchOfflineEventHandler (pSwitchOfflineEventCopy);
                
                if (WAVE_MESSAGE_SUCCESS == status)
                {
                broadcast (pSwitchOfflineEvent);
                }
                else
                {
                    delete (pSwitchOfflineEvent);    
                }
                
                break;

            case VCS_NODE_FABOS_SWITCH_READY_TO_ACCEPT_COMMANDS:
                
                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents: Replaying Switch Ready event"));
                
                pSwitchReadyEvent = new VcsNodeSwitchReadyToAcceptCommandsEvent ();
                pSwitchReadyEvent->setEventId (eventId);
                pSwitchReadyEventCopy = pSwitchReadyEvent;
                status = switchReadyToAcceptCommandsEventHandler (pSwitchReadyEventCopy);
                
                if (WAVE_MESSAGE_SUCCESS == status)
                {
                broadcast (pSwitchReadyEvent);
                }
                else
                {
                    delete (pSwitchReadyEvent);    
                }
                
                break;

            case VCS_NODE_FABOS_SWITCH_ACTIVATION_COMPLETE:

                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents: Replaying Switch Activation Complete event"));

                pSwitchActivationCompleteEvent = new VcsNodeSwitchActivationCompleteEvent ();
                pSwitchActivationCompleteEvent->setEventId (eventId);
                pSwitchActivationCompleteEventCopy = pSwitchActivationCompleteEvent;
                status = switchActivationCompleteEventHandler (pSwitchActivationCompleteEventCopy);

                if (WAVE_MESSAGE_SUCCESS == status)
                {
                    broadcast (pSwitchActivationCompleteEvent);
                }
                else
                {
                    delete (pSwitchActivationCompleteEvent);    
                }

                break;

            case VCS_NODE_FABOS_SYNC_START:
                {
                    struct in_addr peer_ipa;
                    string peerIp = "";
                    SI32 peerPort = FrameworkToolKit::getHaInterfaceReceiverPort();;
                
                    peer_ipa.s_addr = get_cp_ha_peerip();
                    peerIp.assign(inet_ntoa(peer_ipa));

                    status = VcsFabricLocalObjectManager::getInstance ()->notifyStartSyncDumpHandler (peerIp, peerPort, eventId);

                    if (WAVE_MESSAGE_SUCCESS != status)
                    {
                        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents: notifyStartSyncDumpHandler encountered a failure."));
                    }
                }
                break;

            default:
                            
                trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::replaySCNQueueEvents: Unknown SCN type = ")+ eventType+ " for domainId = " + domainId);
                
                // Pop this unknown event so it doesn't hang around, other event are poped by their event handlers
                VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(eventId);
                break;
                
        }

        numberOfScnEventsReplayed++;
    }

    return (numberOfScnEventsReplayed);
}

UI32 VcsNodeFCPrincipalTrackingWorker::getFCPrincipalDomainId ()
{
    return m_fcPrincipalDomainId;
}

void VcsNodeFCPrincipalTrackingWorker::setFCPrincipalDomainId(const UI32 fcPrincipalDomainId)
{
    m_fcPrincipalDomainId = fcPrincipalDomainId;
}

void  VcsNodeFCPrincipalTrackingWorker::cleanupTimerHandlerForDomainId(UI32 domainId, bool delTimer)
{
    UI32 i;
    vector<VcsNodeTimerHandler *>::iterator  theIterator;
    ResourceId  timerStatus;

    trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::cleanupTimerHandlerForDomainId : Enter Domain ID: ") + domainId);

    for (i = 0; i < m_vcsNodeTimerHandler.size (); i++)
    {
        if (m_vcsNodeTimerHandler[i]->getDomainId() == domainId)
        {
            /* Found timer handler so delete the timer and free the structure */
            if (delTimer) 
            {
                timerStatus = deleteTimer (m_vcsNodeTimerHandler [i]->getTimerHandle());

                if (FRAMEWORK_TIMER_CAN_NOT_DELETE == timerStatus) 
                {
                    trace (TRACE_LEVEL_ERROR, string("VcsNodeFCPrincipalTrackingWorker::cleanupTimerHandlerForDomainId : Failed to delete timer for domainId = ")+domainId);
                }
                else
                {
                    m_vcsNodeTimerHandler [i]->setTimerHandle(0);
                }
            }

            // Delete heartbeat data structure as it is no longer needed.
            delete (m_vcsNodeTimerHandler[i]);

            theIterator =  m_vcsNodeTimerHandler.begin() + i;
            m_vcsNodeTimerHandler.erase (theIterator);

            return;
        }
    }
}

void VcsNodeFCPrincipalTrackingWorker::processCleanupTimerForDomainId (TimerHandle timerHandle, void * pContext)
{
    VcsNodeTimerHandler *pVcsNodeTimerHandler   = (VcsNodeTimerHandler *) pContext;
    UI32                 domainId               = pVcsNodeTimerHandler->getDomainId();
    
    if (false == getInstance ()->m_knownDomainsList.reachableDomainsBitMap[domainId]) 
    {
        VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance ();

        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::processCleanupTimerForDomainId : domainId = ")+ domainId +" LocationID = " + pVcsNodeTimerHandler->getLocationId());

        // Domain is not Reachable send message to Framework 
        DisconnectFromNodeMessage *pDisconnectFromNodeMessage = new DisconnectFromNodeMessage (pVcsNodeTimerHandler->getLocationId());

        pDisconnectFromNodeMessage->setServerIpAddress (pVcsNodeTimerHandler->getServerIpAddress ());
        pDisconnectFromNodeMessage->setServerPort (pVcsNodeTimerHandler->getServerPort ());

        WaveMessageStatus status = pVcsFabricLocalObjectManager->sendOneWay (pDisconnectFromNodeMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
           trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::processCleanupTimerForDomainId: One way message not successful");
        }
        else
        {
           trace (TRACE_LEVEL_DEBUG, string ("VcsNodeFCPrincipalTrackingWorker::processCleanupTimerForDomainId: One way message successfully sent to clean up for domainId = ")+ domainId);
        }
    }
    /* Free the timer handler context */
    cleanupTimerHandlerForDomainId (pVcsNodeTimerHandler->getDomainId(), false);

    if (0 == m_vcsNodeTimerHandler.size () && getMyDomainId () == getFCPrincipalDomainId()) // finished handling disconnects from locations & node is FC principal
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::processCleanupTimerForDomainId: completed disconnect from unreachable locations and node is FC Principal, starting Principal Tracking Timer");

        startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
    }
}

ResourceId VcsNodeFCPrincipalTrackingWorker::DomainInvalidEventHandler (const VcsNodeDomainInvalidEvent *&pEvent)
{
    prismAssert (NULL != pEvent, __FILE__, __LINE__);

    UI32        invalidDomainId = pEvent->getDomainId ();
    ResourceId  status          = WAVE_MESSAGE_SUCCESS;

    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(pEvent->getEventId ()); 

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::DomainInvalidEventHandler Event not in queue, skipping handling this event, invalid DomainId = ")+invalidDomainId);
        return (status);
    }

    static UI32 numberOfEventsRceivedSoFar = 0;

    numberOfEventsRceivedSoFar++;

    setDomainValid (false);
    return (status);
}

void VcsNodeFCPrincipalTrackingWorker::setDomainValid (const bool &domainValidOrInvalid)
{
    m_fabDomainValid = domainValidOrInvalid;
}

bool VcsNodeFCPrincipalTrackingWorker::getDomainValid ()
{
    return m_fabDomainValid;
}


ResourceId VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler (const VcsNodeDomainValidEvent *&pEvent)
{
    prismAssert (NULL != pEvent, __FILE__, __LINE__);

    ResourceId  status  =  WAVE_MESSAGE_SUCCESS;
    UI32        eventId = pEvent->getEventId ();

    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(pEvent->getEventId ());

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler: Event not in queue skipping handling this event, eventId  = ")+ eventId);
        return (status);
    }
    
    static UI32 numberOfEventsRceivedSoFar = 0;

    numberOfEventsRceivedSoFar++;

    trace (TRACE_LEVEL_DEBUG, string ("VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler: Event count = ") + numberOfEventsRceivedSoFar);

    UI32 domainValidDomain      = pEvent->getDomainId ();
    UI32 OldFCPrincipalDomainId = getFCPrincipalDomainId ();
    string  interfaceState      = "";

    setFCPrincipalDomainId (pEvent->getPrincipalDomainId ());

    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler: DomainValid DomainId  = ")+ domainValidDomain + "fcPrincipal DomainId = "+ getFCPrincipalDomainId());

    setDomainValid (true);

    string ethernetInterface = PrismFrameworkObjectManager::getEthernetInterfaceForThisLocation ();

    // Reset the ip address since our ip address might have changed.

    if ("" == ethernetInterface)
    {   
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : No specific Ethernet Interface Assigned and hence no IP Address resetting is required.");
    }
    else
    {   
        string ipAddress = FrameworkToolKit::getIpAddressForInterface (ethernetInterface);

        if ("" == ipAddress)
        {   
            trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : No IP Address assigned to user assigned Ethernet Interface \"" + ethernetInterface + "\"");
            interfaceState = getStateForInterface(ethernetInterface);
            trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler:  getStateForInterface ") + ethernetInterface + string(" : ") + interfaceState);

            ////////////////////////////////////
            // Temporary fix - needs to be reenabled
            ////////////////////////////////////
            //prismAssert (false, __FILE__, __LINE__);
        }
        else if (FrameworkToolKit::getThisLocationIpAddress () == ipAddress)
        {
            trace (TRACE_LEVEL_DEBUG, "VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : IP Address unchanged for Ethernet Interface \"" + ethernetInterface + "\"");
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "---VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : Resetting IP Address to :\"" + ipAddress + "\" for this node." + FrameworkToolKit::getThisLocationId ());

            ClusterLocalSetThisNodeIpAddressMessage message         (ipAddress);
            ResourceId                              messageStatus = WAVE_MESSAGE_ERROR;

            messageStatus = sendSynchronously (&message, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != messageStatus)
            {
                trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : Error in sending message to set this node IP Address : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");
                status = messageStatus;
            }
            else
            {
                messageStatus = message.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != messageStatus)
                {
                    trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : Asserting since failed to set this node IP Address : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");

                    status = messageStatus;
                }
                else
                {
                    trace (TRACE_LEVEL_DEBUG, "VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : successfully set this node IP Address : Status : \"" + FrameworkToolKit::localize (messageStatus) + "\"");
                    status = WAVE_MESSAGE_SUCCESS;
                }
            }
        }

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    VcsFabricLocalObjectManager* pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance ();

    if(pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectDomainId () != domainValidDomain)
    {
        (VcsNodeFabosInterfaceObjectManager::getInstance ())->setDomainId (domainValidDomain);

        startTransaction();

        pVcsFabricLocalObjectManager->updateThisVcsNodeLocalManagedObject ();
        pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectDomainId(domainValidDomain);

        status = commitTransaction ();

        if (FRAMEWORK_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler: Failed to set domain id.");

            prismAssert (false, __FILE__, __LINE__);

            return (status);
        }
        else
        {
            //Return WAVE_MESSAGE_SUCCESS else the caller will not broadcast this event to any plugin that has registered for this event
            status = WAVE_MESSAGE_SUCCESS;
        }
    }

    // Notify VIP Global worker to apply virtual ip if =>
    // First Domain Valid Event is being processed &&
    // Distribution mode is CCM_ONLY &&
    // Location Role is SA &&
    // This node is FC principal &&
    // number of domains in fabGetDomainList is one.

    static bool domainValidEventAlreadyReceivedAfterReboot = false;

    if ((false == domainValidEventAlreadyReceivedAfterReboot) && (CCM_ONLY == DcmToolKit::getDistributionMode ()) && (LOCATION_STAND_ALONE == FrameworkToolKit::getThisLocationRole ()) && (getMyDomainId() == getFCPrincipalDomainId()))
    {
        domainValidEventAlreadyReceivedAfterReboot = true;

        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : First domain valid event is being processed.");

        u_short fabDomainList[max_domain_list]  = {0};
        size_t  recv_size                       = (sizeof (u_short)) * 257;
        UI32    retCode                         = fabGetDomainList(fabDomainList, recv_size);

        if (0 != retCode)
        {
            tracePrintf (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : fabGetDomainList returned error. return value = %u", retCode);
        }
        else if ((1 == fabDomainList[0]) && (getMyDomainId() == fabDomainList[1]))
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : fabDomainList has only one domain");

            // Notify VIP Global worker to apply virtual ip.

            VirtualIpGlobalBindVipMessage *pVirtualIpGlobalBindVipMessage = new VirtualIpGlobalBindVipMessage ();

            ResourceId notifyVIPGlobalWorkerToApplyVIPSendStatus = sendOneWay (pVirtualIpGlobalBindVipMessage);

            if (WAVE_MESSAGE_SUCCESS == notifyVIPGlobalWorkerToApplyVIPSendStatus)
            {
                trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : successfully sent oneWay VirtualIpGlobalBindVipMessage.");
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : could not sendOneWay VirtualIpGlobalBindVipMessage. Status : ") + FrameworkToolKit::localize (notifyVIPGlobalWorkerToApplyVIPSendStatus));
            }
        }
    }

    LocationRole thisLocationRole           = FrameworkToolKit::getThisLocationRole ();
    bool         isUnconfirmedLocationRole  = ((thisLocationRole == LOCATION_SECONDARY_UNCONFIRMED)  || (thisLocationRole == LOCATION_PRIMARY_UNCONFIRMED));

    //State Transition for VcsFabricLocalObjectManager
    if (VCS_FABRIC_LOCAL_NODE_IS_OFFLINE == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

        if (getMyDomainId () == getFCPrincipalDomainId ())
        {
            startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
        }

        return (WAVE_MESSAGE_SUCCESS);
    }

    if (VCS_FABRIC_LOCAL_NODE_OFFLINE_TRIGGERED == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : Currently processing switch offline event handler,Appropriate Action will be taken once processing complete");
        return (WAVE_MESSAGE_SUCCESS);
    }

    if (VCS_FABRIC_LOCAL_CLUSTER_SWITCH_OFFLINE_BUT_ADD_REJOIN_IN_PROGRESS == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : Previous Add node request is not compelete, got domain valid after switch offline, waiting for previous request to complete"));

        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_CLUSTER_ADD_REJOIN_IN_PROGRESS);

        return (WAVE_MESSAGE_SUCCESS);
    }

    if (VCS_FABRIC_LOCAL_CLUSTER_CREATION_IN_PROGRESS == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState () || VCS_FABRIC_LOCAL_CLUSTER_ADD_REJOIN_IN_PROGRESS == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState () || VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler : Got Domain Valid Event in the middle of cluster operation, waiting for previous event to complete"));
        return (WAVE_MESSAGE_SUCCESS);
    }

    if (getMyDomainId() == getFCPrincipalDomainId () && true == isUnconfirmedLocationRole) 
    {
        // This will take care of cluster reboot
        startPrincipalTrackingTimer (2 * wait_domain_lists_sync);
        return (WAVE_MESSAGE_SUCCESS);
    }

    if ((getMyDomainId() == getFCPrincipalDomainId()) && (OldFCPrincipalDomainId != getFCPrincipalDomainId ()))
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::DomainValidEventHandler: This node is new principal DomainId  = ") + domainValidDomain + "Old principal domainId = "+ OldFCPrincipalDomainId);

        handleFCPrincipalChange ();

        return (WAVE_MESSAGE_SUCCESS);
    }

    if ((getMyDomainId() != getFCPrincipalDomainId()) && (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGED == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()))
    {
        // if the state is FC_PRINCIPAL_CHANGED, this means fc principal has changed earlier and this didn't receive heart beat failure with primary.
        // reset the state back to READY_TO_START_TIMER and wait either to for HB failure or this node to become primary again.
        disableTimer ();
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

        return (WAVE_MESSAGE_SUCCESS);
    }

    if ((getMyDomainId() == getFCPrincipalDomainId()) && (OldFCPrincipalDomainId == getFCPrincipalDomainId ()))
    {
        // This is node FC principal and previous principal was also this node but state is VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGED. This means after this node become new FC principal we have received domain invalid and domain valid. Handle both cases where Heartbeat failure also may or may not received. There is case where HB will not come when previous primary is still in the fabric. To handle this case, start timer and timer expiration will take care of this case.

        if (VCS_FABRIC_LOCAL_HEARTBEAT_TO_FC_PRINCIPAL_FAILED == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
        {
            disableTimer ();

            VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED);
            becomePrincipal ();

            return (WAVE_MESSAGE_SUCCESS);
        }
    
        if (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGED == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
        {
            startPrincipalTrackingTimer (wait_domain_lists_sync);
            return (WAVE_MESSAGE_SUCCESS);
        }
    }

    startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
    
    return (status);
}

void VcsNodeFCPrincipalTrackingWorker::handleFCPrincipalChange ()
{
    LocationRole currentLocationRole = FrameworkToolKit::getThisLocationRole ();

    if (LOCATION_STAND_ALONE == currentLocationRole)
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

        startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
        return;
    }

    if (LOCATION_PRIMARY == currentLocationRole)
    {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());

        synchronousQueryContext.addAndAttribute (new AttributeResourceId (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY, "specificStatus"));

        vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

        prismAssert (NULL != pResults, __FILE__, __LINE__);

        UI32 numberOfNodes = pResults->size ();

        if (0 == numberOfNodes)
        {
            VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

            startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
            return;
        }

        FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pMessage = new FrameworkObjectManagerSecondaryNodeFailureNotificationMessage ();

        for (UI32 i = 0; i < numberOfNodes; i++)
        {
            VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[i]);
    
            prismAssert (NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__);

            LocationId locationId = pVcsNodeLocalManagedObject->getLocationId ();

            if (false == FrameworkToolKit::isAConnectedLocation (locationId) && (locationId != FrameworkToolKit::getThisLocationId ()))
            {
                pMessage->addFailedLocationId (locationId);
            }
        }

        vector<LocationId> failedLocationIds = pMessage->getFailedLocationIds ();

        UI32 numberOfFailedNodes = failedLocationIds.size ();

        if (0 == numberOfFailedNodes)
        {
            delete pMessage;

            VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

            startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
            return;
        }

        if (CCM_ONLY == DcmToolKit::getDistributionMode ())
        {
            pMessage->setIsControlledFailoverInSpecialCase (true);
        }

        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED);

        WaveMessageStatus status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::handleFCPrincipalChangeCallback), NULL);

        prismAssert (WAVE_MESSAGE_SUCCESS == status, __FILE__, __LINE__);

        return;
    }

    if (VCS_FABRIC_LOCAL_HEARTBEAT_TO_FC_PRINCIPAL_FAILED == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED);
        becomePrincipal ();
    }
    else
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGED);

        // we need to start timer because heartbeat has not failed yet. After timer has expired, if HB has not failed, it will disconnect from the primary to create HB failure
        startPrincipalTrackingTimer (wait_domain_lists_sync);
    }
}

void VcsNodeFCPrincipalTrackingWorker::handleFCPrincipalChangeCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pMessage, void *pContext)
{
    prismAssert (NULL != pMessage, __FILE__, __LINE__);

    delete pMessage;

    if (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

        startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
    }
}

void VcsNodeFCPrincipalTrackingWorker::setReachableDomainBitForDomainId (const u_short &reachableDomain)
{
    // check if this domain is already in list
    if (! getInstance ()->m_knownDomainsList.reachableDomainsBitMap[reachableDomain]) 
    {
            getInstance ()->m_knownDomainsList.reachableDomainsBitMap.set (reachableDomain);
            getInstance ()->m_knownDomainsList.dcaDomainsBitMap.reset (reachableDomain);
    }
    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::setReachableDomainBitForDomainId: myDomainListSize = ")+m_dcaDomainListSize);
}

ResourceId VcsNodeFCPrincipalTrackingWorker::DomainReachableEventHandler (const VcsNodeDomainReachableEvent *&pEvent)
{
    prismAssert (NULL != pEvent, __FILE__, __LINE__);

    ResourceId  status          = WAVE_MESSAGE_SUCCESS;
    UI32        reachableDomain = pEvent->getDomainId ();

    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(pEvent->getEventId ());

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::DomainReachableEventHandler: Event not in queue skipping handling this event, unReachable DomainId  = ")+ reachableDomain);
        return (status);
    }

    static UI32 numberOfEventsRceivedSoFar = 0;

    numberOfEventsRceivedSoFar++;

    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::DomainReachableEventHandler Event count = ")+numberOfEventsRceivedSoFar);

    if (1 == numberOfEventsRceivedSoFar && false == m_knownDomainsList.reachableDomainsBitMap[getMyDomainId ()] )
    {
        initializeDomainList ();        // initialize the domain bit maps the first time DomainReachable is received
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::DomainReachableEventHandler domain list initialized"));
    }
    
    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::DomainReachableEventHandler DomainId for reachable domain = ")+ reachableDomain);

    setReachableDomainBitForDomainId (reachableDomain);
    cleanupTimerHandlerForDomainId (reachableDomain, true);

    return (status);
}

void VcsNodeFCPrincipalTrackingWorker::clearReachableDomainBitForDomainId (const u_short &unReachableDomain)
{
    // check if this domain was in the list before removing it
    if (getInstance ()->m_knownDomainsList.reachableDomainsBitMap[unReachableDomain]) 
    {
       getInstance ()->m_knownDomainsList.reachableDomainsBitMap.reset (unReachableDomain);
       if (getInstance ()->m_knownDomainsList.dcaDomainsBitMap[unReachableDomain])
       {
          if (1 > m_dcaDomainListSize)
          {
             WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::clearReachableDomainBitForDomainId: Error: dcaDomainList size = 0 but dcaBitMap for unreachable domain is set")+ "unreachable domainId = "+unReachableDomain);
          }
          else
          {
             getInstance ()->m_knownDomainsList.dcaDomainsBitMap.reset (unReachableDomain);
             m_dcaDomainListSize--;
          }
       }
       getInstance ()->m_knownDomainsList.switchInfoReceivedBitMap.reset (unReachableDomain);
       getInstance ()->m_knownDomainsList.remoteKnownDCABitMap.reset (unReachableDomain);
       }
    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::clearReachableDomainBitForDomainId: myDomainListSize = ")+m_dcaDomainListSize+" unreachable domainId = "+unReachableDomain);
}

ResourceId VcsNodeFCPrincipalTrackingWorker::switchReadyToAcceptCommandsEventHandler (const VcsNodeSwitchReadyToAcceptCommandsEvent *pSwitchReadyEvent)
{
    bool                isPostbootRequired                 = false;
    bool                isConfigReplayRequired             = false;
    bool                isNodeReadySequenceRequired        = true;
    bool                isBannerOutputToClientsRequired    = true;
    ResourceId          status = WAVE_MESSAGE_SUCCESS;

    prismAssert (NULL != pSwitchReadyEvent, __FILE__, __LINE__);
    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(pSwitchReadyEvent->getEventId ());   
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string("VcsNodeFCPrincipalTrackingWorker::switchReadyToAcceptCommandsEventHandler: Event not in queue so skipping handling this event"));
        return (status);
    }
     
    VcsFabricLocalObjectManager* pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance ();
    pVcsFabricLocalObjectManager->setFcfMacAddress ();
    
    PrismPostPersistenceBootMessage *pPrismPostPersistenceBootMessage = new PrismPostPersistenceBootMessage (isPostbootRequired, isConfigReplayRequired, isNodeReadySequenceRequired, isBannerOutputToClientsRequired);
    pPrismPostPersistenceBootMessage->setEventId (POSTBOOT_EVENT_SYSTEM_READY);

    WaveNs::trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::switchReadyToAcceptCommandsEventHandler: Triggering Node ready sequence processing"));

    status = sendOneWay (pPrismPostPersistenceBootMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_FATAL, "VcsNodeFCPrincipalTrackingWorker::switchReadyToAcceptCommandsEventHandler: Error sending Node ready sequence processing message.");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    return (status);
}

// TODO
ResourceId VcsNodeFCPrincipalTrackingWorker::switchActivationCompleteEventHandler (const VcsNodeSwitchActivationCompleteEvent *pSwitchActivationCompleteEvent)
{
    bool                isPostbootRequired                 = false;
    bool                isConfigReplayRequired             = false;
    bool                isNodeReadySequenceRequired        = true;
    bool                isBannerOutputToClientsRequired    = false;
    ResourceId   		status                             = WAVE_MESSAGE_SUCCESS;

    prismAssert (NULL != pSwitchActivationCompleteEvent, __FILE__, __LINE__);
    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(pSwitchActivationCompleteEvent->getEventId ());
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string("VcsNodeFCPrincipalTrackingWorker::switchActivationCompleteEventHandler: Event not in queue so skipping handling this event"));
        return (status);
    }

    PrismPostPersistenceBootMessage *pPrismPostPersistenceBootMessage = new PrismPostPersistenceBootMessage (isPostbootRequired, isConfigReplayRequired, isNodeReadySequenceRequired, isBannerOutputToClientsRequired);
    pPrismPostPersistenceBootMessage->setEventId (POSTBOOT_EVENT_SERVICE_READY);

    WaveNs::trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::switchActivationCompleteEventHandler: Triggering Activation Complete sequence processing"));

    status = sendOneWay (pPrismPostPersistenceBootMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_FATAL, "VcsNodeFCPrincipalTrackingWorker::switchActivationCompleteEventHandler: Error sending Node ready sequence processing message.");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
    return (status);
}


ResourceId VcsNodeFCPrincipalTrackingWorker::SwitchOfflineEventHandler (const VcsNodeSwitchOfflineEvent *&pEvent)
{
    prismAssert (NULL != pEvent, __FILE__, __LINE__);

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(pEvent->getEventId ());

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::SwitchOfflineEventHandler: event not in queue so skipping handling this event");
        return (status);
    }

    setDomainValid (false);
    setFCPrincipalDomainId (0);

    ResourceId currentVcsFabricLocalState = VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ();

    prismAssert (VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN != currentVcsFabricLocalState, __FILE__, __LINE__);
    prismAssert (VCS_FABRIC_LOCAL_CLUSTER_OPERATION_IN_PROGRESS != currentVcsFabricLocalState, __FILE__, __LINE__);


    // check if we can trigger switch offline.
    // We can cannot trigger switch offline in the middle of cluster formation, we have to wait for cluster formation to complete.
    // In Any of these state we will trigger switch offline
    // VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER, VCS_FABRIC_LOCAL_ENQ_MESSAGE_IN_PROCESS, VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGED, VCS_FABRIC_LOCAL_HEARTBEAT_TO_FC_PRINCIPAL_FAILED, VCS_FABRIC_LOCAL_CLUSTER_MERGE_REQUEST_IN_PROGRESS, VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED

    if (VCS_FABRIC_LOCAL_CLUSTER_CREATION_IN_PROGRESS == currentVcsFabricLocalState || VCS_FABRIC_LOCAL_CLUSTER_ADD_REJOIN_IN_PROGRESS == currentVcsFabricLocalState || VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED == currentVcsFabricLocalState)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::SwitchOfflineEventHandler: In current state can not trigger switch offline, need to wait for current operation ") + FrameworkToolKit::localize (currentVcsFabricLocalState) + string (" to complete."));

        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_CLUSTER_SWITCH_OFFLINE_BUT_ADD_REJOIN_IN_PROGRESS);
        return (WAVE_MESSAGE_SUCCESS);
    }

    if (VCS_FABRIC_LOCAL_NODE_IS_OFFLINE == currentVcsFabricLocalState || VCS_FABRIC_LOCAL_NODE_OFFLINE_TRIGGERED == currentVcsFabricLocalState || VCS_FABRIC_LOCAL_CLUSTER_SWITCH_OFFLINE_BUT_ADD_REJOIN_IN_PROGRESS == currentVcsFabricLocalState)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::SwitchOfflineEventHandler: Another switch online is in progress, no need to handle this."));
        return (WAVE_MESSAGE_SUCCESS);
    }

    VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_NODE_IS_OFFLINE);

    return (triggerSwitchOffline ());
}

ResourceId VcsNodeFCPrincipalTrackingWorker::triggerSwitchOffline ()
{
    // For switch offline trigger, all message should be directly sent to framework. If currently framework is processing any failover, then switch offline be queued in framework until framework complete failover.

    VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_NODE_OFFLINE_TRIGGERED);

    LocationRole currentLocationRole = FrameworkToolKit::getThisLocationRole ();
    ResourceId   status              = WAVE_MESSAGE_SUCCESS;

    if (DCM_CCM == DcmToolKit::getDistributionMode ())
    {
        if (LOCATION_STAND_ALONE == currentLocationRole)
        {
            if (true == getDomainValid ())
            {    
                VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);
                startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
            }    
            else 
            {
                VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_NODE_IS_OFFLINE);
            }

            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::triggerSwitchOffline: No Action required for node in standalone role");
            return (WAVE_MESSAGE_SUCCESS);
        }

        unbindVirtualIp ();

        FrameworkObjectManagerResetNodeToUnconfirmRole *pMessage = new FrameworkObjectManagerResetNodeToUnconfirmRole ();
   
        status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::ResetNodeToUnconfirmRoleCallBack), NULL);

        prismAssert (WAVE_MESSAGE_SUCCESS == status, __FILE__, __LINE__);

        return (status);
    }

    if (CCM_ONLY == DcmToolKit::getDistributionMode ())
    {
        unbindVirtualIp ();

        if (LOCATION_PRIMARY == currentLocationRole) 
        {
            FrameworkObjectManagerDestroyClusterMessage *pFrameworkObjectManagerDestroyClusterMessage = new FrameworkObjectManagerDestroyClusterMessage ();
            pFrameworkObjectManagerDestroyClusterMessage->setOriginalRequester (VcsFabricLocalObjectManager::getPrismServiceId ());
            pFrameworkObjectManagerDestroyClusterMessage->setIsRebootRequired (false);

            status = send (pFrameworkObjectManagerDestroyClusterMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::sendSwitchOfflineDeleteClusterMessageCallBack), NULL);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                prismAssert (false, __FILE__, __LINE__);
            }
        }
        else if (LOCATION_SECONDARY == currentLocationRole || LOCATION_PRIMARY_UNCONFIRMED == currentLocationRole || LOCATION_SECONDARY_UNCONFIRMED == currentLocationRole)
        {
            string ipAddress = FrameworkToolKit::getIpAddressForLocationId (FrameworkToolKit::getThisLocationId ());
            SI32   port      = FrameworkToolKit::getPortForLocationId (FrameworkToolKit::getThisLocationId ());

            FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage = new FrameworkObjectManagerUnconfigureClusterSecondaryMessage (ipAddress, port);

            status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::sendSwitchOfflineUnconfigureMessageCallBack), NULL);

            //  clear delayed HB nodes list since we are destroying the cluster            

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                prismAssert (false, __FILE__, __LINE__);
            }
        }
        else if (LOCATION_STAND_ALONE == currentLocationRole)
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::triggerSwitchOffline: No Action required for node in standalone role");

            if (true == getDomainValid ())
            {
                VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);
                startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
            }
            else
            {
                VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_NODE_IS_OFFLINE);
            }

            status = WAVE_MESSAGE_SUCCESS;
        }
        
        return (status);
    }

    return (status);
}

void VcsNodeFCPrincipalTrackingWorker::ResetNodeToUnconfirmRoleCallBack (FrameworkStatus frameworkStatus, FrameworkObjectManagerResetNodeToUnconfirmRole *pMessage, void *pContext)
{
    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pMessage != NULL, __FILE__, __LINE__);

        ResourceId completionStatus = pMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::ResetNodeToUnconfirmRoleCallBack successful in resetting node to unconfirm role");

            VcsFabricLocalObjectManager* pVcsFabricLocalObjectManager =  VcsFabricLocalObjectManager::getInstance ();

            startTransaction ();

            pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectIsPrincipal   (false);
            pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectInVcs         (false);
            pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectStatus        (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD, VCS_FABRIC_LOCAL_SPECIFIC_STATUS_WAITING_TO_REJOIN);

            pVcsFabricLocalObjectManager->updateThisVcsNodeLocalManagedObject ();

            ResourceId status = commitTransaction ();

            if (FRAMEWORK_SUCCESS != status)
            {
                prismAssert (false, __FILE__, __LINE__);
            }
        }
        else if (WAVE_MESSAGE_ERROR_CLUSTER_OPERATION_IN_PROGRESS == completionStatus)
        {
            // do nothing
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }

        delete pMessage;
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    if (VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {
        // no need to do any thing. This is possible that we received reply after node is shutdown
        return;
    }

    if (true == getDomainValid ())
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);
        startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
    }
    else
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_NODE_IS_OFFLINE);
    }
}

void VcsNodeFCPrincipalTrackingWorker::sendSwitchOfflineUnconfigureMessageCallBack (FrameworkStatus frameworkStatus, FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage, void *pContext)
{
    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pMessage != NULL, __FILE__, __LINE__);

        ResourceId completionStatus = pMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::sendSwitchOfflineUnconfigureMessageCallBack successfully deleted the cluster on chassis disable");
        }
        else if (FRAMEWORK_ERROR_CANNOT_UNCONFIGURE_LOCATION_WITH_IN_CLUSTER_PHASES_ROLE == completionStatus)
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::sendSwitchOfflineUnconfigureMessageCallBack unconfigure failed, node is in the process of joining the cluster");
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }

        delete pMessage;
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    if (VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {
        // no need to do any thing. This is possible that we received reply after node is shutdown
        return;
    }

    if (true == getDomainValid ())
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);
        startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
    }
    else
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_NODE_IS_OFFLINE);
    }
}
                                     
void VcsNodeFCPrincipalTrackingWorker::sendSwitchOfflineDeleteClusterMessageCallBack (FrameworkStatus frameworkStatus, FrameworkObjectManagerDestroyClusterMessage *pFrameworkObjectManagerDestroyClusterMessage, void *pContext)
{
    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pFrameworkObjectManagerDestroyClusterMessage != NULL, __FILE__, __LINE__);

        ResourceId completionStatus = pFrameworkObjectManagerDestroyClusterMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::sendSwitchOfflineDeleteClusterMessageCallBack successfully deleted the cluster on chassis disable");
        }
        else if (WAVE_CLUSTER_DELETE_STATUS_CLUSTER_NOT_FOUND == completionStatus)
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::sendSwitchOfflineDeleteClusterMessageCallBack cluster not found");
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }

        delete pFrameworkObjectManagerDestroyClusterMessage;
    } 
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    if (VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {    
        // no need to do any thing. This is possible that we received reply after node is shutdown
        return;
    }    

    if (true == getDomainValid ())
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);
        startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
    }
    else
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_NODE_IS_OFFLINE);
    }
}
                                                                                                                                                   
ResourceId VcsNodeFCPrincipalTrackingWorker::DomainUnReachableEventHandler     (const VcsNodeDomainUnreachableEvent *&pEvent)
{
    prismAssert (NULL != pEvent, __FILE__, __LINE__);

    ResourceId  status              = WAVE_MESSAGE_SUCCESS;
    UI32        unreachableDomain   = pEvent->getDomainId ();

    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(pEvent->getEventId ());

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::DomainUnReachableEventHandler Event not in queue so skipping handling this event domainId = ")+unreachableDomain);
        return (status);
    }

    static UI32                 numberOfEventsRceivedSoFar = 0;
    VcsNodeTimerHandler        *pVcsNodeTimerHandler = NULL;
    VcsNodeLocalManagedObject  *pVcsNodeLocalManagedObject = NULL;
    TimerHandle                 timerHandle;
    LocationId                  unreachableNodeLocationId  = 0;
  
    numberOfEventsRceivedSoFar++;

    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::DomainUnReachableEventHandler DomainId for Unreachable domain = ")+unreachableDomain);

    clearReachableDomainBitForDomainId (unreachableDomain);

    pVcsNodeLocalManagedObject = getSwitchObjectFromDomainId (unreachableDomain);

    if (NULL == pVcsNodeLocalManagedObject)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::DomainUnReachableEventHandler: switch not in my cluster domain Id = ")+ unreachableDomain);
    }
    else
    {
        unreachableNodeLocationId = pVcsNodeLocalManagedObject->getLocationId ();
    }

    /* Check if process cleanup timer for this domain exists - If found, delete it */
    cleanupTimerHandlerForDomainId (unreachableDomain, true);

    /* Start timer to disconnect from location */
    pVcsNodeTimerHandler = new VcsNodeTimerHandler (unreachableDomain, unreachableNodeLocationId);

    pVcsNodeTimerHandler->setServerIpAddress (getIpAddressFromDomainId (unreachableDomain));
    pVcsNodeTimerHandler->setServerPort (getMgmtPort (unreachableDomain));

    status = startTimer (timerHandle, wait_domain_lists_sync, reinterpret_cast<PrismTimerExpirationHandler> (&VcsNodeFCPrincipalTrackingWorker::processCleanupTimerForDomainId), (void *) pVcsNodeTimerHandler);
    
    if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::DomainUnReachableEventHandler: Timer starting for DomainId = ") + unreachableDomain);

        pVcsNodeTimerHandler->setTimerHandle (timerHandle),
        m_vcsNodeTimerHandler.push_back (pVcsNodeTimerHandler);
        //Return WAVE_MESSAGE_SUCCESS else the caller will not broadcast this event to any plugin that has registered for this event
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("VcsNodeFCPrincipalTrackingWorker::DomainUnReachableEventHandler: Could not be start timer for DomainId = ") + unreachableDomain + " with status = " + FrameworkToolKit::localize (status));
        delete (pVcsNodeTimerHandler);
    }

    if (NULL != pVcsNodeLocalManagedObject)
    {
        delete (pVcsNodeLocalManagedObject);
        pVcsNodeLocalManagedObject = NULL;
    }

    return (status);
}

void VcsNodeFCPrincipalTrackingWorker::setDCADomainBitForDomainId (const u_short &dcaDomain)
{
    string dcaBitMapstr;
    if (getInstance ()->m_knownDomainsList.reachableDomainsBitMap[dcaDomain])
    {
        if (false == getInstance ()->m_knownDomainsList.dcaDomainsBitMap[dcaDomain])
        {       
            // set dca bit for this domain if not already set
            getInstance ()->m_knownDomainsList.dcaDomainsBitMap.set(dcaDomain);
            dcaBitMapstr = getInstance ()->m_knownDomainsList.dcaDomainsBitMap.toString ();
            m_dcaDomainListSize++;

            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::setDCADomainBitForDomainId dcaDomainsBitMap = ")+ dcaBitMapstr);
        }
        else
        {
            m_isNeedToStartTimerForWarmRecovery = true;
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::setDCADomainBitForDomainId dcaDomainsBitMap not set because domain is not in reachable domain list, dcaDomain = ")+ dcaDomain);
    }
}


ResourceId VcsNodeFCPrincipalTrackingWorker::DomainCapabilitiesAvailableEventHandler (const VcsNodeDomainCapabilitiesAvailableEvent *&pEvent)
{
    prismAssert (NULL != pEvent, __FILE__, __LINE__);

    ResourceId  statusOfPop = WAVE_MESSAGE_SUCCESS;
    UI32        dcaDomainId = pEvent->getDomainId ();

    statusOfPop = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(pEvent->getEventId ());  

    if (WAVE_MESSAGE_SUCCESS != statusOfPop)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::DomainCapabilitiesAvailableEventHandler: DCA event not in queue, skipping handling this event dcaDomainId = ")+ dcaDomainId);
        return (statusOfPop);
    }

    static UI32 numberOfEventsRceivedSoFar = 0;

    numberOfEventsRceivedSoFar++;

    UI32 myDomainId = getMyDomainId ();

    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::DomainCapabilitiesAvailableEventHandler: DomainId for DCA domain = ")+ dcaDomainId);

    // if Domain is COS capable, then only set DCA bit else reset the DR bit also.

    int status = 0,is_cos = 0;

    status =  essGetIsCOSDomain( pEvent->getDomainId (), &is_cos, DCMLOG);

    if (ESS_ENOTSUPPORTED == status)
    {
        tracePrintf (TRACE_LEVEL_ERROR, "VcsFabricLocalObjectManager::domainCapabilitiesAvailableEventHandler : essGetIsCOSDomain returned error. return value = %d . Resetting DR bit", status);

        clearReachableDomainBitForDomainId (dcaDomainId);

        if (myDomainId != getFCPrincipalDomainId ())
        {
            // this node is not FC Principal so nothing to do
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::DomainCapabilitiesAvailableEventHandler: this node is not FC Principal so do nothing");
        }
        else
        {
            startPrincipalTrackingTimer (wait_domain_lists_sync);
        }

        return (WAVE_MESSAGE_SUCCESS); 
    }
    else
    {
        if (is_cos != true)
        {
            trace (TRACE_LEVEL_INFO, "VcsFabricLocalObjectManager::domainCapabilitiesAvailableEventHandler : essGetIsCOSDomain is_cos is false. Resetting DR bit");
        
            clearReachableDomainBitForDomainId (dcaDomainId);

            if (myDomainId != getFCPrincipalDomainId ())
            {
                // this node is not FC Principal so nothing to do
                trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::DomainCapabilitiesAvailableEventHandler: this node is not FC Principal so do nothing");
            }
            else
            {
                startPrincipalTrackingTimer (wait_domain_lists_sync);
            }

            return (WAVE_MESSAGE_SUCCESS);  
        }
        else
        {
            if (false == m_knownDomainsList.reachableDomainsBitMap [dcaDomainId])
            {
                setReachableDomainBitForDomainId (dcaDomainId);
            }

            setDCADomainBitForDomainId (dcaDomainId);
        }
    }

    if (myDomainId != getFCPrincipalDomainId ())
    {
        // this node is not FC Principal so nothing to do
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::DomainCapabilitiesAvailableEventHandler: this node is not FC Principal so do nothing");
        // returning success to maintain consistency with previous behavior, the caller is only looking for wheather the pop event was successful not anything else
        return (WAVE_MESSAGE_SUCCESS);  
    }

    trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::DomainCapabilitiesAvailableEventHandler: Node is FC Principal, starting Principal Tracking Timer");

    startPrincipalTrackingTimer (wait_domain_lists_sync);
    return (statusOfPop);
}

void VcsNodeFCPrincipalTrackingWorker::relenquishPrincipal ()
{
    trace(TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::relenquishPrincipal: Current Role  = ") + FrameworkToolKit::localize(FrameworkToolKit::getThisLocationRole ()));

    if (FrameworkToolKit::getThisLocationRole () == LOCATION_PRIMARY_UNCONFIRMED) 
    {
        trace(TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::relenquishPrincipal: I am Unconfirm Primary Role"));
    } 
    else 
    {
        // Bao : Need code to do Principal selection 
        trace(TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::relenquishPrincipal: I am Principal Role : need to do Tracking "));
    }
    return;
}

UI32 VcsNodeFCPrincipalTrackingWorker::wereAllSwitchesAdded (BitMap &switchesNotInCluster)
{
    BitMap domainsInCluster;
    setBitMapSize (domainsInCluster);
    domainsInCluster.reset ();
    UI32 numNodes = getDomainsInCluster (domainsInCluster);

    UI32 i;
    UI32 nNotInCluster = 0;
    switchesNotInCluster.reset ();

    for (i = 1; i < (max_domain_list - 1); i++)
    {
        if (getInstance ()->m_knownDomainsList.dcaDomainsBitMap[i])
        {
            if (! domainsInCluster[i])
            {
                    switchesNotInCluster.set (i);
                    nNotInCluster++;
            }
        }
    }
        
    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::wereAllSwitchesAdded: Num switches in cluster = ")+ numNodes+ " Num not in cluster = "+nNotInCluster);
    return nNotInCluster;
}

UI32 VcsNodeFCPrincipalTrackingWorker::setPrincipalFlag (bool principalFlag)
{
        //VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = getNodeLocalManagedObjectForThisNode ();

    UI32 localSwitchDomainId = getMyDomainId ();

    startTransaction ();

    VcsFabricLocalObjectManager* pVcsFabricLocalObjectManager =  VcsFabricLocalObjectManager::getInstance ();
    
    pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectIsPrincipal(principalFlag);
    if (true == principalFlag)
    {
        pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectInVcs (true);
        pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectStatus (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD,VCS_FABRIC_LOCAL_SPECIFIC_STATUS_PRIMARY);
    }
    pVcsFabricLocalObjectManager->updateThisVcsNodeLocalManagedObject ();

    ResourceId status = commitTransaction ();
    //delete pVcsNodeLocalManagedObject;
    if (FRAMEWORK_SUCCESS != status)
    {
        WaveNs::trace(TRACE_LEVEL_ERROR, string("VcsNodeFCPrincipalTrackingWorker::setPrincipalFlag: error setting isPrincipal flag ")+ FrameworkToolKit::localize (status));
        return GENERIC_ERROR;
    }

    WaveNs::trace(TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::setPrincipalFlag: isPrincipal set for switch with DomainId = ")+ localSwitchDomainId);

    return VCS_SUCCESS;
}

void VcsNodeFCPrincipalTrackingWorker::doControlledPrincipalChangeOver (UI32 fcPrincipalDomainId)
{
    // DCM  framework needs to support this case, till then not much we can do
    return;
}

bool VcsNodeFCPrincipalTrackingWorker::isThereAnotherVcsPrincipalInThisCluster (vector<UI32> &domainIds)
{
    UI32 i;
    UI32 myDomainId = getMyDomainId ();
    
    if (false == validDomainId (myDomainId))
    {
        return false;
    }

    for (i = 0; i < m_remoteSwitchInfoList.size (); i++)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::isThereAnotherVcsPrincipalInCluster: domainId = ") + m_remoteSwitchInfoList[i].getSwitchDomainId () + string (" Principal = ") + m_remoteSwitchInfoList[i].getIsPrincipal () + string (" UnconfirmedPrincipal = ") + m_remoteSwitchInfoList [i].getIsSwitchUnconfirmPrincipal ());

        if ((m_remoteSwitchInfoList [i].getSwitchDomainId() != myDomainId) && isSwitchConnectedToThisCluster (m_remoteSwitchInfoList [i].getSwitchDomainId ()) && m_remoteSwitchInfoList[i].getIsPrincipal () && (false == m_remoteSwitchInfoList[i].getIsSwitchUnconfirmPrincipal ()))
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::isThereAnotherVcsPrincipalInCluster: another principal found in cluster domainId = ")+m_remoteSwitchInfoList[i].getSwitchDomainId ());

            domainIds.push_back (m_remoteSwitchInfoList[i].getSwitchDomainId ());
        }
    }

    if (0 != domainIds.size ())
    {
        return true;
    }

    return false;
}

bool VcsNodeFCPrincipalTrackingWorker::isNodeInTrivialMergeState (vector<WaveManagedObject *> *pResults, UI32 domainId)
{
    UI32                        i;
    UI32                        numLocalNodes               = pResults->size ();
    UI32                        myDomainId                  = getMyDomainId ();
    VcsNodeLocalManagedObject  *pVcsNodeLocalManagedObject  = NULL;

    for (i = 0; i < numLocalNodes; i++)
    {
        pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[i]);

        prismAssert (NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__);

        if ((domainId != myDomainId) && (domainId == pVcsNodeLocalManagedObject->getDomainId()))
        {
            if (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_DISCONNECTED_FROM_CLUSTER == pVcsNodeLocalManagedObject->getSpecificStatus ())
            {    
                return true;    
            }
        }
    }
    return false;
}

bool VcsNodeFCPrincipalTrackingWorker::isThereANodeForTrivialMerge (vector<UI32> &domainForTrivialMerge)
{
    vector<WaveManagedObject *> *pResults = querySynchronously (VcsNodeLocalManagedObject::getClassName ());

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    if (0 >= pResults->size ())
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::isThereANodeForTrivialMerge: No VcsNodeLocalManagedObjects found"));
        return false;
    }
    
    UI32 i;
    UI32 myDomainId = getMyDomainId ();
    
    for (i = 0; i < m_remoteSwitchInfoList.size (); i++)
    {
        UI32         remoteSwitchDomainId          = m_remoteSwitchInfoList[i].getSwitchDomainId ();
        UI32         remoteSwitchPrincipalDomainId = m_remoteSwitchInfoList[i].getPrincipalSwitchDomainId ();
        UI32         remoteSwitchLocationRole      = m_remoteSwitchInfoList[i].getLocationRole ();

        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::isThereANodeForTrivialMerge: node domainId = ") + remoteSwitchDomainId + string (" remote node PrincipalDomainId = ") + remoteSwitchPrincipalDomainId + string(" location role = ") + FrameworkToolKit::localize (remoteSwitchLocationRole));

        if (myDomainId != remoteSwitchPrincipalDomainId && LOCATION_STAND_ALONE != remoteSwitchLocationRole && LOCATION_SECONDARY_UNCONFIRMED != remoteSwitchLocationRole && LOCATION_PRIMARY_UNCONFIRMED != remoteSwitchLocationRole && isNodeInTrivialMergeState (pResults, remoteSwitchDomainId))
        {
            // A node has a principal that is not this node and it's node specific status is disconnected from cluster
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::isThereANodeForTrivialMerge: Node found for trivial merge domainId = ") + m_remoteSwitchInfoList[i].getSwitchDomainId ());

            domainForTrivialMerge.push_back (remoteSwitchDomainId);
        }
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
    
    UI32 numberOfNodesForTrivialMerge = domainForTrivialMerge.size ();

    if (0 == numberOfNodesForTrivialMerge)
    {
        return false;
    }

    return true;
}

bool VcsNodeFCPrincipalTrackingWorker::fabAndMyDomainListsMatch (u_short *fabDomainList)
{
    u_short i;
    string dcaBitMap       = getInstance ()->m_knownDomainsList.dcaDomainsBitMap.toString ();
    string reachableBitMap = getInstance ()->m_knownDomainsList.reachableDomainsBitMap.toString ();
    u_short fabDomainCount = fabDomainList[0];


    if (0 == fabDomainCount) return false;

    if (m_knownDomainsList.reachableDomainsBitMap != m_knownDomainsList.dcaDomainsBitMap)
    {
        trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::fabAndMyDomainListsMatch: dca not received for all reachable domains ") + "reachableDomainBitMap = "+reachableBitMap+" dcaDomainBitMap = "+dcaBitMap);
        return false;   
    }

    for (i = 0; i < fabDomainCount; i++)
    {
        if (false == (m_knownDomainsList.reachableDomainsBitMap [fabDomainList[i+1]] && m_knownDomainsList.dcaDomainsBitMap [fabDomainList[i+1]]))
        {
            trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::fabAndMyDomainListsMatch: fabDomainList and myDomainLists don't match, mismatch at fabDominiID = ") + fabDomainList[i+1] + string (" i = ") + i);
            trace (TRACE_LEVEL_INFO, string("Domain Reachable bitmap ") + getInstance ()->m_knownDomainsList.reachableDomainsBitMap.toString () + " != DCA bitmap " +
            getInstance ()->m_knownDomainsList.dcaDomainsBitMap.toString());

            trace (TRACE_LEVEL_INFO, string ("m_knownDomainsList.reachableDomainsBitMap ") + m_knownDomainsList.reachableDomainsBitMap [fabDomainList [i+1]]);
            trace (TRACE_LEVEL_INFO, string ("m_knownDomainsList.dcaDomainsBitMap ") + m_knownDomainsList.dcaDomainsBitMap [fabDomainList [i+1]]);

            return false;
        }
    }

    if ((m_dcaDomainListSize != fabDomainCount) && (1 != fabDomainCount))
    {
        trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::fabAndMyDomainListsMatch: fabDomainList and myDomainLists don't match, size is different: ") + "fabDomainListSize = "+fabDomainCount+" myDomainListSize = "+m_dcaDomainListSize );
        return false;
    }
    else
    {
        return true;
    }
}

void VcsNodeFCPrincipalTrackingWorker::clearRemoteSwitchInfoList ()
{
    UI32 i;
    UI32 domainId;
    if (! m_remoteSwitchInfoList.empty ())
    {
        for (i = 0; i < getInstance ()->m_remoteSwitchInfoList.size (); i++)
        {
            domainId = (getInstance ()->m_remoteSwitchInfoList)[i].getSwitchDomainId ();
            if (! isSwitchInfoRetrievedForDomainId (domainId))
            {
                // remove switches for which remote switch info has not been received
                (getInstance ()->m_remoteSwitchInfoList).erase ((getInstance ()->m_remoteSwitchInfoList).begin ()+i);
                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::clearRemoteSwitchInfoList: switchInfo removed for domainId = ")+domainId);
            }
        }
    }
}

void VcsNodeFCPrincipalTrackingWorker::clearSwitchInfoRetrievedBitMap ()
{
    getInstance ()->m_knownDomainsList.switchInfoReceivedBitMap.reset ();
}

void VcsNodeFCPrincipalTrackingWorker::setSwitchInfoRetrievedBit (const u_short &infoRetrievedDomain)
{
    m_knownDomainsList.switchInfoReceivedBitMap.set (infoRetrievedDomain);
}

UI32 VcsNodeFCPrincipalTrackingWorker::getLocalNodeVcsId ()
{
    const ObjectId  localSwitchObjectId = (VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeObjectId ();
    VcsNodeLocalManagedObject * pVcsNodeLocalManagedObject = getSwitchObjectFromObjectId(localSwitchObjectId);
    prismAssert (NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__);
    UI32 vcsId = pVcsNodeLocalManagedObject->getVcsId ();
    delete pVcsNodeLocalManagedObject;
    return vcsId;
}

UI32 VcsNodeFCPrincipalTrackingWorker::getVcsClusterId (bool &clusterExists)
{
    VcsClusterManagedObject *pVcsClusterManagedObject = NULL;
    UI32                     vcsClusterId = 0;

    clusterExists = false;

    vector<WaveManagedObject *> *pResultsCluster = fabOMDoSynchronousQuery (VcsClusterManagedObject::getClassName ());

    prismAssert (1 >= pResultsCluster->size (), __FILE__, __LINE__);

    if (1 == pResultsCluster->size())
    {
        clusterExists = true;

        if (NULL == (*pResultsCluster)[0])
        {
            trace (TRACE_LEVEL_FATAL, string ("VcsNodeFCPrincipalTrackingWorker::getVcsClusterId: DB query for VcsClusterManagedObject returned one object but results pointer is null - this shouldn't happen so assert"));
            prismAssert (false, __FILE__, __LINE__);
        }

        pVcsClusterManagedObject = dynamic_cast<VcsClusterManagedObject *>((*pResultsCluster)[0]);

        vcsClusterId = pVcsClusterManagedObject->getVcsClusterId ();

        delete pVcsClusterManagedObject;

        pResultsCluster->clear ();
        delete pResultsCluster;
    }
    else
    {
        delete pResultsCluster;
    }

    return vcsClusterId;
}

bool VcsNodeFCPrincipalTrackingWorker::isSwitchInfoRetrievedForDomainId (const u_short &domainId)
{
        return getInstance ()->m_knownDomainsList.switchInfoReceivedBitMap[domainId];
}

bool VcsNodeFCPrincipalTrackingWorker::isSwitchDomainIdInMyCluster (UI32 domainId)
{
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = getSwitchObjectFromDomainId (domainId);

    if (NULL == pVcsNodeLocalManagedObject)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::isSwitchDomainIdInMyCluster: switch not in my cluster domainId = ")+ domainId);
        return false;
    }
    else
    {
        delete pVcsNodeLocalManagedObject;
        return true;
    }
}

bool VcsNodeFCPrincipalTrackingWorker::isSwitchDomainIdInMyCluster (const VcsFabricLocalSwitchInfoObject &switchInfo)
{
    return isSwitchDomainIdInMyCluster (switchInfo.getSwitchDomainId ());
}

bool VcsNodeFCPrincipalTrackingWorker::isSwitchConnectedToThisCluster (UI32 domainId)
{
    if (getMyDomainId () == domainId)
    {
        return true;
    }
    
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = getSwitchObjectFromDomainId (domainId);

    if (NULL == pVcsNodeLocalManagedObject)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::isSwitchInConnectedState: switch not in my cluster domainId = ")+ domainId);
        return false;
    }
    else
    {
        if ((pVcsNodeLocalManagedObject->getSpecificStatus() == VCS_FABRIC_LOCAL_SPECIFIC_STATUS_PRIMARY) || (pVcsNodeLocalManagedObject->getSpecificStatus() == VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY))
        {
            delete pVcsNodeLocalManagedObject;
            return true;
        }
        else
        {
            delete pVcsNodeLocalManagedObject;
            return false;
        }
    }

}

void VcsNodeFCPrincipalTrackingWorker::addSwitchesNotInCluster ()
{
    trace (TRACE_LEVEL_INFO, "Add Node");
    // do add switch for switches not in cluster
    VcsAddSwitchMessage *pAddSwitchMessage  = new VcsAddSwitchMessage (principal_selection_worker_id);
    vector<UI32>         domainIds;

    VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_CLUSTER_ADD_REJOIN_IN_PROGRESS);

    pAddSwitchMessage->setRemoteSwitchInfoObjects (m_remoteSwitchInfoList);

    // send Message to Vcs Fab Object Manager for add node/rejoin

    ResourceId status = send (pAddSwitchMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::addMessageCallback), NULL);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "VcsNodeFCPrincipalTrackingWorker::addSwitchesNotInCluster Not Able to send message to Vcs Fab Plugin. Asserting");
        prismAssert (false, __FILE__, __LINE__);
    }

    return;
}

bool VcsNodeFCPrincipalTrackingWorker::amIVcsPrincipal ()
{
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = getNodeLocalManagedObjectForThisNode ();

    if (NULL == pVcsNodeLocalManagedObject)
    {
        return false;
    }
    else
    {
        if (pVcsNodeLocalManagedObject->getIsPrincipal ())
        {
            delete pVcsNodeLocalManagedObject;
            return true;
        }
        else
        {
            delete pVcsNodeLocalManagedObject;
            return false;
        }
    }
}

void VcsNodeFCPrincipalTrackingWorker::clusterMergeMessageAsyncCallBack (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    VcsClusterMergeMessage         *pVcsClusterMergeMessage        = dynamic_cast<VcsClusterMergeMessage *>(pPrismMessage);
    VcsNodeRemoteMessagingContext  *pVcsNodeRemoteMessagingContext = reinterpret_cast<VcsNodeRemoteMessagingContext *> (pContext);

    prismAssert (NULL != pVcsNodeRemoteMessagingContext, __FILE__, __LINE__);

    --(*pVcsNodeRemoteMessagingContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pVcsClusterMergeMessage, __FILE__, __LINE__);

        UI32 remoteNodeDomainId = pVcsClusterMergeMessage->getRemoteNodeDomainId ();

        pVcsNodeRemoteMessagingContext->disconnectFromServer (remoteNodeDomainId);

        if (WAVE_MESSAGE_SUCCESS != pVcsClusterMergeMessage->getCompletionStatus ())
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::clusterMergeMessageAsyncCallBack error returned by domain ") + remoteNodeDomainId + string (" reason ") + FrameworkToolKit::localize (pVcsClusterMergeMessage->getCompletionStatus ()));
            removeSwitchInfo (remoteNodeDomainId);
        }
    }
    else
    {
        if (NULL != pVcsClusterMergeMessage)
        {
            UI32 remoteNodeDomainId = pVcsClusterMergeMessage->getRemoteNodeDomainId ();

            pVcsNodeRemoteMessagingContext->disconnectFromServer (remoteNodeDomainId);
            removeSwitchInfo (remoteNodeDomainId);
        }
    }

    if (NULL != pVcsClusterMergeMessage)
    {
        delete (pVcsClusterMergeMessage);
    }

    pVcsNodeRemoteMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsNodeFCPrincipalTrackingWorker::sendClusterMergeMessagesToRemoteLocationsStep (VcsNodeRemoteMessagingContext *pVcsNodeRemoteMessagingContext)
{
    vector<UI32> trivialMergeNodes = pVcsNodeRemoteMessagingContext->getDomainIds ();
    UI32         numberOfNodes     = trivialMergeNodes.size ();
    UI32         myDomainId        = getMyDomainId ();
    string       clientName        = getIpAddressFromDomainId (myDomainId) + ":" + getMgmtPort (myDomainId);

    ++(*pVcsNodeRemoteMessagingContext);

    for (UI32 i = 0; i < numberOfNodes; i++)   // the first element of the buffer contains the # domains returned
    {
        UI32    remoteSwitchDomainId             = trivialMergeNodes [i];
        string  remoteSwitchClusterMgmtIpaddress = getIpAddressFromDomainId (remoteSwitchDomainId);
        UI32    remoteSwitchClusterMgmtPort      = getMgmtPort              (remoteSwitchDomainId);

        // Create ValclientSynchronous connnection
        // sendToWaveServer for the message with Callback, set ValClientSynchronous connection in the context.

        ValClientSynchronousConnection *pValClientSynchronousConnection = new ValClientSynchronousConnection (clientName, remoteSwitchClusterMgmtIpaddress, remoteSwitchClusterMgmtPort);

        VcsClusterMergeMessage *pVcsClusterMergeMessage = new VcsClusterMergeMessage ();

        pVcsClusterMergeMessage->setRemoteNodeDomainId  (remoteSwitchDomainId);

        // connect to server
        stringstream clientNameExt;
        clientNameExt << s_connectionSequenceNumber;

        string clientNameExtenstion = ":" + clientNameExt.str ();
        s_connectionSequenceNumber ++;

        ResourceId connectionStatus = pValClientSynchronousConnection->connect (clientNameExtenstion);

        if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER != connectionStatus)
        {
            delete pValClientSynchronousConnection;

            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::sendClusterMergeMessagesToRemoteLocationsStep Not able to conenct to remote node ") + remoteSwitchDomainId);

            removeSwitchInfo (remoteSwitchDomainId);
        }
        else
        {
            pVcsNodeRemoteMessagingContext->addToValClientSynchronousConnection (remoteSwitchDomainId, pValClientSynchronousConnection);

            ResourceId status = pValClientSynchronousConnection->sendToWaveServer (pVcsClusterMergeMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::clusterMergeMessageAsyncCallBack), this, pVcsNodeRemoteMessagingContext);

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                ++ (*pVcsNodeRemoteMessagingContext);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::sendClusterMergeMessagesToRemoteLocationsStep : not able to send an cluster merge Message to Domain Id ") + remoteSwitchDomainId);
                removeSwitchInfo (remoteSwitchDomainId);
            }
        }
    }

    --(*pVcsNodeRemoteMessagingContext);

    pVcsNodeRemoteMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);  // Call execute next step, if any enq message is sent out then this
}

void VcsNodeFCPrincipalTrackingWorker::handleClusterMergeRejoinAndAddNode (vector<UI32> &domainForTrivialMerge)
{
    trace (TRACE_LEVEL_DEBUG, "VcsNodeFCPrincipalTrackingWorker::clusterMergeSequencer: Entering...");

    PrismLinearSequencerStep sequencerSteps[] =
    {    
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeFCPrincipalTrackingWorker::sendClusterMergeMessagesToRemoteLocationsStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeFCPrincipalTrackingWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeFCPrincipalTrackingWorker::prismLinearSequencerFailedStep)
    };   
    
    PrismAsynchronousContext *pPrismAsynchronousContext = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&VcsNodeFCPrincipalTrackingWorker::handleClusterMergeRejoinAndAddNodeCallback));

    VcsNodeRemoteMessagingContext *pVcsNodeRemoteMessagingContext = new VcsNodeRemoteMessagingContext (pPrismAsynchronousContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    prismAssert (NULL != pVcsNodeRemoteMessagingContext, __FILE__, __LINE__);

    pVcsNodeRemoteMessagingContext->setDomainIds (domainForTrivialMerge);

    VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_CLUSTER_MERGE_REQUEST_IN_PROGRESS);

    pVcsNodeRemoteMessagingContext->start ();
}

void VcsNodeFCPrincipalTrackingWorker::handleClusterMergeRejoinAndAddNodeCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    prismAssert (NULL != pPrismAsynchronousContext, __FILE__, __LINE__);

    if (VCS_FABRIC_LOCAL_CLUSTER_MERGE_REQUEST_IN_PROGRESS == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {
        if (m_remoteSwitchInfoList.size () > 1)
        { 
            addSwitchesNotInCluster ();
        }
        else
        {
            VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

            startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
        }
    }
    else
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

        startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
    }
}

void VcsNodeFCPrincipalTrackingWorker::createClusterAndAddSwitches ()
{
    trace (TRACE_LEVEL_INFO, "Create Cluster");

    VcsAddSwitchMessage *pAddSwitchMessage = new VcsAddSwitchMessage (principal_selection_worker_id);

    VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_CLUSTER_CREATION_IN_PROGRESS);

    VcsFabricLocalObjectManager* pVcsFabricLocalObjectManager =  VcsFabricLocalObjectManager::getInstance ();

    startTransaction ();
    
    pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectIsPrincipal   (true);
    pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectInVcs         (true);
    pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectStatus        (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD, VCS_FABRIC_LOCAL_SPECIFIC_STATUS_PRIMARY);

    pVcsFabricLocalObjectManager->updateThisVcsNodeLocalManagedObject ();

    ResourceId status = commitTransaction ();

    if (FRAMEWORK_SUCCESS != status)
    {    
        prismAssert (false, __FILE__, __LINE__);
    }    

    pAddSwitchMessage->setRemoteSwitchInfoObjects (m_remoteSwitchInfoList);

    status = send (pAddSwitchMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::addMessageCallback), NULL);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

void VcsNodeFCPrincipalTrackingWorker::becomePrincipal ()
{
    // Become Principal Means: This node has received heartbeat Failure for Primary and this node is FC principal.
    // At this point, VcsFabricLocal Should be in  VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGED state, if not it means it has not received Domain Valid. This function should not be called.

    
    prismAssert  (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState (), __FILE__, __LINE__);

    string      previousPrimaryIpAddress  = FrameworkToolKit::getClusterPrimaryIpAddress   ();
    UI32        previousPrimaryPort       = (UI32) FrameworkToolKit::getClusterPrimaryPort ();
    LocationId  previousPrimaryLocationId = FrameworkToolKit::getClusterPrimaryLocationId  ();

    FrameworkObjectManagerNewPrincipalEstablishedMessage *pFrameworkObjectManagerNewPrincipalEstablishedMessage = new FrameworkObjectManagerNewPrincipalEstablishedMessage (previousPrimaryIpAddress, previousPrimaryPort, previousPrimaryLocationId);

    tracePrintf(TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::becomePrincipal : Location Id of failed Principal: %d" , previousPrimaryLocationId);
        
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (previousPrimaryLocationId, "locationId"));

    vector<WaveManagedObject *> *pResultsLocalNodes = querySynchronously (&synchronousQueryContext);

    if (NULL == pResultsLocalNodes)
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::becomePrincipal: results pointer from querySynchronously is NULL");
        prismAssert (false, __FILE__, __LINE__);
    }

    if(pResultsLocalNodes->size () != 1) 
    {
       trace(TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::becomePrincipal: More than one object found");
       prismAssert (false, __FILE__, __LINE__);
    }

    VcsNodeLocalManagedObject* pVcsNodeLocalManagedObjectOfFailedPrincipal = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsLocalNodes)[0]);

    prismAssert (pVcsNodeLocalManagedObjectOfFailedPrincipal != NULL, __FILE__, __LINE__); 

    if (pVcsNodeLocalManagedObjectOfFailedPrincipal->getSwitchDeleteAfterFailover () || (CCM_ONLY == DcmToolKit::getDistributionMode ())) 
    {
        trace(TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::becomePrincipal : Setting trigger for controlled failover");

        pFrameworkObjectManagerNewPrincipalEstablishedMessage->setTriggerControlledFaileover();
    }

    WaveMessageStatus status = send (pFrameworkObjectManagerNewPrincipalEstablishedMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::becomePrincipalCallBack), NULL);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::becomePrincipal: One way message not successful");
        prismAssert (false, __FILE__, __LINE__);
    }
    
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResultsLocalNodes);
}

void VcsNodeFCPrincipalTrackingWorker::becomePrincipalCallBack (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    FrameworkObjectManagerNewPrincipalEstablishedMessage *pMessage = dynamic_cast<FrameworkObjectManagerNewPrincipalEstablishedMessage *> (pPrismMessage);

    prismAssert (NULL != pMessage, __FILE__, __LINE__);

    prismAssert (FRAMEWORK_SUCCESS == frameworkStatus, __FILE__, __LINE__);

    UI32 completionStatus = pMessage->getCompletionStatus ();

    if (WAVE_MESSAGE_ERROR_CLUSTER_OPERATION_IN_PROGRESS == completionStatus)
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::becomePrincipalCallBack: Become principal failed because of cluster formation in progress.");
        return;
    }

    prismAssert (WAVE_MESSAGE_SUCCESS == completionStatus, __FILE__, __LINE__);

    //set this node as vcs principal

    VcsFabricLocalObjectManager* pVcsFabricLocalObjectManager =  VcsFabricLocalObjectManager::getInstance ();
    startTransaction ();
    
    pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectIsPrincipal   (true);
    pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectStatus        (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD, VCS_FABRIC_LOCAL_SPECIFIC_STATUS_PRIMARY);

    pVcsFabricLocalObjectManager->updateThisVcsNodeLocalManagedObject ();

    ResourceId commitStatus = commitTransaction ();

    if (FRAMEWORK_SUCCESS != commitStatus)
    {    
        prismAssert (false, __FILE__, __LINE__);
    }    

    // Have the new principal bind to the vcs virtual ip.

    if (WAVE_MESSAGE_SUCCESS == pMessage->getCompletionStatus ())
    {
        ResourceId status = bindVirtualIp ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::becomePrincipalCallBack : Failed to bind the vcs virtual ip. Status : ") + FrameworkToolKit::localize (status));
        }
    }

    if (NULL != pMessage)
    {
        delete pMessage;
    }

    if (VCS_FABRIC_LOCAL_CLUSTER_SWITCH_OFFLINE_BUT_ADD_REJOIN_IN_PROGRESS == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {
        // need to trigger switch offline
        if ((false == getDomainValid ()) && (0 == getFCPrincipalDomainId ())) 
        {
            triggerSwitchOffline ();
        }
        else
        {
            VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);
            startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
        }    

        return;
    }

    if ((VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()) ||
        (VCS_FABRIC_LOCAL_CLUSTER_ADD_REJOIN_IN_PROGRESS == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()))
    {
        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

        // When secondary is becoming principal, (change_triggered => start_timer) if any heartbeat failure occurs in between
        // we need to trigger failover for all such nodes before we start timer.
        if (m_failedNodesLocationList.size () > 0)
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::becomePrincipalCallBack : Initiating failover for heartbeat failure nodes.");

            set<LocationId>::iterator failedLocationIter = m_failedNodesLocationList.begin ();
            while (failedLocationIter != m_failedNodesLocationList.end ())
            {
                LocationId locationId = *failedLocationIter;
                performFailoverForFailedLocation (locationId);
                failedLocationIter++;
            }

            // clear the set of failed locations
            m_failedNodesLocationList.erase (m_failedNodesLocationList.begin (), m_failedNodesLocationList.end ());
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, "VcsNodeFCPrincipalTrackingWorker::becomePrincipalCallBack : Start timer.");
            startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();    
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "VcsNodeFCPrincipalTrackingWorker::becomePrincipalCallBack : Unknown state.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

void VcsNodeFCPrincipalTrackingWorker::performFailoverForFailedLocation (LocationId &failedLocationId, PrismAsynchronousContext *pPrismAsynchronousContext)
{
    VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager           = VcsFabricLocalObjectManager::getInstance ();
    VcsNodeLocalManagedObject   *pVcsNodeLocalManagedObjectOfFailedNode = pVcsFabricLocalObjectManager->getLocalManagedObjectFromLocationId (failedLocationId);

    string failedNodeIpAddress = FrameworkToolKit::getIpAddressForLocationId (failedLocationId);
    UI32   failedNodePort      = FrameworkToolKit::getPortForLocationId      (failedLocationId);

    if (CCM_ONLY == DcmToolKit::getDistributionMode ()) 
    {
        if (NULL != pVcsNodeLocalManagedObjectOfFailedNode)
        {
            delete pVcsNodeLocalManagedObjectOfFailedNode;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::performFailoverForFailedLocation: Perform failover to remove node from KnownLocations.");
        }

        // start controlled failover by sending DeleteNodesFromCluster.
        FrameworkObjectManagerDeleteNodesFromClusterMessage *pMessage = new FrameworkObjectManagerDeleteNodesFromClusterMessage ();

        pMessage->addNodeIpAddressAndPort (failedNodeIpAddress, failedNodePort);
        pMessage->setIsDisconnected (true);
        
        WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;

        if (NULL != pPrismAsynchronousContext)
        {
            status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::heartbeatFailureDeleteNodeCallback), pPrismAsynchronousContext);
        }
        else
        {
            status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::heartbeatFailureDeleteNodeCallback), NULL);
        }

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::performFailoverForFailedLocation: Failed to send delete node message to the Cluster in CCMd"));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        // In Management cluster mode
        if (NULL != pVcsNodeLocalManagedObjectOfFailedNode)
        {
            bool switchDeleteAfterFailoverFlag = false;
            switchDeleteAfterFailoverFlag = pVcsNodeLocalManagedObjectOfFailedNode->getSwitchDeleteAfterFailover ();

            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::performFailoverForFailedLocation: location = ") + failedLocationId + string(" switchDeleteAfterFailover = ") + switchDeleteAfterFailoverFlag);

            if (switchDeleteAfterFailoverFlag)
            {
                FrameworkObjectManagerDeleteNodesFromClusterMessage *pMessage = new FrameworkObjectManagerDeleteNodesFromClusterMessage ();

                pMessage->addNodeIpAddressAndPort (failedNodeIpAddress, failedNodePort);
                pMessage->setIsDisconnected (true);

                WaveMessageStatus status = sendOneWay (pMessage);

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::performFailoverForFailedLocation: Failed to send delete node message to the Cluster in DCMd"));
                    prismAssert (false, __FILE__, __LINE__);
                }
                else
                {   
                    trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::performFailoverForFailedLocation: Delete node sent successfully to the Cluster. ");
                }
            }
            else
            {
                FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pMessage = new FrameworkObjectManagerSecondaryNodeFailureNotificationMessage (failedNodeIpAddress, failedNodePort, pVcsNodeLocalManagedObjectOfFailedNode->getLocationId ());
                
                WaveMessageStatus status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::heartbeatFailureUnControlledFailoverCallback), NULL);

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::performFailoverForFailedLocation : One way message not successful");
                    prismAssert (false, __FILE__, __LINE__);
                }
            }

            delete pVcsNodeLocalManagedObjectOfFailedNode;
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::performFailoverForFailedLocation: No need for any failover as db is already cleaned up.");
        }
    }

    return;
}

void VcsNodeFCPrincipalTrackingWorker::startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected (const UI32 &timerTimeout)
{
    u_short fabDomainList[max_domain_list];

    UI32 timeoutInterval;

    if (0 == timerTimeout)
    {
        timeoutInterval = wait_domain_lists_sync;
    }
    else
    {
        timeoutInterval = timerTimeout;
    }

    if (false == getDomainValid ())
    {
        trace (TRACE_LEVEL_WARN, string ("VcsNodeFCPrincipalTrackingWorker::startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected: fab domain list is not currently valid"));
        return;
    }

    if (getMyDomainId () != getFCPrincipalDomainId ())
    {
        return;
    }

    size_t recv_size = (sizeof (u_short)) * 257;
    UI32   retCode   = fabGetCosDomainList (fabDomainList, recv_size);

    if (0 != retCode)
    {
        trace (TRACE_LEVEL_WARN, string ("VcsNodeFCPrincipalTrackingWorker::startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected: error getting fab domain list"));
        return;
    }

    if (doesFabricDomainAndConnectedDomainListMatch (fabDomainList))
    {
        disableTimer ();
   
        trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected: fabric domain list and cluster list match - Not starting the principal tracking timer"));
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, string("VcsNodeFCPrincipalTrackingWorker::startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected: fabric domain list and cluster list does not match - Starting principal tracking timer"));
        startPrincipalTrackingTimer (timeoutInterval);
    }

}

bool VcsNodeFCPrincipalTrackingWorker::doesFabricDomainAndConnectedDomainListMatch (u_short *fabDomainList)
{
    BitMap connectedDomainsInCluster;
    setBitMapSize (connectedDomainsInCluster);
    connectedDomainsInCluster.reset ();
    UI32 count = getConnectedDomainsInCluster (connectedDomainsInCluster); // derived from VcsNodeLocalManagedObjects
    u_short i;
    LocationRole locationRole = FrameworkToolKit::getThisLocationRole ();

    if (fabDomainList[0] != count)
    {
        trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::doesFabricDomainAndConnectedDomainListMatch: fabDomainList and domains in cluster don't match"));
        return false;
    }

    for (i = 1; i < fabDomainList[0]+1; i++)
    {
        if (! connectedDomainsInCluster[fabDomainList[i]])
        {
            trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::doesFabricDomainAndConnectedDomainListMatch: fabDomainList and domains in cluster don't match at fabDominiID = ") + fabDomainList[i]+ " restarting timer");
            return false;
        }
    }

    if ((locationRole == LOCATION_PRIMARY_UNCONFIRMED) || (locationRole == LOCATION_SECONDARY_UNCONFIRMED))
    {
        return false;
    }
    
    return (fabAndMyDomainListsMatch (fabDomainList));
}

void VcsNodeFCPrincipalTrackingWorker::trackFCPrincipal ()
{
    string  ipAddress;
    UI32    myDomainId          = getMyDomainId ();
    BitMap  domainsInCluster;
    bool    clusterExists;
    UI32    vcsClusterId        = 0;

    setBitMapSize (domainsInCluster);
    domainsInCluster.reset ();

    vcsClusterId = getVcsClusterId (clusterExists);

    if (0 == vcsClusterId)
    {
        // FIXME : Sagar : WE need to ahndle this case.  Otherwise, remove vcsClusterId.
    }

    VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_CLUSTER_OPERATION_IN_PROGRESS);

    if (false == validDomainId (myDomainId))
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipal: getMyDomainId () returned invalid myDomainId = ") + myDomainId);

        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

        startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();

        return;
    }

    if (0 == m_remoteSwitchInfoList.size ())
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipal: Switches list is empty"));

        VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

        startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();

        return;
    }

    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipal: # switches to add = ") + m_remoteSwitchInfoList.size () + " Principal = " + amIVcsPrincipal ());

    if (false == amIVcsPrincipal ())
    {
        if (true == clusterExists)
        {
            if (FrameworkToolKit::getThisLocationRole() == LOCATION_SECONDARY_UNCONFIRMED || FrameworkToolKit::getThisLocationRole() == LOCATION_PRIMARY_UNCONFIRMED)
            {      
                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipal: This node is going to become the Principal after a reboot. myDomainID = ") + myDomainId);

                addSwitchesNotInCluster ();
                bindVirtualIp ();
                return;
            }
            else
            {   
                trace (TRACE_LEVEL_FATAL, string ("This is node should not reach here, current role is ") + FrameworkToolKit::localize (FrameworkToolKit::getThisLocationRole ()));
                prismAssert (false, __FILE__, __LINE__);
            }
        }
        else
        {      
            if (1 >= m_remoteSwitchInfoList.size ()) // only one switch in domain list so no cluster to form
            {
                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipal: There is only one switch in cluster so no Principal will be selected"));

                VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

                startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
                return;      
            }
            else if (LOCATION_STAND_ALONE != FrameworkToolKit::getThisLocationRole ()) 
            {
                //How this state is possible: Location is secondary but cluster doesn't exists.
                //Ans:  Cluster MO is on primary node when phase 2 reply is received by VcsFabObjectManager
                //      This is possible that VcsFabObjectManager has not processed the phase 2 reply but secondary node has completed phase 3
                //      Unconfirm Location Role is possible if primary node or secondary node is rebooted before Cluster MO is commited but Phase 3 is completed
                //      Location Role Primary can be possible on dual MM live sync enable.

                prepareNodeForClusterCreation ();

                return;
            }
            else
            {
                // create cluster -this will make this switch principal 
                createClusterAndAddSwitches (); 
                return;
            }
        }
    }
    else
    {
        // do I'm principal case
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipal: isPrincipal property is set for this switch"));

        if (1 >= m_remoteSwitchInfoList.size ()) // only one switch in domain list so no cluster to form
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipal: There is only one switch in cluster so no Principal will be selected"));

            VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

            startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
            return;      
        }

        vector<UI32> domainForTrivialMerge;

        if (isThereAnotherVcsPrincipalInThisCluster (domainForTrivialMerge))
        {   
            // there is another vcs principal in the cluster, this should never happen since we are tracking the FC principal which is guaranteed to be unique.
            // if there was a principal failover and this switch was the vcs principal another switch will be the new FC principal and we shouldn't
            // get here.
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipal: There is another VCS Principal in the cluster, this shouldn't happen"));

            // There is a possibility when a node take long time in becoming principal, another node can become primary by that time. Before other node declare HB failure and declare HB failure for this node or vice versa, enq message can cause duplicate primary. Sending the other primary message of cluster merge.

            VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

            handleClusterMergeRejoinAndAddNode (domainForTrivialMerge);
            return;
        }


        if (isThereANodeForTrivialMerge (domainForTrivialMerge))
        {
            // there is a switch in a cluster merge state: has another principal and its state in this node's VcsNodeLocalManagedObject is secondary disconnected
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipal: Node found for trivial merge, cluster merge will be performed"));

            handleClusterMergeRejoinAndAddNode (domainForTrivialMerge);
            return;
        }

        // not a cluster merge case so do add and rejoin
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipal: This switch is still Principal myDomainID = ") + myDomainId + " node add will be performed");

        addSwitchesNotInCluster ();
        return;
    }
}

bool VcsNodeFCPrincipalTrackingWorker::myDomainListMatchesRemoteSwitches ()
{
    if (m_dcaDomainListSize != m_remoteSwitchInfoList.size () || (0 >= m_dcaDomainListSize))
    {
        if (1 >= m_remoteSwitchInfoList.size ())    // local switch is the only switch in the remote switch list
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::myDomainListMatchesRemoteSwitches: No information was retrieved for any remote switch"));
            return false;
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::myDomainListMatchesRemoteSwitches: information for all remote switches has not been retrieved "));
        }
    }

    UI32 i;
    string myList = "";
    string remoteList = "";

    for (i = 0; i < m_remoteSwitchInfoList.size (); i++)
    {
        if ((m_remoteSwitchInfoList [i].getDcaBitMap ()) != m_knownDomainsList.dcaDomainsBitMap)
        {
            myList      = m_knownDomainsList.dcaDomainsBitMap.toString ();
            remoteList  = m_remoteSwitchInfoList[i].getDcaBitMap ().toString ();

            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::myDomainListMatchesRemoteSwitches: myList and remoteList mismatch for domainId = ") + m_remoteSwitchInfoList[i].getSwitchDomainId ()+ " myList = " + myList +" remoteList = "+remoteList);
            return false;
        }
    }

    return true;
}


void VcsNodeFCPrincipalTrackingWorker::addMessageCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    trace (TRACE_LEVEL_INFO, "AddMessageCallbackReceived");

    ResourceId              completionStatus;
    VcsAddSwitchMessage    *pAddSwitchMessage               = dynamic_cast<VcsAddSwitchMessage *> (pPrismMessage);

    prismAssert (NULL != pAddSwitchMessage, __FILE__, __LINE__);
    prismAssert (FRAMEWORK_SUCCESS == frameworkStatus, __FILE__, __LINE__);  // This is Local send to VcsFabObjectManager, this should never fail for framework status.

    completionStatus = pAddSwitchMessage->getCompletionStatus ();

    if (LOCATION_STAND_ALONE == FrameworkToolKit::getThisLocationRole ())
    {
        VcsFabricLocalObjectManager* pVcsFabricLocalObjectManager =  VcsFabricLocalObjectManager::getInstance ();

        startTransaction ();

        pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectIsPrincipal   (false);
        pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectInVcs         (false);
        pVcsFabricLocalObjectManager->setThisVcsNodeLocalManagedObjectStatus        (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD, VCS_FABRIC_LOCAL_SPECIFIC_STATUS_STAND_ALONE);

        pVcsFabricLocalObjectManager->updateThisVcsNodeLocalManagedObject ();

        ResourceId status = commitTransaction ();

        if (FRAMEWORK_SUCCESS != status)
        {
            prismAssert (false, __FILE__, __LINE__);
        }

        bindVirtualIp ();
    }

    if (WAVE_MESSAGE_SUCCESS == completionStatus)
    {
        vector<UI32> failedDomainIds    = pAddSwitchMessage->getFailedDomainIdVector ();
        UI32         numFailedDomainIds = failedDomainIds.size ();

        if (0 != numFailedDomainIds)
        {
            for (UI32 node = 0; node < numFailedDomainIds; node++)
            {
                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::addMessageCallback Not able to Add domain Id ") + failedDomainIds [node]);
            }
        }

        ResourceId currentStateForVcsFabricLocal = VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ();

        if ((VCS_FABRIC_LOCAL_CLUSTER_ADD_REJOIN_IN_PROGRESS == currentStateForVcsFabricLocal) || (VCS_FABRIC_LOCAL_CLUSTER_CREATION_IN_PROGRESS == currentStateForVcsFabricLocal))
        {
            VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

            if (false == (VcsFabricLocalObjectManager::getInstance ())->triggerPrincipalSwitchoverBasedOnPrincipalPriority ())
            {
                startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
            
                bindVirtualIp ();
            }
        }
        else 
        {    
            prismAssert (VCS_FABRIC_LOCAL_CLUSTER_SWITCH_OFFLINE_BUT_ADD_REJOIN_IN_PROGRESS == currentStateForVcsFabricLocal, __FILE__, __LINE__);
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::addMessageCallback : Switch offline will be triggered on Phase 3 complete Event");
            triggerSwitchOffline ();
        }
    }
    else if (VCS_CLUSTER_OPERATION_ALL_LOCATION_FAILED == completionStatus)
    {
        ResourceId currentStateForVcsFabricLocal = VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ();

        if ((VCS_FABRIC_LOCAL_CLUSTER_ADD_REJOIN_IN_PROGRESS == currentStateForVcsFabricLocal) || (VCS_FABRIC_LOCAL_CLUSTER_CREATION_IN_PROGRESS == currentStateForVcsFabricLocal))
        {
            VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);
            startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
        }
        else
        {
            prismAssert (VCS_FABRIC_LOCAL_CLUSTER_SWITCH_OFFLINE_BUT_ADD_REJOIN_IN_PROGRESS == currentStateForVcsFabricLocal, __FILE__, __LINE__);

            if ((false == getDomainValid ()) && (0 == getFCPrincipalDomainId ()))
            { 
                triggerSwitchOffline ();
            }
            else
            {
                VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);
                startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
            }
        }
    }

    delete pAddSwitchMessage;

#if 0            
                vector <UI32> reasonsToSegment;
                for (UI32 i = 0; i < numFailedDomainIds; i++)
                {
                    reasonsToSegment.push_back (R_PEER_VCS_CLUSTER_ADD_FAILURE);
                }

                // Segment the failed nodes. If segment fails for any reason,
                // tracking timer will be restarted.
                // segmentSwitchFromCluster (failedDomainIds, reasonsToSegment);
#endif                

}

void VcsNodeFCPrincipalTrackingWorker::segmentSwitchFromCluster(const vector<UI32> &domainIdsToSegment, const vector<UI32> &reasonsToSegment)
{
    trace (TRACE_LEVEL_DEVEL, "VcsNodeFCPrincipalTrackingWorker::segmentSwitchFromCluster : Entering...");

    UI32  numDomainIdToSegment  = domainIdsToSegment.size();
    trace (TRACE_LEVEL_DEVEL, string ("VcsNodeFCPrincipalTrackingWorker::segmentSwitchFromCluster: numDomainIdToSegment = ") + numDomainIdToSegment);
    for(UI32 i = 0; i < numDomainIdToSegment; ++i)
    {
        tracePrintf(TRACE_LEVEL_DEVEL, "VcsNodeFCPrincipalTrackingWorker::segmentSwitchFromCluster : domainIdsToSegment[%u] = %u", i, domainIdsToSegment[i]);
    }

    if (numDomainIdToSegment > 0)
    {
        // Send VcsSegmentSwitchMessage to global OM.
        VcsSegmentSwitchMessage *pVcsSegmentSwitchMessage = new VcsSegmentSwitchMessage (domainIdsToSegment, reasonsToSegment);

        ResourceId status = send(pVcsSegmentSwitchMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::segmentSwitchFromClusterCallBack), NULL);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_DEVEL, "VcsNodeFCPrincipalTrackingWorker::segmentSwitchFromCluster : Successfully sent segment switch message");
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::segmentSwitchFromCluster : Could not send segment switch message Status : " + FrameworkToolKit::localize (status));
            delete pVcsSegmentSwitchMessage;
            startPrincipalTrackingTimer(wait_domain_lists_sync);
        }
    }
}

void  VcsNodeFCPrincipalTrackingWorker::segmentSwitchFromClusterCallBack (FrameworkStatus frameworkStatus, VcsSegmentSwitchMessage *pVcsSegmentSwitchMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsNodeFCPrincipalTrackingWorker::segmentSwitchFromClusterCallBack : Starting");
    ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pVcsSegmentSwitchMessage != NULL, __FILE__, __LINE__);
        completionStatus = pVcsSegmentSwitchMessage->getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsNodeFCPrincipalTrackingWorker::segmentSwitchFromClusterCallBack : Successfully segmented switch from cluster");
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::segmentSwitchFromClusterCallBack : Failed to segment switch from cluster Status : " + FrameworkToolKit::localize (completionStatus));
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::segmentSwitchFromClusterCallBack : Could not send segment switch message Framework status : " + FrameworkToolKit::localize (frameworkStatus));
        completionStatus = frameworkStatus;
    }

    if (NULL != pVcsSegmentSwitchMessage)
    {
        delete pVcsSegmentSwitchMessage;
    }

    // If segment fails, restart tracking timer.
    if (WAVE_MESSAGE_SUCCESS != completionStatus)
    {
        startPrincipalTrackingTimer(wait_domain_lists_sync);
    }
}

bool VcsNodeFCPrincipalTrackingWorker::startPrincipalTrackingTimer (UI32 timeoutValue)
{
    // Principal selection is disabled
    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == VcsFabricLocalObjectManager::getThisVcsNodeClusterType () || (WAVE_NODE_START_AS_CLIENT == FrameworkToolKit::getStartMode ()))
    {
        trace (TRACE_LEVEL_WARN, "VcsNodeFCPrincipalTrackingWorker::startPrincipalTrackingTimer: Principal selection and automatic cluster formation is disabled.");
        return false;
    }

    if (! VcsNodeFCPrincipalTrackingWorker::vcsModeEnabled ())
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::startPrincipalTrackingTimer: VCS mode not set for this switch PrincipalTrackingTimer not started");
        return false;
    }

    if (!disableTimer ())        // disable timer if it's currently running
    {
        return false;
    }

    if (getMyDomainId() != getFCPrincipalDomainId())
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::startPrincipalSelectionTimer: timer should be started when the node is FC principal. Aserting for now to catch all condition where timer is started incorrectly");
        trace (TRACE_LEVEL_INFO, string ("Location Role is ") + FrameworkToolKit::localize (FrameworkToolKit::getThisLocationRole ()) + string (" VcsFabricLocalObjectManager state is ") + FrameworkToolKit::localize (VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()));

        prismAssert (false , __FILE__, __LINE__);
    }

    if ((VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER != VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()) && (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGED != VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()))
    {
        trace (TRACE_LEVEL_INFO, string ("Can not start Timer in this state, current state is ") + FrameworkToolKit::localize (VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()));
        return (false);
    }

    if (true == m_isNeedToStartTimerForWarmRecovery)
    {
        timeoutValue = 1;
    }

    ResourceId status = startTimer (VcsNodeFCPrincipalTrackingWorker::m_timerHandle, timeoutValue, reinterpret_cast<PrismTimerExpirationHandler> (&VcsNodeFCPrincipalTrackingWorker::trackPrincipalTimerWithAsyncEnq), NULL);

    if (FRAMEWORK_SUCCESS == status)
    {
        VcsNodeFCPrincipalTrackingWorker::m_timerRunningFlag = true;
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::startPrincipalTrackingTimer: PrincipalTrackingTimer starting");
        return true;
    }
    else
    {
        VcsNodeFCPrincipalTrackingWorker::m_timerRunningFlag = false;
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::startPrincipalTrackingTimer: PrincipalTrackingTimer could not be started error = "+FrameworkToolKit::localize (status));
        return false;
    }
}

bool VcsNodeFCPrincipalTrackingWorker::disableTimer ()
{
    if (VcsNodeFCPrincipalTrackingWorker::m_timerRunningFlag)
    {
        if (FRAMEWORK_SUCCESS != deleteTimer (VcsNodeFCPrincipalTrackingWorker::m_timerHandle))
        {
            trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::disableTimer: deleteTimer FAILED.");
            return false;
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "VcsNodeFCPrincipalTrackingWorker::disableTimer deleteTimer SUCCEEDED.");
            VcsNodeFCPrincipalTrackingWorker::m_timerRunningFlag = false;
            return true;
        }
    }
    else
    {
        return true;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Async Remote Enquire message processing functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UI32 VcsNodeFCPrincipalTrackingWorker::segmentMismatchedNodes (u_short *fabDomainList)
{
    return VCS_SUCCESS;
}

void VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps ()
{
    static UI32 numberOfTimeOuts = 1;

    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps : Timeout # ") + numberOfTimeOuts);
    numberOfTimeOuts++;

    if (false == getDomainValid ())
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps : fab domain list is not currently valid, timer will be started upon receiving domainValid event "));
        return;
    }

    if (getMyDomainId() != getFCPrincipalDomainId())
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps : This node is not FC principal anymore, not continuing");
        return;
    }

    if (VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
    {
        trace (TRACE_LEVEL_FATAL, "VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps : Node is not readyForSCNHandling  still timer started and expires.");

        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    // After time expires, VcsFabricLocalObjectManager can have these possible states:
    // VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER : General Case : Proceed Further
    // VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGED    : When Primary Changed but HeartBeat Failure has still not received. Take action based on Location Role as Location Role can be unconfirm also.
    // Only in these possible cases we should proceed further.
   
    LocationRole thisLocationRole          = FrameworkToolKit::getThisLocationRole ();
    bool         isUnconfirmedLocationRole = ((thisLocationRole == LOCATION_SECONDARY_UNCONFIRMED)  || (thisLocationRole == LOCATION_PRIMARY_UNCONFIRMED));

    if (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGED == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState () || LOCATION_SECONDARY == thisLocationRole)
    {
        if ((false == isUnconfirmedLocationRole) && LOCATION_STAND_ALONE != FrameworkToolKit::getThisLocationRole ())
        {
            // FC Principal has changed and this node has not received heartbeat failure. At this point this node has to be in LOCATION_SECONDARY role. 

            prismAssert (LOCATION_SECONDARY == thisLocationRole, __FILE__, __LINE__);

            // 1. Stop Heartbeat to Primary
            // 2. Disconnect From Primary
            // 3. Become Principal

            LocationId primaryNodeLocationId = FrameworkToolKit::getClusterPrimaryLocationId ();
            string     primaryNodeIpAddress  = FrameworkToolKit::getClusterPrimaryIpAddress ();
            UI32       primaryNodePort       = FrameworkToolKit::getClusterPrimaryPort ();

            // 1. Stop Heartbeat to Primary
            StopHeartBeatMessage *pStopHeartBeatMessage = new StopHeartBeatMessage();

            pStopHeartBeatMessage->setDstIpAddress  (primaryNodeIpAddress);
            pStopHeartBeatMessage->setDstPortNumber (primaryNodePort);
   
            WaveMessageStatus status = sendSynchronously (pStopHeartBeatMessage);

            if(WAVE_MESSAGE_SUCCESS != status) 
            {
                trace (TRACE_LEVEL_FATAL, "VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps Not able to send Message to stop heartbeat. Asserting");
                prismAssert (false, __FILE__, __LINE__);
            }
            else 
            {
                trace(TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps: StopHeartBeatMessage: status: ") + FrameworkToolKit::localize (pStopHeartBeatMessage->getCompletionStatus ()));
            }

            delete pStopHeartBeatMessage;

            // 2. Disconnect From Primary
            DisconnectFromNodeMessage *pDisconnectFromNodeMessage = new DisconnectFromNodeMessage (primaryNodeLocationId);

            pDisconnectFromNodeMessage->setServerIpAddress (primaryNodeIpAddress);
            pDisconnectFromNodeMessage->setServerPort      (primaryNodePort);

            status = sendSynchronously (pDisconnectFromNodeMessage);

            if(WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps Not able to send Message to disconnect from Node. Asserting");
                prismAssert (false, __FILE__, __LINE__);
            }   
            else 
            {
                trace(TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps: DisconnectFromNodeMessage : status: " + FrameworkToolKit::localize (pDisconnectFromNodeMessage->getCompletionStatus ()));
            } 

            delete pDisconnectFromNodeMessage;
            
            // 3. Become Principal
            VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED);
            becomePrincipal ();
            
            return;
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Unconfirm Role indicate this node is either comes back after reboot or rollback, These cases will be handled as part of cluster operation");
        }
    }

    u_short     fabDomainList [max_domain_list];
    size_t      recv_size   = (sizeof (u_short)) * 257;
    UI32        retCode     = fabGetCosDomainList (fabDomainList, recv_size);

    if (doesFabricDomainAndConnectedDomainListMatch (fabDomainList))   // This function take care of UNCONFIRM Location Role.
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps: fabric domain list and connected domain list match, not continuing"));
        return;
    }

    if (0 != retCode)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps: fabGetDomainList error code = ") + retCode);

        getInstance ()->startPrincipalTrackingTimer (wait_domain_lists_sync);
        return;
    }

    clearSwitchInfoRetrievedBitMap ();
    clearSwitchInfoLists ();

    UI32 numDomains          = fabDomainList[0];
    UI32 firstDomainIdInList = fabDomainList[1];


    tracePrintf (TRACE_LEVEL_INFO,"VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps: Number of Domains: %d",numDomains);

    if (((true == getDomainValid ()) && fabAndMyDomainListsMatch (fabDomainList)) || (numDomains == 1 && isUnconfirmedLocationRole && firstDomainIdInList == getMyDomainId ()))
    {
        m_isNeedToStartTimerForWarmRecovery = false;

        getRemoteSwitchInfoForDomainList (fabDomainList);
    }
    else
    {
        // Do a config replay of global and local configs for a single-node FC to MC transition case where a cluster is not created.
        // FC Principal check and domain valid check maybe redundant here (these are tested at the very start of this function and will return if either of these conditions are not met).

        if ((getMyDomainId () == getFCPrincipalDomainId ()) && (true == getDomainValid ()) && (LOCATION_STAND_ALONE == thisLocationRole) && (1 == numDomains))
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps: Single node FC to MC transition special handling.  Will do a config replay of any preserved configs."));

            // Replay both Global and Local configs for a single-node FC to MC transtion case where the configs have been preserved.
            bindVirtualIp ();

            //send Message Asynchronously to PrismframeworkObjectManager to replay config file
            PrismFrameworkReplayConfigMessage *pPrismFrameworkReplayConfigMessage = new PrismFrameworkReplayConfigMessage ();

            ResourceId sendStatus = send (pPrismFrameworkReplayConfigMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::replayConfigFileCallback), (void*) NULL);
            if (WAVE_MESSAGE_SUCCESS != sendStatus)
            {
                trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps : send failed for replaying config File "+FrameworkToolKit::localize (sendStatus));
                prismAssert (false, __FILE__, __LINE__);
            }
        }
        else if ((m_dcaDomainListSize != numDomains) && (1 != numDomains))
        {
            // It is possible at the time of domain valid that a reachable domain will not be in the fabric domain list.
            // This can happen if there are problems in the fabric layer due to delay in processing frames.
            // Node was still reachable and had a DCA. After a while the node appears in fabric domain list. No SCN is generated when this happens.
            // We need to read the fabric domain list based on a timer which would eventually get all of the correct info.

            trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps: fabDomainList and myDomainLists don't match, size is different: ") + "fabDomainListSize = "+numDomains+" myDomainListSize = "+m_dcaDomainListSize );
            startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::trackFCPrincipalSteps: my dca list and fab domain lists don't match yet, waiting to receive all DCA"));
        }
    }
}

void VcsNodeFCPrincipalTrackingWorker::replayConfigFileCallback (FrameworkStatus frameworkStatus, PrismMessage *pMessage, void *pContext)
{
    trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::replayConfigFileCallback : entering...");

    prismAssert (NULL != pMessage, __FILE__, __LINE__);
    prismAssert (FRAMEWORK_SUCCESS == frameworkStatus, __FILE__, __LINE__);
    prismAssert (WAVE_MESSAGE_SUCCESS == pMessage->getCompletionStatus (), __FILE__, __LINE__);
   
    delete pMessage;
}

void VcsNodeFCPrincipalTrackingWorker::trackPrincipalTimerWithAsyncEnq (TimerHandle timerHandle, void * pContext)
{
    // Pop out of timer, set to FALSE 
    VcsNodeFCPrincipalTrackingWorker::m_timerRunningFlag = false;

    if (!((VcsFabricLocalObjectManager::getInstance ())->getNoVcsEnableInProgressFlag ())) 
    {
        trackFCPrincipalSteps ();
    }
}

void VcsNodeFCPrincipalTrackingWorker::continueTrackPrincipalAfterRemoteEnqComplete (VcsNodeRemoteMessagingContext *pVcsNodeRemoteMessagingContext)
{
    u_short     *fabDomainList             = pVcsNodeRemoteMessagingContext->getPDomainList ();
    UI32         numDomains                = fabDomainList [0];
    LocationRole thisLocationRole          = FrameworkToolKit::getThisLocationRole();
    bool         isUnconfirmedLocationRole = ((thisLocationRole==LOCATION_SECONDARY_UNCONFIRMED)  || (thisLocationRole == LOCATION_PRIMARY_UNCONFIRMED));
    UI32         firstDomainIdInList       = fabDomainList [1];

    if ((VcsFabricLocalObjectManager::getInstance ())->getNoVcsEnableInProgressFlag ())
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::continueTrackPrincipalAfterRemoteEnqComplete: Terminating principal selection sequence as noVcsEnable is in progress");
        pVcsNodeRemoteMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);    
        return;
    }
   
    if (myDomainListMatchesRemoteSwitches () || ((numDomains == 1) && isUnconfirmedLocationRole && (firstDomainIdInList == getMyDomainId ())))
    {
        if (getMyDomainId() == getFCPrincipalDomainId())
        {
            pVcsNodeRemoteMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::continueTrackPrincipalAfterRemoteEnqComplete: enq message sent to all nodes in cluster, node is not FC principal so do nothing"));
            pVcsNodeRemoteMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);    
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::continueTrackPrincipalAfterRemoteEnqComplete: my dca list and remote switches dca list dont' match yet"));
        pVcsNodeRemoteMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);    
    }
}

void VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoAsyncCallBack  (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    VcsEnqSwitchMessage            *pVcsRemoteEnqSwitchMessage      = dynamic_cast<VcsEnqSwitchMessage *>(pPrismMessage);
    VcsNodeRemoteMessagingContext  *pVcsNodeRemoteMessagingContext  = reinterpret_cast<VcsNodeRemoteMessagingContext *> (pContext);

    prismAssert (NULL != pVcsNodeRemoteMessagingContext, __FILE__, __LINE__);

    --(*pVcsNodeRemoteMessagingContext);
    
    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pVcsRemoteEnqSwitchMessage, __FILE__, __LINE__);

        UI32 remoteNodeDomainId = pVcsRemoteEnqSwitchMessage->getRemoteNodeDomainId ();
    
        pVcsNodeRemoteMessagingContext->disconnectFromServer (remoteNodeDomainId);

		UI32 wait = 0;

        if (WAVE_MESSAGE_SUCCESS == pVcsRemoteEnqSwitchMessage->getCompletionStatus ())
        {
			ResourceId currentDistributionMode = DcmToolKit::getDistributionMode();

			if (DCM_CCM == currentDistributionMode)
			{
				UI32 Status = pVcsRemoteEnqSwitchMessage->getAutoUpgradeStatus(); 
				NodeAutoUpgradeConfiguration      UpgradeConfigData;

				FirmwareToolKit::getNodeAutoUpgradeConfigData(&UpgradeConfigData);

				if (UpgradeConfigData.getStatus() == true)
				{
					if (Status == NA_STS_NOT_REQUIRED)
					{
						trace(TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoAsyncCallBack : Auto-upgrade not required as versions are identical."));
					}
					if (Status == NA_STS_DOWNLOADED)
					{
						trace(TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoAsyncCallBack : Firmware download completed on the new node."));
						raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_FRCLOG, SULB_1203, remoteNodeDomainId);
					}
					else if (Status == NA_STS_IN_PROGRESS)
					{
						trace(TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoAsyncCallBack : Firmware upgrade in progress on the new node. New node will not be added now."));
						raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_FRCLOG, SULB_1201, remoteNodeDomainId);
						wait = 1;
					}
					else if (Status == NA_STS_COMPLETED)
					{
						trace(TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoAsyncCallBack : Auto-upgrade has completed."));
					}
					else if (Status == NA_STS_FAILURE)
					{
						trace(TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoAsyncCallBack : Auto-upgrade has failed on the new node."));
						raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_FRCLOG, SULB_1202, remoteNodeDomainId);
					}
					else if (Status == NA_STS_DISABLED)
					{
						trace(TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoAsyncCallBack : Node auto-upgrade feature not enabled."));
					}
					else if (Status == NA_STS_PRIN_VER_DFIFF)
					{
						trace(TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoAsyncCallBack : Node auto-upgrade not done as the firmware version configured is not same the principle node firmware version"));
					}
					else if (Status == NA_STS_SANITY_FAILED)
					{
						trace(TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoAsyncCallBack : Firmware sanity check failed, auto-upgrade not done."));
					}
					else if (Status == NA_STS_START_SUCCESS)
					{
						trace(TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoAsyncCallBack : Auto-upgrade has started on the new joining node."));
						raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_FRCLOG, SULB_1200, remoteNodeDomainId);
						wait = 1;
					}

				}
			}

			if (!wait) {
            		// set the remote switch info in the list.
            		VcsFabricLocalSwitchInfoObject remoteSwitchInfo = pVcsRemoteEnqSwitchMessage->getRemoteVcsSwitchInfoObject ();        
            		updateRemoteSwitchInfoList (remoteSwitchInfo);
			}
        }
    }
    else
    {
        if (NULL != pVcsRemoteEnqSwitchMessage)
        {
            UI32 remoteNodeDomainId = pVcsRemoteEnqSwitchMessage->getRemoteNodeDomainId ();

            pVcsNodeRemoteMessagingContext->disconnectFromServer (remoteNodeDomainId);
        }
    }

    if (NULL != pVcsRemoteEnqSwitchMessage)
    {
        delete (pVcsRemoteEnqSwitchMessage);
    }

    pVcsNodeRemoteMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsNodeFCPrincipalTrackingWorker::getSwitchInfoForDomainListAsyncMessageStep (VcsNodeRemoteMessagingContext *pVcsNodeRemoteMessagingContext)
{
    VcsFabricLocalSwitchInfoObject    localSwitchInfoObject;
    u_short                           myDomainId            = getMyDomainId ();
    u_short                          *domainIdList          = pVcsNodeRemoteMessagingContext->getPDomainList ();
    ResourceId                        status                = WAVE_MESSAGE_SUCCESS;
    string                            clientName            = getIpAddressFromDomainId (myDomainId) + ":" + getMgmtPort (myDomainId);
    UI32                              numberOfNodes         = domainIdList [0];
    UI32                              numberOfFailures      = 0;
    string                            serializedData;

    getLocalSwitchInfo (localSwitchInfoObject);

	ResourceId currentDistributionMode = DcmToolKit::getDistributionMode();

	if (DCM_CCM == currentDistributionMode)
	{
		NodeAutoUpgradeConfiguration      UpgradeConfigData;
		FirmwareToolKit::getNodeAutoUpgradeConfigData(&UpgradeConfigData);
		FirmwareToolKit::checkPrincipalVersionVersusConfiguredVersion(&UpgradeConfigData);

		UpgradeConfigData.serialize2 (serializedData);
	}

    ++(*pVcsNodeRemoteMessagingContext);

    for (UI32 i = 1; i < (numberOfNodes + 1); i++)   // the first element of the buffer contains the # domains returned
    {
        UI32    remoteSwitchDomainId             = domainIdList [i];
        string  remoteSwitchClusterMgmtIpaddress = getIpAddressFromDomainId (remoteSwitchDomainId);
        UI32    remoteSwitchClusterMgmtPort      = getMgmtPort              (remoteSwitchDomainId);
        
        if (remoteSwitchDomainId != myDomainId) 
        {
            if (false == m_knownDomainsList.switchInfoReceivedBitMap [remoteSwitchDomainId])
            {
                // Create ValclientSynchronous connnection
                // Create VcsEnqMessage and set local switch info
                // sendToWaveServer for the message with Callback, set ValClientSynchronous connection in the context.

                ValClientSynchronousConnection *pValClientSynchronousConnection = new ValClientSynchronousConnection (clientName, remoteSwitchClusterMgmtIpaddress, remoteSwitchClusterMgmtPort);

                VcsEnqSwitchMessage *pVcsEnqMessage = new VcsEnqSwitchMessage ();
                pVcsEnqMessage->setRemoteNodeDomainId  (remoteSwitchDomainId);
                pVcsEnqMessage->setRemoteVcsSwitchInfoObject (localSwitchInfoObject);
				//Using buffer instead of Serializable object till issue in
				//serialize2() for nested attributes is resolved.
				//pVcsEnqMessage->setNodeAutoUpgradeConfiguration(UpgradeConfigData);
				if (DCM_CCM == currentDistributionMode)
				{
					pVcsEnqMessage->addBuffer (AUTO_UPGRADE_OBJECT, serializedData.size (), (void *) (serializedData.c_str()), false);
				}

                // connect to server
                stringstream clientNameExt;
                clientNameExt << s_connectionSequenceNumber;

                string clientNameExtenstion = ":" + clientNameExt.str (); 
                s_connectionSequenceNumber ++; 

                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::getSwitchInfoForDomainListAsyncMessageStep : Getting information from remote switch ") + remoteSwitchDomainId);

                ResourceId connectionStatus = pValClientSynchronousConnection->connect (clientNameExtenstion);

                if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER != connectionStatus)
                {
                    numberOfFailures++;
    
                    delete pValClientSynchronousConnection;
                }
                else
                {
                    pVcsNodeRemoteMessagingContext->addToValClientSynchronousConnection (remoteSwitchDomainId, pValClientSynchronousConnection);

                    status = pValClientSynchronousConnection->sendToWaveServer (pVcsEnqMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoAsyncCallBack), this, pVcsNodeRemoteMessagingContext);

                    if (WAVE_MESSAGE_SUCCESS == status)
                    {
                        ++ (*pVcsNodeRemoteMessagingContext);
                    }
                    else
                    {
                        trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::getSwitchInfoForDomainListAsyncMessageStep : not able to send an Enq Message to Domain Id ") + remoteSwitchDomainId);

                        numberOfFailures++; 
                    }
                }
            }
        }
        else
        {
            updateRemoteSwitchInfoList (localSwitchInfoObject);
        }
    }

    --(*pVcsNodeRemoteMessagingContext);

    pVcsNodeRemoteMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);  // Call execute next step, if any enq message is sent out then this will not go to next step.
}

void VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoForDomainList (u_short *fabDomainList)
{

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeFCPrincipalTrackingWorker::getSwitchInfoForDomainListAsyncMessageStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeFCPrincipalTrackingWorker::continueTrackPrincipalAfterRemoteEnqComplete),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeFCPrincipalTrackingWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeFCPrincipalTrackingWorker::prismLinearSequencerFailedStep)
    };

    PrismAsynchronousContext *pPrismAsynchronousContext = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoForDomainListCallback));

    VcsNodeRemoteMessagingContext *pVcsNodeRemoteMessagingContext = new VcsNodeRemoteMessagingContext (pPrismAsynchronousContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    prismAssert (NULL != pVcsNodeRemoteMessagingContext, __FILE__, __LINE__);

    pVcsNodeRemoteMessagingContext->setPDomainList (fabDomainList);

    VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_ENQ_MESSAGE_IN_PROCESS);

    pVcsNodeRemoteMessagingContext->start ();
}

void VcsNodeFCPrincipalTrackingWorker::getRemoteSwitchInfoForDomainListCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    prismAssert (NULL != pPrismAsynchronousContext, __FILE__, __LINE__);

    ResourceId status = pPrismAsynchronousContext->getCompletionStatus ();
    
    delete pPrismAsynchronousContext;

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        if (getMyDomainId () == getFCPrincipalDomainId())
        {
            if (VCS_FABRIC_LOCAL_ENQ_MESSAGE_IN_PROCESS == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
            {
                // continue Cluster Related Operations.
                trackFCPrincipal ();
            }
            else
            {
                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::getSwitchInfoForDomainListCallback : Some other operation has in the middle of enquiry Message. Current State is ") + FrameworkToolKit::localize (VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ()));
            }
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::getSwitchInfoForDomainListCallback: In the middle of enquiry Message, FC principal changed. This is node not FC principal anymore");
        }
    }
    else
    {
        if (VCS_FABRIC_LOCAL_ENQ_MESSAGE_IN_PROCESS == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
        {
            VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);
            startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End Async Remote Enquire message functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void VcsNodeFCPrincipalTrackingWorker::updateRemoteSwitchInfoList (VcsFabricLocalSwitchInfoObject &switchInfo)
{
    SI32 i;
    SI32 numInList            = m_remoteSwitchInfoList.size ();
    UI32 remoteSwitchDomainId = switchInfo.getSwitchDomainId ();

    for (i = 0; i < numInList; i++)
    {
        if (remoteSwitchDomainId == m_remoteSwitchInfoList [i].getSwitchDomainId ())
        {
            m_remoteSwitchInfoList.erase (m_remoteSwitchInfoList.begin() + i);
            break;
        }
    }

    addRemoteSwitchInfo (switchInfo);
}

void VcsNodeFCPrincipalTrackingWorker::removeSwitchInfo (const UI32 &domainId)
{
    UI32 numInList            = m_remoteSwitchInfoList.size ();

    for (UI32 i = 0; i < numInList; i++)
    {
        if (domainId == m_remoteSwitchInfoList [i].getSwitchDomainId ())
        {
            m_remoteSwitchInfoList.erase (m_remoteSwitchInfoList.begin() + i);
            break;
        }
    }
}

void VcsNodeFCPrincipalTrackingWorker::saveRemoteSwitchInfoFromEnqMessage (VcsEnqSwitchMessage *pVcsEnqSwitchMessage)
{
    VcsFabricLocalSwitchInfoObject switchInfo;

    switchInfo = pVcsEnqSwitchMessage->getRemoteVcsSwitchInfoObject ();

    updateRemoteSwitchInfoList (switchInfo);
}

void VcsNodeFCPrincipalTrackingWorker::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
//    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_BROADCAST_PHASE3_COMPLETE_EVENT, reinterpret_cast<PrismEventHandler> (&VcsNodeFCPrincipalTrackingWorker::phase3CompleteEventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VcsNodeFCPrincipalTrackingWorker::disable (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEBUG, "VcsNodeFCPrincipalTrackingWorker::disable : Entering ...");

    if (m_timerRunningFlag)
    {
        if (FRAMEWORK_SUCCESS != deleteTimer (VcsNodeFCPrincipalTrackingWorker::m_timerHandle))
        {
                trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::disable: delete timer FAILED.");
                return;
        }
        else
        {
                trace (TRACE_LEVEL_SUCCESS, "VcsNodeFCPrincipalTrackingWorker::disable: delete timer SUCCEEDED.");
                m_timerRunningFlag = false;
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::disable: delete timer not running.");
    }
    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void VcsNodeFCPrincipalTrackingWorker::shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    
    trace (TRACE_LEVEL_DEBUG, "VcsNodeFCPrincipalTrackingWorker::shutdown : Entering ...");

    VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_NOT_READY_TO_ACCEPT_SCN);

    m_isNeedToStartTimerForWarmRecovery = false;

    if (m_timerRunningFlag)
    {
        if (FRAMEWORK_SUCCESS != deleteTimer (VcsNodeFCPrincipalTrackingWorker::m_timerHandle))
        {
            trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::shutdown: delete timer FAILED.");
            return;
        }
        else
        {
            trace (TRACE_LEVEL_SUCCESS, "VcsNodeFCPrincipalTrackingWorker::shutdown delete timer SUCCEEDED.");
            m_timerRunningFlag = false;
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::shutdown: delete timer not running.");
    }

    UI32                                        i = 0;
    ResourceId                                  timerStatus = FRAMEWORK_SUCCESS;
    UI32                                        domainId = 0;

    for (i = 0; i < m_vcsNodeTimerHandler.size (); i++)
    {
        domainId = m_vcsNodeTimerHandler[i]->getDomainId();
        
        timerStatus = deleteTimer (m_vcsNodeTimerHandler [i]->getTimerHandle());
        
        if (FRAMEWORK_TIMER_CAN_NOT_DELETE == timerStatus)
        {
            trace (TRACE_LEVEL_ERROR, string("VcsNodeFCPrincipalTrackingWorker::shutdown : Failed to delete domain unreachable timer for domainId = ") + domainId);
        }
        else
        {
            m_vcsNodeTimerHandler[i]->setTimerHandle(0);
        }

        if ((!getInstance ()->m_knownDomainsList.reachableDomainsBitMap[domainId]))
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::shutdown : domainId = ")+ domainId +" LocationID = " + m_vcsNodeTimerHandler[i]->getLocationId());

            DisconnectFromNodeMessage *pDisconnectFromNodeMessage = new DisconnectFromNodeMessage (m_vcsNodeTimerHandler[i]->getLocationId());

            pDisconnectFromNodeMessage->setServerIpAddress (m_vcsNodeTimerHandler[i]->getServerIpAddress ());
            pDisconnectFromNodeMessage->setServerPort (m_vcsNodeTimerHandler [i]->getServerPort ());

            WaveMessageStatus status = sendOneWay (pDisconnectFromNodeMessage);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
               trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::shutdown: One way message not successful");
            }
            else
            {
               trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeFCPrincipalTrackingWorker::shutdown: One way message successfully sent to clean up for domainId =") + domainId);
            }
        }
        
        delete (m_vcsNodeTimerHandler[i]);

        trace (TRACE_LEVEL_INFO, string("VcsNodeFCPrincipalTrackingWorker::shutdown : Found and deleted the domain unreachable timer for domainID = ") + domainId);
    }    
    
    m_vcsNodeTimerHandler.clear ();

    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void VcsNodeFCPrincipalTrackingWorker::heartbeatFailureDeleteNodeCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerDeleteNodesFromClusterMessage *pMessage, void *pContext)
{
    if (NULL != pContext)
    {
        if (FRAMEWORK_SUCCESS == frameworkStatus)
        {
            PrismAsynchronousContext *pPrismAsynchronousContext = reinterpret_cast<PrismAsynchronousContext *> (pContext);

            prismAssert (NULL != pPrismAsynchronousContext, __FILE__, __LINE__);
            prismAssert (NULL != pMessage, __FILE__, __LINE__);

            pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            pPrismAsynchronousContext->callback ();

            delete pMessage;

            trace (TRACE_LEVEL_INFO, "heartbeat failure completed, starting timer if any node discrepency found between fabric and vcs cluster");
            startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected (); 
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        if (FRAMEWORK_SUCCESS == frameworkStatus)
        {
            prismAssert (NULL != pMessage, __FILE__, __LINE__);
            delete pMessage;

            trace (TRACE_LEVEL_INFO, "heartbeat failure completed, starting timer if any node discrepency found between fabric and vcs cluster");
            startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected (); 
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void VcsNodeFCPrincipalTrackingWorker::heartbeatFailureUnControlledFailoverCallback (FrameworkStatus frameworkStatus, FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pMessage, void *pContext)
{
    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pMessage, __FILE__, __LINE__);

        delete pMessage;

        trace (TRACE_LEVEL_INFO, "heartbeat failure completed, starting timer if any node discrepency found between fabric and vcs cluster");
        startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

void  VcsNodeFCPrincipalTrackingWorker::heartBeatFailure (PrismAsynchronousContext     *pPrismAsynchronousContext)
{
    prismAssert (NULL != pPrismAsynchronousContext, __FILE__, __LINE__);
    
    PrismLinearSequencerContext               *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pPrismAsynchronousContext->getPCallerContext ());
    PrismHeartbeatFailureObjectManagerMessage  *pHeartbeatFailureMessage    = reinterpret_cast<PrismHeartbeatFailureObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    string                                      failedNodeIpAddress         = pHeartbeatFailureMessage->getIpAddress().toString();
    UI32                                        failedNodePort              = pHeartbeatFailureMessage->getPortNumber();
    UI32                                        failedNodeDomainId          = DcmToolKit::getDomainIdFromIpAddressAndPort (failedNodeIpAddress, failedNodePort);
    LocationId                                  failedNodeLocationId        = DcmToolKit::getLocationIdFromMappedId       (failedNodeDomainId);
    
    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::heartBeatFailure: handling HB failure for domainId = ") + failedNodeDomainId);
        
    if (getMyDomainId() != getFCPrincipalDomainId())
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::heartBeatFailure : This node is not FC principal anymore, not continuing");
        return;
    }

    removeFromx86DomainList(failedNodeDomainId); // for x86 only. If env is ppc it's a nop
    
    if (true == amIVcsPrincipal ())
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::heartbeatFailure: secondary node heartbeat failure");

        performFailoverForFailedLocation (failedNodeLocationId, pPrismAsynchronousContext);
        
        if (CCM_ONLY == DcmToolKit::getDistributionMode ())
        {
            // We should return since the callback for heartbeat failure (heartbeatFailureDeleteNodeCallback)
            // in FC mode would take care of cleaning up the context. If we continue, we might end up invoking
            // the callback on the context before the reply for DeleteNodesFromCluster has arrived
            return;
        }
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::heartbeatFailure: principal node heartbeat failure");
         
        // execute principal tracking logic if this switch is the new fc principal
        if (getMyDomainId () == getFCPrincipalDomainId())
        {
            disableTimer(); // disable principal tracking timer if its running

            if (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGED == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
            {
                VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED);
                becomePrincipal ();
            }
            else if (VCS_FABRIC_LOCAL_FC_PRINCIPAL_CHANGE_TRIGGERED == VcsFabricLocalObjectManager::getVcsFabricLocalObjectManagerState ())
            {
                trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::heartbeatFailure: Failover pending for domain ") + failedNodeDomainId);
                m_failedNodesLocationList.insert (failedNodeLocationId);
            }
            else
            {
                VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_HEARTBEAT_TO_FC_PRINCIPAL_FAILED);
            }
        }
    }

    if (NULL != pPrismAsynchronousContext)
    {
        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
    }
}
                                                                                                                                                   
bool VcsNodeFCPrincipalTrackingWorker::isClusterEnabled ()
{
    trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::isClusterEnabled: Entering ...... ");
    
    FILE * pFile;
    string clusterConfPath = DcmToolKit::getVcsConfPath ();
    clusterConfPath.append ("/cluster.conf");
    
    pFile = fopen (clusterConfPath.c_str(),"r"); 
    if (pFile == NULL)
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::isClusterEnabled: Cannot open the cluster.conf for reading");        
        return (false);
    }

    int clusterMode = fgetc (pFile);

    trace (TRACE_LEVEL_INFO, string ("VcsNodeFCPrincipalTrackingWorker::isClusterEnabled: VCS_MODE ") + clusterMode);

    fclose (pFile);

    if ('0' == clusterMode)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

int VcsNodeFCPrincipalTrackingWorker::fabGetCosDomainList(u_short *domainList, size_t recv_size)
{
    m_fabGetCosDomainListMutex.lock ();

    int  retCode = 0;
    int  status  = 0;
    int  is_cos  = 0;
    UI32 i;
    UI32 count   = 0;
    u_short fabDomainList[max_domain_list]  = {0};
    size_t  fab_list_size  = (sizeof (u_short)) * 257;

    retCode = fabGetDomainList(fabDomainList, fab_list_size);

    if (0 != retCode)
    {
        WaveNs::tracePrintf (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::fabGetCosDomainList : fabGetDomainList returned error. return value = %u", retCode);

        m_fabGetCosDomainListMutex.unlock ();

        return (retCode);
    }


    if (fabDomainList[0] == 1 )
    {
        domainList[0] = fabDomainList[0];
        domainList[1] = fabDomainList[1];
    }
    else
    {
        for (i = 1; i <= fabDomainList[0]; i++)
        {
            status =  essGetIsCOSDomain( fabDomainList[i], &is_cos, DCMLOG);

            if ((status != 0) && (fabDomainList[i] != getInstance ()->getMyDomainId ()))
            {
                WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::fabGetCosDomainList : essGetIsCOSDomain returned error. return value ") + status + string (" for domainId = ") +  fabDomainList[i]);
                continue;
            }
            else
            {
                if ((is_cos == true) || (fabDomainList[i] == getInstance ()->getMyDomainId ()))
                {
                    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("VcsNodeFCPrincipalTrackingWorker::fabGetCosDomainList :DomainId ") + fabDomainList [i] + string (" added to the list"));
                    domainList[count + 1] = fabDomainList[i];
                    count++;
                }
                else
                {
                    WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodeFCPrincipalTrackingWorker::fabGetCosDomainList : essGetIsCOSDomain returned error. is_cos is false, for domainId = ") +  fabDomainList[i]);
                }
            }
        }

        domainList[0] = count;
    }

    m_fabGetCosDomainListMutex.unlock ();

    return (retCode);
}

void VcsNodeFCPrincipalTrackingWorker::prepareNodeForClusterCreation ()
{
    string ipAddress            = FrameworkToolKit::getIpAddressForLocationId (FrameworkToolKit::getThisLocationId ());
    SI32 port                   = FrameworkToolKit::getPortForLocationId (FrameworkToolKit::getThisLocationId ());
    ResourceId messageStatus    = WAVE_MESSAGE_ERROR;

    FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage = new FrameworkObjectManagerUnconfigureClusterSecondaryMessage (ipAddress, port);

    messageStatus = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::prepareNodeForClusterCreationCallBack), NULL);

// clear delayed HB failure nodes stack since we are destroying the cluster
    
    if (WAVE_MESSAGE_SUCCESS != messageStatus)
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::prepareNodeForClusterCreation Unconfigure Self Node forward call Fails");
        prismAssert (false, __FILE__, __LINE__);
    }

}

void VcsNodeFCPrincipalTrackingWorker::prepareNodeForClusterCreationCallBack (FrameworkStatus frameworkStatus, FrameworkObjectManagerUnconfigureClusterSecondaryMessage *pMessage, void *pContext)
{
    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pMessage != NULL, __FILE__, __LINE__);

        ResourceId completionStatus = pMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::prepareNodeForClusterCreationCallBack :successfully unconfigured the secondary node");

            createClusterAndAddSwitches ();            
        }
        else if (FRAMEWORK_ERROR_CANNOT_UNCONFIGURE_LOCATION_WITH_IN_CLUSTER_PHASES_ROLE == completionStatus)
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeFCPrincipalTrackingWorker::prepareNodeForClusterCreationCallBack unconfigure failed, node is in the process of joining the cluster");

            VcsFabricLocalObjectManager::setVcsFabricLocalObjectManagerState (VCS_FABRIC_LOCAL_IS_READY_TO_START_TIMER);

            startTimerIfFabDomainListIncludesNodesNotInVcsClusterOrDisconnected ();
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }

        delete pMessage;
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

ResourceId VcsNodeFCPrincipalTrackingWorker::bindVirtualIp ()
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    // Bind virtualip. This is to address the case when after cluster merge, new 
    // principal is selected.
    
    // send message to global

    VirtualIpGlobalBindVipMessage *pMessage = new VirtualIpGlobalBindVipMessage ();
    prismAssert (NULL != pMessage, __FILE__, __LINE__);

    status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::bindVirtualIpCallback), (void *) NULL);
    
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        delete pMessage;
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::bindVirtualIp : Unable to send bind virtual ip message");
    }

    return status;
}

void VcsNodeFCPrincipalTrackingWorker::bindVirtualIpCallback (FrameworkStatus frameworkStatus, VirtualIpGlobalBindVipMessage *pMessage, void *pContext)
{
    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pMessage, __FILE__, __LINE__);
        ResourceId status = pMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_WARN, "VcsNodeFCPrincipalTrackingWorker::bindVirtualIp : Status : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::bindVirtualIp : Framework failure.");
    }
    
    delete pMessage;
}

ResourceId VcsNodeFCPrincipalTrackingWorker::unbindVirtualIp ()
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    // send message to global

    VirtualIpGlobalUnbindVipMessage *pMessage = new VirtualIpGlobalUnbindVipMessage ();
    prismAssert (NULL != pMessage, __FILE__, __LINE__);

    status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeFCPrincipalTrackingWorker::unbindVirtualIpCallback), (void *) NULL);
    
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        delete pMessage;
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::unbindVirtualIp : Unable to send unbind virtual ip message");
    }

    return status;
}

void VcsNodeFCPrincipalTrackingWorker::unbindVirtualIpCallback (FrameworkStatus frameworkStatus, VirtualIpGlobalUnbindVipMessage *pMessage, void *pContext)
{
    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pMessage, __FILE__, __LINE__);
        ResourceId status = pMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::unbindVirtualIpCallback : Failed : Status : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeFCPrincipalTrackingWorker::unbindVirtualIpCallback : Framework failure.");
    }

    delete pMessage;
}

}

