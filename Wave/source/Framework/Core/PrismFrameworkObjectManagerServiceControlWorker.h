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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMFRAMEWORKOBJECTMANAGERSERVICECONTROLWORKER_H
#define PRISMFRAMEWORKOBJECTMANAGERSERVICECONTROLWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class FrameworkObjectManagerServiceControlListMessage;
class FrameworkObjectManagerServiceSetCpuAffinityMessage;

class PrismFrameworkObjectManagerServiceControlWorker : public WaveWorker
{
    private :
        virtual PrismMessage *createMessageInstance               (const UI32 &operationCode);

                void          serviceControlListMessageHandler    (FrameworkObjectManagerServiceControlListMessage *pFrameworkObjectManagerServiceControlListMessage);
                void          serviceSetCpuAffinityMessageHandler (FrameworkObjectManagerServiceSetCpuAffinityMessage *pFrameworkObjectManagerServiceSetCpuAffinityMessage);

    protected :
    public :
                 PrismFrameworkObjectManagerServiceControlWorker (WaveObjectManager *pWaveObjectManager);
        virtual ~PrismFrameworkObjectManagerServiceControlWorker ();

    // Now the Data Members

    private :
    protected :
    public :
};

}

#endif // PRISMFRAMEWORKOBJECTMANAGERSERVICECONTROLWORKER_H
