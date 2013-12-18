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

#ifndef AAAGLOBALADDTACACSHOSTMESSAGE_H
#define AAAGLOBALADDTACACSHOSTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalAddTacacsHostMessage : public PrismMessage
{
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAAGlobalAddTacacsHostMessage ();
	AAAGlobalAddTacacsHostMessage (const string &host, const SI32 &port, const SI8 &protocol, const string &key, const SI8 &retries, const SI8 &timeout);
	virtual ~AAAGlobalAddTacacsHostMessage ();
	void setHost(const string &host);
	string getHost() const;
	void setPort(const SI32 &port);
	SI32 getPort() const;
	void setProtocol(const SI8 &protocol);
	SI8 getProtocol() const;
	void setKey(const string &key);
	string getKey() const;
	void setRetries(const SI8 &retries);
	SI8 getRetries() const;
	void setTimeout(const SI8 &timeout);
	SI8 getTimeout() const;

  /* Data Members */
  private:
	string m_host;
	SI32 m_port;
	SI8 m_protocol;
	string m_key;
	SI8 m_retries;
	SI8 m_timeout;
};

}

#endif // AAAGLOBALADDTACACSHOSTMESSAGE_H
