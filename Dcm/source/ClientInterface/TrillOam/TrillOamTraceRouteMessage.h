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
 *   Author : Manju Mahajan                                                *
 **************************************************************************/

#ifndef TRILLOAMTRACEROUTE_H
#define TRILLOAMTRACEROUTE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class TrillOamTraceRouteMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
			virtual void setupAttributesForSerialization();
        public:
            TrillOamTraceRouteMessage ();
            TrillOamTraceRouteMessage (const MacAddress2 &srcMac,
				       const MacAddress2 &dstMac, 
				       const UI32 &vlan,  
				       const UI32 &rbridgeid, 
				       const string &protocol, 
				       const IpV4Address &srcIp, 
				       const IpV4Address &dstIp,
				       const UI32 &srcPort, 
				       const UI32 &dstPort, 
				       const UI32 &cmdCode,
                       const UI32 &sessionid, 
				       const UI32 &begin, 
				       const UI32 &last);
            virtual    ~TrillOamTraceRouteMessage ();

			void  setVlan(const UI32 &vlan);
			UI32  getVlan() const;

			void  setRbridgeid(const UI32 &vlan);
			UI32  getRbridgeid() const;

			void  setSrcPort(const UI32 &srcPort);
			UI32  getSrcPort() const;
			void  setDstPort(const UI32 &dstPort);
			UI32  getDstPort() const;


			MacAddress2  getSrcMac() const;
			void  setSrcMac(const MacAddress2 &srcMac);
									 
			MacAddress2  getDstMac() const;
			void  setDstMac(const MacAddress2 &dstMac);

			string  getProtocol() const;
			void  setProtocol(const string &protocol);

			IpV4Address  getSrcIp() const;
			void  setSrcIp(const IpV4Address &srcIp);

			IpV4Address  getDstIp() const;
			void  setDstIp(const IpV4Address &dstIp);

			void setMsgStatus (const  UI32 &last);
			UI32  getMsgStatus () const;

            void SetSessionId (const  UI32 &sessionid);
            UI32  getSessionId () const;

			void setBegin (const  UI32 &begin);
			UI32  getBegin () const;
			void setCmdCode(const UI32 &cmdCode);
			UI32 getCmdCode()  const;

			virtual const void *getCStructureForInputs    ();
			virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
			//void getBackendData(UI32 &size, void *&pBuffer) ;
			//UI16  getTrillDa() const;
			//UI16  getEgid() const;

			//string getIngress() const;
            //string getEgress() const;
			//void SetIngress( const string &ingress) ;
			//void SetEgress( const string &egress) ;


        private:
        protected:
        public:
			MacAddress2 m_srcMac;
			MacAddress2 m_dstMac;
			UI32 m_vlan;
			UI32 m_rbridgeid;
			string m_protocol;
			IpV4Address m_srcIp;
			IpV4Address m_dstIp;
			UI32 m_srcPort; 
			UI32 m_dstPort;
			UI32  m_cmdCode;
			//UI16  m_trillDa;
			//UI16  m_Egid;
			//string m_Ingress;
            //string m_Egress;
            UI32  m_sessionid;
			UI32  m_begin;
			UI32  m_last;
    };
}
#endif
