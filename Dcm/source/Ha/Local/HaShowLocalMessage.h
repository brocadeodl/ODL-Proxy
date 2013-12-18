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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   October, 2011                                                         *
 *   All rights reserved.                                                  *
 ***************************************************************************/

#ifndef HASHOWLOCALMESSAGE_H
#define HASHOWLOCALMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{   
        
    class HaShowLocalMessage : public PrismMessage
    {       
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            HaShowLocalMessage ();
            HaShowLocalMessage (const string &command);
            virtual    ~HaShowLocalMessage ();
            void setCmd(const string &command);
            string getCmd()  const;
            void setData(const string &data);
            string getData()  const;
			void setMappedId(const UI32 &mappedId);
			UI32 getMappedId() const;
        
        private:
        protected:  
        public:
            string  m_command;
            string  m_data;                     
			UI32 m_mappedId;
    };
}
#endif