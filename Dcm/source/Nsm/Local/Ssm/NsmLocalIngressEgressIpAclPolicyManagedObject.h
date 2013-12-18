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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Bhavin                                                      *
 **************************************************************************/

#ifndef NSMLOCALINGRESSEGRESSIPACLPOLICYMANAGEDOBJECT_H
#define NSMLOCALINGRESSEGRESSIPACLPOLICYMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

#include "Framework/Attributes/AttributeManagedObjectVectorComposition.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Ssm/Global/IpAclPolicyManagedObject.h"
#include "ssmAcl_dcmd_public.h"
#include "vcs.h"
using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmLocalIngressEgressIpAclPolicyManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            NsmLocalIngressEgressIpAclPolicyManagedObject(WaveObjectManager *pNsmLocalObjectManager);
            NsmLocalIngressEgressIpAclPolicyManagedObject(WaveObjectManager *pNsmLocalObjectManager, 
				const ObjectId &ingressEgressIpAcl, const ssmAcl_binding_direction &direction);
			virtual ~NsmLocalIngressEgressIpAclPolicyManagedObject ();
            static string getClassName();

		    void setupKeys();
			void setDirection(ssmAcl_binding_direction &direction);
			ssmAcl_binding_direction getDirection () const;

			ObjectId  getIngressEgressIpAcl() const;
			void  setIngressEgressIpAcl(const ObjectId &ingressEgressIpAcl);
			
        private:
        protected:
        public:
			ObjectId 					m_ingressEgressIpAcl;
			ssmAcl_binding_direction 	m_ipAcldirection;
    };
}
#endif                                            //NSMLOCALINGRESSEGRESSIPACLPOLICYMANAGEDOBJECT_H
