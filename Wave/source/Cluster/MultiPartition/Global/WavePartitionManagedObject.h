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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jay Iyer                                                     *
 *                                                                         *
 ***************************************************************************/

#ifndef WAVEPARTITIONMANAGEDOBJECT_H
#define WAVEPARTITIONMANAGEDOBJECT_H

#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Types/Types.h"

namespace WaveNs
{

class WavePartitionManagedObject  : public WaveManagedObject 
{
    private :
    protected :
        void setupAttributesForPersistence  ();
        void setupAttributesForCreate       ();

    public :
                           WavePartitionManagedObject (WaveObjectManager *pWaveObjectManager);
        virtual           ~WavePartitionManagedObject ();

        static  string      getClassName                ();

                bool        isWaveNodeMappedToPartition(ObjectId &objId);
                void        addWaveNodeToPartition(ObjectId &objId);
                UI32        removeWaveNodeFromPartition(ObjectId &objId);

                void        setPartitionName(const string &name);
                string      getPartitionName(void);

    // Now the data members
    private :
        vector<ObjectId>                m_waveNodeObjectIdList; 
        string                          m_partitionName; 

    protected:
    public :
};

}

#endif // WAVEPARTITIONMANAGEDOBJECT_H
