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
 *   Author : cshah                                                     *
 **************************************************************************/

#ifndef IPADM_LINK_SPEED_CONFIG_MESSAGE_H
#define IPADM_LINK_SPEED_CONFIG_MESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class IpadmLinkSpeedConfigMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            IpadmLinkSpeedConfigMessage ();
            IpadmLinkSpeedConfigMessage (const string &switchid, const LinkSpeed &speed);
            virtual    ~IpadmLinkSpeedConfigMessage ();
            void setSwitchid(const string &switchid);
            string getSwitchid()  const;
            void setLinkSpeed(const LinkSpeed &speed);
            LinkSpeed getLinkSpeed()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_switchid;
            LinkSpeed m_link_speed;
    };
}
#endif  //IPADM_LINK_SPEED_CONFIG_MESSAGE_H
