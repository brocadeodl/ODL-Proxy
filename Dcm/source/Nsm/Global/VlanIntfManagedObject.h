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

#ifndef VLANINTFMANAGEDOBJECT_H
#define VLANINTFMANAGEDOBJECT_H

#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Nsm/Global/IntfManagedObject.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class IgmpsVlanIntfConfManagedObject;
    class IgmpsSgVlanConfigManagedObject;
    class IgmpsMrtrVlanConfigManagedObject;

    class VlanIntfManagedObject : virtual public IntfManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            VlanIntfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
			VlanIntfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager, const UI32 &id, const UI32 &type,const string &ifName,const UI64	&ifIndex,const string &desc,const bool &status,const UI32 &mtu);
            VlanIntfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &id, const WaveManagedObjectPointer<IgmpsVlanIntfConfManagedObject> &igmpsVlanIntfConfig,const vector<WaveManagedObjectPointer<IgmpsSgVlanConfigManagedObject> > &igmpsSgVlanConfig,const vector<WaveManagedObjectPointer<IgmpsMrtrVlanConfigManagedObject> > &igmpsMrtrVlanConfig);
            virtual    ~VlanIntfManagedObject ();
            static string getClassName();
            void setId(const UI32 &id);
            UI32 getId()  const;
            void setStpDisable(const bool &StpDisable);
            bool getStpDisable()  const;
            void setType(const UI32 &type);
            UI32 getType() const;
            void setPVlanType(const pvlanTypeEnum &pVlanType);
            pvlanTypeEnum getPVlanType() const;
            void setPvlanSecAssocRange (const UI32Range &pVlanSecAssocRange);
            UI32Range getPvlanSecAssocRange ()  const;
            void setPvlanRemSecAssocRange(const UI32Range &pVlanRemSecAssocRange);
            UI32Range getPvlanRemSecAssocRange ()  const;
            void setRemoteSpanEnabled(const bool &vlanRemoteSpanEnable);
            bool getRemoteSpanEnabled()  const;
            void setIgmpsVlanIntfConfig(const WaveManagedObjectPointer<IgmpsVlanIntfConfManagedObject> &igmpsVlanIntfConfig);
            WaveManagedObjectPointer<IgmpsVlanIntfConfManagedObject> getIgmpsVlanIntfConfig()  const;
            void setIgmpsSgVlanConfig(const vector<WaveManagedObjectPointer<IgmpsSgVlanConfigManagedObject> > &igmpsSgVlanConfig);
            vector<WaveManagedObjectPointer<IgmpsSgVlanConfigManagedObject> > getIgmpsSgVlanConfig()  const;
            void setIgmpsMrtrVlanConfig(const vector<WaveManagedObjectPointer<IgmpsMrtrVlanConfigManagedObject> > &igmpsMrtrVlanConfig);
            vector<WaveManagedObjectPointer<IgmpsMrtrVlanConfigManagedObject> > getIgmpsMrtrVlanConfig()  const;

            void addIgmpsSgVlanConfigToVector(IgmpsSgVlanConfigManagedObject *igmpsSgMO);
            void delIgmpsSgVlanConfigFromVector(IpV4Address groupAddress, UI32 ifType, string ifName);
            void addIgmpsMrtrVlanConfigToVector(IgmpsMrtrVlanConfigManagedObject *igmpsMrtrMO);
            void delIgmpsMrtrVlanConfigFromVector(UI32 ifType, string ifName);
            virtual bool programHardwareForInputWaveManagedObject (vector<string> &attributeNames, vector<string> &attributeValues, string &clientName);
            virtual bool getPluginDetailsForDistribution (ObjectId &newOperateOnWaveManagedObjectId, PrismServiceId &prismServiceId, vector<LocationId> &locationIds, bool &isNeedSurrogateSupportFlag, bool &isPartialSuccessFlag);
// Now the data members

        private:
        protected:
        public:
            UI32  m_id;
			bool  m_stpDisable;
            UI32  m_type;
			pvlanTypeEnum m_pVlanType;

			UI32Range  m_pVlanSecAssocRange;
			UI32Range  m_pVlanRemSecAssocRange;

			bool  m_remoteSpanEnabled;
            WaveManagedObjectPointer<IgmpsVlanIntfConfManagedObject>  m_igmpsVlanIntfConfig;
            vector<WaveManagedObjectPointer<IgmpsSgVlanConfigManagedObject> >  m_igmpsSgVlanConfig;
            vector<WaveManagedObjectPointer<IgmpsMrtrVlanConfigManagedObject> >  m_igmpsMrtrVlanConfig;
    };
}
#endif                                            //VLANINTFMANAGEDOBJECT_H
