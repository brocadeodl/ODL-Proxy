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

#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Global/AAAGlobalCertutilOpActionMessage.h"
#include "Framework/Attributes/AttributeEnum.h"

using namespace WaveNs;

namespace DcmNs
{

AAAGlobalCertutilOpActionMessage::AAAGlobalCertutilOpActionMessage ()
    : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALCERTUTILOPACTION)
{
}

AAAGlobalCertutilOpActionMessage::AAAGlobalCertutilOpActionMessage (const string &fosCmd, const CertType &certType)
    : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALCERTUTILOPACTION),
	m_fosCmd(fosCmd),
	m_certType(certType)
{
}

AAAGlobalCertutilOpActionMessage::~AAAGlobalCertutilOpActionMessage ()
{
}

void AAAGlobalCertutilOpActionMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_fosCmd, "foscmd"));
	addSerializableAttribute (new AttributeUI32((UI32*)&m_certType, "certType"));
}

void AAAGlobalCertutilOpActionMessage::setFosCmd (const string &fosCmd)
{
	m_fosCmd = fosCmd;
}

string AAAGlobalCertutilOpActionMessage::getFosCmd ()
{
	return m_fosCmd;
}

void AAAGlobalCertutilOpActionMessage::setCertType(const CertType &certType)
{
	m_certType = certType;
}

CertType AAAGlobalCertutilOpActionMessage::getCertType()
{
	return (m_certType);
}
}
