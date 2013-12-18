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
 *   Author : vsatyana                                                     *
 **************************************************************************/

#include "Sflow/Global/SflowGlobalConfigManagedObject.h"
#include "Sflow/Global/SflowGlobalObjectManager.h"
#include "Sflow/Global/SflowGlobalTypes.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "vcs.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    SflowGlobalConfigManagedObject::SflowGlobalConfigManagedObject (SflowGlobalObjectManager *pSflowGlobalObjectManager)
        : PrismElement  (pSflowGlobalObjectManager),
        PrismPersistableObject (SflowGlobalConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSflowGlobalObjectManager),
        DcmManagedObject (pSflowGlobalObjectManager),
		m_enable			(SFLOW_DEFAULT_ENABLE),
		m_pollingInterval	(SFLOW_DEFAULT_POLLING_INTERVAL),
		m_sampleRate		(SFLOW_DEFAULT_SAMPLE_RATE)
    {

    }

    SflowGlobalConfigManagedObject::SflowGlobalConfigManagedObject (SflowGlobalObjectManager *pSflowGlobalObjectManager,const bool &enable,const UI32 &pollingInterval,const UI32 &sampleRate,const vector<WaveManagedObjectPointer<SflowCollectorManagedObject> > &collectorList)
        : PrismElement  (pSflowGlobalObjectManager),
        PrismPersistableObject (SflowGlobalConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSflowGlobalObjectManager),
        DcmManagedObject (pSflowGlobalObjectManager),
        m_enable    (enable),
        m_pollingInterval    (pollingInterval),
        m_sampleRate    (sampleRate),
        m_collectorList    (collectorList)
    {
    }

    SflowGlobalConfigManagedObject::~SflowGlobalConfigManagedObject ()
    {
    }

    string  SflowGlobalConfigManagedObject::getClassName()
    {
        return ("SflowGlobalConfigManagedObject");
    }

    void  SflowGlobalConfigManagedObject::setupAttributesForPersistence()
    {
        IpV4Address defaultIp("");
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_enable,"enable", sflow_enable));
        addPersistableAttribute (new AttributeUI32(&m_pollingInterval,  "pollingInterval", sflow_polling_interval));
        addPersistableAttribute (new AttributeUI32(&m_sampleRate,  "sampleRate", sflow_sample_rate));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<SflowCollectorManagedObject>(&m_collectorList,"collectorList", getClassName (), getObjectId (), SflowCollectorManagedObject::getClassName ()));
    }

    void  SflowGlobalConfigManagedObject::setupAttributesForCreate()
    {
        IpV4Address defaultIp("");
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_enable,"enable", sflow_enable));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_pollingInterval, "pollingInterval", sflow_polling_interval));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_sampleRate, "sampleRate", sflow_sample_rate));

        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<SflowCollectorManagedObject>(&m_collectorList,"collectorList", getClassName (), getObjectId (), SflowCollectorManagedObject::getClassName ()));
    }

    void  SflowGlobalConfigManagedObject::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  SflowGlobalConfigManagedObject::getEnable() const
    {
        return (m_enable);
    }

    void  SflowGlobalConfigManagedObject::setPollingInterval(const UI32 &pollingInterval)
    {
        m_pollingInterval  =  pollingInterval;
    }

    UI32  SflowGlobalConfigManagedObject::getPollingInterval() const
    {
        return (m_pollingInterval);
    }

    void  SflowGlobalConfigManagedObject::setSampleRate(const UI32 &sampleRate)
    {
        m_sampleRate  =  sampleRate;
    }

    UI32  SflowGlobalConfigManagedObject::getSampleRate() const
    {
        return (m_sampleRate);
    }

    void  SflowGlobalConfigManagedObject::setCollectorList(const vector<WaveManagedObjectPointer<SflowCollectorManagedObject> > &collectorList)
    {
    	m_collectorList  =  collectorList;
    }

    vector<WaveManagedObjectPointer<SflowCollectorManagedObject> >  SflowGlobalConfigManagedObject::getCollectorList() const
    {
        return (m_collectorList);
    }

	void SflowGlobalConfigManagedObject::addSflowCollectorManagedObject(SflowCollectorManagedObject *pSflowCollectorMO)
	{
		WaveManagedObjectPointer<SflowCollectorManagedObject> sflowCollector(pSflowCollectorMO);
		m_collectorList.push_back(sflowCollector);
	}

	void SflowGlobalConfigManagedObject::deleteSflowCollectorManagedObject(
			SflowCollectorManagedObject *pSflowCollectorMO) {
		NSM_PLUG_DBG_S("Entering....");
		for (UI32 i = 0; i < m_collectorList.size(); i++) {
			if ((m_collectorList[i])->getCollectorIP().toString()
					== pSflowCollectorMO->getCollectorIP().toString()
					&& (m_collectorList[i])->getCollectorPort()
					== pSflowCollectorMO->getCollectorPort()) {
				NSM_PLUG_DBG_S(
						string("Deleting the sflow collector ") +pSflowCollectorMO->getCollectorIP().toString()+":"+pSflowCollectorMO->getCollectorPort());
				m_collectorList.erase(m_collectorList.begin() + i);
			}
		}
		NSM_PLUG_DBG_S("Exiting....");
	}

}

