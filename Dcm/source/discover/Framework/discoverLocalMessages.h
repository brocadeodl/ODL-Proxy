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
 *   Author : sharis                                                     *
 **************************************************************************/

#ifndef DISCOVERLOCALMESSAGES_H
#define DISCOVERLOCALMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

	// ************* DiscoverLocalDiscoverMessage ************
    class DiscoverLocalDiscoverMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            DiscoverLocalDiscoverMessage ();
            DiscoverLocalDiscoverMessage (const UI32 &cmdcode, const UI32 &msgType, 
							const string &vcenter);
            virtual    ~DiscoverLocalDiscoverMessage ();
            void setCmdcode(const UI32 &cmdcode);
            UI32 getCmdcode()  const;
            void setMsgType(const UI32 &msgType);
            UI32 getMsgType()  const;
            void setVcenter(const string &vcenter);
            string getVcenter()  const;
            void setScriptNames(const string &scriptNames);
            string getScriptNames()  const;
            void setArg1(const string &arg1);
            string getArg1()  const;
            void setArg2(const string &arg2);
            string getArg2()  const;
            void setArg3(const string &arg3);
            string getArg3()  const;
            void setArg4(const string &arg4);
            string getArg4()  const;
            void setDataList(const vector<string> &m_datalist);
            vector<string> getDataList() const;

		// Now the data members

        private:
        protected:
        public:
            UI32  m_cmdcode;
            UI32  m_msgType;
            string  m_vcenter;
            string  m_scriptNames;
            string  m_arg1;
            string  m_arg2;
            string  m_arg3;
            string  m_arg4;
            vector <string> m_datalist;
    };
}
#endif  // DISCOVERLOCALMESSAGES_H
