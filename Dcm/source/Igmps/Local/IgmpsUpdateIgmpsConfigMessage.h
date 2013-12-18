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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#ifndef IGMPSUPDATEIGMPSCONFIGMESSAGE_H
#define IGMPSUPDATEIGMPSCONFIGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class IgmpsUpdateIgmpsConfigMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            IgmpsUpdateIgmpsConfigMessage ();
            IgmpsUpdateIgmpsConfigMessage (const bool &enable);
            virtual    ~IgmpsUpdateIgmpsConfigMessage ();
            void setEnable(const bool &enable);
            bool getEnable()  const;
            bool getRestrictUnknownMcast() const;
            void setRestrictUnknownMcast(const bool &restrictUnknownMcast);
            void setOpCode(const UI32 &opCode);
            UI32 getOpCode() const;


// Now the data members

        private:
        protected:
        public:
            bool  m_enable;
            bool  m_restrictUnknownMcast;
            UI32  m_opCode;

    };
}
#endif                                            //IGMPSUPDATEIGMPSCONFIGMESSAGE_H
