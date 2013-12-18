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
 *   Copyright (C) 2011-2015 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Sudha Balodia                                                     *
 **************************************************************************/

#include "Nsm/Local/QosCosToTailDropManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs
{

    QosCosToTailDropManagedObject::QosCosToTailDropManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (QosCosToTailDropManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		m_cos0_threshold = 12;
		m_cos1_threshold = 13;
		m_cos2_threshold = 12;
		m_cos3_threshold = 13;
		m_cos4_threshold = 12;
		m_cos5_threshold = 13;
		m_cos6_threshold = 12;
		m_cos7_threshold = 13;
    }

	QosCosToTailDropManagedObject::QosCosToTailDropManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager, const UI32 &cos0_threshold, const UI32 &cos1_threshold, const UI32 &cos2_threshold, const UI32 &cos3_threshold, const UI32 &cos4_threshold, const UI32 &cos5_threshold, const UI32 &cos6_threshold, const UI32 &cos7_threshold)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (QosCosToTailDropManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
		m_cos0_threshold(cos0_threshold),
		m_cos1_threshold(cos1_threshold),
		m_cos2_threshold(cos2_threshold),
		m_cos3_threshold(cos3_threshold),
		m_cos4_threshold(cos4_threshold),
		m_cos5_threshold(cos5_threshold),
		m_cos6_threshold(cos6_threshold),
		m_cos7_threshold(cos7_threshold)
    {
    }

    QosCosToTailDropManagedObject::~QosCosToTailDropManagedObject ()
    {
    }

    string  QosCosToTailDropManagedObject::getClassName()
    {
        return ("QosCosToTailDropManagedObject");
    }

    void  QosCosToTailDropManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_cos0_threshold,"cos0threshold", qos_cos0_threshold));
        addPersistableAttribute (new AttributeUI32(&m_cos1_threshold,"cos1threshold", qos_cos1_threshold));
        addPersistableAttribute (new AttributeUI32(&m_cos2_threshold,"cos2threshold", qos_cos2_threshold));
        addPersistableAttribute (new AttributeUI32(&m_cos3_threshold,"cos3threshold", qos_cos3_threshold));
        addPersistableAttribute (new AttributeUI32(&m_cos4_threshold,"cos4threshold", qos_cos4_threshold));
        addPersistableAttribute (new AttributeUI32(&m_cos5_threshold,"cos5threshold", qos_cos5_threshold));
        addPersistableAttribute (new AttributeUI32(&m_cos6_threshold,"cos6threshold", qos_cos6_threshold));
        addPersistableAttribute (new AttributeUI32(&m_cos7_threshold,"cos7threshold", qos_cos7_threshold));
    }

    void  QosCosToTailDropManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeUI32(&m_cos0_threshold,"cos0threshold", qos_cos0_threshold));
        addPersistableAttributeForCreate (new AttributeUI32(&m_cos1_threshold,"cos1threshold", qos_cos1_threshold));
        addPersistableAttributeForCreate (new AttributeUI32(&m_cos2_threshold,"cos2threshold", qos_cos2_threshold));
        addPersistableAttributeForCreate (new AttributeUI32(&m_cos3_threshold,"cos3threshold", qos_cos3_threshold));
        addPersistableAttributeForCreate (new AttributeUI32(&m_cos4_threshold,"cos4threshold", qos_cos4_threshold));
        addPersistableAttributeForCreate (new AttributeUI32(&m_cos5_threshold,"cos5threshold", qos_cos5_threshold));
        addPersistableAttributeForCreate (new AttributeUI32(&m_cos6_threshold,"cos6threshold", qos_cos6_threshold));
        addPersistableAttributeForCreate (new AttributeUI32(&m_cos7_threshold,"cos7threshold", qos_cos7_threshold));
    }


	UI32 QosCosToTailDropManagedObject::getCos0Threshold() const
	{
		return (m_cos0_threshold);
	}
	void QosCosToTailDropManagedObject::setCos0Threshold(const UI32 &threshold)
	{
		m_cos0_threshold = threshold;
	}

	UI32 QosCosToTailDropManagedObject::getCos1Threshold() const
	{
		return (m_cos1_threshold);
	}
	void QosCosToTailDropManagedObject::setCos1Threshold(const UI32 &threshold)
	{
		m_cos1_threshold = threshold;
	}

	UI32 QosCosToTailDropManagedObject::getCos2Threshold() const
	{
		return (m_cos2_threshold);
	}
	void QosCosToTailDropManagedObject::setCos2Threshold(const UI32 &threshold)
	{
		m_cos2_threshold = threshold;
	}

	UI32 QosCosToTailDropManagedObject::getCos3Threshold() const
	{
		return (m_cos3_threshold);
	}
	void QosCosToTailDropManagedObject::setCos3Threshold(const UI32 &threshold)
	{
		m_cos3_threshold = threshold;
	}

	UI32 QosCosToTailDropManagedObject::getCos4Threshold() const
	{
		return (m_cos4_threshold);
	}
	void QosCosToTailDropManagedObject::setCos4Threshold(const UI32 &threshold)
	{
		m_cos4_threshold = threshold;
	}

	UI32 QosCosToTailDropManagedObject::getCos5Threshold() const
	{
		return (m_cos5_threshold);
	}
	void QosCosToTailDropManagedObject::setCos5Threshold(const UI32 &threshold)
	{
		m_cos5_threshold = threshold;
	}

	UI32 QosCosToTailDropManagedObject::getCos6Threshold() const
	{
		return (m_cos6_threshold);
	}
	void QosCosToTailDropManagedObject::setCos6Threshold(const UI32 &threshold)
	{
		m_cos6_threshold = threshold;
	}

	UI32 QosCosToTailDropManagedObject::getCos7Threshold() const
	{
		return (m_cos7_threshold);
	}
	void QosCosToTailDropManagedObject::setCos7Threshold(const UI32 &threshold)
	{
		m_cos7_threshold = threshold;
	}
}
