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
 *   Author : tgajendr                                                     *
 **************************************************************************/

#include "LicenseService/Local/LicenseServiceLocalObjectManager.h"
#include "LicenseService/Local/LicenseServiceShow.h"
#include "LicenseService/Local/LicenseServiceTypes.h"

namespace DcmNs
{

    LicenseServiceShowMessage::LicenseServiceShowMessage ()
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICESHOW)
    {
    }

    LicenseServiceShowMessage::LicenseServiceShowMessage (const vector<string> &license,
        const vector<string> &expirydatestr,
    	const vector<string> &featureName,
    	const vector<string> &capacity,
    	const vector<string> &consumed,
        const vector<string> &validState)
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICESHOW),
        m_license(license),
        m_expirydatestr(expirydatestr),
    	m_featureName(featureName),
    	m_capacity(capacity),
    	m_consumed(consumed),
        m_validState(validState)
    {
    }

    LicenseServiceShowMessage::~LicenseServiceShowMessage ()
    {
    }

    void  LicenseServiceShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeStringVector(&m_license,"license"));
        addSerializableAttribute (new AttributeStringVector(&m_expirydatestr,"expirydatestr"));
    	addSerializableAttribute (new AttributeStringVector(&m_featureName,"featureName"));
    	addSerializableAttribute (new AttributeStringVector(&m_capacity,"capacity"));
    	addSerializableAttribute (new AttributeStringVector(&m_consumed,"consumed"));
        addSerializableAttribute (new AttributeStringVector(&m_validState,"validState"));
    }

    void  LicenseServiceShowMessage::setLicense(const vector<string> &license)
    {
        m_license  =  license;
    }

    vector<string>  LicenseServiceShowMessage::getLicense() const
    {
        return (m_license);
    }

    void  LicenseServiceShowMessage::setExpirydatestr(const vector<string> &expirydatestr)
    {
        m_expirydatestr  =  expirydatestr;
    }

    vector<string>  LicenseServiceShowMessage::getExpirydatestr() const
    {
        return (m_expirydatestr);
    }

    void  LicenseServiceShowMessage::setFeatureName(const vector<string> &featureName)
    {
	    m_featureName  =  featureName;
    }

    vector<string>  LicenseServiceShowMessage::getFeatureName() const
    {
	    return (m_featureName);
    }

    void  LicenseServiceShowMessage::setCapacity(const vector<string> &capacity)
    {
	    m_capacity  =  capacity;
    }

    vector<string>  LicenseServiceShowMessage::getCapacity() const
    {
	    return (m_capacity);
    }

    void  LicenseServiceShowMessage::setConsumed(const vector<string> &consumed)
    {
	    m_consumed  =  consumed;
    }

    vector<string>  LicenseServiceShowMessage::getConsumed() const
    {
	    return (m_consumed);
    }

    void  LicenseServiceShowMessage::setValidState(const vector<string> &validState)
    {
        m_validState  =  validState;
    }

    vector<string>  LicenseServiceShowMessage::getValidState() const
    {
        return (m_validState);
    }
}
