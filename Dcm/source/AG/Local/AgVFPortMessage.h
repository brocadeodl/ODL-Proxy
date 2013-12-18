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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : D. Chung
 **************************************************************************/

#ifndef AGVFPORTMESSAGE_H
#define AGVFPORTMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
using namespace WaveNs;

namespace DcmNs
{

    typedef enum
    {
        VFPORT_MESSAGE_CREATE = 0,
        VFPORT_MESSAGE_DELETE
    }VFPortMessageTypes;

    class AgVFPortMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            AgVFPortMessage ();
			AgVFPortMessage (const UI32 messageType);
            virtual    ~AgVFPortMessage ();

    		void  setMessageType(const UI32 messageType);
		    UI32  getMessageType() const;
    		void  setVFPortsByPort(const vector<UI32> &portList);
		    vector<UI32>  getVFPortsByPort() const;
            void setVFPortsByIfName(const vector<string> &ifList);
            vector<string> getVFPortsByIfName() const;

// Now the data members

        private:
        protected:
        public:
            UI32 m_messageType;
            vector<UI32> m_vfportsByPort;
            vector<string> m_vfportsByIfName;
    };
}
#endif                                            //AGVFPORTMESSAGE_H
