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
 *   Author : lhu                                                     *
 **************************************************************************/

#ifndef TERMINALMANAGEDOBJECT_H
#define TERMINALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "TerminalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class TerminalObjectManager;
    class TerminalManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
			virtual void setupKeys();
        public:
            TerminalManagedObject (TerminalObjectManager *pTerminalObjectManager);
            TerminalManagedObject (TerminalObjectManager *pTerminalObjectManager, const Session_Id &sessionid, const UI32 &exectimeout);
            virtual    ~TerminalManagedObject ();
            static string getClassName();
            void setSessionid(const Session_Id &sessionid);
            Session_Id getSessionid()  const;
            void setExectimeout(const UI32 &exectimeout);
            UI32 getExectimeout()  const;

// Now the data members

        private:
			Session_Id  m_sessionid;
            UI32  m_exectimeout;
        protected:
        public:
    };
}
#endif                                            //TERMINALMANAGEDOBJECT_H
