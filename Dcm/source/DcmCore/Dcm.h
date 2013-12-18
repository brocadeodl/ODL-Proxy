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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef DCM_H
#define DCM_H

#include "Framework/Types/Types.h"

#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"
#include <string>

using namespace std;
using namespace WaveNs;

namespace DcmNs
{

class Dcm
{
    private :
    protected :
    public :
        static void initialize       ();
        static void registerServices (const bool &registerAllServicesInInactiveMode = false);
        static void initializeForWsm ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // DCM_H
