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

#ifndef RASMANAGEDOBJECT_H
#define RASMANAGEDOBJECT_H

// #include <time.h>
#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Types/IpVxAddress.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "RAS/Global/RASObjectManager.h"
//#include "RAS/Global/RASObjectManagerTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class RASObjectManager;

class RASManagedObject : public DcmManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
	virtual void setupKeys();
    public :
	RASManagedObject (RASObjectManager *pRASObjectManager);
        virtual ~RASManagedObject();
        static  string getClassName();
    // Now the data members
        IpVxAddress  getSyslogIp() const;
        void  setSyslogIp (const IpVxAddress &syslogIp);
        UI16  getSyslogPort() const;
        void  setSyslogPort (const UI16 &port);
        bool  getSyslogSecureMode() const;
        void  setSyslogSecureMode (const bool &mode);
    private :
	    string  m_support;
	    IpVxAddress m_syslogIp;
        UI16    m_port;
        bool    m_isSecure;
	    UI32    m_auditLogclass;
    protected :
    public :
};


class RASGlobalConfigManagedObject : public DcmManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
	RASGlobalConfigManagedObject (RASObjectManager *pRASObjectManager);
        virtual          ~RASGlobalConfigManagedObject ();
        static            string getClassName              ();
    // Now the data members
	UI32 getConsole()  const;
		void setConsole(const UI32 &console);
	bool getFFDC()  const;
		void setFFDC(bool ffdc);
    private :
	    string m_support;
		UI32 m_console;
		bool m_isFFDC;
    protected :
    public :
};

class RASAutoUploadManagedObject : public DcmManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
	RASAutoUploadManagedObject (RASObjectManager *pRASObjectManager);
        virtual          ~RASAutoUploadManagedObject ();
        static            string getClassName              ();
    // Now the data members
	
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

    private :
		string m_support;
		bool m_isAutoSupport;
		IpVxAddress m_hostip;
		string  m_username; 
	    string  m_directory;
	    string  m_password;
		string	m_proto;
	protected :
	public :
};

class RASLOGConfigureManagedObject : public DcmManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
	virtual void setupKeys();
    public :
		RASLOGConfigureManagedObject (RASObjectManager *pRASObjectManager);
        virtual          ~RASLOGConfigureManagedObject ();
        static            string getClassName              ();
	   
		string getMsgId() const;
		void setMsgId(const string &msgid);
		UI32 getSeverity() const;
		void setSeverity(const UI32 &severity);
		bool getSyslogEnable() const;
		void setSyslogEnable(const bool &syslog);
		bool getMessageDisable() const;
		void setMessageDisable(const bool &disable);
		
    private :
    protected :
    public :
		string m_msgId;
		UI32 m_severity;
		bool m_syslog;
		bool m_msgDisable;
		string m_support;
};

class RASMODConfigureManagedObject : public DcmManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
	virtual void setupKeys();
    public :
		RASMODConfigureManagedObject (RASObjectManager *pRASObjectManager);
        virtual          ~RASMODConfigureManagedObject ();
        static            string getClassName              ();
	   
		string getModId() const;
		void setModId(const string &modid);
		bool getModuleDisable() const;
		void setModuleDisable(const bool &disable);
		
    private :
    protected :
    public :
		string m_modId;
		bool m_modDisable;
		string m_support;
};

class RASAuditManagedObject : public DcmManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
	virtual void setupKeys();
    public :
	RASAuditManagedObject (RASObjectManager *pRASObjectManager);
	RASAuditManagedObject (RASObjectManager *pRASObjectManager, const RASAuditClassTypes &auditLogclass);
        virtual          ~RASAuditManagedObject ();
        static            string getClassName              ();
    // Now the data members
	    UI32  getAuditLogclass() const;
            void setAuditLogclass (const UI32 &auditLogclass);
    private :
	    string m_support;
	    UI32 m_auditLogclass;
    protected :
    public :
};

class RASSysFcManagedObject : public DcmManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
        RASSysFcManagedObject (RASObjectManager *pRASObjectManager);
        RASSysFcManagedObject (RASObjectManager *pRASObjectManager, const RASSysFacilityTypes &syslogFacility);
        virtual          ~RASSysFcManagedObject ();
        static            string getClassName              ();
    // Now the data members
            UI32  getSysFacility() const;
            void setSysFacility (const UI32 &syslogFacility);
    private :
            string m_support;
            UI32 m_syslogFacility;
    protected :
    public :
};

}

#endif // RASMANAGEDOBJECT_H
