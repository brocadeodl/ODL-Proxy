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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMMESSAGEFACTORY_H
#define PRISMMESSAGEFACTORY_H

#include "Framework/Types/Types.h"

namespace WaveNs
{

class PrismMessage;
class WaveObjectManager;

class PrismMessageFactory
{
    private :
    protected :
    public :
        static PrismMessage      *getMessageInstance                    (const PrismServiceId &serviceId, const UI32 &opcode);
        static WaveObjectManager *getWaveObjectManagerForMessage        (const PrismServiceId &serviceId, const UI32 &opcode);
        static WaveObjectManager *getWaveObjectManagerForEvent          (const PrismServiceId &serviceId, const UI32 &eventOpcode);
        static WaveObjectManager *getWaveObjectManagerForEventListener  (const LocationId &locationId, const PrismServiceId &serviceId, const UI32 &eventOpcode);
        static PrismMessage      *getManagementInterfaceMessageInstance (const string &waveClientName, const PrismServiceId &serviceId, const UI32 &opcode);

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // PRISMMESSAGEFACTORY_H
