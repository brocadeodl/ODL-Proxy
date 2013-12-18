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

#ifndef FCOE_CLIENT_DEBUGMESSAGE_H
#define FCOE_CLIENT_DEBUGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeClientDebugMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeClientDebugMessage ();
            FcoeClientDebugMessage (const UI32 &oper_bmp,
									const UI32 &send_type,
									const UI32 &service_type);

            virtual    ~FcoeClientDebugMessage ();
            virtual const void *getCStructureForInputs    ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure) {
				return ;
			}

            void setOper_bmp(const UI32 &oper_bmp);
            UI32 getOper_bmp()  const;

            void setSend_type(const UI32 &send_type);
            UI32 getSend_type()  const;

            void setServ_type(const UI32 &service_type);
            UI32 getServ_type()  const;

// Now the data members

        private:
        protected:
        public:
            UI32 m_oper_bmp;
			UI32 m_send_type;
			UI32 m_service_type;
    };
}
#endif                                            //FCOE_CLIENT_FIPMESSAGE_H
