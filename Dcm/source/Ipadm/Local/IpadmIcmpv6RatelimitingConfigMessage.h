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

#ifndef IPADMICMPV6RATELIMITINGCONFIGMESSAGE_H
#define IPADMICMPV6RATELIMITINGCONFIGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class IpadmIcmpv6RatelimitingConfigMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            IpadmIcmpv6RatelimitingConfigMessage ();
            IpadmIcmpv6RatelimitingConfigMessage (const string &switchid,const unsigned int &ratelimiting);
            virtual    ~IpadmIcmpv6RatelimitingConfigMessage ();
            void setSwitchid(const string &switchid);
            string getSwitchid()  const;
            void setRatelimiting(const unsigned int &ratelimiting);
            unsigned int getRatelimiting()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_switchid;
            unsigned int m_ratelimiting;
    };
}
#endif                                            //IPADMICMPV6RATELIMITINGCONFIGMESSAGE_H
