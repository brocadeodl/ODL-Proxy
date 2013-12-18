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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#ifndef FCOE_LOCAL_UPD_ET_INTFMESSAGE_H
#define FCOE_LOCAL_UPD_ET_INTFMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Fcoe/Common/FcoePluginTypes.h"
using namespace WaveNs;

namespace DcmNs
{

    class FcoeLocalETIntfMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeLocalETIntfMessage ();
            virtual    ~FcoeLocalETIntfMessage ();
            void setOper_bmp(const UI32 &oper_bmp);
            UI32 getOper_bmp()  const;
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setIfIndex(const UI64 &ifIndex);
            UI64 getIfIndex()  const;
            void setStatus(const bool &status);
            bool getStatus()  const;
            void setFcUportNum(const UI32 &fc_uport_num);
            UI32 getFcUportNum() const;
            void setTrunkStatus(const bool &status);
            bool getTrunkStatus()  const;


// Now the data members

        private:
        protected:
        public:
            UI32    m_oper_bmp;
            string  m_ifName;
            bool    m_status;
            bool    m_trunk_status;
            UI64    m_ifIndex;
            UI32    m_fc_uport_num;
    };
}
#endif                                            //FCOE_LOCAL_UPD_FCOE_INTFMESSAGE_H
