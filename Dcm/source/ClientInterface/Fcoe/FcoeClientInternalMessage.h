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
 *   Author : Jaideep Singh Saluja                                         *
 **************************************************************************/

#ifndef FCOECLIENTINTERNALMESSAGE_H
#define FCOECLIENTINTERNALMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{   

    class FcoeClientInternalMessage : public DcmManagementInterfaceMessage
    {   
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeClientInternalMessage ();
            FcoeClientInternalMessage (const UI32 &cmdCode,
                                   const UI32 &vlan,
                                   const UI32 &vfid,
                                   const UI32 &fc_uport,
                                   const string &intf_name,
                                   const UI64 &if_index);

            virtual    ~FcoeClientInternalMessage ();
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

            UI32 getVlan() const;
            void setVlan(const UI32 &vlan);
            UI32 getVfid() const;
            void setVfid(const UI32 &vfid);
            UI32 getFcUport() const;
            void setFcUport(const UI32 &fc_uport);
            string getIfName() const;
            void setIfName(const string &intf_name);
            UI64 getIfindex() const;
            void setIfindex(const UI64 &if_index);

            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);

            void getOutputBufferRef (UI32   bufferNum,
                                    UI32 &size, void *&pBuffer);
// Now the data members

        private:
        protected:
        public:
            UI32    m_cmdCode;
            UI32    m_vlan;
            UI32    m_vfid;
            UI32    m_fc_uport;
            string  m_intf_name;
            UI64    m_if_index;
    };
}
#endif                                            //FCOECLIENTINTERNALMESSAGE_H

