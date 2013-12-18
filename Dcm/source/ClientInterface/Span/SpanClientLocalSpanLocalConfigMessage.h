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
 *   Author : Umakiran G                                                     *
 **************************************************************************/

#ifndef NSMCLIENTCREATESPANLOCALCONFIG_H
#define NSMCLIENTCREATESPANLOCALCONFIG_H

#include "ClientInterface/Nsm/NsmVlanProvisionMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SpanClientLocalSpanLocalConfigMessage : public NsmVlanProvisionMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SpanClientLocalSpanLocalConfigMessage ();
	    SpanClientLocalSpanLocalConfigMessage(const UI32 &session_id,const string &source_ifName,const string &dest_ifName,const UI32 &direction,const UI32 &opCode,const UI32 &messageType,const string &description, const UI32 &src_ethernet, const UI32 &dest_ethernet);
            virtual    ~SpanClientLocalSpanLocalConfigMessage ();
	    void setSession_id(const UI32 &session_id);
            UI32 getSession_id()  const;
            void setSource_ifName(const string &source_ifName);
            string getSource_ifName()  const;
            void setDest_ifName(const string &dest_ifName);
            string getDest_ifName()  const;
            void setDirection(const UI32 &direction);
            UI32 getDirection()  const;
            void setOpCode(const UI32 &opCode);
            UI32 getOpCode()  const;
            void setMessageType(const UI32 &messageType);
            UI32 getMessageType()  const;
            void setDescription(const string &description);
            string getDescription()  const;
            void setSrcethernet(const UI32 &src_ethernet);
            UI32 getSrcethernet() const;
            void setDestethernet(const UI32 &dest_ethernet);
            UI32 getDestethernet() const;

            void setTrillSA(const UI32 &trill_sa);
            UI32 getTrillSA() const;
            void setTrillDA(const UI32 &trill_da);
            UI32 getTrillDA() const;
            void setRspanVlanId(const UI32 &vlanId);
            UI32 getRspanVlanId()  const;

            virtual const void *getCStructureForInputs();
            virtual void  loadOutputsFromCStructure(const void *pOutputCStructure);
    		void getDuplicateInterfaceOutputBuffer (UI32 bufferNum,UI32 &size, void * &pBuffer);
	

// Now the data members

        private:
        protected:
        public:
		UI32  m_session_id;
                string  m_source_ifName;
                string  m_dest_ifName;
                UI32  m_direction;
                UI32  m_opCode;
                UI32  m_messageType;
                string  m_description;
	            UI32 m_src_ethernet;
    	        UI32 m_dest_ethernet;
    	        UI32 m_trill_sa;
    	        UI32 m_trill_da;
    	        UI32 m_rspan_vlanid;

    };
}
#endif                                            //NSMCLIENTCREATESPANLOCALCONFIG_H
