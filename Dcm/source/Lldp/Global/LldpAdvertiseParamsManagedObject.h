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

#ifndef LLDPADVERTISEPARAMSMANAGEDOBJECT_H
#define LLDPADVERTISEPARAMSMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "Framework/Attributes/AttributeEnum.h"

using namespace WaveNs;

namespace DcmNs
{

    class LldpGlobalObjectManager;
    class LldpAdvertiseParamsManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            LldpAdvertiseParamsManagedObject (LldpGlobalObjectManager *pLldpGlobalObjectManager);
            LldpAdvertiseParamsManagedObject (LldpGlobalObjectManager *pLldpGlobalObjectManager,const string &description,const UI32 &hello,const LldpMode &mode,const UI32 &multiplier,const bool &dcbxFcoeAppTlv,const bool &dcbxFcoeLogicalLinkTlv,const bool &dcbxIscsiAppTlv,const UI32 &iscsiPriority,const bool &dcbxTlv,const bool &dot1xTlv,const bool &dot3xTlv,const bool &mgmtAddrTlv,const bool &portDescrTlv,const bool &sysCapTlv,const bool &sysDescrTlv,const bool &sysNameTlv);
            virtual    ~LldpAdvertiseParamsManagedObject ();
            static string getClassName();
            void setDescription(const string &description);
            string getDescription()  const;
            void setHello(const UI32 &hello);
            UI32 getHello()  const;
            void setMode(const LldpMode &mode);
            LldpMode getMode()  const;
            void setMultiplier(const UI32 &multiplier);
            UI32 getMultiplier()  const;
            void setDcbxFcoeAppTlv(const bool &dcbxFcoeAppTlv);
            bool getDcbxFcoeAppTlv()  const;
            void setDcbxFcoeLogicalLinkTlv(const bool &dcbxFcoeLogicalLinkTlv);
            bool getDcbxFcoeLogicalLinkTlv()  const;
			void setDcbxIscsiAppTlv(const bool &dcbxIscsiAppTlv);
			bool getDcbxIscsiAppTlv()  const;
            void setIscsiPriority(const UI32 &iscsiPriority);
            UI32 getIscsiPriority()  const;
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
            string  m_description;
            UI32  m_hello;
            LldpMode  m_mode;
            UI32  m_multiplier;
            bool  m_dcbxFcoeAppTlv;
            bool  m_dcbxFcoeLogicalLinkTlv;
            bool  m_dcbxIscsiAppTlv;
            UI32  m_iscsiPriority;
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
#endif                                            //LLDPADVERTISEPARAMSMANAGEDOBJECT_H
