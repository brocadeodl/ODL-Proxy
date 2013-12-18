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

#ifndef UPGRADETESTMANAGEDOBJECT1_H
#define UPGRADETESTMANAGEDOBJECT1_H

#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

class UpgradeTestManagedObject1 : public WaveManagedObject
{
private :
protected :
    virtual void setupAttributesForPersistence ();

public :
    UpgradeTestManagedObject1   (WaveObjectManager *pWaveObjectManager);
    virtual        ~UpgradeTestManagedObject1   ();

    static  string   getClassName             ();

    virtual void     setupAttributesForCreate ();

    void     setInteger1              (const UI32 &integer1);
    void     setMessage1              (const string &message1);
    void     setObjectId1             (const ObjectId &objectId1);

    UI32     getInteger1              () const;
    string   getMessage1              () const;
    ObjectId getObjectId1             () const;

    virtual bool     operator ==              (const UpgradeTestManagedObject1 &rhs) const;
    virtual bool     operator !=              (const UpgradeTestManagedObject1 &rhs) const;

    // Now the data members

private :
    UI32     m_integer1;
    string   m_message1;
    ObjectId m_objectId1;

protected :
public :
};

}

#endif // UPGRADETESTMANAGEDOBJECT1_H
