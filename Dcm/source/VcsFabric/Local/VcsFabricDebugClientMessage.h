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
 *   Copyright (C) 2008-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 **************************************************************************/

#ifndef VCS_FABRIC_DEBUG_CLIENT_MESSAGE
#define VCS_FABRIC_DEBUG_CLIENT_MESSAGE

#include "DcmMessaging/DcmClientManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class VcsFabricDebugClientMessage : public DcmClientManagementInterfaceMessage
    {
        private:
        protected:
            virtual     void            setupAttributesForSerialization ();
        public:
                                        VcsFabricDebugClientMessage     ();
                                       ~VcsFabricDebugClientMessage     ();

        virtual         void            loadInputFromCStructure         (const void *pInput);
        virtual         void           *getOutputForCStructure          ();

                        vector<UI32>    getListOfDomainInCluster        ();
                        void            setListOfDomainInCluster        (const vector<UI32> &domainIds);
        // Data Members

        private:
                vector<UI32>    m_domainIds;
    };
}

#endif //VCS_FABRIC_DEBUG_CLIENT_MESSAGE
