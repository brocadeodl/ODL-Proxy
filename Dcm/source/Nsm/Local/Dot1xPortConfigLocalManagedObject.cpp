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
 *   Author : aantony                                                     *
 **************************************************************************/

#include "Nsm/Local/Dot1xPortConfigLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"

#include "vcs.h"

namespace DcmNs
{

    Dot1xPortConfigLocalManagedObject::Dot1xPortConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (Dot1xPortConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		
		m_cmdcode = 0;
        m_enabled = false;
        m_enable_auth = false;
        m_max_reauth_attempt = 0;
        m_opcode = 0;
        m_port_ctrl_type = 0;
        m_proto_version = 0;
        m_quiet_interval = 0;
        m_reauth_timeout_interval = 0;
        m_server_timeout_interval = 0;
        m_supp_timeout_interval = 0;
        m_tx_timeout_interval = 0;
    }

    Dot1xPortConfigLocalManagedObject::Dot1xPortConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const string & ifName,const bool &enabled,const UI32 &port_ctrl_type,const UI32 &proto_version,const UI32 &quiet_interval,const UI32 &max_reauth_attempt,const bool &enable_auth,const UI32 &reauth_timeout_interval,const UI32 &server_timeout_interval,const UI32 &supp_timeout_interval,const UI32 &tx_timeout_interval)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (Dot1xPortConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_enabled    (enabled),
        m_port_ctrl_type    (port_ctrl_type),
        m_proto_version    (proto_version),
        m_quiet_interval    (quiet_interval),
        m_max_reauth_attempt    (max_reauth_attempt),
        m_enable_auth    (enable_auth),
        m_reauth_timeout_interval    (reauth_timeout_interval),
        m_server_timeout_interval    (server_timeout_interval),
        m_supp_timeout_interval    (supp_timeout_interval),
        m_tx_timeout_interval    (tx_timeout_interval)
    {
		m_cmdcode = 0;
		m_opcode = 0;
    }

    Dot1xPortConfigLocalManagedObject::~Dot1xPortConfigLocalManagedObject ()
    {
    }

    string  Dot1xPortConfigLocalManagedObject::getClassName()
    {
        return ("Dot1xPortConfigLocalManagedObject");
    }

    void  Dot1xPortConfigLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString((string *)&m_ifName,"interface", brocade_interface_interface));
        addPersistableAttribute (new AttributeBool((bool *)&m_enabled,"enabled", dot1x_authentication));
        addPersistableAttribute (new AttributeEnum((UI32 * )&m_port_ctrl_type,"port_ctrl_type" , dot1x_port_control));
        addPersistableAttribute (new AttributeUI32((UI32 * )&m_proto_version,"proto_version", dot1x_protocol_version));
        addPersistableAttribute (new AttributeUI32((UI32 * )&m_quiet_interval,"quiet_interval", dot1x_quiet_period));
        addPersistableAttribute (new AttributeUI32((UI32 * )&m_max_reauth_attempt,"max_reauth_attempt",dot1x_reauthMax ));
        addPersistableAttribute (new AttributeBool((bool *)&m_enable_auth,"enable_auth", dot1x_reauthentication));
        addPersistableAttribute (new AttributeUI32((UI32 * )&m_reauth_timeout_interval,"reauth_timeout_interval", dot1x_re_authperiod));
        addPersistableAttribute (new AttributeUI32((UI32 * )&m_server_timeout_interval,"server_timeout_interval", dot1x_server_timeout));
        addPersistableAttribute (new AttributeUI32((UI32 * )&m_supp_timeout_interval,"supp_timeout_interval", dot1x_supp_timeout));
        addPersistableAttribute (new AttributeUI32((UI32 * )&m_tx_timeout_interval,"tx_timeout_interval", dot1x_tx_period));
    }

    void  Dot1xPortConfigLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_ifName,"interface"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_enabled,"enabled"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_proto_version,"proto_version"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_quiet_interval,"quiet_interval"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_max_reauth_attempt,"max_reauth_attempt"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_enable_auth,"enable_auth"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_reauth_timeout_interval,"reauth_timeout_interval"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_server_timeout_interval,"server_timeout_interval"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_supp_timeout_interval,"supp_timeout_interval"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_tx_timeout_interval,"tx_timeout_interval"));
        addPersistableAttributeForCreate  (new AttributeEnum(&m_port_ctrl_type,"port_ctrl_type"));
    }

	void  Dot1xPortConfigLocalManagedObject::setIfName(const string  &ifName)
	{
	  m_ifName  =  ifName ;
	}

	string  Dot1xPortConfigLocalManagedObject::getIfName() const
	{
	  return (m_ifName);
	}

    void  Dot1xPortConfigLocalManagedObject::setEnabled(const bool &enabled)
    {
        m_enabled  =  enabled;
    }

    bool  Dot1xPortConfigLocalManagedObject::getEnabled() const
    {
        return (m_enabled);
    }

    void  Dot1xPortConfigLocalManagedObject::setPort_ctrl_type(const UI32 &port_ctrl_type)
    {
        m_port_ctrl_type  =  port_ctrl_type;
    }

    UI32  Dot1xPortConfigLocalManagedObject::getPort_ctrl_type() const
    {
        return (m_port_ctrl_type);
    }

    void  Dot1xPortConfigLocalManagedObject::setProto_version(const UI32 &proto_version)
    {
        m_proto_version  =  proto_version;
    }

    UI32  Dot1xPortConfigLocalManagedObject::getProto_version() const
    {
        return (m_proto_version);
    }

    void  Dot1xPortConfigLocalManagedObject::setQuiet_interval(const UI32 &quiet_interval)
    {
        m_quiet_interval  =  quiet_interval;
    }

    UI32  Dot1xPortConfigLocalManagedObject::getQuiet_interval() const
    {
        return (m_quiet_interval);
    }

    void  Dot1xPortConfigLocalManagedObject::setMax_reauth_attempt(const UI32 &max_reauth_attempt)
    {
        m_max_reauth_attempt  =  max_reauth_attempt;
    }

    UI32  Dot1xPortConfigLocalManagedObject::getMax_reauth_attempt() const
    {
        return (m_max_reauth_attempt);
    }

    void  Dot1xPortConfigLocalManagedObject::setEnable_auth(const bool &enable_auth)
    {
        m_enable_auth  =  enable_auth;
    }

    bool  Dot1xPortConfigLocalManagedObject::getEnable_auth() const
    {
        return (m_enable_auth);
    }

    void  Dot1xPortConfigLocalManagedObject::setReauth_timeout_interval(const UI32 &reauth_timeout_interval)
    {
        m_reauth_timeout_interval  =  reauth_timeout_interval;
    }

    UI32  Dot1xPortConfigLocalManagedObject::getReauth_timeout_interval() const
    {
        return (m_reauth_timeout_interval);
    }

    void  Dot1xPortConfigLocalManagedObject::setServer_timeout_interval(const UI32 &server_timeout_interval)
    {
        m_server_timeout_interval  =  server_timeout_interval;
    }

    UI32  Dot1xPortConfigLocalManagedObject::getServer_timeout_interval() const
    {
        return (m_server_timeout_interval);
    }

    void  Dot1xPortConfigLocalManagedObject::setSupp_timeout_interval(const UI32 &supp_timeout_interval)
    {
        m_supp_timeout_interval  =  supp_timeout_interval;
    }

    UI32  Dot1xPortConfigLocalManagedObject::getSupp_timeout_interval() const
    {
        return (m_supp_timeout_interval);
    }

    void  Dot1xPortConfigLocalManagedObject::setTx_timeout_interval(const UI32 &tx_timeout_interval)
    {
        m_tx_timeout_interval  =  tx_timeout_interval;
    }

    UI32  Dot1xPortConfigLocalManagedObject::getTx_timeout_interval() const
    {
        return (m_tx_timeout_interval);
    }
#if 1
    void  Dot1xPortConfigLocalManagedObject::setOpcode(const UI32 &cmdcode)
    {
        m_opcode  =  cmdcode;
    }

    UI32  Dot1xPortConfigLocalManagedObject::getOpcode() const
    {
        return (m_opcode);
    }

    void  Dot1xPortConfigLocalManagedObject::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  Dot1xPortConfigLocalManagedObject::getCmdcode() const
    {
        return (m_cmdcode);
    }
#endif

}
