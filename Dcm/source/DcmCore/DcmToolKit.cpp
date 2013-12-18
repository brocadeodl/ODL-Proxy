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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "DcmCore/DcmToolKit.h"
#include <algorithm>
#include <sstream>
#include "DcmCore/DcmClientToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/AssertUtils.h"
#include "VcsFabric/Local/VcsNodeFCPrincipalTrackingWorker.h"
#include "VcsFabric/Local/VcsNodeSlotManagementWorker.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsNodeSlotMessages.h"
#include "VcsFabric/Local/VcsFabricLocalMessages.h"
#include "Framework/Utils/StringUtils.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/QueryUtils.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "Framework/Persistence/ConfigurationManagedObject.h"
#include "Framework/Persistence/PersistenceToolKit.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "sysmod/syscomm.h"
#include "sysmod/syscfg.h"
#include "fc/wwn.h"
#include "fabric/fabric.h"
#include <hasm/hasm.h>
#include <utils/logger.h> 
#include <utils/node_id.h>
#include <utils/slot.h>
#include <raslog/raslog.h>
#include <raslog/raslog_dcm.h>
#include <ha/ipadm.h>


namespace DcmNs
{
  string                                DcmToolKit::m_bpBackPlaneIpAddress;
  string                                DcmToolKit::m_cpBackPlaneIpAddress;
  bool                                  DcmToolKit::m_userForcedBackPlaneIpAddresses = false;
  string                                DcmToolKit::m_wyserAlternateExternalApplicationIpAddress                 = "127.0.0.1";
  SI32                                  DcmToolKit::m_wyserAlternateExternalApplicationPort                      = 4567;
  string                                DcmToolKit::m_vcsConfPath                    = ".";
  bool                                  DcmToolKit::m_ipotEnableFlag                 = false;
  bool                                  DcmToolKit::m_fcPrincipalEnabled             = true;
  ResourceId                            DcmToolKit::m_distributionMode;
  string                                DcmToolKit::m_applicationIdentifier          = "dcm";
  UI32                                  DcmToolKit::m_mgmtPort                       = 9110;
  UI32                                  DcmToolKit::m_swBDModel                      = 0;
  string                                DcmToolKit::m_currentFwVersion               = "";
  string                                DcmToolKit::m_oldFwVersion                   = "";
  ValClientSynchronousConnection       *DcmToolKit::m_pInterDcmCcmClient             = NULL;
  PrismMutex                            DcmToolKit::m_interDcmCcmClientMutex;
  bool                                  DcmToolKit::m_isCliAllowedFromThisLocation   = false;
  bool                                  DcmToolKit::m_isCliAllowedAtThisLocation     = false;


  static SI32                           s_managementInterfaceReceiverPortForDcmd     = 9110;
  

void DcmToolKit::setBackPlaneIpAddresses (const string &bpBAckPlaneIpAddress, const string &cpBackPlaneIpAddress)
{
    m_bpBackPlaneIpAddress = bpBAckPlaneIpAddress;
    m_cpBackPlaneIpAddress = cpBackPlaneIpAddress;

    m_userForcedBackPlaneIpAddresses = true;
}



string DcmToolKit::getWyserAlternateExternalApplicationIpAddress ()
{
    return (m_wyserAlternateExternalApplicationIpAddress);
}

void DcmToolKit::setWyserAlternateExternalApplicationIpAddress (const string &wyserAlternateExternalApplicationIpAddress)
{
    m_wyserAlternateExternalApplicationIpAddress = wyserAlternateExternalApplicationIpAddress;
}

string DcmToolKit::getVcsConfPath ()
{
    return (m_vcsConfPath);
}

void DcmToolKit::setVcsConfPath (const string &vcsConfPath)
{
    m_vcsConfPath = vcsConfPath;
}

SI32 DcmToolKit::getWyserAlternateExternalApplicationPort ()
{
    return (m_wyserAlternateExternalApplicationPort);
}

void DcmToolKit::setWyserAlternateExternalApplicationPort (const SI32 &wyserAlternateExternalApplicationPort)
{
    m_wyserAlternateExternalApplicationPort = wyserAlternateExternalApplicationPort;
}

void DcmToolKit::setMgmtPort                        (const UI32 &mgmtPort)
{
    m_mgmtPort = mgmtPort;
}
UI32 DcmToolKit::getMgmtPort                        ()
{
    return m_mgmtPort;
}

void DcmToolKit::setDistributionMode (const string &distributionMode)
{
    if ("DCM_ONLY" == distributionMode)
    {
        m_distributionMode = DCM_ONLY;
        m_applicationIdentifier = "dcm";
    }
    else if ("CCM_ONLY" == distributionMode)
    {
        m_distributionMode = CCM_ONLY;
        m_applicationIdentifier = "ccm";
    }
    else if ("CCM_NOOP" == distributionMode)
    {
        m_distributionMode = CCM_NOOP;
        m_applicationIdentifier = "ccm";
    }
    else if ("DCM_NSP" == distributionMode)
    {
        m_distributionMode      = DCM_NSP;
        m_applicationIdentifier = "nsp";
    }
    else if ("DCM_SMD" == distributionMode)
    {
        m_distributionMode      = DCM_SMD;
        m_applicationIdentifier = "smd";
    }
    else
    {
        m_distributionMode = DCM_CCM;
        m_applicationIdentifier = "dcm";
    
        FrameworkToolKit::setConfigurationCompatibilityCheckRequired (true);
    }
}

ResourceId DcmToolKit::getDistributionMode ()
{
    return (m_distributionMode);
}

void DcmToolKit::setSwBDModel( const UI32 &swBDModel)
{
    if ( m_swBDModel != 0 )
    {
        //It can be set only once during Dcmd start. Do not allow again.        
        prismAssert (false, __FILE__, __LINE__);
    }
    
    if ( swBDModel == 0 )
    {   
        //It cannot be set with zero.        
        prismAssert (false, __FILE__, __LINE__);
    }

    m_swBDModel = swBDModel;
}

UI32 DcmToolKit::getSwBDModel()
{
    if ( m_swBDModel == 0 )
    {   
        //It cannot be called before we have set as part of Dcmd start. We treat this as unexpected early call.        
        prismAssert (false, __FILE__, __LINE__);
    }
    return (m_swBDModel);
}

UI32 DcmToolKit::getSwBDModelForDomainId( const UI32 &domainId )
{
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;
  
    UI32 swbdModel = 0;
    pVcsNodeLocalManagedObject = getLocalManagedObjectFromDomainId (domainId);
  
    if ( NULL != pVcsNodeLocalManagedObject )
    {
        swbdModel = pVcsNodeLocalManagedObject->getSWBDModel();
        delete  pVcsNodeLocalManagedObject;
    }
    return (swbdModel);
}

string DcmToolKit::getApplicationIdentifier()
{
    return (m_applicationIdentifier);
}

ObjectId DcmToolKit::getThisVcsNodeObjectId ()
{
    if (false == (VcsFabricLocalObjectManager::getIsInstantiated ()))
    {
       return (ObjectId::NullObjectId);
    }
    else
    {
       return ((VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeObjectId ());
    }
}

ObjectId DcmToolKit::getVcsNodeObjectIdFromMappedId (const UI32 &mappedId)
{
    ObjectId vcsNodeObjectId = ObjectId::NullObjectId;

    if (true == (VcsFabricLocalObjectManager::getIsInstantiated ()))
    {
       VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = NULL;  
       pVcsNodeLocalManagedObject = getLocalManagedObjectFromDomainId (mappedId);

       if (NULL != pVcsNodeLocalManagedObject)
       {
          vcsNodeObjectId = pVcsNodeLocalManagedObject->getObjectId();
          delete pVcsNodeLocalManagedObject;  
       }
    }

    return (vcsNodeObjectId);
}

string	 DcmToolKit::getIpAddressFromDomainId (const UI32 &domainId)
{
	prismAssert (256 > domainId, __FILE__, __LINE__);
	return (string ("127.1.0.") + domainId);
}

SI32 DcmToolKit::getVcsId ()
{

   SI32 vcsId = VcsFabricLocalObjectManager::getVcsID();

   return (vcsId);
}

ResourceId DcmToolKit::getThisVcsNodeClusterType ()
{
    return ((VcsFabricLocalObjectManager::getInstance ())->getThisVcsNodeClusterType ());
}

SI32 DcmToolKit::getMappedIdFromDomainId (const UI32 domainId)
{
        return (domainId);
    }

SI32 DcmToolKit::getDomainIdFromMappedId (const UI32 mappedId)
{
        return (mappedId);
    }


SI32 DcmToolKit::getLocalMappedId ()
{
    return (getThisVcsNodeDomainId ());
}

SI32 DcmToolKit::getVcsMode ()
{

    SI32 vcsMode = VcsFabricLocalObjectManager::isVcsEnabled();

    return (vcsMode);
}

ResourceId DcmToolKit::isVcsEnabled ()
{
    
    SI32 vcsMode = DcmToolKit::getVcsMode ();
   
    if (1 == vcsMode )
    {
        return (WAVE_PERSISTENCE_CLUSTER_ENABLED);
    }
    
    return (WAVE_PERSISTENCE_CLUSTER_DISABLED);
}

LocationId DcmToolKit::getLocationIdFromMappedId (const UI32 mappedId)
{
    LocationId                                  locationId = 0;
    VcsNodeLocalManagedObject                   *pVcsNodeLocalManagedObject = NULL;

    pVcsNodeLocalManagedObject = getLocalManagedObjectFromDomainId (mappedId);

    if (NULL != pVcsNodeLocalManagedObject)
    {
        locationId = pVcsNodeLocalManagedObject->getOwnerLocationId ();
        delete pVcsNodeLocalManagedObject;
    }

    //WaveNs::tracePrintf (TRACE_LEVEL_INFO, "DcmToolKit::getLocationIdFromMappedId: mappedId : [%d], locationId:[%d]", mappedId, locationId); 

    return locationId;
}

LocationId DcmToolKit::getLocationIdFromRbridgeId (const UI32 rbridgeId)
{
    return getLocationIdFromMappedId (rbridgeId);
}

SI32 DcmToolKit::getMappedIdFromLocationId (const LocationId locationId)
{
    SI32                                        mappedId = -1;
    VcsNodeLocalManagedObject                   *pVcsNodeLocalManagedObject = NULL;

    pVcsNodeLocalManagedObject = getLocalManagedObjectFromLocationId (locationId);

    if (NULL != pVcsNodeLocalManagedObject)
    {
        mappedId = (SI32) pVcsNodeLocalManagedObject->getMappedId ();
        delete pVcsNodeLocalManagedObject;
    }

    return mappedId;
}

SI32 DcmToolKit::getDomainIdFromLocationId (const LocationId locationId)
{
    return getMappedIdFromLocationId (locationId);
}

SI32 DcmToolKit::getDomainIdFromIpAddressAndPort (const string iPAddress, UI32 port)
{
    LocationId locationId = FrameworkToolKit::getLocationIdForIpAddressAndPort (iPAddress, port);
    return getDomainIdFromLocationId (locationId);
}

void DcmToolKit::getDomainIdsForFullyConnectedLocations ( vector<UI32> &domainIds, const bool includeThisNode )
{
    vector<LocationId> locationIds;
    domainIds.clear();    
    FrameworkToolKit::getFullyConnectedLocations( locationIds );
    
    if ( true == includeThisNode  )
    {
        locationIds.push_back( FrameworkToolKit::getThisLocationId() );
    }

    for ( UI32 i = 0; i < locationIds.size(); i++ )
    {   
        domainIds.push_back( getDomainIdFromLocationId ( locationIds[i] ) );
    }
    std::sort( domainIds.begin(), domainIds.end());
}

void DcmToolKit::getFullyConnectedLocationsSortedOnDomainId ( vector<LocationId> &locationIds, const bool includeThisNode )
{
    map<UI32, LocationId> domainIdLocationId;
    locationIds.clear();

    FrameworkToolKit::getFullyConnectedLocations( locationIds );

    if ( true == includeThisNode  )
    {
        locationIds.push_back( FrameworkToolKit::getThisLocationId() );
    }

    for ( UI32 i = 0; i < locationIds.size(); i++ )
    {   
        UI32 mappedId = getDomainIdFromLocationId ( locationIds[i] );
        domainIdLocationId[mappedId] = locationIds[i];
    }

    map<UI32, LocationId>::iterator element = domainIdLocationId.begin();
    locationIds.clear();
 
    while ( domainIdLocationId.end() != element )
    {
        locationIds.push_back ( element->second );
        element++;
    }   
}

string DcmToolKit::getFormattedSwitchIDs (const vector<UI32> &switchIds, const string &delimiter)
{
    UI32    i = 0;
    char    switchIDString[16];
    string  switchString;

    switchString.clear();
    snprintf (switchIDString, 16, "%d", switchIds[0]);
    switchString += string (switchIDString);

    for(i = 1; i < switchIds.size (); i++)
    {
        snprintf (switchIDString, 16, "%d", switchIds[i]);
        switchString += delimiter  + string (switchIDString);
    }

    return (switchString);
}

void DcmToolKit::setEnableFCPrincipal (const string fcPrincipalEnabled)
{
    if ("false" == fcPrincipalEnabled)
    {
        m_fcPrincipalEnabled = false;
    }
    else
    {
        m_fcPrincipalEnabled = true;
    }
}

bool DcmToolKit::fcPrincipalEnabled ()
{
    return (m_fcPrincipalEnabled);
}


void DcmToolKit::setIpotEnabledFlag (const string ipotEnabled)
{
	if ("false" == ipotEnabled)
	{
		m_ipotEnableFlag = false;
	}
	else
	{
		m_ipotEnableFlag = true;
	}		
}

void DcmToolKit::setFileServiceUsage(const string &fsUseEnabled)
{
	if ("false" == fsUseEnabled)
	{
        FrameworkToolKit::disableFileServiceUse();
	}
	else
	{
        FrameworkToolKit::enableFileServiceUse();
	}		
}

bool DcmToolKit::isFSUseEnabled ()
{
    return (FrameworkToolKit::isFileServiceEnabled());
}

bool DcmToolKit::isIpotEnabled ()
{
	return (m_ipotEnableFlag);
}

void DcmToolKit::insertEscapeCharInBuffer (string &inputString, char charToBeEscaped, const string &stringToEscapeWith)
{
    string::size_type pos = 0;

    while ((pos = inputString.find(charToBeEscaped, pos)) != string::npos )
    {
        inputString.insert( pos, stringToEscapeWith );
        pos+=stringToEscapeWith.length () + 1;
    }

}

UI32 DcmToolKit::getThisVcsNodeDomainId ()
{
    VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager = VcsNodeFabosInterfaceObjectManager::getInstance ();
    
    int domainId = pVcsNodeFabosInterfaceObjectManager->getDomainId ();

    if (0 > domainId)
    {
        WaveNs::tracePrintf (TRACE_LEVEL_ERROR, "DcmToolKit::getThisVcsNodeDomainId: domainId : %d", domainId);
        domainId = 0;
    }

    return (domainId);
}

ResourceId DcmToolKit::getSpecificStatusFromMappedId (const UI32 mappedId)
{
    VcsNodeLocalManagedObject                   *pVcsNodeLocalManagedObject = NULL;
    ResourceId                                  specificStatus              = 0;

    pVcsNodeLocalManagedObject = getLocalManagedObjectFromDomainId (mappedId);

    if (NULL != pVcsNodeLocalManagedObject)
    {
        specificStatus = pVcsNodeLocalManagedObject->getSpecificStatus ();
        delete pVcsNodeLocalManagedObject;
    }

    return specificStatus;
}

WorldWideName DcmToolKit::getSwitchWwnFromDomainId (const UI32 domainId)
{
    VcsNodeLocalManagedObject                   *pVcsNodeLocalManagedObject = NULL;
    WorldWideName                               switchWwn;

    pVcsNodeLocalManagedObject = getLocalManagedObjectFromDomainId (domainId);

    if (NULL != pVcsNodeLocalManagedObject)
    {
        switchWwn = pVcsNodeLocalManagedObject->getSwitchWWN ();
        delete pVcsNodeLocalManagedObject;
        return switchWwn;
    } 
    else 
    {
        wwn_t emptyWWNC;
        memset ((char *)&emptyWWNC, 0, sizeof (wwn_t));
        WorldWideName emptyWWN(string(wwnfmt(&emptyWWNC)));
        return emptyWWN;
    }
}

VcsNodeLocalManagedObject *DcmToolKit::getLocalManagedObjectFromLocationId (const LocationId locationId) 
{
    VcsNodeLocalManagedObject               *pVcsNodeLocalManagedObject = NULL;
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());

    /* Get the object corresponding to this location ID */
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (locationId, "locationId"));

    vector<WaveManagedObject *> *pResults = QueryUtils::querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            tracePrintf (TRACE_LEVEL_ERROR, "DcmToolKit::getLocalManagedObjectFromLocationId : Cannot find Vcs Node for locationId [%u]", locationId);
        }
        else if (1 == numberOfResults)
        {
            pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[0]);
        }
        else
        {
            tracePrintf (TRACE_LEVEL_FATAL, "DcmToolKit::getLocalManagedObjectFromLocationId : There can be only one local node. Number of VcsNodeLocalManagedObjects [%u], where location id [%u]", numberOfResults, locationId);
            prismAssert (false, __FILE__, __LINE__);
        }
        pResults->clear ();
        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "DcmToolKit::getLocalManagedObjectFromLocationId : System failure ");
        prismAssert (false, __FILE__, __LINE__);
    }

    return (pVcsNodeLocalManagedObject);
}

VcsNodeLocalManagedObject* DcmToolKit::getLocalManagedObjectFromDomainId (const UI32 mappedId)
{
    WaveManagedObjectSynchronousQueryContext    synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());
    VcsNodeLocalManagedObject                   *pVcsNodeLocalManagedObject = NULL;

    /* Get the object corresponding to this mapped ID */
    /* NOTE: MappedId has been removed from VcsNodeLocalManagedObject */
    /* so we will query on the domainId which is equivalent. */
    synchronousQueryContext.addAndAttribute (new AttributeUI32 (mappedId, "domainId"));

    vector<WaveManagedObject *> *pResults = QueryUtils::querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            tracePrintf (TRACE_LEVEL_ERROR, "DcmToolKit::getLocalManagedObjectFromDomainId : Cannot find Vcs Node for domainId [%u]", mappedId);
        }
        else if (1 == numberOfResults)
        {
            pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[0]);
        }
        else
        {
            tracePrintf (TRACE_LEVEL_FATAL, "DcmToolKit::getLocalManagedObjectFromDomainId : There can be only one local node. Number of VcsNodeLocalManagedObjects [%u], where domainId [%u]", numberOfResults, mappedId);
            prismAssert (false, __FILE__, __LINE__);
        }
        pResults->clear ();
        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "DcmToolKit::getLocalManagedObjectFromDomainId : System failure ");
        prismAssert (false, __FILE__, __LINE__);
    }

    return (pVcsNodeLocalManagedObject);
}

bool DcmToolKit::validateIntRange (const char* argInput, SI64 min, SI64 max, SI64 &validatedNumber)
{
        SI64    givenNumber = 0;
        char*   firstInvalidCharacterPosition = NULL;

        givenNumber = strtoll (argInput, &(firstInvalidCharacterPosition), 10);

        // Check if the string has invalid character
        if ( *firstInvalidCharacterPosition != '\0' )
        {
            return false;
        }

        // Check if input string is empty
        if ( argInput[0] == '\0' )
        {
            return false;
        }

        // Checking if input value is not within proper range.
        if( (min > givenNumber) || (max < givenNumber) )
        {
            return false;
        }

        validatedNumber = givenNumber;
        return true;
}

bool DcmToolKit::isStartupConfigFileIsLinkedToDefaultConfig ()
{
    string configFile = FrameworkToolKit::getSymbolicLinkToStartupFile ();

    if (("defaultconfig.novcs" == configFile) || ("defaultconfig.vcs" == configFile))
    {
        return (true);
    }
    else
    {
       return (false);
    }
}

SI32 DcmToolKit::isNodeWithRbridgeIdInCluster (const UI32 mappedId)
{
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;

    if (0 >= mappedId || mappedId > 239)
    {
        trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::invalid rbridge-id ") + mappedId);
        return (-1);
    }
    
    VcsNodeIsNodeInClusterMessage message(mappedId);
    status = WaveObjectManagerToolKit::sendSynchronously (&message);
    
    if (WAVE_MESSAGE_SUCCESS == status)
    {   
        return message.getIsNodeInCluster ();
    }
    
    trace (TRACE_LEVEL_WARN, string("DcmToolKit::isNodeWithRbridgeIdInCluster : Message handling failed. Status : ") + FrameworkToolKit::localize (status));
    return (-1);
}

/** 
 * Name:        getMapOfLocationIdsToRbridgeIds
 * Description: Performs a single query of all available VcsNodeLocalMOs and builds a map of LocationIds to RbridgeIds
 *              if query is successful.
 *              Note: Return status must be checked before using the map.
 *                                      
 * @return:     DCMTOOLKIT_SUCCESS, if query was successful. Otherwise an appropriate DCMTOOLKIT error status.
 */
ResourceId DcmToolKit::getMapOfLocationIdsToRbridgeIds (map<LocationId, UI32> &mapOfLocationIdsToRbridgeIds)
{
    string                                      managedObjectName           = VcsNodeLocalManagedObject::getClassName ();
    ResourceId                                  status                      = DCMTOOLKIT_ERROR;

    WaveManagedObjectSynchronousQueryContext  synchronousQueryContext (managedObjectName);
    vector<WaveManagedObject *> *pResults = QueryUtils::querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            status = DCMTOOLKIT_QUERY_NUM_RESULTS_UNEXPECTED;

            trace (TRACE_LEVEL_ERROR, "DcmToolKit::getMapOfLocationIdsToRbridgeIds : " + FrameworkToolKit::localize (status));
        }
        else
        {
            mapOfLocationIdsToRbridgeIds.clear ();

            for (UI32 i = 0; i < numberOfResults; i++)
            {
                VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[i]);

                if (NULL == pVcsNodeLocalManagedObject)
                {
                    trace (TRACE_LEVEL_FATAL, "DcmToolKit::getMapOfLocationIdsToRbridgeIds : Managed object results failed to be dynamic casted to a VcsNodeLocalManagedObject.");

                    prismAssert (false, __FILE__, __LINE__);
                }

                LocationId locationId = pVcsNodeLocalManagedObject->getLocationId ();
                UI32       rbridgeId = pVcsNodeLocalManagedObject->getDomainId ();

                mapOfLocationIdsToRbridgeIds[locationId] = rbridgeId;
            }

            status = DCMTOOLKIT_SUCCESS;
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
    }
    else
    {
        status = DCMTOOLKIT_QUERY_FAILED;
         
        trace (TRACE_LEVEL_ERROR, "DcmToolKit::getMapOfLocationIdsToRbridgeIds : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

bool DcmToolKit::getNewShowVcs ()
{
    return ( VcsFabricLocalObjectManager::getNewShowVcs () );
}

bool DcmToolKit::isVcsClusterEnabled ()
{
    bool vcsClusterEnabled = false;

    ResourceId clusterType = VcsFabricLocalObjectManager::getThisVcsNodeClusterType ();

    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == clusterType)
    {
        vcsClusterEnabled = true;
    }

    return vcsClusterEnabled;
}

vector<WyserUserInfo *> DcmToolKit::getCurrentNetconfSessions ()
{
	vector<WyserUserInfo *> temp;
    // TBDDCMOSS : Sagar - link it with Wyser
	return (temp);
}

vector<WyserUserInfo *> DcmToolKit::getCurrentCliSessions ()
{
	vector<WyserUserInfo *> temp;
    // TBDDCMOSS : Sagar - link it with Wyser
	return (temp);
}

const SI32 DcmToolKit::getManagementInterfaceReceiverPortForCcmd ()
{
    return (DcmClientToolKit::getManagementInterfaceReceiverPortForCcmd ());
}

void DcmToolKit::setManagementInterfaceReceiverPortForCcmd (const SI32 &managementInterfaceReceiverPort)
{
    DcmClientToolKit::setManagementInterfaceReceiverPortForCcmd (managementInterfaceReceiverPort);
}

const SI32 DcmToolKit::getManagementInterfaceReceiverPortForDcmd ()
{
    return (s_managementInterfaceReceiverPortForDcmd);
}

void DcmToolKit::setManagementInterfaceReceiverPortForDcmd (const SI32 &managementInterfaceReceiverPort)
{
    s_managementInterfaceReceiverPortForDcmd = managementInterfaceReceiverPort;
}

SI32 DcmToolKit::changeCcmdCfgValidity(const bool &validity, vector<string> &output, const string schemaFile)
{
    string      changeCfgValidityCommand;
    if ( validity == true )
    {
        // debug command used to set Ccmd Cfg validity to true
        changeCfgValidityCommand = "/fabos/libexec/DcmClient.Linux.powerpc -serverip 127.0.0.1 -serverport 7110 debug setPrismConfigurationValid true " + schemaFile;
    }
    else
    {   
        // debug command used to set Ccmd Cfg validity to false and remove Ccmd cfg
        changeCfgValidityCommand = "/fabos/libexec/DcmClient.Linux.powerpc -serverip 127.0.0.1 -serverport 7110 debug setPrismConfigurationValid false " + schemaFile;
    }
    
    SI32        status = FrameworkToolKit::systemCommandOutput(changeCfgValidityCommand.c_str (), output);
    return (status);
}

string DcmToolKit::getCcmdLockFileName()
{
    // the file name is used to acquire flock and remove Ccmd cfg file
    string ccmdLockFileName("/tmp/Ccmd.Linux.powerpc.cfg.tmp");
    return (ccmdLockFileName);
}

string DcmToolKit::getDcmdLockFileName()
{
    // the file name is used to acquire flock and remove Ccmd cfg file
    string dcmdLockFileName("/tmp/Dcmd.Linux.powerpc.cfg.tmp");
    return (dcmdLockFileName);
}

SI32 DcmToolKit::getDomainListInFabric (u_short *domainList, size_t recv_size)
{
    return (VcsNodeFCPrincipalTrackingWorker::fabGetCosDomainList (domainList, recv_size));
}

SI32 DcmToolKit::getClientInstances (vector<SI32> &connectedInstancesVector)
{
    int nodes[32];
    int i = 0; 
    SI32 instances = has_get_nodeset(nodes, HAS_NSET_BP_ACTIVE);

    trace (TRACE_LEVEL_DEBUG, string ("DcmToolKit::ClientInstances=") + instances);

    for (i = 0; i < instances; i++) {
        int lcType;
	int lcSlot;
	slot_no2id(get_node_slot(nodes[i]), &lcType, &lcSlot);

	connectedInstancesVector.push_back (lcSlot);
    }

    return (instances);
}

SI32 DcmToolKit::getClientInstancesAndSubInstances (vector<SI32> &connectedInstancesVector, vector<SI32> &connectedSubInstancesVector)
{
    int nodes[32];
    int i = 0;
    SI32 instances = has_get_nodeset(nodes, HAS_NSET_BP_ACTIVE);

    trace (TRACE_LEVEL_DEBUG, string ("DcmToolKit::getClientInstancesAndSubInstances:: ClientInstances=") + instances);

    for (i = 0; i < instances; i++) 
    {
        int lcType;
        int lcSlot;
        slot_no2id(get_node_slot(nodes[i]), &lcType, &lcSlot);

        connectedInstancesVector.push_back (lcSlot);
        connectedSubInstancesVector.push_back (getActivePartitionIdBySlot (lcSlot));
    }

    return (instances);
}

SI32 DcmToolKit::getThisSlotInstance ()
{
    SI32 instance = 0;
#if defined(AGENT)
    int lcType;
    slot_no2id(get_node_slot(NID_SELF()), &lcType, &instance);
#endif

    // trace (TRACE_LEVEL_DEBUG, string ("DcmToolKit::ThisSlotInstance=") + instance);

    return (instance);
}


void DcmToolKit::raslogAssert (const string &file, const string &function, const UI32 line, const string &assertString)
{
        trace (TRACE_LEVEL_ERROR, string ("raslogAssert: FILE:") + file + string (" LINE:") + line + string (" SID:") + assertString);
	raslog_ext((char *)file.data(), (char *)function.data(), line, RASLOG_OID_INVALID, RASLOG_NOFLAGS, DCM_3005, (char *)assertString.data());
}

void DcmToolKit::getChassisIpAddress (string &chassisIpAddress)
{
    int rc = 0;

	ipadm_local_addr_id_t local_addr_ids[MAX_LOCAL_ADDR_TABLE_SIZE];
	struct sockaddr_storage local_addrs[MAX_LOCAL_ADDR_TABLE_SIZE];
	ipv4_prefix_len_t local_prefixes[MAX_LOCAL_ADDR_TABLE_SIZE];
	addr_flags_t local_flags[MAX_LOCAL_ADDR_TABLE_SIZE];
	ipadm_gw_addr_id_t gw_addr_ids[MAX_GW_ADDR_TABLE_SIZE];
	struct sockaddr_storage gw_addrs[MAX_GW_ADDR_TABLE_SIZE];
	addr_flags_t gw_flags[MAX_GW_ADDR_TABLE_SIZE];
	ipadm_ip_get_set_t current;

	current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
	current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

	current.local_addr_ids = local_addr_ids;
	current.local_addrs = local_addrs;
	current.local_prefixes = local_prefixes;
	current.local_flags = local_flags;
	current.gw_addr_ids = gw_addr_ids;
	current.gw_addrs = gw_addrs;
	current.gw_flags = gw_flags;

    // Populate the ipadm_ip_get_set_t structure
	rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
	if (rc) {
		trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::getChassisIpAddress : ipAdmLocalAddrIdGet() returned ") + rc);
	}
	rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
	if (rc) {
		trace (TRACE_LEVEL_ERROR, string("DcmToolKit::getChassisIpAddress : ipAdmGwAddrIdGet() returned ") + rc);
	}
	rc = ipAdmIpAddrGet(&current);
	if (rc) {
		trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::getChassisIpAddress : ipAdmIpAddrGet() returned ") + rc);
	}

	char *addr_c;
	int idx = -1;

    for (int i = 0; i < current.num_locals; i++)
    {
        ipadm_local_addr_id_t *p = &(current.local_addr_ids[i]);
        if ((p->entity_type == SWITCH_ENTITY) &&
            (p->entity_instance == 0) &&
            (p->interface_type == ETHERNET_INTERFACE) &&
            (p->addr_family == AF_INET))
        {
            idx = i;
            break;
        }
    }

	if (idx < 0) 
    {
		trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::getChassisIpAddress : could not locate ethernet interface"));
	} 
    else 
    {
		if ((current.local_flags[idx] & IPADM_ADDR_NONE) == IPADM_ADDR_NONE) 
        {
			chassisIpAddress = "";
		}
		else 
        {
			addr_c = inet_ntoa(SOCKADDR_INP(current.local_addrs[idx])->sin_addr);
			chassisIpAddress = addr_c;
		}
	}
}

string DcmToolKit::getFirmwareVersion ()
{
	return ((VcsFabricLocalObjectManager::getInstance ())->getFwVersion ());
}

int	DcmToolKit::handleDcmFssConfigUpdate (void *dcmConfigData)
{
    return (VcsNodeFabosInterfaceObjectManager::getInstance ()->
    		handleDcmFssConfigUpdate ((dcmConfigFssSyncData_t *) dcmConfigData));
}


// First user of this API will be Zone Plugin.
// Using this API,  plugin can check if nodes in fabric match with nodes in Dcmd/Ccmd cluster.

bool DcmToolKit::doesFabricListAndClusterNodesMatch ()
{
    bool        fabricListAndClusterNodesMatch  = false;

    ResourceId  distributionMode                = DcmToolKit::getDistributionMode ();

    if ((DCM_CCM != distributionMode) && (CCM_ONLY != distributionMode))
    {
        // ERROR : not in Ccmd with FC mode, or not in Dcmd with MC/VcsDisabled mode.

        WaveNs::trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::doesFabricListAndClusterNodesMatch : distributionMode [") + FrameworkToolKit::localizeToSourceCodeEnum (distributionMode) + string ("]"));

        return fabricListAndClusterNodesMatch;
    }

    ResourceId  isVcsEnabled    = DcmToolKit::isVcsEnabled (); 

    if (WAVE_PERSISTENCE_CLUSTER_ENABLED != isVcsEnabled)
    {
        // ERROR : vcs is not enabled.

        WaveNs::trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::doesFabricListAndClusterNodesMatch : isVcsEnabled [") + FrameworkToolKit::localizeToSourceCodeEnum (isVcsEnabled) + string ("]"));

        return fabricListAndClusterNodesMatch;
    }

    u_short fabDomainList[257]              = {0};
    size_t  recv_size                       = (sizeof (u_short)) * 257;

    int     fabApiStatus                    = DcmToolKit::getDomainListInFabric (fabDomainList, recv_size);

    if (0 != fabApiStatus)
    {
        // ERROR : fab api error.

        WaveNs::trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::doesFabricListAndClusterNodesMatch : getDomainListInFabric returned [") + fabApiStatus + string ("]"));
        return fabricListAndClusterNodesMatch;
    }

    UI32    numberOfNodesInFabric   = fabDomainList[0];

    if (0 == numberOfNodesInFabric)
    {
        // ERROR : zero nodes returned by fab apis.

        WaveNs::trace (TRACE_LEVEL_ERROR, "DcmToolKit::doesFabricListAndClusterNodesMatch : getDomainListInFabric returned numberOfNodesInFabric = 0");

        return fabricListAndClusterNodesMatch;
    }


    // fab api successful -

    if (1 == numberOfNodesInFabric)
    {
        // Case 1 : single node in fab list.

        // In this case, database query for VcsNodeLocalManagedObject is not made. Only check ThisVcsNodeClusterType and ThisLocationRole.

        LocationRole    thisLocationRole        = FrameworkToolKit::getThisLocationRole ();

        if ((LOCATION_STAND_ALONE == thisLocationRole) || (LOCATION_PRIMARY == thisLocationRole))
        {
            // i.e. single node MC/FC. (This node is not waiting to rejoin, unconfirmed etc.)

            UI32 thisRbridgeId          = DcmToolKit::getThisVcsNodeDomainId ();
            UI32 domainIdFromFabricList = (UI32) fabDomainList[1];

            if (domainIdFromFabricList == thisRbridgeId)
            {
                fabricListAndClusterNodesMatch = true;

                // Need a DEVEL trace for identifing success case.

                WaveNs::trace (TRACE_LEVEL_DEVEL, string ("DcmToolKit::doesFabricListAndClusterNodesMatch : numberOfNodesInFabric [1], thisLocationRole [") + FrameworkToolKit::localizeToSourceCodeEnum (thisLocationRole) + string ("]"));
            }
            else
            {
                // ERROR : fab list has single node, but rbridge-id is different from this node rbridge-id (as known to Dcmd/Ccmd)

                WaveNs::trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::doesFabricListAndClusterNodesMatch : thisRbridgeId [") + thisRbridgeId + string ("], domainIdFromFabricList [") + domainIdFromFabricList + string ("]"));
            }
        }
        else
        {
            // Transient state ? Print thisLocationRole and thisNodeRole in trace.

            WaveNs::trace (TRACE_LEVEL_INFO, string ("DcmToolKit::doesFabricListAndClusterNodesMatch : thisLocationRole [") + FrameworkToolKit::localizeToSourceCodeEnum (thisLocationRole) + string ("]"));
        }
    }
    else
    {
        // Case 2 : multiple nodes in fab list.

        // (filter-by-connected-nodes may not be useful, since it also includes standalone nodes.)
        // Note : currently, the check for fabric principal is Cluster PRIMARY, and others are secondary is not added.
        // Only following is checked -
        //      set of rbridge-ids in fabric is same as the one in Database (VcsNodeLocalManagedObjects), 
        //      and their roles are either PRIMARY or SECONDARY.

        WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());

        // a. add OrConditionForDomainIds

        string orConditionForDomainIds = "OrConditionForDomainIds";

        synchronousQueryContext.createConditionSet (orConditionForDomainIds);

        // as numberOfNodesInFabric != 0 (it is > 1), a nonzero number of attributes will get added to the condition set.

        for (UI32 i = 1; i <= numberOfNodesInFabric; i++)
        {
            synchronousQueryContext.addAttributeToConditionSet (orConditionForDomainIds, new AttributeUI32 (fabDomainList[i], "domainId"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_OR, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        }

        // b. add OrConditionForSpecificStatus

        string orConditionForSpecificStatus = "OrConditionForSpecificStatus";

        synchronousQueryContext.createConditionSet (orConditionForSpecificStatus);

        synchronousQueryContext.addAttributeToConditionSet (orConditionForSpecificStatus, new AttributeResourceId (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_PRIMARY, "specificStatus"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_OR, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
        synchronousQueryContext.addAttributeToConditionSet (orConditionForSpecificStatus, new AttributeResourceId (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY, "specificStatus"), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_OR, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        // c. combine a. and b. with 'AND'

        synchronousQueryContext.combineConditionSets (orConditionForDomainIds + string("*") + orConditionForSpecificStatus);

        // d. now query for count.

        UI32 numberOfVcsNodeLocalManagedObjects = 0;

        ResourceId queryStatus = WaveObjectManagerToolKit::querySynchronouslyForCount (&synchronousQueryContext, numberOfVcsNodeLocalManagedObjects);

        if (WAVE_MESSAGE_SUCCESS == queryStatus)
        {
            if (numberOfNodesInFabric == numberOfVcsNodeLocalManagedObjects)
            {
                fabricListAndClusterNodesMatch = true;

                // Need a DEVEL trace for identifing success case.

                WaveNs::trace (TRACE_LEVEL_DEVEL, string ("DcmToolKit::doesFabricListAndClusterNodesMatch : numberOfNodesInFabric = numberOfVcsNodeLocalManagedObjects (Primary/Secondary) = [") + numberOfNodesInFabric + string ("]"));
            }
            else
            {
                // There is mismatch between number of nodes in fabric and number of nodes in Ccmd/Dcmd cluster.

                WaveNs::trace (TRACE_LEVEL_INFO, string ("DcmToolKit::doesFabricListAndClusterNodesMatch : numberOfNodesInFabric [") + numberOfNodesInFabric + string ("], numberOfVcsNodeLocalManagedObjects (Primary/Secondary) [") + numberOfVcsNodeLocalManagedObjects + string ("]"));
            }
        }
        else
        {
            // query failed. Maybe cluster formation is going on.

            WaveNs::trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::doesFabricListAndClusterNodesMatch : querySynchronouslyForCount failed. queryStatus [") + FrameworkToolKit::localizeToSourceCodeEnum (queryStatus) + string ("]"));
        }
    }

    return fabricListAndClusterNodesMatch;
}

bool DcmToolKit::updateFirmwareVersions ()
{
    int ret = 0;
    int log_handle = 0;
    logger_record_t record;
//    int rec_num = 2;        // second entry (from the last gives the previous firmware version details.

    log_handle = logger_init(LOG_FW_HIST, LOG_NOFLAG);
    if (log_handle < 0) {
        trace(TRACE_LEVEL_INFO, "DcmToolKit::updateFirmwareVersions: Unable to retrieve version history. rc="+log_handle);
        return false;
    }

    logger_fsync(log_handle);
    //ret = logger_get_nth_record (log_handle, rec_num, &record);
    ret = logger_get_nth_record (log_handle, 2, &record);

    if (0 != ret)
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, false, "DcmToolKit::updateFirmwareVersions: failed to get the record entry for previous firmware. ret = %d",ret);
        return (false);
    } 

    string prevFwVersion  = record.msg;
    m_oldFwVersion        = prevFwVersion;

    tracePrintf (TRACE_LEVEL_INFO, true, false, "DcmToolKit::updateFirmwareVersions: prevFwVersion : %s", prevFwVersion.c_str());

    // get current firmware version
    ret = logger_get_nth_record (log_handle, 1, &record);
    
    if (0 != ret)
    {   
        tracePrintf (TRACE_LEVEL_ERROR, true, false, "DcmToolKit::updateFirmwareVersions: failed to get the record entry for previous firmware. ret = %d",ret);
        return (false);
    }
    
    string currFwVersion = record.msg;
    m_currentFwVersion   = currFwVersion;

    tracePrintf (TRACE_LEVEL_INFO, true, false, "DcmToolKit::updateFirmwareVersions: currFwVersion : %s", currFwVersion.c_str());   
 
    logger_exit(log_handle);
    return (true);
}

string DcmToolKit::getPreviousFirmwareVersion ()
{
    return (m_oldFwVersion);
    //return ((VcsFabricLocalObjectManager::getInstance ())->getPrevFwVersion ());
}

string DcmToolKit::getCurrentFirmwareVersion ()
{
    return (m_currentFwVersion);
}

ValClientSynchronousConnection *DcmToolKit::getInterDcmCcmClient ()
{
    ValClientSynchronousConnection *pInterDcmCcmClient = NULL;

    m_interDcmCcmClientMutex.lock ();

    if (NULL == m_pInterDcmCcmClient)
    {
        ResourceId  distributionMode    = getDistributionMode ();
        string      waveClientName      = "InterDcmCcmClient";

        if (DCM_CCM == distributionMode)
        {
            m_pInterDcmCcmClient = new ValClientSynchronousConnection ();

            prismAssert (NULL != m_pInterDcmCcmClient, __FILE__, __LINE__);
        }
        else if (DCM_ONLY == distributionMode)
        {
            m_pInterDcmCcmClient = new ValClientSynchronousConnection (waveClientName, "127.0.0.1", getManagementInterfaceReceiverPortForCcmd ());

            prismAssert (NULL != m_pInterDcmCcmClient, __FILE__, __LINE__);
        }
        else if (CCM_ONLY == distributionMode)
        {
            m_pInterDcmCcmClient = new ValClientSynchronousConnection (waveClientName, "127.0.0.1", getManagementInterfaceReceiverPortForDcmd ());

            prismAssert (NULL != m_pInterDcmCcmClient, __FILE__, __LINE__);
        }
        else
        {
            tracePrintf (TRACE_LEVEL_ERROR, true, false, "DcmToolKit::getInterDcmCcmClient : ValClientSynchronousConnection is NULL.  Invalid distribution mode detected : %s", (FrameworkToolKit::localizeToSourceCodeEnum (distributionMode)).c_str ());

            m_pInterDcmCcmClient = NULL;
        }

        // Try the inter-daemon connection

        if (NULL != m_pInterDcmCcmClient)
        {
            string      waveClientExtention = "_" + getApplicationIdentifier ();
            UI32        numberOfRetries     = 15;

            tracePrintf (TRACE_LEVEL_INFO, true, false, "DcmToolKit::getInterDcmCcmClient : Attempting to connect the ValClientSynchronousConnection.");

            WaveConnectionStatus connectionStatus = m_pInterDcmCcmClient->connect (waveClientExtention, numberOfRetries);

            if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER == connectionStatus)
            {
                tracePrintf (TRACE_LEVEL_INFO, true, false, "DcmToolKit::getInterDcmCcmClient : ValClientSynchronousConnection connection status : %s", (FrameworkToolKit::localize (connectionStatus)).c_str ());

                pInterDcmCcmClient = m_pInterDcmCcmClient;
            }
            else
            {
                // Deleting the client connection may be wasteful here, but ensures both proper intialization and connection for subsequent attempts.

                tracePrintf (TRACE_LEVEL_ERROR, true, false, "DcmToolKit::getInterDcmCcmClient : ValClientSynchronousConnection connection status : %s", (FrameworkToolKit::localize (connectionStatus)).c_str ());

                delete (m_pInterDcmCcmClient);

                m_pInterDcmCcmClient = NULL;
                pInterDcmCcmClient = NULL;
            }
        }
        else
        {
            // Invalid distribution mode

            pInterDcmCcmClient = NULL;
        }
    }
    else
    {
        // Inter-deamon connection is already initialized and connected.

        pInterDcmCcmClient = m_pInterDcmCcmClient;
    }

    m_interDcmCcmClientMutex.unlock ();

    return (pInterDcmCcmClient);
}

bool DcmToolKit::isCliAllowedFromThisLocation ()
{
    return (m_isCliAllowedFromThisLocation);
}

void DcmToolKit::setIsCliAllowedFromThisLocation (bool isCliAllowedFromThisLocation)
{
    m_isCliAllowedFromThisLocation = isCliAllowedFromThisLocation;
}

bool DcmToolKit::isCliAllowedAtThisLocation ()
{
    if (CCM_ONLY == DcmToolKit::getDistributionMode ())
    {
        m_isCliAllowedAtThisLocation = true;
    }
    else
    {
        LocationRole locationRole = FrameworkToolKit::getThisLocationRole ();

        if ((LOCATION_STAND_ALONE == locationRole) || (LOCATION_PRIMARY == locationRole) || (DcmToolKit::isCliAllowedFromThisLocation ()))
        {
            m_isCliAllowedAtThisLocation = true;
        }
        else
        {
            m_isCliAllowedAtThisLocation = false;
        }
    }

    return (m_isCliAllowedAtThisLocation);
}

void DcmToolKit::createDummySlot (UI32 slotNumber)
{
    //Create a message and send the same so that we serialize the requests as well as we execute in the context of correct OM.
    VcsNodeCreateDummySlotMessage *pCreateDummySlotMessage = new VcsNodeCreateDummySlotMessage (slotNumber);

    //We are not worried about the response status but we need to be blocked until the processing is complete
    WaveObjectManagerToolKit::sendSynchronously (pCreateDummySlotMessage);

    delete (pCreateDummySlotMessage);
}

int DcmToolKit::getActivePartitionIdBySlot (const UI32 &slotId)
{
    int  partitionId = VcsNodeFabosInterfaceObjectManager::getActivePartitionIdBySlot (slotId);

    return (partitionId);
}

ResourceId DcmToolKit::getConfigurationNumber (vector<UI64> &configurationNumberVector)
{
    UI64                            configurationTimeStamp      = 0;
    UI64                            configurationTransactionId  = 0;
    DateTime                        configurationTime           ("1970-01-01 00:00:00.0+0");
    DateTime                        latestUpdateTime            ("1970-01-01 00:00:00.0+0"); // latest update time is not required for the configuration number.
    UI64                            clusterGuid                 = 0;
    ResourceId                      status                      = WAVE_MESSAGE_SUCCESS;

    // Query global ConfigurationManagedObject for the configuration transaction id and configuration time.
    
    status = PersistenceToolKit::getConfigurationManagedObjectInformation (configurationTransactionId, configurationTime, latestUpdateTime); 

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::getConfigurationNumber : Could not get configuration managed object information. Status : ") + FrameworkToolKit::localize (status));

        return (status);
    }
    
    // Query global VcsClusterManagedObject for cluster GUID.

    status = DcmToolKit::getVcsClusterGuid (clusterGuid);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::getConfigurationNumber : Could not get vcs cluster managed object information. Status : ") + FrameworkToolKit::localize (status));

        return (status);
    }

    trace (TRACE_LEVEL_DEBUG, string ("DcmToolKit::getConfigurationNumber : transaction id : ") + configurationTransactionId + string (", time stamp : ") + (configurationTime.toString ()).c_str ());

    if (0 != configurationTransactionId)
    {
        //convert TimeStamp in UI64

        UI8  month = configurationTime.getMonth ();
        UI8  day   = configurationTime.getDay ();
        SI16 year  = configurationTime.getYear ();

        UI8 hour   = configurationTime.getHour ();
        UI8 minute = configurationTime.getMin ();
        UI8 second = configurationTime.getSec ();

        configurationTimeStamp = 0;

        configurationTimeStamp = (configurationTimeStamp | month);
        configurationTimeStamp = ((configurationTimeStamp << 8) | day);
        configurationTimeStamp = ((configurationTimeStamp << 16) | year);
        configurationTimeStamp = ((configurationTimeStamp << 8) | hour);
        configurationTimeStamp = ((configurationTimeStamp << 8) | minute);
        configurationTimeStamp = ((configurationTimeStamp << 8) | second);
    }

    configurationNumberVector.push_back (configurationTimeStamp);
    configurationNumberVector.push_back (configurationTransactionId);
    configurationNumberVector.push_back (clusterGuid);
    configurationNumberVector.push_back ((UI64) 0);                             // Setting the WWN to 0 until we include WWN in configuration Managed Object.

    return (status);
}

UI64 DcmToolKit::getConfigurationTransactionId ()
{
    UI64        configurationTransactionId  = 0;
    ResourceId  status                      = WAVE_MESSAGE_SUCCESS;

    status = PersistenceToolKit::getConfigurationTransactionId (configurationTransactionId);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "DcmToolKit::getConfigurationTransactionId : Status : " + FrameworkToolKit::localize (status));
    }

    return (configurationTransactionId);
}

DateTime DcmToolKit::getConfigurationTime ()
{
    DateTime    configurationTime           ("1970-01-01 00:00:00.0+0");
    ResourceId  status                      = WAVE_MESSAGE_SUCCESS;

    status = PersistenceToolKit::getConfigurationTime (configurationTime);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "DcmToolKit::getConfigurationTime : Status : " + FrameworkToolKit::localize (status));
    }

    return (configurationTime);
}

DateTime DcmToolKit::getLatestUpdateTime ()
{
    DateTime    latestUpdateTime            ("1970-01-01 00:00:00.0+0");
    ResourceId  status                      = WAVE_MESSAGE_SUCCESS;

    status = PersistenceToolKit::getLatestUpdateTime (latestUpdateTime);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "DcmToolKit::getLatestUpdateTime : Status : " + FrameworkToolKit::localize (status));
    }
    
    return (latestUpdateTime);
}

ResourceId DcmToolKit::getVcsClusterGuid (UI64 &clusterGuid)
{
    vector<WaveManagedObject *>    *pResults            = NULL;
    LocationRole                    thisLocationRole    = FrameworkToolKit::getThisLocationRole ();
    ResourceId                      status              = WAVE_MESSAGE_SUCCESS;

    if ((LOCATION_SECONDARY_CLUSTER_PHASE_0 == thisLocationRole) || (LOCATION_SECONDARY_CLUSTER_PHASE_1 == thisLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_0 == thisLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_1 == thisLocationRole))
    {
        status = VcsFabricLocalObjectManager::getCachedVcsClusterGuid (clusterGuid);
    }
    else
    {
        // Query for the Global MO "VcsClusterManagedObject"

        pResults = WaveObjectManagerToolKit::querySynchronously (VcsClusterManagedObject::getClassName ());

        if (NULL != pResults)
        {
            prismAssert (0 == (pResults->size ()) || 1 == (pResults->size ()), __FILE__, __LINE__);

            if (1 == pResults->size ())
            {
                VcsClusterManagedObject *pVcsClusterManagedObject = dynamic_cast<VcsClusterManagedObject *> ((*pResults)[0]);
                prismAssert (NULL != pVcsClusterManagedObject, __FILE__, __LINE__);

                clusterGuid = pVcsClusterManagedObject->getClusterGUID ().getUuidHash ();

                status = WAVE_MESSAGE_SUCCESS;
            }
            else if (0 == pResults->size ())
            {
                // it is possible that during cluster creation before Phase 3 Cluster managed object is not present. This can return null.
                clusterGuid = 0;
                status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                status = VCS_FABRIC_LOCAL_VCS_CLUSTER_GUID_QUERY_FAILED;
            }

            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
        }
        else
        {
            status = VCS_FABRIC_LOCAL_VCS_CLUSTER_GUID_QUERY_FAILED;
        }
    }

    return (status);
}

void DcmToolKit::updateDcmFrameworkForISSUInProgress ()
{
 // Add code later, just providing prototype to firmware plugin team.
}

bool DcmToolKit::isIssuInProgress ()
{
    bool flag = VcsNodeFabosInterfaceObjectManager::getIssuInProgress ();

    return (flag);
}

ResourceId DcmToolKit::getOwnerWaveNodeObjectIdForDomainId (const UI32 &domainId, ObjectId &ownerWaveNodeObjectId)
{
    VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = getLocalManagedObjectFromDomainId (domainId);

    if (NULL == pVcsNodeLocalManagedObject)
    {
        ownerWaveNodeObjectId = ObjectId::NullObjectId;

        return WAVE_MESSAGE_ERROR;
    }

    ownerWaveNodeObjectId = pVcsNodeLocalManagedObject->getOwnerWaveNodeObjectId ();

    delete pVcsNodeLocalManagedObject;

    return WAVE_MESSAGE_SUCCESS;
}

int DcmToolKit::isActiveMM ()
{ 
  return ham_is_active(); 
}

bool DcmToolKit::isSwitchGvlanCapable ()
{
    bool gvlanCapable = true;

    /* Switch is GVLAN capable only if it is in MC mode */
    if ( false == DcmToolKit::isVcsClusterEnabled ())
    {
        gvlanCapable = false;
    }

    /* Switch is GVLAN capable only if SWBD model is mercury/castor*/
    UI32 swbdModel = DcmToolKit::getSwBDModel ();

    if ( !(swbdModel >=1000 && swbdModel <= 1006) && swbdModel != 131 && swbdModel != 137)
    {
        gvlanCapable = false;
    }

    return gvlanCapable;
}

bool DcmToolKit::isClusterGvlanCapable ()
{
    bool gvlanCapable = true;

    /* Switch is GVLAN capable only if it is in MC mode */
    if ( false == DcmToolKit::isVcsClusterEnabled ())
    {
        gvlanCapable = false;
        return gvlanCapable;
    }

    VcsNodeLocalManagedObject               *pVcsNodeLocalManagedObject = NULL;
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsNodeLocalManagedObject::getClassName ());

    /* Get the object corresponding to this location ID */
    synchronousQueryContext.addSelectField ("SWBDModel");
    synchronousQueryContext.addSelectField ("domainId");

    vector<WaveManagedObject *> *pResults = WaveObjectManagerToolKit::querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();
        UI32 i               = 0;

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_ERROR, "DcmToolKit::isClusterGvlanCapable : Cannot find any Vcs Nodes. There should be atleast 1 local node entry.");
            prismAssert (false, __FILE__, __LINE__);
            return false;
        }

        for (i = 0; i < numberOfResults; i++)
        {
            pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[i]);
            /* Cluster is GVLAN capable only if SWBD model of all nodes in cluster is mercury/castor*/
            SI32 swbdModel = pVcsNodeLocalManagedObject->getSWBDModel ();

            if ( !(swbdModel >=1000 && swbdModel <= 1006) && swbdModel != 131 && swbdModel != 137)
            {
                gvlanCapable = false;
                trace (TRACE_LEVEL_INFO, string ("DcmToolKit::isClusterGvlanCapable : Rbridge ") + pVcsNodeLocalManagedObject->getDomainId () + " is not gvlan capable.");
                break;
            }
        }
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "DcmToolKit::isClusterGvlanCapable: System failure ");
        prismAssert (false, __FILE__, __LINE__);
    }

    return gvlanCapable;
}

ResourceId DcmToolKit::ioctlCallToKernel (WaveConfigurationAttributes &config)
{
    trace (TRACE_LEVEL_DEBUG, string ("DcmToolKit::ioctlCallToKernel: Entering : "));
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    WaveCValue waveCValue;
    config.getAttributeValue (config.getConfigType (), config.getConfigValue (), &waveCValue);

    sysModHandle_t* pSysModHandle = NULL;
    int             rc           = sysModInit(&pSysModHandle);

    if ((0 != rc) || (NULL == pSysModHandle))
    {   
        trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::ioctlCallToKernel: sysModInit Failed: "));
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        if ("GVlan" == config.getConfigName ())
        {
            int gvlanState = wave_cvalue_get_ui32 (&waveCValue); 
            rc = sysCfgSetGvlanState (pSysModHandle, &gvlanState);
            if (0 != rc)
            {   
                trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::ioctlCallToKernel: Failed to update Gvlan State in Kernel/fabsys module: "));
                status = WAVE_MESSAGE_ERROR;
            }
            else
            {
                trace (TRACE_LEVEL_INFO, string ("DcmToolKit::ioctlCallToKernel: Successfully Updated Kernel/fabsys module gvlanState to value : ") + gvlanState);
            }
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("DcmToolKit::ioctlCallToKernel: Updating Kernel/fabsys module not required for the config : ") + config.getConfigName ());
        }
    }

    sysModClose (&pSysModHandle);
    return status;
}

ResourceId DcmToolKit::pushConfigToKernelOnLocalMM (string &globalConfigs)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (true == globalConfigs.empty ())
    {
        trace (TRACE_LEVEL_ERROR, "DcmToolKit::pushConfigToKernelOnLocalMM : Input Config string is empty");
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        vector<string> globalConfigsVector;
        stringstream ss (globalConfigs);
        string tmp;
        while(getline(ss, tmp, '\n'))
        {
            globalConfigsVector.push_back(tmp);
        }

        UI32 numberOfConfigs = globalConfigsVector.size ();
        for (UI32 i = 0; i < numberOfConfigs; i++)
        {
            string serializedConfig = globalConfigsVector[i];
            trace (TRACE_LEVEL_DEBUG, string ("DcmToolKit::pushConfigToKernelOnLocalMM : Deserializing config : ") + serializedConfig);
            WaveConfigurationAttributes config;
            config.loadWaveConfiguration (serializedConfig);
            ioctlCallToKernel (config);
        }
    }

    return status;
}

ResourceId DcmToolKit::pushConfigToKernel (string &globalConfigs)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (DcmToolKit::isActiveMM ())
    {
        trace (TRACE_LEVEL_DEBUG, string ("DcmToolKit::pushConfigToKernel: Send message to standby to update kernel/fabsys module "));
        if (WAVE_MESSAGE_SUCCESS == FrameworkToolKit::isConnectedToHaPeerLocation())
        {   
            LocationId  haPeerLocationId = FrameworkToolKit::getHaPeerLocationId ();

            if (0 == haPeerLocationId)
            {   
                trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::pushConfigToKernel : FrameworkToolKit::getHaPeerLocationId returned [0]"));
            }
            else
            {
                VcsNodeHaSyncConfigToKernelMessaage *message = new VcsNodeHaSyncConfigToKernelMessaage (globalConfigs);                
                status = WaveObjectManagerToolKit::sendSynchronously (message, haPeerLocationId);
                if (status != WAVE_MESSAGE_SUCCESS)
                {   
                    trace (TRACE_LEVEL_ERROR, string ("DcmToolKit::pushConfigToKernel : failed to set config in kernel on standby MM : ") + FrameworkToolKit::localize (status));
                    //continue even if standby sync fails(for now)
                }   
                else
                {
                    trace (TRACE_LEVEL_INFO, "DcmToolKit::pushConfigToKernel : succeeded in setting config in kernel on standby MM");
                }
            }
        }
    }
    status = pushConfigToKernelOnLocalMM (globalConfigs);

    return status;
}

}
