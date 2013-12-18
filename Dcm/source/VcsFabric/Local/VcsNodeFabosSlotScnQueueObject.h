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

#ifndef VCSNODEFABOSSLOTSCNQUEUEOBJECT_H
#define VCSNODEFABOSSLOTSCNQUEUEOBJECT_H

#include "sysmod/syscomm.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsNodeFabosInterfaceObjectManager.h"
#include <deque>

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeFabosSlotScnQueueObject
{
    private :
    protected :

    public :
				VcsNodeFabosSlotScnQueueObject ();
				VcsNodeFabosSlotScnQueueObject (UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version);
				VcsNodeFabosSlotScnQueueObject (UI32 &slotNumber, UI32 &hwType, UI32 &slotState);
				
				~VcsNodeFabosSlotScnQueueObject ();
				VcsNodeFabosSlotScnQueueObject & operator=(const VcsNodeFabosSlotScnQueueObject &rhs);

	private:
		UI32	getSlotScnObjectId () const;
		void	setSlotScnObjectId (const UI32 &scnObjectId);
        UI32    getSlotNumber                () const;
        void    setSlotNumber                (const UI32 &slotNumber);
		UI32    getObjectType                () const;
		void	setObjectType                (const UI32 &objectType);
		UI32    getHwType                    () const;
		void	setHwType                    (const UI32 &hwType);
		UI32    getHwId                      () const;
		void	setHwId	                     (const UI32 &hwId);
		UI32	getSlotState                 () const;
		void	setSlotState                 (const UI32 &slotState);
		UI32    getReason                    () const;
		void	setReason                    (const UI32 &reason);
		UI32    getVersion                   () const;
		void    setVersion                   (const UI32 &version);

    // Now the data members

    private :
    	UI32   m_slotScnObjectId;
		UI32   m_slotNumber;
		UI32   m_objectType;
		UI32   m_hwType;
		UI32   m_hwId;
		UI32   m_slotState;
		UI32   m_reason;
		UI32   m_version;

    protected :
    public :
		friend class VcsNodeFabosInterfaceObjectManager;
		friend class VcsNodeSlotManagementWorker;
};
}

#endif // VCSNODEFABOSSLOTSCNQUEUEOBJECT_H


