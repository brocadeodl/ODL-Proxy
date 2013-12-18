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
 *   Author : Jitendra Singh                                    *
 ***************************************************************************/

#ifndef VCSFABOBJECTMANAGER_H
#define VCSFABOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Types/BitMap.h"
#include "Framework/Types/Uuid.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "Cluster/Local/WaveNode.h"
#include "Framework/Persistence/PersistenceObjectManagerCopyFileMessage.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#include "VcsFabric/Global/VcsCreateClusterMessage.h"
#include "VcsFabric/Global/VcsAddSwitchMessage.h"
#include "VcsFabric/Global/VcsRejoinSwitchMessage.h"
#include "VcsFabric/Global/VcsRemoveSwitchMessage.h"
#include "VcsFabric/Global/VcsDeleteClusterMessage.h"
#include "VcsFabric/Global/VcsMakePrincipalMessage.h"
#include "VcsFabric/Global/VcsClusterInfoMessage.h"
#include "VcsFabric/Global/VcsFabGetDomainListMessage.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "VcsFabric/Global/VcsClusterManagedObject.h"
#include "VcsFabric/Global/VcsSwitchSerialNumberMappedIdManagedObject.h"
#include "VcsFabric/Global/VcsFabMessagingContext.h"
#include "VcsFabric/Global/VcsUpdatePrincipalMessage.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Global/VirtualIpGlobalWorker.h"
#include "VcsFabric/Global/VcsClusterConnectionQueryWorker.h"
#include "VcsFabric/Global/VcsFabricGlobalMessages.h"
#include "VcsFabric/Global/VcsSegmentSwitchMessage.h"

using namespace WaveNs;

namespace DcmNs
{
class VcsFabricLocalSwitchInfoObject;
class VcsNodeLocalManagedObject;
class VcsFabRemoveSegmentNodeWorker;
class VcsFabDeleteClusterWorker;
class VcsFabVcsModeWorker;
class VirtualIpGlobalWorker;
class VcsClusterConnectionQueryWorker;
class VcsFabConfigDistributionWorker;
class VcsFabConfigSnapshotWorker;
class VcsNodeConfigSnapshotWorker;

class VcsFabObjectManager : public WaveObjectManager
{
    private :
                VcsFabObjectManager									();
        void	makePrincipalMessageHandler							(VcsMakePrincipalMessage *pMakePrincipalMessage);
        void	clusterInfoMessageHandler							(VcsClusterInfoMessage *pClusterInfoMessage);
        void    getDomainListMessageHandler                         (VcsFabGetDomainListMessage *pVcsFabGetDomainListMessage);




		void	DomainValidEventHandler								(const VcsNodeDomainValidEvent *&pEvent);
		void	DomainCapabilitiesAvailableEventHandler				(const VcsNodeDomainCapabilitiesAvailableEvent *&pEvent);
		void	DomainInvalidEventHandler							(const VcsNodeDomainInvalidEvent *&pEvent);
		void	DomainReachableEventHandler							(const VcsNodeDomainReachableEvent *&pEvent);
		void	DomainUnReachableEventHandler						(const VcsNodeDomainUnreachableEvent *&pEvent);
		void	establishPrincipalTimerHandler						(TimerHandle timerHandle, void * pContext);
		void	bootCompleteEventHandler							(const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);
		void	disable												(WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
		void	shutdown											(WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
        void    failover                                            (FailoverAsynchronousContext *pFailoverAsynchronousContext);

		void	listenForEvents										(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
		void	doControlledPrincipalChangeOver						(UI32 fcPrincipalDomainId);
		void	establishVcsPrincipal								(u_short *fabDomainList);
		UI32	setPrincipalFlag									(bool principalFalg);
		void	performClusterMerge									();

		virtual PrismMessage         *createMessageInstance		    (const UI32 &operationCode);
        virtual WaveManagedObject    *createManagedObjectInstance	(const string &managedClassName);
        vector<WaveManagedObject *> *fabOMDoSynchronousQuery        (const string className);
        VcsClusterManagedObject *getVcsClusterManagedObject	        ();

        void            updateClusterStatus                         ();
        ResourceId	deleteVcsClusterManagedObject					(VcsFabMessagingContext *pMessagingContext);

        ResourceId      deleteClusterManagedObject                                              ();

		ResourceId	deleteWaveNodeObjects   						(vector<VcsFabricLocalSwitchInfoObject> &switchesInfo);
		UI32	    deleteWaveNodeObject							(const ObjectId &objectId);

		UI32	deleteObjectIdFromVcsCluster						(ObjectId &removeSwitchObjectId, UI32 &nSwitchesInCluster);

		UI32	getRemoteNodeObjectIdFromLocalNodesDB				(string remoteSerialNumber, ObjectId &remoteNodeObjectId);
		UI32	getRemoteNodeObjectIdFromDomainId					(u_short domainId, ObjectId &remoteNodeObjectId);

        UI32	getPrincipalSwitchObjectId							(ObjectId &principalSwitchObjectId);
		string	getLocalSwitchSerialNumber							();

		UI32	getVcsClusterId										(bool &clusterExists);
		UI32	getRemoteSwitchInfo									(VcsFabricLocalSwitchInfoObject &switchInfo);


		UI32	getIpAddressFromInputString							(const string &ipAddressAndPort, string &ipAddress);
        static string  getIpAddressFromDomainId						(UI32 domainId);

        UI32	getLocalNodeVcsId									();
		VcsNodeLocalManagedObject   * getSwitchObjectFromObjectId	(const ObjectId &switchObjectId);
		VcsNodeLocalManagedObject	* getSwitchObjectFromDomainId	(UI32 domainId);
        UI32    getRemoteNodeDomainIdFromObjectId                   (const ObjectId &objectId, UI32 &domainId);

        ResourceId  getClusterInfoForNodeInWaitForRejonState        (VcsFabMessagingContext *pVcsFabMessagingContext);
        void        getClusterInfo                                  (VcsFabMessagingContext *pVcsFabMessagingContext);
		vector<VcsNodeLocalManagedObject *> *getNodeLocalManagedObjects ();
		UI32	getPortFromInputString								(const string &ipAddressAndPort, UI32 &port);
		static UI32	    getMgmtPort									(UI32 domainId);

		void	getLocalSwitchObjectId								(ObjectId &localSwitchObjectId);
		VcsNodeLocalManagedObject *getNodeLocalManagedObjectForThisNode		();

		void	getListOfSwitchesNotAddedToVcsClusterMO				(vector<VcsFabricLocalSwitchInfoObject> switchesInfo, vector<VcsFabricLocalSwitchInfoObject> &notAddedList);
		void	getListOfLocalNodesInCluster						(vector<VcsFabricLocalSwitchInfoObject> &addList, vector<VcsFabricLocalSwitchInfoObject> &inClusterList);
		UI32	getNumSwitchesInCluster								();

        UI32    getDomainsInCluster                                 (BitMap &switchesInCluster);
		Uuid	getClusterGUID										();
		void    setClusterGUID                                      (const Uuid &clusterGUID);
		void    clearClusterGUID                                    ();

		WaveNode * getWaveNode										(VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject);
		ResourceId getNodeStatus									(VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject, UI32 &specificStatus, UI32 &genericStatus);

		bool	isSwitchInfoRetrievedForDomainId					(const u_short &domainId);
		void	setSwitchInfoRetrievedBit							(const u_short &infoRetrievedDomain);
		void	setSizeForDomainBitMaps								(scnDomainsStruct & domainBitMapStruct);
		void	setBitMapSize										(BitMap &bitMap);

		bool	isSwitchInVcsClusterManagedObject					(string responderSerialNumber);
		bool	isSwitchInVcsClusterManagedObject					(const ObjectId &switchOid);
		bool	isSwitchDomainIdInMyCluster							(const VcsFabricLocalSwitchInfoObject &switchInfo);
                bool    isSwitchDomainIdInMyCluster                                             (UI32 domainId);
        bool    inFailedList                                        (VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject, vector<LocationId> failedLocationIds);
		bool	isFabDomainValid									();
		UI32	wereAllSwitchesAdded								(BitMap &switchesNotAdded);
		ResourceId updateClusterGUIDForAllNodeLocalManagedObjects	();

		bool	validateSwitchParameters							(VcsFabricLocalSwitchInfoObject &switchInfo);
        
        bool    validDomainId                                       (UI32 domainId);

		bool	sndbExists											();
		bool	vcsClusterMOExists									();
		bool	validIpV4SubString									(const string &ipSubString);
		bool	validIpV4Address									(const string &ipAddress);
        void    printLocalNodesList                                 ();
		void	printNodeSpecificStatus								(ResourceId nodeStatus);
		void	printNodeGenericStatus								(ResourceId nodeStatus);

		void	createClusterAndAddSwitches							();

        bool    vcsModeEnabled                                      ();

        void    registerFeatureGroupsAndXPaths                      ();
        void    install                                             (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void    boot                                                (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void    haboot                                              (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

    protected :
    public :
        virtual    ~VcsFabObjectManager								();

        static     string               getClassName				();
        static     VcsFabObjectManager *getInstance					();
        static     PrismServiceId       getPrismServiceId			();

		UI32	    getMyDomainId										();
		UI32 		getMyDomainIdFromVcsFabObjectManager();
        UI32        getPortFromDomainId								(UI32 domainId);
        void        deleteObjects									(vector<WaveManagedObject *> *pResults);
        string      getLocalSwitchIpAddress							();
        UI32        getLocalSwitchPort								();

		void		setBecomePrincipalFlag					(bool flag);
        bool        getBecomePrincipalFlag                                                      ();

		// Now the data members

    private :
		bool							m_bootComplete;
		bool							m_becomePrincipalFlag;
		TimerHandle						m_timerHandle;
		bool							m_timerRunningFlag;
		vector<VcsFabricLocalSwitchInfoObject>	m_remoteSwitchInfoList;
		scnDomainsStruct				m_knownDomainsList;
        VcsFabAddRejoinNodeWorker*      m_pVcsFabAddRejoinNodeWorker;
        VcsFabRemoveSegmentNodeWorker*  m_pVcsFabRemoveSegmentNodeWorker;
        VcsFabDeleteClusterWorker*      m_pVcsFabDeleteClusterWorker;
        VcsFabVcsModeWorker*            m_pVcsFabVcsModeWorker;
        VirtualIpGlobalWorker*          m_pVirtualIpGlobalWorker;
        VcsClusterConnectionQueryWorker* m_pVcsClusterConnectionQueryWorker;
        VcsFabConfigDistributionWorker* m_pVcsFabConfigDistributionWorker;
        VcsFabConfigSnapshotWorker*     m_pVcsFabConfigSnapshotWorker;
        BitMap                          m_removeSwitchDomainIdBitMap;
        Uuid                            m_clusterGUID;

    protected :
    public :
		friend class VcsFabricLocalObjectManager;
        friend class VcsFabAddRejoinNodeWorker;
        friend class VcsFabRemoveSegmentNodeWorker;
        friend class VcsFabDeleteClusterWorker; 
        friend class VirtualIpGlobalWorker; 
        friend class VcsClusterConnectionQueryWorker;
        friend class VcsFabConfigDistributionWorker;
        friend class VcsFabConfigSnapshotWorker;
        friend class VcsNodeConfigSnapshotWorker;

};
    
    inline string VcsFabObjectManager::getClassName ()
    {
        return ("VcsFabObjectManager");
    }

}

#endif // VCSFABOBJECTMANAGER_H

