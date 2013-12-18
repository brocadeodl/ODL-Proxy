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

#ifndef LLDPGLOBALPROFILECONFSETMESSAGE_H
#define LLDPGLOBALPROFILECONFSETMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/UI32UC.h"
#include "Framework/Types/StringUC.h"
#include "Framework/Types/BoolUC.h"

using namespace WaveNs;

namespace DcmNs
{

    class LldpGlobalProfileConfSetMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LldpGlobalProfileConfSetMessage ();
            LldpGlobalProfileConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const string &profileName,const StringUC &description,const UI32UC &hello,const EnumUC &mode,const UI32UC &multiplier,const BoolUC &dcbxFcoeAppTlv,const BoolUC &dcbxFcoeLogicalLinkTlv,const BoolUC &dcbxIscsiAppTlv,const BoolUC &dcbxTlv,const BoolUC &dot1xTlv,const BoolUC &dot3xTlv,const BoolUC &mgmtAddrTlv,const BoolUC &portDescrTlv,const BoolUC &sysCapTlv,const BoolUC &sysDescrTlv,const BoolUC &sysNameTlv);
            virtual    ~LldpGlobalProfileConfSetMessage ();
            void setMgmtFlags(const UI32 &mgmtFlags);
            UI32 getMgmtFlags()  const;
            void setCmdcode(const UI32 &cmdcode);
            UI32 getCmdcode()  const;
            void setMsgType(const UI32 &msgType);
            UI32 getMsgType()  const;
            void setProfileName(const string &profileName);
            string getProfileName()  const;
            void setDescription(const StringUC &description);
            StringUC getDescription()  const;
            void setHello(const UI32UC &hello);
            UI32UC getHello()  const;
            void setMode(const EnumUC &mode);
            EnumUC getMode()  const;
            void setMultiplier(const UI32UC &multiplier);
            UI32UC getMultiplier()  const;
            void setDcbxFcoeAppTlv(const BoolUC &dcbxFcoeAppTlv);
            BoolUC getDcbxFcoeAppTlv()  const;
            void setDcbxFcoeLogicalLinkTlv(const BoolUC &dcbxFcoeLogicalLinkTlv);
            BoolUC getDcbxFcoeLogicalLinkTlv()  const;
			void setDcbxIscsiAppTlv(const BoolUC &dcbxIscsiAppTlv);
			BoolUC getDcbxIscsiAppTlv()  const;
            void setDcbxTlv(const BoolUC &dcbxTlv);
            BoolUC getDcbxTlv()  const;
            void setDot1xTlv(const BoolUC &dot1xTlv);
            BoolUC getDot1xTlv()  const;
            void setDot3xTlv(const BoolUC &dot3xTlv);
            BoolUC getDot3xTlv()  const;
            void setMgmtAddrTlv(const BoolUC &mgmtAddrTlv);
            BoolUC getMgmtAddrTlv()  const;
            void setPortDescrTlv(const BoolUC &portDescrTlv);
            BoolUC getPortDescrTlv()  const;
            void setSysCapTlv(const BoolUC &sysCapTlv);
            BoolUC getSysCapTlv()  const;
            void setSysDescrTlv(const BoolUC &sysDescrTlv);
            BoolUC getSysDescrTlv()  const;
            void setSysNameTlv(const BoolUC &sysNameTlv);
            BoolUC getSysNameTlv()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_mgmtFlags;
            UI32  m_cmdcode;
            UI32  m_msgType;
            string  m_profileName;
            StringUC  m_description;
            UI32UC  m_hello;
            EnumUC  m_mode;
            UI32UC  m_multiplier;
			BoolUC  m_dcbxFcoeAppTlv;
            BoolUC  m_dcbxFcoeLogicalLinkTlv;
			BoolUC  m_dcbxIscsiAppTlv;
            BoolUC  m_dcbxTlv;
            BoolUC  m_dot1xTlv;
            BoolUC  m_dot3xTlv;
            BoolUC  m_mgmtAddrTlv;
            BoolUC  m_portDescrTlv;
            BoolUC  m_sysCapTlv;
            BoolUC  m_sysDescrTlv;
            BoolUC  m_sysNameTlv;
    };
}
#endif                                            //LLDPGLOBALPROFILECONFSETMESSAGE_H
