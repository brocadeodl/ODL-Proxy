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
 *   Author : Manju                                                     *
 **************************************************************************/

#ifndef TRILLOAMMESSAGE_H
#define TRILLOAMMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class TrillOamPingMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();

        public:
            TrillOamPingMessage ();
            TrillOamPingMessage (const MacAddress &srcMac,const MacAddress &dstMac, const UI32 &vlan, const UI32 &cmdCode);
            virtual    ~TrillOamPingMessage ();

			MacAddress  getDstMac() const;
			void  setDstMac(const MacAddress &dstMac);
			MacAddress  getSrcMac() const;
			void  setSrcMac(const MacAddress &srcMac);
			void  setVlan(const UI32 &vlan);
			UI32  getVlan() const;
			void setCmdCode(const UI32 &cmdCode);
			UI32 getCmdCode()  const;

			virtual const void *getCStructureForInputs ();
			virtual void  loadOutputsFromCStructure (const void *pOutputCStructure) {
													return ;
												}	


        private:
			MacAddress m_srcMac;
			MacAddress m_dstMac;
            UI32  m_vlan;
            UI32  m_cmdCode;
    };
}
#endif 
