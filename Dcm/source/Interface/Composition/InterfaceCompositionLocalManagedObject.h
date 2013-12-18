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

#ifndef INTERFACECOMPOSITIONCHILDLOCALMANAGEDOBJECT_H
#define INTERFACECOMPOSITIONCHILDLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{
class InterfaceNDeepCompositionChildLocalManagedObject;
class InterfaceNDeepCompositionGChildLocalManagedObject;

class InterfaceCompositionChildLocalManagedObject : public DcmLocalManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
                        InterfaceCompositionChildLocalManagedObject    (WaveObjectManager *pWaveObjectManager);
                        InterfaceCompositionChildLocalManagedObject    (WaveObjectManager *pWaveObjectManager, SI32 &id, bool allow);
        virtual        ~InterfaceCompositionChildLocalManagedObject    ();
        void            loadOperationalData                            (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext);

        static  string  getClassName                   ();

				SI32    getId                       () const;
                void    setId                       (const SI32 &id);

				bool    getIsAllowed                () const;
                void    setIsAllowed                (const bool &allow);
				
                UI32    getOperationalRule          () const;
                void    setOperationalRule          (const UI32 &operationalRule);
                
                UI32    getOperationalRule1         () const;
                void    setOperationalRule1         (const UI32 &operationalRule1);

    // Now the Data Members

    private :
        SI32          m_rule_id;
        bool          m_allow;

        // Operational Data 
        UI32          m_operationalRule;
        UI32          m_operationalRule1;

    protected :
    public :
};

class InterfaceCompositionLocalManagedObject : public DcmLocalManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();
        virtual void loadOperationalData           (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext);

    protected :
    public :
                        InterfaceCompositionLocalManagedObject    (WaveObjectManager *pWaveObjectManager);
        virtual        ~InterfaceCompositionLocalManagedObject    ();

        static  string  getClassName                   ();

                vector<WaveManagedObjectPointer<InterfaceCompositionChildLocalManagedObject> > getChildren () const;
                void           setChildren             (vector<WaveManagedObjectPointer<InterfaceCompositionChildLocalManagedObject> > children);
                vector<WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> > getNDeepChildren () const;
                void           setNDeepChildren             (vector<WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> > children);

    // Now the Data Members

    private :
        // Composition data Fields
        vector<WaveManagedObjectPointer<InterfaceCompositionChildLocalManagedObject> > m_children;
        vector<WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> > m_nDeepchildren;

    protected :
    public :
};

}

#endif // INTERFACECOMPOSITIONCHILDLOCALMANAGEDOBJECT_H
