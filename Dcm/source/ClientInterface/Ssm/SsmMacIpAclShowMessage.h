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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Bhavin                                                        *
 **************************************************************************/

#ifndef SSMMACIPACLSHOWMESSAGE_H
#define SSMMACIPACLSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmMacIpAclShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmMacIpAclShowMessage();
            SsmMacIpAclShowMessage(const string &PolicyName, const string &ifName, const UI32 &opCode,
				const UI32 &aclType, const UI32 &direction, const UI32 &mappedId);
            SsmMacIpAclShowMessage(const string &PolicyName, const string &ifName, const UI32 &opCode,
				const UI32 &aclType, const UI32 &direction, const UI32 &mappedId, const UI64 &bindOid);
            virtual ~SsmMacIpAclShowMessage();
			
			void   setPolicyName  (const string &PolicyName);
			string getPolicyName  ()  const;

			void   setIfName      (const string &ifName);
			string getIfName      () const;

			void   setOpCode      (const UI32 &opCode);
			UI32   getOpCode      ()  const;

			void   setAclType     (const UI32 &aclType);
			UI32   getAclType     () const;

			void   setDirection   (const UI32 &direction);
			UI32   getDirection   () const;

			void   setMappedId 	  (const UI32 &mappedId);
			UI32   getMappedId 	  ()  const;

			void   setBindOid 	  (const UI64 &bindOid);
			UI64   getBindOid 	  ()  const;

			
            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void *pOutputCStructure);

        private:
        protected:
        public:
			string  m_PolicyName;
			string  m_ifName;
			UI32    m_opCode;
			UI32    m_aclType;
			UI32    m_direction;
			UI32 	m_mappedId;
			UI64    m_bindOid;
    };
}
#endif  //SSMMACIPACLSHOWMESSAGE_H
