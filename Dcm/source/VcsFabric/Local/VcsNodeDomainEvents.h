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

#ifndef VCSNODEDOMAINEVENTS_H
#define VCSNODEDOMAINEVENTS_H

#include "Framework/Messaging/Local/PrismEvent.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeDomainValidEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeDomainValidEvent ();
               ~VcsNodeDomainValidEvent ();

        UI32    getDomainId               () const;
        void    setDomainId               (const UI32 &domainId);
        UI32    getPrincipalDomainId      () const;
        void    setPrincipalDomainId      (const UI32 &domainId);
		UI32    getEventId () const;
		void    setEventId (const UI32 &eventId);

    // Now the data members

    private :
        UI32   m_domainId;
        UI32   m_principalDomainId;
		UI32   m_eventId;

    protected :
    public :
};

class VcsNodeDomainInvalidEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeDomainInvalidEvent ();
               ~VcsNodeDomainInvalidEvent ();

        UI32    getDomainId               () const;
        void    setDomainId               (const UI32 &domainId);
		UI32    getEventId () const;
		void    setEventId (const UI32 &eventId);

    // Now the data members

    private :
        UI32   m_domainId;
		UI32   m_eventId;

    protected :
    public :
};

class VcsNodeDomainCapabilitiesAvailableEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeDomainCapabilitiesAvailableEvent ();
               ~VcsNodeDomainCapabilitiesAvailableEvent ();

        UI32    getDomainId               () const;
        void    setDomainId               (const UI32 &domainId);
		UI32    getEventId () const;
		void    setEventId (const UI32 &eventId);

    // Now the data members

    private :
        UI32   m_domainId;
		UI32   m_eventId;

    protected :
    public :
};

class VcsNodeDomainReachableEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeDomainReachableEvent ();
               ~VcsNodeDomainReachableEvent ();

        UI32    getDomainId               () const;
        void    setDomainId               (const UI32 &domainId);
		UI32    getEventId () const;
		void    setEventId (const UI32 &eventId);

    // Now the data members

    private :
        UI32   m_domainId;
		UI32   m_eventId;

    protected :
    public :
};

class VcsNodeDomainUnreachableEvent : public PrismEvent
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                VcsNodeDomainUnreachableEvent ();
               ~VcsNodeDomainUnreachableEvent ();

        UI32    getDomainId               () const;
        void    setDomainId               (const UI32 &domainId);
		UI32    getEventId () const;
		void    setEventId (const UI32 &eventId);

    // Now the data members

    private :
        UI32   m_domainId;
		UI32   m_eventId;

    protected :
    public :
};

class VcsNodeSwitchOfflineEvent : public PrismEvent
{
    private :
    protected :
		void	setupAttributesForSerialization ();
		
    public :
                VcsNodeSwitchOfflineEvent ();
               ~VcsNodeSwitchOfflineEvent ();
			   
	
	UI32	getEventId () const;
	void	setEventId (const UI32 &eventId);

    // Now the data members

    private :
    
		UI32   m_eventId;
    protected :
    public :
};

class VcsNodeZoneChangedEvent : public PrismEvent
{
    private :
    protected :
    public :
                VcsNodeZoneChangedEvent ();
               ~VcsNodeZoneChangedEvent ();
			   
		UI32    getEventId () const;
		void    setEventId (const UI32 &eventId);

    // Now the data members

    private :
    
		UI32   m_eventId;
    protected :
    public :
};

class VcsNodeZoneAbortedEvent : public PrismEvent
{
    private :
    protected :
    public :
                VcsNodeZoneAbortedEvent ();
               ~VcsNodeZoneAbortedEvent ();
			   
	   UI32    getEventId () const;
	   void    setEventId (const UI32 &eventId);

    // Now the data members

    private :
    
		UI32   m_eventId;
    protected :
    public :
};

}

#endif // VCSNODEDOMAINEVENTS_H
