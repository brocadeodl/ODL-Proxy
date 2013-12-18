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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Min Zhang
 **************************************************************************/

#ifndef IPADMICMPV6ECHOREPLYCONFIGMESSAGE_H
#define IPADMICMPV6ECHOREPLYCONFIGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class IpadmIcmpv6EchoreplyConfigMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            IpadmIcmpv6EchoreplyConfigMessage ();
            IpadmIcmpv6EchoreplyConfigMessage (const string &switchid,const bool &echoreply);
            virtual    ~IpadmIcmpv6EchoreplyConfigMessage ();
            void setSwitchid(const string &switchid);
            string getSwitchid()  const;
            void setEchoreply(const bool &echoreply);
            bool getEchoreply()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_switchid;
            bool	m_echoreply;
    };
}
#endif                                            //IPADMICMPV6ECHOREPLYCONFIGMESSAGE_H