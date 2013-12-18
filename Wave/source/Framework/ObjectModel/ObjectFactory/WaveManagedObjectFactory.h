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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMMANAGEDOBJECTFACTORY_H
#define PRISMMANAGEDOBJECTFACTORY_H

#include "Framework/Types/Types.h"

#include <string>

using namespace std;

namespace WaveNs
{

class WaveManagedObject;
class WaveObjectManager;

class WaveManagedObjectFactory
{
    private :
    protected :
    public :
        static WaveManagedObject *getWaveManagedObjectInstance         (const PrismServiceId &prismServiceId, const  string &className);
        static WaveObjectManager *getWaveObjectManagerForManagedObject (const PrismServiceId &prismServiceId, const  string &className);

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // PRISMMANAGEDOBJECTFACTORY_H
