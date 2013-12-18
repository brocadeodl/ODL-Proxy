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
/*
#ifndef VCSREMOTEMESSAGINGOBJECTMANAGER_H
#define VCSREMOTEMESSAGINGOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/Types/BitMap.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "VcsFabric/Local/VcsRemoteEnqSwitchMessage.h"
#include "VcsFabric/Local/VcsAsyncClusterMergeMessage.h"

#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsNodeRemoteMessagingTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeRemoteMessagingObjectManager : public WaveLocalObjectManager
{
    private :
                VcsNodeRemoteMessagingObjectManager									();
		virtual  PrismMessage         *createMessageInstance		(const UI32 &operationCode);
  //      virtual  WaveManagedObject    *createManagedObjectInstance	(const string &managedClassName);
		void	VcsRemoteEnqSwitchMessageHandler					(VcsRemoteEnqSwitchMessage *pVcsRemoteEnqSwitchMessage);
  		void 	VcsAsyncClusterMergeMessageHandler 				(VcsAsyncClusterMergeMessage *pVcsAsyncClusterMergeMessage);

		void	getLocalSwitchInfo										(switchInfoParameters &localSwitchInfo);
		UI32	getRemoteSwitchInfo										(switchInfoParameters &remoteSwitchInfo);
		UI32 	sendClusterMergeMessage 								(switchInfoParameters &remoteSwitchInfo);

    protected :
    public :
        virtual    ~VcsNodeRemoteMessagingObjectManager			();

        static     string               getClassName						();
        static     VcsNodeRemoteMessagingObjectManager *getInstance		();
        static     PrismServiceId       getPrismServiceId					();
		// Now the data members

    private :

    protected :
    public :
};

}

#endif // VCSREMOTEMESSAGINGOBJECTMANAGER_H
*/
