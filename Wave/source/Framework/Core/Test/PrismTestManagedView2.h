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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Pritee Ghosh                                                 *
 ***************************************************************************/

#ifndef PRISMTESTMANAGEDVIEW2_H
#define PRISMTESTMANAGEDVIEW2_H

#include "Framework/ObjectModel/WaveManagedView.h"

namespace WaveNs
{

class PrismTestManagedView2 : public WaveManagedView  
{
    private :
    protected :
        virtual void setupAttributesForPersistence  ();
        virtual void     setupAttributesForCreate   ();

    public :
                        PrismTestManagedView2       (WaveObjectManager *pWaveObjectManager);
        virtual        ~PrismTestManagedView2       ();

        static  string  getClassName                ();
        virtual void    getSetContextForCreateView  (WaveManagedObjectSynchronousQueryContextForSetOperation &viewContext);

                string  getMessage2                 ();
                UI32    getInteger2                 ();

    // Now the data members

    private :

    protected :
    public :
        UI32    m_integer1;
        string  m_message1;
};

}

#endif // PRISMTESTMANAGEDVIEW2_H
