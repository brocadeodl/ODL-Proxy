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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#ifndef VCSSLOTLOCALMANAGEDOBJECT_H
#define VCSSLOTLOCALMANAGEDOBJECT_H

#include "Cluster/Local/WaveSlotLocalManagedObjectBase.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{
class VcsFabricLocalObjectManager;

class VcsSlotLocalManagedObject : virtual public DcmLocalManagedObject, virtual public WaveSlotLocalManagedObjectBase
{
    private :
        VcsSlotLocalManagedObject ();

    protected :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    public :
        VcsSlotLocalManagedObject (VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager);
        VcsSlotLocalManagedObject (VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager, string &linecardName, UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version);
		
        virtual        ~VcsSlotLocalManagedObject ();

        static  string  getClassName        ();

                UI32    getLinecardType () const;
                void    setLinecardType (const UI32 &linecardType);
                UI32    getLinecardMode () const;
                void    setLinecardMode (const UI32 &linecardMode);
                void    setWaveNodeObjectId (const ObjectId &waveNodeObjectId);

    // Now the data members

    private :
        UI32 m_linecardType;
        UI32 m_linecardMode;
    protected :
    public :
};

}

#endif // VCSSLOTLOCALMANAGEDOBJECT_H
