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

#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalSetPasswordAttributesMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

    AAALocalSetPasswordAttributesMessage::AAALocalSetPasswordAttributesMessage ()
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALSETPASSWORDATTRIBUTES)
    {
    }

    AAALocalSetPasswordAttributesMessage::AAALocalSetPasswordAttributesMessage (const SI32 &minlength,const SI32 &maxretry,const SI32 &uppercase,const SI32 &lowercase,const SI32 &numerics,const SI32 &splchars, const SI32 &adminlockout)
        : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALSETPASSWORDATTRIBUTES),
        m_minlength    (minlength),
        m_maxretry    (maxretry),
        m_uppercase    (uppercase),
        m_lowercase    (lowercase),
        m_numerics    (numerics),
        m_splchars    (splchars),
		m_adminlockout (adminlockout)
    {
    }

    AAALocalSetPasswordAttributesMessage::~AAALocalSetPasswordAttributesMessage ()
    {
    }

    void  AAALocalSetPasswordAttributesMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_minlength,"minlength"));
        addSerializableAttribute (new AttributeSI32(&m_maxretry,"maxretry"));
        addSerializableAttribute (new AttributeSI32(&m_uppercase,"uppercase"));
        addSerializableAttribute (new AttributeSI32(&m_lowercase,"lowercase"));
        addSerializableAttribute (new AttributeSI32(&m_numerics,"numerics"));
        addSerializableAttribute (new AttributeSI32(&m_splchars,"splchars"));
		addSerializableAttribute (new AttributeSI32(&m_adminlockout,"adminlockout"));
    }

    void  AAALocalSetPasswordAttributesMessage::setMinlength(const SI32 &minlength)
    {
        m_minlength  =  minlength;
    }

    SI32  AAALocalSetPasswordAttributesMessage::getMinlength() const
    {
        return (m_minlength);
    }

    void  AAALocalSetPasswordAttributesMessage::setMaxretry(const SI32 &maxretry)
    {
        m_maxretry  =  maxretry;
    }

    SI32  AAALocalSetPasswordAttributesMessage::getMaxretry() const
    {
        return (m_maxretry);
    }

    void  AAALocalSetPasswordAttributesMessage::setUppercase(const SI32 &uppercase)
    {
        m_uppercase  =  uppercase;
    }

    SI32  AAALocalSetPasswordAttributesMessage::getUppercase() const
    {
        return (m_uppercase);
    }

    void  AAALocalSetPasswordAttributesMessage::setLowercase(const SI32 &lowercase)
    {
        m_lowercase  =  lowercase;
    }

    SI32  AAALocalSetPasswordAttributesMessage::getLowercase() const
    {
        return (m_lowercase);
    }

    void  AAALocalSetPasswordAttributesMessage::setNumerics(const SI32 &numerics)
    {
        m_numerics  =  numerics;
    }

    SI32  AAALocalSetPasswordAttributesMessage::getNumerics() const
    {
        return (m_numerics);
    }

    void  AAALocalSetPasswordAttributesMessage::setSplchars(const SI32 &splchars)
    {
        m_splchars  =  splchars;
    }

    SI32  AAALocalSetPasswordAttributesMessage::getSplchars() const
    {
        return (m_splchars);
    }

	void AAALocalSetPasswordAttributesMessage::setAdminlockout(const SI32 &adminlockout)
	{
		m_adminlockout = adminlockout;
    }

	SI32 AAALocalSetPasswordAttributesMessage::getAdminlockout() const
	{   
		return (m_adminlockout);
	}

}
