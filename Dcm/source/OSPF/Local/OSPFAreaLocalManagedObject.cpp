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
 *   Author : kverma                                                     *
 **************************************************************************/

#include "OSPF/Local/OSPFAreaLocalManagedObject.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFAreaRangeLocalManagedObject.h"
#include "OSPF/Local/OSPFVirtualLinkLocalManagedObject.h"
#include "ClientInterface/OSPF/OSPFAreaMessage.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "OSPF/Local/OSPFTypes.h"
#include "OSPF/Local/OSPFAreaLocalMessage.h"
#include "vcs.h"

namespace DcmNs
{

    OSPFAreaLocalManagedObject::OSPFAreaLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFAreaLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager)
    {
		m_isNSSA = false;
        m_areaIdInIntFormat = 0;
    }

    OSPFAreaLocalManagedObject::OSPFAreaLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const string &areaId,const bool &Normal, const bool &isStub,const UI32 &stubMetric,const bool &stubNoSummary,const bool &isNSSA,const UI32 &NSSAMetric,const bool &NSSADefInfoOrig,const bool &NSSANoSummary,const vector<WaveManagedObjectPointer<OSPFAreaRangeLocalManagedObject> > &range,const vector<WaveManagedObjectPointer<OSPFVirtualLinkLocalManagedObject> > &virtualLink)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFAreaLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager),
        m_areaId    (areaId),
        m_Normal    (Normal),
        m_isStub    (isStub),
        m_stubMetric    (stubMetric),
        m_stubNoSummary    (stubNoSummary),
        m_isNSSA    (isNSSA),
        m_NSSAMetric    (NSSAMetric),
        m_NSSADefInfoOrig    (NSSADefInfoOrig),
        m_NSSANoSummary    (NSSANoSummary),
        m_range    (range),
        m_virtualLink    (virtualLink)

    {
        setAreaIdInIntFormat(); //converts areaId to integer and stores in m_areaIdInIntFormat
    }

    OSPFAreaLocalManagedObject::~OSPFAreaLocalManagedObject ()
    {
    }

    string  OSPFAreaLocalManagedObject::getClassName()
    {
        return ("OSPFAreaLocalManagedObject");
    }

    void  OSPFAreaLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_areaId,"areaId"));
        addPersistableAttribute (new AttributeBoolUC(&m_Normal,true,"Normal"));
        addPersistableAttribute (new AttributeBool(&m_isStub,"isStub",ospf_stub));
        addPersistableAttribute (new AttributeUI32UC(&m_stubMetric,true,"stubMetric"));
        addPersistableAttribute (new AttributeBoolUC(&m_stubNoSummary,true,"stubNoSummary"));
        addPersistableAttribute (new AttributeBool(&m_isNSSA,"isNSSA"));
        addPersistableAttribute (new AttributeUI32UC(&m_NSSAMetric,true,"NSSAMetric"));
        addPersistableAttribute (new AttributeBoolUC(&m_NSSADefInfoOrig,true,"NSSADefInfoOrig"));
        //addPersistableAttribute (new AttributeBool(&m_NSSANoSummary,"NSSANoSummary",ospf_no_summary));
        addPersistableAttribute (new AttributeBoolUC(&m_NSSANoSummary,true,"NSSANoSummary"));
        addPersistableAttribute (new AttributeUI64(&m_areaIdInIntFormat,"areaIdInIntFormat"));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<OSPFAreaRangeLocalManagedObject>(&m_range,"range", getClassName (), getObjectId (), OSPFAreaRangeLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<OSPFVirtualLinkLocalManagedObject>(&m_virtualLink,"virtualLink", getClassName (), getObjectId (), OSPFVirtualLinkLocalManagedObject::getClassName ()));

		setUserTagForAttribute("areaId", ospf_area_id);
        setUserTagForAttribute("Normal",ospf_normal);
		setUserTagForAttribute("isNSSA", ospf_nssa);
        setUserTagForAttribute("stubMetric",ospf_stub_value);
        setUserTagForAttribute("stubNoSummary",ospf_no_summary);
		setUserTagForAttribute("NSSAMetric", ospf_nssa_value);
		setUserTagForAttribute("NSSADefInfoOrig", ospf_default_information_originate);
        setUserTagForAttribute("NSSANoSummary",ospf_no_summary1);

    }

    void  OSPFAreaLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_areaId,"areaId"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_Normal,true,"Normal"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isStub,"isStub"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_stubMetric,true,"stubMetric"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_stubNoSummary,true,"stubNoSummary"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isNSSA,"isNSSA"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_NSSAMetric,true,"NSSAMetric"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_NSSADefInfoOrig,true,"NSSADefInfoOrig"));
        addPersistableAttributeForCreate  (new AttributeBoolUC(&m_NSSANoSummary,true,"NSSANoSummary"));
        addPersistableAttributeForCreate  (new AttributeUI64(&m_areaIdInIntFormat,"areaIdInIntFormat"));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<OSPFAreaRangeLocalManagedObject>(&m_range,"range", getClassName (), getObjectId (), OSPFAreaRangeLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<OSPFVirtualLinkLocalManagedObject>(&m_virtualLink,"virtualLink", getClassName (), getObjectId (), OSPFVirtualLinkLocalManagedObject::getClassName ()));

    }

    void OSPFAreaLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("areaId");
        setUserDefinedKeyCombination (keyName);
    }

    void  OSPFAreaLocalManagedObject::setAreaId(const string &areaId)
    {
        m_areaId  =  areaId;
        setAreaIdInIntFormat();
    }

    string  OSPFAreaLocalManagedObject::getAreaId() const
    {
        return (m_areaId);
    }

    void  OSPFAreaLocalManagedObject::setNormal(const bool &Normal, const bool &set)
    {
        m_Normal.setBoolValue(Normal);
        m_Normal.setIsUserConfigured(set);
    }

    bool  OSPFAreaLocalManagedObject::getNormal() const
    {
        return (m_Normal.getBoolValue());
    }

    void  OSPFAreaLocalManagedObject::setIsStub(const bool &isStub)
    {
        m_isStub  =  isStub;
    }

    bool  OSPFAreaLocalManagedObject::getIsStub() const
    {
        return (m_isStub);
    }

    void  OSPFAreaLocalManagedObject::setStubMetric(const UI32 &stubMetric, const bool &set)
    {
        //m_stubMetric  =  stubMetric;
        m_stubMetric.setUI32Value(stubMetric);
        m_stubMetric.setIsUserConfigured(set);
    }

    UI32  OSPFAreaLocalManagedObject::getStubMetric() const
    {
        return (m_stubMetric.getUI32Value());
    }

    void  OSPFAreaLocalManagedObject::setStubNoSummary(const bool &stubNoSummary, const bool &set)
    {
        //m_stubNoSummary  =  stubNoSummary;
        m_stubNoSummary.setBoolValue(stubNoSummary);
        m_stubNoSummary.setIsUserConfigured(set);
    }

    bool  OSPFAreaLocalManagedObject::getStubNoSummary() const
    {
        return (m_stubNoSummary.getBoolValue());
    }

    void  OSPFAreaLocalManagedObject::setIsNSSA(const bool &isNSSA)
    {
        m_isNSSA  =  isNSSA;
    }

    bool  OSPFAreaLocalManagedObject::getIsNSSA() const
    {
        return (m_isNSSA);
    }

    void  OSPFAreaLocalManagedObject::setNSSAMetric(const UI32 &NSSAMetric, const bool &set)
    {
        //m_NSSAMetric  =  NSSAMetric;
        m_NSSAMetric.setUI32Value(NSSAMetric);
        m_NSSAMetric.setIsUserConfigured(set);
    }

    UI32  OSPFAreaLocalManagedObject::getNSSAMetric() const
    {
        return (m_NSSAMetric.getUI32Value());
    }

    void  OSPFAreaLocalManagedObject::setNSSADefInfoOrig(const bool &NSSADefInfoOrig, const bool &set)
    {
        //m_NSSADefInfoOrig  =  NSSADefInfoOrig;
        m_NSSADefInfoOrig.setBoolValue(NSSADefInfoOrig);
        m_NSSADefInfoOrig.setIsUserConfigured(set);
    }

    bool  OSPFAreaLocalManagedObject::getNSSADefInfoOrig() const
    {
        return (m_NSSADefInfoOrig.getBoolValue());
    }

    void  OSPFAreaLocalManagedObject::setNSSANoSummary(const bool &NSSANoSummary, const bool &set)
    {
        //m_NSSANoSummary  =  NSSANoSummary;
        m_NSSANoSummary.setBoolValue(NSSANoSummary);
        m_NSSANoSummary.setIsUserConfigured(set);
    }

    bool  OSPFAreaLocalManagedObject::getNSSANoSummary() const
    {
        return (m_NSSANoSummary.getBoolValue());
    }

    void  OSPFAreaLocalManagedObject::setRange(const vector<WaveManagedObjectPointer<OSPFAreaRangeLocalManagedObject> > &range)
    {
        m_range  =  range;
    }

    vector<WaveManagedObjectPointer<OSPFAreaRangeLocalManagedObject> >  OSPFAreaLocalManagedObject::getRange() const
    {
        return (m_range);
    }

	void OSPFAreaLocalManagedObject::setAreaRangeLocalMO(OSPFAreaRangeLocalManagedObject *areaRangeMO)
	{
		WaveManagedObjectPointer<OSPFAreaRangeLocalManagedObject> rangeMO(areaRangeMO);
		m_range.push_back(rangeMO);
	}

    void OSPFAreaLocalManagedObject::setAreaIdInIntFormat()
    {   
         
        m_areaIdInIntFormat = convertOSPFAreaToInteger();
    }

    UI64  OSPFAreaLocalManagedObject::getAreaIdInIntFormat() const
    {
        return (m_areaIdInIntFormat);
    }

    UI64 OSPFAreaLocalManagedObject::convertOSPFAreaToInteger()
    {
         unsigned int ipaddr[4];
         char buf[INET_ADDRSTRLEN];
         UI64 areaIdInIntFormat = 0;
         buf[0] = '\0';
         strcpy(buf, m_areaId.c_str());
         if(strchr (buf, '.'))
         {
            sscanf(buf, "%u.%u.%u.%u", ipaddr, ipaddr+1, ipaddr+2, ipaddr+3);
            areaIdInIntFormat = ( (ipaddr[0] << 24) | (ipaddr[1] << 16) | (ipaddr[2] << 8) | ipaddr[3] ) ;
            areaIdInIntFormat = areaIdInIntFormat << 32;
         }
         else if ( m_areaId.length() )
         {
            areaIdInIntFormat = (UI64)atoi(buf);
         }
        return (areaIdInIntFormat);

    }
   
	OSPFAreaRangeLocalManagedObject * OSPFAreaLocalManagedObject::getAreaRangeLocalMO(IpV4Address rangeIp, IpV4Address rangeMask)
	{
		unsigned int k;

		// VSFIXME if the user has configured 1.1.1.0/24 and wants to configure 1.1.0.0/24,
		// do we overwrite, add or return an error? Fix this depending on the answer.
		// Also see delete_area_range_MO().
		for (k = 0; k < m_range.size(); ++k) {
			trace (TRACE_LEVEL_INFO, "Comparing Range: " +
				   (m_range[k])->m_rangeAddress.toString() + " / " +
				   (m_range[k])->m_rangeMask.toString() + " with " +
				   rangeIp.toString() + " / " + rangeMask.toString());

			if (m_range[k]->m_rangeAddress == rangeIp)
				return (m_range[k].operator->());
		}

		return (0);
	}

	void OSPFAreaLocalManagedObject::deleteAreaRangeMO(OSPFAreaRangeLocalManagedObject *rangeMO)
	{
		unsigned int k;

		trace(TRACE_LEVEL_INFO, "Deleting Range MO: " +
			  rangeMO->m_rangeAddress.toString() + " / " +
			  rangeMO->m_rangeMask.toString());

		for (k = 0; k < m_range.size(); ++k) {
			trace (TRACE_LEVEL_INFO, "Considering Range: " +
				   (m_range[k])->m_rangeAddress.toString() + " / " +
				   (m_range[k])->m_rangeMask.toString());

			// VSFIXME change based on comment in getAreaRangeLocalMO()
			if ((m_range[k])->m_rangeAddress.toString() == rangeMO->m_rangeAddress.toString())
            {
				m_range.erase(m_range.begin() + k);
                break;
            }    
		}
	}

    void  OSPFAreaLocalManagedObject::setVirtualLink(const vector<WaveManagedObjectPointer<OSPFVirtualLinkLocalManagedObject> > &virtualLink)
    {
        m_virtualLink  =  virtualLink;
    }

    vector<WaveManagedObjectPointer<OSPFVirtualLinkLocalManagedObject> >  OSPFAreaLocalManagedObject::getVirtualLink() const
    {
        return (m_virtualLink);
    }

    void OSPFAreaLocalManagedObject::setVirtualLinkLocalMO(OSPFVirtualLinkLocalManagedObject *virtualLinkMO)
    {
        WaveManagedObjectPointer<OSPFVirtualLinkLocalManagedObject> virtualLinkLocalMO (virtualLinkMO);
        m_virtualLink.push_back(virtualLinkLocalMO);
    }

    OSPFVirtualLinkLocalManagedObject * OSPFAreaLocalManagedObject::getVirtualLinkLocalMO(IpV4Address &virtualLinkRouterId)
    {
        unsigned int k;

        for (k = 0; k < m_virtualLink.size(); ++k)
        {
            trace (TRACE_LEVEL_INFO, "Comparing VirtualLink: " +
                    (m_virtualLink[k])->m_virtualLinkRouterId.toString() + "to " + virtualLinkRouterId.toString() );

            if (m_virtualLink[k]->m_virtualLinkRouterId == virtualLinkRouterId)
                return (m_virtualLink[k].operator->());
        }

        return (0);
    }

    void OSPFAreaLocalManagedObject::deleteVirtualLinkLocalMO(OSPFVirtualLinkLocalManagedObject *pVirtualLinkLocalMO)
    {
        unsigned int k;

        trace (TRACE_LEVEL_INFO, "OSPFAreaLocalManagedObject::deleteVirtualLinkLocalMO: Deleting VirtualLinkMO: " +
                pVirtualLinkLocalMO->m_virtualLinkRouterId.toString() );
        for (k = 0; k < m_virtualLink.size(); ++k)
        {
            if (m_virtualLink[k]->m_virtualLinkRouterId == pVirtualLinkLocalMO->m_virtualLinkRouterId) {
                break;
            }
        }
        trace (TRACE_LEVEL_DEVEL, string("OSPFAreaLocalManagedObject::deleteVirtualLinkLocalMO: Value of k=")+k);
        trace (TRACE_LEVEL_DEBUG, string("OSPFAreaLocalManagedObject::deleteVirtualLinkLocalMO: Vm_virtualLinkRouterId=")+(m_virtualLink[k]->m_virtualLinkRouterId.toString()));
        m_virtualLink.erase(m_virtualLink.begin() + k);
        trace (TRACE_LEVEL_DEBUG, "OSPFAreaLocalManagedObject::deleteVirtualLinkLocalMO: Deleted VirtualLinkMO and exiting" );
    }

	void OSPFAreaLocalManagedObject::updateMOFromLocalMessage(OSPFAreaLocalMessage *pMsg)
	{
		trace (TRACE_LEVEL_INFO, string("OSPFAreaLocalManagedObject::updateMOFromLocalMessage: opCode:") + pMsg->getOpCode() + string(" cmdCode:") + pMsg->getCmd());
		setUserSpecifiedPartitionName(pMsg->getPartitionName());
		trace (TRACE_LEVEL_DEBUG, string("OSPFAreaLocalManagedObject::updateMOFromLocalMessage: VRF Name:") + pMsg->getPartitionName());
		switch (pMsg->getOpCode())
		{
			case OSPF_CREATE:
			case OSPF_SET_ELEM:
				switch (pMsg->getCmd())
				{
					case OSPF_AREA_CONFIG_NSSA: 
						setIsNSSA(true);
						setNSSAMetric(pMsg->getNssa(), true);
						setNSSANoSummary(false, false);
						break;

					case OSPF_AREA_CONFIG_NSSA_NO_SUMMARY:
						setIsNSSA(true);
						if (pMsg->getNssa())
							setNSSAMetric(pMsg->getNssa(), true);
						setNSSANoSummary(true, true);
						break;

					case OSPF_AREA_CONFIG_NSSA_DEF_INFO_ORIG:
						setNSSADefInfoOrig(true, true);
						break;

					case OSPF_AREA_CONFIG_STUB:
						setStubMetric(pMsg->getStub(), true);
						setStubNoSummary(false, false);
						break;

					case OSPF_AREA_CONFIG_STUB_NO_SUMMARY:
						if(pMsg->getStub())
							setStubMetric(pMsg->getStub(), true);
						setStubNoSummary(true, true);
						break;

					default:
						trace (TRACE_LEVEL_ERROR, string("OSPFLocalConfWorker::OSPFUpdateAreaMOStep: Unknown cmdCode ")+ pMsg->getCmd());
						break;
				}
				break;
			case OSPF_REMOVE:
				switch (pMsg->getCmd())
				{
					case OSPF_AREA_CONFIG_NSSA: 
						setIsNSSA(false);
						setNSSAMetric(0, false);
						setNSSANoSummary(false, false);
						break;

					case OSPF_AREA_CONFIG_NSSA_NO_SUMMARY:
						setIsNSSA(false);
						setNSSAMetric(0, false);
						setNSSANoSummary(false, false);
						setNSSANoSummary(false, false);
						break;

					case OSPF_AREA_CONFIG_NSSA_DEF_INFO_ORIG:
						setNSSADefInfoOrig(false, false);
						break;

					case OSPF_AREA_CONFIG_STUB:
						setStubMetric(0, false);
						setStubNoSummary(false, false);
						break;

					case OSPF_AREA_CONFIG_STUB_NO_SUMMARY:
						setStubMetric(0, false);
						setStubNoSummary(false, false);
						break;

					default:
						trace (TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateAreaMOStep: Unknown cmdCode ") + pMsg->getCmd());
						break;
				}
				break;
		}
	}

               
	void OSPFAreaLocalManagedObject::updateClientMessageForReplay(OSPFAreaMessage *pMsg)
	{
		pMsg->setOpCode(OSPF_CREATE);
		pMsg->setCmdCode(OSPF_AREA_CONFIG_REPLAY);
		pMsg->setAreaId(m_areaId);
		for (UI32 idx = OSPF_AREA_CONFIG_NSSA; idx < OSPF_AREA_CONFIG_RANGE; idx ++) {
			switch (idx) {
				case OSPF_AREA_CONFIG_NSSA:
					if (m_isNSSA) {
						pMsg->setNssa(m_NSSAMetric.getUI32Value());
						if (m_NSSANoSummary.getBoolValue() == false) {
							pMsg->setOpCode(OSPF_SET_ELEM);
							pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_NSSA);
							
						}
					}
					break;
				case OSPF_AREA_CONFIG_NSSA_NO_SUMMARY:
					if (m_isNSSA && m_NSSANoSummary.getBoolValue()) {
						pMsg->setOpCode(OSPF_SET_ELEM);
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_NSSA_NO_SUMMARY);

					}
					break;
				case OSPF_AREA_CONFIG_NSSA_DEF_INFO_ORIG:
					if (m_NSSADefInfoOrig.getBoolValue()) {
						pMsg->setOpCode(OSPF_SET_ELEM);
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_NSSA);
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_NSSA_DEF_INFO_ORIG);
					}
					break;
				case OSPF_AREA_CONFIG_STUB:
					if (m_stubMetric.getIsUserConfigured()) {
						pMsg->setStub(m_stubMetric.getUI32Value());
						if (m_stubNoSummary.getIsUserConfigured() == false) {
							pMsg->setOpCode(OSPF_SET_ELEM);
							pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_STUB);
						}
					}
					break;
				case OSPF_AREA_CONFIG_STUB_NO_SUMMARY:
					if (m_stubMetric.getIsUserConfigured() && m_stubNoSummary.getIsUserConfigured()) {
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_STUB_NO_SUMMARY);
						pMsg->setOpCode(OSPF_SET_ELEM);
					}
					break;
				default:
					break;
			}
		}
	}
}
