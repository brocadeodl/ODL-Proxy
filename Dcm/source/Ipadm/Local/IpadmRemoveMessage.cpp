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
 *   Author : cshah                                                     *
 **************************************************************************/

#include "Ipadm/Local/IpadmLocalObjectManager.h"
#include "Ipadm/Local/IpadmRemoveMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmRemoveMessage::IpadmRemoveMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMREMOVECONFIG)
    {
    }

    IpadmRemoveMessage::IpadmRemoveMessage (const string &switchid,const UI32 &tag)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMREMOVECONFIG),
        m_switchid    (switchid),
        m_tag    (tag)
    {
    }

    IpadmRemoveMessage::~IpadmRemoveMessage ()
    {
    }

    void  IpadmRemoveMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeUI32(&m_tag,"tag"));
    }

    void  IpadmRemoveMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmRemoveMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmRemoveMessage::setTag(const UI32 &tag)
    {
        m_tag  =  tag;
    }

    UI32 IpadmRemoveMessage::getTag() const
    {
        return (m_tag);
    }

}
