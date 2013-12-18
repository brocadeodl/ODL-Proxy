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
 *   Author : sharis                                                       *
 **************************************************************************/

#include "discover/Framework/discoverLocalObjectManager.h"
#include "discover/Framework/discoverLocalMessages.h"
#include "discover/Framework/discoverLocalTypes.h"

namespace DcmNs
{
	// ************* DiscoverLocalDiscoverMessage ************

    DiscoverLocalDiscoverMessage::DiscoverLocalDiscoverMessage ()
        : PrismMessage (DiscoverLocalObjectManager::getPrismServiceId (), DISCOVERLOCALDISCOVER)
    {
        m_cmdcode = 0;
        m_msgType = 0;
    }

    DiscoverLocalDiscoverMessage::DiscoverLocalDiscoverMessage (const UI32 &cmdcode, 
                        const UI32 &msgType, const string &vcenter)
        : PrismMessage (DiscoverLocalObjectManager::getPrismServiceId (), DISCOVERLOCALDISCOVER),
        m_cmdcode    (cmdcode),
        m_msgType    (msgType),
        m_vcenter    (vcenter)
    {
    }

    DiscoverLocalDiscoverMessage::~DiscoverLocalDiscoverMessage ()
    {
    }

    void  DiscoverLocalDiscoverMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeString(&m_vcenter,"vcenter"));
        addSerializableAttribute (new AttributeString(&m_scriptNames,"scriptNames"));
        addSerializableAttribute (new AttributeString(&m_arg1,"arg1"));
        addSerializableAttribute (new AttributeString(&m_arg2,"arg2"));
        addSerializableAttribute (new AttributeString(&m_arg3,"arg3"));
        addSerializableAttribute (new AttributeString(&m_arg4,"arg4"));
        addSerializableAttribute(
                              new AttributeStringVector(&m_datalist, "dataList"));
    }
    
    void  DiscoverLocalDiscoverMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  DiscoverLocalDiscoverMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  DiscoverLocalDiscoverMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  DiscoverLocalDiscoverMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  DiscoverLocalDiscoverMessage::setVcenter(const string &vcenter)
    {
        m_vcenter = vcenter;
    }

    string  DiscoverLocalDiscoverMessage::getVcenter() const
    {
        return (m_vcenter);
    }
    
	void  DiscoverLocalDiscoverMessage::setScriptNames(const string &scriptNames)
    {
        m_scriptNames = scriptNames;
    }

    string  DiscoverLocalDiscoverMessage::getScriptNames() const
    {
        return (m_scriptNames);
    }
	
	void  DiscoverLocalDiscoverMessage::setArg1(const string &arg1)
    {
        m_arg1 = arg1;
    }

    string  DiscoverLocalDiscoverMessage::getArg1() const
    {
        return (m_arg1);
    }
	
	void  DiscoverLocalDiscoverMessage::setArg2(const string &arg2)
    {
        m_arg2 = arg2;
    }

    string  DiscoverLocalDiscoverMessage::getArg2() const
    {
        return (m_arg2);
    }
	
	void  DiscoverLocalDiscoverMessage::setArg3(const string &arg3)
    {
        m_arg3 = arg3;
    }

    string  DiscoverLocalDiscoverMessage::getArg3() const
    {
        return (m_arg3);
    }
	
	void  DiscoverLocalDiscoverMessage::setArg4(const string &arg4)
    {
        m_arg4 = arg4;
    }

    string  DiscoverLocalDiscoverMessage::getArg4() const
    {
        return (m_arg4);
    }
    void DiscoverLocalDiscoverMessage::setDataList(const vector<string> &dataList) {
        m_datalist = dataList;
    }

    vector<string> DiscoverLocalDiscoverMessage::getDataList() const {
        return (m_datalist);
    }
}
