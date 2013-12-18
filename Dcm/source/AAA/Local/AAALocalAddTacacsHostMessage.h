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

#ifndef AAALOCALADDTACACSHOSTMESSAGE_H
#define AAALOCALADDTACACSHOSTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class AAALocalAddTacacsHostMessage : public PrismMessage
{
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAALocalAddTacacsHostMessage ();
	AAALocalAddTacacsHostMessage (const string &host, const UI16 &port, const SI8 &protocol, const string &key, const SI8 &retries, const SI8 &timeout);
	virtual ~AAALocalAddTacacsHostMessage ();
	void setHost(const string &host);
	string getHost() const;
	void setPort(const UI16 &port);
	UI16 getPort() const;
	void setProtocol(const SI8 &protocol);
	SI8 getProtocol() const;
	void setKey(const string &key);
	string getKey() const;
	void setRetries(const SI8 &retries);
	SI8 getRetries() const;
	void setTimeout(const SI8 &timeout);
	SI8 getTimeout() const;

  /* Data members */
  private:
	string m_host;
	UI16 m_port;
	SI8 m_protocol;
	string m_key;
	SI8 m_retries;
	SI8 m_timeout;
};

}

#endif // AAALOCALADDTACACSHOSTMESSAGE_H
