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

/****************************************************************************
 *  Copyright (C) 2011 Brocade Communications Systems, Inc.                 *
 *  All rights reserved.                                                    *
 *                                                                          *
 *  Author : Mayank Maheshwari                                              *
 *                                                                          *
 ****************************************************************************/

#ifndef NSMLOCALMACREBALANCEMESSAGE_H
#define NSMLOCALMACREBALANCEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class NsmLocalMacRebalanceMessage : public PrismMessage {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmLocalMacRebalanceMessage();
            NsmLocalMacRebalanceMessage (const UI32 &poIfIndex, const UI32 &loc_id);
            virtual ~NsmLocalMacRebalanceMessage();

            UI32 getDestLocationId() const;
            void setDestLocationId(const UI32 &loc_id);
            UI32 getPoIfIndex() const;
            void setPoIfIndex(const UI32 &poIfIndex);

        // Data Members
        private:
        protected:
        public:
            UI32 m_poIfIndex;
            UI32 m_locationId;
    };
}
#endif //NSMLOCALMACREBALANCEMESSAGE_H
