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

#include "Framework/Utils/FrameworkToolKit.h"
#include "VcsFabric/Local/VcsNodePrincipalSelectionWorker.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodePSMessagingContext.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosScnQueue.h"
#include "VcsFabric/Local/VcsRemoteEnqSwitchMessage.h"
#include "Cluster/PrismCluster.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Cluster/Local/HeartBeat/HeartBeatObjectManager.h"
#include "fabric/fabric.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "VcsFabric/Local/VcsFabricLocalSerializableObject.h"

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


UI32            getMgmtPort                     (UI32 domainId);
string          getIpAddressFromDomainId        (UI32 domainId);
void            getx86ipAddressAndPort          (const UI32 &x86DomainId, string &x86ipAddress, UI32 &x86port);
UI32            getDomainIdFromIpAddressAndPort (string ipAddress, UI32 port);
void            getKnownDomainList              (scnDomainsStruct &knownDomainList);
void            getLocalSwitchInfo              (VcsFabricLocalSwitchInfoObject &localSwitchInfo);
bool            vcsModeEnabled                  ();

vector<VcsFabricLocalSwitchInfoObject>   getRemoteSwitchInfoList          ();
vector<VcsFabricLocalSwitchInfoObject>   getInvalidParametersSwitchList   ();
vector<VcsFabricLocalSwitchInfoObject>   getSwitchInfoNotRetrievedList    ();
//bool isClusterFormationInProgress                                                             ();
//void setClusterFormationInProgress                                                            (const bool &clusterFormationInProgress);

TimerHandle             VcsNodePrincipalSelectionWorker::m_timerHandle;
bool            VcsNodePrincipalSelectionWorker::m_timerRunningFlag;
    
extern "C" 
{
// This is a wrapper function so it can be called from a 'c' module without having to include the .h file for VcsNodePrincipalSelectionTrackingWorker
vector<UI32> getx86DomainListFromPrincipalSelectionWorker ()
{
    vector<UI32> i;
    return i;
}
}

VcsNodePrincipalSelectionWorker::VcsNodePrincipalSelectionWorker (VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager)
    : WaveWorker (pVcsFabricLocalObjectManager)
{
        m_timerRunningFlag = false;
        m_clusterFormationFlag = false;
    m_principalFailureFlag = false;
    m_failedNodeIp = "";
    m_failedNodePort = 0;
    m_remoteSwitchInfoList.clear ();
    m_isNodeReadyForSCNHandling = false;
}

VcsNodePrincipalSelectionWorker::~VcsNodePrincipalSelectionWorker ()
{
}

VcsNodePrincipalSelectionWorker *VcsNodePrincipalSelectionWorker::getInstance ()
{
    static VcsNodePrincipalSelectionWorker *pVcsNodePrincipalSelectionWorker = (VcsFabricLocalObjectManager::getInstance ())->m_VcsPrincipalSelectionWorker;
    WaveNs::prismAssert (NULL != pVcsNodePrincipalSelectionWorker, __FILE__, __LINE__);
    return pVcsNodePrincipalSelectionWorker;
}

string VcsNodePrincipalSelectionWorker::getClassName ()
{
    return ("VcsNodePrincipalSelectionWorker");
}

#if 0
void VcsNodePrincipalSelectionWorker::initializeClusterGUIDs ()
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

void VcsNodePrincipalSelectionWorker::deleteObjects (vector<WaveManagedObject *> *pResults)
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

bool VcsNodePrincipalSelectionWorker::isEnvx86 ()
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

void VcsNodePrincipalSelectionWorker::getx86ipAddressAndPort    (const UI32 &x86DomainId, string &x86ipAddress, UI32 &x86port)
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
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::getx86ipAddressAndPort domainId not found: ")+ x86DomainId);
}

string VcsNodePrincipalSelectionWorker::getIpAddressFromDomainId(UI32 domainId)
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

vector<UI32> VcsNodePrincipalSelectionWorker::getx86DomainList ()
{
    return (getInstance ())->m_x86domainList;
}

vector<string> VcsNodePrincipalSelectionWorker::getx86ipAddresses ()
{
    return (getInstance ())->m_x86ipAddresses;
}

vector<UI32> VcsNodePrincipalSelectionWorker::getx86ports ()
{
    return (getInstance ())->m_x86ports;
}

void VcsNodePrincipalSelectionWorker::setx86DomainList (vector<UI32> &x86DomainList)
{
    (getInstance ())->m_x86domainList = x86DomainList;
}

void VcsNodePrincipalSelectionWorker::setx86ipAddresses (vector<string> &x86iPAddress)
{
    (getInstance ())->m_x86ipAddresses = x86iPAddress;
}

void VcsNodePrincipalSelectionWorker::setx86ports (vector<UI32> &x86port)
{
    (getInstance ())->m_x86ports = x86port;
}

void VcsNodePrincipalSelectionWorker::addTox86DomainList        (UI32 &x86DomainId)
{
    UI32 i;
    for (i = 0; i < (getInstance ())->m_x86domainList.size (); i++)
    {
        if (x86DomainId == getInstance ()->m_x86domainList[i])
        {
            ((getInstance ())->m_x86domainList).erase ((getInstance ())->m_x86domainList.begin ()+i);
            ((getInstance ())->m_x86ipAddresses).erase ((getInstance ())->m_x86ipAddresses.begin ()+i);
            ((getInstance ())->m_x86ports).erase ((getInstance ())->m_x86ports.begin ()+i);
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::addTox86DomainList: deleted previous domainId = ")+x86DomainId);
        }
    }
        getInstance ()->m_x86domainList.push_back (x86DomainId);
}

void VcsNodePrincipalSelectionWorker::removeFromx86DomainList   (UI32 &x86DomainId)
{
    UI32 i;
    for (i = 0; i < (getInstance ())->m_x86domainList.size (); i++)
    {
        if (x86DomainId == getInstance ()->m_x86domainList[i])
        {
            ((getInstance ())->m_x86domainList).erase ((getInstance ())->m_x86domainList.begin ()+i);
            ((getInstance ())->m_x86ipAddresses).erase ((getInstance ())->m_x86ipAddresses.begin ()+i);
            ((getInstance ())->m_x86ports).erase ((getInstance ())->m_x86ports.begin ()+i);
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::removeFromx86DomainList: deleted domainId = ")+x86DomainId);
        }
    }
}


void    VcsNodePrincipalSelectionWorker::addTox86IpAddressList (string &x86IpAddress)
{
        (getInstance ())->m_x86ipAddresses.push_back (x86IpAddress);
}

void    VcsNodePrincipalSelectionWorker::addTox86PortList (UI32 &x86Port)
{
        (getInstance ())->m_x86ports.push_back (x86Port);
}

UI32 VcsNodePrincipalSelectionWorker::getMgmtPort(UI32 domainId)
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
                return default_mgmt_port; 
        }
}

UI32 VcsNodePrincipalSelectionWorker::getPortFromDomainId(UI32 domainId)
{
    return FrameworkToolKit::getThisLocationPort();
}

UI32 VcsNodePrincipalSelectionWorker::getDomainIdFromIpAddressAndPort (string ipAddress, UI32 port)
{

    size_t i = 0;
    string domainValStr;
    UI32 domainId = 0;

    if (isEnvx86 ())
    {
        printf ("getDomainIdFromIpAddressAndPort: ipAddress = %s, port = %d\n\r",ipAddress.c_str (), port);
        for (i = 0; i < getInstance ()->m_x86domainList.size (); i++)
        {
            if (((getInstance ()->m_x86ipAddresses)[i] == ipAddress) && ((getInstance ()->m_x86ports)[i] == port))
            {
                return (getInstance ()->m_x86domainList)[i];
            }
        }
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::getDomainIdFromIpAddressAndPort: ipAddress not found can't get domain id "+ipAddress);
        return 0;
    }
    else
    {
        i = ipAddress.rfind ('.');
        if (i != string::npos)
        {
            domainValStr = ipAddress.substr (i+1,string::npos);
            domainId = (UI32) atoi (domainValStr.c_str ());
            return domainId;
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::getDomainIdFromIpAddressAndPort: invalid ipAddress, can't get domain id "+ipAddress);
            return 0;
        }
    }
}

void VcsNodePrincipalSelectionWorker::cancelPrincipalSelectionTimer ()
{
    if (VcsNodePrincipalSelectionWorker::m_timerRunningFlag)
    {
        if (FRAMEWORK_SUCCESS != deleteTimer (VcsNodePrincipalSelectionWorker::m_timerHandle))
        {
            trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::cancelPrincipalSelectionTimer: delete timer FAILED");
        }
        else
        {
            VcsNodePrincipalSelectionWorker::m_timerRunningFlag = false;
        }
    }
}

vector<WaveManagedObject *> *VcsNodePrincipalSelectionWorker::fabOMDoSynchronousQuery (const string className)
{
        vector<WaveManagedObject *> *pResults = querySynchronously (className);
        prismAssert (NULL != pResults, __FILE__, __LINE__);
        return pResults;
}

vector<VcsNodeLocalManagedObject *> *VcsNodePrincipalSelectionWorker::getNodeLocalManagedObjects ()
{
        vector<WaveManagedObject *> *pResults = fabOMDoSynchronousQuery (VcsNodeLocalManagedObject::getClassName());
        return (reinterpret_cast <vector<VcsNodeLocalManagedObject *> *> (pResults));
}

ObjectId VcsNodePrincipalSelectionWorker::getLocalSwitchObjectId ()
{
    return (VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeObjectId ();
}

UI32 VcsNodePrincipalSelectionWorker::getMyDomainId ()
{
        UI32 myDomainId = VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeLocalManagedObjectDomainId ();
        return myDomainId;
}

UI32 VcsNodePrincipalSelectionWorker::getMyMappedId ()
{
    return getMyDomainId ();
}
VcsNodeLocalManagedObject *VcsNodePrincipalSelectionWorker::getNodeLocalManagedObjectForThisNode ()
{
        ObjectId localSwitchObjectId = getLocalSwitchObjectId ();
        return getSwitchObjectFromObjectId (localSwitchObjectId);
}

VcsNodeLocalManagedObject * VcsNodePrincipalSelectionWorker::getSwitchObjectFromObjectId(const ObjectId &switchObjectId)
{
        vector<ObjectId> switchObjectIds;
        switchObjectIds.insert(switchObjectIds.begin(), 1, switchObjectId);
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;

        vector<WaveManagedObject *> *pResults = querySynchronously (VcsNodeLocalManagedObject::getClassName (), switchObjectIds);
        if (NULL == pResults)
        {
                trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::getSwitchObjectFromObjectId: pResults for LocalNode query by ObjectId returned NULL");
                prismAssert (false, __FILE__, __LINE__);
        }
        if (1 < pResults->size ())
        {
                delete pResults;
                trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::getSwitchObjectFromObjectId: multiple objects with same object ID found");
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

void VcsNodePrincipalSelectionWorker::addRemoteSwitchInfo (const VcsFabricLocalSwitchInfoObject &remoteSwitchInfo)
{
        getInstance ()->m_remoteSwitchInfoList.push_back (remoteSwitchInfo);
        setSwitchInfoRetrievedBit (remoteSwitchInfo.switchDomainId);
}

void VcsNodePrincipalSelectionWorker::addInvalidParametersSwitchList (const VcsFabricLocalSwitchInfoObject &invalidParmsSwitchInfo)
{
    getInstance ()->m_invalidParametersSwitchList.push_back (invalidParmsSwitchInfo);
    setSwitchInfoRetrievedBit (invalidParmsSwitchInfo.switchDomainId);
}

void  VcsNodePrincipalSelectionWorker::addSwitchInfoNotRetrievedList (const VcsFabricLocalSwitchInfoObject &infoNotRetrievedSwitchInfo)
{ 
    getInstance ()->m_switchInfoNotRetrievedList.push_back (infoNotRetrievedSwitchInfo);
}
 
void  VcsNodePrincipalSelectionWorker::clearSwitchInfoLists ()
{
    getInstance ()->m_remoteSwitchInfoList.clear ();
    getInstance ()->m_invalidParametersSwitchList.clear ();
    getInstance ()->m_switchInfoNotRetrievedList.clear ();
}

vector<VcsFabricLocalSwitchInfoObject>   VcsNodePrincipalSelectionWorker::getRemoteSwitchInfoList          ()
{
    return getInstance ()->m_remoteSwitchInfoList;
}

vector<VcsFabricLocalSwitchInfoObject>   VcsNodePrincipalSelectionWorker::getInvalidParametersSwitchList   ()
{
    return getInstance ()->m_invalidParametersSwitchList;
}

vector<VcsFabricLocalSwitchInfoObject>   VcsNodePrincipalSelectionWorker::getSwitchInfoNotRetrievedList    ()
{
    return getInstance ()->m_switchInfoNotRetrievedList;
}

bool VcsNodePrincipalSelectionWorker::transactionMoved ()
{
        return false;
}

#endif

UI32 VcsNodePrincipalSelectionWorker::getVcsClusterId (bool &clusterExists)
{
/*    VcsClusterManagedObject *pVcsClusterManagedObject = NULL;
    UI32 vcsClusterId = 0; 
    clusterExists = false;

    vector<WaveManagedObject *> *pResultsCluster = fabOMDoSynchronousQuery (VcsClusterManagedObject::getClassName ()); 
        prismAssert (1 >= pResultsCluster->size (), __FILE__, __LINE__);

    trace(TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::getVcsClusterId cluster size = ") + pResultsCluster->size ()); 

    if (1 == pResultsCluster->size())
    {    
        clusterExists = true;
        pVcsClusterManagedObject = dynamic_cast<VcsClusterManagedObject *>((*pResultsCluster)[0]);
        vcsClusterId = pVcsClusterManagedObject->getVcsClusterId ();
                delete pVcsClusterManagedObject;
                delete pResultsCluster;
    }    
    else 
    {    
//        vcsClusterId = getLocalNodeVcsId ();
                delete pResultsCluster;
    }    

    trace(TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::getVcsClusterId cluster ID = ") + vcsClusterId);
*/
        return 0;
}

#if 0
vector<string> VcsNodePrincipalSelectionWorker::getClusterGUIDs ()
{
        return m_vcsClusterGUIDs;
}

void VcsNodePrincipalSelectionWorker::setVcsClusterGUIDs (const vector<string> &vcsClusterGUIDs)
{
        m_vcsClusterGUIDs = vcsClusterGUIDs;
}

void VcsNodePrincipalSelectionWorker::getLocalSwitchInfo (VcsFabricLocalSwitchInfoObject &localSwitchInfo)
{
        localSwitchInfo.switchIpAddress = FrameworkToolKit::getThisLocationIpAddress();
        localSwitchInfo.switchPort = FrameworkToolKit::getThisLocationPort();
        localSwitchInfo.switchMgmtIpAddress = FrameworkToolKit::getThisLocationIpAddress();
        localSwitchInfo.switchObjectId = getInstance ()->getLocalSwitchObjectId ();
        VcsFabricLocalObjectManager* pVcsFabricLocalObjectManager =  VcsFabricLocalObjectManager::getInstance ();

        localSwitchInfo.switchDomainId = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectDomainId ();
        localSwitchInfo.switchMgmtPort =  getMgmtPort (localSwitchInfo.switchDomainId);
        localSwitchInfo.switchSerialNumber = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectSerialNumber ();
        localSwitchInfo.switchInCluster = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectInVcs ();
        localSwitchInfo.switchVcsMode =  pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectVcsMode();
        localSwitchInfo.switchVcsId = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectVcsId ();
        localSwitchInfo.switchIsPrincipal = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectIsPrincipal ();
        localSwitchInfo.switchClusterFormationFlag = getInstance ()->getClusterFormationFlag ();
        localSwitchInfo.switchWillingMode = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectWillingMode ();
        localSwitchInfo.switchPublicIpAddress = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectPublicIpAddress ();
        localSwitchInfo.switchSWVersion = my_sw_version;
        localSwitchInfo.dcaDomainsBitMap = getInstance ()->m_knownDomainsList.dcaDomainsBitMap;
        localSwitchInfo.nodeState = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectSpecificStatus ();
        localSwitchInfo.transactionMoved = getInstance ()->transactionMoved ();
        localSwitchInfo.vcsClusterGUIDs = getInstance ()->getClusterGUIDs ();
        localSwitchInfo.noVcsEnableInProgressFlag = (VcsFabricLocalObjectManager::getInstance ())->getNoVcsEnableInProgressFlag ();
    if (FrameworkToolKit::getThisLocationRole () == LOCATION_PRIMARY_UNCONFIRMED)
        {
        localSwitchInfo.switchIsUnconfirmPrincipal = true;
        }
    else
        {
        localSwitchInfo.switchIsUnconfirmPrincipal = false;
        }
    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::getLocalSwitchInfo: getting local switch info for domainId = ")+localSwitchInfo.switchDomainId
        + " Role = " + FrameworkToolKit::localize(FrameworkToolKit::getThisLocationRole ())
        + " UnconfirmPrincipal = " + localSwitchInfo.switchIsUnconfirmPrincipal);
        /*
        if (0 < pVcsNodeLocalManagedObject->getClusterGUIDs ().size ())
        {
                localSwitchInfo.vcsClusterGUIDs = pVcsNodeLocalManagedObject->getClusterGUIDs ();
        }
        else
        {
                localSwitchInfo.vcsClusterGUIDs.clear ();
        }
        */
}

void VcsNodePrincipalSelectionWorker::getKnownDomainList (scnDomainsStruct &knownDomainList)
{
    knownDomainList.reachableDomainsBitMap = getInstance ()->m_knownDomainsList.reachableDomainsBitMap;
    knownDomainList.dcaDomainsBitMap = getInstance ()->m_knownDomainsList.dcaDomainsBitMap;
    knownDomainList.switchInfoReceivedBitMap = getInstance ()->m_knownDomainsList.switchInfoReceivedBitMap;
    knownDomainList.remoteKnownDCABitMap = getInstance ()->m_knownDomainsList.remoteKnownDCABitMap;
}

UI32 VcsNodePrincipalSelectionWorker::getDomainsInCluster (BitMap &domainsIncluster)
{
    vector<WaveManagedObject *> *pVcsLocalNodes = fabOMDoSynchronousQuery (VcsNodeLocalManagedObject::getClassName ());
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;
    UI32 numNodes = pVcsLocalNodes->size ();
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::getDomainsInCluster num nodes in cluster = ")+ numNodes);
    UI32 i;
    UI32 count = 0;
    UI32 domainId = 0;
    for (i = 0; i < numNodes; i++)
    {
        pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pVcsLocalNodes)[i]);
        if (NULL != pVcsNodeLocalManagedObject)
        {
            domainId = pVcsNodeLocalManagedObject->getDomainId ();
            trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::getDomainsInCluster domainId = ")+ domainId);
            domainsIncluster.set (pVcsNodeLocalManagedObject->getDomainId ());
            count++;
        }
    }
    deleteObjects (pVcsLocalNodes); 
    return count;
}


VcsNodeLocalManagedObject * VcsNodePrincipalSelectionWorker::getSwitchObjectFromDomainId (UI32 domainId)
{
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (domainId, "domainId"));
    vector<WaveManagedObject *> *pResultsLocalNodes = querySynchronously (&synchronousQueryContext);
    if (NULL == pResultsLocalNodes)
    {
            trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::getSwitchObjectFromDomainId: results pointer from querySynchronously is NULL");
            prismAssert (false, __FILE__, __LINE__);
    }

    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;
    string ipAddress = "";
    UI32 i;
    if (1 < pResultsLocalNodes->size ())
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::getSwitchObjectFromDomainId: multiple objects with same domain ID found domainId = ")+domainId + " numObjects = "+pResultsLocalNodes->size ());
        for (i = 0; i < pResultsLocalNodes->size (); i++)
        {
            pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *>((*pResultsLocalNodes)[i]);
            domainId = pVcsNodeLocalManagedObject->getDomainId ();
            ipAddress = (pVcsNodeLocalManagedObject->getPublicIpAddress ())[0];
            trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::getSwitchObjectFromDomainId: object list: domainId = ")+domainId + " pubIpAddress = "+ipAddress);
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
        

void VcsNodePrincipalSelectionWorker::setBitMapSize (BitMap &bitMap)
{
        bitMap.resize (max_domain_list -1);
}

void VcsNodePrincipalSelectionWorker::setSizeForDomainBitMaps (scnDomainsStruct & domainBitMapStruct)
{
        setBitMapSize (domainBitMapStruct.dcaDomainsBitMap);
        setBitMapSize (domainBitMapStruct.reachableDomainsBitMap);
        setBitMapSize (domainBitMapStruct.remoteKnownDCABitMap);
        setBitMapSize (domainBitMapStruct.switchInfoReceivedBitMap);
}



bool VcsNodePrincipalSelectionWorker::validDomainId (UI32 domainId)
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

bool VcsNodePrincipalSelectionWorker::vcsModeEnabled ()
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

void VcsNodePrincipalSelectionWorker::initializeDomainList ()
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
        trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::initializeDomainList: getMyDomainId () returned invalid myDomainId, domain list initialization did not succeed myDomainId = ")+myDomainId);
        m_dcaDomainListSize = 0;
    }
}

void VcsNodePrincipalSelectionWorker::replaySCNQueueEvents ()
{
    trace (TRACE_LEVEL_DEBUG, "VcsNodePrincipalSelectionWorker::replaySCNQueueEvents: Entering...");
    
    m_isNodeReadyForSCNHandling = true;
    
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

        
    UI32 eventType;
    UI32 domainId;
    UI32 principalDomainId;
    ResourceId status = WAVE_MESSAGE_ERROR;
    UI32 eventId;

    trace (TRACE_LEVEL_INFO,string ("VcsNodePrincipalSelectionWorker::replaySCNQueueEvents: SCNQsize = ")+ VcsNodeFabosInterfaceObjectManager::getInstance ()->scnQueueSize ());
    
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

                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::replaySCNQueueEvents: Replaying Domain Valid event for domainId = ")+ domainId);
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
                break;

            case VCS_NODE_FABOS_DOMAIN_CAPABILITIES_AVAILABLE:
                            
                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::replaySCNQueueEvents: Replaying DCA event for domainId = ")+ domainId);
                pDCAEvent = new VcsNodeDomainCapabilitiesAvailableEvent ();
                pDCAEvent->setDomainId (domainId);
                pDCAEvent->setEventId (eventId);
                pDCAEventCopy = pDCAEvent;
                status = DomainCapabilitiesAvailableEventHandler(pDCAEventCopy);
                if (WAVE_MESSAGE_SUCCESS == status)
                {
                    broadcast (pDCAEvent);
                }
                break;

            case VCS_NODE_FABOS_DOMAIN_INVALID:
                            
                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::replaySCNQueueEvents: Replaying Domain Invalid event for domainId = ")+ domainId);
                pDIEvent = new VcsNodeDomainInvalidEvent ();
                pDIEvent->setDomainId (domainId);
                pDIEvent->setEventId (eventId);
                pDIEventCopy = pDIEvent;
                status = DomainInvalidEventHandler(pDIEventCopy);
                if (WAVE_MESSAGE_SUCCESS == status)
                {
                    broadcast (pDIEvent);
                }
                break;

            case VCS_NODE_FABOS_DOMAIN_REACHABLE:
                            
                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::replaySCNQueueEvents: Replaying Domain Reachable event for domainId = ")+ domainId);
                pDREvent = new VcsNodeDomainReachableEvent ();
                pDREvent->setDomainId (domainId);
                pDREventCopy = pDREvent;
                DomainReachableEventHandler(pDREventCopy);
                broadcast (pDREvent);
                break;

            case VCS_NODE_FABOS_DOMAIN_UNREACHABLE:
                            
                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::replaySCNQueueEvents: Replaying Domain UnReachablg event for domainId = ")+ domainId);
                pDUEvent = new VcsNodeDomainUnreachableEvent ();
                pDUEvent->setDomainId (domainId);
                pDUEvent->setEventId (eventId);
                pDUEventCopy = pDUEvent;
                status = DomainUnReachableEventHandler(pDUEventCopy);
                if (WAVE_MESSAGE_SUCCESS == status)
                {
                    broadcast (pDUEvent);
                }
                break;

            default:
                            
                trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::replaySCNQueueEvents: Unknown SCN type = ")+ eventType+ " for domainId = " + domainId);
                VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(eventId);
                break;
        }
    }
}

ResourceId VcsNodePrincipalSelectionWorker::DomainInvalidEventHandler (const VcsNodeDomainInvalidEvent *&pEvent)
{
    static UI32 numberOfEventsRceivedSoFar = 0;
    numberOfEventsRceivedSoFar++;
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainInvalidEventHandler Event count = ")+numberOfEventsRceivedSoFar);
    prismAssert (NULL != pEvent, __FILE__, __LINE__);
    UI32 invalidSwitchDomainId = pEvent->getDomainId ();
    UI32 eventId = pEvent->getEventId ();
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(pEvent->getEventId ());
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainInvalidEventHandler DomainInValid event not in event queue, eventId = ")+eventId);
        return (status);
    }
    
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainInvalidEventHandler DomainId for Invalid Domain = ")+invalidSwitchDomainId);
    setDomainValid (false);
    return (status);
}

void VcsNodePrincipalSelectionWorker::setDomainValid (const bool &domainValidOrInvalid)
{
    m_fabDomainValid = domainValidOrInvalid;
}

ResourceId VcsNodePrincipalSelectionWorker::DomainValidEventHandler (const VcsNodeDomainValidEvent *&pEvent)
{
    static UI32 numberOfEventsRceivedSoFar = 0;
    numberOfEventsRceivedSoFar++;
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainValidEventHandler: Event count = ")+numberOfEventsRceivedSoFar);
    prismAssert (NULL != pEvent, __FILE__, __LINE__);
    UI32 domainValidDomain = pEvent->getDomainId ();
    UI32 eventId = pEvent->getEventId ();
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(pEvent->getEventId ());
    if (WAVE_MESSAGE_SUCCESS !=  status)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainValidEventHandler: event not in queue, eventId  = ")+ eventId);
        return (status);
    
    }
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainValidEventHandler: DomainValid DomainId  = ")+ domainValidDomain);
    setDomainValid (true);
    return (status);
}

void VcsNodePrincipalSelectionWorker::setReachableDomainBitForDomainId (const u_short &reachableDomain)
{
    // check if this domain is already in list
    if (! getInstance ()->m_knownDomainsList.reachableDomainsBitMap[reachableDomain]) 
    {
        getInstance ()->m_knownDomainsList.reachableDomainsBitMap.set (reachableDomain);
        getInstance ()->m_knownDomainsList.dcaDomainsBitMap.reset (reachableDomain);
    }
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::setReachableDomainBitForDomainId: myDomainListSize = ")+m_dcaDomainListSize);
}

ResourceId VcsNodePrincipalSelectionWorker::DomainReachableEventHandler (const VcsNodeDomainReachableEvent *&pEvent)
{
    static UI32 numberOfEventsRceivedSoFar = 0;
    numberOfEventsRceivedSoFar++;
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainReachableEventHandler Event count = ")+numberOfEventsRceivedSoFar);
    prismAssert (NULL != pEvent, __FILE__, __LINE__);
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    UI32 eventId = pEvent->getEventId ();
    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(pEvent->getEventId ());
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainReachableEventHandler: event not in queue, eventId  = ")+ eventId);
        return (status);
    }

    if (1 == numberOfEventsRceivedSoFar)
    {
        initializeDomainList ();        // initialize the domain bit maps the first time DomainReachable is received
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainReachableEventHandler domain list initialized"));
    }
    
    UI32 reachableDomain = pEvent->getDomainId ();
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainReachableEventHandler DomainId for reachable domain = ")+ reachableDomain);
    setReachableDomainBitForDomainId (reachableDomain);
    cleanupTimerHandlerForDomainId (reachableDomain, true);
    return (status);
}

void VcsNodePrincipalSelectionWorker::clearReachableDomainBitForDomainId (const u_short &unReachableDomain)
{
    // check if this domain was in the list before removing it
    if (getInstance ()->m_knownDomainsList.reachableDomainsBitMap[unReachableDomain]) 
    {
       getInstance ()->m_knownDomainsList.reachableDomainsBitMap.reset (unReachableDomain);
       if (getInstance ()->m_knownDomainsList.dcaDomainsBitMap[unReachableDomain])
       {
          if (1 > m_dcaDomainListSize)
          {
             WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::clearReachableDomainBitForDomainId: Error: dcaDomainList size = 0 but dcaBitMap for unreachable domain is set")+ "unreachable domainId = "+unReachableDomain);
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
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::clearReachableDomainBitForDomainId: myDomainListSize = ")+m_dcaDomainListSize+" unreachable domainId = "+unReachableDomain);
}
                                                                                                                                                                                        
ResourceId VcsNodePrincipalSelectionWorker::DomainUnReachableEventHandler     (const VcsNodeDomainUnreachableEvent *&pEvent)
{
    static UI32 numberOfEventsRceivedSoFar = 0;
    VcsNodeTimerHandler *pVcsNodeTimerHandler = NULL;
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;
    TimerHandle timerHandle;
    
    numberOfEventsRceivedSoFar++;
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainUnReachableEventHandler Event count = ")+numberOfEventsRceivedSoFar);
    prismAssert (NULL != pEvent, __FILE__, __LINE__);
    UI32 unreachableDomain = pEvent->getDomainId ();
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainUnReachableEventHandler DomainId for Unreachable domain = ")+unreachableDomain);
    UI32 eventId = pEvent->getEventId ();
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(pEvent->getEventId ());
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainUnReachableEventHandler: event not in queue, eventId  = ")+ eventId);
        return (status);
    }

    clearReachableDomainBitForDomainId (unreachableDomain);
    pVcsNodeLocalManagedObject = getSwitchObjectFromDomainId(unreachableDomain);
    if (NULL == pVcsNodeLocalManagedObject)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainUnReachableEventHandler: switch not in my cluster domain Id = ")+ unreachableDomain);
        return (status);
    }

    /* First let lookup for exist timer - If found, delete it */
    cleanupTimerHandlerForDomainId (unreachableDomain, true);
    /* Start timer for disconnect from location */
    pVcsNodeTimerHandler = new VcsNodeTimerHandler (unreachableDomain, pVcsNodeLocalManagedObject->getLocationId());
    
    status = startTimer (timerHandle, wait_domain_lists_sync, reinterpret_cast<PrismTimerExpirationHandler> (&VcsNodePrincipalSelectionWorker::processCleanupTimerForDomainId), (void *)pVcsNodeTimerHandler);
    if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::DomainUnReachableEventHandler: Timer starting for DomainId = ") + unreachableDomain);
        pVcsNodeTimerHandler->setTimerHandle(timerHandle),
        m_vcsNodeTimerHandler.push_back(pVcsNodeTimerHandler);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("VcsNodePrincipalSelectionWorker::DomainUnReachableEventHandler: Could not be start timer for DomainId = ") + unreachableDomain + " with status = " + FrameworkToolKit::localize (status));
        delete (pVcsNodeTimerHandler);
    }
    return (status);
}

void  VcsNodePrincipalSelectionWorker::cleanupTimerHandlerForDomainId(UI32 domainId, bool delTimer)
{
    UI32 i;
    vector<VcsNodeTimerHandler *>::iterator  theIterator;
    ResourceId  timerStatus;
    trace (TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::cleanupTimerHandlerForDomainId : Enter Domain ID: ") + domainId);
    for (i = 0; i < m_vcsNodeTimerHandler.size (); i++)
    {
        if (m_vcsNodeTimerHandler[i]->getDomainId() == domainId)
        {
            /* Found let delete the timer and free the structure */
            if (delTimer) 
            {
                timerStatus = deleteTimer (m_vcsNodeTimerHandler [i]->getTimerHandle());
                if (FRAMEWORK_TIMER_CAN_NOT_DELETE == timerStatus) 
                {
                    trace (TRACE_LEVEL_ERROR, string("VcsNodePrincipalSelectionWorker::cleanupTimerHandlerForDomainId : Failed to delete timer."));
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

            trace (TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::cleanupTimerHandlerForDomainId : Found and delete timer DomainID = ") + domainId);
            return;
        }
    }
}

void VcsNodePrincipalSelectionWorker::processCleanupTimerForDomainId (TimerHandle timerHandle, void * pContext)
{
    VcsNodeTimerHandler *pVcsNodeTimerHandler = (VcsNodeTimerHandler *) pContext;
    
    if (!getInstance ()->m_knownDomainsList.reachableDomainsBitMap[pVcsNodeTimerHandler->getDomainId()]) 
    {
        /* Not Found Domain Reachable - Let send message to Framework */
        VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance ();

        /* Send Message to Framework */
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::processCleanupTimerForDomainId : LocationID ") + pVcsNodeTimerHandler->getLocationId());
        DisconnectFromNodeMessage *pDisconnectFromNodeMessage = new DisconnectFromNodeMessage (pVcsNodeTimerHandler->getLocationId());

        WaveMessageStatus status = pVcsFabricLocalObjectManager->sendOneWay (pDisconnectFromNodeMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
           trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::processCleanupTimerForDomainId: One way message not successful");
        }
    }
    /* Let Free the timer handler context */
    cleanupTimerHandlerForDomainId(pVcsNodeTimerHandler->getDomainId(), false);
}

void VcsNodePrincipalSelectionWorker::setDCADomainBitForDomainId (const u_short &dcaDomain)
{
        string dcaBitMapstr;
        if (getInstance ()->m_knownDomainsList.reachableDomainsBitMap[dcaDomain])
        {
                if (! getInstance ()->m_knownDomainsList.dcaDomainsBitMap[dcaDomain])
                {       
                        // set dca bit for this domain if not already set
                        getInstance ()->m_knownDomainsList.dcaDomainsBitMap.set(dcaDomain);
                        dcaBitMapstr = getInstance ()->m_knownDomainsList.dcaDomainsBitMap.toString ();
                        m_dcaDomainListSize++;
                }
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::setDCADomainBitForDomainId dcaDomainsBitMap = ")+ dcaBitMapstr);
        }
        else
        {
                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::setDCADomainBitForDomainId dcaDomainsBitMap not set because domain is not in reachable domain list, dcaDomain = ")+ dcaDomain);
        }
}

ResourceId VcsNodePrincipalSelectionWorker::DomainCapabilitiesAvailableEventHandler (const VcsNodeDomainCapabilitiesAvailableEvent *&pEvent)
{
    static UI32 numberOfEventsRceivedSoFar = 0;
    numberOfEventsRceivedSoFar++;
    u_short fabDomainList[max_domain_list];
    size_t recv_size = (sizeof (u_short)) * 257;
    UI32 myDomainId = getMyDomainId ();
    ResourceId  status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainCapabilitiesAvailableEventHandler: event count: ")+ numberOfEventsRceivedSoFar);
    prismAssert (NULL != pEvent, __FILE__, __LINE__);
    UI32 dcaDomain = pEvent->getDomainId ();
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainCapabilitiesAvailableEventHandler: DomainId for DCA domain = ")+ dcaDomain);
    UI32  eventId = pEvent->getEventId ();
    status = VcsNodeFabosInterfaceObjectManager::getInstance ()->popPScnQueueObject(pEvent->getEventId ());
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainCapabilitiesAvailableEventHandler: event not in queue, skipping handling this event, eventId = ")+ eventId);
        return (status);
    }

    UI32 retCode;

    setDCADomainBitForDomainId (dcaDomain);

    retCode = fabGetDomainList(fabDomainList, recv_size);
    if (0 != retCode)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainCapabilitiesAvailableEventHandler: error getting fab domain list, principal selection timer not started"));
                
        if (VcsNodePrincipalSelectionWorker::m_timerRunningFlag)
        {
            if (FRAMEWORK_SUCCESS != deleteTimer (VcsNodePrincipalSelectionWorker::m_timerHandle))
            {
                trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::DomainCapabilitiesAvailableEventHandler: delete timer failed.");
                return (status);
            }
            else
            {
                VcsNodePrincipalSelectionWorker::m_timerRunningFlag = false;
            }
        }
        // even though we got an error from fabGetDomainList we'll start the timer in the hope this will recover by the time the timer pops
        // if it does not then the timer will be reset again.
        // if we don't set the timer we can get into a situation where the DCA SCNs are received but the Principal Selection Process will never get triggered
        startPrincipalSelectionTimer (wait_domain_lists_sync);
        return (status);
    }

    if (m_fabDomainValid && fabAndMyDomainListsMatch (fabDomainList))
    {
        if (VcsNodePrincipalSelectionWorker::m_timerRunningFlag)
        {
            if (FRAMEWORK_SUCCESS != deleteTimer (VcsNodePrincipalSelectionWorker::m_timerHandle))
            {
                trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::DomainCapabilitiesAvailableEventHandler: delete timer failed.");
                return (status);
            }
            else
            {
                VcsNodePrincipalSelectionWorker::m_timerRunningFlag = false;
            }
        }
        startPrincipalSelectionTimer (wait_domain_lists_sync);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::DomainCapabilitiesAvailableEventHandler: my dca list and fab domain lists don't match yet, my domainId = ")+myDomainId);
    }
    return (status);
}

UI32 VcsNodePrincipalSelectionWorker::getLowestDomainId (UI32 &minIndex)
{
        UI32 minDomainId;
        UI32 i;
        if (0 == m_remoteSwitchInfoList.size ())
        {
                return 0;
        }
        else if (1 >= m_remoteSwitchInfoList.size ())
        {
                return m_remoteSwitchInfoList[0].switchDomainId;
        }
        minDomainId = m_remoteSwitchInfoList[0].switchDomainId;
    minIndex = 0;
        for (i = 1; i < m_remoteSwitchInfoList.size (); i++)
        {
                if (m_remoteSwitchInfoList[i].switchDomainId < minDomainId)
                {
                        minDomainId = m_remoteSwitchInfoList[i].switchDomainId;
            minIndex = i;
                }
        }
        return minDomainId;
}

bool VcsNodePrincipalSelectionWorker::vcsPrincipalExistsInDomainList ()
{
        UI32 i;
        UI32 myDomainId = getMyDomainId ();
    if (! validDomainId (myDomainId))
    {
        return false;
    }

        for (i = 0; i < m_remoteSwitchInfoList.size (); i++)
                {
            trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::vcsPrincipalExistsInDomainList: domainId = ")+m_remoteSwitchInfoList[i].switchDomainId
            + " Principal = " + m_remoteSwitchInfoList[i].switchIsPrincipal + " UnconfirmPrincipal = " + m_remoteSwitchInfoList[i].switchIsUnconfirmPrincipal);
            if (m_remoteSwitchInfoList[i].switchIsPrincipal && (m_remoteSwitchInfoList[i].switchDomainId != myDomainId)
                && (!m_remoteSwitchInfoList[i].switchIsUnconfirmPrincipal))
                        {
                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::vcsPrincipalExistsInDomainList: another principal found in domain list with domainId = ")+m_remoteSwitchInfoList[i].switchDomainId);
                                return true;    
                        }
                }
        return false;
}

bool VcsNodePrincipalSelectionWorker::clusterFormationInDomainList ()
{
        UI32 i;
        UI32 myDomainId = getMyDomainId ();
    if (! validDomainId (myDomainId))
    {
        return false;
    }

        for (i = 0; i < m_remoteSwitchInfoList.size (); i++)
                {
                if (m_remoteSwitchInfoList[i].switchClusterFormationFlag && (m_remoteSwitchInfoList[i].switchDomainId != myDomainId))
                        {
                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::clusterFormationInDomainList: another switch forming a cluster with domainId = ")+m_remoteSwitchInfoList[i].switchDomainId);
                                return true;    
                        }
                }
        return false;
}

bool VcsNodePrincipalSelectionWorker::isMyDomainIdLowest (UI32 &minIndex)
{
        UI32 myDomainId = getMyDomainId ();
        UI32 lowestDomainId;
        if (! validDomainId (myDomainId))
        {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::isMyDomainIdLowest: getMyDomainId () returned invalid domainId = ")+myDomainId);
                return false;
        }
        else
        {
                lowestDomainId = getLowestDomainId (minIndex);
                if (myDomainId == lowestDomainId)
                {
                        return true;
                }
                else
                {
                        return false;
                }
        }
}

bool VcsNodePrincipalSelectionWorker::principalFailOverInProgress ()
{
        return false;
}

void VcsNodePrincipalSelectionWorker::relenquishPrincipal ()
{
    trace(TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::relenquishPrincipal: Current Role  = ") +
        FrameworkToolKit::localize(FrameworkToolKit::getThisLocationRole ()));
    if (FrameworkToolKit::getThisLocationRole () == LOCATION_PRIMARY_UNCONFIRMED) {
        trace(TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::relenquishPrincipal: I am Unconfirm Primary Role"));
    } else {
        // Bao : Need code to do Principal selection 
        trace(TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::relenquishPrincipal: I am Principal Role : need to do Selection "));
    }
        return;
}

UI32 VcsNodePrincipalSelectionWorker::wereAllSwitchesAdded (BitMap &switchesNotInCluster)
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
        
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::wereAllSwitchesAdded: Num switches in cluster = ")+ numNodes+ " Num not in cluster = "+nNotInCluster);
        return nNotInCluster;
}

UI32 VcsNodePrincipalSelectionWorker::setPrincipalFlag (bool principalFlag)
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
                WaveNs::trace(TRACE_LEVEL_ERROR, string("VcsNodePrincipalSelectionWorker::setPrincipalFlag: error setting isPrincipal flag ")+ FrameworkToolKit::localize (status));
                return GENERIC_ERROR;
        }
   
        WaveNs::trace(TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::setPrincipalFlag: isPrincipal set for switch with DomainId = ")+ localSwitchDomainId);
    
        return VCS_SUCCESS;
}

void VcsNodePrincipalSelectionWorker::doControlledPrincipalChangeOver (UI32 fcPrincipalDomainId)
{
        // DCM  framework needs to support this case, till then not much we can do
        return;
}

bool VcsNodePrincipalSelectionWorker::isFabDomainValid ()
{
        return m_fabDomainValid;
}

bool VcsNodePrincipalSelectionWorker::isThereAnotherVcsPrincipalInCluster ()
{
        UI32 i;
        UI32 myDomainId = getMyDomainId ();
    if (! validDomainId (myDomainId))
    {
        return false;
    }
        for (i = 0; i < m_remoteSwitchInfoList.size (); i++)
        {
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::isThereAnotherVcsPrincipalInCluster: domainId = ")+m_remoteSwitchInfoList[i].switchDomainId
            + " Principal = " + m_remoteSwitchInfoList[i].switchIsPrincipal + " UnconfirmPrincipal = " + m_remoteSwitchInfoList[i].switchIsUnconfirmPrincipal);
        // is switch in my cluster that is principal and its not me
        if ((isSwitchDomainIdInMyCluster (m_remoteSwitchInfoList[i])) && m_remoteSwitchInfoList[i].switchIsPrincipal &&
            (!m_remoteSwitchInfoList[i].switchIsUnconfirmPrincipal) && (m_remoteSwitchInfoList[i].switchDomainId != myDomainId))
                {
            trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::isThereAnotherVcsPrincipalInCluster: another principal found in cluster domainId = ")+m_remoteSwitchInfoList[i].switchDomainId);
                        return true;    
                }
        }
        return false;
}

bool VcsNodePrincipalSelectionWorker::fabAndMyDomainListsMatch (u_short *fabDomainList)
{
        u_short i;
        string dcaBitMap = getInstance ()->m_knownDomainsList.dcaDomainsBitMap.toString ();
        string reachableBitMap = getInstance ()->m_knownDomainsList.reachableDomainsBitMap.toString ();
        u_short fabDomainCount = fabDomainList[0];
        if (0 == fabDomainCount) return false;
        if (getInstance ()->m_knownDomainsList.reachableDomainsBitMap != getInstance ()->m_knownDomainsList.dcaDomainsBitMap)
        {
                trace (TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::fabAndMyDomainListsMatch: dca not received for all reachable domains ") + "reachableDomainBitMap = "+reachableBitMap+" dcaDomainBitMap = "+dcaBitMap);
                return false;   
        }
        for (i = 0; i < fabDomainCount; i++)
        {
                if (!(getInstance ()->m_knownDomainsList.reachableDomainsBitMap[fabDomainList[i+1]] && getInstance ()->m_knownDomainsList.dcaDomainsBitMap[fabDomainList[i+1]]))
                {
                        trace (TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::fabAndMyDomainListsMatch: fabDomainList and myDomainLists don't match, mismatch at fabDominiID = ") + fabDomainList[i+1]);
            trace (TRACE_LEVEL_INFO, string("Domain Reachable bitmap ") + getInstance ()->m_knownDomainsList.reachableDomainsBitMap.toString () + " != DCA bitmap " +
                        getInstance ()->m_knownDomainsList.dcaDomainsBitMap.toString());
                        return false;
                }
        }
        if (getInstance ()->m_dcaDomainListSize != fabDomainCount)
        {
                trace (TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::fabAndMyDomainListsMatch: fabDomainList and myDomainLists don't match, size is different: ") + "fabDomainListSize = "+fabDomainCount+" myDomainListSize = "+m_dcaDomainListSize );
                return false;
        }
        else
        {
                return true;
        }
}

void VcsNodePrincipalSelectionWorker::clearRemoteSwitchInfoList ()
{
    UI32 i;
    UI32 domainId;
        if (! m_remoteSwitchInfoList.empty ())
    {
        for (i = 0; i < getInstance ()->m_remoteSwitchInfoList.size (); i++)
        {
            domainId = (getInstance ()->m_remoteSwitchInfoList)[i].switchDomainId;
            if (! isSwitchInfoRetrievedForDomainId (domainId))
            {
                // remove switches for which remote switch info has not been received
                (getInstance ()->m_remoteSwitchInfoList).erase ((getInstance ()->m_remoteSwitchInfoList).begin ()+i);
                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::clearRemoteSwitchInfoList: switchInfo removed for domainId = ")+domainId);
            }
        }
    }
}

void VcsNodePrincipalSelectionWorker::clearSwitchInfoRetrievedBitMap ()
{
        getInstance ()->m_knownDomainsList.switchInfoReceivedBitMap.reset ();
}

void VcsNodePrincipalSelectionWorker::setSwitchInfoRetrievedBit (const u_short &infoRetrievedDomain)
{
        getInstance ()->m_knownDomainsList.switchInfoReceivedBitMap.set (infoRetrievedDomain);
}

UI32 VcsNodePrincipalSelectionWorker::retrieveRemoteSwitchInfo (VcsFabricLocalSwitchInfoObject &remoteSwitchInfo)
{

    ResourceId      status           = WAVE_MESSAGE_ERROR;
        ResourceId              connectionStatus;
        u_short                 myDomainId = getMyDomainId ();
        u_short                 remoteDomainId = remoteSwitchInfo.switchDomainId;
        string                  myIpAddress = getIpAddressFromDomainId (myDomainId);
        UI32                    port = getPortFromDomainId (myDomainId);

        if (! validDomainId (myDomainId))
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::retreiveRemoteSwitchInfo: getMyDomainId () returned invalid myDomainId = ")+myDomainId);
        return GENERIC_ERROR;
    }

    string myName = this->getClassName () + string (":") + myIpAddress + string (":") + port;
    trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::retreiveRemoteSwitchInfo: myName = "+myName);

        // get remote switch information

        trace (TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::retreiveRemoteSwitchInfo : setting up connection to remote wave server ")+ "From: " + myName + " To: " + " ipAddress: "+remoteSwitchInfo.switchMgmtIpAddress+" port: "+remoteSwitchInfo.switchMgmtPort);
        ValClientSynchronousConnection valClientSynchronousConnection (myName, remoteSwitchInfo.switchMgmtIpAddress, remoteSwitchInfo.switchMgmtPort);
        connectionStatus = valClientSynchronousConnection.connect ();
        if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER != connectionStatus)
        {
                trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::retreiveRemoteSwitchInfo : Error establishing connection to remote switch domainId = ")+remoteSwitchInfo.switchDomainId);
                return GET_REMOTE_INFO_ERROR;
        }

    getLocalSwitchInfo (remoteSwitchInfo);

        status = valClientSynchronousConnection.getRemoteSwitchInfo (remoteSwitchInfo);
        valClientSynchronousConnection.close ();
        if (valClientSynchronousConnection.isCurrentlyConnected ())
        {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::retreiveRemoteSwitchInfo : Connection to server was not properly closed ipAddress = " )+remoteSwitchInfo.switchMgmtIpAddress);
        }

        if (WAVE_MESSAGE_SUCCESS != status)
    {
                trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::retreiveRemoteSwitchInfo : Error getting remote switch information for domainId = ")+remoteDomainId);
                return GET_REMOTE_INFO_ERROR;
    }
        else
        {
                trace (TRACE_LEVEL_SUCCESS, string("VcsNodePrincipalSelectionWorker::retreiveRemoteSwitchInfo : responderSerialNumber = ") + remoteSwitchInfo.switchSerialNumber + " respSWVer = " +remoteSwitchInfo.switchSWVersion + " IpAddr : " + remoteSwitchInfo.switchIpAddress + " Port: " + remoteSwitchInfo.switchPort);
                return VCS_SUCCESS;
        }
}

UI32 VcsNodePrincipalSelectionWorker::getLocalNodeVcsId ()
{
    const ObjectId  localSwitchObjectId = (VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeObjectId ();
    VcsNodeLocalManagedObject * pVcsNodeLocalManagedObject = getSwitchObjectFromObjectId(localSwitchObjectId);
    prismAssert (NULL != pVcsNodeLocalManagedObject, __FILE__, __LINE__);
    UI32 vcsId = pVcsNodeLocalManagedObject->getVcsId ();
    delete pVcsNodeLocalManagedObject;
    return vcsId;
}

UI32 VcsNodePrincipalSelectionWorker::getVcsClusterId (bool &clusterExists)
{
    VcsClusterManagedObject *pVcsClusterManagedObject = NULL;
    UI32 vcsClusterId = 0;
    clusterExists = false;

    vector<WaveManagedObject *> *pResultsCluster = fabOMDoSynchronousQuery (VcsClusterManagedObject::getClassName ());
        prismAssert (1 >= pResultsCluster->size (), __FILE__, __LINE__);

    trace(TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::getVcsClusterId cluster size = ") + pResultsCluster->size ());

    if (1 == pResultsCluster->size())
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

    trace(TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::getVcsClusterId cluster ID = ") + vcsClusterId);
        return vcsClusterId;
}

vector<string> VcsNodePrincipalSelectionWorker::getVcsClusterGUIDsFromClusterMO ()
{
    VcsClusterManagedObject *pVcsClusterManagedObject = NULL;
        vector<string> vcsClusterGUIDs;

    vector<WaveManagedObject *> *pResultsCluster = fabOMDoSynchronousQuery (VcsClusterManagedObject::getClassName ());
        prismAssert (1 >= pResultsCluster->size (), __FILE__, __LINE__);

    if (1 == (*pResultsCluster).size())
    {
        pVcsClusterManagedObject = dynamic_cast<VcsClusterManagedObject *>((*pResultsCluster)[0]);
                vcsClusterGUIDs = pVcsClusterManagedObject->getVcsClusterGUIDs ();
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

bool VcsNodePrincipalSelectionWorker::validateSwitchParameters (VcsFabricLocalSwitchInfoObject &switchInfo)
{
    if (! validDomainId (switchInfo.switchDomainId))
    {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::validateSwitchParameters: invalid remote domainId = ")+ switchInfo.switchDomainId+" remote dcaBitMap = "+switchInfo.dcaDomainsBitMap.toString ());
        return false;
    }

        if (switchInfo.switchSWVersion != my_sw_version)
        {
            trace (TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::validateSwitchParameters: SW version mismatch ver = ") + switchInfo.switchSWVersion + 
                " Domain = " + switchInfo.switchDomainId);
            return false;
        }


        VcsFabricLocalObjectManager* pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance ();
        UI32 myNodeVcsId = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectVcsId ();

        if (switchInfo.switchVcsId != myNodeVcsId)
        {
            trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::validateSwitchParameters: VCS ID mismatch");
            return false;
        }
        
        
        if (! switchInfo.switchVcsMode)
        {
        trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::validateSwitchParameters: VCS mode not set");
                return false;
        }

        if (switchInfo.noVcsEnableInProgressFlag)
        {
            trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::validateSwitchParameters: no vcs enable in progress on remote switch");
            return false;
        }

        if (0 >= switchInfo.switchSerialNumber.length ())
        {
        trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::validateSwitchParameters: invalid serial number");
                return false;
        }
        return true;
}

UI32 VcsNodePrincipalSelectionWorker::getSwitchInfoForDomainList (u_short *domainIdList)
{
        int retCode = 0;
        UI32 i;
        VcsFabricLocalSwitchInfoObject switchInfo;
        u_short myDomainId = getMyDomainId ();
        UI32 triesRemaining = max_retry_count;
        UI32 skippedSwitchCount = 0;
    UI32 infoRetrievedCount = 0;
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::getSwitchInfoForDomainList: myDomainId = ")+ myDomainId);

    if (! validDomainId (myDomainId))
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::getSwitchInfoForDomainList: getMyDomainId () returned invalid myDomainId = ")+myDomainId);
        return GENERIC_ERROR;
    }

        for (i = 0; i < domainIdList[0]; i++)   // the first element of the buffer contains the # domains returned
        {
                triesRemaining = max_retry_count;
                switchInfo.switchMgmtIpAddress = getIpAddressFromDomainId (domainIdList[i+1]);
                switchInfo.switchMgmtPort = getMgmtPort (domainIdList[i+1]);
                switchInfo.switchDomainId = domainIdList[i+1];
                if (domainIdList[i+1] != myDomainId) 
                {
                        while (triesRemaining)
                        {
                        triesRemaining--;
                if (isSwitchInfoRetrievedForDomainId (domainIdList[i+1]))
                {
                    break; // switch info has already been retrieved the remote switch sent an enq message to this switch
                }
                else
                {
                                    retCode = retrieveRemoteSwitchInfo (switchInfo);
                }

                                if (VCS_SUCCESS == retCode)
                                {
                    if (! validateSwitchParameters (switchInfo))
                                    {
                                            addInvalidParametersSwitchList (switchInfo);
                        skippedSwitchCount++;
                                            trace (TRACE_LEVEL_ERROR, string ("VcsFabObjectManager::gatherSwitchInfo: Vcs switch parameters not valid for domainId = ")+ switchInfo.switchDomainId);
                                    }
                                    else
                                    {
                                                addRemoteSwitchInfo (switchInfo);
                        infoRetrievedCount++;
                        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::gatherSwitchInfo: remote domainId = ")+ switchInfo.switchDomainId+" remote dcaBitMap = "+switchInfo.dcaDomainsBitMap.toString ());
                                    }
                                        setSwitchInfoRetrievedBit (switchInfo.switchDomainId);
                    break;
                                }
                else
                {
                                    if (0 == triesRemaining)
                                    {
                                            trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::getSwitchInfoForDomainList: retry count exceeded getting switch info for domain = ")+ domainIdList[i+1]);
                        addSwitchInfoNotRetrievedList (switchInfo);
                                    skippedSwitchCount++;
                                            break;
                                    }
                                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::getSwitchInfoForDomainList: Error retrieving info for domainId = ")+ domainIdList[i+1] + " retries remaining = "+triesRemaining);
                    prismSleep (10);    // retry case
                }
                        }
                }
                else
                {
                        getLocalSwitchInfo (switchInfo);
                        addRemoteSwitchInfo (switchInfo);
                }
        }

    if (0 < skippedSwitchCount)
    {
                trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::getSwitchInfoForDomainList: unable to get remote switch info for all switches, domain list size = ")+ domainIdList[0] + "remote info retrieved for: " + infoRetrievedCount);
        return REMOTE_SWITCH_COMMUNICATION_ERROR;
    }
    else
    {
                return VCS_SUCCESS;
    }
}


bool VcsNodePrincipalSelectionWorker::isSwitchInfoRetrievedForDomainId (const u_short &domainId)
{
        return getInstance ()->m_knownDomainsList.switchInfoReceivedBitMap[domainId];
}

bool VcsNodePrincipalSelectionWorker::isSwitchDomainIdInMyCluster (const VcsFabricLocalSwitchInfoObject &switchInfo)
{
    trace (TRACE_LEVEL_INFO, "Entering: VcsNodePrincipalSelectionWorker::isSwitchDomainIdInMyCluster");
        VcsNodeLocalManagedObject       *pVcsNodeLocalManagedObject = getSwitchObjectFromDomainId (switchInfo.switchDomainId);
        if (NULL == pVcsNodeLocalManagedObject)
        {
                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::isSwitchDomainIdInMyCluster: switch not in my cluster domianId = ")+ switchInfo.switchDomainId);
                return false;
        }
        else
        {
                delete pVcsNodeLocalManagedObject;
                return true;
        }
}

bool VcsNodePrincipalSelectionWorker::isSwitchDomainIdInMyCluster (UI32 domainId)
{
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = getSwitchObjectFromDomainId (domainId);
    if (NULL == pVcsNodeLocalManagedObject)
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::isSwitchDomainIdInMyCluster: switch not in my cluster domainId = ")+ domainId);

        return false;
    }
    else
    {
        delete pVcsNodeLocalManagedObject;
        return true;
    }
}



void VcsNodePrincipalSelectionWorker::addSwitchesNotInCluster ()
{
    // do add switch for switches not in cluster
    VcsAddSwitchMessage *pAddSwitchMessage = new VcsAddSwitchMessage (principal_selection_worker_id);
    vector<UI32> domainIds;
    UI32 i;
    u_short myDomainId = VcsNodePrincipalSelectionWorker::getMyDomainId ();

    if (! validDomainId (myDomainId))
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::addSwitchesNotInCluster: myDomainId is invalid = ")+myDomainId);
        return;
    }

    for (i = 0; i < m_remoteSwitchInfoList.size (); i ++)
    {
        if ((m_remoteSwitchInfoList[i].switchDomainId != myDomainId) && 
            ((! isSwitchDomainIdInMyCluster (m_remoteSwitchInfoList[i])) || (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_WAITING_TO_REJOIN == m_remoteSwitchInfoList[i].nodeState)))
        {
            domainIds.push_back (m_remoteSwitchInfoList[i].switchDomainId);
        } 
        else if ((isSwitchDomainIdInMyCluster (m_remoteSwitchInfoList[i]))) 
        {
            VcsNodeLocalManagedObject   *pVcsNodeLocalManagedObject = getSwitchObjectFromDomainId (m_remoteSwitchInfoList[i].switchDomainId);
            //
            if (NULL == pVcsNodeLocalManagedObject)
            {
                trace (TRACE_LEVEL_FATAL, string ("VcsNodePrincipalSelectionWorker::addSwitchesNotInCluster: VcsNodeLocalManagedObject not found for domainId = ")+m_remoteSwitchInfoList[i].switchDomainId);
            }
            else
            {
                if (pVcsNodeLocalManagedObject->getSwitchDeleteAfterFailover() || pVcsNodeLocalManagedObject->getSpecificStatus () == VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_DISCONNECTED_FROM_CLUSTER)
                {
                    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::addSwitchesNotInCluster: SwitchDeleteAfterFailover is true, switch will be included in add list domainId = ")+m_remoteSwitchInfoList[i].switchDomainId);
                    domainIds.push_back (m_remoteSwitchInfoList[i].switchDomainId);
                }
            }

        }
    }
    if (0 < domainIds.size ())
    {
        pAddSwitchMessage->setDomainIds (domainIds);
        sendAddSwitchMessageSequencer (pAddSwitchMessage, ADD_SWITCH_CASE);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::addSwitchesNotInCluster: no new switches to add"));
    }
    return;
}

bool VcsNodePrincipalSelectionWorker::isPrincipalFailoverInProgress ()
{
        return false;
}

bool VcsNodePrincipalSelectionWorker::isThisAClusterMerge ()
{
        return false;
}

bool VcsNodePrincipalSelectionWorker::amIVcsPrincipal ()
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
void VcsNodePrincipalSelectionWorker::performClusterMerge ()
{
        trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::performClusterMerge: cluster merge will be performed");
        return;
}

void VcsNodePrincipalSelectionWorker::createClusterAndAddSwitches ()
{
        VcsAddSwitchMessage *pAddSwitchMessage = new VcsAddSwitchMessage (principal_selection_worker_id);
        vector<UI32> domainIds;
        UI32 i;
        u_short myDomainId = VcsNodePrincipalSelectionWorker::getMyDomainId ();
    if (! validDomainId (myDomainId))
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::createClusterAndAddSwitches: getMyDomainId () returned invalid myDomainId = ")+myDomainId);
        return;
    }

        for (i = 0; i < m_remoteSwitchInfoList.size (); i ++)
        {
                if (m_remoteSwitchInfoList[i].switchDomainId != myDomainId)
                {
                        domainIds.push_back (m_remoteSwitchInfoList[i].switchDomainId);
                }
        }
        setClusterFormationFlag (true);
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::createClusterAndAddSwitches: clusterFormationFlag set to true, sending addswitch message to VcsFabObjectManager"));
        pAddSwitchMessage->setDomainIds (domainIds);
        sendAddSwitchMessageSequencer (pAddSwitchMessage, CREATE_CLUSTER_CASE);
        //sendSynchronousAddSwitchMessageSequencer (pAddSwitchMessage, CREATE_CLUSTER_CASE);
}

void VcsNodePrincipalSelectionWorker::becomePrincipal ()
{
    if (getInstance ()->getPrincipalFailureFlag ())
    {
        getInstance ()->setPrincipalFailureFlag(false);

        m_isNodeReadyForSCNHandling = false;

        /* Send Message to Framework */
        VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance ();

        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::becomePrincipal : Message IP " + getInstance()->getFailedNodeIp()
                + " Port " + getInstance()->getFailedNodePort() + " LocationID " + FrameworkToolKit::getLocationIdForIpAddressAndPort(getInstance()->getFailedNodeIp(), getInstance()->getFailedNodePort())));
        
        FrameworkObjectManagerNewPrincipalEstablishedMessage *pFrameworkObjectManagerNewPrincipalEstablishedMessage = new FrameworkObjectManagerNewPrincipalEstablishedMessage
            (getInstance()->getFailedNodeIp(), getInstance()->getFailedNodePort(), FrameworkToolKit::getLocationIdForIpAddressAndPort(getInstance()->getFailedNodeIp(), getInstance()->getFailedNodePort()));

        UI32 locationIdOfFailedNode =  FrameworkToolKit::getLocationIdForIpAddressAndPort(getInstance()->getFailedNodeIp(), getInstance()->getFailedNodePort());

        tracePrintf(TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::becomePrincipal : domain Id of failed Principal: %d",locationIdOfFailedNode);

        
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (locationIdOfFailedNode,"locationId"));
        vector<WaveManagedObject *> *pResultsLocalNodes = querySynchronously (&synchronousQueryContext);
        if (NULL == pResultsLocalNodes)
        {
            trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::becomePrincipal: results pointer from querySynchronously is NULL");
            prismAssert (false, __FILE__, __LINE__);
        }

        if(pResultsLocalNodes->size () > 1) 
        {
           trace(TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::becomePrincipal: More than one object found");
           prismAssert (false, __FILE__, __LINE__);
    
        }
        VcsNodeLocalManagedObject* pVcsNodeLocalManagedObjectOfFailedPrincipal = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsLocalNodes)[0]);

        prismAssert(pVcsNodeLocalManagedObjectOfFailedPrincipal!=NULL, __FILE__, __LINE__); 
        if(pVcsNodeLocalManagedObjectOfFailedPrincipal->getSwitchDeleteAfterFailover ()) 
        {
          trace(TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::becomePrincipal : Setting trigger for controlled failover");
          pFrameworkObjectManagerNewPrincipalEstablishedMessage->setTriggerControlledFaileover();
        }

        WaveMessageStatus status = pVcsFabricLocalObjectManager->sendOneWay (pFrameworkObjectManagerNewPrincipalEstablishedMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
           trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::becomePrincipal: One way message not successful");
        }

        VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance();

        prismAssert ((NULL != pVcsFabObjectManager), __FILE__, __LINE__);

        pVcsFabObjectManager->setBecomePrincipalFlag(true); 

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsLocalNodes);


    } 
    else 
    {
        trace (TRACE_LEVEL_FATAL, "VcsNodePrincipalSelectionWorker::becomePrincipal: NO HEARTBEAT FAILURE ");
    }
   
}

UI32 VcsNodePrincipalSelectionWorker::establishVcsPrincipal (u_short *fabDomainList)
{

        trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::establishVcsPrincipal: Entering ...");
        string ipAddress;
        UI32 myDomainId = getMyDomainId ();
    UI32 minIndex = 0;
    BitMap domainsInCluster;
    setBitMapSize (domainsInCluster);
    domainsInCluster.reset ();
        bool clusterExists;
        UI32 vcsClusterId = 0;
        vcsClusterId = getVcsClusterId (clusterExists);

        if (! validDomainId (myDomainId))
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: getMyDomainId () retruned invalid myDomainId = ")+myDomainId);
        return DOMAIN_ID_INVALID;
    }

    if (0 == m_remoteSwitchInfoList.size ())
        {
                trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: Switches list is empty"));
                return NO_SWITCHES_TO_ADD;
        }
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: # switches to add = ") + m_remoteSwitchInfoList.size () + " Principal = " + amIVcsPrincipal ());

        if (! amIVcsPrincipal ())
        {
                // check if a remote switch is already principal, it also checks if the switch is in vcs cluster
                if (vcsPrincipalExistsInDomainList ())
                {
                        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: Vcs Principal already exists"));
                        return PRINCIPAL_AND_CLUSTER_ALREADY_EXIST; // principal & cluster already exists
                }
                else if (clusterFormationInDomainList ())
                {
                        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: Vcs Cluster formation in progress"));
                        return CLUSTER_FORMATION_IN_PROGRESS;
                }
                else if (clusterExists)
                {
                        //cluster exists, but no principal in cluster, and no cluster fromation in progress hence there must have
                        // a principal heartbeat failure
                        if (isMyDomainIdLowest (minIndex))
                        {
                          if(FrameworkToolKit::getThisLocationRole() != LOCATION_SECONDARY_UNCONFIRMED && FrameworkToolKit::getThisLocationRole() != LOCATION_PRIMARY_UNCONFIRMED)
                           {
                                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: Principal failover: domain ID for this switch is lowest myDomainId = ")+ myDomainId );

                                becomePrincipal ();
                                return PRINCIPAL_FAILOVER_CASE;
                           }
                           else 
                           {
                            trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: This is a cluster reboot case. This switch is going to become the Principal. myDomainID = ") + myDomainId);
                            addSwitchesNotInCluster ();
                            return ADD_SWITCH_CASE;
                           }

                        }
                        else
                        {
                                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: Principal failover domain ID for this switch is not the lowest another switch will become Principal myDomainId = ")+ myDomainId );
                                m_willNotBePrincipalFlag = true;
                                return DOMAINID_NOT_LOWEST;
                        }
                }
                else
        {      
            // I'm not principal vcs cluster doesn't exist and another switch is not forming a cluster
                        if (isMyDomainIdLowest (minIndex))
                        {
                                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: Domain ID for this switch is lowest myDomainId = ")+ myDomainId );
                                if (1 >= m_remoteSwitchInfoList.size ()) // only one switch in domain list so no cluster to form
                                {
                                        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: There is only one switch in cluster so no Principal will be selected"));
                                        return NO_SWITCHES_TO_ADD;      
                                }
                                else
                                {
                                        createClusterAndAddSwitches (); // this will make this switch principal also
                        return CREATE_CLUSTER_CASE;
                                }
                        }
                        else
                        {
                                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: Domain ID for this switch is not the lowest another switch will become Principal myDomainId = ")+ myDomainId );
                                m_willNotBePrincipalFlag = true;
                                return DOMAINID_NOT_LOWEST;
                        }
                }
        }
        else
        {
                // do I'm principal case
                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: isPrincipal property is set for this switch"));
            if (vcsPrincipalExistsInDomainList ())
                {
                        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: Multiple principals exist in Vcs Cluster"));
                    if (isThereAnotherVcsPrincipalInCluster     ())
                        {   // another switch is now principal
                                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: Relinquish principal since there is another Principal in the cluster"));
                                relenquishPrincipal ();
                                return ANOTHER_SWITCH_IS_PRINCIPAL;
                        }
                        else
                        {       // there is another Principal in the known domain list but not in my cluster so must be a cluster merge
                                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: Principal exists in fabDomainList, cluster merge will be performed"));
                                performClusterMerge ();
                                return CLUSTER_MERGE_CASE;
                        }
                }
                else if (isPrincipalFailoverInProgress ())
                {
                        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: Relinquish principal since a Principal failover is in progress"));
                        relenquishPrincipal ();
                        return PRINCIPAL_FAILOVER_CASE;
                }
                else
                {
                        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::establishVcsPrincipal: This switch is still Principal myDomainID = ") + myDomainId);
                        addSwitchesNotInCluster ();
                        return ADD_SWITCH_CASE;
                }
        }
}

bool VcsNodePrincipalSelectionWorker::myDomainListMatchesRemoteSwitches ()
{
        if (getInstance ()->m_dcaDomainListSize != getInstance ()->m_remoteSwitchInfoList.size () || (0 >= getInstance ()->m_dcaDomainListSize))
        {
        if (1 >= getInstance ()->m_remoteSwitchInfoList.size ())    // local switch is the only switch in the remote switch list
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::myDomainListMatchesRemoteSwitches: No information was retrieved for any remote switch"));
            return false;
        }
        else
        {
                    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::myDomainListMatchesRemoteSwitches: information for all remote switches has not been retrieved these will not join the cluster # in domain list bit map = ")+ m_dcaDomainListSize +" # remoteInfoList = "+ m_remoteSwitchInfoList.size ());
        }
        }
        UI32 i;
        string myList = "";
        string remoteList = "";
        for (i = 0; i < getInstance ()->m_remoteSwitchInfoList.size (); i++)
        {
                if (getInstance ()->m_remoteSwitchInfoList[i].dcaDomainsBitMap != getInstance ()->m_knownDomainsList.dcaDomainsBitMap)
                {
                        myList = getInstance ()->m_knownDomainsList.dcaDomainsBitMap.toString ();
                        remoteList = getInstance ()->m_remoteSwitchInfoList[i].dcaDomainsBitMap.toString ();
                        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::myDomainListMatchesRemoteSwitches: myList and remoteList mismatch for domainId = ")+getInstance ()->m_remoteSwitchInfoList[i].switchDomainId+" myList = "+myList +" remoteList = "+remoteList);
                        return false;
                }
        }
        return true;
}

void VcsNodePrincipalSelectionWorker::establishPrincipalTimer ()
{
        BitMap switchesNotAdded;
        setBitMapSize (switchesNotAdded);
        u_short fabDomainList[max_domain_list];
        size_t recv_size = (sizeof (u_short)) * 257;
        
    VcsNodePrincipalSelectionWorker::m_timerRunningFlag = false;
    UI32 retCode = fabGetDomainList(fabDomainList, recv_size);
        if (0 != retCode)
        {
                trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::estahlishPrincipalTimer: fabGetDomainList returned error = ")+retCode+" timer will be restarted");
        startPrincipalSelectionTimer (wait_domain_lists_sync);
                return;
        }

        if (m_fabDomainValid && fabAndMyDomainListsMatch (fabDomainList))
        {
                getSwitchInfoForDomainList (fabDomainList);
        if (0 < (VcsNodePrincipalSelectionWorker::getInvalidParametersSwitchList ()).size ())
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::estahlishPrincipalTimer: # switches with invalid parameters = ")+(VcsNodePrincipalSelectionWorker::getInvalidParametersSwitchList ()).size ());
        }
        if (0 < (VcsNodePrincipalSelectionWorker::getSwitchInfoNotRetrievedList ()).size ())
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::estahlishPrincipalTimer: # switches for which information could not be retrieved = ")+(VcsNodePrincipalSelectionWorker::getSwitchInfoNotRetrievedList ()).size ());
        }
                if (myDomainListMatchesRemoteSwitches ())
                {
                        m_willNotBePrincipalFlag = false;
                        retCode = establishVcsPrincipal (fabDomainList);
            switch (retCode)
            {
            // Even though there were switches based on DCA SCNs information could not be retrieved for any
            // So we'll keep the timer running
            case NO_SWITCHES_TO_ADD:
                        // Another switch is in process of forming a cluster
            
                        case CLUSTER_FORMATION_IN_PROGRESS:
            // No principal in cluster, another switch to become principal, heart beat monitoring is not
            // yet enabled, so set timer in case the principal to be dies.
            
            case DOMAINID_NOT_LOWEST:
                {
                                        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::estahlishPrincipalTimer: setting long timeout, waiting for cluster formation"));
                            startPrincipalSelectionTimer (wait_for_principal);
                    break;
                }
            // We're done for all other cases
            
            default:
                {
                    break;
                }
            }
        }
        else
                {
                        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::estahlishPrincipalTimer: my dca list and remote switches dca list dont' match yet"));
                    startPrincipalSelectionTimer (wait_domain_lists_sync);
                }
        }
        else
        {
                WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::estahlishPrincipalTimer: my dca list and fab domain lists don't match yet"));
                startPrincipalSelectionTimer (wait_domain_lists_sync);
        }
}

void VcsNodePrincipalSelectionWorker::addMessageCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, VcsNodePSMessagingContext *pVcsNodePSMessagingContext)
{
    ResourceId completionStatus;
        vector<string> clusterGUIDs;
    VcsAddSwitchMessage *pAddSwitchMessage = dynamic_cast<VcsAddSwitchMessage *> (pPrismMessage);
    setClusterFormationFlag (false);
        --(*pVcsNodePSMessagingContext);
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::addMessageCallback: clusterFormationFlag set to false, returned from addswitch message"));
    if (FRAMEWORK_SUCCESS == frameworkStatus) 
    {
        prismAssert ((NULL != pAddSwitchMessage), __FILE__, __LINE__);
        completionStatus = pAddSwitchMessage->getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS != completionStatus) 
        {
                WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::addMessageCallback: AddSwitchMessage completionStatus error timer will be restarted error = ") +FrameworkToolKit::localize (completionStatus));
                    startPrincipalSelectionTimer (wait_domain_lists_sync);
        } 
        else 
        {
            BitMap domainsInCluster;
            setBitMapSize (domainsInCluster);
            domainsInCluster.reset ();
            UI32 myDomainId = getMyDomainId ();
            UI32 numDomainsInCluster = getDomainsInCluster (domainsInCluster);
                BitMap switchesNotAdded;
                setBitMapSize (switchesNotAdded);
            UI32 nSwitchesNotAdded;
            UI32 i;
            if (1 < numDomainsInCluster)
            {
                
                                if (CREATE_CLUSTER_CASE == pVcsNodePSMessagingContext->getEstablishPrincipalCase ())
                {
                    setPrincipalFlag (true);

                }
                                clusterGUIDs = getVcsClusterGUIDsFromClusterMO ();
                                setVcsClusterGUIDs(clusterGUIDs);
                nSwitchesNotAdded = wereAllSwitchesAdded (switchesNotAdded);
                            if (0 == nSwitchesNotAdded)
                            {
                    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::addMessageCallback: cluster creation done #nodes in cluster = ") + numDomainsInCluster+ " myDomainId = "+myDomainId+" clusterGUID = "+clusterGUIDs[0]);
                            }
                            else
                            {
                                        trace (TRACE_LEVEL_INFO, "All known switches have not been added to the cluster yet so timer will be set");
                                        for     (i = 0; i < (max_domain_list -1); i++)
                                        {
                                                if (switchesNotAdded[i])
                                                {
                                                        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::addMessageCallback: switch not added domainId = ")+i);
                                                }
                                        }
                                    startPrincipalSelectionTimer (wait_domain_lists_sync);
                            }
            }
            else
            {
                                trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::addMessageCallback: cluster creation failed timer will be restarted myDomainId = ") + myDomainId+" domains in cluster = "+ numDomainsInCluster );
                            startPrincipalSelectionTimer (wait_domain_lists_sync);
            }
        }
        }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::addMessageCallback: framework status error = ")+ FrameworkToolKit::localize (frameworkStatus));
    }

    if (NULL != pAddSwitchMessage)
    {
        delete pAddSwitchMessage;
    }
    if (NULL != pVcsNodePSMessagingContext)
    {
        delete pVcsNodePSMessagingContext;
    }
}

ResourceId VcsNodePrincipalSelectionWorker::sendAddSwitchMessageStep (VcsNodePSMessagingContext *pVcsNodePSMessagingContext)
{
    VcsAddSwitchMessage *pVcsAddSwitchMessage = dynamic_cast<VcsAddSwitchMessage *>(pVcsNodePSMessagingContext->getPMessage ());
    prismAssert (NULL != pVcsAddSwitchMessage, __FILE__, __LINE__);
    ++(*pVcsNodePSMessagingContext);
        UI32 i;
        vector<UI32> domainList = pVcsAddSwitchMessage->getDomainIds ();
        for (i = 0; i < domainList.size (); i++)
        {
                printf ("sendAddSwitchMessageStep: domainId%d = %d\n\r",i,domainList[i]);
        }

        pVcsAddSwitchMessage->setSenderId (principal_selection_worker_id);
    ResourceId status = send (pVcsAddSwitchMessage, 
        reinterpret_cast<PrismMessageResponseHandler> (&VcsNodePrincipalSelectionWorker::addMessageCallback), 
        pVcsNodePSMessagingContext, 0, FrameworkToolKit::getThisLocationId ());
    
    if (WAVE_MESSAGE_SUCCESS != status)
        {
                trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::sendAddSwitchMessageStep: send async add switch message failed, status = " + FrameworkToolKit::localize (status));
        --(*pVcsNodePSMessagingContext);
        return WAVE_MESSAGE_ERROR;
        }
    else
    {
        trace (TRACE_LEVEL_SUCCESS, "VcsNodePrincipalSelectionWorker::sendAddSwitchMessageStep: add async switch message successfully sent");
        return WAVE_MESSAGE_SUCCESS;
    }
}


void VcsNodePrincipalSelectionWorker::sendAddSwitchMessageSequencer (VcsAddSwitchMessage *pAddSwitchMessage, UI32 establishPrincipalCase)
{

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodePrincipalSelectionWorker::sendAddSwitchMessageStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodePrincipalSelectionWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodePrincipalSelectionWorker::prismLinearSequencerFailedStep)
    };
    VcsNodePSMessagingContext *pVcsNodePSMessagingContext = new VcsNodePSMessagingContext (reinterpret_cast<PrismMessage *> (pAddSwitchMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVcsNodePSMessagingContext, __FILE__, __LINE__);
    pVcsNodePSMessagingContext->setEstablishPrincipalCase (establishPrincipalCase);
    pVcsNodePSMessagingContext->setPMessage (pAddSwitchMessage);
    pVcsNodePSMessagingContext->start ();
}

ResourceId VcsNodePrincipalSelectionWorker::addSwitchWithSynchronousMessageStep (VcsNodePSSynchronousMessagingContext *pVcsNodePSMessagingContext)
{
        VcsAddSwitchMessage *pVcsAddSwitchMessage = dynamic_cast<VcsAddSwitchMessage *>(pVcsNodePSMessagingContext->getPMessage ());

        if (NULL == pVcsAddSwitchMessage)
        {
                trace (TRACE_LEVEL_FATAL, "VcsNodePrincipalSelectionWorker::addSwitchWithSynchronousMessageStep: pVcsAddSwitchMessage is NULL");
        }
    prismAssert (NULL != pVcsAddSwitchMessage, __FILE__, __LINE__);
        ResourceId completionStatus = WAVE_MESSAGE_ERROR;
        UI32 i;
        vector<UI32> domainList = pVcsAddSwitchMessage->getDomainIds ();
        for (i = 0; i < domainList.size (); i++)
        {
                trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::addSwitchWithSynchronousMessageStep: add domainId = ")+domainList[i]);
        }

        pVcsAddSwitchMessage->setSenderId (principal_selection_worker_id);
    ResourceId status = sendSynchronously (pVcsAddSwitchMessage);
    
    if (WAVE_MESSAGE_SUCCESS != status)
        {
                trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::addSwitchWithSynchronousMessageStep: send async add switch message failed, status = " + FrameworkToolKit::localize (status));
        return WAVE_MESSAGE_ERROR;
        }
    else
    {
        completionStatus = pVcsAddSwitchMessage->getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS != completionStatus) 
        {
                WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::addSwitchWithSynchronousMessageStep: add message completion status error timer will be restarted: ") +FrameworkToolKit::localize (completionStatus));
                    startPrincipalSelectionTimer (wait_domain_lists_sync);
        } 
                else 
        {
            BitMap domainsInCluster;
            setBitMapSize (domainsInCluster);
            domainsInCluster.reset ();
            UI32 myDomainId = getMyDomainId ();
            UI32 numDomainsInCluster = getDomainsInCluster (domainsInCluster);
                BitMap switchesNotAdded;
                setBitMapSize (switchesNotAdded);
            UI32 nSwitchesNotAdded;
            if (1 < numDomainsInCluster)
            {
                nSwitchesNotAdded = wereAllSwitchesAdded (switchesNotAdded);
                            if (0 == nSwitchesNotAdded)
                            {
                    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::addSwitchWithSynchronousMessageStep: cluster creation done #nodes in cluster = ") + numDomainsInCluster+ " myDomainId = "+myDomainId);
                            }
                            else
                            {
                                        trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::addSwitchWithSynchronousMessageStep: All known switches have not been added to the cluster yet so timer will be set");
                                        for     (i = 0; i < (max_domain_list -1); i++)
                                        {
                                                if (switchesNotAdded[i])
                                                {
                                                        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::addSwitchWithSynchronousMessageStep: switch not added domainId = ")+i);
                                                }
                                        }
                                    startPrincipalSelectionTimer (wait_domain_lists_sync);
                            }
            }
            else
            {
                                trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::addSwitchWithSynchronousMessageStep: cluster creation failed timer will be restarted myDomainId = ") + myDomainId+" domains in cluster = "+ numDomainsInCluster );
                            startPrincipalSelectionTimer (wait_domain_lists_sync);
            }
        }
    }
    if (NULL != pVcsAddSwitchMessage)
    {
        delete pVcsAddSwitchMessage;
    }
    if (NULL != pVcsNodePSMessagingContext)
    {
        delete pVcsNodePSMessagingContext;
    }
        return completionStatus;
}

void VcsNodePrincipalSelectionWorker::sendSynchronousAddSwitchMessageSequencer (VcsAddSwitchMessage *pAddSwitchMessage, UI32 establishPrincipalCase)
{

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodePrincipalSelectionWorker::addSwitchWithSynchronousMessageStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodePrincipalSelectionWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodePrincipalSelectionWorker::prismSynchronousLinearSequencerFailedStep)
    };
    VcsNodePSSynchronousMessagingContext *pVcsNodePSSynchronousMessagingContext = new VcsNodePSSynchronousMessagingContext (reinterpret_cast<PrismMessage *> (pAddSwitchMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVcsNodePSSynchronousMessagingContext, __FILE__, __LINE__);
    prismAssert (NULL != pAddSwitchMessage, __FILE__, __LINE__);
    pVcsNodePSSynchronousMessagingContext->setEstablishPrincipalCase (establishPrincipalCase);
    pVcsNodePSSynchronousMessagingContext->setPMessage (pAddSwitchMessage);
    pVcsNodePSSynchronousMessagingContext->execute ();
}

void VcsNodePrincipalSelectionWorker::startPrincipalSelectionTimer (UI32 timeoutValue)
{
    clearSwitchInfoRetrievedBitMap ();
    clearSwitchInfoLists ();

        // Principal selection is disabled
        if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == VcsFabricLocalObjectManager::getThisVcsNodeClusterType ())
    {
        trace (TRACE_LEVEL_WARN, "VcsNodePrincipalSelectionWorker::startPrincipalSelectionTimer: Principal selection and automatic cluster formation is disabled.");
                return;
    }

    if (! VcsNodePrincipalSelectionWorker::vcsModeEnabled ())
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::startPrincipalSelectionTimer: VCS mode not set for this switch PrincipalSelectionTimer not started");
        //trace (TRACE_LEVEL_WARN, "VcsNodePrincipalSelectionWorker::startPrincipalSelectionTimer: Principal selection and automatic cluster formation is disabled in this build");
                return;
    }

    // Check if the node is in the middle of rolling back the previous failed cluster formation step.
    if (isNodeRollingBack ())
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::startPrincipalSelectionTimer: Node in the middle of rolling back, PrincipalSelectionTimer not started.");
        return;
    }
    
    ResourceId status = startTimer (VcsNodePrincipalSelectionWorker::m_timerHandle, timeoutValue, reinterpret_cast<PrismTimerExpirationHandler> (&VcsNodePrincipalSelectionWorker::establishPrincipalTimerWithAsyncEnq), NULL);
    if (FRAMEWORK_SUCCESS == status)
    {
        VcsNodePrincipalSelectionWorker::m_timerRunningFlag = true;
        trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::startPrincipalSelectionTimer: PrincipalSelectionTimer starting");
    }
    else
    {
        VcsNodePrincipalSelectionWorker::m_timerRunningFlag = false;
        trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::startPrincipalSelectionTimer: PrincipalSelectionTimer could not be started error = "+FrameworkToolKit::localize (status));
    }
}

void VcsNodePrincipalSelectionWorker::disableTimer ()
{
    trace (TRACE_LEVEL_DEBUG, "VcsNodePrincipalSelectionWorker::disableTimer : Entering ...");

        if (VcsNodePrincipalSelectionWorker::m_timerRunningFlag)
        {
                if (FRAMEWORK_SUCCESS != deleteTimer (VcsNodePrincipalSelectionWorker::m_timerHandle))
                {
                        trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::disableTimer: deleteTimer FAILED.");
                        return;
                }
                else
                {
                        trace (TRACE_LEVEL_SUCCESS, "VcsNodePrincipalSelectionWorker::disableTimer deleteTimer SUCCEEDED.");
                        VcsNodePrincipalSelectionWorker::m_timerRunningFlag = false;
                }
        }
        else
        {
                trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::disableTimer: timer not running.");
        }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Async Remote Enquire message processing functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void VcsNodePrincipalSelectionWorker::startEstablishPrincipalSteps ()
{
        static UI32 numberOfTimeOuts = 1;
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::startEstablishPrincipalSteps : Timeout # ") + numberOfTimeOuts);
        numberOfTimeOuts++;

        if (! getInstance ()->m_isNodeReadyForSCNHandling)
        {
                trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::startEstablishPrincipalSteps : NodeReadyForSCNHandling not received yet so timer will be set");
        getInstance ()->startPrincipalSelectionTimer (wait_domain_lists_sync);
                return;
        }
    VcsNodePrincipalSelectionWorker::m_timerRunningFlag = false;

    u_short fabDomainList[max_domain_list];
    size_t recv_size = (sizeof (u_short)) * 257;
    UI32 retCode = fabGetDomainList(fabDomainList, recv_size);

        if (0 != retCode)
        {
                trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::startEstablishPrincipalSteps: fabGetDomainList error code = ") + retCode);
        getInstance ()->startPrincipalSelectionTimer (wait_domain_lists_sync);
                return;
        }

        if (getInstance ()->m_fabDomainValid && fabAndMyDomainListsMatch (fabDomainList))
        {
        getInstance ()->getSwitchInfoForDomainListAsyncMessageSequencer (fabDomainList);
        }
        else
        {
                WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::startEstablishPrincipalSteps: my dca list and fab domain lists don't match yet"));
        WaveNs::trace (TRACE_LEVEL_INFO, string ("Domain Valid: ") + getInstance ()->m_fabDomainValid);
                getInstance ()->startPrincipalSelectionTimer (wait_domain_lists_sync);
        }
}

void VcsNodePrincipalSelectionWorker::startPrincipalCleanupSteps (PrismHeartbeatFailureObjectManagerMessage *pHeartbeatFailureMessage)
{
    VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance ();

    /* Send Message to Framework */
    trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::startPrincipalCleanupSteps : Message IP " + (pHeartbeatFailureMessage->getIpAddress()).toString()
            + " Port " + pHeartbeatFailureMessage->getPortNumber() + " LocationID " + FrameworkToolKit::getLocationIdForIpAddressAndPort((pHeartbeatFailureMessage->getIpAddress()).toString(), pHeartbeatFailureMessage->getPortNumber())));
    FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage = new FrameworkObjectManagerSecondaryNodeFailureNotificationMessage
        ((pHeartbeatFailureMessage->getIpAddress()).toString(), pHeartbeatFailureMessage->getPortNumber(), 
        FrameworkToolKit::getLocationIdForIpAddressAndPort((pHeartbeatFailureMessage->getIpAddress()).toString(), pHeartbeatFailureMessage->getPortNumber()));

    WaveMessageStatus status = pVcsFabricLocalObjectManager->sendOneWay (pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
       WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::startPrincipalCleanupSteps: One way message not successful");
    }
}

void VcsNodePrincipalSelectionWorker::establishPrincipalTimerWithAsyncEnq (TimerHandle timerHandle, void * pContext)
{
    // Pop out of timer, set to FALSE 
    VcsNodePrincipalSelectionWorker::m_timerRunningFlag = false;

    if (!((VcsFabricLocalObjectManager::getInstance ())->getNoVcsEnableInProgressFlag ())) 
    {
        startEstablishPrincipalSteps ();
}
}

void VcsNodePrincipalSelectionWorker::continueEstablishPrincipalAfterRemoteEnqComplete (VcsNodePSMessagingContext *pVcsNodePSMessagingContext)
{
    BitMap switchesNotAdded;
        setBitMapSize (switchesNotAdded);
    UI32 retCode;
    u_short *fabDomainList = pVcsNodePSMessagingContext->getPDomainList ();

        WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::continueEstablishPrincipalAfterRemoteEnqComplete: async enq message processing completed");

   if ((VcsFabricLocalObjectManager::getInstance ())->getNoVcsEnableInProgressFlag ())
   {
       WaveNs::trace (TRACE_LEVEL_INFO,
       "VcsNodePrincipalSelectionWorker::continueEstablishPrincipalAfterRemoteEnqComplete: Terminating principal selection sequence as noVcsEnable is in progress");
       return;
    }

        if (myDomainListMatchesRemoteSwitches ())
        {
                VcsNodePrincipalSelectionWorker::m_willNotBePrincipalFlag = false;
                WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::continueEstablishPrincipalAfterRemoteEnqComplete: my dca list and remote switches dca list match calling establishPrincipal"));
                retCode = establishVcsPrincipal (fabDomainList);
        switch (retCode)
        {
        // Even though there were switches based on DCA SCNs information could not be retrieved for any
        // So we'll keep the timer running
        case NO_SWITCHES_TO_ADD:

        // No principal in cluster, another switch to become principal, heart beat monitoring is not
        // yet enabled, so set timer in case the principal to be dies.
        case DOMAINID_NOT_LOWEST:
            {
                        startPrincipalSelectionTimer (wait_for_principal);
                break;
            }
        // We're done for all other cases
        default:
            {
                break;
            }
        }
    }
    else
        {
                WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::estahlishPrincipalTimer: my dca list and remote switches dca list dont' match yet"));
                startPrincipalSelectionTimer (wait_domain_lists_sync);
        }
}

void VcsNodePrincipalSelectionWorker::getRemoteSwitchInfoAsyncCallBack  (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, VcsNodePSMessagingContext *pVcsNodePSMessagingContext)
{
    VcsFabricLocalSwitchInfoObject switchInfo;
    ResourceId completionStatus;
        vector<string> clusterGUIDs;
    VcsRemoteEnqSwitchMessage *pVcsRemoteEnqSwitchMessage = dynamic_cast<VcsRemoteEnqSwitchMessage *>(pPrismMessage);
    if (FRAMEWORK_SUCCESS == frameworkStatus) 
    {
        prismAssert ((NULL != pVcsRemoteEnqSwitchMessage), __FILE__, __LINE__);
        completionStatus = pVcsRemoteEnqSwitchMessage->getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS != completionStatus) 
        {
                WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::getRemoteSwitchInfoAsyncCallBack : VcsRemoteEnqSwitchMessage completionStatus error = ") +FrameworkToolKit::localize (completionStatus));
            pVcsRemoteEnqSwitchMessage->getSwitchInfo (switchInfo);
            addInvalidParametersSwitchList (switchInfo);
            pVcsNodePSMessagingContext->incrementNumberOfFailures ();
            delete pVcsRemoteEnqSwitchMessage;
        } 
        else
        {
            pVcsRemoteEnqSwitchMessage->getSwitchInfo (switchInfo);
            delete pVcsRemoteEnqSwitchMessage;
            if (! validateSwitchParameters (switchInfo))
            {
                    WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::getRemoteSwitchInfoAsyncCallBack: invalid remote switch parameters for domainId = ")+ switchInfo.switchDomainId);
                addInvalidParametersSwitchList (switchInfo);
            }
            else
            {
                        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::getRemoteSwitchInfoAsyncCallBack: remote domainId = ")+ switchInfo.switchDomainId+" serial# "+ switchInfo.switchSerialNumber+" remote dcaBitMap = "+switchInfo.dcaDomainsBitMap.toString ());
                                updateRemoteSwitchInfoList (switchInfo);
            }
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::getRemoteSwitchInfoAsyncCallBack: Framework error for remote enq message ")+FrameworkToolKit::localize (frameworkStatus));
        pVcsNodePSMessagingContext->incrementNumberOfFailures ();
    }
        --(*pVcsNodePSMessagingContext);
    
    if ((pVcsNodePSMessagingContext->getNumberOfCallbacksBeforeAdvancingToNextStep () == 0) && (pVcsNodePSMessagingContext->getNumberOfFailures () > 0))
    {
        disableTimer();
        startPrincipalSelectionTimer(wait_domain_lists_sync);
        pVcsNodePSMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }
    else
    {
        pVcsNodePSMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);      
    }
}

void VcsNodePrincipalSelectionWorker::getRemoteSwitchInfoAsync (VcsNodePSMessagingContext *pVcsNodePSMessagingContext)
{
    VcsFabricLocalSwitchInfoObject *pRemoteSwitchInfo = pVcsNodePSMessagingContext->getSwitchInfoParametersP ();
        VcsRemoteEnqSwitchMessage *pVcsRemoteEnqSwitchMessage = new VcsRemoteEnqSwitchMessage (*pRemoteSwitchInfo);
    prismAssert (NULL != pVcsRemoteEnqSwitchMessage, __FILE__, __LINE__);
        pVcsNodePSMessagingContext->setPMessage (pVcsRemoteEnqSwitchMessage);

    ++(*pVcsNodePSMessagingContext);
    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::getRemoteSwitchInfoAsync: sending async remote enq message to remoteMgmtIpAddress = ")+pRemoteSwitchInfo->switchMgmtIpAddress+" mgmtPort = "+pRemoteSwitchInfo->switchMgmtPort);
    ResourceId status = send (pVcsRemoteEnqSwitchMessage, 
        reinterpret_cast<PrismMessageResponseHandler> (&VcsNodePrincipalSelectionWorker::getRemoteSwitchInfoAsyncCallBack), 
        pVcsNodePSMessagingContext, 0, FrameworkToolKit::getThisLocationId ());
    
    if (WAVE_MESSAGE_SUCCESS != status)
        {
                trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::getRemoteSwitchInfoAsync: send async remote enq switch message failed, status = " + FrameworkToolKit::localize (status));
        --(*pVcsNodePSMessagingContext);
        return;
        }
    else
    {
        trace (TRACE_LEVEL_SUCCESS, "VcsNodePrincipalSelectionWorker::getRemoteSwitchInfoAsync: remote enq async switch message successfully sent");
        return;
    }
}

void VcsNodePrincipalSelectionWorker::getSwitchInfoForDomainListAsyncMessageStep (VcsNodePSMessagingContext *pVcsNodePSMessagingContext)
{
        UI32 i;
        VcsFabricLocalSwitchInfoObject switchInfo;
        u_short myDomainId = getMyDomainId ();
    u_short *domainIdList = pVcsNodePSMessagingContext->getPDomainList ();
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::getSwitchInfoForDomainListAsync: myDomainId = ")+ myDomainId);

    if (! validDomainId (myDomainId))
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::getSwitchInfoForDomainListAsync: getMyDomainId () returned invalid myDomainId = ")+myDomainId);
        return;
    }
        UI32 notRemoteCount = 0;
        for (i = 0; i < domainIdList[0]; i++)   // the first element of the buffer contains the # domains returned
    {
                
        switchInfo.switchMgmtIpAddress = getIpAddressFromDomainId (domainIdList[i+1]);
                switchInfo.switchMgmtPort = getMgmtPort (domainIdList[i+1]);
                switchInfo.switchDomainId = domainIdList[i+1];
        switchInfo.switchSWVersion = my_sw_version;
                
                if (domainIdList[i+1] != myDomainId) 
                {
                        if (! getInstance ()->m_knownDomainsList.switchInfoReceivedBitMap[domainIdList[i+1]])
                        {
                                // retrieve info from remote switch if we don't have its information - which we might if it sent us an enq message
                                pVcsNodePSMessagingContext->setSwitchInfoParametersP (&switchInfo);
                                getRemoteSwitchInfoAsync (pVcsNodePSMessagingContext);
                        }
                        else
                        {
                                WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::getSwitchInfoForDomainListAsync: switchInfo already retrieved for domainId = ")+domainIdList[i+1]);
                                notRemoteCount++;
                        }
                }
                else
                {
                        getLocalSwitchInfo (switchInfo);
                        switchInfo.switchMgmtPort = getMgmtPort (switchInfo.switchDomainId); // override default info from local switch info
                        switchInfo.dcaDomainsBitMap = getInstance ()->m_knownDomainsList.dcaDomainsBitMap;
                        updateRemoteSwitchInfoList (switchInfo);
                        notRemoteCount++;
                }
    }
        if (notRemoteCount == (domainIdList[0]))
        {
                pVcsNodePSMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);     // execute next step since no remote async request is required
        }
}

void VcsNodePrincipalSelectionWorker::getSwitchInfoForDomainListAsyncMessageSequencer (u_short *fabDomainList)
{

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodePrincipalSelectionWorker::getSwitchInfoForDomainListAsyncMessageStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodePrincipalSelectionWorker::continueEstablishPrincipalAfterRemoteEnqComplete),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodePrincipalSelectionWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodePrincipalSelectionWorker::prismLinearSequencerFailedStep)
    };
    VcsRemoteEnqSwitchMessage *pVcsRemoteEnqSwitchMessage = NULL;
    VcsNodePSMessagingContext *pVcsNodePSMessagingContext = new VcsNodePSMessagingContext (reinterpret_cast<PrismMessage *> (pVcsRemoteEnqSwitchMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVcsNodePSMessagingContext, __FILE__, __LINE__);
    pVcsNodePSMessagingContext->setPMessage (pVcsRemoteEnqSwitchMessage);
    pVcsNodePSMessagingContext->setPDomainList (fabDomainList);
    pVcsNodePSMessagingContext->start ();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End Async Remote Enquire message functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void VcsNodePrincipalSelectionWorker::updateRemoteSwitchInfoList (VcsFabricLocalSwitchInfoObject &switchInfo)
{
    SI32 i;
    SI32 numInList = (getInstance ()->m_remoteSwitchInfoList).size ();
        if (! validDomainId (switchInfo.switchDomainId))
        {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::updateRemoteSwitchInfoList: invalid domainId for remote switch = ")+switchInfo.switchDomainId);
                return;
        }
    for (i = 0; i < numInList; i++)
    {
        if (switchInfo.switchDomainId == ((getInstance ()->m_remoteSwitchInfoList)[i]).switchDomainId)
        {
            (getInstance ()->m_remoteSwitchInfoList).erase ((getInstance ()->m_remoteSwitchInfoList).begin ()+i);
                        numInList--;
                        i--;
        }
    }
        trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::updateRemoteSwitchInfoList: adding remote switch to info retrieved list for domainId = ")+switchInfo.switchDomainId+" clusterFormationFlag = "+switchInfo.switchClusterFormationFlag);
        addRemoteSwitchInfo (switchInfo);
}

void VcsNodePrincipalSelectionWorker::saveRemoteSwitchInfoFromEnqMessage (VcsEnqSwitchMessage *pVcsEnqSwitchMessage)
{
    VcsFabricLocalSwitchInfoObject switchInfo;

    switchInfo.switchDomainId = pVcsEnqSwitchMessage->getDomainId ();
    if (! validDomainId (switchInfo.switchDomainId))
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::saveRemoteSwitchInfoFromEnqMessage: invalid domainId for remote switch = ")+switchInfo.switchDomainId);
        return;
    }
        pVcsEnqSwitchMessage->getSwitchInfo (switchInfo);

    if (! validateSwitchParameters (switchInfo))
    {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodePrincipalSelectionWorker::saveRemoteSwitchInfoFromEnqMessage: invalid remote switch parameters for domainId = ")+ switchInfo.switchDomainId);
        addInvalidParametersSwitchList (switchInfo);
    }
    else
    {
                WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::saveRemoteSwitchInfoFromEnqMessage: remote domainId = ")+ switchInfo.switchDomainId+" serial# "+ switchInfo.switchSerialNumber+" cluster GUID = "+switchInfo.vcsClusterGUIDs[0]+" remote dcaBitMap = "+switchInfo.dcaDomainsBitMap.toString ());
                updateRemoteSwitchInfoList (switchInfo);
    }
}

void VcsNodePrincipalSelectionWorker::disable (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEBUG, "VcsNodePrincipalSelectionWorker::disable : Entering ...");

        if (m_timerRunningFlag)
        {
                if (FRAMEWORK_SUCCESS != deleteTimer (VcsNodePrincipalSelectionWorker::m_timerHandle))
                {
                        trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::disable: delete timer FAILED.");
                        return;
                }
                else
                {
                        trace (TRACE_LEVEL_SUCCESS, "VcsNodePrincipalSelectionWorker::disable: delete timer SUCCEEDED.");
                        m_timerRunningFlag = false;
                }
        }
        else
        {
                        trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::disable: delete timer not running.");
        }
        pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void VcsNodePrincipalSelectionWorker::shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    
    trace (TRACE_LEVEL_DEBUG, "VcsNodePrincipalSelectionWorker::shutdown : Entering ...");

    m_isNodeReadyForSCNHandling = false;

        if (m_timerRunningFlag)
        {
                if (FRAMEWORK_SUCCESS != deleteTimer (VcsNodePrincipalSelectionWorker::m_timerHandle))
                {
                        trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::shutdown: delete timer FAILED.");
                        return;
                }
                else
                {
                        trace (TRACE_LEVEL_SUCCESS, "VcsNodePrincipalSelectionWorker::shutdown delete timer SUCCEEDED.");
                        m_timerRunningFlag = false;
                }
        }
        else
        {
                        trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::shutdown: delete timer not running.");
        }
    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void  VcsNodePrincipalSelectionWorker::heartBeatFailure (PrismAsynchronousContext     *pPrismAsynchronousContext)
{
    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *> (pPrismAsynchronousContext->getPCallerContext ());
    PrismHeartbeatFailureObjectManagerMessage *pHeartbeatFailureMessage = reinterpret_cast<PrismHeartbeatFailureObjectManagerMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    string failedNodeIpAddress = pHeartbeatFailureMessage->getIpAddress().toString();
    UI32 failedNodePort = pHeartbeatFailureMessage->getPortNumber();
    UI32 failedNodeDomainId = DcmToolKit::getDomainIdFromIpAddressAndPort (failedNodeIpAddress, failedNodePort);
    cleanupTimerHandlerForDomainId(failedNodeDomainId, true);   // delete process cleanup timer handler if started by domain unreachable scn
    removeFromx86DomainList(failedNodeDomainId); // for x86 only. If env is ppc it's a nop
    
    WaveNs::trace (TRACE_LEVEL_INFO, string("VcsNodePrincipalSelectionWorker::heartbeatFailure: handling heart beat failure mydomainId = ")+getInstance ()->getMyDomainId ());
    if (getInstance ()->amIVcsPrincipal ())
    {
        // secondary failover case
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::heartbeatFailure: secondary node heartbeat failure"));
        // Start Cleanup process for Principal
        getInstance ()->startPrincipalCleanupSteps (pHeartbeatFailureMessage);
    }
    else
    {
        // principal failover case
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodePrincipalSelectionWorker::heartbeatFailure: principal node heartbeat failure"));
        // Principal selection trigger for the separate cluster
        // Secondary node will get 2 HB failure only, so we can cache the IP
        getInstance ()->setFailedNodeIp((pHeartbeatFailureMessage->getIpAddress()).toString());
        getInstance ()->setFailedNodePort(pHeartbeatFailureMessage->getPortNumber());
        getInstance ()->setPrincipalFailureFlag(true);
        getInstance ()->startEstablishPrincipalSteps ();
    }
    pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pPrismAsynchronousContext->callback ();
}

bool VcsNodePrincipalSelectionWorker::isClusterEnabled ()
{
    trace (TRACE_LEVEL_INFO, "VcsNodePrincipalSelectionWorker::isClusterEnabled: Entering ...... ");
    
    FILE * pFile;
    string clusterConfPath = DcmToolKit::getVcsConfPath ();
    clusterConfPath.append ("/cluster.conf");
    
    pFile = fopen (clusterConfPath.c_str(),"r"); 
    if (pFile == NULL)
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodePrincipalSelectionWorker::isClusterEnabled: Cannot open the cluster.conf for reading");        
        return (false);
    }

    int clusterMode = fgetc (pFile);

    tracePrintf (TRACE_LEVEL_INFO, false, false, "VcsNodePrincipalSelectionWorker::isClusterEnabled: VCS_MODE %d\n", clusterMode);

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

bool VcsNodePrincipalSelectionWorker::getClusterFormationFlag ()
{
    bool clusterFormationFlag;

    m_prismMutex.lock ();
    clusterFormationFlag = m_clusterFormationFlag;
    m_prismMutex.unlock ();

    return (clusterFormationFlag);
}

void VcsNodePrincipalSelectionWorker::setClusterFormationFlag (bool clusterFormationFlag)
{
    m_prismMutex.lock ();
    m_clusterFormationFlag = clusterFormationFlag;
    m_prismMutex.unlock ();
}


void VcsNodePrincipalSelectionWorker::setFailedNodeIp(string ip)
{
    (getInstance ())->m_failedNodeIp = ip;
}

string VcsNodePrincipalSelectionWorker::getFailedNodeIp()
{
    return (getInstance ())->m_failedNodeIp;
}

void VcsNodePrincipalSelectionWorker::setFailedNodePort(UI32 port)
{
    (getInstance ())->m_failedNodePort = port;
}

UI32 VcsNodePrincipalSelectionWorker::getFailedNodePort()
{
    return (getInstance ())->m_failedNodePort;
}

bool VcsNodePrincipalSelectionWorker::isNodeRollingBack ()
{
    bool isRollingBack = false;

    switch (FrameworkToolKit::getThisLocationRole ())                                                                                                            {
        case LOCATION_SECONDARY_CLUSTER_PHASE_0:
        case LOCATION_SECONDARY_CLUSTER_PHASE_1:                                                                                                                     case LOCATION_SECONDARY_CLUSTER_PHASE_2:
        case LOCATION_SECONDARY_CLUSTER_PHASE_3:
        case LOCATION_SECONDARY_REJOIN_PHASE_0:
        case LOCATION_SECONDARY_REJOIN_PHASE_1:
        case LOCATION_SECONDARY_REJOIN_PHASE_2:
        case LOCATION_SECONDARY_REJOIN_PHASE_3:
            isRollingBack = true;
            break;
        default:
            isRollingBack = false;
            break;
    }

    return isRollingBack;
}

void VcsNodePrincipalSelectionWorker::setPrincipalFailureFlag (bool principalFailure)
{       
    m_principalFailureMutex.lock ();
    m_principalFailureFlag = principalFailure;
    m_principalFailureMutex.unlock ();
}   
    
bool VcsNodePrincipalSelectionWorker::getPrincipalFailureFlag ()
{       
    bool isPrincipalDisconnected;
            
    m_principalFailureMutex.lock ();    
    isPrincipalDisconnected = m_principalFailureFlag;
    m_principalFailureMutex.unlock ();
                
    return (isPrincipalDisconnected);
}
#endif
}

