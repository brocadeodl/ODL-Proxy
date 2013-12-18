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
 *   Author : chenj                                                        *
 **************************************************************************/

#ifndef RASLOGGINGMESSAGE_H
#define RASLOGGINGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/IpVxAddress.h"
#include "RASTypes.h"
#include "ClientInterface/RAS/RASApp.h"
#include "ClientInterface/RAS/RASClient.h"
#include "RAS/Global/RASObjectManagerTypes.h"

#include <raslog/raslogd.h>
#include <raslog/raslogm.h>
#include <utils/support.h>
#include <hasm/smi.h>
#include <hasm/hasm.h>
#include <ha/ham_api.h>

using namespace WaveNs;

namespace DcmNs
{

//////////////////////////////////////
//  RASLoggingMessage definition    //
//////////////////////////////////////

class RASLoggingMessage : public PrismMessage
{
    private:
	void setupAttributesForSerialization();
        protected:
        public:
            RASLoggingMessage ();
	    RASLoggingMessage (const ObjectId &LoggingOId);
	    RASLoggingMessage (const ObjectId &LoggingOId, const string &data, UI32 &userWyserTag);
	    RASLoggingMessage (const ObjectId &LoggingOId, const bool &data, UI32 &userWyserTag);
		RASLoggingMessage (const ObjectId &LoggingOId, const bool &data, const bool &data1, UI32 &userWyserTag);
		RASLoggingMessage (const ObjectId &LoggingOId, const bool &data, UI32 &dataSupport, UI32 &userWyserTag);
		RASLoggingMessage (const ObjectId &LoggingOId, const string &data, UI32 &dataSupport, UI32 &userWyserTag);
	    RASLoggingMessage (const ObjectId &LoggingOId, const Severity &enumData, UI32 &userWyserTag);
		//Severity setting
	    RASLoggingMessage (const ObjectId &LoggingOId, const UI32 &enumData, const string &msgid, UI32 &userWyserTag);
		//Syslog enable
		RASLoggingMessage (const ObjectId &LoggingOId, UI32 &userWyserTag, const bool &data, const string &msgid);
		//Message disable
		RASLoggingMessage (const ObjectId &LoggingOId, const bool &data, const string &msgid, UI32 &userWyserTag);
		//Module disable
		RASLoggingMessage (const ObjectId &LoggingOId, const string &modid, const bool &data, UI32 &userWyserTag);
	    RASLoggingMessage (const ObjectId &LoggingOId, UI32 &userWyserTag);
	    RASLoggingMessage (const ObjectId &LoggingOId, UI32 &auditLogclass, UI32 &userWyserTag);
	    RASLoggingMessage (const ObjectId &LoggingOId, const RASSysFacilityTypes &sysLogfacility, UI32 &isSingleton, UI32 &userWyserTag);
//	    RASLoggingMessage (const ObjectId &LoggingOId, const bool &data);
	    // RASLoggingMessage (const ObjectId &LoggingOId, const vcsmsg_t &vcsmsg);
	    RASLoggingMessage (const ObjectId &LoggingOId,
                           ss_progress_t &ss_status);

            virtual    ~RASLoggingMessage ();
	    ObjectId  getObjectId() const;
            void setRaslog(const string &raslog);
            string getRaslog()  const;
	    void setData(const string &Data);
	    string getData() const;
	    void setEnumData(const Severity &enumData);
	    Severity getEnumData() const;
	    void setBoolData(const bool &Data);
	    bool getBoolData() const;
	    void setWyserUserTag(const UI32 &userWyserTag);
	    UI32 getWyserUserTag() const;
	    void setErrdump(const string &errdump);
	    string getErrdump()  const;
	    void setCmd(const string &cmd);
	    string getCmd() const;
	    void setMappedId(const UI32 &mappedId);
	    UI32 getMappedId() const;
		void setSwitchid(const string &switchid);
            string getSwitchid()  const;
	    void setSyslogIp(const IpVxAddress &syslogIp);
            IpVxAddress getSyslogIp()  const;
		void setAutoSupport(const bool &support);
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
		void setMsgId(const string &msgid);
			string getMsgId() const;
		void setModId(const string &modid);
			string getModId() const;
		void setSeverity(const UI32 &severity);
			UI32 getSeverity() const;
	    void setSyslogEnable(const bool &syslog);
    	    bool getSyslogEnable() const;
	    void setMessageDisable(const bool &msgDisable);
    	    bool getMessageDisable() const;
	    void setModuleDisable(const bool &modDisable);
    	    bool getModuleDisable() const;
		void setAuditLogclass(const UI32 &auditLogclass);
            UI32 getAuditLogclass()  const;
	    void setSysLogfacility(const RASSysFacilityTypes &sysLogfacility);
            RASSysFacilityTypes getSysLogfacility()  const;
	    string getVcsmsg() const;
            void setVcsmsg (const string &msg);
	    string getCLIStart() const;
            void setCLIStart(const string &msg);
	    string getCLIEnd() const;
            void setCLIEnd(const string &msg);
	    string getCLIError() const;
            void setCLIError(const string &msg);
	    bool getCLIOutput() const;
	    void setCLIOutput(const bool &CLIOutput);

	    void  setSsStatus(const UI32 &status);
	    UI32  getSsStatus() const;
	    void  setSsMsg(const string &msg);
	    string  getSsMsg() const;
	    void  setSsPercentageOfCompletion(const UI32 &percentage);
	    UI32  getSsPercentageOfCompletion() const;
	    bool  getNetConf() const;
	    void  setNetConf(const bool &);
		bool getSync() const;
		void setSync(const bool &boolData);
		bool getFFDC() const;
			void setFFDC(const bool &boolData);

// Now the data members

        private:
	    ObjectId m_ObjectId;
	    UI32    m_mappedId;
	    string  m_raslog;
	    UI32    m_userWyserTag;
	    Severity    m_enumData;
	    string  m_data;
	    bool    m_boolData;
	    bool    m_ffdc;
	    string  m_errdump;
	    string  m_cmd;
	    IpVxAddress m_syslogIp;
		bool    m_isAutoSupport;
		IpVxAddress  m_hostip;
        string  m_username;
		string	m_proto;
        string  m_directory;
        string  m_password;
		UI32	m_severity;
		bool    m_syslog;
		bool    m_msgDisable;
    	bool    m_modDisable;
		string	m_msgid;
		string	m_modid;
	    UI32 m_auditLogclass;
	    RASSysFacilityTypes m_sysLogfacility;
	    string  m_vcsmsg;
	    string  m_cliStart;
	    string  m_cliEnd;
	    string  m_cliError;
	    bool   m_cliOutput;

	    // Support Save Status
	    UI32      m_ssStatus;
	    string   m_ssMsg;
	    UI32   m_percentageOfCompletion;

        bool    m_netconf;
		bool m_sync;

        protected:
        public:
		string m_switchid;
          
};

class RASLCCLIMsg : public PrismMessage
{
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RASLCCLIMsg ();
            RASLCCLIMsg (const string &Arg, const UI32 &CliCode);
            virtual ~RASLCCLIMsg ();
            void setArg(const string &Arg);
            string getArg() const;
            void setCliCode(const UI32 &CliCode);
            UI32 getCliCode() const;
            void addOutData(const string &data);
            string getOutData() const;


            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void 	*pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            string m_Arg;
            UI32 m_CliCode;
            string m_outData;
};




//////////////////////////////////////
//  RASNetConfMessage definition    //
//////////////////////////////////////

class RASNetConfMessage : public PrismMessage
{
        private:
            void setupAttributesForSerialization();

        protected:

        public:
            RASNetConfMessage();
            RASNetConfMessage(NCSubCmd cmd);
            virtual ~RASNetConfMessage();
            void setCmd(const NCSubCmd &cmd);
            NCSubCmd getCmd() const;

        private:
            NCSubCmd    mCmd;
};

// Chassis show output structure
// this will be followed by a 'frucnt'  emChassisInfo_t structures
typedef struct _chassisShowOut {
    UI32                maxport;
    UI32                bd;         /* SWBD number */
    UI32                frucnt;
    SI32                ha_state;
    UI32                slotETHPortCnt[SMI_SLOTS];
    SI32                slotType[SMI_SLOTS];
    SI32                slotId[SMI_SLOTS];
} chassisShowOut_t;

//////////////////////////////////////
//  RASLogShowMessage definition    //
//////////////////////////////////////

    typedef struct _netconfEvent_s {
        int  index;
        char id[MAX_MESSAGE_VALUE_LEN + 8];
        int  extSeqNum;
        char timeInfo[RASLOG_TIME_LEN + 1];
        int  severity;
		int	 log_type;
        char descr[MAX_MESSAGE_LEN];
        //char descr[MAX_MESSAGE_LEN + RASEVT_MSG_ARG_LEN];
        char name[RLS_SWNAME];
        int  flag;
    } netconfEvent_t;

    class RASLogShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RASLogShowMessage ();
            RASLogShowMessage (const string &switchid,const UI32 &number_of_latest_events,const UI32 &number_of_entries);
            virtual    ~RASLogShowMessage ();
            void setSwitchid(const string &switchid);
            string getSwitchid()  const;
            void setNumber_of_latest_events(const UI32 &number_of_latest_events);
            UI32 getNumber_of_latest_events()  const;
            void setNumber_of_entries(const UI32 &number_of_entries);
            UI32 getNumber_of_entries()  const;

            void getOutputBuffer (UI32 bufferNum, UI32 &size,
                                             netconfEvent_t *&pBuffer);
            void transferOutputBuffer(netconfEvent_t *pMsg, UI32 size, UI32 i);

            // RASLog data methods
            int getIndex(void *pData);
            string getMessageId(void *pData);
            string getDateAndTimeInfo(void *pData);
            int getSeverity(void *pData);
            string getMessage(void *pData);
            int getMessageFlag(void *pData);
			int getLogType(void *pData);
            const char *getSwitchOrChassisName(void *pData);


// Now the data members

        private:
        protected:
        public:
            string  m_switchid;
            UI32  m_number_of_latest_events;
            UI32  m_number_of_entries;
    };
//////////////////////////////////////
//  RASRasmanMessage definition    //
//////////////////////////////////////

class RASRasmanMessage : public PrismMessage
{
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RASRasmanMessage ();
            void setCmd(const string &cmd);
            void setMappedId(const UI32 &mappedId);
            string getCmd() const;
        UI32 getMappedId() const;
            virtual    ~RASRasmanMessage ();
    private:
        string  m_cmd;
        UI32    m_mappedId;

};

} // namespace DcmNs

#endif
