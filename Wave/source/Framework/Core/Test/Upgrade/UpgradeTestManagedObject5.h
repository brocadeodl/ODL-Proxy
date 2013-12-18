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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef UPGRADETESTMANAGEDOBJECT5_H
#define UPGRADETESTMANAGEDOBJECT5_H

#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveLocalManagedObjectBase.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject4.h"

namespace WaveNs
{

class UpgradeTestManagedObject5 : public virtual UpgradeTestManagedObject4, public virtual WaveLocalManagedObjectBase
{
private :
    virtual void setupAttributesForPersistence ();

protected :
public :
    UpgradeTestManagedObject5  (WaveObjectManager *pWaveObjectManager);
    virtual                  ~UpgradeTestManagedObject5  ();

    static  string            getClassName             ();

    virtual void              setupAttributesForCreate ();

    void              setAssociationVector5    (const vector<ObjectId> &associationVector5);

    vector<ObjectId>  getAssociationVector5    () const;

    virtual bool              operator ==              (const UpgradeTestManagedObject5 &rhs) const;
    virtual bool              operator !=              (const UpgradeTestManagedObject5 &rhs) const;


    // Now the data members

private :
    vector<ObjectId> m_associationVector5;

protected :
public :
};

}

#endif // UPGRADETESTMANAGEDOBJECT5_H
