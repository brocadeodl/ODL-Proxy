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

#ifndef CMDRULEMANAGEDOBJECT_H
#define CMDRULEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

#include "AAAGlobalTypes.h"
using namespace WaveNs;

namespace DcmNs
{

    class AAAGlobalObjectManager;
    class CmdRuleManagedObject : virtual public DcmManagedObject
    {
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
			virtual void setupKeys();
        public:
            CmdRuleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
            CmdRuleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager, const UI32 &index, const string &context, const string &command, const string &group, const RBACOperation &ops, const RBACAction &action);
            virtual    ~CmdRuleManagedObject ();
            static string getClassName();
            void setIndex(const UI32 &index);
            UI32 getIndex()  const;
            void setContext(const string &context);
            string getContext()  const;
            void setCommand(const string &command);
            string getCommand()  const;
            void setGroup(const string &group);
            string getGroup()  const;
            void setOps(const RBACOperation &ops);
            RBACOperation getOps()  const;
            void setAction(const RBACAction &action);
            RBACAction getAction()  const;

// Now the data members
        private:
            UI32  m_index;
            string  m_context;
            string  m_command;
			string m_group;
			RBACOperation m_ops;
            RBACAction  m_action;
    };
}
#endif	//CMDRULEMANAGEDOBJECT_H
