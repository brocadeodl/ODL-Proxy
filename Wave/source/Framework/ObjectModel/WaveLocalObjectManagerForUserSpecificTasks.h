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

#ifndef PRISMLOCALOBJECTMANAGERFORUSERSPECIFICTASKS_H
#define PRISMLOCALOBJECTMANAGERFORUSERSPECIFICTASKS_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

namespace WaveNs
{

class WaveLocalObjectManagerForUserSpecificTasks : public WaveLocalObjectManager
{
    private :
        static void addToUserspecificServicesMap (const PrismServiceId &prismServiceId);

    protected :
        WaveLocalObjectManagerForUserSpecificTasks (const string &objectManagerName, const UI32 &stackSize = 0, const vector<UI32> *pCpuAffinityVector = NULL);

    public :
        virtual      ~WaveLocalObjectManagerForUserSpecificTasks ();
        static  bool  isAUserSpecificService                      (const PrismServiceId &prismServiceId);

    // now the data members

    private :
    protected :
    public :
};

}

#endif // PRISMLOCALOBJECTMANAGERFORUSERSPECIFICTASKS_H
