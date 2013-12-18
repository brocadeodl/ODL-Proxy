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
 *   Author : mmohan                                                       *
 **************************************************************************/

#include "discover/Framework/discoverObjectManager.h"
#include "discover/Framework/discoverLocalObjectManager.h"
#include "discover/Framework/discoverManagementInterfaceMessages.h"
#include "discover/Framework/discoverTypes.h"
#include "discover/events/EventDefines.h"

namespace DcmNs
{

    // ************* DiscoverDiscoverMessage ************

    DiscoverInternalMessage:: DiscoverInternalMessage()
        : ManagementInterfaceMessage (DiscoverObjectManager::getClassName (), DISCOVER_INTERNAL)
    {
        m_cmdcode = 0;

    }

    DiscoverInternalMessage::DiscoverInternalMessage (const UI32 &cmdcode,vector<vector<string> >  &m_respList)
        : ManagementInterfaceMessage (DiscoverObjectManager::getClassName (), DISCOVER_INTERNAL),
        m_cmdcode    (cmdcode),respList(m_respList)

    {
    }

    DiscoverInternalMessage::~DiscoverInternalMessage ()
    {
    }

    void  DiscoverInternalMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute(
                        new AttributeStringVectorVector(&respList, "respList",0,false));

    }

        void  DiscoverInternalMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

        UI32  DiscoverInternalMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

	
	// ************* DiscoverDiscoverMessage ************

    DiscoverDiscoverMessage::DiscoverDiscoverMessage ()
        : ManagementInterfaceMessage (DiscoverObjectManager::getClassName (), DISCOVERDISCOVER)
    {
        m_cmdcode = 0;
        m_msgType = 0;
        m_exitStatus = 0;
        m_state = 0;
    }

    DiscoverDiscoverMessage::DiscoverDiscoverMessage (const UI32 &cmdcode,const UI32 &msgType, const string &vcenter)
        : ManagementInterfaceMessage (DiscoverObjectManager::getClassName (), DISCOVERDISCOVER),
        m_cmdcode    (cmdcode),
        m_msgType    (msgType),
        m_vcenter    (vcenter)
    {
    }

    DiscoverDiscoverMessage::~DiscoverDiscoverMessage ()
    {
    }

    void  DiscoverDiscoverMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeString(&m_vcenter,"vcenter"));
        addSerializableAttribute (new AttributeString(&m_url,"url"));
        addSerializableAttribute (new AttributeString(&m_username,"username"));
        addSerializableAttribute (new AttributeString(&m_password,"password"));
        addSerializableAttribute (new AttributeUI32(&m_ignore_value,"ignoreValue"));
        addSerializableAttribute (new AttributeBool(&m_always,"always"));
        addSerializableAttribute (new AttributeString(&m_globalScriptNames,"globalScriptNames"));
        addSerializableAttribute (new AttributeString(&m_localScriptNames,"localScriptNames"));
        addSerializableAttribute (new AttributeUI32Vector(&m_locationIds,"locationIds"));
        addSerializableAttribute (new AttributeUI32(&m_exitStatus,"exitStatus"));
        addSerializableAttribute (new AttributeUI32(&m_state,"state"));
        addSerializableAttribute (new AttributeString(&m_arg1,"arg1"));
        addSerializableAttribute (new AttributeString(&m_arg2,"arg2"));
        addSerializableAttribute (new AttributeString(&m_arg3,"arg3"));
        addSerializableAttribute (new AttributeString(&m_arg4,"arg4"));
    }
    
	void  DiscoverDiscoverMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }
    
	UI32  DiscoverDiscoverMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  DiscoverDiscoverMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  DiscoverDiscoverMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  DiscoverDiscoverMessage::setVcenter(const string &vcenter)
    {
        m_vcenter = vcenter;
    }

    string  DiscoverDiscoverMessage::getVcenter() const
    {
        return (m_vcenter);
    }
    
    void  DiscoverDiscoverMessage::setUrl(const string &url)
    {
        m_url = url;
    }

    string  DiscoverDiscoverMessage::getUrl() const
    {
        return (m_url);
    }
    
    void  DiscoverDiscoverMessage::setUsername(const string &username)
    {
        m_username = username;
    }

    string  DiscoverDiscoverMessage::getUsername() const
    {
        return (m_username);
    }
    
    void  DiscoverDiscoverMessage::setPassword(const string &password)
    {
        m_password = password;
    }

    string  DiscoverDiscoverMessage::getPassword() const
    {
        return (m_password);
    }
    
	void  DiscoverDiscoverMessage::setGlobalScriptNames(const string &globalScriptNames)
    {
        m_globalScriptNames = globalScriptNames;
    }

    string  DiscoverDiscoverMessage::getGlobalScriptNames() const
	{
		return (m_globalScriptNames);
    }
	
    void  DiscoverDiscoverMessage::setLocalScriptNames(const string &localScriptNames)
    {
        m_localScriptNames = localScriptNames;
    }

    string  DiscoverDiscoverMessage::getLocalScriptNames() const
	{
		return (m_localScriptNames);
    }
    
    
    void  DiscoverDiscoverMessage::setState(const UI32 &state)
    {
        m_state  =  state;
    }

    UI32  DiscoverDiscoverMessage::getState() const
    {
        return (m_state);
    }
    
    void  DiscoverDiscoverMessage::setLocationIds(const UI32 &locationId)
    {
        m_locationIds.push_back(locationId);
    }

    vector<UI32>  DiscoverDiscoverMessage::getLocationIds() const
    {
        return (m_locationIds);
    }
    
    void  DiscoverDiscoverMessage::setExitStatus(const UI32 &exitStatus)
    {
        m_exitStatus  =  exitStatus;
    }

    UI32  DiscoverDiscoverMessage::getExitStatus() const
    {
        return (m_exitStatus);
    }
	
	void  DiscoverDiscoverMessage::setArg1(const string &arg1)
    {
        m_arg1 = arg1;
    }

    string  DiscoverDiscoverMessage::getArg1() const
    {
        return (m_arg1);
    }
	
	void  DiscoverDiscoverMessage::setArg2(const string &arg2)
    {
        m_arg2 = arg2;
    }

    string  DiscoverDiscoverMessage::getArg2() const
    {
        return (m_arg2);
    }
	
	void  DiscoverDiscoverMessage::setArg3(const string &arg3)
    {
        m_arg3 = arg3;
    }

    string  DiscoverDiscoverMessage::getArg3() const
    {
        return (m_arg3);
    }
	
	void  DiscoverDiscoverMessage::setArg4(const string &arg4)
    {
        m_arg4 = arg4;
    }

    string  DiscoverDiscoverMessage::getArg4() const
    {
        return (m_arg4);
    }

    void  DiscoverDiscoverMessage::setIgnoreValue(const UI32 &ignoreValue)
    {
        m_ignore_value  =  ignoreValue;
    }

    UI32  DiscoverDiscoverMessage::getIgnoreValue() const
    {
        return (m_ignore_value);
    }

    void  DiscoverDiscoverMessage::setAlways (const bool &always)
    {
        m_always = always;
    }

    bool DiscoverDiscoverMessage::getAlways () const
    {
        return (m_always);
    }
	
	// ************* DiscoverUpdateMessage ************

    DiscoverUpdateMessage::DiscoverUpdateMessage() :
            ManagementInterfaceMessage(DiscoverObjectManager::getClassName(),
                    DISCOVER_UPDATE) {
        m_state = DISC_STATE_UNKNOWN;
		m_exitStatus = DISC_EXIT_STATUS_SUCCESS;
    }

    DiscoverUpdateMessage::DiscoverUpdateMessage(const UI32 &cmdcode,const string &vcenter , const
		UI32 &state,const UI32 &exitStatus) :
            ManagementInterfaceMessage(DiscoverObjectManager::getClassName(),
                    DISCOVER_UPDATE), m_vcenter(vcenter), m_state(state), m_exitStatus(exitStatus),m_cmdcode(cmdcode) {
    }

    DiscoverUpdateMessage::~DiscoverUpdateMessage ()
    {
    }

    void DiscoverUpdateMessage::setupAttributesForSerialization() {
        ManagementInterfaceMessage::setupAttributesForSerialization();
        addSerializableAttribute(new AttributeString(&m_vcenter, "vcenter"));
        addSerializableAttribute(new AttributeUI32(&m_state, "state"));
        addSerializableAttribute(new AttributeUI32(&m_exitStatus, "exitStatus"));
        addSerializableAttribute(new AttributeUI32(&m_cmdcode, "cmdcode"));
    }

    void DiscoverUpdateMessage::setCmdcode(const UI32 &cmdcode) {
        m_cmdcode = cmdcode;
    }

    UI32 DiscoverUpdateMessage::getCmdcode() const {
        return (m_cmdcode);
    }

    void DiscoverUpdateMessage::setVcenter(const string &vcenter) {
        m_vcenter = vcenter;
    }

    string DiscoverUpdateMessage::getVcenter() const {
        return (m_vcenter);
    }

	void DiscoverUpdateMessage::setState(const UI32 &state){
		m_state = state;	
	}

	UI32 DiscoverUpdateMessage::getState() const {
		return (m_state);
	}

	void DiscoverUpdateMessage::setExitStatus(const UI32 &exitStatus){
		m_exitStatus = exitStatus;
	}
	
	UI32 DiscoverUpdateMessage::getExitStatus() const {
		return (m_exitStatus);
	}


	DiscoverDistributeMessage::DiscoverDistributeMessage() :
            ManagementInterfaceMessage(DiscoverObjectManager::getClassName(),
                    DISCOVER_DISTRIBUTE) {
        m_cmdcode = 0;
        m_msgType = 0;

        m_state = 0;
    }

    DiscoverDistributeMessage::DiscoverDistributeMessage(const UI32 &cmdcode,
            const UI32 &msgType, const string &vcenter) :
            ManagementInterfaceMessage(DiscoverObjectManager::getClassName(),
                    DISCOVER_DISTRIBUTE), m_cmdcode(cmdcode), m_msgType(msgType), m_vcenter(
                    vcenter) {
    }

    DiscoverDistributeMessage::~DiscoverDistributeMessage() {
    }

    void DiscoverDistributeMessage::setupAttributesForSerialization() {
        ManagementInterfaceMessage::setupAttributesForSerialization();
        addSerializableAttribute(new AttributeUI32(&m_cmdcode, "cmdcode"));
        addSerializableAttribute(new AttributeUI32(&m_msgType, "msgType"));
        addSerializableAttribute(new AttributeString(&m_vcenter, "vcenter"));

        addSerializableAttribute(
                new AttributeUI32Vector(&m_locationIds, "locationIds"));
        addSerializableAttribute(
                       new AttributeStringVector(&m_datalist, "dataList"));

    }

    void DiscoverDistributeMessage::setCmdcode(const UI32 &cmdcode) {
        m_cmdcode = cmdcode;
    }

    UI32 DiscoverDistributeMessage::getCmdcode() const {
        return (m_cmdcode);
    }

    void DiscoverDistributeMessage::setMsgType(const UI32 &msgType) {
        m_msgType = msgType;
    }

    UI32 DiscoverDistributeMessage::getMsgType() const {
        return (m_msgType);
    }

    void DiscoverDistributeMessage::setVcenter(const string &vcenter) {
        m_vcenter = vcenter;
    }

    string DiscoverDistributeMessage::getVcenter() const {
        return (m_vcenter);
    }
    void DiscoverDistributeMessage::setLocationIds(vector<UI32> &locationIds) {
        m_locationIds = locationIds;
    }

    vector<UI32> DiscoverDistributeMessage::getLocationIds() const {
        return (m_locationIds);
    }
    void DiscoverDistributeMessage::setDataList(vector<string> &dataList) {
        m_datalist = dataList;
    }

    vector<string> DiscoverDistributeMessage::getDataList() const {
        return (m_datalist);
    }

    //DistributeDBMessage

    DistributeDBMessage::DistributeDBMessage() :
            ManagementInterfaceMessage(DiscoverObjectManager::getClassName(),
                    DISCOVER_DISTRIBUTE_DB) {

        m_state = 0;
        m_dest_dbfile = "";
    }

    DistributeDBMessage::DistributeDBMessage(const string &dbfile) :
            ManagementInterfaceMessage(DiscoverObjectManager::getClassName(),
                    DISCOVER_DISTRIBUTE_DB),m_dbfile(dbfile),m_dest_dbfile(dbfile) {
    }

    DistributeDBMessage::~DistributeDBMessage() {
    }

    void DistributeDBMessage::setupAttributesForSerialization() {
        ManagementInterfaceMessage::setupAttributesForSerialization();

        addSerializableAttribute(new AttributeString(&m_dbfile, "dbfile"));
        addSerializableAttribute(new AttributeString(&m_dest_dbfile, "destdbfile"));

        addSerializableAttribute(
                new AttributeUI32Vector(&m_locationIds, "locationIds"));

    }

    void DistributeDBMessage::setDBfile(const string &dbfile) {
        m_dbfile = dbfile;
    }

    string DistributeDBMessage::getDBfile() const {
        return (m_dbfile);
    }

   void DistributeDBMessage::setDestDBfile(const string &destdbfile) {
        m_dest_dbfile = destdbfile;
    }

    string DistributeDBMessage::getDestDBfile() const {
        return (m_dest_dbfile);
    }

    void DistributeDBMessage::setLocationIds(vector<UI32> &locationIds) {
        m_locationIds = locationIds;
    }

    vector<UI32> DistributeDBMessage::getLocationIds() const {
        return (m_locationIds);
    }


   VNMDPMessage::VNMDPMessage ()
        : ManagementInterfaceMessage (DiscoverObjectManager::getClassName(),DP_EVENT)
    {
    }

    VNMDPMessage::VNMDPMessage (const UI32 &cmdCode,const string &ifName,const string &hostMor,const string &portId, const string &vcenter)
        : ManagementInterfaceMessage (DiscoverObjectManager::getClassName(),DP_EVENT),
        m_cmdCode    (cmdCode),
        m_ifName    (ifName),
        m_hostMor    (hostMor),
        m_portId    (portId),
        m_vcenter    (vcenter)
    {
    }

    VNMDPMessage::~VNMDPMessage ()
    {
    }

    void  VNMDPMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_hostMor,"hostMor"));
        addSerializableAttribute (new AttributeString(&m_portId,"portId"));
        addSerializableAttribute (new AttributeString(&m_vcenter,"vcenter"));
    }


    void  VNMDPMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  VNMDPMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  VNMDPMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  VNMDPMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  VNMDPMessage::setHostMor(const string &hostMor)
    {
        m_hostMor  =  hostMor;
    }

    string  VNMDPMessage::getHostMor() const
    {
        return (m_hostMor);
    }

    void  VNMDPMessage::setPortId(const string &portId)
    {
        m_portId  =  portId;
    }

    string  VNMDPMessage::getPortId() const
    {
        return (m_portId);
    }

    void  VNMDPMessage::setVcenter(const string &vcenter)
    {
        m_vcenter  =  vcenter;
    }

    string  VNMDPMessage::getVcenter() const
    {
        return (m_vcenter);
    }


}
