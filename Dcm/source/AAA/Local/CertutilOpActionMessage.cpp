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

#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalTypes.h"
#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Local/CertutilOpActionMessage.h"
#include "Framework/Attributes/AttributeEnum.h"

using namespace WaveNs;

namespace DcmNs
{

CertutilOpActionMessage::CertutilOpActionMessage ()
    : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), CERTUTILOPACTION)
{
}

CertutilOpActionMessage::CertutilOpActionMessage (const string &fosCmd, const CertType &certType)
    : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), CERTUTILOPACTION),
	m_fosCmd(fosCmd),
	m_certType(certType)
{
}

CertutilOpActionMessage::~CertutilOpActionMessage ()
{
}

void CertutilOpActionMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_fosCmd, "foscmd"));
	addSerializableAttribute (new AttributeUI32((UI32*)&m_certType, "certType"));
}

void CertutilOpActionMessage::setFosCmd (const string &fosCmd)
{
	m_fosCmd = fosCmd;
}

string CertutilOpActionMessage::getFosCmd ()
{
	return m_fosCmd;
}

void CertutilOpActionMessage::setCertType(const CertType &certType)
{
	m_certType = certType;
}

CertType CertutilOpActionMessage::getCertType()
{
	return (m_certType);
}
}
