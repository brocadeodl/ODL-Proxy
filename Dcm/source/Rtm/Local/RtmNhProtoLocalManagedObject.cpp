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
 *   Author : hzhu                                                     *
 **************************************************************************/

#include "Rtm/Local/RtmNhProtoLocalManagedObject.h"
#include "Rtm/Local/RtmLocalObjectManager.h"
#include "Rtm/Local/RtmTypes.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    RtmNhProtoLocalManagedObject::RtmNhProtoLocalManagedObject (RtmLocalObjectManager *pRtmLocalObjectManager)
        : PrismElement  (pRtmLocalObjectManager),
        PrismPersistableObject (RtmNhProtoLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pRtmLocalObjectManager),
        DcmManagedObject (pRtmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pRtmLocalObjectManager)
    {
    }

    RtmNhProtoLocalManagedObject::~RtmNhProtoLocalManagedObject ()
    {
    }

    string  RtmNhProtoLocalManagedObject::getClassName()
    {
        return ("RtmNhProtoLocalManagedObject");
    }

    void  RtmNhProtoLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum(&m_nhProto,"nhProto",rtm_proto));
    }

    void  RtmNhProtoLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeEnum(&m_nhProto,"nhProto",rtm_proto));
    }

    void  RtmNhProtoLocalManagedObject::setNhProto(const UI32 &nhProto)
    {
        m_nhProto  =  nhProto;
    }

    UI32  RtmNhProtoLocalManagedObject::getNhProto() const
    {
        return (m_nhProto);
    }

	void  RtmNhProtoLocalManagedObject::setupKeys()
	{
		vector<string> keyName;
		keyName.push_back("nhProto");
		setUserDefinedKeyCombination (keyName);
	}

}
