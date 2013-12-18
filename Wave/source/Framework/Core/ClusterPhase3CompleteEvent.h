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
 *   Author : Himanshu Varshney                                              *
 ***************************************************************************/

#ifndef CLUSTERPHASE3COMPLETEEVENT_H
#define CLUSTERPHASE3COMPLETEEVENT_H
#include "Framework/Messaging/Local/PrismEvent.h"

namespace WaveNs
{

class ClusterPhase3CompleteEvent : public PrismEvent
{
    private :

    protected :
        virtual void setupAttributesForSerialization ();

    public :
                 ClusterPhase3CompleteEvent ();
        virtual ~ClusterPhase3CompleteEvent ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // CLUSTERPHASE3COMPLETEEVENT_H
