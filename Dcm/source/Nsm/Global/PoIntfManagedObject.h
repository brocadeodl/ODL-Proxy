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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#ifndef POINTFMANAGEDOBJECT_H
#define POINTFMANAGEDOBJECT_H

#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/VlanClassifierActivateGlobalManagedObject.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Global/FcoeWorker/FcoeportAttrGlobalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{
    class NsmGlobalObjectManager;
    class VlanClassifierActivateGlobalManagedObject;
    class NativeVlanIntfConfLocalManagedObject;
    class NsmNodePoManagedObject;

    class PoIntfManagedObject : virtual public PoPhyIntfManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PoIntfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            PoIntfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &id,const UI32 &poMode,const UI32 &poType,const UI32 &poMinLinks, const LineSpeed &poAllowedSpeed, const UI32 &poLoadBalanceMode, const vector<ObjectId> &portMembers);
            virtual    ~PoIntfManagedObject ();
            static string getClassName();
            void setId(const UI32 &id);
            UI32 getId()  const;
            void setPoMode(const UI32 &poMode);
            UI32 getPoMode()  const;
            void setPoType(const UI32 &poType);
            UI32 getPoType()  const;
            void setPoMinLinks(const UI32 &poMinLinks);
            UI32 getPoMinLinks()  const;
			void setPoAllowedSpeed(const LineSpeed &poAllowedSpeed);
			LineSpeed getPoAllowedSpeed() const;
			void setPoLoadBalanceMode(const UI32 &poLoadBalanceMode);
            UI32 getPoLoadBalanceMode()  const;
            void setPoPrimaryLinkDomainId(const UI32 &poPrimaryLinkDomainId);
            UI32 getPoPrimaryLinkDomainId()  const;
            void setPoPartnerSystemPriority(const UI32 &poPartnerSystemPriority);
            UI32 getPoPartnerSystemPriority()  const;
            void setPoPartnerKey(const UI32 &poPartnerKey);
            UI32 getPoPartnerKey()  const;
            void setPoPartnerSystem(const string &poPartnerSystem);
            string getPoPartnerSystem()  const;
            void setPoLacpSystemId(const string &poLacpSystemId);
            string getPoLacpSystemId()  const;
            void setPoLacpSystemIdDomainId(const UI32 &poLacpSystemIdDomainId);
            UI32 getPoLacpSystemIdDomainId()  const;
             void setPortMembers(const vector<ObjectId> &portMembers);
            vector<ObjectId> getPortMembers()  const;
            WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject> getFcoeport() const;
            void setFcoeport(const WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject>);
	    void setVlanClassifierActivate(const vector<WaveManagedObjectPointer<VlanClassifierActivateGlobalManagedObject> > &vlanClassifierActivate);
            vector<WaveManagedObjectPointer<VlanClassifierActivateGlobalManagedObject> > getVlanClassifierActivate()  const;
	    void addVlanClassifierActivate(VlanClassifierActivateGlobalManagedObject *VlanClassifierActivateMO); 
	    int deleteVlanClassifierActivatedGroup(const UI32 &groupId);
		void deleteAllVlanClassifierActivatedGroup();

	    void setNativeVlanIntfConfig(const WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> &nativeVlanIntfConfig);
	    WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> getNativeVlanIntfConfig()  const;
            void setNodePo(const WaveManagedObjectPointer<NsmNodePoManagedObject> &nodePo);
            WaveManagedObjectPointer<NsmNodePoManagedObject> getNodePo()  const;
        /* ELD */
			void setVlanidList(const vector<UI32> &vlanidList);
			vector<UI32> getVlanidList()  const;
			vector<ObjectId> getVlanVector()  const;
			void setEldPrio(const UI32 &eldprio);
			UI32 getEldPrio()  const;
			ResourceId addVlanObjectId(ObjectId &vlanOid);
			ResourceId deleteVlanObjectId(ObjectId &vlanOid);
        /* ELD */
            
// Now the data members

        private:
        protected:
        public:
            UI32  m_id;
            UI32  m_poMode;
            UI32  m_poType;
            UI32  m_poMinLinks;
			LineSpeed m_poAllowedSpeed;
            UI32  m_poLoadBalanceMode;
            UI32  m_poPrimaryLinkDomainId;
            UI32  m_poPartnerSystemPriority;
            UI32  m_poPartnerKey;
            string  m_poPartnerSystem;
            string  m_poLacpSystemId;
            UI32  m_poLacpSystemIdDomainId;
            vector<ObjectId>  m_portMembers;
            WaveManagedObjectPointer<NsmNodePoManagedObject>  m_nodePo;
	    vector<WaveManagedObjectPointer<VlanClassifierActivateGlobalManagedObject> >  m_vlanClassifierActivate;
	    WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject>  m_nativeVlanIntfConfig;
		/* ELD */
		UI32 m_eld_prio_val;
		vector<ObjectId>  m_vlanid;
		vector<UI32>  m_vlanidList;
		/* ELD */
            WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject> m_fcoeport;
    };
}
#endif                                            //POINTFMANAGEDOBJECT_H
