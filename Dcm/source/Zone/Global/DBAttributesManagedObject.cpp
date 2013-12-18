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
 *   Author : dchung                                                     *
 **************************************************************************/

#include "Zone/Global/DBAttributesManagedObject.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/ZoneTypes.h"

#include "vcs.h"
/*#include "brocade-zone.h"*/

namespace DcmNs
{

    DBAttributesManagedObject::DBAttributesManagedObject (ZoneObjectManager *pZoneObjectManager)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (DBAttributesManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager),
		m_effectiveCfgName    (""),
        m_effectiveCfgAction    (brocade_zone_cfg_save),	// default to 'cfg-save'
        m_dbTransToken    (0),
        m_dbTransFlag    (0),
        m_dbMaxSize    (0),
        m_dbAvailableSize    (0),
        m_dbCommittedSize    (0),
        m_dbTransactionSize    (0),
        m_defaultZoneAccess    (brocade_zone_allaccess),		// init default to 'allaccess'
	    m_effectiveCfgNameCurrent    (""),
	    m_bootUpType    (0),
		m_lastZoneChangedTimestamp   (""),
		m_lastZoneCommittedTimestamp ("")
    {
    }

    DBAttributesManagedObject::DBAttributesManagedObject (ZoneObjectManager *pZoneObjectManager,const string &effectiveCfgName,const UI32 &effectiveCfgAction,const UI32 &dbTransToken,const UI32 &dbTransFlag,const UI32 &dbMaxSize,const UI32 &dbAvailableSize,const UI32 &dbCommittedSize,const UI32 &dbTransactionSize,const UI32 &defaultZoneAccess,const string &effectiveCfgNameCurrent,const UI32 &bootUpType,const string &lastZoneChangedTimestamp,const string &lastZoneCommittedTimestamp)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (DBAttributesManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager),
        m_effectiveCfgName    (effectiveCfgName),
        m_effectiveCfgAction    (effectiveCfgAction),
        m_dbTransToken    (dbTransToken),
        m_dbTransFlag    (dbTransFlag),
        m_dbMaxSize    (dbMaxSize),
        m_dbAvailableSize    (dbAvailableSize),
        m_dbCommittedSize    (dbCommittedSize),
        m_dbTransactionSize    (dbTransactionSize),
        m_defaultZoneAccess    (defaultZoneAccess),
	    m_effectiveCfgNameCurrent    (effectiveCfgNameCurrent),
	    m_bootUpType    (bootUpType),
		m_lastZoneChangedTimestamp    (lastZoneChangedTimestamp),
		m_lastZoneCommittedTimestamp    (lastZoneCommittedTimestamp)
    {
    }

    DBAttributesManagedObject::~DBAttributesManagedObject ()
    {
    }

    string  DBAttributesManagedObject::getClassName()
    {
        return ("DBAttributesManagedObject");
    }

    void  DBAttributesManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_effectiveCfgName,"effectiveCfgName"));
        addPersistableAttribute (new AttributeEnum((UI32*)(&m_effectiveCfgAction),"effectiveCfgAction"));
        addPersistableAttribute (new AttributeEnum((UI32*)(&m_defaultZoneAccess),"defaultZoneAccess"));
	    addPersistableAttribute (new AttributeEnum((UI32*)(&m_bootUpType),"bootUpType"));
        addPersistableAttribute (new AttributeString(&m_lastZoneChangedTimestamp,"lastZoneChangedTimestamp"));
        addPersistableAttribute (new AttributeString(&m_lastZoneCommittedTimestamp,"lastZoneCommittedTimestamp"));

	    setUserTagForAttribute ("effectiveCfgName", brocade_zone_cfg_name);
	    setUserTagForAttribute ("effectiveCfgAction", brocade_zone_cfg_action);
	    setUserTagForAttribute ("defaultZoneAccess", brocade_zone_default_zone_access);
    }

    void  DBAttributesManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_effectiveCfgName,"effectiveCfgName"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)(&m_effectiveCfgAction),"effectiveCfgAction"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)(&m_defaultZoneAccess),"defaultZoneAccess"));
	    addPersistableAttributeForCreate  (new AttributeEnum((UI32*)(&m_bootUpType),"bootUpType"));
        addPersistableAttributeForCreate  (new AttributeString(&m_lastZoneChangedTimestamp,"lastZoneChangedTimestamp"));
        addPersistableAttributeForCreate  (new AttributeString(&m_lastZoneCommittedTimestamp,"lastZoneCommittedTimestamp"));
    }

    void  DBAttributesManagedObject::setEffectiveCfgName(const string &effectiveCfgName)
    {
        m_effectiveCfgName  =  effectiveCfgName;
    }

    string  DBAttributesManagedObject::getEffectiveCfgName() const
    {
        return (m_effectiveCfgName);
    }

    void  DBAttributesManagedObject::setEffectiveCfgAction(const UI32 &effectiveCfgAction)
    {
        m_effectiveCfgAction  =  effectiveCfgAction;
    }

    UI32  DBAttributesManagedObject::getEffectiveCfgAction() const
    {
        return (m_effectiveCfgAction);
    }

    void  DBAttributesManagedObject::setDbTransToken(const UI32 &dbTransToken)
    {
        m_dbTransToken  =  dbTransToken;
    }

    UI32  DBAttributesManagedObject::getDbTransToken() const
    {
        return (m_dbTransToken);
    }

    void  DBAttributesManagedObject::setDbTransFlag(const UI32 &dbTransFlag)
    {
        m_dbTransFlag  =  dbTransFlag;
    }

    UI32  DBAttributesManagedObject::getDbTransFlag() const
    {
        return (m_dbTransFlag);
    }

    void  DBAttributesManagedObject::setDbMaxSize(const UI32 &dbMaxSize)
    {
        m_dbMaxSize  =  dbMaxSize;
    }

    UI32  DBAttributesManagedObject::getDbMaxSize() const
    {
        return (m_dbMaxSize);
    }

    void  DBAttributesManagedObject::setDbAvailableSize(const UI32 &dbAvailableSize)
    {
        m_dbAvailableSize  =  dbAvailableSize;
    }

    UI32  DBAttributesManagedObject::getDbAvailableSize() const
    {
        return (m_dbAvailableSize);
    }

    void  DBAttributesManagedObject::setDbCommittedSize(const UI32 &dbCommittedSize)
    {
        m_dbCommittedSize  =  dbCommittedSize;
    }

    UI32  DBAttributesManagedObject::getDbCommittedSize() const
    {
        return (m_dbCommittedSize);
    }

    void  DBAttributesManagedObject::setDbTransactionSize(const UI32 &dbTransactionSize)
    {
        m_dbTransactionSize  =  dbTransactionSize;
    }

    UI32  DBAttributesManagedObject::getDbTransactionSize() const
    {
        return (m_dbTransactionSize);
    }

    void  DBAttributesManagedObject::setDefaultZoneAccess(const UI32 &defaultZoneAccess)
    {
        m_defaultZoneAccess  =  defaultZoneAccess;
    }

    UI32  DBAttributesManagedObject::getDefaultZoneAccess() const
    {
        return (m_defaultZoneAccess);
    }

    void  DBAttributesManagedObject::setEffectiveCfgNameCurrent(const string &effectiveCfgNameCurrent)
    {
        m_effectiveCfgNameCurrent  =  effectiveCfgNameCurrent;
    }

    string  DBAttributesManagedObject::getEffectiveCfgNameCurrent() const
    {
        return (m_effectiveCfgNameCurrent);
    }

	void  DBAttributesManagedObject::setBootUpType(const UI32 &bootUpType)
	{
	    m_bootUpType  =  bootUpType;
	}

	UI32  DBAttributesManagedObject::getBootUpType() const
	{
	    return (m_bootUpType);
	}

	void  DBAttributesManagedObject::setLastZoneChangedTimestamp(const string &lastZoneChangedTimestamp)
    {
        m_lastZoneChangedTimestamp  =  lastZoneChangedTimestamp;
    }

    string  DBAttributesManagedObject::getLastZoneChangedTimestamp() const
    {
        return (m_lastZoneChangedTimestamp);
    }

	void  DBAttributesManagedObject::setLastZoneCommittedTimestamp(const string &lastZoneCommittedTimestamp)
    {
        m_lastZoneCommittedTimestamp  =  lastZoneCommittedTimestamp;
    }

    string  DBAttributesManagedObject::getLastZoneCommittedTimestamp() const
    {
        return (m_lastZoneCommittedTimestamp);
    }
}
