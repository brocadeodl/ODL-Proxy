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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : vbagavat                                                     *
 **************************************************************************/

#ifndef PORTFCCFGLOCALMANAGEDOBJECT_H
#define PORTFCCFGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortCfgLocalObjectManager;
    class PortFCCfgLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PortFCCfgLocalManagedObject (PortCfgLocalObjectManager *pPortCfgLocalObjectManager);
PortFCCfgLocalManagedObject (PortCfgLocalObjectManager *pPortCfgLocalObjectManager,const string &id,const bool &portStateShutdown,const UI32 &fillWord,const UI32 &longDistance,const UI32 &vcLinkInit,const bool &islRRdyMode,const bool &status,const UI32 &switchPort,const UI32 &cliPort,const string &type,const UI32 &slotPort,const SI32 &slot,const UI32 &oldDistance,const bool &trunkEnable,const UI32 &speed,const bool &fcAccConfig,const bool &disableEport,const bool &lportLockdown,const bool &privLportLockdown,const bool &vcxltLinit,const UI32 &delayFlogi,const bool &islInterop,const bool &pvtPortPersistentlyDisabled,const UI32 &distance,const bool &bufStarvFlag,const bool &lport_halfduplex,const bool &lport_fairness,const bool &credit_sharing,const bool &pvt_port_permanently_disabled,const bool &npiv_enabled,const bool &npiv_configured,const bool &rscn_suppress,const bool &ex_port,const bool &mirror_port,const bool &soft_neg,const bool &qos_enabled,const bool &qos_configured,const UI32 &rlimit,const bool &alpa13,const UI32 &export_mode,const bool &losig,const bool &asn_frc_hwretry,const bool &cr_recov,const bool &auto_disable,const bool &fport_buffers, const UI32 &rbridgeId,const string &clear_counter_ts,const UI32 &lock_port);
 
            virtual    ~PortFCCfgLocalManagedObject ();
            static string getClassName();
            void setId(const string &id);
            string getId()  const;
            void setPortStateShutdown(const bool &portStateShutdown);
            bool getPortStateShutdown()  const;
            void setFillWord(const UI32 &fillWord);
            UI32 getFillWord()  const;
            void setLongDistance(const UI32 &longDistance);
            UI32 getLongDistance()  const;
            void setVcLinkInit(const UI32 &vcLinkInit);
            UI32 getVcLinkInit()  const;
            void setIslRRdyMode(const bool &islRRdyMode);
            bool getIslRRdyMode()  const;
			void setStatus(const bool &status);
			bool getStatus()  const;
            void setSwitchPort(const UI32 &switchPort);
            UI32 getSwitchPort()  const;
            void setCliPort(const UI32 &cliPort);
            UI32 getCliPort()  const;
            void setType(const string &type);
            string getType()  const;
            void setSlotPort(const UI32 &slotPort);
            UI32 getSlotPort()  const;
            void setSlot(const SI32 &slot);
            SI32 getSlot()  const;
            void setOldDistance(const UI32 &oldDistance);
            UI32 getOldDistance()  const;
            void setTrunkEnable(const bool &trunkEnable);
            bool getTrunkEnable()  const;
            void setSpeed(const UI32 &speed);
            UI32 getSpeed()  const;
            void setFcAccConfig(const bool &fcAccConfig);
            bool getFcAccConfig()  const;
            void setDisableEport(const bool &disableEport);
            bool getDisableEport()  const;
            void setLportLockdown(const bool &lportLockdown);
            bool getLportLockdown()  const;
            void setPrivLportLockdown(const bool &privLportLockdown);
            bool getPrivLportLockdown()  const;
            void setVcxltLinit(const bool &vcxltLinit);
            bool getVcxltLinit()  const;
            void setDelayFlogi(const UI32 &delayFlogi);
            UI32 getDelayFlogi()  const;
            void setIslInterop(const bool &islInterop);
            bool getIslInterop()  const;
            void setPvtPortPersistentlyDisabled(const bool &pvtPortPersistentlyDisabled);
            bool getPvtPortPersistentlyDisabled()  const;
            void setDistance(const UI32 &distance);
            UI32 getDistance()  const;
            void setBufStarvFlag(const bool &bufStarvFlag);
            bool getBufStarvFlag()  const;
            void setLport_halfduplex(const bool &lport_halfduplex);
            bool getLport_halfduplex()  const;
            void setLport_fairness(const bool &lport_fairness);
            bool getLport_fairness()  const;
            void setCredit_sharing(const bool &credit_sharing);
            bool getCredit_sharing()  const;
            void setPvt_port_permanently_disabled(const bool &pvt_port_permanently_disabled);
            bool getPvt_port_permanently_disabled()  const;
            void setNpiv_enabled(const bool &npiv_enabled);
            bool getNpiv_enabled()  const;
            void setNpiv_configured(const bool &npiv_configured);
            bool getNpiv_configured()  const;
            void setRscn_suppress(const bool &rscn_suppress);
            bool getRscn_suppress()  const;
            void setEx_port(const bool &ex_port);
            bool getEx_port()  const;
            void setMirror_port(const bool &mirror_port);
            bool getMirror_port()  const;
            void setSoft_neg(const bool &soft_neg);
            bool getSoft_neg()  const;
            void setQos_enabled(const bool &qos_enabled);
            bool getQos_enabled()  const;
            void setQos_configured(const bool &qos_configured);
            bool getQos_configured()  const;
            void setRlimit(const UI32 &rlimit);
            UI32 getRlimit()  const;
            void setAlpa13(const bool &alpa13);
            bool getAlpa13()  const;
            void setExport_mode(const UI32 &export_mode);
            UI32 getExport_mode()  const;
            void setLosig(const bool &losig);
            bool getLosig()  const;
            void setAsn_frc_hwretry(const bool &asn_frc_hwretry);
            bool getAsn_frc_hwretry()  const;
            void setCr_recov(const bool &cr_recov);
            bool getCr_recov()  const;
            void setAuto_disable(const bool &auto_disable);
            bool getAuto_disable()  const;
            void setFport_buffers(const bool &fport_buffers);
            bool getFport_buffers()  const;
            void setRbridgeId(const UI32 &rbridgeId);
            UI32 getRbridgeId () const;
            void setClear_counter_ts(const string &clear_counter_ts);
            string getClear_counter_ts()  const;
            void setLock_port(const UI32 &lock_port);
            UI32 getLock_port()  const;
// Now the data members

        private:
        protected:
        public:
            string  m_id;
            bool  m_portStateShutdown;
            UI32  m_fillWord;
            UI32  m_longDistance;
            UI32  m_vcLinkInit;
            bool  m_islRRdyMode;
            bool  m_status;
            UI32  m_switchPort;
            UI32  m_cliPort;
            string  m_type;
            UI32  m_slotPort;
            SI32  m_slot;
            UI32  m_oldDistance;
            bool  m_trunkEnable;
            UI32  m_speed;
            bool  m_fcAccConfig;
            bool  m_disableEport;
            bool  m_lportLockdown;
            bool  m_privLportLockdown;
            bool  m_vcxltLinit;
            UI32  m_delayFlogi;
            bool  m_islInterop;
            bool  m_pvtPortPersistentlyDisabled;
            UI32  m_distance;
            bool  m_bufStarvFlag;
            bool  m_lport_halfduplex;
            bool  m_lport_fairness;
            bool  m_credit_sharing;
            bool  m_pvt_port_permanently_disabled;
            bool  m_npiv_enabled;
            bool  m_npiv_configured;
            bool  m_rscn_suppress;
            bool  m_ex_port;
            bool  m_mirror_port;
            bool  m_soft_neg;
            bool  m_qos_enabled;
            bool  m_qos_configured;
            UI32  m_rlimit;
            bool  m_alpa13;
            UI32  m_export_mode;
            bool  m_losig;
            bool  m_asn_frc_hwretry;
            bool  m_cr_recov;
            bool  m_auto_disable;
            bool  m_fport_buffers;
            UI32  m_rbridgeId;
            string  m_clear_counter_ts;
            UI32  m_lock_port;
    };
}
#endif                                            //PORTFCCFGLOCALMANAGEDOBJECT_H
