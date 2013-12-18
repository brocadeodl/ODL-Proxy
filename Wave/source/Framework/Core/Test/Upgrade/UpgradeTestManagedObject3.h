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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef UPGRADETESTMANAGEDOBJECT3_H
#define UPGRADETESTMANAGEDOBJECT3_H

#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject1.h"

namespace WaveNs
{

class UpgradeTestManagedObject3 : public UpgradeTestManagedObject1
{
private :
    virtual void setupAttributesForPersistence ();

protected :
public :
    UpgradeTestManagedObject3  (WaveObjectManager *pWaveObjectManager);
    virtual        ~UpgradeTestManagedObject3  ();

    static  string  getClassName             ();

    virtual void    setupAttributesForCreate ();

    void    setInteger3              (const UI32 &integer3);
    void    setMessage3              (const string &message3);
    void    setObjectId3             (const ObjectId &objectId3);
    void    setAssociation3          (const ObjectId &association3);

    UI32      getInteger3            () const;
    string    getMessage3            () const;
    ObjectId  getObjectId3           () const;
    ObjectId  getAssociation3        () const;

    virtual bool     operator ==             (const UpgradeTestManagedObject3 &rhs) const;
    virtual bool     operator !=             (const UpgradeTestManagedObject3 &rhs) const;

    // Now the data members

private :
    UI32     m_integer3;
    string   m_message3;
    ObjectId m_objectId3;
    ObjectId m_association3;

protected :
public :
};

}

#endif // UPGRADETESTMANAGEDOBJECT3_H
