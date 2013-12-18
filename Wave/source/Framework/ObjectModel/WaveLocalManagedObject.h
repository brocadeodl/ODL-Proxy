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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVELOCALMANAGEDOBJECT_H
#define WAVELOCALMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveLocalManagedObjectBase.h"

namespace WaveNs
{

class WaveLocalManagedObject : virtual public WaveManagedObject, virtual public WaveLocalManagedObjectBase
{
    private :
    protected :
        void setupAttributesForPersistence ();
        void setupAttributesForCreate      ();

    public :
                       WaveLocalManagedObject (WaveObjectManager *pWaveObjectManager);
        virtual       ~WaveLocalManagedObject ();

        static string  getClassName           ();
		void addWaveSlotLMOObjectId (const ObjectId &waveSlotObjectId);
		vector<ObjectId> getWaveSlotLMOObjectIds ();
		ResourceId removeWaveSlotLMOObjectId (const ObjectId &waveSlotObjectId);

    // Now the data members

    private :
    	vector<ObjectId> m_waveSlotLMOObjectIds;
    protected:
    public :
};

}

#endif // WAVELOCALMANAGEDOBJECT_H
