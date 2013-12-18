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
 *   Author : aantony                                                     *
 **************************************************************************/

#ifndef LSYSGLOBALUPDATELSYSAGINGTIMEOUTMESSAGE_H
#define LSYSGLOBALUPDATELSYSAGINGTIMEOUTMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class L2sysGlobalUpdateL2sysAgingTimeoutMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            L2sysGlobalUpdateL2sysAgingTimeoutMessage ();
            L2sysGlobalUpdateL2sysAgingTimeoutMessage (const UI32 &AgingTimeout);
            virtual    ~L2sysGlobalUpdateL2sysAgingTimeoutMessage ();
            void setAgingTimeout(const UI32 &AgingTimeout);
            UI32 getAgingTimeout()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_AgingTimeout;
    };
}
#endif                                            //LSYSGLOBALUPDATELSYSAGINGTIMEOUTMESSAGE_H
