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
 *   Author : vivekk                                                     *
 **************************************************************************/

#ifndef RMONCLIENTDELETERMONEVENTMESSAGE_H
#define RMONCLIENTDELETERMONEVENTMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class RmonClientDeleteRmonEventMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RmonClientDeleteRmonEventMessage ();
            RmonClientDeleteRmonEventMessage (const SI32 &eventIndex);
            virtual    ~RmonClientDeleteRmonEventMessage ();
            void setEventIndex(const SI32 &eventIndex);
            SI32 getEventIndex()  const;
			virtual const void *getCStructureForInputs    ();
			virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);


// Now the data members

        private:
        protected:
        public:
            SI32 m_eventIndex;
    };
}
#endif                                            //RMONDELETERMONEVENTMESSAGE_H
