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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef VCSNODESYSTEMEVENTS_H
#define VCSNODESYSTEMEVENTS_H

#include "Framework/Messaging/Local/PrismEvent.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeSlotOnlineEvent  : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeSlotOnlineEvent  ();
                VcsNodeSlotOnlineEvent (UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version);
               ~VcsNodeSlotOnlineEvent  ();

        UI32    getSlotNumber                () const;
        void    setSlotNumber                (const UI32 &slotNumber);
        UI32    getObjectType                () const;
        void    setObjectType                (const UI32 &objectType);
        UI32    getHwType                    () const;
        void    setHwType                    (const UI32 &hwType);
        UI32    getHwId                      () const;
        void    setHwId                      (const UI32 &hwId);
        UI32    getSlotState                 () const;
        void    setSlotState                 (const UI32 &slotState);
        UI32    getReason                    () const;
        void    setReason                    (const UI32 &reason);
        UI32    getVersion                   () const;
        void    setVersion                   (const UI32 &version);
		UI32    getEventId () const;
		void    setEventId (const UI32 &eventId);

    // Now the data members

    private :
        UI32    m_slotNumber;
        UI32    m_objectType; // blade function type
        UI32    m_hwType;   // blade type
        UI32    m_hwId;
        UI32    m_slotState;
        UI32    m_reason;
        UI32    m_version;
		
		UI32   m_eventId;
    protected :
    public :
};

class VcsNodeSlotOfflineEvent  : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeSlotOfflineEvent  ();
                VcsNodeSlotOfflineEvent (UI32 &slotNumber, UI32 &hwType, UI32 &slotState);
               ~VcsNodeSlotOfflineEvent  ();

        UI32    getSlotNumber            () const;
        void    setSlotNumber            (const UI32 &slotNumber);
		
		UI32 	getHwType() const;
		void 	setHwType (const UI32 &hwType);
        UI32    getSlotState             () const;
        void    setSlotState             (const UI32 &slotState);
		UI32    getEventId () const;
		void    setEventId (const UI32 &eventId);

    // Now the data members

    private :
        UI32    m_slotNumber;
		UI32 	m_hwType;
        UI32    m_slotState;
		
		UI32   m_eventId;
    protected :
    public :
};


class VcsNodeSwitchReadyToAcceptCommandsEvent : public PrismEvent
{
    private :
    protected :
		void	 setupAttributesForSerialization ();
		
    public :
                VcsNodeSwitchReadyToAcceptCommandsEvent ();
               ~VcsNodeSwitchReadyToAcceptCommandsEvent ();
			   
	   UI32    getEventId () const;
	   void    setEventId (const UI32 &eventId);

    // Now the data members

    private :
    
		UI32   m_eventId;
    protected :
    public :
};

class VcsNodeSwitchActivationCompleteEvent : public PrismEvent
{
    private :
    protected :
		void	setupAttributesForSerialization ();
    public :
                VcsNodeSwitchActivationCompleteEvent ();
               ~VcsNodeSwitchActivationCompleteEvent ();

		UI32    getEventId () const;
       void    setEventId (const UI32 &eventId);

    // Now the data members

    private :
		UI32   m_eventId;
    protected :
    public :
};


}

#endif // VCSNODESYSTEMEVENTS_H
