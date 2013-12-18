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
 *   Author : mmahajan                                                     *
 **************************************************************************/

#ifndef TRILLOAMTRILLOAMTRACEROUTEREQUESTMESSAGE_H
#define TRILLOAMTRILLOAMTRACEROUTEREQUESTMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/MacAddress2.h"
#include "Framework/Types/MacAddress.h"
#include "Framework/Types/IpV4Address.h"

using namespace WaveNs;

namespace DcmNs
{

    class TrillOamTrillOamTracerouteRequestMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            TrillOamTrillOamTracerouteRequestMessage ();
            TrillOamTrillOamTracerouteRequestMessage (const MacAddress2 &srcmac,const MacAddress2 &destmac,const UI32 &vlanid,const UI32 &rbridgeid,const string &protocol,const IpV4Address &srcip,const IpV4Address &destip,const UI32 &srcport,const UI32 &destport, const UI32 &sessionid, const UI32 &begin, const UI32 &last);
            virtual    ~TrillOamTrillOamTracerouteRequestMessage ();
            void setSrcmac(const MacAddress2 &srcmac);
            MacAddress2 getSrcmac()  const;
            void setDestmac(const MacAddress2 &destmac);
            MacAddress2 getDestmac()  const;
            void setVlanid(const UI32 &vlanid);
            UI32 getVlanid()  const;
            void setRbridgeid(const UI32 &rbridgeid);
            UI32 getRbridgeid()  const;
            void setProtocol(const string &protocol);
            string getProtocol()  const;
            void setSrcip(const IpV4Address &srcip);
            IpV4Address getSrcip()  const;
            void setDestip(const IpV4Address &destip);
            IpV4Address getDestip()  const;
            void setSrcport(const UI32 &srcport);
            UI32 getSrcport()  const;

            void setDestport(const UI32 &destport);
            UI32 getDestport()  const;

            void setBegin(const UI32 &begin);
            UI32 getBegin()  const;

            void setMsgStatus(const UI32 &last);
            UI32 getMsgStatus()  const;
            //UI16  getTrillDa(UI16 result_index) const;
            //UI16  getEgid(UI16 result_index) const;
			//string getIngress(UI16 result_index) const;
		    //string getEgress(UI16 result_index) const;	
            //UI32  getRttime(UI16 result_index) const;
    		//void SetTrillDa(UI16 result_index, const UI16 &trillDa) ;
	    	//void SetEgid( UI16 result_index, const UI16 &egid) ;
		    //void SetIngress( UI16 result_index, const string &ingress) ;
    		//void SetEgress( UI16 result_index, const string &egress) ;
            //void SetRttime( UI16 result_index, const UI32 &rttime) ;
            void SetSessionId(const UI32 &sessionid);
            UI32 getSessionId() const;
            //UI32 getResultCount() const;
            //void SetResultCount(const UI32 &resultcount);

            void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);




        private:
        protected:
        public:
            MacAddress2  m_srcmac;
            MacAddress2  m_destmac;
            UI32  m_vlanid;
            UI32  m_rbridgeid;
            string  m_protocol;
            IpV4Address  m_srcip;
            IpV4Address  m_destip;
            UI32  m_srcport;
            UI32  m_destport;

            UI32  m_sessionid;
            UI32  m_begin;
            UI32  m_last;
    };

}
#endif                                            //TRILLOAMTRILLOAMTRACEROUTEREQUESTMESSAGE_H
