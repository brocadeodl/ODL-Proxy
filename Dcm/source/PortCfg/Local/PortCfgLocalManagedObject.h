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
 *   Author : dchung                                                     *
 **************************************************************************/

#ifndef PORTCFGLOCALMANAGEDOBJECT_H
#define PORTCFGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortCfgLocalObjectManager;
    class PortCfgLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PortCfgLocalManagedObject (PortCfgLocalObjectManager *pPortCfgLocalObjectManager);
            PortCfgLocalManagedObject (PortCfgLocalObjectManager *pPortCfgLocalObjectManager,const string &id,const bool &portStateEnable,const bool &trunkEnable,const UI32 &areaNumber,const bool &longDistance,const bool &vcLinkInit,const bool &disabledEPort,const bool &lockedEPort,const bool &npivCapability,const bool &portAutoDisable,const bool &rateLimit,const bool &mirrorPort,const bool &creditRecovery,const bool &fportBuffers,const UI32 &npivPPLimit,const bool &csctlMode, const int &switchPort, const string &portType, const int &rbridgeId, const string &switchPortType, const int &slot, const int &phyPort, const int &fcoePort, const int &vlan);
            virtual    ~PortCfgLocalManagedObject ();
            static string getClassName();
            void setId(const string &id);
            string getId()  const;
            void setPortStateEnable(const bool &portStateEnable);
            bool getPortStateEnable()  const;
            void setTrunkEnable(const bool &trunkEnable);
            bool getTrunkEnable()  const;
            void setAreaNumber(const UI32 &areaNumber);
            UI32 getAreaNumber()  const;
            void setLongDistance(const bool &longDistance);
            bool getLongDistance()  const;
            void setVcLinkInit(const bool &vcLinkInit);
            bool getVcLinkInit()  const;
            void setDisabledEPort(const bool &disabledEPort);
            bool getDisabledEPort()  const;
            void setLockedEPort(const bool &lockedEPort);
            bool getLockedEPort()  const;
            void setNpivCapability(const bool &npivCapability);
            bool getNpivCapability()  const;
            void setPortAutoDisable(const bool &portAutoDisable);
            bool getPortAutoDisable()  const;
            void setRateLimit(const bool &rateLimit);
            bool getRateLimit()  const;
            void setMirrorPort(const bool &mirrorPort);
            bool getMirrorPort()  const;
            void setCreditRecovery(const bool &creditRecovery);
            bool getCreditRecovery()  const;
            void setFportBuffers(const bool &fportBuffers);
            bool getFportBuffers()  const;
            void setNpivPPLimit(const UI32 &npivPPLimit);
            UI32 getNpivPPLimit()  const;
            void setCsctlMode(const bool &csctlMode);
            bool getCsctlMode()  const;
			void setSwitchPort(const int &switchPort);
			int  getSwitchPort() const;
			void setPortType(const string &portType);
			string getPortType()  const;
			void setRbridgeId(const int &rbridgeId);
			int  getRbridgeId() const;
			void setSwitchPortType(const string &switchPortType);
			string getSwitchPortType()  const;
			void setSlot(const int &slot);
			int  getSlot() const;
			void setPhyPort(const int &phyPort);
			int  getPhyPort() const;
			void setFcoePort(const int &fcoePort);
			int  getFcoePort() const;
			void setVlan(const int &vlan);
			int  getVlan() const;


// Now the data members

        private:
        protected:
        public:
            string  m_id;
            bool  m_portStateEnable;
            bool  m_trunkEnable;
            UI32  m_areaNumber;
            bool  m_longDistance;
            bool  m_vcLinkInit;
            bool  m_disabledEPort;
            bool  m_lockedEPort;
            bool  m_npivCapability;
            bool  m_portAutoDisable;
            bool  m_rateLimit;
            bool  m_mirrorPort;
            bool  m_creditRecovery;
            bool  m_fportBuffers;
            UI32  m_npivPPLimit;
            bool  m_csctlMode;
			int   m_switchPort;
			string  m_portType;
			int   m_rbridgeId;
			string  m_switchPortType;
			int   m_slot;
			int   m_phyPort;
			int   m_fcoePort;
			int   m_vlan;

    };
}
#endif                                            //PORTCFGLOCALMANAGEDOBJECT_H
