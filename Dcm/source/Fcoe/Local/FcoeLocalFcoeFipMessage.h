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

#ifndef FCOE_LOCAL_FCOE_FIPMESSAGE_H
#define FCOE_LOCAL_FCOE_FIPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeLocalFcoeFipMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeLocalFcoeFipMessage ();
            FcoeLocalFcoeFipMessage (const UI32 &oper_bmp,const UI32 &fka_intvl,const bool &timeout);
            virtual    ~FcoeLocalFcoeFipMessage ();
            void setOper_bmp(const UI32 &oper_bmp);
            UI32 getOper_bmp()  const;
            void setFka_intvl(const UI32 &fka_intvl);
            UI32 getFka_intvl()  const;
            void setTimeout(const bool &timeout);
            bool getTimeout()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_oper_bmp;
            UI32  m_fka_intvl;
            bool  m_timeout;
    };
}
#endif                                            //FCOE_LOCAL_FCOE_FIPMESSAGE_H
