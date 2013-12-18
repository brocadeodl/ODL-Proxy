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

#ifndef VCSNODEFABOSSYSTEMEVENTS_H
#define VCSNODEFABOSSYSTEMEVENTS_H

#include "Framework/Messaging/Local/PrismEvent.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeFabosSwitchReadyToAcceptCommandsEvent : public PrismEvent
{
    private :
    protected :
		
       virtual void setupAttributesForSerialization ();
    public :
                VcsNodeFabosSwitchReadyToAcceptCommandsEvent ();
               ~VcsNodeFabosSwitchReadyToAcceptCommandsEvent ();
	   UI32    getEventId () const;
	   void    setEventId (const UI32 &eventId);

    // Now the data members

    private :

		UI32   m_eventId;
	
    protected :
    public :
};

class VcsNodeFabosSwitchActivationCompleteEvent : public PrismEvent
{
    private :
    protected :

       virtual void setupAttributesForSerialization ();
    public :
                VcsNodeFabosSwitchActivationCompleteEvent ();
               ~VcsNodeFabosSwitchActivationCompleteEvent ();
       UI32    getEventId () const;
       void    setEventId (const UI32 &eventId);

    // Now the data members

    private :

        UI32   m_eventId;

    protected :
    public :
};

class VcsNodeFabosSyncStartEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeFabosSyncStartEvent ();
                VcsNodeFabosSyncStartEvent (string &peerHaIpAddress, SI32 &peerHaIpPort);
               ~VcsNodeFabosSyncStartEvent ();

        string  getPeerHaIpAddress         () const;
        void    setPeerHaIpAddress         (const string &peerHaIpAddress);
        SI32    getPeerHaIpPort            () const;
        void    setPeerHaIpPort            (const SI32 &peerHaIpPort);
		bool    getIsChassisFirmwaredownload () const;
		void	setIsChassisFirmwaredownload (const bool &isChassisFirmwaredownload);
		UI32    getEventId                 () const;
		void	setEventId                 (const UI32 &eventId);

    // Now the data members

    private :
        string m_peerHaIpAddress;
        SI32   m_peerHaIpPort;
		bool   m_isChassisFirmwaredownload;
		UI32   m_eventId;

    protected :
    public :
};

class VcsNodeFabosServiceReadyEvent : public PrismEvent
{
    private :
    protected :

       virtual void setupAttributesForSerialization ();
    public :
                VcsNodeFabosServiceReadyEvent ();
               ~VcsNodeFabosServiceReadyEvent ();
       ResourceId   getRecoveryType () const;
       void    		setRecoveryType (const ResourceId &recoveryType);
	   UI32    		getEventId      () const;
       void    		setEventId      (const UI32 &eventId);

    // Now the data members

    private :

        ResourceId  m_recoveryType;
		UI32   		m_eventId;

    protected :
    public :
};

class VcsNodeFabosLinecardReadyEvent : public PrismEvent
{
    private :
    protected :

       virtual void setupAttributesForSerialization ();
    public :
                VcsNodeFabosLinecardReadyEvent ();
                VcsNodeFabosLinecardReadyEvent (const UI32 &slotId, const ResourceId &recoveryType, const UI32 &eventId);
               ~VcsNodeFabosLinecardReadyEvent ();
       ResourceId    getRecoveryType 	() const;
       void    setRecoveryType 	(const ResourceId &recoveryType);
       UI32    getSlotId 		() const;
       void    setSlotId 		(const UI32 &slotId);
	   UI32    getEventId      	() const;
       void    setEventId      	(const UI32 &eventId);
    // Now the data members

    private :

        ResourceId   m_recoveryType;
		UI32   m_slotId;
		UI32   m_eventId;

    protected :
    public :
};


class VcsNodeFabosSystemReadyEvent : public PrismEvent
{
    private :
    protected :

       virtual void setupAttributesForSerialization ();
    public :
                VcsNodeFabosSystemReadyEvent ();
               ~VcsNodeFabosSystemReadyEvent ();
       ResourceId    getRecoveryType () const;
       void    setRecoveryType (const ResourceId &recoveryType);
	   UI32    getEventId       () const;
       void    setEventId       (const UI32 &eventId); 
    // Now the data members

    private :

        ResourceId   m_recoveryType;
		UI32   m_eventId;
    protected :
    public :
};

}

#endif // VCSNODEFABOSDOMAINEVENTS_H
