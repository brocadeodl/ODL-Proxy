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
 *   Author : Arun kumar viswanathan                                       *
 **************************************************************************/

#include "Ipadm/Local/IpadmLocalObjectManager.h"
#include "Ipadm/Local/IpadmLocalDnsCfgMessage.h"
#include "Ipadm/Local/IpadmTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Types/IpVxAddress.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

//#include "ipadm.h"
#include "vcs.h"
namespace DcmNs
{

IpadmLocalDnsCfgMessage::IpadmLocalDnsCfgMessage ()
            : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMNSCFG)
{       
}       
    
IpadmLocalDnsCfgMessage::~IpadmLocalDnsCfgMessage ()
{   
}
//For name server
IpadmLocalDnsCfgMessage::IpadmLocalDnsCfgMessage (const ObjectId &LoggingOID, UI32 &userWyserTag)
            : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMNSCFG),
        m_ObjectId(LoggingOID), m_userWyserTag(userWyserTag)
{
}
//For domain name
IpadmLocalDnsCfgMessage::IpadmLocalDnsCfgMessage (const ObjectId &LoggingOID, string &domain_name, UI32 &userWyserTag)
            : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMDNCFG),
        m_ObjectId(LoggingOID), m_domain_name(domain_name)
{
}

void  IpadmLocalDnsCfgMessage::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute    (new AttributeString        (&m_data,           "data"));
     addSerializableAttribute   (new AttributeUI32          (&m_userWyserTag,    "userWyserTag"));
    addSerializableAttribute    (new AttributeString        (&m_domain_name,    "domain_name"));
    addSerializableAttribute    (new AttributeIpVxAddress   (&m_nameSrvIp,      "ipaddress"));
     addSerializableAttribute   (new AttributeUI32          (&m_mappedId,       "mappedId"));
     addSerializableAttribute   (new AttributeString        (&m_opr,            "opr"));
}

string  IpadmLocalDnsCfgMessage::getData() const
{
    return (m_data);
}
void  IpadmLocalDnsCfgMessage::setData(const string &Data)
{
    m_data = Data;
}

UI32 IpadmLocalDnsCfgMessage::getWyserUserTag() const
{
    return (m_userWyserTag);
}
void  IpadmLocalDnsCfgMessage::setWyserUserTag(const  UI32 &userWyserTag)
{
    m_userWyserTag = userWyserTag;
}
IpVxAddress  IpadmLocalDnsCfgMessage::getNameSrvIp () const
{
    return (m_nameSrvIp);
}

void  IpadmLocalDnsCfgMessage::setNameSrvIp (const IpVxAddress &nameSrvIp)
{
    m_nameSrvIp = nameSrvIp;
}

string  IpadmLocalDnsCfgMessage::getDomainName () const
{
    return (m_domain_name);
}

void  IpadmLocalDnsCfgMessage::setDomainName (const string &domain_name)
{
    m_domain_name = domain_name;
}

UI32 IpadmLocalDnsCfgMessage::getMappedId() const
{   
    return (m_mappedId);
} 
void  IpadmLocalDnsCfgMessage::setMappedId(const  UI32 &mappedId)
{   
    m_mappedId = mappedId;
}

string  IpadmLocalDnsCfgMessage::getOpr () const
{
    return (m_opr);
}

void  IpadmLocalDnsCfgMessage::setOpr (const string &opr)
{
    m_opr = opr;
}

}
