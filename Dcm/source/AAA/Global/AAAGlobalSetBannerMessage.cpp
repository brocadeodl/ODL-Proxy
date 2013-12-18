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

#include "AAA/Global/AAAGlobalSetBannerMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

AAAGlobalSetBannerMessage::AAAGlobalSetBannerMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALSETBANNER)
{
}

AAAGlobalSetBannerMessage::AAAGlobalSetBannerMessage (const string &banner, const bool &reset, const UI8 &bannerType)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALSETBANNER),
	m_banner    (banner),
	m_reset (reset),
        m_bannerType (bannerType)
{
}

AAAGlobalSetBannerMessage::~AAAGlobalSetBannerMessage ()
{
}

void  AAAGlobalSetBannerMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_banner,"banner"));
	addSerializableAttribute (new AttributeBool(&m_reset,"reset"));
        addSerializableAttribute (new AttributeUI8(&m_bannerType,"bannerType"));
}

void  AAAGlobalSetBannerMessage::setBanner(const string &banner)
{
	m_banner  =  banner;
}

string  AAAGlobalSetBannerMessage::getBanner() const
{
	return (m_banner);
}

void  AAAGlobalSetBannerMessage::setReset(const bool &reset)
{
	m_reset  =  reset;
}

bool  AAAGlobalSetBannerMessage::getReset() const
{
	return (m_reset);
}

void  AAAGlobalSetBannerMessage::setBannerType(const UI8 &bannerType)
{
        m_bannerType  =  bannerType;
}

UI8  AAAGlobalSetBannerMessage::getBannerType() const
{
        return (m_bannerType);
}

}
