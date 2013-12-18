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

#ifndef WAVELOCALMANAGEDOBJECTBASE_H
#define WAVELOCALMANAGEDOBJECTBASE_H

#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/Types/Types.h"

namespace WaveNs
{

class WaveManagedObject;

class WaveLocalManagedObjectBase
{
    private :
    protected :
        void setupAttributesForPersistence  ();
        void setupAttributesForCreate       ();

        void setOwnerWaveNodeObjectId       (const ObjectId &ownerWaveNodeObjectId);

    public :
                           WaveLocalManagedObjectBase (WaveManagedObject *pWaveManagedObject);
        virtual           ~WaveLocalManagedObjectBase ();

        static  string      getClassName                ();
                ObjectId    getOwnerWaveNodeObjectId    () const ;
                LocationId  getOwnerLocationId          () const ;

                UI32        getAssociatedSlot           () const ;
                void        associateWithSlot           (const UI32 &slotNumber, const LocationId &locationId = 0);

                LocationId  getAssociatedLocationId     () const ;
                void        associateWithLocationId     (const LocationId &locationId, const UI32 &slotNumber = WAVE_INVALID_SLOT);

                ObjectId    getWaveSlotObjectId         () const;
                void        setWaveSlotObjectId         (const ObjectId &waveSlotObjectId);


    // Now the data members
    private :
        WaveManagedObject *m_pWaveManagedObject;
        ObjectId           m_ownerWaveNodeObjectId;
		ObjectId		   m_waveSlotLocalManagedObjectObjectId;

    protected:
    public :
};

}

#endif // WAVELOCALMANAGEDOBJECTBASE_H
