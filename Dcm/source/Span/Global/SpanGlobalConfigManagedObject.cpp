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
 *   Author : Ojasvita                                                     *
 **************************************************************************/


#include "Span/Global/SpanGlobalConfigManagedObject.h"
#include "Span/Global/SpanGlobalObjectManager.h"
#include "Span/Global/SpanGlobalTypes.h"
#include "vcs.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

	UI32 span=0;
    SpanGlobalConfigManagedObject::SpanGlobalConfigManagedObject (SpanGlobalObjectManager *pSpanGlobalObjectManager)
       : PrismElement  (pSpanGlobalObjectManager),
		PrismPersistableObject (SpanGlobalConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	    WaveManagedObject (pSpanGlobalObjectManager),
	    DcmManagedObject (pSpanGlobalObjectManager)
	{
	}

	SpanGlobalConfigManagedObject::SpanGlobalConfigManagedObject (SpanGlobalObjectManager *pSpanGlobalObjectManager,const UI32 &session_id,const string &source_ifName,const string &dest_ifName,const UI32 &direction,const string &description, const UI32 &default_source_enum, const UI32 &default_destination_enum, const UI32 &source_tengiga_enum, const UI32 &dest_tengiga_enum,const UI32 &dstVlanId)
	    : PrismElement  (pSpanGlobalObjectManager),
	    PrismPersistableObject (SpanGlobalConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	    WaveManagedObject (pSpanGlobalObjectManager),
	    DcmManagedObject (pSpanGlobalObjectManager),
	    m_session_id    (session_id),
	    m_source_ifName    (source_ifName),
	    m_dest_ifName    (dest_ifName),
	    m_direction    (direction),
	    m_description    (description),
	    m_source_enum    (default_source_enum),
	    m_destination_enum (default_destination_enum),
	    m_source_tengiga_enum (source_tengiga_enum),
	    m_dest_tengiga_enum (dest_tengiga_enum),
	    m_destVlanId    (dstVlanId)
	{
	}
	
	SpanGlobalConfigManagedObject::~SpanGlobalConfigManagedObject ()
	{
	}

	string  SpanGlobalConfigManagedObject::getClassName()
	{
	    return ("SpanGlobalConfigManagedObject");
	}

	void  SpanGlobalConfigManagedObject::setupAttributesForPersistence()
	{
	    UI32 enum_default = ENUM_DEFAULT_VALUE;
	    DcmManagedObject::setupAttributesForPersistence ();
	    addPersistableAttribute (new AttributeUI32(&m_session_id,"session_id",span_session_number));
	    addPersistableAttribute (new AttributeString(&m_source_ifName,true,"None","source_ifName",span_src_tengigabitethernet_val));
	    addPersistableAttribute (new AttributeString(&m_dest_ifName,true,"None","dest_ifName",span_dest_tengigabitethernet_val));
	    addPersistableAttribute (new AttributeEnum((UI32 *)&m_direction,enum_default,"direction",span_direction));
	    addPersistableAttribute (new AttributeString(&m_description,true,"","description",span_description));
	    addPersistableAttribute (new AttributeEnum((UI32 *)&m_source_enum,enum_default,"source",span_source));
	    addPersistableAttribute (new AttributeEnum((UI32 *)&m_destination_enum,enum_default,"destination",span_destination));
	    addPersistableAttribute (new AttributeEnum((UI32 *)&m_source_tengiga_enum,enum_default,"source_tengigabitethernet",span_src_tengigabitethernet));
	    addPersistableAttribute (new AttributeEnum((UI32 *)&m_dest_tengiga_enum,enum_default,"destination_tengigabitethernet",span_dest_tengigabitethernet));

	    addPersistableAttribute (new AttributeUI32(&m_destVlanId,RSPAN_VLAN_DEFAULT_VALUE,"destVlanId",span_dest_vlan_val));
	}

	void  SpanGlobalConfigManagedObject::setupAttributesForCreate()
	{
	     UI32 enum_default = ENUM_DEFAULT_VALUE;
		 DcmManagedObject::setupAttributesForCreate ();
		 addPersistableAttributeForCreate  (new AttributeUI32(&m_session_id,"session_id",span_session_number));
		 addPersistableAttributeForCreate  (new AttributeString(&m_source_ifName,"source_ifName",span_src_tengigabitethernet_val));
		 addPersistableAttributeForCreate  (new AttributeString(&m_dest_ifName,"dest_ifName",span_dest_tengigabitethernet_val));
		 addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_direction,enum_default,"direction",span_direction));
		 addPersistableAttributeForCreate  (new AttributeString(&m_description,"description",span_description));
		 addPersistableAttributeForCreate (new AttributeEnum((UI32 *)&m_source_enum,enum_default,"source",span_source));
		 addPersistableAttributeForCreate (new AttributeEnum((UI32 *)&m_destination_enum,enum_default,"destination",span_destination));
		 addPersistableAttributeForCreate (new AttributeEnum((UI32 *)&m_source_tengiga_enum,"source_tengigabitethernet",span_src_tengigabitethernet));
		 addPersistableAttributeForCreate (new AttributeEnum((UI32 *)&m_dest_tengiga_enum,"destination_tengigabitethernet",span_dest_tengigabitethernet));

		 addPersistableAttributeForCreate  (new AttributeUI32(&m_destVlanId,RSPAN_VLAN_DEFAULT_VALUE,"destVlanId",span_dest_vlan_val));

         vector<string > keyName;
		 keyName.push_back ("session_id");
		 setUserDefinedKeyCombination (keyName);
    }

	void  SpanGlobalConfigManagedObject::setSession_id(const UI32 &session_id)
	{
	     m_session_id  =  session_id;
	}

	UI32  SpanGlobalConfigManagedObject::getSession_id() const
	{
	     return (m_session_id);
	}
	
	void  SpanGlobalConfigManagedObject::setSource_ifName(const string &source_ifName)
	{
	     m_source_ifName  =  source_ifName;
	}

	string  SpanGlobalConfigManagedObject::getSource_ifName() const
	{
	      return (m_source_ifName);
	}

	void  SpanGlobalConfigManagedObject::setDest_ifName(const string &dest_ifName)
	{
	      m_dest_ifName  =  dest_ifName;
	}

	string  SpanGlobalConfigManagedObject::getDest_ifName() const
	{
	      return (m_dest_ifName);
	}

	void  SpanGlobalConfigManagedObject::setDirection(const UI32 &direction)
	{
	      m_direction  =  direction;
	}

	UI32  SpanGlobalConfigManagedObject::getDirection() const
	{
	      return (m_direction);
	}

	void  SpanGlobalConfigManagedObject::setSource(const UI32 &source)
	{
	      m_source_enum  =  source;
	}

	UI32  SpanGlobalConfigManagedObject::getSource() const
	{
	      return (m_source_enum);
	}

	void  SpanGlobalConfigManagedObject::setDestination(const UI32 &destination)
	{
	      m_destination_enum  =  destination;
	}

	UI32  SpanGlobalConfigManagedObject::getDestination() const
	{
	      return (m_destination_enum);
	}

	void  SpanGlobalConfigManagedObject::setSource_tengiga(const UI32 &source_tengiga)
	{
	      m_source_tengiga_enum  =  source_tengiga;
	}

	UI32  SpanGlobalConfigManagedObject::getSource_tengiga() const
	{
	      return (m_source_tengiga_enum);
	}

	void  SpanGlobalConfigManagedObject::setDest_tengiga(const UI32 &dest_tengiga)
	{
	      m_dest_tengiga_enum  =  dest_tengiga;
	}

	UI32  SpanGlobalConfigManagedObject::getDest_tengiga() const
	{
	      return (m_dest_tengiga_enum);
	}

	void  SpanGlobalConfigManagedObject::setDescription(const string &description)
	{
	      m_description  =  description;
	}

	string  SpanGlobalConfigManagedObject::getDescription() const
	{
	      return (m_description);
	}

	void SpanGlobalConfigManagedObject::setEnumleaves_default()
	{
	      UI32 enum_default = ENUM_DEFAULT_VALUE;
	      m_source_enum = (const UI32 &)enum_default;
	      m_destination_enum = (const UI32 &)enum_default;
	}

	void SpanGlobalConfigManagedObject::unsetEnumleaves_default()
	{
	      UI32 enum_set_value = 0;
	      m_source_enum = (const UI32 &)enum_set_value;
	      m_destination_enum = (const UI32 &)enum_set_value;
	}

	void  SpanGlobalConfigManagedObject::setDestVlanId(const UI32 &vlanId)
	{
		m_destVlanId  =  vlanId;
	}

	UI32  SpanGlobalConfigManagedObject::getDestVlanId() const
	{
	     return (m_destVlanId);
	}

}


