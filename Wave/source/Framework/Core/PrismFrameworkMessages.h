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
 ***************************************************************************/

#ifndef PRISMFRAMEWORKMESSAGES_H
#define PRISMFRAMEWORKMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/Core/PrismFrameworkObjectManagerInitializeWorker.h"

#include <vector>
#include <string>

using namespace std;

namespace WaveNs
{

class WaveBrokerPublishMessage;

class PrismInitializeObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
                       PrismInitializeObjectManagerMessage (PrismServiceId id, const WaveBootReason &reason);

        WaveBootReason getReason                           () const;

    // Now the data members

    private :
        WaveBootReason m_reason;

    protected :
    public :
};

class PrismInstallObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
                       PrismInstallObjectManagerMessage (PrismServiceId id, const WaveBootReason &reason);

        WaveBootReason getReason                        () const;

    // Now the data members

    private :
        WaveBootReason m_reason;

    protected :
    public :
};

class PrismEnableObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
                       PrismEnableObjectManagerMessage (PrismServiceId id, const WaveBootReason &reason);
        WaveBootReason getReason                       () const;

    // Now the data members

    private :
        WaveBootReason m_reason;

    protected :
    public :
};

class PrismUpgradeObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
                       PrismUpgradeObjectManagerMessage (PrismServiceId id, const WaveBootReason &reason);
        WaveBootReason getReason                        () const;

    // Now the data members

    private :
        WaveBootReason m_reason;

    protected :
    public :
};

class PrismBootObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
                       PrismBootObjectManagerMessage (PrismServiceId id, const WaveBootReason &reason, const bool &rollBackFlag = false);
        WaveBootReason getReason                     () const;
        bool           getRollBackFlag               () const;
    // Now the data members

    private :
        WaveBootReason m_reason;
        bool           m_rollBackFlag;
    protected :
    public :
};

class PrismHaInstallObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
                       PrismHaInstallObjectManagerMessage (PrismServiceId id, const WaveBootReason &reason);

        WaveBootReason getReason                        () const;
    // Now the data members
    private :
        WaveBootReason m_reason;

    protected :
    public :
};

class PrismHaBootObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
                       PrismHaBootObjectManagerMessage (PrismServiceId id, const WaveBootReason &reason);
        WaveBootReason getReason                     () const;
    // Now the data members
    private :
        WaveBootReason m_reason;
    protected :
    public :
};

class PrismPostbootObjectManagerMessage : public PrismMessage
{
    private :
        virtual void setupAttributesForSerialization ();
    protected :
    public :
                       PrismPostbootObjectManagerMessage (PrismServiceId id, SI8 passNum, string passName, UI32 slotNum, UI32 recoveryType);
        SI8            getPassNum                    () const;
        string         getPassName                   () const;
        UI32           getSlotNum                    () const;
        UI32           getRecoveryType               () const;
    // Now the data members
    private :
        SI8            m_passNum;
        string         m_passName;
        UI32           m_slotNum;
        UI32           m_recoveryType;
    protected :
    public :
};

class PrismShutdownObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
        PrismShutdownObjectManagerMessage (PrismServiceId id);

    // Now the data members

    private :
    protected :
    public :
};

class PrismDisableObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
        PrismDisableObjectManagerMessage (PrismServiceId id);

    // Now the data members

    private :
    protected :
    public :
};

class PrismUninstallObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
        PrismUninstallObjectManagerMessage (PrismServiceId id);

    // Now the data members

    private :
    protected :
    public :
};

class PrismUninitializeObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
        PrismUninitializeObjectManagerMessage (PrismServiceId id);

    // Now the data members

    private :
    protected :
    public :
};

class PrismDestructObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
        PrismDestructObjectManagerMessage (PrismServiceId id);

    // Now the data members

    private :
    protected :
    public :
};

class PrismPingObjectManagerMessage : public PrismMessage
{
    private :
        PrismPingObjectManagerMessage (PrismServiceId id);

    protected :
    public :

    // Now the data members

    private :
    protected :
    public:
};

class PrismTimerExpiredObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
                                               PrismTimerExpiredObjectManagerMessage (PrismServiceId id, TimerHandle m_timerId, PrismTimerExpirationHandler pPrismTimerExpirationCallback, void *pPrismTimerExpirationContext, PrismElement *pPrismTimerSender);
                                               PrismTimerExpiredObjectManagerMessage (const PrismTimerExpiredObjectManagerMessage &prismTimerExpiredObjectManagerMessage);
        PrismTimerExpiredObjectManagerMessage &operator =                            (const PrismTimerExpiredObjectManagerMessage &prismTimerExpiredObjectManagerMessage);
        PrismTimerExpirationHandler            getTimerExpirationCallback            ();
        void                                  *getTimerExpirationContext             ();
        TimerHandle                            getTimerId                            ();
        PrismElement                          *getTimerSender                        ();

    // Now the data members

    private :
        TimerHandle                  m_timerId;
        PrismTimerExpirationHandler  m_pPrismTimerExpirationCallback;
        void                        *m_pPrismTimerExpirationContext;
        PrismElement                *m_pPrismTimerSender;

    protected :
    public :

};

class WaveObjectManagerDatabaseSanityCheckMessage : public PrismMessage
{
    private :
    protected :
    public :
                       WaveObjectManagerDatabaseSanityCheckMessage (PrismServiceId id, const WaveBootReason &bootReason);
        WaveBootReason getBootReason                               () const;

    // Now the data members

    private :
        WaveBootReason  m_bootReason;

    protected :
    public :
};

class WaveObjectManagerCollectValidationDataMessage : public PrismMessage
{
    private :
    protected :
    public :
              WaveObjectManagerCollectValidationDataMessage (PrismServiceId id);
             ~WaveObjectManagerCollectValidationDataMessage ();
        void  setValidationDetails                           (void *const pData, const UI32 &size, bool willBeOwnedByMessage = false);
        void  getValidationDetails                           (void *&pData, UI32 &size);

    // Now the data members

    private :
    protected :
    public:
};

class WaveObjectManagerValidateClusterCreationMessage : public PrismMessage
{
    private :
    protected :
    public :
                WaveObjectManagerValidateClusterCreationMessage (PrismServiceId id);
                ~WaveObjectManagerValidateClusterCreationMessage ();
        void  setValidationDetails                             (void *const pData, const UI32 &size, bool willBeOwnedByMessage = false);
        void  getValidationDetails                             (void *&pData, UI32 &size);
        void  setValidationResults                             (void *const pData, const UI32 &size, bool willBeOwnedByMessage = false);
        void  getValidationResults                             (void *&pData, UI32 &size);

    // Now the data members

    private :
    protected :
    public:
};

class WaveObjectManagerSendValidationResultsMessage : public PrismMessage
{
    private :
    protected :
    public :
              WaveObjectManagerSendValidationResultsMessage (PrismServiceId id);
             ~WaveObjectManagerSendValidationResultsMessage ();
        void  addValidationResults                           (const string &ipAddress, const SI32 &port, void *pValidationResults, const UI32 &validationResultsSize);
        void  getValidationResults                           (const string &ipAddress, const SI32 &port, void *&pValidationResults, UI32 &validationResultsSize);
        UI32  getNumberOfLocationsThatSentValidationResults  ();
        void  getLocationDetailsAtIndex                      (const UI32 &index, string &ipAddress, SI32 &port);

    // Now the data members

    private :
        vector<string> m_ipAddressesVector;
        vector<SI32>   m_portsVector;

    protected :
    public:
};

class WaveObjectManagerHaSyncCollectValidationDataMessage : public PrismMessage
{
    private :
    protected :
    public :
              WaveObjectManagerHaSyncCollectValidationDataMessage (PrismServiceId id);
             ~WaveObjectManagerHaSyncCollectValidationDataMessage ();
        void  setValidationDetails                           (void *const pData, const UI32 &size, bool willBeOwnedByMessage = false);
        void  getValidationDetails                           (void *&pData, UI32 &size);

    // Now the data members

    private :
    protected :
    public:
};

class WaveObjectManagerHaSyncValidateDataMessage : public PrismMessage
{
    private :
    protected :
    public :
                WaveObjectManagerHaSyncValidateDataMessage (PrismServiceId id);
                ~WaveObjectManagerHaSyncValidateDataMessage ();
        void  setValidationDetails                             (void *const pData, const UI32 &size, bool willBeOwnedByMessage = false);
        void  getValidationDetails                             (void *&pData, UI32 &size);
        void  setValidationResults                             (void *const pData, const UI32 &size, bool willBeOwnedByMessage = false);
        void  getValidationResults                             (void *&pData, UI32 &size);

    // Now the data members

    private :
    protected :
    public:
};

class WaveObjectManagerHaSyncSendValidationResultsMessage : public PrismMessage
{
    private :
    protected :
    public :
              WaveObjectManagerHaSyncSendValidationResultsMessage (PrismServiceId id);
             ~WaveObjectManagerHaSyncSendValidationResultsMessage ();
        void  addValidationResults                           (void *pValidationResults, const UI32 &validationResultsSize);
        void  getValidationResults                           (void *&pValidationResults, UI32 &validationResultsSize);

    // Now the data members

    private :

    protected :
    public:
};


class WaveObjectManagerRegisterEventListenerMessage : public PrismMessage
{
    private :
    protected :
    public :
              WaveObjectManagerRegisterEventListenerMessage (PrismServiceId id, const UI32 &operationCodeToListenFor, const PrismServiceId &listenerPrismServiceId, const LocationId &listenerLocationId);
             ~WaveObjectManagerRegisterEventListenerMessage ();

        UI32           getOperationCodeToListenFor           () const;
        void           setOperationCodeToListenFor           (const UI32 &operationCodeToListenFor);
        PrismServiceId getListenerPrismServiceId             () const;
        void           setListenerPrismServiceId             (const PrismServiceId &listenerPrismServiceId);
        LocationId     getListenerLocationId                 () const;
        void           setListenerLocationId                 (const LocationId &listenerLocationId);

    // Now the data members

    private :
        UI32           m_operationCodeToListenFor;
        PrismServiceId m_listenerPrismServiceId;
        LocationId     m_listenerLocationId;

    protected :
    public :
};

class PrismFailoverObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
                                                      PrismFailoverObjectManagerMessage (PrismServiceId id, const FrameworkObjectManagerFailoverReason &failoverReason, const bool isPrincipalChangedWithThisFailover);
        virtual                                      ~PrismFailoverObjectManagerMessage ();

                FrameworkObjectManagerFailoverReason  getFailoverReason                 () const;
                vector<LocationId>                   &getFailedLocationIds              ();
                bool                                  getIsPrincipalChangedWithThisFailover () const;

    // Now the data members

    private :
        FrameworkObjectManagerFailoverReason m_failoverReason;
        vector<LocationId>                   m_failedLocationIds;
        bool                                 m_isPrincipalChangedWithThisFailover;

    protected :
    public :
};

class PrismPauseObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
        PrismPauseObjectManagerMessage (PrismServiceId id);

    // Now the data members

    private :
    protected :
    public :
};

class PrismResumeObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
        PrismResumeObjectManagerMessage (PrismServiceId id);

    // Now the data members

    private :
    protected :
    public :
};

class PrismSetCpuAffinityObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
                     PrismSetCpuAffinityObjectManagerMessage (PrismServiceId id);

        vector<UI32> getCpuAffinityVector                    () const;
        void         setCpuAffinityVector                    (const vector<UI32> &cpuAffinityVector);

    // Now the data members

    private :
        vector<UI32> m_cpuAffinityVector;

    protected :
    public :
};

class PrismListenForEventsObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
        PrismListenForEventsObjectManagerMessage (PrismServiceId id);

    // Now the data members

    private :
    protected :
    public :
};

class PrismLoadOperationalDataForManagedObjectObjectManagerMessage : public PrismMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
        PrismLoadOperationalDataForManagedObjectObjectManagerMessage ();
        PrismLoadOperationalDataForManagedObjectObjectManagerMessage (PrismServiceId id, ObjectId objectId, const vector<string> &operationalDataFields);

        ObjectId       getObjectId              () const;
        vector<string> getOperationalDataFields () const;

    // Now the data members

    private :
        ObjectId       m_objectId;
        vector<string> m_operationalDataFields;

    protected :
    public :
};

class PrismHeartbeatFailureObjectManagerMessage : public PrismMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
        PrismHeartbeatFailureObjectManagerMessage (PrismServiceId id, IpV4Address ipAddress, UI16 portNumber);

        IpV4Address  getIpAddress () const;
		UI16         getPortNumber () const;

	// Now the data members
    private :
		IpV4Address m_ipAddress;
		UI16        m_portNumber;
    protected :
    public :
};

class PrismExternalStateSynchronizationObjectManagerMessage : public PrismMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
        PrismExternalStateSynchronizationObjectManagerMessage (PrismServiceId id, UI32 fssStageNumber, ResourceId serviceType);

		UI32         getFssStageNumber () const;
		ResourceId   getServiceType() const;

	// Now the data members
    private :
        UI32          m_fssStageNumber;
        ResourceId    m_serviceType;
    protected :
    public :
};

class PrismConfigReplayEndObjectManagerMessage : public PrismMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
        PrismConfigReplayEndObjectManagerMessage (PrismServiceId id);

	// Now the data members
    private :
    protected :
    public :
};

class PrismFileReplayEndObjectManagerMessage : public PrismMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
        PrismFileReplayEndObjectManagerMessage (PrismServiceId id);

	// Now the data members
    private :
    protected :
    public :
};

class PrismSlotFailoverObjectManagerMessage : public PrismMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
        PrismSlotFailoverObjectManagerMessage (PrismServiceId id, UI32 slotNumber);
        UI32    getSlotNumber () const;


	// Now the data members
    private :
        UI32          m_slotNumber;
    protected :
    public :
};

class PrismMultiPartitionCleanupObjectManagerMessage : public PrismMessage
{
    private :
        virtual     void    setupAttributesForSerialization                 ();

    protected :
    public :
        PrismMultiPartitionCleanupObjectManagerMessage    (PrismServiceId id, string &partitionName, ObjectId &ownerPartitionManagedObjectId);
        string      getPartitionName                                   () const;
        ObjectId    getOwnerPartitionManagedObjectId                   () const ;

        bool        getIsPartialCleanup                                () const;

        void        setPartialCleanupTag                               (const ResourceId &partialCleanupTag);
        ResourceId  getPartialCleanupTag                               ();

        // Now the data members
    private :
        string        m_partitionName;
        ObjectId      m_ownerPartitionManagedObjectId;
        bool          m_isPartialCleanup;
        ResourceId    m_partialCleanupTag;


    protected :
    public :
};

class WaveObjectManagerBackendSyncUpMessage : public PrismMessage
{
    private :
    protected :
    public :
              WaveObjectManagerBackendSyncUpMessage (PrismServiceId id);


    // Now the data members

    private :

    protected :
    public :
};


class WaveObjectManagerMessageHistoryDumpMessage : public PrismMessage
{
    private :
        virtual void                   setupAttributesForSerialization ();
    protected :
    public :
                                       WaveObjectManagerMessageHistoryDumpMessage (PrismServiceId id);
        virtual                       ~WaveObjectManagerMessageHistoryDumpMessage ();

                vector<string>        &getMessageHistoryDumpStringVector          ();
                void                   setMessageHistoryDumpStringVector          (vector<string> &messageHistoryDumpStringVector);

    // Now the data members

    private :
        vector<string>                 m_messageHistoryDumpStringVector;

    protected :
    public :
};

class WaveObjectManagerMessageHistoryConfigMessage : public PrismMessage
{
    private :
        virtual void                    setupAttributesForSerialization ();

    protected :
    public :
                                        WaveObjectManagerMessageHistoryConfigMessage (PrismServiceId id);
        virtual                        ~WaveObjectManagerMessageHistoryConfigMessage ();

                bool                    getMessageHistoryState                       () const;
                void                    setMessageHistoryState                       (bool messageHistoryState);
                UI32                    getMessageHistoryMaxSize                     () const;
                void                    setMessageHistoryMaxSize                     (UI32 messageHistoryMaxSize);

    // Now the data members

    private :
        bool                m_messageHistoryState;
        UI32                m_messageHistoryMaxSize;

    protected :
    public :
};

class WaveObjectManagerUpdateRelationshipMessage : public PrismMessage
{
    private:
    protected:
                void    setupAttributesForSerialization ();
    public:
                        WaveObjectManagerUpdateRelationshipMessage ();
                        WaveObjectManagerUpdateRelationshipMessage (PrismServiceId id);
                        WaveObjectManagerUpdateRelationshipMessage (PrismServiceId id, UI32 operationCode);
        virtual        ~WaveObjectManagerUpdateRelationshipMessage ();

        void            setRelationshipVariables    (const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        void            getRelationshipVariables    (string &parentClassName, string &childClassName, string &relationshipName, ObjectId &parentObjectId, ObjectId &childObjectId);

        void            setIsAddingRelationship     (const bool &isAddingRelationship);
        bool            getIsAddingRelationship     () const;

    // data member

    private:
                string      m_parentClassName;
                string      m_childClassName;
                string      m_relationshipName;
                ObjectId    m_parentObjectId;
                ObjectId    m_childObjectId;
                bool        m_isAddingRelationship;
};

class WaveObjectManagerUpdateWaveManagedObjectMessage: public PrismMessage
{
    private :
    protected :
        void setupAttributesForSerialization ();

    public :
                                WaveObjectManagerUpdateWaveManagedObjectMessage ();
                                WaveObjectManagerUpdateWaveManagedObjectMessage (PrismServiceId id);
                                WaveObjectManagerUpdateWaveManagedObjectMessage (PrismServiceId id, UI32 operationCode);
        virtual                ~WaveObjectManagerUpdateWaveManagedObjectMessage ();

                ObjectId        getInputWaveManagedObjectId                     () const;
                void            setInputWaveManagedObjectId                     (const ObjectId &waveManagedObjectId);

                ObjectId        getOperateOnWaveManagedObjectId                 () const;
                void            setOperateOnWaveManagedObjectId                 (const ObjectId &waveManagedObjectId);

                vector<UI32>    getAttributeUserTags                            () const;
                void            setAttributeUserTags                            (const vector<UI32> &attributeUserTags);

                vector<string>  getAttributeNames                               () const;
                void            setAttributeNames                               (const vector<string> &attributeNames);

                vector<string>  getAttributeValues                              () const;
                void            setAttributeValues                              (const vector<string> &attributeValues);

                ObjectId        getParentObjectId                               () const;
                void            setParentObjectId                               (const ObjectId &parentObjectId);

                UI32            getChildUserTag                                 () const;
                void            setChildUserTag                                 (const UI32 &childUserTag);

                UI32            getChoiceUserTag                                () const;
                void            setChoiceUserTag                                (const UI32 &choiceUserTag);

                bool            getIsPropagateErrorCodeNeeded                   () const;
                void            setIsPropagateErrorCodeNeeded                   (const bool isErrorCode);

                UI32            getChoiceUserTagNeedToRemove                    () const;
                void            setChoiceUserTagNeedToRemove                    (const UI32 &choiceUserTagNeedToRemove);

                UI32            getWarningResourceId                            () const;
                void            setWarningResourceId                            (const UI32 warningResourceId);

                bool            getConfigReplayInProgressFlag                   () const;
                void            setConfigReplayInProgressFlag                   (const bool isConfigReplayInProgress);
    // Now the data members

    private :
        ObjectId       m_inputWaveManagedObjectId;
        ObjectId       m_operateOnWaveManagedObjectId;
        vector<UI32>   m_attributeUserTags;
        vector<string> m_attributeNames;
        vector<string> m_attributeValues;
        ObjectId       m_parentObjectId;
        UI32           m_childUserTag;
        UI32           m_choiceUserTag;
        bool           m_isErrorCode;
        UI32           m_choiceUserTagNeedToRemove;
        UI32           m_warningResourceId;
        bool           m_isConfigReplayInProgress;

    protected :
    public :
};

class WaveObjectManagerCreateWaveManagedObjectMessage : public WaveObjectManagerUpdateWaveManagedObjectMessage
{
    private :
        void setupAttributesForSerialization ();

    protected:
    public:

                                WaveObjectManagerCreateWaveManagedObjectMessage     ();
                                WaveObjectManagerCreateWaveManagedObjectMessage     (PrismServiceId id);
        virtual                ~WaveObjectManagerCreateWaveManagedObjectMessage     ();

        string                  getManagedObjectClassNameNeedToBeCreated            () const;
        void                    setManagedObjectClassNameNeedToBeCreated            (const string &managedObjectClassName);

        ObjectId                getObjectId                                         () const;
        void                    setObjectId                                         (const ObjectId &objectId);

    // Now the data members
    private:
        string      m_managedObjectClassName;
        ObjectId    m_objectId;
    protected:
    public:

};


class WaveObjectManagerDeleteWaveManagedObjectMessage: public WaveObjectManagerUpdateWaveManagedObjectMessage
{
    private :
    protected :
        void setupAttributesForSerialization ();
    public :
                                WaveObjectManagerDeleteWaveManagedObjectMessage ();
                                WaveObjectManagerDeleteWaveManagedObjectMessage (PrismServiceId id);
                                WaveObjectManagerDeleteWaveManagedObjectMessage (PrismServiceId id, UI32 operationCode);
        virtual                ~WaveObjectManagerDeleteWaveManagedObjectMessage ();

                UI32            getIsMO                                         () const;
                void            setIsMO                                         (const UI32 &isMO);
    // Now the data members

    private :
        UI32           m_isMO;                          // Is the delete operation being done on a MO?

    protected :
    public :
};

class WaveObjectManagerDeleteWaveManagedObjectsMessage: public WaveObjectManagerDeleteWaveManagedObjectMessage
{

    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                                WaveObjectManagerDeleteWaveManagedObjectsMessage ();
                                WaveObjectManagerDeleteWaveManagedObjectsMessage (PrismServiceId id);
        virtual                ~WaveObjectManagerDeleteWaveManagedObjectsMessage ();

              vector<ObjectId>        getManagedObjectsIdVector                        () const;
              void                    setManagedObjectsIdVector                        (const vector<ObjectId> &ManagedObjectIdVector);

              vector<vector<string> > getAttributeNameVectorVector                     () const;
              void                    setAttributeNameVectorVector                     (const vector<vector<string> > &AttributeNameVectorVector);

              vector<vector<UI32> >   getAttributeUserTagVectorVector                  () const;
              void                    setAttributeUserTagVectorVector                  (const vector<vector<UI32> > &AttributeUserTagVectorVector);

    // Now the data members

    private :
            vector<ObjectId>        m_ManagedObjectIdVector;
            vector<vector<string> > m_AttributeNameVectorVector;
            vector<vector<UI32> >   m_AttributeUserTagVectorVector;
    protected :
    public :
};

class PrismZeroizeObjectManagerMessage : public PrismMessage
{
    private :
    protected :
    public :
        PrismZeroizeObjectManagerMessage (PrismServiceId id);

    // Now the data members

    private :
    protected :
    public :
};

class WaveObjectManagerGetDebugInformationMessage : public ManagementInterfaceMessage
{
    private :
                void                setupAttributesForSerialization                 ();
    protected :
    public :
                                    WaveObjectManagerGetDebugInformationMessage     ();
                                    WaveObjectManagerGetDebugInformationMessage     (const string &serviceName);
        virtual                    ~WaveObjectManagerGetDebugInformationMessage     ();

                void                setDebugInformation                             (const string &debugInformation);
                const   string &    getDebugInformation                             () const;

    // Now the data members

    private :
                        string      m_debugInformation;

    protected :
    public :

};

class WaveObjectManagerResetDebugInformationMessage : public ManagementInterfaceMessage
{
    private :
    protected :
    public :
                                    WaveObjectManagerResetDebugInformationMessage   ();
                                    WaveObjectManagerResetDebugInformationMessage   (const string &serviceName);
        virtual                    ~WaveObjectManagerResetDebugInformationMessage   ();

    // Now the data members

    private :
    protected :
    public :

};

class WaveObjectManagerGetDataFromClientMessage : public PrismMessage
{
    private :
                    void            setupAttributesForSerialization                 ();
    protected :
    public :
                                    WaveObjectManagerGetDataFromClientMessage   ();
                                    WaveObjectManagerGetDataFromClientMessage   (PrismServiceId id);
                                    WaveObjectManagerGetDataFromClientMessage   (PrismServiceId id, UI32 commandCode, UI32 commandType, bool sendToClusterRequested = false );
        virtual                    ~WaveObjectManagerGetDataFromClientMessage   ();

                    void            setEnablePaginate               (const bool &enablePaginate);
                    bool            getEnablePaginate               () const;

                    void            setGlobalPluginServiceCode      (const UI32 &globalPluginServiceCode);
                    UI32            getGlobalPluginServiceCode      () const;

                    void            setLocalPluginServiceCode       (const UI32 &localPluginServiceCode);
                    UI32            getLocalPluginServiceCode       () const;

                    void            setClientName                   (const string &clientName);
                    void            getClientName                   (string &client) const;

                    void            setSelectedNodes                (const vector<LocationId> &selectedNodes);
                    void            getSelectedNodes                (vector<LocationId> &selectedNodes) const;

                    void            setCommandCode                  (const UI32 &commandCode);
                    UI32            getCommandCode                  () const ;

                    void            setCommandType                  (const UI32 &commandType);
                    UI32            getCommandType                  () const ;

                    void            setNumberOfRecordsRequested     (const UI32 &numberOfRecordsRequested);
                    UI32            getNumberOfRecordsRequested     () const;

                    void            setNumberOfCommandStrings       (const SI32 numberOfCommandStrings);
                    SI32            getNumberOfCommandStrings       () const;

                    void            setCommandStrings               (const vector<string> &commandStrings);
                    vector<string>  getCommandStrings               () const;

                    void            setSendToClusterRequested       (const bool &sendToClusterRequested );
                    bool            getSendToClusterRequested       () const;

                    void            addToMoreRecordsVector          (const bool &moreRecord);
                    void            getMoreRecordsVector            (vector<bool> &moreRecordsVector) const;

                    void            setNewServiceCode               (const UI32 &serviceCode);
                    void            copyClientDataFromMessage       (PrismMessage *pPrismMessage);
                    void            getDataSentFromAllClients       (vector<LocationId> &selectedNodes, vector<UI32> &clientStatus, vector<void *> &buffers, vector<bool> &moreRecordsVector);

                    void            copyAllBuffersFromMessage       (const PrismMessage *pPrismMessage);

 // Now the data members

    private :
    protected :
    public :
                bool                    m_enablePaginate;
                vector<bool>            m_moreRecordsVector;
                string                  m_clientName;
                UI32                    m_globalPluginServiceCode;
                UI32                    m_localPluginServiceCode;
                vector<LocationId>      m_selectedNodes;
                UI32                    m_commandCode;
                UI32                    m_commandType;
                UI32                    m_numberOfRecordsRequested;
                bool                    m_sendToClusterRequested;
                SI32                    m_numberOfCommandStrings;
                vector<string>          m_commandStrings;
};

class WaveDeliverBrokerPublishMessage : public PrismMessage
{
    private :
    protected :
    public :
                                  WaveDeliverBrokerPublishMessage (PrismServiceId id);
                                 ~WaveDeliverBrokerPublishMessage ();

        WaveBrokerPublishMessage *getPWaveBrokerPublishMessage    ();
        void                      setPWaveBrokerPublishMessage    (WaveBrokerPublishMessage *pWaveBrokerPublishMessage);

    // Now the data members

    private :
        WaveBrokerPublishMessage *m_pWaveBrokerPublishMessage;

    protected :
    public :
};

}

#endif //PRISMFRAMEWORKMESSAGES_H
