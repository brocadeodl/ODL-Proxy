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
 *   Author : Subhani Shaik                                                *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"

#include "Diag/Local/DiagLocalObjectManager.h"
#include "Diag/Local/DiagLocalPostMessage.h"
#include "Diag/Local/DiagLocalTypes.h"

namespace DcmNs
{

DiagLocalPostMessage::DiagLocalPostMessage ()
	: PrismMessage (DiagLocalObjectManager::getPrismServiceId (), DIAGLOCALPOST)
{
}

DiagLocalPostMessage::DiagLocalPostMessage (const UI32 &rbridgeId,const bool &enablePost)
	: PrismMessage (DiagLocalObjectManager::getPrismServiceId (), DIAGLOCALPOST),
	m_rbridgeId    (rbridgeId),
    m_enable    (enablePost)
{
}

DiagLocalPostMessage::~DiagLocalPostMessage ()
{
}

void DiagLocalPostMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeUI32(&m_rbridgeId,"rbridgeId"));
	addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
}

void  DiagLocalPostMessage::setrbridgeId(const UI32 &rbridgeId)
{
    m_rbridgeId  =  rbridgeId;
}

UI32  DiagLocalPostMessage::getrbridgeId() const
{
	return (m_rbridgeId);

}

void  DiagLocalPostMessage::setDiagPost(const bool &enablePost)
{
	m_enable  =  enablePost;
}

bool  DiagLocalPostMessage::getDiagPost() const
{
	return (m_enable);
}

}
