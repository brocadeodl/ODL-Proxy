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

#include "Nsm/Global/Igmps/IgmpsVlanIntfConfManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "vcs.h"
#include "Modeling/XPath/WaveXPathExpressionElement.h"
#include "Modeling/YANG/ObjectModel/YangDisplayConfigurationContext.h"
#include "Modeling/YANG/ObjectModel/YangElement.h"

namespace DcmNs
{

    IgmpsVlanIntfConfManagedObject::IgmpsVlanIntfConfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (IgmpsVlanIntfConfManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager)
    {
		m_enable = false;
        m_fastLeave = false;
        m_lmqi = 0;
        m_mrouterTimeout = 0;
        m_qmrt = 0;
        m_querierEnable = false;
        m_queryInterval = 0;
        m_vlanId = 0;
    }

    IgmpsVlanIntfConfManagedObject::IgmpsVlanIntfConfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &lmqi,const UI32 &qmrt,const bool &enable,const bool &fastLeave,const UI32 &queryInterval,const bool &querierEnable,const UI32 &mrouterTimeout,
    		const UI32 &vlanId)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (IgmpsVlanIntfConfManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        m_lmqi    (lmqi),
        m_qmrt    (qmrt),
        m_enable    (enable),
        m_fastLeave    (fastLeave),
        m_queryInterval    (queryInterval),
        m_querierEnable    (querierEnable),
        m_mrouterTimeout    (mrouterTimeout),
        m_vlanId    (vlanId)
    {
    }

    IgmpsVlanIntfConfManagedObject::~IgmpsVlanIntfConfManagedObject ()
    {
    }

    string  IgmpsVlanIntfConfManagedObject::getClassName()
    {
        return ("IgmpsVlanIntfConfManagedObject");
    }

    void  IgmpsVlanIntfConfManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_lmqi,"lmqi", igmp_snooping_last_member_query_interval));
        addPersistableAttribute (new AttributeUI32(&m_qmrt,"qmrt", igmp_snooping_query_max_response_time));
        addPersistableAttribute (new AttributeBool(&m_enable,"enable", igmp_snooping_enable));
        addPersistableAttribute (new AttributeBool(&m_fastLeave,"fastLeave", igmp_snooping_fast_leave));
        addPersistableAttribute (new AttributeUI32(&m_queryInterval,"queryInterval", igmp_snooping_query_interval));
        addPersistableAttribute (new AttributeBool(&m_querierEnable,"querierEnable", igmp_snooping_qenable));
        addPersistableAttribute (new AttributeUI32(&m_mrouterTimeout,"mrouterTimeout", igmp_snooping_mrouter_timeout));
        addPersistableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
    }

    void  IgmpsVlanIntfConfManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_lmqi,"lmqi"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_qmrt,"qmrt"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_enable,"enable"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_fastLeave,"fastLeave"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_queryInterval,"queryInterval"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_querierEnable,"querierEnable"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_mrouterTimeout,"mrouterTimeout"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_vlanId,"vlanId"));
    }

    void  IgmpsVlanIntfConfManagedObject::setLmqi(const UI32 &lmqi)
    {
        m_lmqi  =  lmqi;
    }

    UI32  IgmpsVlanIntfConfManagedObject::getLmqi() const
    {
        return (m_lmqi);
    }

    void  IgmpsVlanIntfConfManagedObject::setQmrt(const UI32 &qmrt)
    {
        m_qmrt  =  qmrt;
    }

    UI32  IgmpsVlanIntfConfManagedObject::getQmrt() const
    {
        return (m_qmrt);
    }

    void  IgmpsVlanIntfConfManagedObject::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  IgmpsVlanIntfConfManagedObject::getEnable() const
    {
        return (m_enable);
    }

    void  IgmpsVlanIntfConfManagedObject::setFastLeave(const bool &fastLeave)
    {
        m_fastLeave  =  fastLeave;
    }

    bool  IgmpsVlanIntfConfManagedObject::getFastLeave() const
    {
        return (m_fastLeave);
    }

    void  IgmpsVlanIntfConfManagedObject::setQueryInterval(const UI32 &queryInterval)
    {
        m_queryInterval  =  queryInterval;
    }

    UI32  IgmpsVlanIntfConfManagedObject::getQueryInterval() const
    {
        return (m_queryInterval);
    }

    void  IgmpsVlanIntfConfManagedObject::setQuerierEnable(const bool &querierEnable)
    {
        m_querierEnable  =  querierEnable;
    }

    bool  IgmpsVlanIntfConfManagedObject::getQuerierEnable() const
    {
        return (m_querierEnable);
    }

    void  IgmpsVlanIntfConfManagedObject::setMrouterTimeout(const UI32 &mrouterTimeout)
    {
        m_mrouterTimeout  =  mrouterTimeout;
    }

    UI32  IgmpsVlanIntfConfManagedObject::getMrouterTimeout() const
    {
        return (m_mrouterTimeout);
    }

    void  IgmpsVlanIntfConfManagedObject::setVlanId(const UI32 &vlanId)
	{
		m_vlanId  =  vlanId;
	}

	UI32  IgmpsVlanIntfConfManagedObject::getVlanId() const
	{
		return (m_vlanId);
	}

    string IgmpsVlanIntfConfManagedObject::customConfigurationDisplay ( WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext)
    {

       string displayString; 
        
       YangDisplayConfigurationContext *pYangDisplayConfigurationContext = waveCustomCliDisplayConfigurationContext->getYangDisplayConfigurationContext ();
       YangElement *pYangElement = waveCustomCliDisplayConfigurationContext->getYangElement();

       WaveLocationPath locationPath;

       locationPath.addForwardStep("igmp-snooping","");
       locationPath.addForwardStep("ip","");
       locationPath.addForwardStep("igmp","");
       locationPath.addForwardStep("snooping","");
       locationPath.addForwardStep("enable","");

       locationPath.computeStringRepresentation ();

       WaveXPathExpressionValue value = pYangDisplayConfigurationContext->evaluateValueOfLocationPath(locationPath, pYangElement);

       if(true == value.getBoolValue())
       {
           if(!m_enable)
           {
                displayString =  "no ip igmp snooping enable\n" ;         
           }

       }
       else
       {
           if(m_enable)
           {
                displayString =  "ip igmp snooping enable\n";
           }
       }
       
        return displayString;
    }

}
