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
 *   Author : kghanta                                                     *
 **************************************************************************/
 #include "Framework/Attributes/AttributeEnum.h"
#include "AAA/Global/RuleManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/RoleManagedObject.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "brocade-aaa.h"


namespace DcmNs
{

    RuleManagedObject::RuleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
        : PrismElement  (pAAAGlobalObjectManager),
        PrismPersistableObject (RuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAAAGlobalObjectManager),
        DcmManagedObject (pAAAGlobalObjectManager)
    {
    }

    RuleManagedObject::RuleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,const UI32 &index, const UI32 &cmdTypeChoice, const SI32 &command, const string &interfaceTELeaf, const string &interfaceGELeaf, const string &interfacePCLeaf, const string &interfaceVLANLeaf, const string &interfaceFCOELeaf, const ObjectId &role, const RBACOperation &operation, const RBACAction &action, const string &group, const bool &isPredefined, const bool &interfaceInt, const bool &interfaceTe, const bool &interfaceGe, const bool &interfacePc, const bool &interfaceVlan, const bool &interfaceFcoe, const bool &interfaceMgmt, const bool &copyCp, const bool &copyRunning, const bool &copySupport, const bool &clearCl, const bool &clearLogging, const bool &clearSupport, const bool &protocolPr, const bool &protocolSpanning, const bool &protocolLldp, const UI32 &cmdRuleCounter, const UI32 &dataRuleCounter)
        : PrismElement  (pAAAGlobalObjectManager),
        PrismPersistableObject (RuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAAAGlobalObjectManager),
        DcmManagedObject (pAAAGlobalObjectManager),
        m_index    (index),
        m_cmdTypeChoice    (cmdTypeChoice),
        m_command    (command),
        m_interfaceTELeaf    (interfaceTELeaf),
		m_interfaceGELeaf	(interfaceGELeaf),
		m_interfacePCLeaf	(interfacePCLeaf),
		m_interfaceVLANLeaf (interfaceVLANLeaf),
        m_interfaceFCOELeaf    (interfaceFCOELeaf),
        m_role    (role),
        m_operation    (operation),
        m_action    (action),
        m_group    (group),
		m_isPredefined (isPredefined),
		m_interfaceInt (interfaceInt),
		m_interfaceTe (interfaceTe),
		m_interfaceGe (interfaceGe),
		m_interfacePc (interfacePc),
		m_interfaceVlan (interfaceVlan),
		m_interfaceFcoe (interfaceFcoe),
		m_interfaceMgmt (interfaceMgmt),
		m_copyCp (copyCp),
		m_copyRunning (copyRunning),
		m_copySupport (copySupport),
		m_clearCl (clearCl),
		m_clearLogging (clearLogging),
		m_clearSupport (clearSupport),
		m_protocolPr (protocolPr),
		m_protocolSpanning (protocolSpanning),
		m_protocolLldp (protocolLldp),
        m_cmdRuleCounter (cmdRuleCounter),
        m_dataRuleCounter (dataRuleCounter)
    {
    }

    RuleManagedObject::~RuleManagedObject ()
    {
    }

    string  RuleManagedObject::getClassName()
    {
        return ("RuleManagedObject");
    }

    void  RuleManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_index,"index", brcd_aaa_index));
		addPersistableAttribute (new AttributeUI32(&m_cmdTypeChoice,"cmdTypeChoice"));
        addPersistableAttribute  (new AttributeEnum((UI32*)&m_command, INVALID_ENUM_COMMAND, "cmdlist", brcd_aaa_enumList));
        addPersistableAttribute  (new AttributeString(&m_interfaceTELeaf, "interfaceTELeaf", brcd_aaa_tengigabitethernet_leaf));
		addPersistableAttribute  (new AttributeString(&m_interfaceGELeaf, "interfaceGELeaf", brcd_aaa_gigabitethernet_leaf));
        addPersistableAttribute  (new AttributeString(&m_interfaceFCOELeaf, "interfaceFCOELeaf", brcd_aaa_fcoe_leaf));
		addPersistableAttribute  (new AttributeString(&m_interfacePCLeaf, "interfacePCLeaf", brcd_aaa_port_channel_leaf));
		addPersistableAttribute  (new AttributeString(&m_interfaceVLANLeaf, "interfaceVLANLeaf", brcd_aaa_vlan_leaf));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_role,"role", RoleManagedObject::getClassName (), false, brcd_aaa_role));
		addPersistableAttribute (new AttributeEnum((UI32*)&m_operation,"operation", brcd_aaa_operation));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_action,"action", brcd_aaa_action));
    	addPersistableAttribute (new AttributeBool(&m_isPredefined, "isFactorydefined"));
    	addPersistableAttribute (new AttributeBool(&m_interfaceInt, "interfaceInt", brcd_aaa_interface_cont));
    	addPersistableAttribute (new AttributeBool(&m_interfaceTe, "interfaceTe", brcd_aaa_tengigabitethernet_cont));
		addPersistableAttribute (new AttributeBool(&m_interfaceGe, "interfaceGe", brcd_aaa_gigabitethernet_cont));
    	addPersistableAttribute (new AttributeBool(&m_interfaceFcoe, "interfaceFcoe", brcd_aaa_fcoe_cont));
		addPersistableAttribute (new AttributeBool(&m_interfacePc, "interfacePc", brcd_aaa_port_channel_cont));
		addPersistableAttribute (new AttributeBool(&m_interfaceVlan, "interfaceVlan", brcd_aaa_vlan_cont));
    	addPersistableAttribute (new AttributeBool(&m_interfaceMgmt, "interfaceMgmt", brcd_aaa_management_cont));
    	addPersistableAttribute (new AttributeBool(&m_copyCp, "copyCp", brcd_aaa_copy_cont));
    	addPersistableAttribute (new AttributeBool(&m_copyRunning, "copyRunning", brcd_aaa_running_config_cont));
    	addPersistableAttribute (new AttributeBool(&m_copySupport, "copySupport", brcd_aaa_support_cp));
    	addPersistableAttribute (new AttributeBool(&m_clearCl, "clearCl", brcd_aaa_clear_cont));
    	addPersistableAttribute (new AttributeBool(&m_clearLogging, "clearLogging", brcd_aaa_logging_cont));
    	addPersistableAttribute (new AttributeBool(&m_clearSupport, "clearSupport", brcd_aaa_support_cl));
    	addPersistableAttribute (new AttributeBool(&m_protocolPr, "protocolPr", brcd_aaa_protocol_cont));
    	addPersistableAttribute (new AttributeBool(&m_protocolSpanning, "protocolSpanning", brcd_aaa_spanning_tree_cont));
    	addPersistableAttribute (new AttributeBool(&m_protocolLldp, "protocolLldp", brcd_aaa_lldp_cont));
        addPersistableAttribute (new AttributeUI32(&m_cmdRuleCounter, "cmdRuleCounter"));
        addPersistableAttribute (new AttributeUI32(&m_dataRuleCounter, "dataRuleCounter"));
	 }

	void RuleManagedObject::setupKeys()
	{
		vector<string> key;
		key.push_back ("index");
		setUserDefinedKeyCombination (key);
	}

    void  RuleManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_index,"index", brcd_aaa_index));
		addPersistableAttributeForCreate (new AttributeUI32(&m_cmdTypeChoice,"cmdTypeChoice"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_command, INVALID_ENUM_COMMAND, "cmdlist", brcd_aaa_enumList));
        addPersistableAttributeForCreate  (new AttributeString(&m_interfaceTELeaf, "interfaceTELeaf", brcd_aaa_tengigabitethernet_leaf));
		addPersistableAttributeForCreate  (new AttributeString(&m_interfaceGELeaf, "interfaceGELeaf", brcd_aaa_gigabitethernet_leaf));
        addPersistableAttributeForCreate  (new AttributeString(&m_interfaceFCOELeaf, "interfaceFCOELeaf", brcd_aaa_fcoe_leaf));
		addPersistableAttributeForCreate  (new AttributeString(&m_interfacePCLeaf, "interfacePCLeaf", brcd_aaa_port_channel_leaf));
		addPersistableAttributeForCreate  (new AttributeString(&m_interfaceVLANLeaf, "interfaceVLANLeaf", brcd_aaa_vlan_leaf));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_role,"role", RoleManagedObject::getClassName (), false, brcd_aaa_role));
	 	addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_operation,"operation", brcd_aaa_operation));
		addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_action,"action", brcd_aaa_action));
    	addPersistableAttributeForCreate (new AttributeBool(&m_isPredefined, "isFactorydefined"));
    	addPersistableAttributeForCreate (new AttributeBool(&m_interfaceInt, "interfaceInt", brcd_aaa_interface_cont));
    	addPersistableAttributeForCreate (new AttributeBool(&m_interfaceTe, "interfaceTe", brcd_aaa_tengigabitethernet_cont));
		addPersistableAttributeForCreate (new AttributeBool(&m_interfaceGe, "interfaceGe", brcd_aaa_gigabitethernet_cont));
    	addPersistableAttributeForCreate (new AttributeBool(&m_interfaceFcoe, "interfaceFcoe", brcd_aaa_fcoe_cont));
		addPersistableAttributeForCreate (new AttributeBool(&m_interfacePc, "interfacePc", brcd_aaa_port_channel_cont));
		addPersistableAttributeForCreate (new AttributeBool(&m_interfaceVlan, "interfaceVlan", brcd_aaa_vlan_cont));
    	addPersistableAttributeForCreate (new AttributeBool(&m_interfaceMgmt, "interfaceMgmt", brcd_aaa_management_cont));
    	addPersistableAttributeForCreate (new AttributeBool(&m_copyCp, "copyCp", brcd_aaa_copy_cont));
    	addPersistableAttributeForCreate (new AttributeBool(&m_copyRunning, "copyRunning", brcd_aaa_running_config_cont));
    	addPersistableAttributeForCreate (new AttributeBool(&m_copySupport, "copySupport", brcd_aaa_support_cp));
    	addPersistableAttributeForCreate (new AttributeBool(&m_clearCl, "clearCl", brcd_aaa_clear_cont));
    	addPersistableAttributeForCreate (new AttributeBool(&m_clearLogging, "clearLogging", brcd_aaa_logging_cont));
    	addPersistableAttributeForCreate (new AttributeBool(&m_clearSupport, "clearSupport", brcd_aaa_support_cl));
    	addPersistableAttributeForCreate (new AttributeBool(&m_protocolPr, "protocolPr", brcd_aaa_protocol_cont));
    	addPersistableAttributeForCreate (new AttributeBool(&m_protocolSpanning, "protocolSpanning", brcd_aaa_spanning_tree_cont));
    	addPersistableAttributeForCreate (new AttributeBool(&m_protocolLldp, "protocolLldp", brcd_aaa_lldp_cont));
        addPersistableAttributeForCreate (new AttributeUI32(&m_cmdRuleCounter, "cmdRuleCounter"));
        addPersistableAttributeForCreate (new AttributeUI32(&m_dataRuleCounter, "dataRuleCounter"));
	}

	UI32 RuleManagedObject::getCase (const UI32 &wyserCmdXmlTag)
	{
    trace (TRACE_LEVEL_DEBUG, "RuleManagedObject::getCase: Entering ..."); 
    
    	if (CMD_TYPE_ENUM_CMDS == m_cmdTypeChoice)
    	{
        	return brcd_aaa_container_cmds;
    	}
    	else if (CMD_TYPE_INTERFACE == m_cmdTypeChoice)
    	{
        	return brcd_aaa_interface_a;
    	} 
		else if (CMD_TYPE_INTERFACE_FCOE == m_cmdTypeChoice)
    	{
        	return brcd_aaa_interface_b;
    	}
		else if (CMD_TYPE_INTERFACE_TE == m_cmdTypeChoice)
    	{
        	return brcd_aaa_interface_c;
    	}
		else if (CMD_TYPE_INTERFACE_FCOE_LEAF == m_cmdTypeChoice)
    	{
        	return brcd_aaa_interface_d;
    	}
		else if (CMD_TYPE_INTERFACE_TE_LEAF == m_cmdTypeChoice)
    	{
        	return brcd_aaa_interface_e;
		}
    	else if (CMD_TYPE_INTERFACE_MGMT == m_cmdTypeChoice)
    	{
        	return brcd_aaa_interface_f;
    	} 
		else if (CMD_TYPE_INTERFACE_GE == m_cmdTypeChoice)
		{
			return brcd_aaa_interface_g;
		}
		else if (CMD_TYPE_INTERFACE_GE_LEAF == m_cmdTypeChoice)
		{
			return brcd_aaa_interface_h;
		} 
		else if (CMD_TYPE_INTERFACE_PC == m_cmdTypeChoice)
		{
			return brcd_aaa_interface_i;
		}
		else if (CMD_TYPE_INTERFACE_PC_LEAF == m_cmdTypeChoice)
		{
			return brcd_aaa_interface_j;
		}
		else if (CMD_TYPE_INTERFACE_VLAN == m_cmdTypeChoice)
		{
			return brcd_aaa_interface_k;
		}
		else if (CMD_TYPE_INTERFACE_VLAN_LEAF == m_cmdTypeChoice)
		{
			return brcd_aaa_interface_l;
		}
		else if (CMD_TYPE_COPY == m_cmdTypeChoice)
    	{
        	return brcd_aaa_copy_a;
    	}
   		else if (CMD_TYPE_COPY_RUNNING == m_cmdTypeChoice)
    	{
        	return brcd_aaa_copy_b;
    	} 
		else if (CMD_TYPE_COPY_SUPPORT == m_cmdTypeChoice)
    	{
        	return brcd_aaa_copy_c;
    	}
		else if (CMD_TYPE_CLEAR == m_cmdTypeChoice)
    	{
        	return brcd_aaa_clear_a;
    	}
   		else if (CMD_TYPE_CLEAR_LOGGING == m_cmdTypeChoice)
    	{
        	return brcd_aaa_clear_b;
    	} 
		else if (CMD_TYPE_CLEAR_SUPPORT == m_cmdTypeChoice)
    	{
        	return brcd_aaa_clear_c;
    	}
		else if (CMD_TYPE_PROTOCOL == m_cmdTypeChoice)
    	{
        	return brcd_aaa_protocol_a;
    	}
   		else if (CMD_TYPE_PROTOCOL_SPANNING_TREE == m_cmdTypeChoice)
    	{
        	return brcd_aaa_protocol_b;
    	} 
		else if (CMD_TYPE_PROTOCOL_LLDP == m_cmdTypeChoice)
    	{
        	return brcd_aaa_protocol_c;
    	}
    
    	return 0; 
	}
    void  RuleManagedObject::setIndex(const UI32 &index)
    {
        m_index  =  index;
    }

    UI32  RuleManagedObject::getIndex() const
    {
        return (m_index);
    }

	UI32 RuleManagedObject::getCmdTypeChoice () const
	{
    	return (m_cmdTypeChoice);
	}

	void RuleManagedObject::setCmdTypeChoice (const UI32 &cmdTypeChoice)
	{
    	m_cmdTypeChoice = cmdTypeChoice;
	}

    void  RuleManagedObject::setCommand(const SI32 &command)
    {
        m_command  =  command;
    }

    SI32  RuleManagedObject::getCommand() const
    {
        return (m_command);
    }

    void  RuleManagedObject::setInterfaceTELeaf(const string &interface)
    {
        m_interfaceTELeaf  =  interface;
    }

    string  RuleManagedObject::getInterfaceTELeaf() const
    {
        return (m_interfaceTELeaf);
    }
	
	void  RuleManagedObject::setInterfaceGELeaf(const string &interface)
	{
		m_interfaceGELeaf  =  interface;
	}

	string  RuleManagedObject::getInterfaceGELeaf() const
	{
		return (m_interfaceGELeaf);
	}
	
    void  RuleManagedObject::setInterfaceFCOELeaf(const string &interface)
    {
        m_interfaceFCOELeaf  =  interface;
    }

    string  RuleManagedObject::getInterfaceFCOELeaf() const
    {
        return (m_interfaceFCOELeaf);
    }

	void  RuleManagedObject::setInterfacePCLeaf(const string &interface)
	{
		m_interfacePCLeaf = interface;
	}

	string  RuleManagedObject::getInterfacePCLeaf() const
	{
		return (m_interfacePCLeaf);
	}

	void  RuleManagedObject::setInterfaceVLANLeaf(const string &interface)
	{
		m_interfaceVLANLeaf = interface;
	}

	string  RuleManagedObject::getInterfaceVLANLeaf() const
	{
		return (m_interfaceVLANLeaf);
	}

    void  RuleManagedObject::setRole(const ObjectId &role)
    {
        m_role  =  role;
    }

    ObjectId  RuleManagedObject::getRole() const
    {
        return (m_role);
    }

    void  RuleManagedObject::setOperation(const RBACOperation &operation)
    {
        m_operation  =  operation;
    }

    RBACOperation  RuleManagedObject::getOperation() const
    {
        return (m_operation);
    }

    void  RuleManagedObject::setAction(const RBACAction &action)
    {
        m_action  =  action;
    }

    RBACAction  RuleManagedObject::getAction() const
    {
        return (m_action);
    }

    void  RuleManagedObject::setGroup(const string &group)
    {
        m_group  =  group;
    }

    string  RuleManagedObject::getGroup() const
    {
        return (m_group);
    }

	void RuleManagedObject::setIsPredefined(const bool &isPredefined) {
		m_isPredefined = isPredefined;
	}

 	bool RuleManagedObject::getIsPredefined() const{
		return (m_isPredefined);
	}

	void RuleManagedObject::setInterfaceInt(const bool &interfaceInt) 
	{
		m_interfaceInt = interfaceInt;
	}
	
	bool RuleManagedObject::getInterfaceInt()  const 
	{
		return (m_interfaceInt);
	}

	void RuleManagedObject::setInterfaceTe(const bool &interfaceTe) 
	{
		m_interfaceTe = interfaceTe;
	}

	bool RuleManagedObject::getInterfaceTe()  const 
	{
		return (m_interfaceTe);
	}		
	
	void RuleManagedObject::setInterfaceGe(const bool &interfaceGe)	
	{
		m_interfaceGe = interfaceGe;
	}

	bool RuleManagedObject::getInterfaceGe() const
	{
		return (m_interfaceGe);
	}
	void RuleManagedObject::setInterfaceFcoe(const bool &interfaceFcoe) 
	{
		m_interfaceFcoe = interfaceFcoe;
	}
	
	bool RuleManagedObject::getInterfaceFcoe()  const 
	{
		return (m_interfaceFcoe);
	}

	void RuleManagedObject::setInterfacePc(const bool &interfacePc)
	{
		m_interfacePc = interfacePc;
	}

	bool RuleManagedObject::getInterfacePc() const
	{
		return (m_interfacePc);
	}

	void RuleManagedObject::setInterfaceVlan(const bool &interfaceVlan)
	{
		m_interfaceVlan = interfaceVlan;
	}

	bool RuleManagedObject::getInterfaceVlan() const
	{
		return (m_interfaceVlan);
	}

	void RuleManagedObject::setInterfaceMgmt(const bool &interfaceMgmt)
	{
		m_interfaceMgmt = interfaceMgmt;
	}
	
	bool RuleManagedObject::getInterfaceMgmt()  const 
	{
		return (m_interfaceMgmt);
	}
			
	void RuleManagedObject::setCopyCp(const bool &copyCp) 
	{
		m_copyCp = copyCp;
	}
	
	bool RuleManagedObject::getCopyCp()  const 
	{
		return (m_copyCp);
	}
	
	void RuleManagedObject::setCopyRunning(const bool &copyRunning) 
	{
		m_copyRunning = copyRunning;
	}
	
	bool RuleManagedObject::getCopyRunning()  const 
	{
		return (m_copyRunning);
	}
	
	void RuleManagedObject::setCopySupport(const bool &copySupport) 
	{
		m_copySupport = copySupport;
	}
	
	bool RuleManagedObject::getCopySupport()  const 
	{
		return (m_copySupport);
	}
	
	void RuleManagedObject::setClearCl(const bool &clearCl) 
	{
		m_clearCl = clearCl;
	}
	
	bool RuleManagedObject::getClearCl()  const 
	{
		return (m_clearCl);
	}
	
	void RuleManagedObject::setClearLogging(const bool &clearLogging) 
	{
		m_clearLogging = clearLogging;
	}
	
	bool RuleManagedObject::getClearLogging()  const 
	{
		return (m_clearLogging);
	}
	
	void RuleManagedObject::setClearSupport(const bool &clearSupport) 
	{
		m_clearSupport = clearSupport;
	}
	
	bool RuleManagedObject::getClearSupport()  const 
	{
		return (m_clearSupport);
	}
	
	void RuleManagedObject::setProtocolPr(const bool &protocolPr) 
	{
		m_protocolPr = protocolPr;
	}
	
	bool RuleManagedObject::getProtocolPr()  const 
	{
		return (m_protocolPr);	
	}
	
	void RuleManagedObject::setProtocolSpanning(const bool &protocolSpanning) 
	{
		m_protocolSpanning = protocolSpanning;	
	}
	
	bool RuleManagedObject::getProtocolSpanning()  const 
	{
		return (m_protocolSpanning);	
	}
	
	void RuleManagedObject::setProtocolLldp(const bool &protocolLldp) 
	{
		m_protocolLldp = protocolLldp;	
	}
	
	bool RuleManagedObject::getProtocolLldp()  const 
	{
		return (m_protocolLldp);
	}

	void RuleManagedObject::setCmdRuleCounter(const UI32 &counter)
	{
		m_cmdRuleCounter = counter;
	}

	UI32 RuleManagedObject::getCmdRuleCounter()  const
	{
		return (m_cmdRuleCounter);
	}

	void RuleManagedObject::setDataRuleCounter(const UI32 &counter)
	{
		m_dataRuleCounter = counter;
	}

	UI32 RuleManagedObject::getDataRuleCounter()  const
	{
		return (m_dataRuleCounter);
	}
}
