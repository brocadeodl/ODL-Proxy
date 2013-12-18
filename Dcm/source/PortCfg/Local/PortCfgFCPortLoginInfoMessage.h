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
 *   Author : vbagavat                                                     *
 **************************************************************************/

#ifndef PORTCFGFCPORTLOGININFOMESSAGE_H
#define PORTCFGFCPORTLOGININFOMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortCfgFCPortLoginInfoMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PortCfgFCPortLoginInfoMessage ();
            PortCfgFCPortLoginInfoMessage (const UI32 &cmdCode,const vector<string> &portId,const vector<UI32> &rbridgeId,const vector<UI32> &index,const vector<string> &pid,const vector<string> &portWWN,const vector<string> &configMode,const vector<string> &portstatus, const vector<string> &portspeed, const vector<UI32> &device_count);
            virtual    ~PortCfgFCPortLoginInfoMessage ();
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setPortId(const vector<string> &portId);
            vector<string> getPortId()  const;
            void setRbridgeId(const vector<UI32> &rbridgeId);
            vector<UI32> getRbridgeId()  const;
            void setIndex(const vector<UI32> &index);
            vector<UI32> getIndex()  const;
            void setPid(const vector<string> &pid);
            vector<string> getPid()  const;
            void setPortWWN(const vector<string> &portWWN);
            vector<string> getPortWWN()  const;
            void setConfigMode(const vector<string> &configMode);
            vector<string> getConfigMode()  const;
            void setPortstatus(const vector<string> &portstatus);
            vector<string> getPortstatus()  const;
            void setPortspeed(const vector<string> &portspeed);
            vector<string> getPortspeed()  const;
            void setDevice_count(const vector<UI32> &device_count);
            vector<UI32> getDevice_count()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_cmdCode;
            vector<string>  m_portId;
            vector<UI32>  m_rbridgeId;
            vector<UI32>  m_index;
            vector<string>  m_pid;
            vector<string>  m_portWWN;
            vector<string>  m_configMode;
            vector<string>  m_portstatus;
            vector<string>  m_portspeed;
            vector<UI32>  m_device_count;
    };
}
#endif                                            //PORTCFGFCPORTLOGININFOMESSAGE_H
