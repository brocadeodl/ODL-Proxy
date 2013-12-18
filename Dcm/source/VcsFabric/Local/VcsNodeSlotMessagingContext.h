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
 *   Author : Jitendra Singh                                               *
 ***************************************************************************/

#ifndef VCSNODESLOTMESSAGINGCONTEXT_H
#define VCSNODESLOTMESSAGINGCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "VcsFabric/Local/VcsSlotLocalManagedObject.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "Cluster/Local/WaveSlotLocalManagedObject.h"
#include "dcm/ipc.h"

namespace DcmNs
{
class VcsNodeSlotMessagingContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
            VcsNodeSlotMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);

           ~VcsNodeSlotMessagingContext ();

           PrismMessage *getSlotMessage ();
           void setSlotMessage (PrismMessage *pSlotMessage);
           
           UI32 getSlotNumber   () const;
           void setSlotNumber   (const UI32 slotNumber);
           
           UI32 getHwType       () const;
           void setHwType       (const UI32 hwType);
		   UI32 getRbridgeId	() const;
		   void setRbridgeId	(const UI32 rbridgeId);
		   LocationId getLocationId ();
		   void setLocationId	(const LocationId &locationId);
           UI32 getSlotState    () const;
           void setSlotState    (const UI32 slotState);
           UI32 getHwId         () const;
           void setHwId         (const UI32 hwId);
           UI32 getLinecardMode () const;
           void setLinecardMode (const UI32 linecardMode);
           UI32 getObjectType   () const;
           void setObjectType   (const UI32 objectType);
           UI32 getReason       () const;
           void setReason       (const UI32 reason);
           UI32 getVersion      () const;
           void setVersion      (const UI32 version);
		   
		   WaveSlotLocalManagedObject  *getWaveSlotManagedObjectPointer () const;
		   void 						setWaveSlotManagedObjectPointer (WaveSlotLocalManagedObject *m_pWaveSlotLocalManagedObject);
		   VcsSlotLocalManagedObject  *getVcsSlotManagedObjectPointer () const;
		   void 					   setVcsSlotManagedObjectPointer (VcsSlotLocalManagedObject *m_pVcsSlotLocalManagedObject);
		   VcsNodeLocalManagedObject  *getVcsNodeManagedObjectPointer () const;
		   void 					   setVcsNodeManagedObjectPointer (VcsNodeLocalManagedObject *m_pVcsNodeLocalManagedObject);

           bool                 getSlotCreatedFlag ();
           void                 setSlotCreatedFlag (bool slotCreatedFlag);
           int                  getMessageType ();
           void                 setMessageType (const int &messageType);
           int                  getIscDestination ();
           void                 setIscDestination (int reqh);
		   ObjectId             getWaveNodeObjectId () const;
		   void	                setWaveNodeObjectId (const ObjectId &waveNodeObjectId);
		   ObjectId             getWaveSlotObjectId () const;
		   void	                setWaveSlotObjectId (const ObjectId &waveSlotObjectId);

    // Now the data members

    private :
        PrismMessage   *m_slotMessage;
        UI32            m_slotNumber;
        UI32            m_hwType;
		UI32 			m_rbridgeId;
		LocationId		m_locationId;
        UI32            m_slotState;
        UI32            m_hwId;
        UI32            m_linecardMode;
        UI32            m_objectType;
        UI32            m_reason;
        UI32            m_version;
		WaveSlotLocalManagedObject *m_pWaveSlotLocalManagedObject;
		VcsSlotLocalManagedObject  *m_pVcsSlotLocalManagedObject;
		VcsNodeLocalManagedObject  *m_pVcsNodeLocalManagedObject;
        bool                        m_slotCreatedFlag;    
        int                         m_msgType;
        int                         m_iscDestination;
        ObjectId                    m_waveNodeObjectId;
        ObjectId                    m_waveSlotObjectId;

    protected :
    public :
};
}

#endif // VCSNODESLOTMESSAGINGCONTEXT_H
