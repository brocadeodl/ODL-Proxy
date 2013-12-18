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
 *   Author : lhu                                                     *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"
#include "Terminal/Global/TerminalTypes.h"
#include "Terminal/Global/TerminalManagedObject.h"
#include "Terminal/Global/TerminalObjectManager.h"
#include "vcs.h"
//#include "brocade-terminal.h"

namespace DcmNs
{

    TerminalManagedObject::TerminalManagedObject (TerminalObjectManager *pTerminalObjectManager)
        : PrismElement  (pTerminalObjectManager),
        PrismPersistableObject (TerminalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pTerminalObjectManager),
        DcmManagedObject (pTerminalObjectManager)
    {
    }

    TerminalManagedObject::TerminalManagedObject (TerminalObjectManager *pTerminalObjectManager,const Session_Id &sessionid,const UI32&exectimeout)
        : PrismElement  (pTerminalObjectManager),
        PrismPersistableObject (TerminalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pTerminalObjectManager),
        DcmManagedObject (pTerminalObjectManager),
        m_sessionid    (sessionid),
        m_exectimeout    (exectimeout)
    {
    }

    TerminalManagedObject::~TerminalManagedObject ()
    {
    }

    string  TerminalManagedObject::getClassName()
    {
        return ("TerminalManagedObject");
    }

	// attribute name m_sessionid must match "sessionid" and match setup key "sessionid".
    void  TerminalManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeEnum((UI32*)(&m_sessionid),"sessionid", terminal_sessionid));
        addPersistableAttribute (new AttributeUI32(&m_exectimeout,"exectimeout", terminal_exec_timeout));
    }

    void  TerminalManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate  (new AttributeEnum((UI32*)(&m_sessionid),"sessionid", terminal_sessionid));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_exectimeout,"exectimeout", terminal_exec_timeout));
    }

	void TerminalManagedObject::setupKeys()
	{
    	vector<string> key;
    	key.push_back ("sessionid");
    	setUserDefinedKeyCombination (key);
	}

    void  TerminalManagedObject::setSessionid(const Session_Id &sessionid)
    {
        m_sessionid  =  sessionid;
    }

    Session_Id  TerminalManagedObject::getSessionid() const
    {
        return (m_sessionid);
    }

    void  TerminalManagedObject::setExectimeout(const UI32 &exectimeout)
    {
        m_exectimeout  =  exectimeout;
    }

    UI32  TerminalManagedObject::getExectimeout() const
    {
        return (m_exectimeout);
    }

}
