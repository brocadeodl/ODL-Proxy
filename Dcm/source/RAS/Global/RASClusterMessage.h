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

#ifndef RASCLUSTERMESSAGE_H
#define RASCLUSTERMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "RAS/Local/RASTypes.h"
#include "ClientInterface/RAS/RASApp.h"
#include "RAS/Global/RASObjectManagerTypes.h"
#include "Framework/Types/IpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{

//////////////////////////////////////
//  RASClusterMessage definition    //
//////////////////////////////////////

class RASClusterMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
	RASClusterMessage		();
	RASClusterMessage (const UI32 mappedId);
	RASClusterMessage (const UI32 startMappedId, const UI32 endMappedId);
	RASClusterMessage (const string &data, const UI32 &userWyserTag);
	virtual        ~RASClusterMessage				();
	void setErrdump(const string &errdump);
	string getErrdump()  const;
	void setCmd(const string &cmd);
	string getCmd() const;
	void setData(const string &data);
	string getData()  const;
	string getVcsmsg() const;
        void setVcsmsg (const string &vcsmsg);
    void setSyslogIp(IpVxAddress &syslogIp);
    IpVxAddress getSyslogIp()  const;
    void setSyslogPort(UI16 &port);
    UI16 getSyslogPort()  const;
    void setSyslogSecureMode(const bool &mode);
    bool getSyslogSecureMode()  const;
	void setBoolData(const bool &Data);
		bool getBoolData() const;
	void setEnumData(const Severity &enumData);
		Severity getEnumData() const;
	void setAutoSupport(bool support);
		bool getAutoSupport()  const;
	void setHostIp(const IpVxAddress &hostIp);
		IpVxAddress getHostIp() const;	
	void setUser(const string &username);
		string getUser() const;
	void setProtocol(const string &proto);
		string getProtocol() const;
	void setDirectory(const string &dir);
		string getDirectory() const;
	void setPassword(const string &password);
		string getPassword() const;
	void setSeverity(const UI32 &severity);
		UI32 getSeverity() const;
	void setSyslogEnable(bool syslog);
		bool getSyslogEnable() const;
	void setMessageDisable(bool msgDisable);
		bool getMessageDisable() const;
	void setModuleDisable(bool modDisable);
		bool getModuleDisable() const;
	void setMsgId(const string &msgid);
		string getMsgId() const;
	void setModId(const string &modid);
		string getModId() const;
	void setAuditLogclass(UI32 &auditLogclass);
        UI32 getAuditLogclass()  const;
	void setSysLogfacility(RASSysFacilityTypes &syslogFacility);
		RASSysFacilityTypes getSysLogfacility()  const;
	void setWyserUserTag(const UI32 &userWyserTag);
	UI32 getWyserUserTag() const;
	void setMappedId(const UI32 &mappedId);
	UI32 getMappedId() const;
	void setOrigMappedId(const UI32 &mappedId);
	UI32 getOrigMappedId() const;
	string getCLIStart() const;
	void setCLIStart(const string &msg);
	string getCLIEnd() const;
	void setCLIEnd(const string &msg);
	string getCLIError() const;
	void setCLIError(const string &msg);
	bool getCLIOutput() const;
	void setCLIOutput(const bool &CLIOutput);
	void setCLITimeout(const UI32 &CLITimeout);
	UI32 getCLITimeout() const;
	bool getNetConf() const;
	void setNetConf(const bool &);
	bool getSync() const;
	void setSync(const bool &boolData);
	// Now the data members

    private :
    UI32   m_auditLogclass;
    RASSysFacilityTypes m_syslogFacility;
    UI32   m_startMappedId;
    UI32   m_endMappedId;
    UI32   m_mappedId;
    string m_support;
    UI32    m_userWyserTag;
    string  m_data;
    string  m_cmd;

    string m_errdump;
    UI32   m_origMappedId;
 
    string  m_vcsmsg;
    IpVxAddress m_syslogIp;
    bool    m_isSecure;
    UI16    m_port;
	IpVxAddress	m_hostip;
	bool    m_isAutoSupport;
	string  m_username;
	string	m_proto;
	string  m_directory;
	string  m_password;
	bool	m_boolData;
	Severity    m_enumData;
	UI32	m_severity;
	bool	m_syslog;
	bool	m_msgDisable;
	bool	m_modDisable;
	string	m_msgid;
	string	m_modid;
    string  m_cliStart;
    string  m_cliEnd;
    string  m_cliError;
    bool   m_cliOutput;
    UI32    m_cliTimeout;
    bool    m_netconf;
	bool m_sync;

    protected :
    public :
};

/////////////////////////////////////////////
//  RASClusterNetConfMessage definition    //
/////////////////////////////////////////////

class RASClusterNetConfMessage : public PrismMessage
{
        private:
            void setupAttributesForSerialization();

        protected:

        public:
            RASClusterNetConfMessage();
            RASClusterNetConfMessage(NCSubCmd cmd);
            virtual ~RASClusterNetConfMessage();
            void setCmd(const NCSubCmd &cmd);
            NCSubCmd getCmd() const;

        private:
            NCSubCmd    mCmd;
};

}

#endif // RASCLUSTERMESSAGE
