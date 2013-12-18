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
 *   Author : chenj                                                     *
 **************************************************************************/

#include "RAS/Local/RASLocalObjectManager.h"
#include "RAS/Local/RASLoggingMessage.h"
#include "RAS/Local/RASTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Types/IpVxAddress.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

#include "vcs.h"
#include "raslog/raslog.h"
//#include "brocade-RAS-show.h"

namespace DcmNs
{

//////////////////////////////////////////////////////////////
//  RASLoggingMessage Implementation  - BEGIN               //

	// Every MSG O will be use in different inquiry
RASLoggingMessage::RASLoggingMessage ()
	    : PrismMessage (RASLocalObjectManager::getPrismServiceId (), FOSCLI),
	      m_errdump(""), m_cmd(""),
	      m_cliStart(""), m_cliEnd(""), m_cliError(""),
          m_cliOutput(false), m_netconf(false), m_sync(true)
{
	    // trace (TRACE_LEVEL_DEBUG, string (" RASLoggingMessage::RASLoggingMessage()"));
}

RASLoggingMessage::~RASLoggingMessage ()
{
}
// config RAS string data type
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, const string &data, UI32 &userWyserTag)
	    : PrismMessage (RASLocalObjectManager::getPrismServiceId (), RASLOGGING),
	m_ObjectId(LoggingOID),	m_userWyserTag(userWyserTag), m_data(data), m_cmd(""),
	m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(false),
	m_netconf(false), m_sync(true)
{
	    // trace (TRACE_LEVEL_DEBUG, string (" RASLoggingMessage::RASLoggingMessage()"));
}

// config RAS bool data type
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, const bool &data, UI32 &userWyserTag)
	    : PrismMessage (RASLocalObjectManager::getPrismServiceId (), RASLOGGING),
	m_ObjectId(LoggingOID),	m_userWyserTag(userWyserTag), m_boolData(data),
        m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(false),
		m_netconf(false), m_sync(true)
{
	    // trace (TRACE_LEVEL_DEBUG, string (" RASLoggingMessage::RASLoggingMessage()"));
}

RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, const bool &data, const bool &data1, UI32 &userWyserTag)
	    : PrismMessage (RASLocalObjectManager::getPrismServiceId (), RASGLOBALCONFIG),
	m_ObjectId(LoggingOID),	m_userWyserTag(userWyserTag), m_boolData(data),
        m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(false),
		m_netconf(false), m_sync(true)
{
	    // trace (TRACE_LEVEL_DEBUG, string (" RASLoggingMessage::RASLoggingMessage()"));
}
// config RAS autoSupport bool data type
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, const bool &data, UI32 &dataSupport, UI32 &userWyserTag)
	    : PrismMessage (RASLocalObjectManager::getPrismServiceId (), AUTOUPLOADCFG),
	m_ObjectId(LoggingOID),	m_userWyserTag(userWyserTag), m_isAutoSupport(data),
        m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(false),
		m_netconf(false), m_sync(true)
{
}

// config RAS autoSupport Param data type
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, const string &data, UI32 &dataSupport, UI32 &userWyserTag)
	    : PrismMessage (RASLocalObjectManager::getPrismServiceId (), AUTOUPLOADCFGPARAM),
	m_ObjectId(LoggingOID),	m_userWyserTag(userWyserTag), 
        m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(false),
		m_netconf(false), m_sync(true)
{
}

// config RASLOG severity data type
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, const UI32 &severity, const string &msgid, UI32 &userWyserTag)
	: PrismMessage (RASLocalObjectManager::getPrismServiceId (), RASLOGSEVCFG),
	m_ObjectId(LoggingOID), m_userWyserTag(userWyserTag), m_severity(severity), m_msgid(msgid),
	m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(false), m_sync(true)
{
}

// config RASLOG syslog data type
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, UI32 &userWyserTag, const bool &data, const string &msgid)
	: PrismMessage (RASLocalObjectManager::getPrismServiceId (), RASLOGSYSLOGCFG),
	m_ObjectId(LoggingOID), m_userWyserTag(userWyserTag), m_syslog(data), m_msgid(msgid),
	m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(false), m_sync(true)
{
}
// config RASLOG message data type
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, const bool &data, const string &msgid, UI32 &userWyserTag)
    : PrismMessage (RASLocalObjectManager::getPrismServiceId (), RASLOGMSGCFG),
    m_ObjectId(LoggingOID), m_userWyserTag(userWyserTag), m_msgDisable(data), m_msgid(msgid),
    m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(false), m_sync(true)
{
}

// config RASLOG module data type
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, const string &modid, const bool &data, UI32 &userWyserTag)
    : PrismMessage (RASLocalObjectManager::getPrismServiceId (), RASLOGMODCFG),
    m_ObjectId(LoggingOID), m_userWyserTag(userWyserTag),  m_modDisable(data), m_modid(modid),
    m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(false), m_sync(true)
{
}

// config RAS bool data type
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, const Severity &enumData, UI32 &userWyserTag)
	: PrismMessage (RASLocalObjectManager::getPrismServiceId (), RASGLOBALCONFIG),
	m_ObjectId(LoggingOID), m_userWyserTag(userWyserTag), m_enumData(enumData),
	m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(false), m_sync(true)
{
	// trace (TRACE_LEVEL_DEBUG, string (" RASLoggingMessage::RASLoggingMessage()"));
}
// config RAS enum data type
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, UI32 &userWyserTag)
	    : PrismMessage (RASLocalObjectManager::getPrismServiceId (), IPVXCFG),
	m_ObjectId(LoggingOID),	m_userWyserTag(userWyserTag), m_cliStart(""),
	m_cliEnd(""), m_cliError(""), m_cliOutput(false), m_netconf(false), m_sync(true)
{
	    // trace (TRACE_LEVEL_DEBUG, string (" RASLoggingMessage::RASLoggingMessage()"));
}
// RAS action request
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, UI32 &auditLogclass, UI32 &userWyserTag)
	    : PrismMessage (RASLocalObjectManager::getPrismServiceId (), AUDITCLASSCFG),
	m_ObjectId(LoggingOID), m_auditLogclass(auditLogclass), m_cliStart(""), m_cliEnd(""), m_cliError(""),
	m_cliOutput(false), m_sync(true)
{
	    // trace (TRACE_LEVEL_DEBUG, string (" RASLoggingMessage::RASLoggingMessage()"));
}
// RAS syslog facility request
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, const RASSysFacilityTypes &syslogFacility, UI32 &isSingleton, UI32 &userWyserTag)
	    : PrismMessage (RASLocalObjectManager::getPrismServiceId (), SYSFACILITYCFG),
	m_ObjectId(LoggingOID),	m_userWyserTag(userWyserTag), m_sysLogfacility(syslogFacility), m_cliStart(""),
	m_cliEnd(""), m_cliError(""), m_cliOutput(false), m_sync(true)
{
	    // trace (TRACE_LEVEL_DEBUG, string (" RASLoggingMessage::RASLoggingMessage()"));
}
// RAS audit class request
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID)
	    : PrismMessage (RASLocalObjectManager::getPrismServiceId (), FOSCLI),
	m_ObjectId(LoggingOID), m_errdump(""), m_cmd(""), m_cliStart(""),
	m_cliEnd(""), m_cliError(""), m_cliOutput(false), m_netconf(false), m_sync(true)
    {
	    // trace (TRACE_LEVEL_DEBUG, string (" RASLoggingMessage::RASLoggingMessage()"));
    }

// RAS action request for Support Save Status
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID,
                                      ss_progress_t &ss_status) :
                   PrismMessage (RASLocalObjectManager::getPrismServiceId (),
                        SUPPORTSAVESTATUS), m_ObjectId(LoggingOID)
  	{
    }

#if 0
// RAS action request
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, const bool &data)
	    : PrismMessage (SSLocalObjectManager::getPrismServiceId (), SUPPORTSAVE),
	m_ObjectId(LoggingOID), m_cliStart(""), m_cliEnd(""), m_cliError(""),
	m_cliOutput(false), m_sync(true)
    {
	    // trace (TRACE_LEVEL_DEBUG, string (" RASLoggingMessage::RASLoggingMessage()"));
    }
#endif


#if 0
RASLoggingMessage::RASLoggingMessage (const ObjectId &LoggingOID, const vcsmsg_t &vcsmsg)
	    : PrismMessage (RASLocalObjectManager::getPrismServiceId (), VCSRASLOGGING),
	m_ObjectId(LoggingOID),	m_vcsmsg(vcsmsg),
	m_cliStart(""), m_cliEnd(""), m_cliError(""), m_cliOutput(true)
    {
	    // trace (TRACE_LEVEL_DEBUG, string (" RASLoggingMessage::RASLoggingMessage()"));
    }
#endif

// Prepare message for transmission over the network
// any data which referenced by remote node need addSerializableAttribute here
void  RASLoggingMessage::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeObjectId         (&m_ObjectId,           "ObjectId"));
    addSerializableAttribute (new AttributeString           (&m_raslog,             "raslog"));
    addSerializableAttribute (new AttributeString           (&m_data,               "data"));
    addSerializableAttribute (new AttributeBool             (&m_boolData,           "boolData"));
    addSerializableAttribute (new AttributeEnum             ((UI32*)(&m_enumData),  "enumData"));
    addSerializableAttribute (new AttributeUI32             (&m_userWyserTag,        "userWyserTag"));
    addSerializableAttribute (new AttributeString           (&m_cmd,                "cmd"));
    addSerializableAttribute (new AttributeUI32             (&m_mappedId,           "mappedId"));
    // addSerializableAttribute (new AttributeIpV4Address(&m_syslogIp));
    addSerializableAttribute (new AttributeIpVxAddress      (&m_syslogIp,           "ipaddress"));
    addSerializableAttribute (new AttributeEnum(     (UI32*)(&m_auditLogclass),     "auditlogclass"));
    addSerializableAttribute (new AttributeEnum     ((UI32*)(&m_sysLogfacility),    "syslogfacility"));
    // addSerializableAttribute (new AttributeBool(&m_ffdc,"ffdc"));
    addSerializableAttribute (new AttributeBool             (&m_isAutoSupport,      "autosupport"));
    addSerializableAttribute (new AttributeIpVxAddress      (&m_hostip,             "hostip"));
    addSerializableAttribute (new AttributeString           (&m_username,           "username"));
    addSerializableAttribute (new AttributeString           (&m_directory,          "directory"));
    addSerializableAttribute (new AttributeString           (&m_password,           "password"));
    addSerializableAttribute (new AttributeString           (&m_msgid,              "msgid"));
    addSerializableAttribute (new AttributeString           (&m_modid,              "modid"));
    addSerializableAttribute (new AttributeEnum     ((UI32*)(&m_severity),          "severity"));
    addSerializableAttribute (new AttributeBool             (&m_syslog,             "syslog"));
    addSerializableAttribute (new AttributeBool             (&m_msgDisable,         "msgdisable"));
    addSerializableAttribute (new AttributeBool             (&m_modDisable,         "moddisable"));
    addSerializableAttribute (new AttributeBool             (&m_cliOutput,          "cliOutput"));
    addSerializableAttribute (new AttributeBool             (&m_netconf,            "netconf"));
    addSerializableAttribute (new AttributeBool             (&m_sync,               "sync"));
}

ObjectId RASLoggingMessage::getObjectId () const
{
	return (m_ObjectId);
}

void  RASLoggingMessage::setRaslog(const string &raslog)
{
        m_raslog  =  raslog;
}

string  RASLoggingMessage::getRaslog() const
{
        return (m_raslog);
}

string  RASLoggingMessage::getData() const
{
        return (m_data);
}
void  RASLoggingMessage::setData(const string &Data)
{
        m_data = Data;
}

bool  RASLoggingMessage::getBoolData() const
{
        return (m_boolData);
}
void  RASLoggingMessage::setBoolData(const bool &boolData)
{
        m_boolData = boolData;
}

Severity  RASLoggingMessage::getEnumData() const
{
	return (m_enumData);
}

void  RASLoggingMessage::setEnumData(const Severity &enumData)
{
	m_enumData = enumData;
}

UI32 RASLoggingMessage::getWyserUserTag() const
    {
        return (m_userWyserTag);
    }
void  RASLoggingMessage::setWyserUserTag(const  UI32 &userWyserTag)
    {
        m_userWyserTag = userWyserTag;
    }
string  RASLoggingMessage::getErrdump() const
    {
        return (m_errdump);
    }
void  RASLoggingMessage::setErrdump(const string &errdump)
    {
        m_errdump = errdump;
    }
string  RASLoggingMessage::getCmd() const
    {
        return (m_cmd);
    }
void  RASLoggingMessage::setCmd(const string &cmd)
    {
        m_cmd = cmd;
    }
#if 1
IpVxAddress  RASLoggingMessage::getSyslogIp () const
{
    return (m_syslogIp);
}

void  RASLoggingMessage::setSyslogIp (const IpVxAddress &syslogIp)
{
    m_syslogIp = syslogIp;
}
#endif

bool  RASLoggingMessage::getAutoSupport() const
{
	return (m_isAutoSupport);
}   

void  RASLoggingMessage::setAutoSupport(const bool &support)
{
	m_isAutoSupport  =  support;
}       
        
IpVxAddress RASLoggingMessage::getHostIp () const
{       
    return (m_hostip);
}       
        
void RASLoggingMessage::setHostIp (const IpVxAddress &hostIp)
{       
    m_hostip = hostIp;
}       

string RASLoggingMessage::getUser () const
{
    return (m_username);
}

void RASLoggingMessage::setUser (const string &username)
{
    m_username = username;
}

string RASLoggingMessage::getProtocol () const
{
    return (m_proto);
}

void RASLoggingMessage::setProtocol (const string &proto)
{
    m_proto = proto;
}

string RASLoggingMessage::getDirectory () const
{
    return (m_directory);
}

void RASLoggingMessage::setDirectory (const string &dir)
{
    m_directory = dir;
}

string RASLoggingMessage::getPassword () const
{
    return (m_password);
}

void RASLoggingMessage::setPassword (const string &pass)
{
    m_password = pass;
}

string RASLoggingMessage::getMsgId () const
{ 
    return (m_msgid);
} 

void RASLoggingMessage::setMsgId (const string &msgid)
{
    m_msgid = msgid;
}

string RASLoggingMessage::getModId () const
{ 
    return (m_modid);
} 

void RASLoggingMessage::setModId (const string &modid)
{
    m_modid = modid;
}

UI32  RASLoggingMessage::getSeverity () const
{
    return (m_severity);
}       
    
void  RASLoggingMessage::setSeverity (const UI32 &severity)
{
    m_severity = severity;
}

bool  RASLoggingMessage::getSyslogEnable() const
{
    return (m_syslog);
}

void  RASLoggingMessage::setSyslogEnable(const bool &syslog)
{
    m_syslog  =  syslog;
}

bool  RASLoggingMessage::getMessageDisable() const
{
    return (m_msgDisable);
}

void  RASLoggingMessage::setMessageDisable(const bool &disable)
{
    m_msgDisable  =  disable;
}

bool  RASLoggingMessage::getModuleDisable() const
{
    return (m_modDisable);
}

void  RASLoggingMessage::setModuleDisable(const bool &disable)
{
    m_modDisable  =  disable;
}


UI32  RASLoggingMessage::getAuditLogclass () const
{
    return (m_auditLogclass);
}

void  RASLoggingMessage::setAuditLogclass (const UI32 &auditLogclass)
{
    m_auditLogclass = auditLogclass;
}

RASSysFacilityTypes RASLoggingMessage::getSysLogfacility () const
{
    return (m_sysLogfacility);
}

void  RASLoggingMessage::setSysLogfacility (const RASSysFacilityTypes &sysLogfacility)
{
    m_sysLogfacility = sysLogfacility;
}

#if 0
void  RASLoggingMessage::setSyslogIp(IpV4Address &syslogIp)
    {
         m_syslogIp = syslogIp;
    }

IpV4Address  RASLoggingMessage::getSyslogIp () const
    {
        return ( m_syslogIp);
    }
#endif

UI32 RASLoggingMessage::getMappedId() const
    {
        return (m_mappedId);
    }
void  RASLoggingMessage::setMappedId(const  UI32 &mappedId)
    {
        m_mappedId = mappedId;
    }


string RASLoggingMessage::getVcsmsg() const
    {
        return (m_vcsmsg);
    }
void  RASLoggingMessage::setVcsmsg(const  string &vcsmsg)
    {
        m_vcsmsg = vcsmsg;
    }
string RASLoggingMessage::getCLIStart() const
{
        return (m_cliStart);
}
void  RASLoggingMessage::setCLIStart(const  string &cliStart)
{
        m_cliStart = cliStart;
}
string RASLoggingMessage::getCLIEnd() const
{
        return (m_cliEnd);
}
void  RASLoggingMessage::setCLIEnd(const  string &cliEnd)
{
        m_cliEnd = cliEnd;
}
string RASLoggingMessage::getCLIError() const
{
        return (m_cliError);
}
void  RASLoggingMessage::setCLIError(const  string &cliError)
{
        m_cliError = cliError;
}
bool  RASLoggingMessage::getCLIOutput() const
{
        return (m_cliOutput);
}
void  RASLoggingMessage::setCLIOutput(const bool &boolData)
{
        m_cliOutput = boolData;
}

    void  RASLoggingMessage::setSsMsg(const string &msg)
    {
        m_ssMsg = msg;
    }

    string  RASLoggingMessage::getSsMsg() const
    {
        return (m_ssMsg);
    }

    void  RASLoggingMessage::setSsPercentageOfCompletion(const UI32 &percentage)
    {
        m_percentageOfCompletion = percentage;
    }

    UI32  RASLoggingMessage::getSsPercentageOfCompletion() const
    {
        return (m_percentageOfCompletion);
    }

    void  RASLoggingMessage::setSsStatus(const UI32 &status)
    {
         switch (status)
         {
             case SS_STATUS_COMPLETED:
                 m_ssStatus = ras_extensions_completed;
                 break; 
    
             case SS_STATUS_INPROGRESS:
                 m_ssStatus = ras_extensions_inProgress;
                 break; 
    
             case SS_STATUS_ERROR:
                 m_ssStatus = ras_extensions_status_error;
                 break; 
    
             default:
                 m_ssStatus = ras_extensions_status_unknown;
                 break; 
         }
    
         return;
    }
    
    UI32  RASLoggingMessage::getSsStatus() const
    {
         return (m_ssStatus);
    }

bool  RASLoggingMessage::getNetConf() const
{
        return (m_netconf);
}
void  RASLoggingMessage::setNetConf(const bool &boolData)
{
        m_netconf = boolData;
}

bool RASLoggingMessage::getSync() const
{
	return (m_sync);
}

void RASLoggingMessage::setSync(const bool &boolData)
{
	m_sync = boolData;
}

bool RASLoggingMessage::getFFDC() const
{
	return (m_ffdc);
}   

void RASLoggingMessage::setFFDC(const bool &boolData)
{
	m_ffdc = boolData;
}

//  RASLoggingMessage Implementation  - END                 //
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
//  RASNetConfMessage Implementation - BEGIN                //

RASNetConfMessage::RASNetConfMessage():PrismMessage(RASLocalObjectManager::getPrismServiceId(), RAS_NETCONF)
{
}

RASNetConfMessage::RASNetConfMessage(NCSubCmd cmd):PrismMessage(RASLocalObjectManager::getPrismServiceId(), RAS_NETCONF)
{
    mCmd = cmd;
}

RASNetConfMessage::~RASNetConfMessage()
{
}

void
RASNetConfMessage::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization();
    addSerializableAttribute(new AttributeSI32((SI32*)&mCmd, "Cmd"));
}

NCSubCmd
RASNetConfMessage::getCmd() const
{
    return (mCmd);
}

void
RASNetConfMessage::setCmd(const NCSubCmd &cmd)
{
    mCmd = cmd;
}

//  RASNetConfMessage Implementation - END                     //
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//  RASLogShowMessage Implementation - BEGIN                //

    RASLogShowMessage::RASLogShowMessage ()
        : PrismMessage (RASLogLocalObjectManager::getPrismServiceId (),
                        RASLOGSHOW)
    {
    }

    RASLogShowMessage::RASLogShowMessage (const string &switchid,
                                          const UI32 &number_of_latest_events,
                                          const UI32 &number_of_entries)
        : PrismMessage (RASLogLocalObjectManager::getPrismServiceId (),
                        RASLOGSHOW),
        m_switchid    (switchid),
        m_number_of_latest_events    (number_of_latest_events),
        m_number_of_entries    (number_of_entries)
    {
    }

    RASLogShowMessage::~RASLogShowMessage ()
    {
    }

    void  RASLogShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeUI32(&m_number_of_latest_events,
                                      "number_of_latest_events"));
        addSerializableAttribute (new AttributeUI32(&m_number_of_entries,
                                      "number_of_entries"));
    }

    void  RASLogShowMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  RASLogShowMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  RASLogShowMessage::setNumber_of_latest_events(
                                 const UI32 &number_of_latest_events)
    {
        m_number_of_latest_events  =  number_of_latest_events;
    }

    UI32  RASLogShowMessage::getNumber_of_latest_events() const
    {
        return (m_number_of_latest_events);
    }

    void  RASLogShowMessage::setNumber_of_entries(const UI32 &number_of_entries)
    {
        m_number_of_entries  =  number_of_entries;
    }

    UI32  RASLogShowMessage::getNumber_of_entries() const
    {
        return (m_number_of_entries);
    }

    void RASLogShowMessage::getOutputBuffer (UI32 bufferNum, UI32 &size,
                                             netconfEvent_t *&pBuffer)
    {
            pBuffer   =   (netconfEvent_t *) findBuffer(bufferNum, size);
    }

    void RASLogShowMessage::transferOutputBuffer(netconfEvent_t *pMsg,
                                                            UI32 size, UI32 i)
    {
        WaveMessageStatus status; 

        status = addBuffer(i, size, (void *) pMsg);
        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_ERROR,
            "RASLogShowMessage::transferOutputBuffer : addBuffer is failed: " +
            status);
        }
    }

    int RASLogShowMessage::getIndex(void *pData)
    {
        rls_msg_t *pRasLogBuf = (rls_msg_t *) pData;

        if (!pRasLogBuf)
            return (0);

        return (ntohl(RLS_GET_ESEQ(pRasLogBuf)));
    }

    string RASLogShowMessage::getMessageId(void *pData)
    {
        rls_msg_t *pRasLogBuf = (rls_msg_t *) pData;
        string str;

        if (!pRasLogBuf)
            return (str);

        if (strcmp(RAS_CAT_NAME, RLMD_GET_CATNAME(
                                 RLS_GET_MSGDEF(pRasLogBuf))) == 0)
        {
            str.assign(RLMD_GET_MSG_ID_STRING((RLS_GET_MSGDEF(pRasLogBuf))));
        } else {
            str.assign(RLMD_GET_CATNAME(RLS_GET_MSGDEF(pRasLogBuf)));
            str.append(":");
            str.append(RLMD_GET_MSG_ID_STRING(RLS_GET_MSGDEF(pRasLogBuf)));
        }
    
        return (str);
    }

    string RASLogShowMessage::getDateAndTimeInfo(void *pData)
    {
        rls_msg_t *pRasLogBuf = (rls_msg_t *) pData;
        time_t cur_time = 0;
        struct tm *pTm = NULL;
        char buf[RASLOG_TIME_LEN+1];
        string str("");

        if (!pRasLogBuf)
            return (str);

        cur_time = (time_t) ntohl(RLS_GET_TIMESEC(pRasLogBuf));
        pTm = gmtime(&cur_time);
        strftime(buf, RASLOG_TIME_LEN, RASLOG_TIME_FMT, pTm);
        str.assign(buf);

        bzero(buf, sizeof (buf));
        snprintf(buf, sizeof (buf), ":%06d",
                     ntohl(RLS_GET_TIMEUSEC(pRasLogBuf)));
        str.append(buf);
    
        return (str);
    }

    int RASLogShowMessage::getSeverity(void *pData)
    {
        rls_msg_t *pRasLogBuf = (rls_msg_t *) pData;
        int ret = ras_extensions_severity_unknown;
    
        if (!pRasLogBuf)
            return (ret);

        switch (ntohl(RLMD_GET_SEVERITY(RLS_GET_MSGDEF(pRasLogBuf))))
        {
            case RASLOG_CRITICAL:
                ret = ras_extensions_critical;
                break;
    
            case RASLOG_ERROR:
                ret = ras_extensions_severity_error;
                break;
    
            case RASLOG_WARNING:
                ret = ras_extensions_severity_warning;
                break;
    
            case RASLOG_INFO:
                ret = ras_extensions_informational;
                break;
    
            default:
                ret = ras_extensions_severity_unknown;
                break;
        }

        return (ret);
    }

    string RASLogShowMessage::getMessage(void *pData)
    {
        rls_msg_t *pRasLogBuf = (rls_msg_t *) pData;
        int rc;
        char buf[MAX_MESSAGE_LEN + 1] = {0};
        string str("");

        if (!pRasLogBuf)
            return (str);

        rc = raslogm_msg2str(pRasLogBuf, buf, sizeof (buf));

        if (rc == RASLOG_OK)
        {
            str.assign(buf);
        }
    
        return (str);
    }

    int RASLogShowMessage::getMessageFlag(void *pData)
    {
        rls_msg_t *pRasLogBuf = (rls_msg_t *) pData;
        int attr, ret = ras_extensions_message_flag_unknown;
    
        if (!pRasLogBuf)
            return (ret);

        attr = ntohl(RLMD_GET_ATTR(RLS_GET_MSGDEF(pRasLogBuf)));
        if (attr & RLMD_ATTR_FFDC)
        {
            ret = ras_extensions_ffdc;
        } else if (attr & RLMD_ATTR_VCS) {
            ret = ras_extensions_vcs;
        }
    
        return (ret);
    }

	int RASLogShowMessage::getLogType(void *pData)
	{
		rls_msg_t *pRasLogBuf = (rls_msg_t *) pData;
		int attr, ret = ras_extensions_log_type_system;

		if (!pRasLogBuf)
			return (ret);

		attr = ntohl(RLMD_GET_ATTR(RLS_GET_MSGDEF(pRasLogBuf)));
		if (attr & RLMD_ATTR_DCE)
		{
			ret = ras_extensions_log_type_dce;
		}

		return (ret);
	}

    const char *RASLogShowMessage::getSwitchOrChassisName(void *pData)
    {
        rls_msg_t *pRasLogBuf = (rls_msg_t *) pData;
        const char *pTmp = "?";

        if (!pRasLogBuf)
            return (pTmp);

        if (RLS_GET_SWNAME(pRasLogBuf)[0] != '\0')
        {
            pTmp = RLS_GET_SWNAME(pRasLogBuf);
        }
    
        return (pTmp);
    }

//  RASLogShowMessage Implementation - END                  //
//////////////////////////////////////////////////////////////

RASLCCLIMsg::RASLCCLIMsg ()
	: PrismMessage (RASLocalObjectManager::getPrismServiceId (), RASLCCLI)
{
}

RASLCCLIMsg::RASLCCLIMsg (const string &Arg, const UI32 &CliCode)
	: PrismMessage (RASLocalObjectManager::getPrismServiceId (), RASLCCLI),
        m_Arg    (Arg),
        m_CliCode   (CliCode)
{
}

RASLCCLIMsg::~RASLCCLIMsg ()
{
}

void  RASLCCLIMsg::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeUI32(&m_CliCode,"CliCode"));
    addSerializableAttribute (new AttributeString(&m_Arg,"Arg"));
}

void  RASLCCLIMsg::setArg(const string &Arg)
{
    m_Arg  =  Arg;
}

string  RASLCCLIMsg::getArg() const
{
        return (m_Arg);
}
    
void  RASLCCLIMsg::setCliCode(const UI32 &CliCode)
{
    m_CliCode  =  CliCode;
}

UI32  RASLCCLIMsg::getCliCode() const
{
    return (m_CliCode);
}

void RASLCCLIMsg::addOutData(const string &data)
{
    m_outData += data;
}

string RASLCCLIMsg::getOutData() const
{
    return (m_outData);
}

void RASLCCLIMsg::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
						  UI32 &size, void *&pBuffer)
{
        pBuffer   =   findBuffer(bufferNum, size);
}
    
const void *RASLCCLIMsg::getCStructureForInputs ()
{
        lccli_in_msg_t *pInput = new lccli_in_msg_t;

        strncpy (pInput->argv, m_Arg.c_str (), LCCLI_INPUT__MAX);
        pInput->cli_code    = m_CliCode;

        return (pInput);
}

void RASLCCLIMsg::loadOutputsFromCStructure (const void *pOutputCStructure)
{
        lccli_out_msg_t *msg = (lccli_out_msg_t *)pOutputCStructure;

	tracePrintf(TRACE_LEVEL_INFO, "RASLCCLIMsg:data len=%d",  msg->out_buf_len);

	// duplicate data to buf for server to process
	// m->getDuplicateInterfaceOutputBuffer(LCCLI_MSG, size, data);
	// Call addBuffer(...., false) If the data is not included as part of lccli_out_msg_t
	// Expect pOutputCStructure should include lccli_out_msg_t and real data.
        addBuffer (RASLCCLI, sizeof(lccli_out_msg_t) + msg->out_buf_len,
		   pOutputCStructure, false);
}

//////////////////////////////////////
//  RASRasmanMessage definition     //
//////////////////////////////////////
void  RASRasmanMessage::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization ();
}

RASRasmanMessage::RASRasmanMessage ()
        : PrismMessage (RASLocalObjectManager::getPrismServiceId (), RASMANCLI)
{
}

RASRasmanMessage::~RASRasmanMessage ()
{
}

string  RASRasmanMessage::getCmd() const
{
        return (m_cmd);
}
void  RASRasmanMessage::setCmd(const string &cmd)
{
        m_cmd = cmd;
}
void  RASRasmanMessage::setMappedId(const  UI32 &mappedId)
{
        m_mappedId = mappedId;
}

UI32 RASRasmanMessage::getMappedId() const
{
        return (m_mappedId);
}

}

