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

#include "PortCfg/Local/PortFCCfgLocalManagedObject.h"
#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgTypes.h"

#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"


namespace DcmNs
{

    PortFCCfgLocalManagedObject::PortFCCfgLocalManagedObject (PortCfgLocalObjectManager *pPortCfgLocalObjectManager)
        : PrismElement  (pPortCfgLocalObjectManager),
        PrismPersistableObject (PortFCCfgLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pPortCfgLocalObjectManager),
        DcmManagedObject (pPortCfgLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pPortCfgLocalObjectManager)
    {
    }

    PortFCCfgLocalManagedObject::PortFCCfgLocalManagedObject (PortCfgLocalObjectManager *pPortCfgLocalObjectManager,const string &id,const bool &portStateShutdown,const UI32 &fillWord,const UI32 &longDistance,const UI32 &vcLinkInit,const bool &islRRdyMode, const bool &status, const UI32 &switchPort,const UI32 &cliPort,const string &type,const UI32 &slotPort,const SI32 &slot,const UI32 &oldDistance,const bool &trunkEnable,const UI32 &speed,const bool &fcAccConfig,const bool &disableEport,const bool &lportLockdown,const bool &privLportLockdown,const bool &vcxltLinit,const UI32 &delayFlogi,const bool &islInterop,const bool &pvtPortPersistentlyDisabled,const UI32 &distance,const bool &bufStarvFlag,const bool &lport_halfduplex,const bool &lport_fairness,const bool &credit_sharing,const bool &pvt_port_permanently_disabled,const bool &npiv_enabled,const bool &npiv_configured,const bool &rscn_suppress,const bool &ex_port,const bool &mirror_port,const bool &soft_neg,const bool &qos_enabled,const bool &qos_configured,const UI32 &rlimit,const bool &alpa13,const UI32 &export_mode,const bool &losig,const bool &asn_frc_hwretry,const bool &cr_recov,const bool &auto_disable,const bool &fport_buffers, const UI32 &rbridgeId,const string &clear_counter_ts,const UI32 &lock_port)
        : PrismElement  (pPortCfgLocalObjectManager),
        PrismPersistableObject (PortFCCfgLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pPortCfgLocalObjectManager),
        DcmManagedObject (pPortCfgLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pPortCfgLocalObjectManager),
        m_id    (id),
        m_portStateShutdown    (portStateShutdown),
        m_fillWord    (fillWord),
        m_longDistance    (longDistance),
        m_vcLinkInit    (vcLinkInit),
        m_islRRdyMode    (islRRdyMode),
        m_status    (status),
        m_switchPort    (switchPort),
        m_cliPort    (cliPort),
        m_type    (type),
        m_slotPort    (slotPort),
        m_slot    (slot),
        m_oldDistance    (oldDistance),
        m_trunkEnable    (trunkEnable),
        m_speed    (speed),
        m_fcAccConfig    (fcAccConfig),
        m_disableEport    (disableEport),
        m_lportLockdown    (lportLockdown),
        m_privLportLockdown    (privLportLockdown),
        m_vcxltLinit    (vcxltLinit),
        m_delayFlogi    (delayFlogi),
        m_islInterop    (islInterop),
        m_pvtPortPersistentlyDisabled    (pvtPortPersistentlyDisabled),
        m_distance    (distance),
        m_bufStarvFlag    (bufStarvFlag),
        m_lport_halfduplex    (lport_halfduplex),
        m_lport_fairness    (lport_fairness),
        m_credit_sharing    (credit_sharing),
        m_pvt_port_permanently_disabled    (pvt_port_permanently_disabled),
        m_npiv_enabled    (npiv_enabled),
        m_npiv_configured    (npiv_configured),
        m_rscn_suppress    (rscn_suppress),
        m_ex_port    (ex_port),
        m_mirror_port    (mirror_port),
        m_soft_neg    (soft_neg),
        m_qos_enabled    (qos_enabled),
        m_qos_configured    (qos_configured),
        m_rlimit    (rlimit),
        m_alpa13    (alpa13),
        m_export_mode    (export_mode),
        m_losig    (losig),
        m_asn_frc_hwretry    (asn_frc_hwretry),
        m_cr_recov    (cr_recov),
        m_auto_disable    (auto_disable),
        m_fport_buffers    (fport_buffers),
        m_rbridgeId    (rbridgeId),
        m_clear_counter_ts    (clear_counter_ts),
        m_lock_port    (lock_port)
    {
    }

    PortFCCfgLocalManagedObject::~PortFCCfgLocalManagedObject ()
    {
    }

    string  PortFCCfgLocalManagedObject::getClassName()
    {
        return ("PortFCCfgLocalManagedObject");
    }

    void  PortFCCfgLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
		/* Mapped Attributes With Yang */
        addPersistableAttribute (new AttributeString(&m_id,"id"));
		setUserTagForAttribute ("id", brocade_interface_name);

        addPersistableAttribute (new AttributeEnum((UI32 * )(&m_fillWord), "fillWord"));
		setUserTagForAttribute ("fillWord", brocade_interface_fill_word);

        addPersistableAttribute (new AttributeEnum((UI32 * )(&m_longDistance),"longDistance"));
		setUserTagForAttribute ("longDistance", brocade_interface_long_distance);

        addPersistableAttribute (new AttributeEnum((UI32 * )(&m_vcLinkInit),"vcLinkInit"));
		setUserTagForAttribute ("vcLinkInit", brocade_interface_vc_link_init);

        addPersistableAttribute (new AttributeBool(&m_islRRdyMode,"islRRdyMode"));
		setUserTagForAttribute ("islRRdyMode", brocade_interface_isl_r_rdy_mode);

        addPersistableAttribute (new AttributeEnum((UI32 * )(&m_speed), "speed"));
		setUserTagForAttribute ("speed", brocade_interface_fc_speed_cfg);
		
        addPersistableAttribute (new AttributeBool(&m_portStateShutdown,"portStateShutdown"));
		setUserTagForAttribute ("portStateShutdown", brocade_interface_shutdown);

		addPersistableAttribute (new AttributeBool(&m_trunkEnable,"trunkEnable"));
		setUserTagForAttribute ("trunkEnable", brocade_interface_trunk_enable);

		addPersistableAttribute (new AttributeUI32(&m_distance,"desiredistance"));
		setUserTagForAttribute ("desiredistance", brocade_interface_desire_distance);

		addPersistableAttribute (new AttributeBool(&m_status,"status"));

        addPersistableAttribute (new AttributeEnum((UI32 * )(&m_lock_port), "lock_port"));
        setUserTagForAttribute ("lock_port", brocade_interface_config_mode);

		/* Switch-Port to CLI Lookup Information */
        addPersistableAttribute (new AttributeUI32(&m_switchPort,"switchPort"));
        addPersistableAttribute (new AttributeUI32(&m_cliPort,"cliPort"));
        addPersistableAttribute (new AttributeString(&m_type,"type"));
        addPersistableAttribute (new AttributeUI32(&m_slotPort,"slotPort"));
        addPersistableAttribute (new AttributeSI32(&m_slot,"slot"));
        addPersistableAttribute (new AttributeUI32(&m_rbridgeId, "rbridgeId"));

        addPersistableAttribute (new AttributeUI32(&m_oldDistance,"oldDistance"));
        addPersistableAttribute (new AttributeBool(&m_fcAccConfig,"fcAccConfig"));
        addPersistableAttribute (new AttributeBool(&m_disableEport,"disableEport"));
        addPersistableAttribute (new AttributeBool(&m_lportLockdown,"lportLockdown"));
        addPersistableAttribute (new AttributeBool(&m_privLportLockdown,"privLportLockdown"));
        addPersistableAttribute (new AttributeBool(&m_vcxltLinit,"vcxltLinit"));
        addPersistableAttribute (new AttributeUI32(&m_delayFlogi,"delayFlogi"));
        addPersistableAttribute (new AttributeBool(&m_islInterop,"islInterop"));
        addPersistableAttribute (new AttributeBool(&m_pvtPortPersistentlyDisabled,
									"pvtPortPersistentlyDisabled"));
        addPersistableAttribute (new AttributeBool(&m_bufStarvFlag,"bufStarvFlag"));
        addPersistableAttribute (new AttributeBool(&m_lport_halfduplex,"lport_halfduplex"));
        addPersistableAttribute (new AttributeBool(&m_lport_fairness,"lport_fairness"));
        addPersistableAttribute (new AttributeBool(&m_credit_sharing,"credit_sharing"));
        addPersistableAttribute (new AttributeBool(&m_pvt_port_permanently_disabled,
									"pvt_port_permanently_disabled"));
        addPersistableAttribute (new AttributeBool(&m_npiv_enabled,"npiv_enabled"));
        addPersistableAttribute (new AttributeBool(&m_npiv_configured,"npiv_configured"));
        addPersistableAttribute (new AttributeBool(&m_rscn_suppress,"rscn_suppress"));
        addPersistableAttribute (new AttributeBool(&m_ex_port,"ex_port"));
        addPersistableAttribute (new AttributeBool(&m_mirror_port,"mirror_port"));
        addPersistableAttribute (new AttributeBool(&m_soft_neg,"soft_neg"));
        addPersistableAttribute (new AttributeBool(&m_qos_enabled,"qos_enabled"));
        addPersistableAttribute (new AttributeBool(&m_qos_configured,"qos_configured"));
        addPersistableAttribute (new AttributeUI32(&m_rlimit,"rlimit"));
        addPersistableAttribute (new AttributeBool(&m_alpa13,"alpa13"));
        addPersistableAttribute (new AttributeUI32(&m_export_mode,"export_mode"));
        addPersistableAttribute (new AttributeBool(&m_losig,"losig"));
        addPersistableAttribute (new AttributeBool(&m_asn_frc_hwretry,"asn_frc_hwretry"));
        addPersistableAttribute (new AttributeBool(&m_cr_recov,"cr_recov"));
        addPersistableAttribute (new AttributeBool(&m_auto_disable,"auto_disable"));
        addPersistableAttribute (new AttributeBool(&m_fport_buffers,"fport_buffers"));
        addPersistableAttribute (new AttributeString(&m_clear_counter_ts,"clear_counter_ts"));
    }

    void  PortFCCfgLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_id,"id"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_speed), "speed"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_fillWord),"fillWord"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_longDistance),"longDistance"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_vcLinkInit),"vcLinkInit"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_islRRdyMode,"islRRdyMode"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_status,"status"));

        addPersistableAttributeForCreate  (new AttributeBool(&m_portStateShutdown,"portStateShutdown"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_switchPort,"switchPort"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_cliPort,"cliPort"));
        addPersistableAttributeForCreate  (new AttributeString(&m_type,"type"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_slotPort,"slotPort"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_slot,"slot"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_oldDistance,"oldDistance"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_trunkEnable,"trunkEnable"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_fcAccConfig,"fcAccConfig"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_disableEport,"disableEport"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_lportLockdown,"lportLockdown"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_privLportLockdown,"privLportLockdown"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_vcxltLinit,"vcxltLinit"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_delayFlogi,"delayFlogi"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_islInterop,"islInterop"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_pvtPortPersistentlyDisabled,"pvtPortPersistentlyDisabled"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_distance,"desiredistance"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_bufStarvFlag,"bufStarvFlag"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_lport_halfduplex,"lport_halfduplex"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_lport_fairness,"lport_fairness"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_credit_sharing,"credit_sharing"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_pvt_port_permanently_disabled,"pvt_port_permanently_disabled"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_npiv_enabled,"npiv_enabled"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_npiv_configured,"npiv_configured"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_rscn_suppress,"rscn_suppress"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_ex_port,"ex_port"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_mirror_port,"mirror_port"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_soft_neg,"soft_neg"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_qos_enabled,"qos_enabled"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_qos_configured,"qos_configured"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_rlimit,"rlimit"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_alpa13,"alpa13"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_export_mode,"export_mode"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_losig,"losig"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_asn_frc_hwretry,"asn_frc_hwretry"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_cr_recov,"cr_recov"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_auto_disable,"auto_disable"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_fport_buffers,"fport_buffers"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_rbridgeId,"rbridgeId"));
        addPersistableAttributeForCreate  (new AttributeString(&m_clear_counter_ts,"clear_counter_ts"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_lock_port), "lock_port"));	
        vector<string> keyName;
        keyName.push_back ("id"); 
        setUserDefinedKeyCombination (keyName);
    }

    void  PortFCCfgLocalManagedObject::setId(const string &id)
    {
        m_id  =  id;
    }

    string  PortFCCfgLocalManagedObject::getId() const
    {
        return (m_id);
    }

    void  PortFCCfgLocalManagedObject::setPortStateShutdown(const bool &portStateShutdown)
    {
        m_portStateShutdown  =  portStateShutdown;
    }

    bool  PortFCCfgLocalManagedObject::getPortStateShutdown() const
    {
        return (m_portStateShutdown);
    }

    void  PortFCCfgLocalManagedObject::setFillWord(const UI32 &fillWord)
    {
        m_fillWord  =  fillWord;
    }

    UI32  PortFCCfgLocalManagedObject::getFillWord() const
    {
        return (m_fillWord);
    }

    void  PortFCCfgLocalManagedObject::setLongDistance(const UI32 &longDistance)
    {
        m_longDistance  =  longDistance;
    }

    UI32  PortFCCfgLocalManagedObject::getLongDistance() const
    {
        return (m_longDistance);
    }

    void  PortFCCfgLocalManagedObject::setVcLinkInit(const UI32 &vcLinkInit)
    {
        m_vcLinkInit  =  vcLinkInit;
    }

    UI32  PortFCCfgLocalManagedObject::getVcLinkInit() const
    {
        return (m_vcLinkInit);
    }

    void  PortFCCfgLocalManagedObject::setIslRRdyMode(const bool &islRRdyMode)
    {
        m_islRRdyMode  =  islRRdyMode;
    }

    bool  PortFCCfgLocalManagedObject::getIslRRdyMode() const
    {
        return (m_islRRdyMode);
    }

    void  PortFCCfgLocalManagedObject::setStatus(const bool &status)
    {
        m_status  =  status; 
    }
 
    bool  PortFCCfgLocalManagedObject::getStatus() const
    {
        return (m_status);
    }

    void  PortFCCfgLocalManagedObject::setSwitchPort(const UI32 &switchPort)
    {
        m_switchPort  =  switchPort;
    }

    UI32  PortFCCfgLocalManagedObject::getSwitchPort() const
    {
        return (m_switchPort);
    }

    void  PortFCCfgLocalManagedObject::setCliPort(const UI32 &cliPort)
    {
        m_cliPort  =  cliPort;
    }

    UI32  PortFCCfgLocalManagedObject::getCliPort() const
    {
        return (m_cliPort);
    }

    void  PortFCCfgLocalManagedObject::setType(const string &type)
    {
        m_type  =  type;
    }

    string  PortFCCfgLocalManagedObject::getType() const
    {
        return (m_type);
    }

    void  PortFCCfgLocalManagedObject::setSlotPort(const UI32 &slotPort)
    {
        m_slotPort  =  slotPort;
    }

    UI32  PortFCCfgLocalManagedObject::getSlotPort() const
    {
        return (m_slotPort);
    }

    void  PortFCCfgLocalManagedObject::setSlot(const SI32 &slot)
    {
        m_slot  =  slot;
    }

    SI32  PortFCCfgLocalManagedObject::getSlot() const
    {
        return (m_slot);
    }

    void  PortFCCfgLocalManagedObject::setOldDistance(const UI32 &oldDistance)
    {
        m_oldDistance  =  oldDistance;
    }

    UI32  PortFCCfgLocalManagedObject::getOldDistance() const
    {
        return (m_oldDistance);
    }

    void  PortFCCfgLocalManagedObject::setTrunkEnable(const bool &trunkEnable)
    {
        m_trunkEnable  =  trunkEnable;
    }

    bool  PortFCCfgLocalManagedObject::getTrunkEnable() const
    {
        return (m_trunkEnable);
    }

    void  PortFCCfgLocalManagedObject::setSpeed(const UI32 &speed)
    {
        m_speed  =  speed;
    }

    UI32  PortFCCfgLocalManagedObject::getSpeed() const
    {
        return (m_speed);
    }

    void  PortFCCfgLocalManagedObject::setFcAccConfig(const bool &fcAccConfig)
    {
        m_fcAccConfig  =  fcAccConfig;
    }

    bool  PortFCCfgLocalManagedObject::getFcAccConfig() const
    {
        return (m_fcAccConfig);
    }

    void  PortFCCfgLocalManagedObject::setDisableEport(const bool &disableEport)
    {
        m_disableEport  =  disableEport;
    }

    bool  PortFCCfgLocalManagedObject::getDisableEport() const
    {
        return (m_disableEport);
    }

    void  PortFCCfgLocalManagedObject::setLportLockdown(const bool &lportLockdown)
    {
        m_lportLockdown  =  lportLockdown;
    }

    bool  PortFCCfgLocalManagedObject::getLportLockdown() const
    {
        return (m_lportLockdown);
    }

    void  PortFCCfgLocalManagedObject::setPrivLportLockdown(const bool &privLportLockdown)
    {
        m_privLportLockdown  =  privLportLockdown;
    }

    bool  PortFCCfgLocalManagedObject::getPrivLportLockdown() const
    {
        return (m_privLportLockdown);
    }

    void  PortFCCfgLocalManagedObject::setVcxltLinit(const bool &vcxltLinit)
    {
        m_vcxltLinit  =  vcxltLinit;
    }

    bool  PortFCCfgLocalManagedObject::getVcxltLinit() const
    {
        return (m_vcxltLinit);
    }

    void  PortFCCfgLocalManagedObject::setDelayFlogi(const UI32 &delayFlogi)
    {
        m_delayFlogi  =  delayFlogi;
    }

    UI32  PortFCCfgLocalManagedObject::getDelayFlogi() const
    {
        return (m_delayFlogi);
    }

    void  PortFCCfgLocalManagedObject::setIslInterop(const bool &islInterop)
    {
        m_islInterop  =  islInterop;
    }

    bool  PortFCCfgLocalManagedObject::getIslInterop() const
    {
        return (m_islInterop);
    }

    void  PortFCCfgLocalManagedObject::setPvtPortPersistentlyDisabled(const bool &pvtPortPersistentlyDisabled)
    {
        m_pvtPortPersistentlyDisabled  =  pvtPortPersistentlyDisabled;
    }

    bool  PortFCCfgLocalManagedObject::getPvtPortPersistentlyDisabled() const
    {
        return (m_pvtPortPersistentlyDisabled);
    }

    void  PortFCCfgLocalManagedObject::setDistance(const UI32 &distance)
    {
        m_distance  =  distance;
    }

    UI32  PortFCCfgLocalManagedObject::getDistance() const
    {
        return (m_distance);
    }

    void  PortFCCfgLocalManagedObject::setBufStarvFlag(const bool &bufStarvFlag)
    {
        m_bufStarvFlag  =  bufStarvFlag;
    }

    bool  PortFCCfgLocalManagedObject::getBufStarvFlag() const
    {
        return (m_bufStarvFlag);
    }

    void  PortFCCfgLocalManagedObject::setLport_halfduplex(const bool &lport_halfduplex)
    {
        m_lport_halfduplex  =  lport_halfduplex;
    }

    bool  PortFCCfgLocalManagedObject::getLport_halfduplex() const
    {
        return (m_lport_halfduplex);
    }

    void  PortFCCfgLocalManagedObject::setLport_fairness(const bool &lport_fairness)
    {
        m_lport_fairness  =  lport_fairness;
    }

    bool  PortFCCfgLocalManagedObject::getLport_fairness() const
    {
        return (m_lport_fairness);
    }

    void  PortFCCfgLocalManagedObject::setCredit_sharing(const bool &credit_sharing)
    {
        m_credit_sharing  =  credit_sharing;
    }

    bool  PortFCCfgLocalManagedObject::getCredit_sharing() const
    {
        return (m_credit_sharing);
    }

    void  PortFCCfgLocalManagedObject::setPvt_port_permanently_disabled(const bool &pvt_port_permanently_disabled)
    {
        m_pvt_port_permanently_disabled  =  pvt_port_permanently_disabled;
    }

    bool  PortFCCfgLocalManagedObject::getPvt_port_permanently_disabled() const
    {
        return (m_pvt_port_permanently_disabled);
    }

    void  PortFCCfgLocalManagedObject::setNpiv_enabled(const bool &npiv_enabled)
    {
        m_npiv_enabled  =  npiv_enabled;
    }

    bool  PortFCCfgLocalManagedObject::getNpiv_enabled() const
    {
        return (m_npiv_enabled);
    }

    void  PortFCCfgLocalManagedObject::setNpiv_configured(const bool &npiv_configured)
    {
        m_npiv_configured  =  npiv_configured;
    }

    bool  PortFCCfgLocalManagedObject::getNpiv_configured() const
    {
        return (m_npiv_configured);
    }

    void  PortFCCfgLocalManagedObject::setRscn_suppress(const bool &rscn_suppress)
    {
        m_rscn_suppress  =  rscn_suppress;
    }

    bool  PortFCCfgLocalManagedObject::getRscn_suppress() const
    {
        return (m_rscn_suppress);
    }

    void  PortFCCfgLocalManagedObject::setEx_port(const bool &ex_port)
    {
        m_ex_port  =  ex_port;
    }

    bool  PortFCCfgLocalManagedObject::getEx_port() const
    {
        return (m_ex_port);
    }

    void  PortFCCfgLocalManagedObject::setMirror_port(const bool &mirror_port)
    {
        m_mirror_port  =  mirror_port;
    }

    bool  PortFCCfgLocalManagedObject::getMirror_port() const
    {
        return (m_mirror_port);
    }

    void  PortFCCfgLocalManagedObject::setSoft_neg(const bool &soft_neg)
    {
        m_soft_neg  =  soft_neg;
    }

    bool  PortFCCfgLocalManagedObject::getSoft_neg() const
    {
        return (m_soft_neg);
    }

    void  PortFCCfgLocalManagedObject::setQos_enabled(const bool &qos_enabled)
    {
        m_qos_enabled  =  qos_enabled;
    }

    bool  PortFCCfgLocalManagedObject::getQos_enabled() const
    {
        return (m_qos_enabled);
    }

    void  PortFCCfgLocalManagedObject::setQos_configured(const bool &qos_configured)
    {
        m_qos_configured  =  qos_configured;
    }

    bool  PortFCCfgLocalManagedObject::getQos_configured() const
    {
        return (m_qos_configured);
    }

    void  PortFCCfgLocalManagedObject::setRlimit(const UI32 &rlimit)
    {
        m_rlimit  =  rlimit;
    }

    UI32  PortFCCfgLocalManagedObject::getRlimit() const
    {
        return (m_rlimit);
    }

    void  PortFCCfgLocalManagedObject::setAlpa13(const bool &alpa13)
    {
        m_alpa13  =  alpa13;
    }

    bool  PortFCCfgLocalManagedObject::getAlpa13() const
    {
        return (m_alpa13);
    }

    void  PortFCCfgLocalManagedObject::setExport_mode(const UI32 &export_mode)
    {
        m_export_mode  =  export_mode;
    }

    UI32  PortFCCfgLocalManagedObject::getExport_mode() const
    {
        return (m_export_mode);
    }

    void  PortFCCfgLocalManagedObject::setLosig(const bool &losig)
    {
        m_losig  =  losig;
    }

    bool  PortFCCfgLocalManagedObject::getLosig() const
    {
        return (m_losig);
    }

    void  PortFCCfgLocalManagedObject::setAsn_frc_hwretry(const bool &asn_frc_hwretry)
    {
        m_asn_frc_hwretry  =  asn_frc_hwretry;
    }

    bool  PortFCCfgLocalManagedObject::getAsn_frc_hwretry() const
    {
        return (m_asn_frc_hwretry);
    }

    void  PortFCCfgLocalManagedObject::setCr_recov(const bool &cr_recov)
    {
        m_cr_recov  =  cr_recov;
    }

    bool  PortFCCfgLocalManagedObject::getCr_recov() const
    {
        return (m_cr_recov);
    }

    void  PortFCCfgLocalManagedObject::setAuto_disable(const bool &auto_disable)
    {
        m_auto_disable  =  auto_disable;
    }

    bool  PortFCCfgLocalManagedObject::getAuto_disable() const
    {
        return (m_auto_disable);
    }

    void  PortFCCfgLocalManagedObject::setFport_buffers(const bool &fport_buffers)
    {
        m_fport_buffers  =  fport_buffers;
    }

    bool  PortFCCfgLocalManagedObject::getFport_buffers() const
    {
        return (m_fport_buffers);
    }

    void  PortFCCfgLocalManagedObject::setRbridgeId(const UI32 &rbridgeId)
    {
        m_rbridgeId  =  rbridgeId;
    }

    UI32  PortFCCfgLocalManagedObject::getRbridgeId() const
    {
       return (m_rbridgeId);
    }

    void  PortFCCfgLocalManagedObject::setClear_counter_ts(const string &clear_counter_ts)
    {
        m_clear_counter_ts  =  clear_counter_ts;
    }

    string  PortFCCfgLocalManagedObject::getClear_counter_ts() const
    {
        return (m_clear_counter_ts);
    }

    void  PortFCCfgLocalManagedObject::setLock_port(const UI32 &lock_port)
    {
        m_lock_port  =  lock_port;
    }

    UI32  PortFCCfgLocalManagedObject::getLock_port() const
    {
        return (m_lock_port);
    }
}

