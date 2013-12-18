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
 *   Author : ckunduru                                                     *
 **************************************************************************/

#ifndef AAALOCALCHANGERADIUSHOSTMESSAGE_H
#define AAALOCALCHANGERADIUSHOSTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class AAALocalChangeRadiusHostMessage : public PrismMessage
{
  protected:
	virtual void setupAttributesForSerialization();

  public:
	AAALocalChangeRadiusHostMessage ();
	AAALocalChangeRadiusHostMessage (const string &host, const SI32 &port, const UI32 &protocol, const string &key, const SI32 &retransmit, const SI32 &timeout);
	virtual    ~AAALocalChangeRadiusHostMessage ();
	void setHost(const string &host);
	string getHost()  const;
	void setPort(const SI32 &port);
	SI32 getPort()  const;
	void setProtocol(const UI32 &protocol);
	UI32 getProtocol()  const;
	void setKey(const string &key);
	string getKey()  const;
	void setRetransmit(const SI32 &retransmit);
	SI32 getRetransmit()  const;
	void setTimeout(const SI32 &timeout);
	SI32 getTimeout()  const;

  /* Data Members */
  private:
	string  m_host;
	SI32  m_port;
	UI32  m_protocol;
	string  m_key;
	SI32  m_retransmit;
	SI32  m_timeout;
};

}
#endif                                            //AAALOCALCHANGERADIUSHOSTMESSAGE_H
