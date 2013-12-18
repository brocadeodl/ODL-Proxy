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

 /**************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh			                                   *
 ***************************************************************************/

#ifndef CLUSTERLOCALSLOTONLINEMESSAGE_H
#define CLUSTERLOCALSLOTONLINEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Cluster/Local/ClusterLocalTypes.h"

using namespace WaveNs;

namespace WaveNs
{

class ClusterLocalSlotOnlineMessage : public PrismMessage
{
    private :
        			void 	setupAttributesForSerialization ();

    protected :
    public :
                    ClusterLocalSlotOnlineMessage ();
                    ClusterLocalSlotOnlineMessage (const UI32 &slotNumber, const UI32 &objectType, const UI32 &hwType, const UI32 &hwId, const UI32 &slotState, const UI32 &reason, const UI32 &version);
    virtual        ~ClusterLocalSlotOnlineMessage ();

					 UI32	  getSlotNumber		     () const;
					 void	  setSlotNumber		     (const UI32 &slotNumber);
					 UI32	  getObjectType			 () const;
					 void	  setObjectType			 (const UI32 &objectType);
					 UI32	  getHwType				 () const;
					 void	  setHwType				 (const UI32 &hwType);
					 UI32	  getHwId				 () const;
					 void	  setHwId				 (const UI32 &hwId);
					 UI32	  getSlotState			 () const;
					 void	  setSlotState			 (const UI32 &slotState);
					 UI32	  getReason				 () const;
					 void	  setReason				 (const UI32 &reason);
					 UI32	  getVersion 			 () const;
					 void	  setVersion 			 (const UI32 &version);
					 ObjectId getWaveNodeObjectId 	 () const;
					 void	  setWaveNodeObjectId 	 (const ObjectId &waveNodeObjectId);
					 ObjectId getWaveSlotObjectId 	 () const;
					 void	  setWaveSlotObjectId 	 (const ObjectId &waveSlotObjectId);
                     UI32     getIsDummySlotUpdation   () const;
                     void     setIsDummySlotUpdation   (const bool isDummySlotUpdation);                     
					
					 // Now the data members
					
	private :
						 UI32	  m_slotNumber;
						 UI32	  m_objectType; // blade function type
						 UI32	  m_hwType;	 // blade type
						 UI32	  m_hwId;
						 UI32	  m_slotState;
						 UI32	  m_reason;
						 UI32	  m_version;
						 ObjectId m_waveNodeObjectId;
						 ObjectId m_waveSlotObjectId;
                         bool     m_isDummySlotUpdation;

    protected :
    public :
};

}

#endif // CLUSTERLOCALSLOTONLINEMESSAGE_H


