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

#ifndef PHYSICALINTERFACELOCALMANAGEDOBJECT_H
#define PHYSICALINTERFACELOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

namespace DcmNs
{

class PhysicalInterfaceLocalManagedObject : public DcmLocalManagedObject
{
    private :
        void setupAttributesForPersistence ();
        void setupAttributesForCreate      ();

    protected :
    public :
                        PhysicalInterfaceLocalManagedObject (WaveObjectManager *pWaveObjectManager);
                        PhysicalInterfaceLocalManagedObject (WaveObjectManager *pWaveObjectManager, const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description);
        virtual        ~PhysicalInterfaceLocalManagedObject ();

        static  string  getClassName                        ();
                UI32    getIfIndex                     () const;
                UI32    getMtu                         () const;
                string  getDescription                 () const;
        
                void    setIfIndex                     (const UI32 &inIndex);
                void    setMtu                         (const UI32 &mtu);
                void    setDescription                 (const string &ipAddress);

    // Now the data members

    private :
        UI32   m_ifIndex;
        UI32   m_mtu;
        string m_description;

    protected :
    public :
};

}

#endif // PHYSICALINTERFACELOCALMANAGEDOBJECT_H
