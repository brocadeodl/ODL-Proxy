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
 *   Author : Jitendra Singh                                               *
 ***************************************************************************/

#ifndef WAVESLOTLOCALMANAGEDOBJECT_H
#define WAVESLOTLOCALMANAGEDOBJECT_H

#include "Cluster/Local/WaveSlotLocalManagedObjectBase.h"
#include "Framework/ObjectModel/WaveLocalManagedObject.h"

namespace WaveNs
{
class ClusterLocalObjectManager;

class WaveSlotLocalManagedObject : virtual public WaveLocalManagedObject, virtual public WaveSlotLocalManagedObjectBase
{
    private :
        WaveSlotLocalManagedObject ();

    protected :
        void setupAttributesForPersistence ();
        void setupAttributesForCreate      ();

    public :
        WaveSlotLocalManagedObject (ClusterLocalObjectManager *pClusterLocalObjectManager);
		WaveSlotLocalManagedObject (ClusterLocalObjectManager *pClusterLocalObjectManager, UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version);
		
        virtual        ~WaveSlotLocalManagedObject ();

        static  string  getClassName        ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // WAVESLOTLOCALMANAGEDOBJECT_H
