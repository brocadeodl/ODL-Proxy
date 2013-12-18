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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "Nsm/Global/IntfManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "vcs.h"
#include "Ssm/Global/MacAclPolicyManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "string.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Local/Ssm/NsmLocalIngressEgressIpAclPolicyManagedObject.h"
#include "Nsm/Local/Ssm/NsmLocalPBRManagedObject.h"
#include "Nsm/Local/Ssm/NsmLocalIngressEgressMacAclPolicyManagedObject.h"
#include "ssmAcl_public.h"

namespace DcmNs
{

    IntfManagedObject::IntfManagedObject (WaveObjectManager *pWaveObjectManager)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (IntfManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager)
    {
		m_ifIndex = 0;
		m_mtu = NSM_MTU_DEF_VAL;
		m_status = false;
		m_active = true;
    }

    IntfManagedObject::IntfManagedObject (WaveObjectManager *pWaveObjectManager,const string &ifName,const UI64 &ifIndex,const string &desc,const bool &status,const UI32 &mtu)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (IntfManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        m_ifName    (ifName),
        m_ifIndex    (ifIndex),
        m_desc    (desc),
        m_status    (status),
        m_mtu    (mtu)
    {
		m_active = true;
    }

    IntfManagedObject::~IntfManagedObject ()
    {
    }

    string  IntfManagedObject::getClassName()
    {
        return ("IntfManagedObject");
    }

    void  IntfManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_ifName,"ifName"));
        addPersistableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex",brocade_interface_ifindex));
        addPersistableAttribute (new AttributeString(&m_desc,true,"","description",brocade_interface_description));
        addPersistableAttribute (new AttributeBool(&m_status,"status",brocade_interface_shutdown));
        addPersistableAttribute (new AttributeBool(&m_active,"active"));
        addPersistableAttribute (new AttributeUI32(&m_mtu,"mtu",brocade_interface_mtu));
        addPersistableAttribute (new AttributeManagedObjectComposition<NsmL3ReadyManagedObject>(&m_layer3Ready,"layer3Ready", NsmL3ReadyManagedObject::getClassName(), true));
		/* Mac ACL */
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<NsmLocalIngressEgressMacAclPolicyManagedObject>(&m_ingressEgressMacAclPolicy, "ingressEgressMacAclPolicy", getClassName (), getObjectId (), NsmLocalIngressEgressMacAclPolicyManagedObject::getClassName()));
		/* Ip Acl */
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<NsmLocalIngressEgressIpAclPolicyManagedObject>(&m_ingressEgressIpAclPolicy, "ingressEgressIpAclPolicy", getClassName (), getObjectId (), NsmLocalIngressEgressIpAclPolicyManagedObject::getClassName()));
        addPersistableAttribute (new AttributeManagedObjectComposition<NsmLocalPBRManagedObject>(&m_PBR, "interfaceRouteMap", NsmLocalPBRManagedObject::getClassName(), true));
		//addPersistableAttribute (new AttributeManagedObjectVectorComposition<NsmLocalPBRManagedObject> (&m_PBR, "interfaceRouteMap",  getClassName (), getObjectId (), NsmLocalPBRManagedObject::getClassName()));

    }

    void  IntfManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_ifName,"ifName"));
        addPersistableAttributeForCreate (new AttributeUI64(&m_ifIndex,"ifIndex",brocade_interface_ifindex));
        addPersistableAttributeForCreate (new AttributeString(&m_desc,"description",brocade_interface_description));
        addPersistableAttributeForCreate  (new AttributeBool(&m_status,"status",brocade_interface_shutdown));
        addPersistableAttributeForCreate  (new AttributeBool(&m_active,"active"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_mtu,"mtu",brocade_interface_mtu));
        addPersistableAttributeForCreate (new AttributeManagedObjectComposition<NsmL3ReadyManagedObject>(&m_layer3Ready,"layer3Ready", NsmL3ReadyManagedObject::getClassName(), true));
		/* Mac ACL */
		addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<NsmLocalIngressEgressMacAclPolicyManagedObject>(&m_ingressEgressMacAclPolicy, "ingressEgressMacAclPolicy", getClassName (), getObjectId (), NsmLocalIngressEgressMacAclPolicyManagedObject::getClassName()));
		/* Ip ACL */
 		addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<NsmLocalIngressEgressIpAclPolicyManagedObject>(&m_ingressEgressIpAclPolicy, "ingressEgressIpAclPolicy", getClassName (), getObjectId (), NsmLocalIngressEgressIpAclPolicyManagedObject::getClassName())); 
        addPersistableAttributeForCreate (new AttributeManagedObjectComposition<NsmLocalPBRManagedObject>(&m_PBR, "interfaceRouteMap", NsmLocalPBRManagedObject::getClassName(), true));
		//addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<NsmLocalPBRManagedObject> (&m_PBR, "interfaceRouteMap",  getClassName (), getObjectId (), NsmLocalPBRManagedObject::getClassName()));
    }

    void  IntfManagedObject::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  IntfManagedObject::getIfName() const
    {
        return (m_ifName);
    }

    void  IntfManagedObject::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  IntfManagedObject::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  IntfManagedObject::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  IntfManagedObject::getDesc() const
    {
        return (m_desc);
    }

    void  IntfManagedObject::setActive(const bool &active)
    {
        m_active  =  active;
    }

    bool  IntfManagedObject::getActive() const
    {
        return (m_active);
    }

    void  IntfManagedObject::setStatus(const bool &status)
    {
        m_status  =  status;
    }

    bool  IntfManagedObject::getStatus() const
    {
        return (m_status);
    }

    void  IntfManagedObject::setMtu(const UI32 &mtu)
    {
        m_mtu  =  mtu;
    }

    UI32  IntfManagedObject::getMtu() const
    {
        return (m_mtu);
    }
    
    void  IntfManagedObject::setIngressEgressMacAclPolicy(const vector<WaveManagedObjectPointer<NsmLocalIngressEgressMacAclPolicyManagedObject> > &ingressEgressMacAclPolicy)
	{
        m_ingressEgressMacAclPolicy  =  ingressEgressMacAclPolicy;
	}

    vector<WaveManagedObjectPointer<NsmLocalIngressEgressMacAclPolicyManagedObject> >IntfManagedObject::getIngressEgressMacAclPolicy() const
	{
        return (m_ingressEgressMacAclPolicy);
	}

	int IntfManagedObject::delIngressEgressMacAclManagedObjectFromVector(const ssmAcl_binding_direction &direction)
    {

        unsigned int i;

        for (i = 0; i < m_ingressEgressMacAclPolicy.size(); i++) {
            if (m_ingressEgressMacAclPolicy[i]->getDirection() == direction) {
                trace(TRACE_LEVEL_INFO, string("IngressEgressMacAclPolicyManagedObject with direction ") + direction);
                m_ingressEgressMacAclPolicy.erase(m_ingressEgressMacAclPolicy.begin() + i);
                return (1);
            }
        }

        return (0);

    }

	int IntfManagedObject::delIngressEgressMacAclManagedObjectFromVectorAll(const ssmAcl_binding_direction &direction)
    {
        unsigned int i;
		if (direction == SSMACL_BINDING_DIRECTION_BOTH_E) {
			for (i = 0; i < m_ingressEgressMacAclPolicy.size(); i++) {
					trace(TRACE_LEVEL_INFO, string("IngressEgressMacAclPolicyManagedObject with direction ") + direction);
					m_ingressEgressMacAclPolicy.erase(m_ingressEgressMacAclPolicy.begin() + i);
			}
			return (1);
		}
        return (0);
	}

	int IntfManagedObject::delIngressEgressIpAclManagedObjectFromVectorAll(const ssmAcl_binding_direction &direction)
    {
        unsigned int i;
        if (direction == SSMACL_BINDING_DIRECTION_BOTH_E) {
            for (i = 0; i < m_ingressEgressIpAclPolicy.size(); i++) {
                    trace(TRACE_LEVEL_INFO, string("IngressEgressIpAclPolicyManagedObject with direction ") + direction);
                    m_ingressEgressIpAclPolicy.erase(m_ingressEgressIpAclPolicy.begin() + i);
            }
            return (1);
        }
        return (0);
    }

	int IntfManagedObject::getIngressEgressMacAclPolicyObjectId(ObjectId &policyObjId, const ssmAcl_binding_direction direction)
	{
		unsigned int iter = 0;
		int found = 0;
		for (iter = 0 ; iter < m_ingressEgressMacAclPolicy.size(); iter++)
		{
			if((m_ingressEgressMacAclPolicy[iter]->getDirection()) == direction) {
				policyObjId = (m_ingressEgressMacAclPolicy[iter]->getIngressEgressMacAcl());
				found = 1;
				break;
			}
		}
			if (found) {
				trace(TRACE_LEVEL_INFO, string ("IntfManagedObject::found ingressEgressMacAclPolicyObjectId with direction ") + direction);
				return (1);
			} else 
				return (0);
			
	}
	
	int IntfManagedObject::getIngressEgressMacAclPolicyDirectioncount (const ssmAcl_binding_direction direction) 
	{
        unsigned int iter = 0;
		int found = 0;
		trace(TRACE_LEVEL_INFO,string("Inside getIngressEgressMacAclPolicyDirectioncount"));		
        for (iter = 0 ; iter < m_ingressEgressMacAclPolicy.size(); iter++) {
            if (direction == SSMACL_BINDING_DIRECTION_BOTH_E) {
				if ((m_ingressEgressMacAclPolicy[iter]->getDirection() == SSMACL_BINDING_DIRECTION_IN_E) || (m_ingressEgressMacAclPolicy[iter]->getDirection() == SSMACL_BINDING_DIRECTION_OUT_E)) {
					found = 1;
					break;
				}
			} else if ((m_ingressEgressMacAclPolicy[iter]->getDirection()) == direction) {
				found = 1;				                
                break;
            }
        }
        if (found) {
            trace(TRACE_LEVEL_INFO, string(" IntfManagedObject::found ingressEgressMacAclPolicy with direction ") + direction);
            return (1);
		} else
			return (0);
		
	}
	
	void IntfManagedObject::addIngressEgressMacAclManagedObjectToVector(NsmLocalIngressEgressMacAclPolicyManagedObject *NsmIngressEgressMacAclMO)
    {
        WaveManagedObjectPointer<NsmLocalIngressEgressMacAclPolicyManagedObject> nsmIngressEgressMacAclMO(NsmIngressEgressMacAclMO);
        m_ingressEgressMacAclPolicy.push_back(nsmIngressEgressMacAclMO);
    }
    
    void  IntfManagedObject::setIngressEgressIpAclPolicy(const vector<WaveManagedObjectPointer<NsmLocalIngressEgressIpAclPolicyManagedObject> > &ingressEgressIpAclPolicy)
    {
        m_ingressEgressIpAclPolicy  =  ingressEgressIpAclPolicy;
    }

    vector<WaveManagedObjectPointer<NsmLocalIngressEgressIpAclPolicyManagedObject> >IntfManagedObject::getIngressEgressIpAclPolicy() const
    {
        return (m_ingressEgressIpAclPolicy);
    }

	int IntfManagedObject::delIngressEgressIpAclManagedObjectFromVector(const ssmAcl_binding_direction &direction)
    {

        unsigned int i;

        for (i = 0; i < m_ingressEgressIpAclPolicy.size(); i++) {
            if (m_ingressEgressIpAclPolicy[i]->getDirection() == direction) {
                trace(TRACE_LEVEL_INFO, string("IngressEgressIpAclPolicyManagedObject with direction ") + direction);
                m_ingressEgressIpAclPolicy.erase(m_ingressEgressIpAclPolicy.begin() + i);
                return (1);
            }
        }

        return (0);

    }
	
    int IntfManagedObject::getPBRNameFromInterface(string &routeMapName)
    {
        routeMapName = m_PBR->getRouteMapName();
        return 1;
    }

	int IntfManagedObject::getIngressEgressIpAclPolicyObjectId(ObjectId &policyObjId, const ssmAcl_binding_direction direction)
	{
		unsigned int iter = 0;
		int found = 0;
		for (iter = 0 ; iter < m_ingressEgressIpAclPolicy.size(); iter++)
		{
			if((m_ingressEgressIpAclPolicy[iter]->getDirection()) == direction) {
				policyObjId = (m_ingressEgressIpAclPolicy[iter]->getIngressEgressIpAcl());
				found = 1;
				break;
			}
		}
			if (found) {
				trace(TRACE_LEVEL_INFO, string ("IntfManagedObject::found ingressEgressIpAclPolicyObjectId with direction ") + direction);
				return (1);
			} else 
				return (0);
			
	}
	
	int IntfManagedObject::getIngressEgressIpAclPolicyDirectioncount (const ssmAcl_binding_direction direction) 
	{
        unsigned int iter = 0;
		int found = 0;
        trace(TRACE_LEVEL_DEBUG,string("Inside getIngressEgressIpAclPolicyDirectioncount"));
        for (iter = 0 ; iter < m_ingressEgressIpAclPolicy.size(); iter++) {
            if (direction == SSMACL_BINDING_DIRECTION_BOTH_E) {
                if ((m_ingressEgressIpAclPolicy[iter]->getDirection() == SSMACL_BINDING_DIRECTION_IN_E) || (m_ingressEgressIpAclPolicy[iter]->getDirection() == SSMACL_BINDING_DIRECTION_OUT_E)) {
                    found = 1;
                    break;
                }
            } else if ((m_ingressEgressIpAclPolicy[iter]->getDirection()) == direction) {
                found = 1;
                break;
            }
        }
        if (found) {
            trace(TRACE_LEVEL_DEBUG, string(" IntfManagedObject::found ingressEgressIpAclPolicy with direction ") + direction);
            return (1);
        } else
            return (0);
	}
	
	void IntfManagedObject::addIngressEgressIpAclManagedObjectToVector(NsmLocalIngressEgressIpAclPolicyManagedObject *NsmIngressEgressIpAclMO)
    {
        WaveManagedObjectPointer<NsmLocalIngressEgressIpAclPolicyManagedObject> nsmIngressEgressIpAclMO(NsmIngressEgressIpAclMO);
        m_ingressEgressIpAclPolicy.push_back(nsmIngressEgressIpAclMO);
    }

    void  IntfManagedObject::setPBR(const WaveManagedObjectPointer<NsmLocalPBRManagedObject> &PBR)
    {
        m_PBR  =  PBR;
    }

    WaveManagedObjectPointer<NsmLocalPBRManagedObject> IntfManagedObject::getPBR() const
    {
        return (m_PBR);
    }

    NsmLocalPBRManagedObject* IntfManagedObject::getPBRMO() const
    {
        return (WaveManagedObjectPointer<NsmLocalPBRManagedObject>
            (m_PBR)).operator->();
    }

	
    //int IntfManagedObject::delNsmLocalPBRManagedObjectFromVector()
    //{

        //unsigned int i;
		/*
        for (i = 0; i < m_PBR.size(); i++) {
            if (m_PBR[i] == pNsmLocalPBRManagedObject) {
                trace(TRACE_LEVEL_INFO, string("NsmLocalPBRManagedObject Delete "));
                m_PBR.erase(m_PBR.begin() + i);
                return (1);
            }
        } */
	//  	m_PBR.erase(m_PBR.begin() + 0);

    //    return (0);

    //}

    //void IntfManagedObject::addNsmLocalPBRManagedObjectToVector(NsmLocalPBRManagedObject *NsmPbrMO)
    //{
    //    WaveManagedObjectPointer<NsmLocalPBRManagedObject> nsmPbrMO(NsmPbrMO);
    //    m_PBR.push_back(nsmPbrMO);
    //}	

    void  IntfManagedObject::setL3Ready(const WaveManagedObjectPointer<NsmL3ReadyManagedObject> &l3ready)
    {
        m_layer3Ready  =  l3ready;
    }

    WaveManagedObjectPointer<NsmL3ReadyManagedObject> IntfManagedObject::getL3Ready() const
    {
        return (m_layer3Ready);
    }

    NsmL3ReadyManagedObject* IntfManagedObject::getL3ReadyMO() const
    {
        return (WaveManagedObjectPointer<NsmL3ReadyManagedObject>
            (m_layer3Ready)).operator->();
    }

    const NsmIpAddrConfigManagedObject*
        IntfManagedObject::getIpAddrConfigMO() const
    {
        const NsmL3ReadyManagedObject *pL3ReadyMo = getL3ReadyMO();

        if (pL3ReadyMo) {
            return (WaveManagedObjectPointer<NsmIpAddrConfigManagedObject>
                (pL3ReadyMo->getIpAddrConfig())).operator->();
        }

        return NULL;
    }
	
    const NsmIpConfigManagedObject* 
        IntfManagedObject::getIpConfigMO() const
    {
        const NsmL3ReadyManagedObject *pL3ReadyMo = getL3ReadyMO();

        if (pL3ReadyMo) {
            return (WaveManagedObjectPointer<NsmIpConfigManagedObject>
                (pL3ReadyMo->getIpConfig())).operator->();
        }
        
        return NULL;
    }
}
