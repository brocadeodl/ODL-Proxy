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

#ifndef DCMINTERFACETESTCONTEXT_H
#define DCMINTERFACETESTCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"

#include <vector>
#include <map>

using namespace WaveNs;
using namespace std;

namespace DcmNs
{

class InterfaceLocalManagedObject;

class DcmInterfaceTestContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                               DcmInterfaceTestContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                              ~DcmInterfaceTestContext ();

        vector<InterfaceLocalManagedObject *> &getInterfaces           ();

    // Now the data members

    private :
        vector<InterfaceLocalManagedObject *> m_interfaces;

    protected :
    public :
};

}

#endif // DCMINTERFACETESTCONTEXT_H
