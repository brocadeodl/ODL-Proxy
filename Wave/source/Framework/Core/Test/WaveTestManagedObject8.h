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

#ifndef WAVETESTMANAGEDOBJECT8_H
#define WAVETESTMANAGEDOBJECT8_H

#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

class WaveTestManagedObject8 : public WaveManagedObject
{
    private :
    protected :
        virtual void setupAttributesForPersistence ();

    public :
                                                                            WaveTestManagedObject8   (WaveObjectManager *pWaveObjectManager);
        virtual                                                            ~WaveTestManagedObject8   ();

        static  string                                                      getClassName             ();

        virtual void                                                        setupAttributesForCreate ();

                void                                                        setInteger8              (const UI32 &integer8);
                void                                                        setMessage8              (const string &message8);

                UI32                                                        getInteger8              () const;
                string                                                      getMessage8              () const;

    // Now the data members

    private :
        UI32                                                       m_integer8;
        string                                                     m_message8;


    protected :
    public :
};

}

#endif // WaveTestManagedObject8_H
