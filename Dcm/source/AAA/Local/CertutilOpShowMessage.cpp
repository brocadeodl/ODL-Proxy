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
#include "AAA/Local/CertutilOpShowMessage.h"
#include "Framework/Attributes/AttributeEnum.h"

using namespace WaveNs;

namespace DcmNs
{

CertutilOpShowMessage::CertutilOpShowMessage ()
    : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), CERTUTILOPSHOW)
{
}

CertutilOpShowMessage::CertutilOpShowMessage (const string &fosCmd, const CertType &certType, const vector<string> &showOutput)
    : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), CERTUTILOPSHOW),
	m_fosCmd(fosCmd),
	m_certType(certType),
    m_vShowOutput(showOutput)
{
}

CertutilOpShowMessage::~CertutilOpShowMessage ()
{
}

void CertutilOpShowMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_fosCmd, "foscmd"));
	addSerializableAttribute (new AttributeUI32((UI32*)&m_certType, "certType"));
	addSerializableAttribute (new AttributeStringVector(&m_vShowOutput, "vShowOutput"));
}

void CertutilOpShowMessage::setFosCmd (const string &fosCmd)
{
	m_fosCmd = fosCmd;
}

string CertutilOpShowMessage::getFosCmd ()
{
	return m_fosCmd;
}

void CertutilOpShowMessage::setCertType(const CertType &certType)
{
	m_certType = certType;
}

CertType CertutilOpShowMessage::getCertType()
{
	return (m_certType);
}


UI32 CertutilOpShowMessage::getNumShowOutput() const
{
    return (m_vShowOutput.size());
}

void CertutilOpShowMessage::clearNumVectors()
{
    m_vShowOutput.clear();
}

void CertutilOpShowMessage::setShowOutput(const string &showOutputLine)
{
    m_vShowOutput.push_back(showOutputLine);
}

bool CertutilOpShowMessage::getShowOutput(const UI32 &index,
    string &showOutputLine)
{
    if (index >= m_vShowOutput.size())
    {
        return false;
    }

    showOutputLine = m_vShowOutput[index];
    return true;
}

}
