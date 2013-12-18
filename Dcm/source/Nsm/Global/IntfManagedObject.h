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

#ifndef INTFMANAGEDOBJECT_H
#define INTFMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/Ssm/NsmLocalIngressEgressIpAclPolicyManagedObject.h"
#include "Nsm/Local/Ssm/NsmLocalIngressEgressMacAclPolicyManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Nsm/Local/Ssm/NsmLocalPBRManagedObject.h"

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{
	class NsmLocalPBRManagedObject;
	class NsmLocalIngressEgressIpAclPolicyManagedObject;
	class NsmLocalIngressEgressMacAclPolicyManagedObject;
	
    class IntfManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            IntfManagedObject (WaveObjectManager *pWaveObjectManager);
            IntfManagedObject (WaveObjectManager *pWaveObjectManager,const string &ifName,const UI64 &ifIndex,const string &desc,const bool &status,const UI32 &mtu);
            virtual    ~IntfManagedObject ();
            static string getClassName();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setIfIndex(const UI64 &ifIndex);
            UI64 getIfIndex()  const;
            void setDesc(const string &desc);
            string getDesc()  const;
            void setStatus(const bool &status);
            bool getStatus()  const;
            void setActive(const bool &status);
            bool getActive()  const;
            void setMtu(const UI32 &mtu);
            UI32 getMtu()  const;
            void setL3Ready(const WaveManagedObjectPointer<NsmL3ReadyManagedObject> &l3ready);
            WaveManagedObjectPointer<NsmL3ReadyManagedObject> getL3Ready()  const;

            NsmL3ReadyManagedObject* getL3ReadyMO()  const;
            const NsmIpAddrConfigManagedObject* getIpAddrConfigMO()  const;
            const NsmIpConfigManagedObject* getIpConfigMO()  const;

			/* Mac Acl :Methods */
			void  setIngressEgressMacAclPolicy(const vector<WaveManagedObjectPointer<NsmLocalIngressEgressMacAclPolicyManagedObject> > &ingressEgressMacAclPolicy);

			vector<WaveManagedObjectPointer<NsmLocalIngressEgressMacAclPolicyManagedObject> > getIngressEgressMacAclPolicy() const;

			int getIngressEgressMacAclPolicyObjectId(ObjectId &policyObjId, const ssmAcl_binding_direction direction);
			
			void addIngressEgressMacAclManagedObjectToVector(NsmLocalIngressEgressMacAclPolicyManagedObject *NsmIngressEgressMacAclMO);
			
			int delIngressEgressMacAclManagedObjectFromVector(const ssmAcl_binding_direction &direction);
			
			/* For both directions : Used in slotfailover */
			int delIngressEgressMacAclManagedObjectFromVectorAll(const ssmAcl_binding_direction &direction);
			int delIngressEgressIpAclManagedObjectFromVectorAll(const ssmAcl_binding_direction &direction);
			
			int getIngressEgressMacAclPolicyDirectioncount (const ssmAcl_binding_direction direction);
			
			/* IP Acl :Methods */
			void  setIngressEgressIpAclPolicy(const vector<WaveManagedObjectPointer<NsmLocalIngressEgressIpAclPolicyManagedObject> > &ingressEgressIpAclPolicy);

			vector<WaveManagedObjectPointer<NsmLocalIngressEgressIpAclPolicyManagedObject> > getIngressEgressIpAclPolicy() const;

			int getIngressEgressIpAclPolicyObjectId(ObjectId &policyObjId, const ssmAcl_binding_direction direction);
			
			void addIngressEgressIpAclManagedObjectToVector(NsmLocalIngressEgressIpAclPolicyManagedObject *NsmIngressEgressIpAclMO);
			int delIngressEgressIpAclManagedObjectFromVector(const ssmAcl_binding_direction &direction);
			
			int getIngressEgressIpAclPolicyDirectioncount ( const ssmAcl_binding_direction direction);
			
			/* End IP Acl :Methods */ 

			/* PBR methods */
            void  setPBR(const WaveManagedObjectPointer<NsmLocalPBRManagedObject> &pbr_);

            WaveManagedObjectPointer<NsmLocalPBRManagedObject> getPBR() const;
			NsmLocalPBRManagedObject* getPBRMO() const;
			//int delNsmLocalPBRManagedObjectFromVector();
			//void addNsmLocalPBRManagedObjectToVector(NsmLocalPBRManagedObject *NsmPbrMO);
			int getPBRNameFromInterface(string &routeMapName);

			/* End of PBR methods */
 

// Now the data members

        private:
        protected:
        public:
            string     m_ifName;
            UI64       m_ifIndex;
            string     m_desc;
            bool       m_status;
            bool       m_active;
            UI32       m_mtu;
            WaveManagedObjectPointer<NsmL3ReadyManagedObject> m_layer3Ready;
			vector<WaveManagedObjectPointer<NsmLocalIngressEgressMacAclPolicyManagedObject> >	m_ingressEgressMacAclPolicy;
			vector<WaveManagedObjectPointer<NsmLocalIngressEgressIpAclPolicyManagedObject> >	m_ingressEgressIpAclPolicy;
			//WaveManagedObjectPointer<NsmLocalPBRManagedObject>    m_PBR;
			WaveManagedObjectPointer<NsmLocalPBRManagedObject > m_PBR;
    };
}
#endif                                            //INTFMANAGEDOBJECT_H
