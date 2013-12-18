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

#ifndef VCSNODEFABOSDOMAINEVENTS_H
#define VCSNODEFABOSDOMAINEVENTS_H

#include "Framework/Messaging/Local/PrismEvent.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeFabosDomainValidEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeFabosDomainValidEvent ();
               ~VcsNodeFabosDomainValidEvent ();

        UI32    getDomainId               () const;
        void    setDomainId               (const UI32 &domainId);
        UI32    getPrincipalDomainId      () const;
        void    setPrincipalDomainId      (const UI32 &domainId);
		UI32    getEventId () const;
		void	setEventId (const UI32 &eventId);

    // Now the data members

    private :
        UI32   m_domainId;
        UI32   m_principalDomainId;
		UI32   m_eventId;

    protected :
    public :
};

class VcsNodeFabosDomainInvalidEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeFabosDomainInvalidEvent ();
               ~VcsNodeFabosDomainInvalidEvent ();

        UI32    getDomainId               () const;
        void    setDomainId               (const UI32 &domainId);
		UI32    getEventId () const;
		void	setEventId (const UI32 &eventId);

    // Now the data members

    private :
    
        UI32   m_domainId;
		UI32   m_eventId;

    protected :
    public :
};

class VcsNodeFabosDomainCapabilitiesAvailableEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeFabosDomainCapabilitiesAvailableEvent ();
               ~VcsNodeFabosDomainCapabilitiesAvailableEvent ();

        UI32    getDomainId               () const;
        void    setDomainId               (const UI32 &domainId);
		UI32    getEventId () const;
		void	setEventId (const UI32 &eventId);

    // Now the data members

    private :
    
        UI32   m_domainId;
		UI32   m_eventId;

    protected :
    public :
};

class VcsNodeFabosDomainReachableEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeFabosDomainReachableEvent ();
               ~VcsNodeFabosDomainReachableEvent ();

        UI32    getDomainId               () const;
        void    setDomainId               (const UI32 &domainId);
		UI32    getEventId () const;
		void	setEventId (const UI32 &eventId);

    // Now the data members

    private :
    
        UI32   m_domainId;
		UI32   m_eventId;

    protected :
    public :
};

class VcsNodeFabosDomainUnreachableEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeFabosDomainUnreachableEvent ();
               ~VcsNodeFabosDomainUnreachableEvent ();

        UI32    getDomainId               () const;
        void    setDomainId               (const UI32 &domainId);
		UI32    getEventId () const;
		void	setEventId (const UI32 &eventId);

    // Now the data members

    private :
    
        UI32   m_domainId;
		UI32   m_eventId;

    protected :
    public :
};

class VcsNodeFabosSwitchOfflineEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeFabosSwitchOfflineEvent ();
               ~VcsNodeFabosSwitchOfflineEvent ();
	   UI32    getEventId () const;
	   void    setEventId (const UI32 &eventId);

    // Now the data members

    private :
    
		UI32   m_eventId;

    protected :
    public :
};

class VcsNodeFabosZoneChangedEvent : public PrismEvent
{
    private :
    protected :

    public :
                VcsNodeFabosZoneChangedEvent ();
               ~VcsNodeFabosZoneChangedEvent ();
	   UI32    getEventId () const;
	   void    setEventId (const UI32 &eventId);

    // Now the data members

    private :
    
		UI32   m_eventId;

    protected :
    public :
};

class VcsNodeFabosZoneAbortedEvent : public PrismEvent
{
    private :
    protected :

    public :
                VcsNodeFabosZoneAbortedEvent ();
               ~VcsNodeFabosZoneAbortedEvent ();
	   UI32    getEventId () const;
	   void    setEventId (const UI32 &eventId);

    // Now the data members

    private :

		UI32   m_eventId;

    protected :
    public :
};

}

#endif // VCSNODEFABOSDOMAINEVENTS_H
