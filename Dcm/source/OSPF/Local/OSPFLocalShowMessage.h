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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved. 
 *   Author :   Brundar                                               *
 **************************************************************************/

#ifndef OSPFLocalShowMessage_H
#define OSPFLocalShowMessage_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class OSPFLocalShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            OSPFLocalShowMessage ();
            virtual    ~OSPFLocalShowMessage ();
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setIpAddr1(const string &str);
            IpV4Address getIpAddr1() const;
            void setIpAddr2(const string &str);
            IpV4Address getIpAddr2() const;
            void setIntVal(const string &str);
            UI32 getIntVal() const;
            void setStrVal(const string &str);
            string getStrVal() const;
            void setIfname(const string &str);
            string getIfname() const;
            void setAreaId(const string &areaId);
            string getAreaId() const;
            void setMoreDataFlag(const UI32 &moreDataFlag);
            UI32 getMoreDataFlag() const;

            string  getVRF() const;
            void setVRF(const string  &vrfName);
            void setSequenceNumIntVal(char *hex_str);
            void getOutputBufferRef (UI32   bufferNum,UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);
// Now the data members

        private:
            UI32        m_cmdCode;
            string      m_areaId;
            IpV4Address m_ipAddr1;
            IpV4Address m_ipAddr2;
            UI32	m_intVal;
            string	m_strVal;
            string	m_ifName;
            UI32	m_moreDataFlag;
            string	m_vrf;
        protected:
        public:
    };
}
#endif                                            //OSPFLocalShowMessage_H