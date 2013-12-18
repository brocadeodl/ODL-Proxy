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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef FRAMEWORKTESTABILITY3OBJECTMANAGER_H
#define FRAMEWORKTESTABILITY3OBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"

namespace WaveNs
{

class FrameworkTestabilityEvent1;

class FrameworkTestability3ObjectManager : public WaveObjectManager
{
    private :
                      FrameworkTestability3ObjectManager    ();
        PrismMessage *createMessageInstance                 (const UI32 &operationCode);

        void          listenForEvents                        (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        void          frameworkTestabilityEvent1EventHandler (const FrameworkTestabilityEvent1 *&pEvent);

    protected :
    public :
        virtual                                    ~FrameworkTestability3ObjectManager     ();
        static  FrameworkTestability3ObjectManager *getInstance                            ();
        static  PrismServiceId                      getPrismServiceId                      ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // FRAMEWORKTESTABILITY3OBJECTMANAGER_H
