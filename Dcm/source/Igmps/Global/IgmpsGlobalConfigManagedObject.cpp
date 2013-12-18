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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#include "Igmps/Global/IgmpsGlobalConfigManagedObject.h"
#include "Igmps/Global/IgmpsGlobalObjectManager.h"
#include "vcs.h"

namespace DcmNs
{

    IgmpsGlobalConfigManagedObject::IgmpsGlobalConfigManagedObject (IgmpsGlobalObjectManager *pIgmpsGlobalObjectManager)
        : PrismElement  (pIgmpsGlobalObjectManager),
        PrismPersistableObject (IgmpsGlobalConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pIgmpsGlobalObjectManager),
        DcmManagedObject (pIgmpsGlobalObjectManager),
        m_enable    (false), m_restrictUnknownMcast (false)
    {
    }

    IgmpsGlobalConfigManagedObject::IgmpsGlobalConfigManagedObject (IgmpsGlobalObjectManager *pIgmpsGlobalObjectManager,const bool &enable,
                                                                    const bool &restrictUnknownMcast)
        : PrismElement  (pIgmpsGlobalObjectManager),
        PrismPersistableObject (IgmpsGlobalConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pIgmpsGlobalObjectManager),
        DcmManagedObject (pIgmpsGlobalObjectManager),
        m_enable    (enable), m_restrictUnknownMcast (restrictUnknownMcast)
    {
    }

    IgmpsGlobalConfigManagedObject::~IgmpsGlobalConfigManagedObject ()
    {
    }

    string  IgmpsGlobalConfigManagedObject::getClassName()
    {
        return ("IgmpsGlobalConfigManagedObject");
    }

    void  IgmpsGlobalConfigManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_enable,"enable", igmp_snooping_enable));
        addPersistableAttribute (new AttributeBool(&m_restrictUnknownMcast,"restrictunknownmcast", igmp_snooping_restrict_unknown_mcast));
    }

    void  IgmpsGlobalConfigManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_enable,"enable", igmp_snooping_enable));
        addPersistableAttributeForCreate (new AttributeBool(&m_restrictUnknownMcast,"restrictunknownmcast", igmp_snooping_restrict_unknown_mcast));
    }

    void  IgmpsGlobalConfigManagedObject::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  IgmpsGlobalConfigManagedObject::getEnable() const
    {
        return (m_enable);
    }

    void  IgmpsGlobalConfigManagedObject::setRestrictUnknownMcast(const bool &restrictUnknownMcast)
    {
        m_restrictUnknownMcast  =  restrictUnknownMcast;
    }

    bool  IgmpsGlobalConfigManagedObject::getRestrictUnknownMcast() const
    {
        return (m_restrictUnknownMcast);
    }
}
