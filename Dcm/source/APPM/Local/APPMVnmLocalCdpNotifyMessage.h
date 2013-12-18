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
 *   All rights reserved.                                                  *
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#ifndef APPMVNMLOCALCDPNOTIFYMESSAGE_H
#define APPMVNMLOCALCDPNOTIFYMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMVnmLocalCdpNotifyMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            APPMVnmLocalCdpNotifyMessage ();
            APPMVnmLocalCdpNotifyMessage (const UI32 &cmdCode,const string &ifName,const string &hostMor,const string &portId);
            virtual    ~APPMVnmLocalCdpNotifyMessage ();
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setHostMor(const string &hostMor);
            string getHostMor()  const;
            void setPortId(const string &portId);
            string getPortId()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_cmdCode;
            string  m_ifName;
            string  m_hostMor;
            string  m_portId;
    };
}
#endif                                            //APPMVNMLOCALCDPNOTIFYMESSAGE_H
