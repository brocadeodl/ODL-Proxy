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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou	                                                   *
 ***************************************************************************/

#ifndef VCSNODESLOTSYNCHRONOUSMESSAGINGCONTEXT_H
#define VCSNODESLOTSYNCHRONOUSMESSAGINGCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "VcsFabric/Local/VcsSlotLocalManagedObject.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "Cluster/Local/WaveSlotLocalManagedObject.h"

namespace DcmNs
{
class VcsNodeSlotSynchronousMessagingContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
            VcsNodeSlotSynchronousMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);

           ~VcsNodeSlotSynchronousMessagingContext ();

           UI32 getHwType       () const;
           void setHwType       (const UI32 hwType);
		   LocationId getLocationId ();
		   void setLocationId	(const LocationId &locationId);
		   
		   VcsSlotLocalManagedObject  *getVcsSlotManagedObjectPointer () const;
		   void 					   setVcsSlotManagedObjectPointer (VcsSlotLocalManagedObject *m_pVcsSlotLocalManagedObject);
		   VcsNodeLocalManagedObject  *getVcsNodeManagedObjectPointer () const;
		   void 					   setVcsNodeManagedObjectPointer (VcsNodeLocalManagedObject *m_pVcsNodeLocalManagedObject);

    // Now the data members

    private :
        UI32            m_hwType;
		LocationId		m_locationId;
		VcsSlotLocalManagedObject  *m_pVcsSlotLocalManagedObject;
		VcsNodeLocalManagedObject  *m_pVcsNodeLocalManagedObject;

    protected :
    public :
};
}

#endif // VCSNODESLOTSYNCHRONOUSMESSAGINGCONTEXT_H
