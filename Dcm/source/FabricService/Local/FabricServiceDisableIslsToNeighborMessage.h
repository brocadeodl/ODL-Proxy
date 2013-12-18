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
 *   Author : krangara                                                     *
 **************************************************************************/

#ifndef FABRICSERVICEDISABLEISLSTONEIGHBORMESSAGE_H
#define FABRICSERVICEDISABLEISLSTONEIGHBORMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/WorldWideName.h"

using namespace WaveNs;

namespace DcmNs
{

    class FabricServiceDisableIslsToNeighborMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FabricServiceDisableIslsToNeighborMessage ();
            FabricServiceDisableIslsToNeighborMessage (const WorldWideName &switch_wwn,const SI32 &reason_code);
            virtual    ~FabricServiceDisableIslsToNeighborMessage ();
            void setSwitch_wwn(const WorldWideName &switch_wwn);
            WorldWideName getSwitch_wwn()  const;
            void setReason_code(const SI32 &reason_code);
            SI32 getReason_code()  const;

// Now the data members

        private:
        protected:
        public:
            WorldWideName  m_switch_wwn;
            SI32  m_reason_code;
    };
}
#endif                                            //FABRICSERVICEDISABLEISLSTONEIGHBORMESSAGE_H
