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

#ifndef INTERFACENDEEPCOMPOSITIONLOCALMANAGEDOBJECT_H
#define INTERFACENDEEPCOMPOSITIONLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
//#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{
class InterfaceNDeepCompositionChildLocalManagedObject;
class InterfaceNDeepCompositionGChildLocalManagedObject;

class InterfaceNDeepCompositionLocalManagedObject : public DcmLocalManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();
        virtual void loadOperationalData           (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext);

    protected :
    public :
                        InterfaceNDeepCompositionLocalManagedObject    (WaveObjectManager *pWaveObjectManager);
        virtual        ~InterfaceNDeepCompositionLocalManagedObject    ();

        static  string  getClassName                   ();

                vector<WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> > getNDeepChildren () const;
                void           setNDeepChildren             (vector<WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> > children);

                WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> getNDeepChild () const;
                void           setNDeepChild             (WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> child);

    // Now the Data Members

    private :
        // Composition data Fields
        vector<WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> > m_nDeepchildren;
        WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> m_nDeepchild;

    protected :
    public :
};

}

#endif // INTERFACENDEEPCOMPOSITIONLOCALMANAGEDOBJECT_H
