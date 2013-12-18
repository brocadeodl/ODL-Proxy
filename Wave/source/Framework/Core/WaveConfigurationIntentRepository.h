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
 *   Description: This file defines the configuration intent repository
 *                which stores configuration intents as serialized messages.
 *                A configuration intent can be categorized as any plug-in
 *                handled message that triggers hardware programming and a
 *                change in the wave configuration database.  These
 *                configuration intents are temporarily stored on the HA Peer
 *                and used to replay the last configuration if there is an
 *                HA failover while a configuration was in progress.  The
 *                configuration intents are intercepted and stored on the
 *                HA peer prior to the plug-in handling the message and
 *                removed from the HA peer once the configuration has 
 *                completed processing from the plug-in.
 *
 *   Author : Brian Adaniya 
 *   Date   : 10/23/2012                                           
 ***************************************************************************/

#ifndef WAVECONFIGURATIONINTENTREPOSITORY_H
#define WAVECONFIGURATIONINTENTREPOSITORY_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"
#include <vector>
#include <map>

namespace WaveNs
{

class WaveConfigurationIntentRepository
{
    private :
    protected :
    public :                        
        static  void                                addConfigurationIntent                  (const UI32 &configurationIntentMessageId, const string &configurationIntentSerializedMessage);
        static  string                              getConfigurationIntent                  (const UI32 &configurationIntentMessageId);
        static  string                              getLastConfigurationIntent              ();
        static  void                                removeConfigurationIntent               (const UI32 &configurationIntentMessageId);
        static  string                              getAndRemoveConfigurationIntent         (const UI32 &configurationIntentMessageId);
        static  void                                clearConfigurationIntentRepository      ();
        static  UI32                                getConfigurationIntentRepositorySize    ();
        static  void                                getConfigurationIntentRepository        (vector<UI32> &configurationIntentRepositoryVector, map<UI32, string> &configurationIntentRepositoryMap);
        
    // Now the data members

    private :
        static  vector <UI32>                       s_waveConfigurationIntentRepositoryVector;
        static  map <UI32, string>                  s_waveConfigurationIntentRepositoryMap;
        static  PrismMutex                          s_waveConfigurationIntentRepositoryMutex;

    protected :
    public :
};

}

#endif // WAVECONFIGURATIONINTENTREPOSITORY_H

