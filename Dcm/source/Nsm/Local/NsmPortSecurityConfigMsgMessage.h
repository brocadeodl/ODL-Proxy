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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : skrastog                                                     *
 **************************************************************************/

#ifndef NSMPORTSECURITYCONFIGMSGMESSAGE_H
#define NSMPORTSECURITYCONFIGMSGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmPortSecurityConfigMsgMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmPortSecurityConfigMsgMessage ();
            NsmPortSecurityConfigMsgMessage (const UI8 &opCode,const UI32 &noAllowedMacs,const UI8 &action,const string &mac);
            virtual    ~NsmPortSecurityConfigMsgMessage ();
            void setOpCode(const UI32 &opCode);
            UI32 getOpCode()  const;
            void setNumAllowedMacs(const UI32 &numAllowedMacs);
            UI32 getNumAllowedMacs()  const;
            void setAction(const UI32 &action);
            UI32 getAction()  const;
            void setMac(const string &mac);
            string getMac() const;
            void setOui(const string &oui);
            string getOui() const;
			void setIfName (const string &ifName) ;
			string getIfName () const;
			void setIfType (const InterfaceType &ifType) ;
			InterfaceType getIfType () const;
			void setSticky(const bool &sticky);
			bool getSticky() const;
			void setVlanId(const UI32 &vlanId);
			UI32 getVlanId() const;
			void setShutdownTime(const UI32 &time);
			UI32 getShutdownTime() const;

// Now the data members

        private:
        protected:
        public:
            UI32 m_opCode;
            UI32 m_numAllowedMacs;
            UI32 m_action;
            string m_mac;
			UI32 m_vlanId;
            string m_oui;
			string m_ifName;
			InterfaceType m_ifType;
			bool m_sticky;
			UI32 m_shutdownTime;
    };
}
#endif                                            //NSMPORTSECURITYCONFIGMSGMESSAGE_H
