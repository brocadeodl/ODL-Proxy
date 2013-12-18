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
 *   Author : dchung                                                     *
 **************************************************************************/

#ifndef PORTCFGPORTCFGPORTSTATEUPDATEMESSAGE_H
#define PORTCFGPORTCFGPORTSTATEUPDATEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortCfgPortCfgPortStateUpdateMessage  : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PortCfgPortCfgPortStateUpdateMessage ();
			PortCfgPortCfgPortStateUpdateMessage (const string &id, const bool &portStateEnable,
			                                      const bool &isIdComplete, const SI32 &portDisReason);
            virtual    ~PortCfgPortCfgPortStateUpdateMessage ();
            void setId(const string &id);
            string getId()  const;
            void setPortStateEnable(const bool &portStateEnable);
            bool getPortStateEnable()  const;
			void setIsIdComplete(const bool &isIdComplete);
			bool getIsIdComplete()  const;
			void setPortDisReason(const SI32 &portDisReason);
			SI32 getPortDisReason()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_id;
            bool  m_portStateEnable;
            bool  m_isIdComplete;
            SI32  m_portDisReason;
    };
}
#endif                                            //PORTCFGPORTCFGPORTSTATEUPDATEMESSAGE_H
