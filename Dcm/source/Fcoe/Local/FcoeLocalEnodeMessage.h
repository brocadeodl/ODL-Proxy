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
 *   Author : MMREDDY                                                     *
 **************************************************************************/

#ifndef FCOE_LOCAL_FCOE_ENODE_MESSAGE_H
#define FCOE_LOCAL_FCOE_ENODE_MESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeLocalEnodeMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeLocalEnodeMessage ();
            FcoeLocalEnodeMessage (const UI32 &oper_bmp, const uint32_t &fcoe_enodes);
            virtual    ~FcoeLocalEnodeMessage ();
            void setFcoeEnodes(const uint32_t &fcoe_enodes);
            uint32_t getFcoeEnodes() const;
            void setFcoeEnodesOperBmp(const UI32 &oper_bmp);
            UI32 getFcoeEnodesOperBmp();
        // Now the data members

        private:
        protected:
        public:
            UI32    m_oper_bmp;
            uint32_t    m_fcoe_enodes;
    };
}
#endif                                            //FCOE_LOCAL_FCOE_ENODE_MESSAGE_H

