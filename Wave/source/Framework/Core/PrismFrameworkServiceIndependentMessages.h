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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               
 *   All rights reserved.  
 *   Description: This file defines the prism framework service independent
 *                messages.  These service independent messages are directly
 *                handled in the HaPeerMessageReceiverThread or 
 *                InterLocationMessageReceiverThread.
 *   Author : Brian Adaniya 
 *   Date   : 10/22/2012                                           
 ***************************************************************************/

#ifndef PRISMFRAMEWORKSERVICEINDEPENDENTMESSAGES_H
#define PRISMFRAMEWORKSERVICEINDEPENDENTMESSAGES_H

#include "Framework/Messaging/Local/WaveServiceIndependentMessage.h"

namespace WaveNs
{

typedef enum
{
    SERIALIZED_CONFIGURATION_INTENT_BUFFER = 1
} FrameworkObjectManagerConfigurationIntentBufferId;

class FrameworkObjectManagerStoreConfigurationIntentMessage : public WaveServiceIndependentMessage
{
    private :
    protected :
                void            setupAttributesForSerialization                         ();

    public :
                                FrameworkObjectManagerStoreConfigurationIntentMessage   ();
                                FrameworkObjectManagerStoreConfigurationIntentMessage   (const UI32 &configurationIntentMessageId);
        virtual                ~FrameworkObjectManagerStoreConfigurationIntentMessage   ();

                UI32            getConfigurationIntentMessageId                         () const;
                void            setConfigurationIntentMessageId                         (const UI32 &configurationIntentMessageId);

    // Now the data members
    
    private :
                UI32            m_configurationIntentMessageId;

    protected :
    public :
        
};

class FrameworkObjectManagerRemoveConfigurationIntentMessage : public WaveServiceIndependentMessage
{
    private :
    protected :
                void            setupAttributesForSerialization                         ();

    public :
                                FrameworkObjectManagerRemoveConfigurationIntentMessage  ();
                                FrameworkObjectManagerRemoveConfigurationIntentMessage  (const UI32 &configurationIntentMessageId);
        virtual                ~FrameworkObjectManagerRemoveConfigurationIntentMessage  ();

                UI32            getConfigurationIntentMessageId                         () const;
                void            setConfigurationIntentMessageId                         (const UI32 &configurationIntentMessageId);

    // Now the data members

    private :
                UI32            m_configurationIntentMessageId;

    protected :
    public :

};

}

#endif // PRISMFRAMEWORKSERVICEINDEPENDENTMESSAGES_H
