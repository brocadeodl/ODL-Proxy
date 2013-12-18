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

#ifndef CLUSTERLOCALWAVESLOTMANAGEMENTWORKER_H
#define CLUSTERLOCALWAVESLOTMANAGEMENTWORKER_H

#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/ClusterLocalSlotOnlineMessage.h"
#include "Cluster/Local/ClusterLocalSlotOfflineMessage.h"
#include "Cluster/Local/ClusterLocalSlotRemoveMessage.h"
#include "Cluster/Local/ClusterLocalSlotRemoveAllMessage.h"
#include "Cluster/Local/ClusterLocalMessagingContext.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"


namespace WaveNs
{

class ClusterLocalWaveSlotManagementWorker : public WaveWorker
{
    protected :
    public :
                    ClusterLocalWaveSlotManagementWorker         (ClusterLocalObjectManager *pClusterLocalObjectManager);
        virtual     ~ClusterLocalWaveSlotManagementWorker        ();
        static      ClusterLocalWaveSlotManagementWorker *getInstance ();
        string      getClassName                            ();
		
	private :
			
		void		slotOnlineMessageHandler (ClusterLocalSlotOnlineMessage *pClusterLocalSlotOnlineMessage);
		ResourceId  getSlotObjects (ClusterLocalMessagingContext *pClusterLocalMessagingContext);
		ResourceId  validateSlotAdd (ClusterLocalMessagingContext *pClusterLocalMessagingContext);
		ResourceId  createOrUpdateSlotManagedObject(ClusterLocalMessagingContext *pClusterLocalMessagingContext);
		ResourceId createCompositionAssociationIfNewSlotMO (ClusterLocalMessagingContext *pClusterLocalMessagingContext);
		ResourceId  deleteSlotManagedObject (ClusterLocalMessagingContext *pClusterLocalMessagingContext);
		
		void 		slotRemoveMessageHandler (ClusterLocalSlotRemoveMessage *pSlotRemoveMessage);
		//ResourceId  validateRemoveSlot (ClusterLocalMessagingContext *pClusterLocalMessagingContext);
		ResourceId  removeSlot (ClusterLocalMessagingContext *pClusterLocalMessagingContext);
		ResourceId  removeSlotCompositionAssociation (ClusterLocalMessagingContext *pClusterLocalMessagingContext);

		void 	   slotOfflineMessageHandler (ClusterLocalSlotOfflineMessage *pClusterLocalSlotOfflineMessage);
		ResourceId validateSlotUpdate (ClusterLocalMessagingContext *pClusterLocalMessagingContext);
		ResourceId updateSlotManagedObject(ClusterLocalMessagingContext *pClusterLocalMessagingContext);

		void 	   slotRemoveAllMessageHandler (ClusterLocalSlotRemoveAllMessage *pSlotRemoveAllMessage);
		ResourceId removeAllSlots (ClusterLocalMessagingContext *pClusterLocalMessagingContext);

    // Now the Data Members

        friend class ClusterLocalObjectManager;

    protected :
    public :

};

}

#endif // CLUSTERLOCALWAVESLOTMANAGEMENTWORKER_H
