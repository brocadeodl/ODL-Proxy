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

#ifndef HAOPACTIONMESSAGE_H
#define HAOPACTIONMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{   
        
    class HaOpActionMessage : public PrismMessage
    {       
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            HaOpActionMessage ();
            HaOpActionMessage (const string &command);
            virtual    ~HaOpActionMessage ();
            void setCommand(const string &command);
            string getCommand()  const;
            void setResult(const string &result);
            string getResult()  const;

			void setFlag(const UI32 &flag);
			UI32 getFlag() const;
			void setRbridgeId(const UI32 &rbridge_id);
			UI32 getRbridgeId() const;
        
        private:
        protected:  
        public:
            string  m_command;
            string  m_result;                     

			UI32 m_rbridge_id;
			UI32 m_flag;
    };
}
#endif
