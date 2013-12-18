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

#ifndef FCOE_GLOBAL_FCOE_ATTR_MESSAGE_H
#define FCOE_GLOBAL_FCOE_ATTR_MESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeGlobalFcoeportAttrMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeGlobalFcoeportAttrMessage ();
            FcoeGlobalFcoeportAttrMessage (const UI32 &oper_bmp, const string &if_name, const string &map_name);
            FcoeGlobalFcoeportAttrMessage (const UI32 &oper_bmp, const string &if_name, const string &map_name, const UI32 prov_context);
            virtual    ~FcoeGlobalFcoeportAttrMessage ();
            void setOper_bmp(const UI32 &oper_bmp);
            UI32 getOper_bmp()  const;
            UI32 get_prov_context()  const;
            void setMap_name(const string &map_name);
            string getMap_name() const;
            void setIfId(const string &if_id);
            string getIfId() const;

        // Now the data members
        private:
        protected:
        public:
            UI32    m_oper_bmp;
            string  m_if_id;
            string  m_map_name;
            UI32    m_prov_context;
    };
}

#endif
