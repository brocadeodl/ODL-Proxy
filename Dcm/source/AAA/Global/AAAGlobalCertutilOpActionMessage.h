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
 *   Author : Kumar Ghanta                                                 *
 **************************************************************************/

#ifndef AAAGLOBALCERTUTILOPACTIONMESSAGE_H
#define AAAGLOBALCERTUTILOPACTIONMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalCertutilOpActionMessage : public PrismMessage
{
  /* Data members*/
  private:
	string m_fosCmd;
	CertType m_certType;

  /* Member functions */
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAAGlobalCertutilOpActionMessage ();
	AAAGlobalCertutilOpActionMessage (const string &fosCmd, const CertType &certType);
	virtual ~AAAGlobalCertutilOpActionMessage ();
	void setFosCmd(const string &fosCmd);
	string getFosCmd();
	void setCertType(const CertType &certType);
	CertType getCertType();
};

}

#endif
