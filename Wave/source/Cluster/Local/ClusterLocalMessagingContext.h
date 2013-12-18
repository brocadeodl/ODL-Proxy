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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh                                             *
 ***************************************************************************/

#ifndef CLUSTERLOCALMESSAGINGCONTEXT_H
#define CLUSTERLOCALMESSAGINGCONTEXT_H

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Cluster/Local/WaveSlotLocalManagedObject.h"
#include "Cluster/Local/WaveNode.h"

namespace WaveNs
{

class ClusterLocalMessagingContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
                                    ClusterLocalMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                   ~ClusterLocalMessagingContext ();
								   
  	   WaveSlotLocalManagedObject *getWaveSlotManagedObjectPointer ();
	   void						   setWaveSlotManagedObjectPointer (WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject);
	   UI32 					   getSlotNumber ();
	   void						   setSlotNumber (UI32 &slotNumber);
	   UI32 					   getHwType ();
	   void						   setHwType (UI32 &slotNumber);
	   LocationId				   getLocationId ();
	   void						   setLocationId (LocationId &locationId);
	   bool						   getSlotCreatedFlag ();
	   void						   setSlotCreatedFlag (bool slotCreatedFlag);
	   WaveNode					   *getWaveNodePointer ();
	   void						   setWaveNodePointer (WaveNode *pWaveNode);
	   

    // Now the data members

    private :
    	
		WaveSlotLocalManagedObject *m_WaveSlotLocalManagedObjectPointer;
		WaveNode				   *m_waveNodePointer;
		UI32 						m_slotNumber;
		UI32 						m_hwType;
		LocationId 					m_locationId;
		bool						m_slotCreatedFlag;

    protected :
    public :
};

}

#endif // CLUSTERLOCALMESSAGINGCONTEXT_H
