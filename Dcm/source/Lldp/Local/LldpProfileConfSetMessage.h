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
 *   Author : mmohan                                                     *
 **************************************************************************/

#ifndef LLDPPROFILECONFSETMESSAGE_H
#define LLDPPROFILECONFSETMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class LldpProfileConfSetMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LldpProfileConfSetMessage ();
            LldpProfileConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const string &ProfileName,const string &description,const UI32 &hello,const UI8 &mode,const UI32 &multiplier,const bool &dcbxFcoeAppTlv,const bool &dcbxFcoeLogicalLinkTlv,const bool &dcbxIscsiAppTlv,const bool &dcbxTlv,const bool &dot1xTlv,const bool &dot3xTlv,const bool &mgmtAddrTlv,const bool &portDescrTlv,const bool &sysCapTlv,const bool &sysDescrTlv,const bool &sysNameTlv);
            virtual    ~LldpProfileConfSetMessage ();
            void setMgmtFlags(const UI32 &mgmtFlags);
            UI32 getMgmtFlags()  const;
            void setCmdcode(const UI32 &cmdcode);
            UI32 getCmdcode()  const;
            void setMsgType(const UI32 &msgType);
            UI32 getMsgType()  const;
            void setProfileName(const string &ProfileName);
            string getProfileName()  const;
            void setDescription(const string &description);
            string getDescription()  const;
            void setHello(const UI32 &hello);
            UI32 getHello()  const;
            void setMode(const UI32 &mode);
            UI32 getMode()  const;
            void setMultiplier(const UI32 &multiplier);
            UI32 getMultiplier()  const;
            void setDcbxFcoeAppTlv(const bool &dcbxFcoeAppTlv);
            bool getDcbxFcoeAppTlv()  const;
            void setDcbxFcoeLogicalLinkTlv(const bool &dcbxFcoeLogicalLinkTlv);
            bool getDcbxFcoeLogicalLinkTlv()  const;
			void setDcbxIscsiAppTlv(const bool &dcbxIscsiAppTlv);
			bool getDcbxIscsiAppTlv()  const;
            void setDcbxTlv(const bool &dcbxTlv);
            bool getDcbxTlv()  const;
            void setDot1xTlv(const bool &dot1xTlv);
            bool getDot1xTlv()  const;
            void setDot3xTlv(const bool &dot3xTlv);
            bool getDot3xTlv()  const;
            void setMgmtAddrTlv(const bool &mgmtAddrTlv);
            bool getMgmtAddrTlv()  const;
            void setPortDescrTlv(const bool &portDescrTlv);
            bool getPortDescrTlv()  const;
            void setSysCapTlv(const bool &sysCapTlv);
            bool getSysCapTlv()  const;
            void setSysDescrTlv(const bool &sysDescrTlv);
            bool getSysDescrTlv()  const;
            void setSysNameTlv(const bool &sysNameTlv);
            bool getSysNameTlv()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_mgmtFlags;
            UI32  m_cmdcode;
            UI32  m_msgType;
            string  m_ProfileName;
            string  m_description;
            UI32  m_hello;
            UI32  m_mode;
            UI32  m_multiplier;
            bool  m_dcbxFcoeAppTlv;
            bool  m_dcbxFcoeLogicalLinkTlv;
			bool  m_dcbxIscsiAppTlv;
            bool  m_dcbxTlv;
            bool  m_dot1xTlv;
            bool  m_dot3xTlv;
            bool  m_mgmtAddrTlv;
            bool  m_portDescrTlv;
            bool  m_sysCapTlv;
            bool  m_sysDescrTlv;
            bool  m_sysNameTlv;
    };
}
#endif                                            //LLDPPROFILECONFSETMESSAGE_H
