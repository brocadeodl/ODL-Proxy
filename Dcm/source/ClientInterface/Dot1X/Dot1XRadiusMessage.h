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
 *   Author : Srikanth
 **************************************************************************/

#ifndef DOT1XRADIUSMESSAGE_H
#define DOT1XRADIUSMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class Dot1XRadiusMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            Dot1XRadiusMessage ();
            Dot1XRadiusMessage (const string &host,const UI16 &port,const UI32 &protocol,const string &key,const UI32 &retransmit, const UI32 &timeout, const UI32 &cmdCode);
            virtual ~Dot1XRadiusMessage ();
    void setHost(const string &host);
    string getHost()  const;
    void setPort(const UI16 &port);
    UI16 getPort()  const;
    void setProtocol(const UI32 &protocol);
    UI32 getProtocol()  const;
    void setKey(const string &key);
    string getKey()  const;
    void setRetransmit(const UI32 &retransmit);
    UI32 getRetransmit()  const;
    void setTimeout(const UI32 &timeout);
    UI32 getTimeout()  const;
	void setCmdCode(const UI32 &cmdCode);
	UI32 getCmdCode() const;

    virtual const void *getCStructureForInputs ();
    virtual void loadOutputsFromCStructure (const void 
							*pOutputCStructure);

    void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, 
												UI32 &size, void *&pBuffer);
	void getOutputBufferRef (UI32 bufferNum,
				UI32 &size, void *&pBuffer);

	 /* Data Members */
  private:
    string  m_host;
    UI16  m_port;
    UI32  m_protocol;
    string  m_key;
    UI32  m_retransmit;
    UI32  m_timeout;
	UI32 m_cmdCode;

    };
}
#endif  //Dot1XRadiusMessage_H


