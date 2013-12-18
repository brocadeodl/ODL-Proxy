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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : MITHUN BS                                                    *
 ***************************************************************************/

#ifndef PRISMTESTMANAGEDVIEW1_H
#define PRISMTESTMANAGEDVIEW1_H 

#include "Framework/ObjectModel/WaveManagedView.h"

namespace WaveNs
{

class PrismTestManagedView1 : virtual public WaveManagedView
{
    private :
    protected :
        void setupAttributesForPersistence  ();
        void setupAttributesForCreate       ();

    public :
                           PrismTestManagedView1            (WaveObjectManager *pWaveObjectManager);
        virtual           ~PrismTestManagedView1            ();

        static  string      getClassName             ();
        virtual string      getSqlForCreateView      ();

                void        setInteger1              (const UI32 &integer1);
                void        setMessage1              (const string &message1);

                UI32        getInteger1              () const;
                string      getMessage1              () const;


    // Now the data members
    private :
    UI32        m_integer1;
    string      m_message1;

    protected:
    public :
};

}

#endif // PRISMTESTMANAGEDVIEW1_H 
