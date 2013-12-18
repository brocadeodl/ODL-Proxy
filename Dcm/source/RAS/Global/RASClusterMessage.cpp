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

 /**************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : James Chen                                               *
 ***************************************************************************/

#include "RAS/Global/RASClusterMessage.h"
#include "RAS/Global/RASObjectManager.h"
#include "RAS/Global/RASObjectManagerTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

namespace DcmNs
{

//////////////////////////////////////////////////////////////
//  RASClusterMessage Implementation  - BEGIN               //
#if 0
RASClusterMessage::RASClusterMessage ()
: ManagementInterfaceMessage (RASObjectManager::getClassName (), RAS_COPY_SUPPORT),
  m_support	(""), m_cmd(""), m_cliStart(""), m_cliEnd(""), m_cliError(""),
  m_cliOutput(false), m_netconf(false), m_sync(true)
{
}
#endif
RASClusterMessage::RASClusterMessage (const UI32 mappedId)
: ManagementInterfaceMessage (RASObjectManager::getClassName (), RAS_COPY_SUPPORT),
	m_mappedId         (mappedId), m_cmd(""),
	      m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(false),
		  m_netconf(false), m_sync(true)
{
}

RASClusterMessage::RASClusterMessage (const UI32 startMappedId, const UI32 endMappedId)
: ManagementInterfaceMessage (RASObjectManager::getClassName (), RAS_COPY_SUPPORT),
	m_startMappedId         (startMappedId),
	m_endMappedId		(endMappedId), m_cmd(""),
	      m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(false),
		  m_netconf(false), m_sync(true)
{
}

RASClusterMessage::RASClusterMessage (const string &data, const UI32 &userWyserTag)
: ManagementInterfaceMessage (RASObjectManager::getClassName (), RAS_CLUSTER_CFG),
	m_userWyserTag(userWyserTag),
	m_data(data), m_cmd(""),
		m_isSecure(false), m_port(514),
		m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(false),
		m_netconf(false), m_sync(true)
{
}

RASClusterMessage::RASClusterMessage ()
: ManagementInterfaceMessage (RASObjectManager::getClassName (), RAS_FOSCLI),
  m_support (""), m_cmd(""), m_cliStart(""), m_cliEnd(""), m_cliError(""),
  m_cliOutput(false), m_netconf(false), m_sync(true)
{
}

string  RASClusterMessage::getErrdump() const
{
        return (m_errdump);
}
void  RASClusterMessage::setErrdump(const string &errdump)
{
        m_errdump += errdump;
}

string  RASClusterMessage::getCmd() const
{
        return (m_cmd);
}
void  RASClusterMessage::setCmd(const string &cmd)
{
       m_cmd = cmd;
}

string  RASClusterMessage::getData() const
{
        return (m_data);
}
void  RASClusterMessage::setData(const string &data)
{
        m_data += data;
}
UI32  RASClusterMessage::getWyserUserTag() const
{
    return (m_userWyserTag);
}
void   RASClusterMessage::setWyserUserTag(const  UI32 &userWyserTag)
{
    m_userWyserTag = userWyserTag;
}
UI32  RASClusterMessage::getMappedId() const
{
    return (m_mappedId);
}
void   RASClusterMessage::setMappedId(const  UI32 &mappedId)
{
    m_mappedId = mappedId;
}
UI32  RASClusterMessage::getOrigMappedId() const
{
    return (m_origMappedId);
}
void   RASClusterMessage::setOrigMappedId(const  UI32 &mappedId)
{
    m_origMappedId = mappedId;
}
string RASClusterMessage::getVcsmsg() const
{
    return (m_vcsmsg);
}

void  RASClusterMessage::setVcsmsg(const  string &vcsmsg)
{
    m_vcsmsg = vcsmsg;
}

void  RASClusterMessage::setSyslogIp(IpVxAddress &syslogIp)
{
    m_syslogIp = syslogIp;
}

IpVxAddress  RASClusterMessage::getSyslogIp () const
{
    return ( m_syslogIp);
}

bool  RASClusterMessage::getBoolData() const
{
	return (m_boolData);
}
void  RASClusterMessage::setBoolData(const bool &boolData)
{
	m_boolData = boolData;
}

Severity  RASClusterMessage::getEnumData() const
{
	return (m_enumData);
}

void  RASClusterMessage::setEnumData(const Severity &enumData)
{
	m_enumData = enumData;
}


void  RASClusterMessage::setSyslogPort(UI16 &port)
{
    m_port = port;
}

UI16  RASClusterMessage::getSyslogPort () const
{
    return (m_port);
}

void  RASClusterMessage::setSyslogSecureMode(const bool &mode)
{
    m_isSecure = mode;
}

bool  RASClusterMessage::getSyslogSecureMode () const
{
    return (m_isSecure);
}

bool  RASClusterMessage::getAutoSupport() const
{
    return (m_isAutoSupport);
}

void  RASClusterMessage::setAutoSupport(bool support)
{
    m_isAutoSupport  =  support;
}

IpVxAddress RASClusterMessage::getHostIp () const
{
    return (m_hostip);
}

void RASClusterMessage::setHostIp (const IpVxAddress &hostIp)
{
    m_hostip = hostIp;
}

string RASClusterMessage::getUser () const
{
    return (m_username);
}

void RASClusterMessage::setUser (const string &username)
{
    m_username = username;
}

string RASClusterMessage::getProtocol () const
{
	return(m_proto);
}

void RASClusterMessage::setProtocol (const string &proto)
{
	m_proto = proto;
}

string RASClusterMessage::getDirectory () const
{
    return (m_directory);
}

void RASClusterMessage::setDirectory (const string &dir)
{
    m_directory = dir;
}

string RASClusterMessage::getPassword () const
{
    return (m_password);
}

void RASClusterMessage::setPassword (const string &pass)
{
    m_password = pass;
}

UI32 RASClusterMessage::getSeverity() const
{
	return (m_severity);
}

void RASClusterMessage::setSeverity(const UI32 &severity)
{
	m_severity = severity;
}

bool  RASClusterMessage::getSyslogEnable() const
{
    return (m_syslog);
}

void  RASClusterMessage::setSyslogEnable(bool syslog)
{
    m_syslog  =  syslog;
}

bool  RASClusterMessage::getMessageDisable() const
{
    return (m_msgDisable);
}

void  RASClusterMessage::setMessageDisable(bool disable)
{
    m_msgDisable  =  disable;
}

bool  RASClusterMessage::getModuleDisable() const
{
    return (m_modDisable);
}

void  RASClusterMessage::setModuleDisable(bool disable)
{
    m_modDisable  =  disable;
}

string RASClusterMessage::getMsgId () const
{
    return (m_msgid);
}

void RASClusterMessage::setMsgId (const string &msgid)
{
    m_msgid = msgid;
}

string RASClusterMessage::getModId () const
{
    return (m_modid);
}

void RASClusterMessage::setModId (const string &modid)
{
    m_modid = modid;
}

void  RASClusterMessage::setAuditLogclass(UI32 &auditLogclass)
{
    m_auditLogclass = auditLogclass;
}

UI32  RASClusterMessage::getAuditLogclass () const
{
    return ( m_auditLogclass);
}

void  RASClusterMessage::setSysLogfacility(RASSysFacilityTypes &syslogFacility)
{
    m_syslogFacility = syslogFacility;
}

RASSysFacilityTypes RASClusterMessage::getSysLogfacility () const
{
    return (m_syslogFacility);
}

string  RASClusterMessage::getCLIStart() const
{
        return (m_cliStart);
}
void   RASClusterMessage::setCLIStart(const  string &cliStart)
{
        m_cliStart = cliStart;
}
string  RASClusterMessage::getCLIEnd() const
{
        return (m_cliEnd);
}
void   RASClusterMessage::setCLIEnd(const  string &cliEnd)
{
        m_cliEnd = cliEnd;
}
string  RASClusterMessage::getCLIError() const
{
        return (m_cliError);
}
void   RASClusterMessage::setCLIError(const  string &cliError)
{
        m_cliError = cliError;
}
bool   RASClusterMessage::getCLIOutput() const
{
        return (m_cliOutput);
}
void   RASClusterMessage::setCLIOutput(const bool &boolData)
{
        m_cliOutput = boolData;
}

UI32  RASClusterMessage::getCLITimeout() const
{
        return (m_cliTimeout);
}

void   RASClusterMessage::setCLITimeout(const UI32 &CLITimeout)
{
        m_cliTimeout = CLITimeout;
}

RASClusterMessage::~RASClusterMessage ()
{
}

bool   RASClusterMessage::getNetConf() const
{
        return (m_netconf);
}

void   RASClusterMessage::setNetConf(const bool &boolData)
{
        m_netconf = boolData;
}

void RASClusterMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32            (&m_startMappedId,      "startMappedId"));
    addSerializableAttribute (new AttributeUI32            (&m_endMappedId,        "endMappedId"));
    addSerializableAttribute (new AttributeUI32            (&m_mappedId,           "mappedId"));
    addSerializableAttribute (new AttributeString          (&m_support,            "support"));
    addSerializableAttribute (new AttributeUI32            (&m_userWyserTag,        "userWyserTag"));
    addSerializableAttribute (new AttributeString          (&m_data,               "data"));
    addSerializableAttribute (new AttributeBool	        (&m_boolData,           "boolData"));
    addSerializableAttribute (new AttributeString          (&m_cmd,                "cmd"));
    addSerializableAttribute (new AttributeString          (&m_errdump,            "errdump"));
    addSerializableAttribute (new AttributeUI32            (&m_origMappedId,       "origMappedId"));
    addSerializableAttribute (new AttributeString          (&m_vcsmsg,             "vcsmsg"));
    addSerializableAttribute (new AttributeIpVxAddress     (&m_syslogIp,           "syslogIp"));
    addSerializableAttribute (new AttributeBool            (&m_isAutoSupport,      "isAutoSupport"));
    addSerializableAttribute (new AttributeIpVxAddress     (&m_hostip,             "hostip"));
    addSerializableAttribute (new AttributeString          (&m_username,           "username"));
    addSerializableAttribute (new AttributeString          (&m_directory,          "directory"));
    addSerializableAttribute (new AttributeString          (&m_password,           "password"));
    addSerializableAttribute (new AttributeEnum    ((UI32*)(&m_severity),          "severity"));
    addSerializableAttribute (new AttributeBool            (&m_syslog,             "syslog"));
    addSerializableAttribute (new AttributeBool            (&m_msgDisable,         "msgDisable"));
    addSerializableAttribute (new AttributeBool            (&m_modDisable,         "modDisable"));
    addSerializableAttribute (new AttributeString          (&m_msgid,              "msgid"));
    addSerializableAttribute (new AttributeString          (&m_modid,              "modid"));
    addSerializableAttribute (new AttributeUI32            (&m_auditLogclass,      "auditLogclass"));
	addSerializableAttribute (new AttributeEnum    ((UI32*)(&m_syslogFacility),    "syslogFacility"));
    addSerializableAttribute (new AttributeString          (&m_cliStart,           "cliStart"));
    addSerializableAttribute (new AttributeString          (&m_cliEnd,             "cliEnd"));
    addSerializableAttribute (new AttributeString          (&m_cliError,           "cliError"));
    addSerializableAttribute (new AttributeBool            (&m_cliOutput,          "cliOutput"));
    addSerializableAttribute (new AttributeUI32            (&m_cliTimeout,         "cliTimeout"));
    addSerializableAttribute (new AttributeBool            (&m_netconf,            "netconf"));
    addSerializableAttribute (new AttributeBool            (&m_sync,               "sync"));
}

//  RASClusterMessage Implementation  - END                 //
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//  RASClusterNetConfMessage Implementation  - BEGIN        //

RASClusterNetConfMessage::RASClusterNetConfMessage():PrismMessage(RASObjectManager::getPrismServiceId(), RAS_NETCONF)
{
}

RASClusterNetConfMessage::RASClusterNetConfMessage(NCSubCmd cmd):PrismMessage(RASObjectManager::getPrismServiceId(), RAS_NETCONF)
{
    mCmd = cmd;
}

RASClusterNetConfMessage::~RASClusterNetConfMessage()
{
}

void
RASClusterNetConfMessage::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeSI32((SI32*)&mCmd));
}

NCSubCmd
RASClusterNetConfMessage::getCmd() const
{
    return (mCmd);
}

void
RASClusterNetConfMessage::setCmd(const NCSubCmd &cmd)
{
    mCmd = cmd;
}

bool RASClusterMessage::getSync() const
{
        return (m_sync);
}

void RASClusterMessage::setSync(const bool &boolData)
{
        m_sync = boolData;
}



//  RASClusterNetConfMessage Implementation  - END          //
//////////////////////////////////////////////////////////////

}

