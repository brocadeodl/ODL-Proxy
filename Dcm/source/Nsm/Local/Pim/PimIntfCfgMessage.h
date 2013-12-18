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
 *   Author : rjain                                                     *
 **************************************************************************/

#ifndef PIMPIMINTFCFGMESSAGE_H
#define PIMPIMINTFCFGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimIntfCfgMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PimIntfCfgMessage ();
            virtual    ~PimIntfCfgMessage ();
            void setMsgType(const UI8 &msgType);
            UI8 getMsgType()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;
            void setIntfType(const UI8 &intfType);
            UI8 getIntfType()  const;
            void setIntfName(const string &intfName);
            string getIntfName()  const;
            void setEnable(const bool &enable);
            bool getEnable()  const;
            void setDrPriority(const UI32 &drPriority);
            UI32 getDrPriority()  const;
			/* Changing the type as we dont accept the acl name now */
#if 0
            void setMultBoundaryAcl(const string &multBoundaryAcl);
            string getMultBoundaryAcl()  const;
#endif
			void setMultBoundaryAcl(const bool &multBoundaryAcl);
			bool getMultBoundaryAcl() const;

            void setPimBorder(const bool &pimBorder);
            bool getPimBorder()  const;
            void setTtlThold(const UI32 &ttlThold);
            UI32 getTtlThold() const;

// Now the data members

        private:
        protected:
        public:
            UI8  m_msgType;
            UI8  m_cmdCode;
            UI8  m_intfType;
            string m_intfName;
            bool  m_enable;
            UI32  m_drPriority;
			/* Changing the type as we dont accept the acl name now */
            //string  m_multBoundaryAcl;
			bool m_multBoundaryAcl;
            bool  m_pimBorder;
            UI32 m_ttlThold;
    };
}
#endif                                            //PIMPIMINTFCFGMESSAGE_H
