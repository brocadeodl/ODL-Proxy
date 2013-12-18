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
 *   Description: This file implements the configuration intent repository
 *                which stores configuration intents as serialized messages.
 *                A configuration intent can be categorized as any plug-in
 *                handled message that triggers hardware programming and a
 *                change in the wave configuration database.  These
 *                configuration intents temporarily stored on the HA Peer
 *                and used to replay the last configuration if there is an
 *                HA failover while a configuration was in progress.  The
 *                configuration intents are intercepted and stored on the
 *                HA peer prior to the plug-in handling the message and
 *                removed from the HA peer once the configuration has 
 *                completed processing from the plug-in.
 * 
 *                The repository vector stores config intent message id's
 *                in order.
 *
 *                The repository map maintains the mapping of message id's
 *                to the actual serialized config intent message.
 *
 *   Author : Brian Adaniya 
 *   Date   : 10/23/2012                                           
 ***************************************************************************/

#include "Framework/Core/WaveConfigurationIntentRepository.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

vector <UI32>           WaveConfigurationIntentRepository::s_waveConfigurationIntentRepositoryVector;
map <UI32, string>      WaveConfigurationIntentRepository::s_waveConfigurationIntentRepositoryMap;
PrismMutex              WaveConfigurationIntentRepository::s_waveConfigurationIntentRepositoryMutex;

void WaveConfigurationIntentRepository::addConfigurationIntent (const UI32 &configurationIntentMessageId, const string &configurationIntentSerializedMessage)
{
    s_waveConfigurationIntentRepositoryMutex.lock ();

    s_waveConfigurationIntentRepositoryVector.push_back (configurationIntentMessageId);
    s_waveConfigurationIntentRepositoryMap[configurationIntentMessageId] = configurationIntentSerializedMessage;

    s_waveConfigurationIntentRepositoryMutex.unlock ();
}

string WaveConfigurationIntentRepository::getConfigurationIntent (const UI32 &configurationIntentMessageId)
{
    string                          serializedConfigurationIntentMessage    = "";
    map <UI32, string>::iterator    element;

    s_waveConfigurationIntentRepositoryMutex.lock ();

    element = s_waveConfigurationIntentRepositoryMap.find (configurationIntentMessageId);

    if (s_waveConfigurationIntentRepositoryMap.end () != element)
    {
        serializedConfigurationIntentMessage = (*element).second;
    }

    s_waveConfigurationIntentRepositoryMutex.unlock ();

    return (serializedConfigurationIntentMessage);
}

string WaveConfigurationIntentRepository::getLastConfigurationIntent ()
{
    string                          serializedConfigurationIntentMessage    = "";
    map <UI32, string>::iterator    element;

    s_waveConfigurationIntentRepositoryMutex.lock ();

    if (0 == s_waveConfigurationIntentRepositoryVector.size ())
    {
		s_waveConfigurationIntentRepositoryMutex.unlock ();
        return (serializedConfigurationIntentMessage);
    }

    element = s_waveConfigurationIntentRepositoryMap.find (s_waveConfigurationIntentRepositoryVector.back ());

    if (s_waveConfigurationIntentRepositoryMap.end () != element)
    {
        serializedConfigurationIntentMessage = (*element).second;
    }

    s_waveConfigurationIntentRepositoryMutex.unlock ();

    return (serializedConfigurationIntentMessage);
}

void WaveConfigurationIntentRepository::removeConfigurationIntent (const UI32 &configurationIntentMessageId)
{
    map <UI32, string>::iterator    mapElement;
    vector <UI32>::iterator         vectorElement;

    s_waveConfigurationIntentRepositoryMutex.lock ();

    // Erase config intent message mapping in map repository 

    mapElement = s_waveConfigurationIntentRepositoryMap.find (configurationIntentMessageId);    

    if (s_waveConfigurationIntentRepositoryMap.end () != mapElement)
    {
        s_waveConfigurationIntentRepositoryMap.erase (mapElement);
    }

    // Erase in-order config intent message id in vector repository
    
    for (vectorElement = s_waveConfigurationIntentRepositoryVector.begin (); vectorElement != s_waveConfigurationIntentRepositoryVector.end (); vectorElement++)
    {
        if (configurationIntentMessageId == (*vectorElement))
        {
            s_waveConfigurationIntentRepositoryVector.erase (vectorElement);

            break;
        }
    }

    s_waveConfigurationIntentRepositoryMutex.unlock ();
}

string WaveConfigurationIntentRepository::getAndRemoveConfigurationIntent (const UI32 &configurationIntentMessageId)
{
    string                          serializedConfigurationIntentMessage    = "";
    map <UI32, string>::iterator    mapElement;
    vector <UI32>::iterator         vectorElement;

    s_waveConfigurationIntentRepositoryMutex.lock ();

    // Get and erase config intent message mapping in map repository 

    mapElement = s_waveConfigurationIntentRepositoryMap.find (configurationIntentMessageId);

    if (s_waveConfigurationIntentRepositoryMap.end () != mapElement)
    {
        serializedConfigurationIntentMessage = (*mapElement).second;
        s_waveConfigurationIntentRepositoryMap.erase (mapElement);
    }

    // Erase in-order config intent message id in vector repository

    for (vectorElement = s_waveConfigurationIntentRepositoryVector.begin (); vectorElement != s_waveConfigurationIntentRepositoryVector.end (); vectorElement++)
    {
        if (configurationIntentMessageId == (*vectorElement))
        {
            s_waveConfigurationIntentRepositoryVector.erase (vectorElement);

            break;
        }
    }

    s_waveConfigurationIntentRepositoryMutex.unlock ();

    return (serializedConfigurationIntentMessage);
}

void WaveConfigurationIntentRepository::clearConfigurationIntentRepository ()
{
    s_waveConfigurationIntentRepositoryMutex.lock ();

    s_waveConfigurationIntentRepositoryVector.clear ();
    s_waveConfigurationIntentRepositoryMap.clear ();

    s_waveConfigurationIntentRepositoryMutex.unlock ();
}

UI32 WaveConfigurationIntentRepository::getConfigurationIntentRepositorySize ()
{
    UI32 configurationIntentRepositoryVectorSize    = 0;
    UI32 configurationIntentRepositoryMapSize       = 0;
    UI32 configurationIntentRepositorySize          = 0;

    s_waveConfigurationIntentRepositoryMutex.lock ();

    configurationIntentRepositoryVectorSize = s_waveConfigurationIntentRepositoryVector.size ();
    configurationIntentRepositoryMapSize    = s_waveConfigurationIntentRepositoryMap.size ();

    s_waveConfigurationIntentRepositoryMutex.unlock ();

    if (configurationIntentRepositoryVectorSize != configurationIntentRepositoryMapSize)
    {
        trace (TRACE_LEVEL_FATAL, string ("WaveConfigurationIntentRepository::getConfigurationIntentRepositorySize : Configuration intent size for vector and map mismatch. vector size : ") + configurationIntentRepositoryVectorSize + ", map size : " + configurationIntentRepositoryMapSize + ". This should never happen!");
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        configurationIntentRepositorySize = configurationIntentRepositoryVectorSize;
    }

    return (configurationIntentRepositorySize);
}

void WaveConfigurationIntentRepository::getConfigurationIntentRepository (vector<UI32> &configurationIntentRepositoryVector, map<UI32, string> &configurationIntentRepositoryMap)
{
    configurationIntentRepositoryVector.clear ();
    configurationIntentRepositoryMap.clear ();

    s_waveConfigurationIntentRepositoryMutex.lock ();

    configurationIntentRepositoryVector = s_waveConfigurationIntentRepositoryVector;
    configurationIntentRepositoryMap    = s_waveConfigurationIntentRepositoryMap;

    s_waveConfigurationIntentRepositoryMutex.unlock ();
}

}
