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

#ifndef FRAMEWORKTESTABILITY5OBJECTMANAGER_H
#define FRAMEWORKTESTABILITY5OBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

namespace WaveNs
{

class FrameworkTestabilityMessage10;

class FrameworkTestability5ObjectManager : public WaveLocalObjectManager
{
    private :
                            FrameworkTestability5ObjectManager     ();
        PrismMessage       *createMessageInstance                  (const UI32 &operationCode);
        WaveManagedObject  *createManagedObjectInstance            (const string &managedClassName);

        void                frameworkTestability10MessageHandler    (FrameworkTestabilityMessage10 *pMessage);

    protected :
    public :
        virtual                                    ~FrameworkTestability5ObjectManager     ();
        static  FrameworkTestability5ObjectManager *getInstance                            ();
        static  PrismServiceId                      getPrismServiceId                      ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // FRAMEWORKTESTABILITY5OBJECTMANAGER_H