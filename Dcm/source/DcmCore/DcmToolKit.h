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

#ifndef DCMTOOLKIT_H
#define DCMTOOLKIT_H

#include "Framework/Types/Types.h"
#include "Framework/Types/WorldWideName.h"
#include "Framework/Types/DateTime.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Persistence/WaveConfigurationAttributes.h"
#include "VcsFabric/Local/VcsNodeLocalLinecardTypes.h"
#include <string>
#include <map>

using namespace WaveNs;
using namespace std;

extern "C" int g_thisSlotInstance;

namespace DcmNs
{
class VcsNodeLocalManagedObject;
class VcsFabricLocalObjectManager;
class WyserUserInfo;
class ValClientSynchronousConnection;

class DcmToolKit
{
    private :
        DcmToolKit ();

        static VcsNodeLocalManagedObject               *getLocalManagedObjectFromLocationId         (const LocationId locationId);
        static SI32                                     getTokensFromIpAddress                      (const string ipAddress, SI32 &ip1, SI32 &ip2, SI32 &ip3, SI32 &ip4);

    protected :
    public :
        static VcsNodeLocalManagedObject               *getLocalManagedObjectFromDomainId           (const UI32 mappedId);
        static void                                     setBackPlaneIpAddresses                     (const string &bpBAckPlaneIpAddress, const string &cpBackPlaneIpAddress);
        static bool                                     getUserForcedBackPlaneIpAddresses           ();

        static string                                   getCurrentBackPlaneIpAddress                ();
        static bool                                     isADcmidBackPlaneIpAddress                  (const string &ipAddress);
        static bool                                     isABpBackPlaneIpAddress                     (const string &ipAddress);
        static bool                                     isACpBackPlaneIpAddress                     (const string &ipAddress);
        static string                                   getCurrentCpBackPlaneIpAddress              ();
        static string                                   getCurrentBpBackPlaneIpAddress              ();
        static string                                   deriveCpBackPlaneIpAddress                  ();
        static SI32                                     getMySlotNumber                             ();

        static string                                   getWyserAlternateExternalApplicationIpAddress                           ();
        static void                                     setWyserAlternateExternalApplicationIpAddress                           (const string &wyserAlternateExternalApplicationIpAddress);
        static SI32                                     getWyserAlternateExternalApplicationPort                                ();
        static void                                     setWyserAlternateExternalApplicationPort                                (const SI32 &wyserAlternateExternalApplicationPort);

        static ObjectId                                 getThisVcsNodeObjectId                      ();
        static ObjectId                                 getVcsNodeObjectIdFromMappedId              (const UI32 &mappedId);
        static string                                   getIpAddressFromDomainId                    (const UI32 &domainId);

        static SI32                                     getVcsId                                    ();
        static SI32                                     getMappedIdFromDomainId                     (const UI32 domainId);
        static SI32                                     getDomainIdFromMappedId                     (const UI32 mappedId);
        static SI32                                     getLocalMappedId                            ();
        static SI32                                     getVcsMode                                  ();
        static ResourceId                               isVcsEnabled                                ();
        static LocationId                               getLocationIdFromMappedId                   (const UI32 mappedId);
        static LocationId                               getLocationIdFromRbridgeId                  (const UI32 rbridgeId);
        static SI32                                     getMappedIdFromLocationId                   (const UI32 locationId);
        static SI32                                     getDomainIdFromLocationId                   (const LocationId locationId);
        static SI32                                     getDomainIdFromIpAddressAndPort             (const string iPAddress, UI32 port);
        static void                                     getDomainIdsForFullyConnectedLocations      (vector<UI32> &domainIds, const bool includeThisNode = false );
        static void                                     getFullyConnectedLocationsSortedOnDomainId  (vector<LocationId> &locationIds, const bool includeThisNode = false ); 
        static ResourceId                               getThisVcsNodeClusterType                   ();
        static ResourceId                               getSpecificStatusFromMappedId               (const UI32 mappedId);
        
        static ResourceId                               getMapOfLocationIdsToRbridgeIds             (map<LocationId, UI32> &mapOfLocationIdsToRbridgeIds);
        static ResourceId                               getMapOfRbridgeIdsToLocationIds             (map<UI32, LocationId> &mapOfRbridgeIdsToLocationIds);
        
        static string                                   getVcsConfPath                              ();
        static void                                     setVcsConfPath                              (const string &vcsConfPath);
        
        static string                                   getFormattedSwitchIDs                       (const vector<UI32> &switchIds, const string &delimiter = ", ");

        static void                                     setIpotEnabledFlag                          (const string ipotEnabled);
        static bool                                     isIpotEnabled                               ();
        
        static void                                     setEnableFCPrincipal                        (const string fcPrincipalEnabled);
        static bool                                     fcPrincipalEnabled                          ();

        static void                                     setFileServiceUsage                         (const string &fsUseEnabled); 
        static bool                                     isFSUseEnabled                              ();

        static void                                     insertEscapeCharInBuffer                    (string &inputString, char charToBeEscaped, const string &stringToEscapeWith);

        static UI32                                     getThisVcsNodeDomainId                      ();
        static bool                                     validateIntRange                            (const char* argInput, SI64 min, SI64 max, SI64 &validatedNumber);

        static bool                                     isStartupConfigFileIsLinkedToDefaultConfig  ();
        static SI32                                     isNodeWithRbridgeIdInCluster                (const UI32 mappedId);

        static ResourceId                               getDistributionMode                         ();
        static void                                     setDistributionMode                         (const string &distributionMode);

        static UI32                                     getSwBDModel                                ();
        static void                                     setSwBDModel                                (const UI32 &swBDModel);          
        static UI32                                     getSwBDModelForDomainId                     (const UI32 &domainId);
        static string                                   getApplicationIdentifier                    ();

        static WorldWideName                            getSwitchWwnFromDomainId                    (const UI32 domainId);

        static bool                                     getNewShowVcs                               ();
        static bool                                     isVcsClusterEnabled                         ();
        static SI32                                     getClientInstances                          (vector<SI32> &connectedInstancesVector);
        static SI32                                     getClientInstancesAndSubInstances           (vector<SI32> &connectedInstancesVector, vector<SI32> &connectedSubInstancesVector);
        static SI32                                     getThisSlotInstance                         ();
        static void                                     setMgmtPort                                 (const UI32 &mgmtPort);
        static UI32                                     getMgmtPort                                 ();
        static void                                     raslogAssert                                (const string &file, const string &function, const UI32 line, const string &assertString);

        static SI32                                     changeCcmdCfgValidity                       (const bool &validity, vector<string> &output, const string schemaFile = "");
        static string                                   getCcmdLockFileName                         ();
        static string                                   getDcmdLockFileName                         ();
        static vector<WyserUserInfo *>                  getCurrentNetconfSessions                   ();
        static vector<WyserUserInfo *>                  getCurrentCliSessions                       ();
        static const SI32                               getManagementInterfaceReceiverPortForCcmd   ();
        static       void                               setManagementInterfaceReceiverPortForCcmd   (const SI32 &managementInterfaceReceiverPort);
        static const SI32                               getManagementInterfaceReceiverPortForDcmd   ();
        static       void                               setManagementInterfaceReceiverPortForDcmd   (const SI32 &managementInterfaceReceiverPort);


        static SI32                                     getDomainListInFabric                       (u_short *domainList, size_t recv_size);
        static void                                     getChassisIpAddress                         (string &chassisIpAddress);
        static string                                   getFirmwareVersion                          ();
        static int                                      handleDcmFssConfigUpdate                    (void *dcmConfigData);

        static bool                                     doesFabricListAndClusterNodesMatch          ();
        static bool                                     updateFirmwareVersions                      ();
        static string                                   getPreviousFirmwareVersion                  ();
        static string                                   getCurrentFirmwareVersion                   ();

        static ValClientSynchronousConnection          *getInterDcmCcmClient                        (); 
        static bool                                     isCliAllowedFromThisLocation                ();
        static void                                     setIsCliAllowedFromThisLocation             (bool isCliAllowedFromThisLocation);
        static bool                                     isCliAllowedAtThisLocation                  ();
        static void                                     createDummySlot                             (UI32 slotNumber);
        static int                                      getActivePartitionIdBySlot                  (const UI32 &slotId);
        static ResourceId                               getConfigurationNumber                      (vector<UI64> &configurationNumberVector);
        static UI64                                     getConfigurationTransactionId               ();
        static DateTime                                 getConfigurationTime                        ();
        static DateTime                                 getLatestUpdateTime                         ();
        static ResourceId                               getVcsClusterGuid                           (UI64 &clusterGuid);
        static void                                     updateDcmFrameworkForISSUInProgress         ();
        static bool                                     isIssuInProgress                            ();
        static ResourceId                               getOwnerWaveNodeObjectIdForDomainId         (const UI32 &domainId, ObjectId &ownerWaveNodeObjectId);
		static int                                      isActiveMM                                  ();
        static bool                                     isSwitchGvlanCapable                        ();
        static bool                                     isClusterGvlanCapable                       ();
        static ResourceId                               pushConfigToKernel                          (string &globalConfigs);
        static ResourceId                               pushConfigToKernelOnLocalMM                 (string &globalConfigs);
        static ResourceId                               ioctlCallToKernel                           (WaveConfigurationAttributes &config);

    // Now the data members

    private :
        static string                                   m_bpBackPlaneIpAddress;
        static string                                   m_cpBackPlaneIpAddress;
        static bool                                     m_userForcedBackPlaneIpAddresses;
        static string                                   m_wyserAlternateExternalApplicationIpAddress;
        static SI32                                     m_wyserAlternateExternalApplicationPort;
        static string                                   m_vcsConfPath;
        static bool                                     m_ipotEnableFlag;
        static bool                                     m_fcPrincipalEnabled;
        static ResourceId                               m_distributionMode;
        static string                                   m_applicationIdentifier;
        static UI32                                     m_mgmtPort;
        static UI32                                     m_swBDModel;  
        static string                                   m_currentFwVersion;
        static string                                   m_oldFwVersion;
        static ValClientSynchronousConnection          *m_pInterDcmCcmClient;
        static PrismMutex                               m_interDcmCcmClientMutex;
        static bool                                     m_isCliAllowedFromThisLocation;
        static bool                                     m_isCliAllowedAtThisLocation;

    protected :
    public :
};

}

#endif // DCMTOOLKIT_H
