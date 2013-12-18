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
 *   Author : dchung                                                     *
 **************************************************************************/

#ifndef PORTCFGPORTCFGTRUNKUPDATEMESSAGE_H
#define PORTCFGPORTCFGTRUNKUPDATEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortCfgPortCfgTrunkUpdateMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PortCfgPortCfgTrunkUpdateMessage ();
            PortCfgPortCfgTrunkUpdateMessage (const string &id,const bool &trunkEnable,const bool &isIdComplete);
            virtual    ~PortCfgPortCfgTrunkUpdateMessage ();
            void setId(const string &id);
            string getId()  const;
            void setTrunkEnable(const bool &trunkEnable);
            bool getTrunkEnable()  const;
            void setIsIdComplete(const bool &isIdComplete);
            bool getIsIdComplete()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_id;
            bool  m_trunkEnable;
            bool  m_isIdComplete;
    };
}
#endif                                            //PORTCFGPORTCFGTRUNKUPDATEMESSAGE_H
