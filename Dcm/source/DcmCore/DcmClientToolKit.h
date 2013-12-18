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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka, Kuai Yu                           *
 ***************************************************************************/

#ifndef DCMCLIENTTOOLKIT_H
#define DCMCLIENTTOOLKIT_H

#include "Framework/Types/Types.h"
#include "Framework/Types/WorldWideName.h"
#include "Framework/Types/DateTime.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/Utils/PrismMutex.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{

class DcmClientToolKit
{
    private:
        DcmClientToolKit ();
    protected:
    public:
        static const SI32              getManagementInterfaceReceiverPortForCcmd       ();
        static void                    setManagementInterfaceReceiverPortForCcmd       (const SI32 &managementInterfaceReceiverPort); 
    // Now the data members

    private:
    protected:
    public:
};

}

#endif //DCMCLIENTTOOLKIT_H
