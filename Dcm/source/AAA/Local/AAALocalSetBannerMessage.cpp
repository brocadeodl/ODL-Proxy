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
#include "AAA/Local/AAALocalSetBannerMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

AAALocalSetBannerMessage::AAALocalSetBannerMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALSETBANNER)
{
}

AAALocalSetBannerMessage::AAALocalSetBannerMessage (const string &banner, const UI8 &bannerType)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALSETBANNER),
	m_banner    (banner),
	m_bannerType  (bannerType)
{
}

AAALocalSetBannerMessage::~AAALocalSetBannerMessage ()
{
}

void  AAALocalSetBannerMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_banner,"banner"));
        addSerializableAttribute (new AttributeUI8(&m_bannerType,"bannerType"));
}

void  AAALocalSetBannerMessage::setBanner(const string &banner)
{
	m_banner  =  banner;
}

string  AAALocalSetBannerMessage::getBanner() const
{
	return (m_banner);
}

void  AAALocalSetBannerMessage::setBannerType(const UI8 &bannerType)
{
        m_bannerType  =  bannerType;
}

UI8  AAALocalSetBannerMessage::getBannerType() const
{
        return (m_bannerType);
}


}
