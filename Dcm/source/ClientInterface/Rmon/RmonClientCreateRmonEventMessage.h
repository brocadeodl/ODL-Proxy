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

#ifndef RMONCLIENTCREATERMONEVENTMESSAGE_H
#define RMONCLIENTCREATERMONEVENTMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class RmonClientCreateRmonEventMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RmonClientCreateRmonEventMessage ();
            RmonClientCreateRmonEventMessage (const SI32 &eventIndex,const bool &log,const string &communityName,const string &description,const string &owner);
            virtual    ~RmonClientCreateRmonEventMessage ();
            void setEventIndex(const SI32 &eventIndex);
            SI32 getEventIndex()  const;
            void setLog(const bool &log);
            bool getLog()  const;
            void setCommunityName(const string &communityName);
            string getCommunityName()  const;
            void setDescription(const string &description);
            string getDescription()  const;
            void setOwner(const string &owner);
            string getOwner()  const;

			virtual const void *getCStructureForInputs    ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
			const void CopyCString(char* dst, string& src);
// Now the data members

        private:
        protected:
        public:
            SI32  m_eventIndex;
            bool m_log;
            string  m_communityName;
            string  m_description;
            string  m_owner;
    };
}
#endif                                            //RMONCREATERMONEVENTMESSAGE_H
