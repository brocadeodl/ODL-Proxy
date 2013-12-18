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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef FRAMEWORKOBJECTMANAGERMESSAGES_H
#define FRAMEWORKOBJECTMANAGERMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkConfiguration.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

namespace WaveNs
{

typedef enum
{
    VALIDATION_DETAILS_BUFFER_TAG = 1000,
    VALIDATION_RESULTS_BUFFER_TAG = 1001
} BufferTagsForFrameworkClusterMessages;

class PrismCreateClusterWithNodesMessage : public PrismMessage
{
    private :
        bool isNewNodeIpAddressExists (const string &newNodeIpAddress, const SI32 &newNodePort);

    protected :
        PrismCreateClusterWithNodesMessage (PrismServiceId serviceCode, UI32 operationCode);

    public :
                            PrismCreateClusterWithNodesMessage ();
        virtual            ~PrismCreateClusterWithNodesMessage ();
                void        addNewNodeIpAddressAndPort         (const string &newNodeIpAddress, const SI32 &newNodePort);
                UI32        getNumberOfNewNodes                ();
                string      getNodeAt                          (UI32 i);
                SI32        getNodePortAt                      (UI32 i);
                void        setNewNodeStatus                   (const string &newNodeIpAddress, const SI32 &newNodePort, ResourceId status);
                void        updateNewNodeStatus                (const string &newNodeIpAddress, const SI32 &newNodePort, ResourceId status);
                ResourceId  getNewNodeStatus                   (const string &newNodeIpAddress, const SI32 &newNodePort);
                bool        isNewNodeStatusSet                 (const string &newNodeIpAddress, const SI32 &newNodePort);
                void        setValidationDetails               (const UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToMessage = false);
                void        getValidationDetails               (UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToUser = false);
                void        setValidationResults               (const string &newNodeIpAddress, const SI32 &newNodePort, const UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToMessage = false);
                void        getValidationResults               (const string &newNodeIpAddress, const SI32 &newNodePort, UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToUser = false);
                void        setIsAddNodeForSpecialCaseFlag     (const bool &isAddNodeForSpecialCase);
                bool        getIsAddNodeForSpecialCaseFlag     () const;

                void        getFilenamesToSync                 ( vector<string> &filenamesToSync ) ;
                void        setFilenamesToSync                 ( const vector<string> &filenamesToSync );

    // Now the data members

    private :
        vector<string>          m_newNodesIpAddresses;
        vector<SI32>            m_newNodesPorts;
        map<string, ResourceId> m_newNodesStatus;
        map<string, UI32>       m_newNodeValidationResults;
        UI32                    m_currentValidtionResultsBufferTag;
        bool                    m_isAddNodeForSpecialCase;
        vector <string>         m_filenamesToSync;

    protected :
    public :
};

class PrismConfigureClusterSecondaryMessage : public PrismMessage
{
    private :
    protected :
                     PrismConfigureClusterSecondaryMessage (PrismServiceId serviceCode, UI32 operationCode);
                     PrismConfigureClusterSecondaryMessage (PrismServiceId serviceCode, UI32 operationCode, const string &clusterPrimaryPrismVersion, const string &clusterPrimaryIpAddress, const SI32 &clusterPrimaryPort, const LocationId &clusterPrimaryLocationId, const string &ipAddress, const SI32 &port, const LocationId &locationId);
        virtual void setupAttributesForSerialization       ();

    public :
                            PrismConfigureClusterSecondaryMessage ();
                            PrismConfigureClusterSecondaryMessage (const string &clusterPrimaryPrismVersion, const string &clusterPrimaryIpAddress, const SI32 &clusterPrimaryPort, const LocationId &clusterPrimaryLocationId, const string &ipAddress, const SI32 &port, const LocationId &locationId);
        virtual            ~PrismConfigureClusterSecondaryMessage ();
                string      getClusterPrimaryPrismVersion         ();
                string      getClusterPrimaryIpAddress            ();
                SI32        getClusterPrimaryPort                 ();
                LocationId  getClusterPrimaryLocationId           ();
                string      getIpAddress                          ();
                SI32        getPort                               ();
                LocationId  getLocationId                         ();
                void        setValidationDetails                  (const UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToMessage = false);
                void        getValidationDetails                  (UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToUser = false);
                void        setValidationResults                  (const UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToMessage = false);
                void        getValidationResults                  (UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToUser = false);
                void        setIsAddNodeForSpecialCaseFlag        (const bool &isAddNodeForSpecialCase);
                bool        getIsAddNodeForSpecialCaseFlag        () const;
                void        getSchemaChangeVectors                (vector<string>   & managedObjectNamesForSchemaChange,vector<string>   & fieldNamesStringsForSchemaChange,vector<string> & fieldNamesTypesForSchemaChange,vector<UI32>   & classIds,vector<UI32> & m_parentTableIds);
                void        setSchemaChangeVectors                ();

    // Now the data members

    private :
        string     m_clusterPrimaryPrismVersion;
        string     m_clusterPrimaryIpAddress;
        SI32       m_clusterPrimaryPort;
        LocationId m_clusterPrimaryLocationId;

        string     m_ipAddress;
        SI32       m_port;
        LocationId m_locationId;
        bool       m_isAddNodeForSpecialCase;

        vector<string>              m_managedObjectNamesForSchemaChange;
        vector<string>              m_fieldNamesStringsForSchemaChange;
        vector<string>              m_fieldNamesTypesForSchemaChange;
        vector<UI32>                m_classIds;
        vector<UI32>                m_parentTableIds;

    protected :
    public :
};

class PrismConfigureClusterSecondaryHaPeerMessage : public PrismConfigureClusterSecondaryMessage
{
    private :
    protected :
     virtual void setupAttributesForSerialization ();

    public :
                  PrismConfigureClusterSecondaryHaPeerMessage ();
     virtual     ~PrismConfigureClusterSecondaryHaPeerMessage ();

    // Now the data members

    private :
    protected :
    public :
};

class PrismConfigureClusterSecondaryPhase1Message : public PrismMessage
{
    private :
    protected :
                  PrismConfigureClusterSecondaryPhase1Message (PrismServiceId serviceCode, UI32 operationCode);
     virtual void setupAttributesForSerialization ();

    public :
                  PrismConfigureClusterSecondaryPhase1Message ();
     virtual     ~PrismConfigureClusterSecondaryPhase1Message ();

     static UI32  getDatabaseBackupBufferTag                  ();

            void  getFilenameToSync                           ( string &filenameToSync );
            void  setFilenameToSync                           ( const string &filenameToSync );    
    // Now the data members

    private :
            string              m_filenameToSync;
    protected :
    public :
};

class PrismConfigureClusterSecondaryHaPeerPhase1Message : public PrismConfigureClusterSecondaryPhase1Message
{
    private :
    protected :
     virtual void setupAttributesForSerialization                   ();

    public :
                  PrismConfigureClusterSecondaryHaPeerPhase1Message ();
     virtual     ~PrismConfigureClusterSecondaryHaPeerPhase1Message ();

    // Now the data members

    private :
    protected :
    public :
};

class PrismConfigureClusterSecondaryPhase2Message : public PrismMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                 PrismConfigureClusterSecondaryPhase2Message ();
        virtual ~PrismConfigureClusterSecondaryPhase2Message ();

    // Now the data members

    private :
    protected :
    public :
};

class PrismConfigureClusterSecondaryPhase3Message : public PrismMessage
{
    private :
    protected :
                  PrismConfigureClusterSecondaryPhase3Message (PrismServiceId serviceCode, UI32 operationCode);
     virtual void setupAttributesForSerialization ();

    public :
                  PrismConfigureClusterSecondaryPhase3Message ();
        virtual  ~PrismConfigureClusterSecondaryPhase3Message ();

    // Now the data members

    private :
    protected :
    public :
};

class PrismConfigureClusterSecondaryHaPeerPhase3Message : public PrismConfigureClusterSecondaryPhase3Message
{
    private :
    protected :
     virtual void setupAttributesForSerialization  ();

    public :
                  PrismConfigureClusterSecondaryHaPeerPhase3Message ();
     virtual     ~PrismConfigureClusterSecondaryHaPeerPhase3Message ();

    // Now the data members

    private :
    protected :
    public :
};

class PrismRejoinClusterSecondaryPhase1Message : public PrismConfigureClusterSecondaryPhase1Message
{
    private :
    protected :
                  PrismRejoinClusterSecondaryPhase1Message (PrismServiceId serviceCode, UI32 operationCode);
     virtual void setupAttributesForSerialization          ();

    public :
                  PrismRejoinClusterSecondaryPhase1Message ();
     virtual     ~PrismRejoinClusterSecondaryPhase1Message ();

    // Now the data members

    private :
    protected :
    public :
};

class PrismRejoinClusterSecondaryHaPeerPhase1Message : public PrismRejoinClusterSecondaryPhase1Message
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                     PrismRejoinClusterSecondaryHaPeerPhase1Message ();
        virtual     ~PrismRejoinClusterSecondaryHaPeerPhase1Message ();

    // Now the data members

    private :
    protected :
    public :
};

//class: PrismRejoinClusterSecondaryPhase2Message
//Description: This class is used to send a message to
//the secondaries to start phase 2 processing
class PrismRejoinClusterSecondaryPhase2Message : public PrismMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                 PrismRejoinClusterSecondaryPhase2Message ();
        virtual ~PrismRejoinClusterSecondaryPhase2Message ();

        bool    getIsReplaceRejoin ();
        void    setIsReplaceRejoin (const bool &isReplace);
        LocationId  getReplacedNodeLocationId ();
        void        setReplacedNodeLocationId (const LocationId replacedNodeLocationId);

    // Now the data members

    private :
        bool        m_isReplaceRejoin;
        LocationId  m_replacedNodeLocationId;
    protected :
    public :
};

//class: PrismRejoinClusterSecondaryPhase3Message
//Description: This class is used to send a message to
//the secondaries to start phase 3 processing
class PrismRejoinClusterSecondaryPhase3Message : public PrismMessage
{
    private :
    protected :
                  PrismRejoinClusterSecondaryPhase3Message (PrismServiceId serviceCode, UI32 operationCode);
     virtual void setupAttributesForSerialization ();

    public :
                  PrismRejoinClusterSecondaryPhase3Message ();
     virtual     ~PrismRejoinClusterSecondaryPhase3Message ();

    // Now the data members

    private :
    protected :
    public :
};

class PrismRejoinClusterSecondaryHaPeerPhase3Message : public PrismRejoinClusterSecondaryPhase3Message
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                 PrismRejoinClusterSecondaryHaPeerPhase3Message ();
        virtual ~PrismRejoinClusterSecondaryHaPeerPhase3Message ();

    // Now the data members

    private :
    protected :
    public :
};

class FrameworkObjectManagerUpdateListOfSecondariesMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                            FrameworkObjectManagerUpdateListOfSecondariesMessage ();
        virtual            ~FrameworkObjectManagerUpdateListOfSecondariesMessage ();

                void        addSecondaryNodeDetails                              (const LocationId &locationId, const string &ipAddress, const UI32 &port,bool isNodeNew);
                UI32        getNumberOfSecondaryLocations                        () const;
                LocationId  getLocationIdAtIndex                                 (const UI32 &index) const;
                string      getIpAddressAtIndex                                  (const UI32 &index) const;
                UI32        getPortAtIndex                                       (const UI32 &index) const;
                bool        isNodeNewAtIndex                                     (const UI32 &index)  const;
                bool        isDeletion                                           ()   const;
                void        setIsDeletion                                        (bool deletion);
                LocationId  getLastUsedLocationId                                () const;
                void        setLastUsedLocationId                                (const LocationId &locationId);
                void        getConnectedLocationIds                              (vector<LocationId> &locationIds);
                void        setConnectedLocationIds                              (vector<LocationId> &locationIds);

    // Now the Data Members

    private :
        vector<LocationId> m_locationIdsForSecondaryNodes;
        vector<string>     m_IpAddressesForSecondaryNodes;
        vector<UI32>       m_portsForSecondaryNodes;
        vector<bool>       m_isNodeNewVector;
        bool               m_isDeletion;
        LocationId         m_lastUsedLocationId;
        vector<LocationId> m_connectedLocationIds;

    protected :
    public :
};

class FrameworkObjectManagerAddNodesToClusterMessage : public PrismCreateClusterWithNodesMessage
{
    private :
    protected :
    public :
                 FrameworkObjectManagerAddNodesToClusterMessage ();
        virtual ~FrameworkObjectManagerAddNodesToClusterMessage ();

    // Now the data members

    private :
    protected :
    public :
};

class FrameworkObjectManagerDeleteNodesFromClusterMessage : public PrismCreateClusterWithNodesMessage
{
    private :
    protected :
        FrameworkObjectManagerDeleteNodesFromClusterMessage (PrismServiceId serviceCode, UI32 operationCode);

    public :
                            FrameworkObjectManagerDeleteNodesFromClusterMessage ();
        virtual            ~FrameworkObjectManagerDeleteNodesFromClusterMessage ();
                void        addNodeIpAddressAndPort                             (const string &nodeIpAddress, const SI32 &nodePort);
                UI32        getNumberOfNodes                                    ();
                string      getNodeAt                                           (UI32 i);
                SI32        getNodePortAt                                       (UI32 i);
                void        setNodeStatus                                       (const string &nodeIpAddress, const SI32 &nodePort, ResourceId status);
                ResourceId  getNodeStatus                                       (const string &nodeIpAddress, const SI32 &nodePort);
                bool        isNodeStatusSet                                     (const string &nodeIpAddress, const SI32 &nodePort);
                bool        getIsDisconnected                                   ();
                void        setIsDisconnected                                   (bool isDisconnected);

    private :
        bool               m_isDisconnected;

    protected :
    public :
};

class FrameworkObjectManagerDestroyClusterMessage : public PrismMessage
{
    private :
    protected :
    public :
                                                      FrameworkObjectManagerDestroyClusterMessage ();
        virtual                                      ~FrameworkObjectManagerDestroyClusterMessage ();
                FrameworkObjectManagerFailoverReason  getReasonForDestroyingCluster               () const;
                void                                  setReasonForDestroyingCluster               (const FrameworkObjectManagerFailoverReason &reasonForDestroyingCluster = FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED);
                void                                  setOriginalRequester                        (const PrismServiceId &prismServiceId);
                PrismServiceId                        getOriginalRequester                        () const;
                void                                  setIsRebootRequired                         (const bool &isRebootRequired);
                bool                                  getIsRebootRequired                         () const;

    // Now the data members

    private :
        FrameworkObjectManagerFailoverReason m_reasonForDestroyingCluster;
        PrismServiceId                       m_originalRequester;
        bool                                 m_isRebootRequired;
    protected :
    public :
};

class FrameworkObjectManagerUnconfigureClusterSecondaryMessage : public PrismMessage
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                        FrameworkObjectManagerUnconfigureClusterSecondaryMessage (const string &clusterSecondaryIpAddress, const SI32 &clusterSecondaryPort);
                        FrameworkObjectManagerUnconfigureClusterSecondaryMessage ();
        virtual        ~FrameworkObjectManagerUnconfigureClusterSecondaryMessage ();
                string  getClusterSecondaryIpAddress                             ();
                SI32    getClusterSecondaryPort                                  ();

                bool    getIsPerformWarmRecovery                                 ();
                void    setIsPerformWarmRecovery                                 (const bool &isPerformWarmRecovery);

    // Now the data members

    private :
        string m_clusterSecondaryIpAddress;
        SI32   m_clusterSecondaryPort;
        bool   m_isPerformWarmRecovery;

    protected :
    public :
};

class FrameworkObjectManagerRejoinNodesToClusterMessage : public FrameworkObjectManagerDeleteNodesFromClusterMessage
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                 FrameworkObjectManagerRejoinNodesToClusterMessage ();
        virtual ~FrameworkObjectManagerRejoinNodesToClusterMessage ();

        bool    getIsReplaceRejoin ();
        void    setIsReplaceRejoin (const bool &isReplace);
        void                setNodeLocationId                        (const LocationId &locationId);
        LocationId          getNodeLocationId                        (const UI32 index) const;

    // Now the data members

    private :
        bool    m_isReplaceRejoin;
        vector <LocationId>    m_locationIds;
    protected :
    public :
};

class FrameworkObjectManagerRejoinClusterSecondaryMessage : public PrismConfigureClusterSecondaryMessage
{
    private :
    protected :
        virtual void    setupAttributesForSerialization ();

    public :
                        FrameworkObjectManagerRejoinClusterSecondaryMessage     ();
                        FrameworkObjectManagerRejoinClusterSecondaryMessage     (PrismServiceId serviceCode, UI32 operationCode);
                        FrameworkObjectManagerRejoinClusterSecondaryMessage     (const string &clusterPrimaryPrismVersion, const string &clusterPrimaryIpAddress, const SI32 &clusterPrimaryPort, const LocationId &clusterPrimaryLocationId, const string &ipAddress, const SI32 &port, const LocationId &locationId);
        virtual        ~FrameworkObjectManagerRejoinClusterSecondaryMessage    ();

        bool            getIsReplaceRejoin                                      ();
        void            setIsReplaceRejoin                                      (const bool &isReplace);

        void            setConfigurationNumber                                  (const vector<UI64> configurationNumber);
        vector<UI64>    getConfigurationNumber                                  ();
 
    // Now the data members

    private :
        bool            m_isReplaceRejoin;
        vector<UI64>    m_configurationNumber;

    protected :
    public :
};

class FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage : public FrameworkObjectManagerRejoinClusterSecondaryMessage
{
    private :
    protected :
            virtual void setupAttributesForSerialization ();

    public :
                  FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage ();
     virtual     ~FrameworkObjectManagerRejoinClusterSecondaryHaPeerMessage ();

    // Now the data members

    private :
    protected :
    public :
};

class FrameworkObjectManagerDetachFromClusterMessage : public PrismMessage
{
    private :
    protected :
    public :
                                                      FrameworkObjectManagerDetachFromClusterMessage ();
        virtual                                      ~FrameworkObjectManagerDetachFromClusterMessage ();

                FrameworkObjectManagerFailoverReason  getReasonForDetachingFromCluster               () const;
                void                                  setReasonForDetachingFromCluster               (const FrameworkObjectManagerFailoverReason &reasonForDetachingFromCluster = FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED);

    // Now the data members

    private :
        FrameworkObjectManagerFailoverReason m_reasonForDetachingFromCluster;

    protected :
    public :
};

class FrameworkObjectManagerLostHeartBeatMessage : public FrameworkObjectManagerDeleteNodesFromClusterMessage
{
    private :
    protected :
    public :
                 FrameworkObjectManagerLostHeartBeatMessage ();
        virtual ~FrameworkObjectManagerLostHeartBeatMessage ();

    // Now the data members

    private :
    protected :
    public :
};


class FrameworkObjectManagerPrimaryChangedMessage : public PrismMessage
{
    private :
    protected :
        void setupAttributesForSerialization ();

    public :
                            FrameworkObjectManagerPrimaryChangedMessage ();
                            FrameworkObjectManagerPrimaryChangedMessage (PrismServiceId serviceCode, UI32 operationCode);
                            FrameworkObjectManagerPrimaryChangedMessage (const LocationId &newPrimaryLocationId, const string &newPrimaryIpAddress, const UI32 &newPrimaryPort);
        virtual            ~FrameworkObjectManagerPrimaryChangedMessage ();

                LocationId  getNewPrimaryLocationId                     () const;
                void        setNewPrimaryLocationId                     (const LocationId &newPrimaryLocationId);
                string      getNewPrimaryIpAddress                      () const;
                void        setNewPrimaryIpAddress                      (const string &newPrimaryIpAddress);
                UI32        getNewPrimaryPort                           () const;
                void        setNewPrimaryPort                           (const UI32 &newPrimaryPort);
                void        setPrimaryChangeDueToControlledFailover     ();
                bool        isPrimaryChangeDueToControlledFailover      () const;
                void        getSchemaChangeVectors                      (vector<string>   & managedObjectNamesForSchemaChange,vector<string>   & fieldNamesStringsForSchemaChange,vector<string> & fieldNamesTypesForSchemaChange,vector<UI32>   & classIds,vector<UI32> & m_parentTableIds);
                void        setSchemaChangeVectors                      ();
                void        setAnyConfigurationChangeTrackingNumber     (const UI64 &trackingNumber);
                UI64        getAnyConfigurationChangeTrackingNumber     () const;


    // Now the data members

    private :
        LocationId                  m_newPrimaryLocationId;
        string                      m_newPrimaryIpAddress;
        UI32                        m_newPrimaryPort;
        bool                        m_isPrimaryChangeDueToControlledFailover;

        vector<string>              m_managedObjectNamesForSchemaChange;
        vector<string>              m_fieldNamesStringsForSchemaChange;
        vector<string>              m_fieldNamesTypesForSchemaChange;
        vector<UI32>                m_classIds;
        vector<UI32>                m_parentTableIds;
        UI64                        m_anyConfiguraitonChangeTrackingNumber;

    protected :
    public :
};

class FrameworkObjectManagerPrimaryChangedHaPeerMessage : public FrameworkObjectManagerPrimaryChangedMessage
{
    private :
    protected :
        virtual void setupAttributesForSerialization                   ();

    public :
                     FrameworkObjectManagerPrimaryChangedHaPeerMessage ();
        virtual     ~FrameworkObjectManagerPrimaryChangedHaPeerMessage ();

                void setDBEmptyRequired         (const bool &isDBEmptyRequired);
                bool getIsDBEmptyRequired       () const;

    // Now the data members

    private :
                bool m_isDBEmptyRequired;
    protected :
    public :
};

class FrameworkObjectManagerPrimaryChangedPhase1Message : public PrismMessage
{
    private :
    protected :
                 FrameworkObjectManagerPrimaryChangedPhase1Message (PrismServiceId serviceCode, UI32 operationCode);
         void    setupAttributesForSerialization ();

    public :
                      FrameworkObjectManagerPrimaryChangedPhase1Message ();
        virtual      ~FrameworkObjectManagerPrimaryChangedPhase1Message ();
        static  UI32  getDatabaseBackupBufferTag                        ();
                void  setAnyConfigurationChangeTrackingNumber           (const UI64 &trackingNumber);
                UI64  getAnyConfigurationChangeTrackingNumber           () const;


    // Now the data members

    private :
                UI64  m_anyConfiguraitonChangeTrackingNumber;
    protected :
    public :
};

class FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message : public FrameworkObjectManagerPrimaryChangedPhase1Message
{
    private :
    protected :
        virtual void setupAttributesForSerialization                      ();

    public :
                     FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message ();
     virtual        ~FrameworkObjectManagerPrimaryChangedHaPeerPhase1Message ();

                void setDBRestoreRequired         (const bool &isDBRestoreRequired);
                bool getIsDBRestoreRequired       () const;

    // Now the data members

    private :
                 bool m_isDBRestoreRequired;
    protected :
    public :
};
class FrameworkObjectManagerPrimaryChangedPhase2Message : public PrismMessage
{
    private :
    protected :
    public :
                 FrameworkObjectManagerPrimaryChangedPhase2Message ();
        virtual ~FrameworkObjectManagerPrimaryChangedPhase2Message ();

    // Now the data members

    private :
    protected :
    public :
};

class FrameworkObjectManagerPrimaryChangedPhase3Message : public PrismMessage
{
    private :
    protected :
                 FrameworkObjectManagerPrimaryChangedPhase3Message (PrismServiceId serviceCode, UI32 operationCode);
            void setupAttributesForSerialization ();

    public :
                 FrameworkObjectManagerPrimaryChangedPhase3Message ();
        virtual ~FrameworkObjectManagerPrimaryChangedPhase3Message ();

    // Now the data members

    private :
    protected :
    public :
};

class FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message : public FrameworkObjectManagerPrimaryChangedPhase3Message
{
    private :
    protected :
            virtual void setupAttributesForSerialization ();

    public :
                  FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message ();
     virtual     ~FrameworkObjectManagerPrimaryChangedHaPeerPhase3Message ();

    // Now the data members

    private :
    protected :
    public :
};

class FrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage :  public PrismMessage
{
    private :
    protected :
            virtual void setupAttributesForSerialization ();

    public :
                      FrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage ();
        virtual      ~FrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage ();

    // Now the data members
    private :
    protected :
    public :
};

class FrameworkObjectManagerRemoveKnownLocationsMessage : public PrismMessage
{
    private :
        void                setupAttributesForSerialization ();
    protected :
    public :
                            FrameworkObjectManagerRemoveKnownLocationsMessage ();
        virtual            ~FrameworkObjectManagerRemoveKnownLocationsMessage ();

        void                setFailedLocations                                (const vector<LocationId> locations);
        vector<LocationId>  getFailedLocationIds                        () const;
    // Now the data members

    private :
        vector<LocationId>  m_failedLocationIds;
    protected :
    public :
};

class FrameworkObjectManagerServiceControlListMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
              FrameworkObjectManagerServiceControlListMessage ();
             ~FrameworkObjectManagerServiceControlListMessage ();

        UI32  getNumberOfServices                             () const;
        void  addAService                                     (const PrismServiceId &prismServiceId, const string &serviceName, const bool &isEnabled, const bool &isLocal, const string &cpuAffinity);
        void  getServiceAtIndex                               (const UI32 &index, PrismServiceId &prismServiceId, string &serviceName, bool &isEnabled, bool &isLocal, string &cpuAffinity);

    // Now the data members

    private :
        vector<PrismServiceId> m_prismServiceIds;
        vector<string>         m_serviceNames;
        vector<bool>           m_enabled;
        vector<bool>           m_local;
        vector<string>         m_cpuAffinity;

    protected :
    public :
};

class FrameworkObjectManagerServiceSetCpuAffinityMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    FrameworkObjectManagerServiceSetCpuAffinityMessage ();
                    FrameworkObjectManagerServiceSetCpuAffinityMessage (const PrismServiceId &prismServiceId, const vector<UI32> &cpuAffinityVector);
                   ~FrameworkObjectManagerServiceSetCpuAffinityMessage ();

    PrismServiceId  getPrismServiceId                                  ();
    vector<UI32>    getCpuAffinityVector                               () const;

    // Now the data members

    private :
        PrismServiceId m_prismServiceId;
        vector<UI32>   m_cpuAffinityVector;

    protected :
    public :
};

class FrameworkObjectManagerStartExternalStateSynchronizationMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();
    protected :
    public :
        FrameworkObjectManagerStartExternalStateSynchronizationMessage (const UI32 fssStageNumber, const ResourceId m_serviceType);

        UI32        getFssStageNumber() const;
        ResourceId  getServiceType() const;

    // Now the data members
    private :
        UI32        m_fssStageNumber;
        ResourceId  m_serviceType;
    protected :
    public :
};

class FrameworkObjectManagerStartSlotFailoverMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();
    protected :
    public :
        FrameworkObjectManagerStartSlotFailoverMessage  (const UI32 slotNumber);

        UI32        getSlotNumber () const;

    // Now the data members
    private :
        UI32        m_slotNumber;
    protected :
    public :
};

class FrameworkObjectManagerNewPrincipalEstablishedMessage : public PrismMessage
{
 public:

    FrameworkObjectManagerNewPrincipalEstablishedMessage (const string & failedPrincipalIpAddress,UI32 failedPrincipalPort,LocationId failedPrincipalLocationId);
    virtual ~FrameworkObjectManagerNewPrincipalEstablishedMessage();

    //Accessors
    void setFailedPrincipalIpAddress(const string & failedPrincipalIpAddress);
    void setFailedPrinciapalPort(UI32 failedPrincipalPort);
    void setFailedPrincipalLocationId(LocationId locationId);
    void setTriggerControlledFaileover ();
    //Mutators
    string const & getFailedPrincipalIpAddress() const;
    UI32           getFailedPrincipalPort()  const;
    LocationId     getFailedPrincipalLocationId() const;
    bool           getTriggerControlledFailover () const;
 protected:
 private:



 public:
 protected:
 private:
     string mFailedPrincipalIpAddress;
     UI32   mFailedPrincipalPort;
     LocationId mFailedPrincipalLocationId;
     bool   mTriggerControlledFailover;
};

class FrameworkObjectManagerSecondaryNodeFailureNotificationMessage : public PrismMessage
{
 public:

                                   FrameworkObjectManagerSecondaryNodeFailureNotificationMessage ();
                                   FrameworkObjectManagerSecondaryNodeFailureNotificationMessage (const string & failedIpAddress,UI32 failedPort,LocationId failedLocationId);
        virtual                   ~FrameworkObjectManagerSecondaryNodeFailureNotificationMessage();

    //Mutators
        void                       addFailedIpAddress(const string & failedlIpAddress);
        void                       addFailedPort(UI32 failedPort);
        void                       addFailedLocationId(LocationId locationId);
        void                       setIsControlledFailoverInSpecialCase (bool const &isControlledFailoverInSpecialCase);

    //Accessors
        vector<string>     const & getFailedIpAddresses() const;
        vector<UI32>       const & getFailedPorts()  const;
        vector<LocationId> const & getFailedLocationIds() const;
        bool                       getIsControlledFailoverInSpecialCase () const;

 protected:
 private:



 public:
 protected:
 private:
        vector<string>     mFailedIpAddresses;
        vector<UI32>       mFailedPorts;
        vector<LocationId> mFailedLocationIds;
        bool               m_isControlledFailoverInSpecialCase;

};

class FrameworkObjectManagerDisconnectFromAllNodesMessage : public PrismMessage
{
 public:

    FrameworkObjectManagerDisconnectFromAllNodesMessage ();
    virtual ~FrameworkObjectManagerDisconnectFromAllNodesMessage();

 protected:
 private:


 public:
 protected:
 private:

};

class FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage: public PrismMessage
{

public:
        FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage ();
        virtual ~FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage ();
    protected:
    private:


    public:
    protected:
    private:

};

class FrameworkObjectManagerResetNodeToUnconfirmRole : public PrismMessage
{
    public:
                     FrameworkObjectManagerResetNodeToUnconfirmRole ();
            virtual ~FrameworkObjectManagerResetNodeToUnconfirmRole ();

    protected:
    private:


    public:
    protected:
    private:
};

class FrameworkObjectManagerStartHaSyncMessage : public PrismMessage
{
 public:

    FrameworkObjectManagerStartHaSyncMessage (const string &peerHaIpAddress, const SI32 &peerHaIpPort);
    virtual ~FrameworkObjectManagerStartHaSyncMessage();

    void   setPeerHaIpAddress(const string& peerHaIpAddress);
    void   setPeerHaIpPort(const SI32& peerHaIpPort);
    void   setFirmwareVersion (const string &firmwareVersion);
    void   setHAVersion (const UI32 &haVersion);
    void   setReturnData (const string &returnData);

    string getPeerHaIpAddress() const;
    SI32   getPeerHaIpPort() const;
    string getFirmwareVersion () const;
    UI32   getHAVersion () const;
    string getReturnData () const;

 protected:
 private:
    virtual void setupAttributesForSerialization                        ();

 public:
 protected:
 private:
    string m_peerHaIpAddress;
    SI32 m_peerHaIpPort;
    string m_firmwareVersion;
    UI32 m_haVersion;
    string m_returnData;
};

class FrameworkObjectManagerInitHaIpAddressMessage : public PrismMessage
{
 public:

    FrameworkObjectManagerInitHaIpAddressMessage (const string &localHaIpAddress, const SI32 &LocalHaIpPort);
    virtual ~FrameworkObjectManagerInitHaIpAddressMessage();

    void setLocalHaIpAddress(const string& localHaIpAddress);
    void setLocalHaIpPort(const SI32& LocalHaIpPort);

    string getLocalHaIpAddress() const;
    SI32 getLocalHaIpPort() const;
 protected:
 private:
    virtual void setupAttributesForSerialization       ();

 public:
 protected:
 private:
    string m_localHaIpAddress;
    SI32 m_localHaIpPort;
};

class PrismHaSyncConfigureStandbyMessage : public PrismMessage
{
    private :
    protected :
        virtual void setupAttributesForSerialization                        ();
        virtual void setupAttributesForSerializationInAttributeOrderFormat  ();

    public :
                            PrismHaSyncConfigureStandbyMessage ();
                            PrismHaSyncConfigureStandbyMessage (const string &activePrismVersion, const LocationRole &activeLocationRole);
        virtual            ~PrismHaSyncConfigureStandbyMessage ();
                string      getActivePrismVersion              ();
                LocationRole getActiveLocationRole             ();
                void        setValidationDetails                  (const UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToMessage = false);
                void        getValidationDetails                  (UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToUser = false);
                void        setValidationResults                  (const UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToMessage = false);
                void        getValidationResults                  (UI32 &bufferSize, void *&pBuffer, bool transferBufferOwnershipToUser = false);
        static  UI32        getDatabaseBackupBufferTag            ();

    // Now the data members

    private :
        string              m_activePrismVersion;
        UI32                m_activeLocationRole;
    protected :
    public :
};

class FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage : public ManagementInterfaceMessage
{
 public:

    FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage ();
    virtual ~FrameworkObjectManagerDisconnectFromAllInstanceClientsMessage();
    void setupAttributesForSerialization ();
    void setClientIpAddress(const string & ClientIpAddress);
    string const & getClientIpAddress() const;
 protected:
 private:


 public:
 protected:
 private:
  string m_clientIpAddress;
};


class FrameworkObjectManagerPrepareNodeForHAMessage : public PrismMessage
{

    public:
                 FrameworkObjectManagerPrepareNodeForHAMessage ();
        virtual ~FrameworkObjectManagerPrepareNodeForHAMessage ();
    protected:
    private:


    public:
    protected:
    private:

};

class FrameworkObjectManagerUpdateDatabaseSchema :  public PrismMessage
{
    public:
                FrameworkObjectManagerUpdateDatabaseSchema();
           virtual ~FrameworkObjectManagerUpdateDatabaseSchema();
    protected:
    private:

    public:
    protected:
    private:
};

class FrameworkObjectManagerListenForManagedObjectMessage : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                                                  FrameworkObjectManagerListenForManagedObjectMessage (const string &managedObjectClassName, const WaveManagedObjectListenOperation &managedObjectOperation);
        virtual                                  ~FrameworkObjectManagerListenForManagedObjectMessage ();

                string                            getManaegdObjectClassName                           () const;
                WaveManagedObjectListenOperation  getManagedObjectOperation                           ();

    // Now the data members

    private :
        string                           m_managedObjectClassName;
        WaveManagedObjectListenOperation m_managedObjectOperation;

    protected :
    public :
};

class FrameworkObjectManagerHaSyncUpdateMessage :  public PrismMessage
{
    public:
    			     FrameworkObjectManagerHaSyncUpdateMessage();
    	virtual      ~FrameworkObjectManagerHaSyncUpdateMessage();
                void setIsDbSyncRequired (const bool &isDbSyncRequired);
                bool getIsDbSyncRequired ();
    protected:
    private:

    public:
    protected:
    private:
        bool m_isDbSyncRequired;
};

class FrameworkObjectManagerGetFirmwareVersionMessage :  public PrismMessage
{
    public:
    			       FrameworkObjectManagerGetFirmwareVersionMessage();
    	virtual        ~FrameworkObjectManagerGetFirmwareVersionMessage();
                void   setFirmwareVersion              (const string & firmwareVersion );
                string getFirmwareVersion              ();
    protected:
        virtual void setupAttributesForSerialization                        ();
        virtual void setupAttributesForSerializationInAttributeOrderFormat  ();
    private:

    public:
    protected:
    private:
        string m_firmwareVersion;
};

class FrameworkObjectManagerEndHaSyncMessage : public PrismMessage
{
 public:

             FrameworkObjectManagerEndHaSyncMessage ();
    virtual ~FrameworkObjectManagerEndHaSyncMessage ();


 protected:
 private:

 public:
 protected:
 private:
};

class FrameworkObjectManagerStartCcmdHaSyncMessage : public PrismMessage
{
 public:

    FrameworkObjectManagerStartCcmdHaSyncMessage (const string &peerHaIpAddress, const SI32 &peerHaIpPort);
    virtual ~FrameworkObjectManagerStartCcmdHaSyncMessage();

    void   setPeerHaIpAddress(const string& peerHaIpAddress);
    void   setPeerHaIpPort(const SI32& peerHaIpPort);
    void   setFirmwareVersion (const string &firmwareVersion);
    void   setHAVersion (const UI32 &haVersion);
    void   setReturnData (const string &returnData);

    string getPeerHaIpAddress() const;
    SI32   getPeerHaIpPort() const;
    string getFirmwareVersion () const;
    UI32   getHAVersion () const;
    string getReturnData () const;

 protected:
 private:
    virtual void setupAttributesForSerialization       ();

 public:
 protected:
 private:
    string m_peerHaIpAddress;
    SI32 m_peerHaIpPort;
    string m_firmwareVersion;
    UI32 m_haVersion;
    string m_returnData;
};

class FrameworkObjectManagerCcmdHaSyncUpdateMessage :  public ManagementInterfaceMessage
{
    public:
    			     FrameworkObjectManagerCcmdHaSyncUpdateMessage();
    	virtual      ~FrameworkObjectManagerCcmdHaSyncUpdateMessage();
    protected:
    private:
        virtual void setupAttributesForSerialization       ();

    public:
    protected:
    private:
};

class FrameworkObjectManagerSetSyncStateMessage :  public PrismMessage
{
    public:
    			     FrameworkObjectManagerSetSyncStateMessage ();
    			     FrameworkObjectManagerSetSyncStateMessage (const UI32 &syncState);
    	virtual      ~FrameworkObjectManagerSetSyncStateMessage ();

                void setSyncState (const UI32 &syncState);
                UI32 getSyncState () const;
    protected:
    private:
        virtual void setupAttributesForSerialization       ();

    public:
    protected:
    private:
       UI32   m_syncState;
};

class FrameworkObjectManagerGetSyncStateMessage :  public PrismMessage
{
    public:
    			     FrameworkObjectManagerGetSyncStateMessage ();
    	virtual      ~FrameworkObjectManagerGetSyncStateMessage ();

                void setSyncState (const UI32 &syncState);
                UI32 getSyncState () const;
    protected:
    protected:
    private:
        virtual void setupAttributesForSerialization       ();

    public:
    protected:
    private:
       UI32   m_syncState;
};

class FrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration :  public PrismMessage
{
    public:
                      FrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration ();
        virtual      ~FrameworkObjectManagerPrepareForWarmRecoveryWithDefaultConfiguration ();

    protected:
    protected:
    private:
        virtual void setupAttributesForSerialization       ();

    public:
    protected:
    private:
};

class FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration :  public PrismMessage
{
    public:
                      FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration ();
        virtual      ~FrameworkObjectManagerPrepareForHaFailoverWithPersistentConfiguration ();

                bool  isWarmHaRecoveryPreparationInProgress  ();
                void  setWarmHaRecoveryPreparationInProgress (const bool &isWarmRecovery);
    protected:
    protected:
    private:
        virtual void setupAttributesForSerialization       ();

    public:
    protected:
    private:

               bool m_isWarmRecoveryInvolved;
};

class FrameworkObjectManagerSyncConfigurationMessage : public PrismMessage
{
    private :
        virtual void setupAttributesForSerialization       ();

    protected :
    public :
                                     FrameworkObjectManagerSyncConfigurationMessage ();
                                     FrameworkObjectManagerSyncConfigurationMessage (const string &prismFrameworkConfigurationFileName);
                                    ~FrameworkObjectManagerSyncConfigurationMessage ();

        string                       getPrismFrameworkConfigurationFileName () const;
        void                         setPrismFrameworkConfigurationFileName (const string &prismFrameworkConfigurationFileName);
        PrismFrameworkConfiguration  getPrismFrameworkConfigurationObject   () const;
        void                         setPrismFrameworkConfigurationObject   (const PrismFrameworkConfiguration &prismFrameworkConfiguration);

    // Now the data members

    private :
        string                      m_prismFrameworkConfigurationFileName;
        PrismFrameworkConfiguration m_prismFrameworkConfigurationObject;

    protected :
    public :
};

}
#endif // FRAMEWORKOBJECTMANAGERMESSAGES_H
