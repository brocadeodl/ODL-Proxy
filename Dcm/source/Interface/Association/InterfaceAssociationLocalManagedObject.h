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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef INTERFACEASSOCIATIONCHILDLOCALMANAGEDOBJECT_H
#define INTERFACEASSOCIATIONCHILDLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class InterfaceSampleVlanLocalManagedObject : public DcmLocalManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
                        InterfaceSampleVlanLocalManagedObject    (WaveObjectManager *pWaveObjectManager);
                        InterfaceSampleVlanLocalManagedObject    (WaveObjectManager *pWaveObjectManager, UI32 &id);
        virtual        ~InterfaceSampleVlanLocalManagedObject    ();

        static  string  getClassName                   ();

				UI32    getId                       () const;
                void    setId                       (const UI32 &id);

    // Now the Data Members

    private :
        UI32          m_id;

    protected :
    public :
};

class DcmInterfaceAssociationLocalManagedObject : public DcmLocalManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
                        DcmInterfaceAssociationLocalManagedObject    (WaveObjectManager *pWaveObjectManager);
        virtual        ~DcmInterfaceAssociationLocalManagedObject    ();

        static  string              getClassName                   ();

                void                setVlanId                       (const ObjectId &vlanId);
                ObjectId            getVlanId                       ();
                void                setVlanIdVector                 (const vector<ObjectId> &vlanId);
                vector<ObjectId>    getVlanIdVector                 ();
                void                setObjectId                     (const ObjectId &objectId);
                void                setVlanIdVector2                (const vector<ObjectId> &vlanId);
                vector<ObjectId>    getVlanIdVector2                ();

    // Now the Data Members

    private :
        // Association data Fields
        ObjectId        m_vlanId;
        vector<ObjectId> m_vlanIdList;
        ObjectId        m_objectId;
        vector<ObjectId> m_vlanIdList2; // this is a string in front end

    protected :
    public :
};

}

#endif // INTERFACEASSOCIATIONCHILDLOCALMANAGEDOBJECT_H
