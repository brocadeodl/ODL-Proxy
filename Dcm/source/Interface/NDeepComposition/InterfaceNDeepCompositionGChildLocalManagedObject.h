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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef INTERFACENDEEPCOMPOSITIONGCHILDLOCALMANAGEDOBJECT_H
#define INTERFACENDEEPCOMPOSITIONGCHILDLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
//#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
//#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

class InterfaceNDeepCompositionGChildLocalManagedObject : public DcmLocalManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
                        InterfaceNDeepCompositionGChildLocalManagedObject    (WaveObjectManager *pWaveObjectManager);
                        InterfaceNDeepCompositionGChildLocalManagedObject    (WaveObjectManager *pWaveObjectManager, SI32 &id, bool allow, SI32 &xid);
        virtual        ~InterfaceNDeepCompositionGChildLocalManagedObject    ();

        static  string  getClassName                   ();

				SI32    getId                       () const;
                void    setId                       (const SI32 &id);
				SI32    getXId                       () const;
                void    setXId                       (const SI32 &xid);


    // Now the Data Members

    private :
        SI32          m_vlan_id;
        SI32          m_x_id;

    protected :
    public :
};
}

#endif // INTERFACENDEEPCOMPOSITIONGCHILDLOCALMANAGEDOBJECT_H
