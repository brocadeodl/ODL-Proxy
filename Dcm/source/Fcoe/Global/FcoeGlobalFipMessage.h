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

#ifndef FCOE_GLOBAL_FCOE_FIPMESSAGE_H
#define FCOE_GLOBAL_FCOE_FIPMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeGlobalFipMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeGlobalFipMessage ();
            FcoeGlobalFipMessage (const uint32_t &oper_bmp, const SI32 &fka_intvl, const bool &timeout);
            virtual    ~FcoeGlobalFipMessage ();
            void setFka_intvl(const SI32 &fka_intvl);
            SI32 getFka_intvl()  const;
            void setOperBmp(const uint32_t &oper_bmp);
            uint32_t getOperBmp() const;
            void setTimeout(const bool &timeout);
            bool getTimeout() const;

        // Now the data members
        private:
        protected:
        public:
            uint32_t    m_oper_bmp;
            SI32  m_fka_intvl;
            bool  m_timeout;
    };
}
#endif                                            //FCOE_GLOBAL_FCOE_FIPMESSAGE_H
