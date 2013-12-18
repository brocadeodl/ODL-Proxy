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

#ifndef FCOE_GLOBAL_FCOE_MAPMESSAGE_H
#define FCOE_GLOBAL_FCOE_MAPMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeGlobalFcoeMapMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeGlobalFcoeMapMessage ();
            FcoeGlobalFcoeMapMessage (const string &name, const uint32_t &oper_bmp, const string &cee_map_name, const string &fabric_map_name);
            virtual    ~FcoeGlobalFcoeMapMessage ();
            void setName(const string &name);
            string getName()  const;
            void setOperBmp(const uint32_t &oper_bmp);
            uint32_t getOperBmp() const;
            void setFabricMapName(const string &fabric_map_name);
            string getFabricMapName() const;
            void setCeeMapName(const string &cee_map_name);
            string getCeeMapName() const;

// Now the data members

        private:
        protected:
        public:
            string  m_fcoe_map_name;
            uint32_t m_oper_bmp;
            string  m_cee_map_name;
            string  m_fabric_map_name;
    };
}
#endif                                            //FCOE_GLOBAL_FCOE_MAPMESSAGE_H
