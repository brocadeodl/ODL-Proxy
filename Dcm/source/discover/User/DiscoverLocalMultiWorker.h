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

/*
 * DiscoverLocalMultiWorker.h
 *
 *  Created on: Jun 6, 2012
 *      Author: rmadapur
 */

#ifndef DISCOVERLOCALMULTIWORKER_H_
#define DISCOVERLOCALMULTIWORKER_H_
#include "Framework/ObjectModel/WaveWorker.h"
#include "discover/events/vCenterEventsMessage.h"
#include "discover/Framework/discoverLocalMessages.h"
#include "discover/Framework/discoverManagementInterfaceMessages.h"
#include "VirtualDb/DAO/DAO.h"

using namespace WaveNs;
namespace DcmNs {

    class DiscoverLocalObjectManager;
    class DiscoverLocalDiscoverMessage;

    class DiscoverLocalMultiWorker: public WaveWorker {
        public:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(
                    const string &managedClassName);
            void DiscoverLocalDiscoverMessageHandler(
                    DiscoverLocalDiscoverMessage *pDiscoverLocalDiscoverMessage);
			void DatacenterEventMessageHandler(
                    LocalDatacenterEventMessage *pEventMessage);
            void VswitchEventMessageHandler(
                    LocalVswitchEventMessage *pEventMessage);
            void VmnicEventMessageHandler(
                    LocalVmnicEventMessage *pEventMessage);
            void PgHostVsEventMessageHandler(
                    LocalPgHostVsEventMessage *pEventMessage);
		    void VMEventMessageHandler(
		    		LocalVMEventMessage *pEventMessage);
		    void DVSEventMessageHandler(
		    		LocalDVSEventMessage *pEventMessage);
		    void DVPGEventMessageHandler(
		    		LocalDVPGEventMessage *pEventMessage);
		    void HostEventMessageHandler(
		    		LocalHostEventMessage *pEventMessage);
		    void VnicEventMessageHandler(
		    		LocalVNICEventMessage *pEventMessage);
		    void SpgEventMessageHandler(
		    		LocalSPGEventMessage *pEventMessage);
		    void SnicEventMessageHandler(
		    		LocalSNICEventMessage *pEventMessage);
//		    void SimpleMessageHandler(
//		    		TestLocalDiscoverMessage *pEventMessage);
		    void DPEventMessageHandler(
		    		LocalVNMDPMessage *pEventMessage);

           DiscoverLocalMultiWorker ( DiscoverLocalObjectManager *pDiscoverLocalObjectManager);
           void dbInitialize(string vcenter);
           void dbDelete(string vcenter);
           void correlate(string vcenter,bool isNodeRejoin=false);
           void deleteVCenter(string vcenter);
		   void cleanupInterfaces(string vcenter);
           void dbSync(string vcenter);
    	   UI32 getVcCount();
		   vector<Domain*> getSource(daoType_t type, std::string db_file, std::string mor = "");

            virtual ~DiscoverLocalMultiWorker();
    };

} /* namespace DcmNs */
#endif /* DISCOVERLOCALMULTIWORKER_H_ */
