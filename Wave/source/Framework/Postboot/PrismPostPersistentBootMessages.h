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

#ifndef PRISMPOSTPERSISTENTBOOTMESSAGES_H
#define PRISMPOSTPERSISTENTBOOTMESSAGES_H

#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Messaging/Local/PrismMessage.h"

namespace WaveNs
{

class PrismPostPersistenceBootMessage : public PrismMessage
{
    private :
        virtual void           setupAttributesForSerialization();
    protected :
    public :
                               PrismPostPersistenceBootMessage      ();
                               PrismPostPersistenceBootMessage      (bool isPostbootRequired, bool executeConfigReplaySequence, bool executeNodeReadySequence, bool isBannerOutputToClientsRequired);
                               PrismPostPersistenceBootMessage      (const UI32 &event, const UI32 &parameter, const ResourceId &recoveryType);
                virtual       ~PrismPostPersistenceBootMessage      ();
                bool           getExecuteConfigReplaySequence       () const;
                void           setExecuteConfigReplaySequence       (bool executeConfigReplaySequence);
                bool           getExecuteNodeReadySequence          () const;
                void           setExecuteNodeReadySequence          (bool executeNodeReadySequence);
                bool           getIsPostbootRequired                () const;
                void           setIsPostbootRequired                (bool isPostbootRequired);
                bool           getIsBannerOutputToClientsRequired   () const;
                void           setIsBannerOutputToClientsRequired   (bool isBannerOutputToClientsRequired);
		void                   setEventId(const UI32 &eventId);
        UI32                   getEventId() const;
		void                   setParameter(const UI32 &parameter);
        UI32                   getParameter() const;
        void                   setRecoveryType(const ResourceId &recoveryType);
        ResourceId             getRecoveryType() const;

    // Now the data members
    private :
                bool           m_isPostbootRequired;
                bool           m_executeConfigReplaySequence;
                bool           m_executeNodeReadySequence;
                bool           m_isBannerOutputToClientsRequired;
                UI32           m_eventId;
                UI32           m_parameter;
                ResourceId     m_recoveryType;

    protected :
    public :
};

class PrismPostPersistenceBootServiceAndWaveSlotsMessage : public PrismMessage
{
    private :
        virtual void            setupAttributesForSerialization                     ();
    protected :
    public :
                                PrismPostPersistenceBootServiceAndWaveSlotsMessage  ();
        virtual                ~PrismPostPersistenceBootServiceAndWaveSlotsMessage  ();

    // Now the data members
    private :
    protected :
    public :
};

}

#endif // PRISMPOSTPERSISTENTBOOTMESSAGES_H
