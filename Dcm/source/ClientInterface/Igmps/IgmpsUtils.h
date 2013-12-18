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

#ifndef IGMPS_UTILS_H 
#define IGMPS_UTILS_H 

#include <sstream>
#include <string>
#include <vector>
#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"


using namespace WaveNs;
using namespace std;

namespace DcmNs
{

template<class T>  
string toString(const T & t)
{
    stringstream ss; 
    ss<<t;
    return ss.str();
}


    class IgmpsUtils
    {
        private:
            IgmpsUtils();
        public:
            static int verifyPOValue (char *argument);
            static int verifyGroupAddress (const string& group_addr);
            static ResourceId mapBackEndErrorsToDcmdErrors(int err);
            static ResourceId IgmpsStatusReturn(PrismMessage* msg_p, DcmManagementInterfaceMessage* cl_msg_p, ResourceId status);
			static void getTwoTupleNameIfVcsEnabled(const string& ifName, string& twoTupleIfName);
			static int compare (const void * a, const void * b);
    };

}

#endif  //IGMPS_UTILS_H


