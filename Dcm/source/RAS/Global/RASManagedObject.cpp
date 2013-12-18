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
 *   Author : James Chen                                    *
 ***************************************************************************/
#include "Framework/Types/IpVxAddress.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

#include "RAS/Global/RASManagedObject.h"
#include "RAS/Global/RASObjectManager.h"
#include "RAS/Global/RASObjectManagerTypes.h"
//#include "RAS.h"
#include "vcs.h"
namespace DcmNs
{

RASManagedObject::RASManagedObject (RASObjectManager *pRASObjectManager)
    : PrismElement           (pRASObjectManager),
      PrismPersistableObject (RASManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject      (pRASObjectManager),
      DcmManagedObject       (pRASObjectManager)
{
}

RASManagedObject::~RASManagedObject ()
{
}

string RASManagedObject::getClassName ()
{
    return ("RASManagedObject");
}

void RASManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();
    addPersistableAttribute (new AttributeIpVxAddress(&m_syslogIp,"ipaddress", ras_syslogip));
    addPersistableAttribute (new AttributeUI16(&m_port, "port", ras_port));
    addPersistableAttribute (new AttributeBool(&m_isSecure, "issecure", ras_secure));

    // for internal cfg
    addPersistableAttribute (new AttributeString (&m_support, "support"));
}

void RASManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();
    addPersistableAttributeForCreate (new AttributeIpVxAddress(&m_syslogIp,"ipaddress", ras_syslogip));
    addPersistableAttributeForCreate (new AttributeUI16(&m_port, "port", ras_port));
    addPersistableAttributeForCreate (new AttributeBool(&m_isSecure, "issecure", ras_secure));

    // for internal cfg
    addPersistableAttributeForCreate (new AttributeString (&m_support, "support"));
}

void RASManagedObject::setupKeys()
{
        vector<string > keyName;
        keyName.push_back ("ipaddress");
        setUserDefinedKeyCombination (keyName);
}

IpVxAddress RASManagedObject::getSyslogIp () const
{
    return (m_syslogIp);
}

void RASManagedObject::setSyslogIp (const IpVxAddress &syslogIp)
{
    m_syslogIp = syslogIp;
}

/*
 * Global Config Class Managed Object 
 */

RASGlobalConfigManagedObject::RASGlobalConfigManagedObject (RASObjectManager *pRASObjectManager)
    : PrismElement           (pRASObjectManager),
      PrismPersistableObject (RASGlobalConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject      (pRASObjectManager),
      DcmManagedObject       (pRASObjectManager),
	  m_console(DCM_RASLOG_INFO),
	  m_isFFDC(true)
{
}
RASGlobalConfigManagedObject::~RASGlobalConfigManagedObject ()
{
}

string RASGlobalConfigManagedObject::getClassName ()
{
    return ("RASGlobalConfigManagedObject");
}

void RASGlobalConfigManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeEnum((UI32*)(&m_console), "console", ras_console));
	addPersistableAttribute (new AttributeBool (&m_isFFDC, "ffdc", ras_ffdc));
    // for internal cfg
    addPersistableAttribute (new AttributeString (&m_support, "support"));
}

void RASGlobalConfigManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();

	addPersistableAttributeForCreate  (new AttributeEnum((UI32*)(&m_console),"console", ras_console));
	addPersistableAttributeForCreate  (new AttributeBool(&m_isFFDC, "ffdc", ras_ffdc));
	
    // for internal cfg
    addPersistableAttributeForCreate (new AttributeString (&m_support, "support"));
}

void RASGlobalConfigManagedObject::setConsole(const UI32 &console)
{
	m_console  =  console;
}

UI32 RASGlobalConfigManagedObject::getConsole() const
{
	return (m_console);
}
void RASGlobalConfigManagedObject::setFFDC(bool ffdc)
{   
	m_isFFDC  =  ffdc;
}

bool RASGlobalConfigManagedObject::getFFDC() const
{
	return (m_isFFDC);
}



UI16  RASManagedObject::getSyslogPort() const
{
    return (m_port);
}

void  RASManagedObject::setSyslogPort (const UI16 &port)
{
    m_port = port;
}

bool  RASManagedObject::getSyslogSecureMode() const
{
    return (m_isSecure);
}

void  RASManagedObject::setSyslogSecureMode (const bool &mode)
{
    m_isSecure = mode;
}


/*
 * Autoupload class managed object 
 */
RASAutoUploadManagedObject::RASAutoUploadManagedObject (RASObjectManager *pRASObjectManager)
    : PrismElement           (pRASObjectManager),
      PrismPersistableObject (RASAutoUploadManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject      (pRASObjectManager),
      DcmManagedObject       (pRASObjectManager),
	  m_isAutoSupport(false)
{
}
RASAutoUploadManagedObject::~RASAutoUploadManagedObject ()
{
}

string RASAutoUploadManagedObject::getClassName ()
{
    return ("RASAutoUploadManagedObject");
}

void RASAutoUploadManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();
    addPersistableAttribute (new AttributeBool(&m_isAutoSupport,"autosupport", ras_enable));
    addPersistableAttribute (new AttributeIpVxAddress(&m_hostip,"hostip", ras_hostip));
	addPersistableAttribute (new AttributeString(&m_username,"username", ras_username));
	addPersistableAttribute (new AttributeString(&m_directory,"directory", ras_directory));
	addPersistableAttribute (new AttributeString(&m_password,"password", ras_password));
	addPersistableAttribute (new AttributeString(&m_proto,"protocol", ras_protocol));

    // for internal cfg
	addPersistableAttribute (new AttributeString (&m_support, "support"));
}

void RASAutoUploadManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();
    addPersistableAttributeForCreate (new AttributeBool(&m_isAutoSupport,"autosupport", ras_enable));
    addPersistableAttributeForCreate (new AttributeIpVxAddress(&m_hostip,"hostip", ras_hostip));
	addPersistableAttributeForCreate  (new AttributeString(&m_username,"username", ras_username));
	addPersistableAttributeForCreate  (new AttributeString(&m_directory,"directory", ras_directory));
	addPersistableAttributeForCreate  (new AttributeString(&m_password,"password", ras_password));
	addPersistableAttributeForCreate  (new AttributeString(&m_proto,"protocol", ras_protocol));

    // for internal cfg
	addPersistableAttributeForCreate (new AttributeString (&m_support, "support"));
}

bool RASAutoUploadManagedObject::getAutoSupport () const
{
    return (m_isAutoSupport);
}

void RASAutoUploadManagedObject::setAutoSupport (const bool &autosupport)
{
    m_isAutoSupport = autosupport;
}

void RASAutoUploadManagedObject::setHostIp (const IpVxAddress &hostIp)
{       
    m_hostip = hostIp;
}

IpVxAddress RASAutoUploadManagedObject::getHostIp () const
{       
    return (m_hostip);
}

string RASAutoUploadManagedObject::getUser () const
{
    return (m_username);
}

void RASAutoUploadManagedObject::setUser (const string &username)
{
    m_username = username;
}

string RASAutoUploadManagedObject::getProtocol () const
{
	return(m_proto);
}

void RASAutoUploadManagedObject::setProtocol (const string &proto)
{
	m_proto = proto;
}

string RASAutoUploadManagedObject::getDirectory () const
{
    return (m_directory);
}

void RASAutoUploadManagedObject::setDirectory (const string &dir)
{
    m_directory = dir;
}

string RASAutoUploadManagedObject::getPassword () const
{ 
    return (m_password);
} 

void RASAutoUploadManagedObject::setPassword (const string &pass)
{
    m_password = pass;
}


/*
 * RASLOG Configure managed object
 */
RASLOGConfigureManagedObject::RASLOGConfigureManagedObject (RASObjectManager *pRASObjectManager)
    : PrismElement           (pRASObjectManager),
      PrismPersistableObject (RASLOGConfigureManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject      (pRASObjectManager),
      DcmManagedObject       (pRASObjectManager),
	  m_severity(DCM_RASLOG_DEFAULT),
	  m_syslog(false),
	  m_msgDisable(false)
{
}

RASLOGConfigureManagedObject::~RASLOGConfigureManagedObject ()
{
}

string RASLOGConfigureManagedObject::getClassName ()
{
    return ("RASLOGConfigureManagedObject");
}

void RASLOGConfigureManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeString(&m_msgId, "msgId", ras_msgId));
    addPersistableAttribute (new AttributeEnum(((UI32*)(&m_severity)),"severity", ras_severity));
	addPersistableAttribute (new AttributeBool(&m_syslog, "syslog", ras_syslog));
	addPersistableAttribute (new AttributeBool(&m_msgDisable, "msgdisable", ras_suppress));

    // for internal cfg
    addPersistableAttribute (new AttributeString (&m_support, "support"));
}

void RASLOGConfigureManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeString(&m_msgId, "msgId", ras_msgId));
    addPersistableAttributeForCreate (new AttributeEnum(((UI32*)(&m_severity)),"severity", ras_severity));
	addPersistableAttributeForCreate (new AttributeBool(&m_syslog, "syslog", ras_syslog));
	addPersistableAttributeForCreate (new AttributeBool(&m_msgDisable, "msgdisable", ras_suppress));

    // for internal cfg
    addPersistableAttributeForCreate (new AttributeString (&m_support, "support"));
}

void RASLOGConfigureManagedObject::setupKeys()
{
        vector<string > keyName;
        keyName.push_back ("msgId");
        setUserDefinedKeyCombination (keyName);
}

string RASLOGConfigureManagedObject::getMsgId() const
{
    return (m_msgId);
}

void RASLOGConfigureManagedObject::setMsgId(const string &msgid)
{ 
    m_msgId = msgid;
}

UI32 RASLOGConfigureManagedObject::getSeverity() const
{
    return (m_severity);
}

void RASLOGConfigureManagedObject::setSeverity(const UI32 &severity)
{ 
    m_severity = severity;
}

bool RASLOGConfigureManagedObject::getSyslogEnable() const
{
    return (m_syslog);
}

void RASLOGConfigureManagedObject::setSyslogEnable(const bool &syslog)
{ 
    m_syslog = syslog;
}

bool RASLOGConfigureManagedObject::getMessageDisable() const
{
    return (m_msgDisable);
}

void RASLOGConfigureManagedObject::setMessageDisable(const bool &disable)
{ 
    m_msgDisable = disable;
}


/*
 * RASMOD Configure managed object
 */
RASMODConfigureManagedObject::RASMODConfigureManagedObject (RASObjectManager *pRASObjectManager)
    : PrismElement           (pRASObjectManager),
      PrismPersistableObject (RASMODConfigureManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject      (pRASObjectManager),
      DcmManagedObject       (pRASObjectManager),
	  m_modDisable(false)
{
}

RASMODConfigureManagedObject::~RASMODConfigureManagedObject ()
{
}

string RASMODConfigureManagedObject::getClassName ()
{
    return ("RASMODConfigureManagedObject");
}

void RASMODConfigureManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeString(&m_modId, "modId", ras_modId));
	addPersistableAttribute (new AttributeBool(&m_modDisable, "moddisable"));

    // for internal cfg
    addPersistableAttribute (new AttributeString (&m_support, "support"));
}

void RASMODConfigureManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeString(&m_modId, "modId", ras_modId));
	addPersistableAttributeForCreate (new AttributeBool(&m_modDisable, "moddisable"));

    // for internal cfg
    addPersistableAttributeForCreate (new AttributeString (&m_support, "support"));
}

void RASMODConfigureManagedObject::setupKeys()
{
        vector<string > keyName;
        keyName.push_back ("modId");
        setUserDefinedKeyCombination (keyName);
}

string RASMODConfigureManagedObject::getModId() const
{
    return (m_modId);
}

void RASMODConfigureManagedObject::setModId(const string &modid)
{ 
    m_modId = modid;
}

bool RASMODConfigureManagedObject::getModuleDisable() const
{
    return (m_modDisable);
}

void RASMODConfigureManagedObject::setModuleDisable(const bool &disable)
{ 
    m_modDisable = disable;
}

/*
 * Auditlog class managed object
 */
RASAuditManagedObject::RASAuditManagedObject (RASObjectManager *pRASObjectManager)
    : PrismElement           (pRASObjectManager),
      PrismPersistableObject (RASAuditManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject      (pRASObjectManager),
      DcmManagedObject       (pRASObjectManager)
{
}

RASAuditManagedObject::RASAuditManagedObject (RASObjectManager *pRASObjectManager, const RASAuditClassTypes &auditLogclass)
    : PrismElement           (pRASObjectManager),
      PrismPersistableObject (RASAuditManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject      (pRASObjectManager),
      DcmManagedObject       (pRASObjectManager),
      m_auditLogclass (auditLogclass)
{
}

RASAuditManagedObject::~RASAuditManagedObject ()
{
}

string RASAuditManagedObject::getClassName ()
{
    return ("RASAuditManagedObject");
}

void RASAuditManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();
    addPersistableAttribute (new AttributeEnum((UI32*)(&m_auditLogclass),"auditlogclass", ras_class));

    // for internal cfg
    addPersistableAttribute (new AttributeString (&m_support, "support"));
}

void RASAuditManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();
    addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_auditLogclass),"auditlogclass", ras_class));

    // for internal cfg
    addPersistableAttributeForCreate (new AttributeString (&m_support, "support"));
}

void RASAuditManagedObject::setupKeys()
{
        vector<string > keyName;
	  keyName.push_back ("auditlogclass");
        setUserDefinedKeyCombination (keyName);
}

UI32 RASAuditManagedObject::getAuditLogclass () const
{
    return (m_auditLogclass);
}

void RASAuditManagedObject::setAuditLogclass (const UI32 &auditLogclass)
{
    m_auditLogclass = auditLogclass;
}

/*
 * Syslog facility managed object
 */
RASSysFcManagedObject::RASSysFcManagedObject (RASObjectManager *pRASObjectManager)
    : PrismElement           (pRASObjectManager),
      PrismPersistableObject (RASSysFcManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject      (pRASObjectManager),
      DcmManagedObject       (pRASObjectManager)
{
}

RASSysFcManagedObject::RASSysFcManagedObject (RASObjectManager *pRASObjectManager, const RASSysFacilityTypes &syslogFacility)
    : PrismElement           (pRASObjectManager),
      PrismPersistableObject (RASSysFcManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject      (pRASObjectManager),
      DcmManagedObject       (pRASObjectManager),
      m_syslogFacility (syslogFacility)
{
}

RASSysFcManagedObject::~RASSysFcManagedObject ()
{
}

string RASSysFcManagedObject::getClassName ()
{
    return ("RASSysFcManagedObject");
}

void RASSysFcManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();
    addPersistableAttribute (new AttributeEnum((UI32*)(&m_syslogFacility),"syslogfacility", ras_local));

    // for internal cfg
    addPersistableAttribute (new AttributeString (&m_support, "support"));
}

void RASSysFcManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();
    addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_syslogFacility),"syslogfacility", ras_local));

    // for internal cfg
    addPersistableAttributeForCreate (new AttributeString (&m_support, "support"));
}


UI32 RASSysFcManagedObject::getSysFacility () const
{
    return (m_syslogFacility);
}

void RASSysFcManagedObject::setSysFacility (const UI32 &syslogFacility)
{
    m_syslogFacility = syslogFacility;
}
}
