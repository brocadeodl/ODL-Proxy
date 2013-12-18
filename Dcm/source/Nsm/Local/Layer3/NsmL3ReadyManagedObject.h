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
 *   Author : justinh                                                      *
 **************************************************************************/

#ifndef NSML3READYMANAGEDOBJECT_H
#define NSML3READYMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

#include "Framework/Attributes/AttributeManagedObjectComposition.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Nsm/Local/OSPF/OSPFInterfaceLocalManagedObject.h"
#include "Nsm/Local/Dhcp/DhcpRelInterfaceLocalManagedObject.h"
#include "Nsm/Local/VRRPWorker/VRRPSessionConfigLocalManagedObject.h"
#include "Nsm/Local/VRRPWorker/VRRPESessionConfigLocalManagedObject.h"
#include "Nsm/Local/VRRPWorker/VRRPVcsSessionConfigLocalManagedObject.h"
#include "Nsm/Local/Pim/PimIntfCfgLocalManagedObject.h"
#include "Nsm/Local/Igmp/IgmpIntfConfManagedObject.h"
#include "Nsm/Local/Igmp/IgmpSgIntfConfManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpConfigManagedObject.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmIpAddrConfigManagedObject;
	class Dot1xPortConfigLocalManagedObject;
    class PimIntfCfgLocalManagedObject;
    class VRRPSessionConfigLocalManagedObject;
    class VRRPESessionConfigLocalManagedObject;
    class VRRPVcsSessionConfigLocalManagedObject;
    class IgmpIntfConfManagedObject;
    class IgmpSgIntfConfManagedObject;
    class DhcpRelInterfaceLocalManagedObject;
    class NsmL3ReadyManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            NsmL3ReadyManagedObject(WaveObjectManager *pNsmLocalObjectManager);
            virtual ~NsmL3ReadyManagedObject ();
            static string getClassName();

            void setIpConfig(
                const WaveManagedObjectPointer<NsmIpConfigManagedObject> 
                &ipCfg);
            WaveManagedObjectPointer<NsmIpConfigManagedObject> 
                getIpConfig() const;
                
            void setIpAddrConfig(
                const WaveManagedObjectPointer<NsmIpAddrConfigManagedObject> 
                &ipAddrCfg);
            WaveManagedObjectPointer<NsmIpAddrConfigManagedObject> 
                getIpAddrConfig() const;
                
            void setPimSmCfg(
                const WaveManagedObjectPointer<PimIntfCfgLocalManagedObject> 
                &pimsmCfg);
            WaveManagedObjectPointer<PimIntfCfgLocalManagedObject> 
                getPimSmCfg() const;

            void setOSPFIntfConfig(const WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject> &OSPFIntfConfig);
            WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject> getOSPFIntfConfig() const;

            void setVRRPIntfConfig(const vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> > &vrrpIntfConfig);
            vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> > getVRRPIntfConfig()  const;
            void setVRRPEIntfConfig(const vector<WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> > &vrrpeIntfConfig);
            vector<WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> > getVRRPEIntfConfig()  const;
            void setVRRPVcsIntfConfig(const vector<WaveManagedObjectPointer<VRRPVcsSessionConfigLocalManagedObject> > &vrrpVcsIntfConfig);
            vector<WaveManagedObjectPointer<VRRPVcsSessionConfigLocalManagedObject> > getVRRPVcsIntfConfig()  const;

			void addVRRPIntfConfig(VRRPSessionConfigLocalManagedObject *pVRRPSessionConfig);
			void deleteVRRPIntfConfig(UI8 vrid);
			void deleteallVRRPIntfConfig();
			WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> getVRRPIntfSession(UI8 vrid);
			void addVRRPEIntfConfig(VRRPESessionConfigLocalManagedObject *pVRRPSessionConfig);
			void deleteVRRPEIntfConfig(UI8 vrid);
			void deleteallVRRPEIntfConfig();
			WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> getVRRPEIntfSession(UI8 vrid);
			void addVRRPVcsIntfConfig(VRRPVcsSessionConfigLocalManagedObject *pVRRPVcsSessionConfig);
			void deleteVRRPVcsIntfConfig(UI8 vrid);
			WaveManagedObjectPointer<VRRPVcsSessionConfigLocalManagedObject> getVRRPVcsIntfSession(UI8 vrid);
            void setIgmpIntfConf(const WaveManagedObjectPointer<IgmpIntfConfManagedObject> &IgmpIntfConf);
            IgmpIntfConfManagedObject *getIgmpIntfConf();
            void setIgmpSgIntfConf(const vector<WaveManagedObjectPointer<IgmpSgIntfConfManagedObject> > &igmpSgIntfConf);
            vector<WaveManagedObjectPointer<IgmpSgIntfConfManagedObject> > getIgmpSgIntfConf()  const;
            void addIgmpSgIntfConfToVector(IgmpSgIntfConfManagedObject *igmpSgMO);
            void delIgmpSgIntfConfFromVector(IpV4Address groupAddress, UI32 ifType, string ifName);

            void setDhcpRelIntfConf(const vector<WaveManagedObjectPointer<DhcpRelInterfaceLocalManagedObject> > &dhcpRelIntfConf);
            vector<WaveManagedObjectPointer<DhcpRelInterfaceLocalManagedObject> > getDhcpRelIntfConf()  const;
            void addDhcpRelIntfConfToVector(DhcpRelInterfaceLocalManagedObject *dhcpRelMO);
            void delDhcpRelIntfConfFromVector(IpV4Address serverAddress, UI32 ifType, string ifName, UI32 vlanId);//TBD AK - do i need vrfname here????

            InterfaceType getIntfType() const;
            void setIntfType(InterfaceType type);
            bool getCanDeleteL3ReadyMO(void);
            void setInterfaceName(string ifName);
            string getInterfaceName (void) const ;
            bool isL3ConfigPresent();  
        private:
            WaveManagedObjectPointer<NsmIpConfigManagedObject> m_ipConfig;
            WaveManagedObjectPointer<NsmIpAddrConfigManagedObject> m_ipAddrCfg;
			WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject>  m_OSPFIntfConfig;
            WaveManagedObjectPointer<PimIntfCfgLocalManagedObject> m_pimsmCfg;
            vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> >  m_vrrpIntfConfig;
            vector<WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> >  m_vrrpeIntfConfig;
            vector<WaveManagedObjectPointer<VRRPVcsSessionConfigLocalManagedObject> >  m_vrrpVcsIntfConfig;
            WaveManagedObjectPointer<IgmpIntfConfManagedObject> m_igmpIntfConf;
            vector<WaveManagedObjectPointer<IgmpSgIntfConfManagedObject> >  m_igmpSgIntfConf;
            vector<WaveManagedObjectPointer<DhcpRelInterfaceLocalManagedObject> >  m_dhcpRelIntfConf;
            string m_interfaceName;
            InterfaceType m_intf_type;

    };
}
#endif                                            //NSML3READYMANAGEDOBJECT_H
