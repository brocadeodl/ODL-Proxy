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
 *   Author : kghanta                                                     *
 **************************************************************************/

#ifndef AAAGLOBALCHANGERULEMESSAGE_H
#define AAAGLOBALCHANGERULEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class AAAGlobalChangeRuleMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            AAAGlobalChangeRuleMessage ();
            AAAGlobalChangeRuleMessage (const UI32 &index, const vector<string> &command, const vector<string> &keypath, const string &context, const SI32 &commandEnum, const string &instanceCommand, const RBACCommandType &commandType, const string &role,const RBACOperation &operation,const RBACAction &action);
            virtual    ~AAAGlobalChangeRuleMessage ();
            void setIndex(const UI32 &index);
            UI32 getIndex()  const;
            void setCommand(const vector<string> &command);
            vector<string> getCommand()  const;
            void setKeypath(const vector<string> &keypath);
            vector<string> getKeypath()  const;
            void setContext(const string &context);
            string getContext()  const;
            void setCommandEnum(const SI32 &commandEnum);
            SI32 getCommandEnum()  const;
            void setInstanceCommand(const string &instanceCommand);
            string getInstanceCommand()  const;
            void setCommandType(const RBACCommandType &command);
            RBACCommandType getCommandType()  const;
            void setRole(const string &role);
            string getRole()  const;
            void setOperation(const RBACOperation &operation);
            RBACOperation getOperation()  const;
            void setAction(const RBACAction &action);
            RBACAction getAction()  const;
// Now the data members

        private:
        protected:
        public:
            UI32  m_index;
            vector<string>  m_command;
            vector<string>  m_keypath;
            string  m_context;
            SI32  m_commandEnum;
			string m_instanceCommand;
			RBACCommandType m_commandType;
            string  m_role;
            RBACOperation  m_operation;
            RBACAction  m_action;
    };
}
#endif                                            //AAAGLOBALCHANGERULEMESSAGE_H
