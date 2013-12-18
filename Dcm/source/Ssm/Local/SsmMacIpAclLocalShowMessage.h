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
 *   Author : Bhavin                                                     *
 **************************************************************************/

#ifndef SSMMACIPACLLOCALSHOWMESSAGE_H
#define SSMMACIPACLLOCALSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class SsmMacIpAclLocalShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmMacIpAclLocalShowMessage ();
            SsmMacIpAclLocalShowMessage (const string &PolicyName, const string &ifName, const UI32 &opCode, 
				bool isGwRequest, const UI32 &aclType, const UI32 &ifType, const UI32 &direction);
            SsmMacIpAclLocalShowMessage (const string &PolicyName, const string &ifName, const UI32 &opCode,
				bool isGwRequest, const UI32 &aclType, const UI32 &ifType, const UI32 &direction, const UI64 &bindOid);
            SsmMacIpAclLocalShowMessage (const string &PolicyName, const string &ifName, const UI32 &opCode,
				bool isGwRequest, const UI32 &aclType, const UI32 &ifType, const UI32 &direction, const UI64 &bindOid, const string &rbridgeId);
            virtual     ~SsmMacIpAclLocalShowMessage ();

            void   setPolicyName  (const string &PolicyName);
            string getPolicyName  ()  const;

            void   setIfName	  (const string &ifName);
            string getIfName	  () const;

            void   setOpCode      (const UI32 &opCode);
            UI32   getOpCode      ()  const;

            void   setIsGwRequest (bool isGwRequest);
            bool   getIsGwRequest ()  const;

			void   setAclType	  (const UI32 &aclType);
			UI32   getAclType 	  () const;

			void   setIfType	  (const UI32 &ifType);
            UI32   getIfType	  () const;

			void   setDirection   (const UI32 &direction);
			UI32   getDirection	  () const;

			void   setBindOid	  (const UI64 &bindOid);
			UI64   getBindOid     () const;

            void getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);

            void setRbridgeId(const string &rbridgeId);
            string getRbridgeId() const;
			
		// Now the data members

        private:
        protected:
        public:
            string  m_PolicyName;
            string  m_ifName;
            UI32    m_opCode;
            bool    m_isGwRequest;
			UI32	m_aclType;
            UI32    m_ifType;
			UI32	m_direction;
			UI64    m_bindOid;
			string  m_rbridgeId;
    };
}

#endif                                            //SSMMACIPACLLOCALSHOWMESSAGE_H


