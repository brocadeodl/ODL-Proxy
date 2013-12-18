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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef WAVETESTMANAGEDOBJECT9_H
#define WAVETESTMANAGEDOBJECT9_H

#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

class WaveTestManagedObject9 : public WaveManagedObject
{
    private :
    protected :
        virtual void setupAttributesForPersistence ();

    public :
                                                                            WaveTestManagedObject9   (WaveObjectManager *pWaveObjectManager);
        virtual                                                            ~WaveTestManagedObject9   ();

        static  string                                                      getClassName             ();

        virtual void                                                        setupAttributesForCreate ();

                void                                                        setInteger9              (const UI32 &integer9);
                void                                                        setMessage9              (const string &message9);

                UI32                                                        getInteger9              () const;
                string                                                      getMessage9              () const;

    // Now the data members

    private :
        UI32                                                       m_integer9;
        UI32                                                       m_integer9_1;  // This is a dummy data member and not used in BIST, this is just used to create a schema change
        string                                                     m_message9;


    protected :
    public :
};

}

#endif // WaveTestManagedObject9_H
