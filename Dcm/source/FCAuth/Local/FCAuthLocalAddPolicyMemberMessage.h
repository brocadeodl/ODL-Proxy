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
 *   Author : noku                                                     *
 **************************************************************************/

#ifndef FCAUTHLOCALADDPOLICYMEMBERMESSAGE_H
#define FCAUTHLOCALADDPOLICYMEMBERMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FCAuthLocalAddPolicyMemberMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FCAuthLocalAddPolicyMemberMessage ();
			FCAuthLocalAddPolicyMemberMessage (const string &PolicyName,const string &PolicyEntryList,const bool &DefinedFlag,const bool &CfgReplayFlag);
            virtual    ~FCAuthLocalAddPolicyMemberMessage ();
            void setPolicyName(const string &PolicyName);
            string getPolicyName()  const;
            void setPolicyEntryList(const string &PolicyEntryList);
            string getPolicyEntryList()  const;
			void setDefinedFlag(const bool &DefinedFlag);
            bool getDefinedFlag()  const;
            void setCfgReplayFlag(const bool &CfgReplayFlag);
            bool getCfgReplayFlag()  const;
// Now the data members

        private:
        protected:
        public:
            string  m_PolicyName;
            string  m_PolicyEntryList;
			bool  m_DefinedFlag;
            bool  m_CfgReplayFlag;
    };
}
#endif                                            //FCAUTHLOCALADDPOLICYMEMBERMESSAGE_H
