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

#ifndef FCOE_CLIENT_MAPMESSAGE_H
#define FCOE_CLIENT_MAPMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeClientMapMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeClientMapMessage ();
            FcoeClientMapMessage (const UI32 &oper_bmp,const string &map_name,const string &fabric_map_name,const string &cee_map_name);
            virtual    ~FcoeClientMapMessage ();
            void setOper_bmp(const UI32 &oper_bmp);
            UI32 getOper_bmp()  const;
            void setMap_name(const string &map_name);
            string getMap_name()  const;
            void setFabric_map_name(const string &fabric_map_name);
            string getFabric_map_name()  const;
            void setCee_map_name(const string &cee_map_name);
            string getCee_map_name()  const;
            virtual const void *getCStructureForInputs    ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure) {
				return ;
			}

// Now the data members

        private:
        protected:
        public:
            UI32  m_oper_bmp;
            string  m_map_name;
            string  m_fabric_map_name;
            string  m_cee_map_name;
    };
}
#endif                                            //FCOE_CLIENT_MAPMESSAGE_H
