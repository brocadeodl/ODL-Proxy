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
 *   Author : MITHUN BS                                                    *
 ***************************************************************************/

#ifndef WAVETESTMANAGEDOBJECT12_H
#define WAVETESTMANAGEDOBJECT12_H

#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

class WaveTestManagedObject12 : public WaveManagedObject
{
private :
protected :
    virtual void setupAttributesForPersistence ();

public :
                                                                WaveTestManagedObject12   (WaveObjectManager *pWaveObjectManager);
    virtual                                                     ~WaveTestManagedObject12   ();

    static  string                                              getClassName             ();

    virtual void                                                setupAttributesForCreate ();

    void                                                        setInteger12              (const UI32 &integer12);
    void                                                        setMessage12              (const string &message12);

    UI32                                                        getInteger12              () const;
    string                                                      getMessage12              () const;

    // Now the data members

private :
    UI32                                                       m_integer12;
    string                                                     m_message12;


protected :
public :
};

}

#endif // WaveTestManagedObject12_H
