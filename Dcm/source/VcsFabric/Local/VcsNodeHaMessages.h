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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Leifang Hu                                    *
 ***************************************************************************/
#ifndef VCSNODEHAMESSAGE_H
#define VCSNODEHAMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"


using namespace WaveNs;

namespace DcmNs
{
class VcsNodeHaObjectManager;

class VcsNodeNotifyStartSyncDumpMessage : public ManagementInterfaceMessage
{
 public:
    VcsNodeNotifyStartSyncDumpMessage (const string &peerHaIpAddress, const SI32 &peerHaIpPort);
    virtual ~VcsNodeNotifyStartSyncDumpMessage();

    void setPeerHaIpAddress(const string& peerHaIpAddress);
    void setPeerHaIpPort(const SI32& peerHaIpPort);

    string getPeerHaIpAddress() const;
    SI32 getPeerHaIpPort() const;
 protected:
 private:
    virtual void setupAttributesForSerialization       ();

 public:
 protected:
 private:
    string m_peerHaIpAddress;
    SI32 m_peerHaIpPort;
};

class VcsNodeNotifyEndSyncDumpMessage : public ManagementInterfaceMessage
{
 public:
             VcsNodeNotifyEndSyncDumpMessage (const string &peerHaIpAddress, const SI32 &peerHaIpPort);
    virtual ~VcsNodeNotifyEndSyncDumpMessage ();

    void     setPeerHaIpAddress              (const string& peerHaIpAddress);
    void     setPeerHaIpPort                 (const SI32& peerHaIpPort);

    string   getPeerHaIpAddress              () const;
    SI32     getPeerHaIpPort                 () const;
 protected:
 private:
    virtual void setupAttributesForSerialization       ();

 public:
 protected:
 private:
    string m_peerHaIpAddress;
    SI32   m_peerHaIpPort;
};

class VcsNodeHaPeerEnqMessage : public ManagementInterfaceMessage
{
	private:
		VcsNodeHaPeerEnqMessage			();
		VcsNodeHaPeerEnqMessage 			(const string &objectManagerName, const UI32 &operationCode);
		
		void 	setupAttributesForSerialization ();
		UI32 	getReceiverCpState ()	const;
		void 	setReceiverCpState (const UI32 &cpState);
		UI32 	getSenderCpState ()	const;
		void 	setSenderCpState (const UI32 &cpState);
		
		string	getReceiverIpAddress () const;
		void	setReceiverIpAddress (const string &peerIpAddress);
		UI32	getReceiverPort () const;
		void	setReceiverPort (const UI32 &peerPort);
		string  getSenderIpAddress () const;
		void 	setSenderIpAddress (const string &localIpAddress);
		UI32 	getSenderPort () const;
		void	setSenderPort (const UI32 &localPort);
		
	public:
	    virtual  ~VcsNodeHaPeerEnqMessage ();
		
	private:
		UI32 	m_receiverCpState;
		string 	m_receiverIpAddress;
		UI32 	m_receiverPort;
		UI32 	m_senderCpState;
		string	m_senderIpAddress;
		UI32 	m_senderPort;

	friend class VcsNodeHaObjectManager;	
	friend class VcsNodeHaHeartbeatMessage;
};

class VcsNodeHaHeartbeatMessage : public VcsNodeHaPeerEnqMessage
{
	private:
			VcsNodeHaHeartbeatMessage 			();
			VcsNodeHaHeartbeatMessage 			(const UI32 &messageCount);
			VcsNodeHaHeartbeatMessage 			(const UI32 &messageCount, const string &objectManagerName, const UI32 &operationCode);
	void 	setupAttributesForSerialization 			();
	void 	setMessageCount 						(const UI32 &messageCount);
	UI32 	getMessageCount 						() const;

public:
	virtual ~VcsNodeHaHeartbeatMessage 			();

private:

	UI32 	m_messageCount;
	
	friend class VcsNodeHaObjectManager;		
};

class VcsNodeCpStateChangeClientMessage : public DcmManagementInterfaceMessage
{
    private:
    protected:
        virtual void setupAttributesForSerialization();
    public:
		
					VcsNodeCpStateChangeClientMessage (UI32 msgOpCode);
        			VcsNodeCpStateChangeClientMessage (UI32 msgOpCode, string &clientName);
        virtual    ~VcsNodeCpStateChangeClientMessage ();
		void 		setCpState (const UI32 &cpState);
		UI32 		getCpState ()  const;
		string 		getClientName () const;
		void		setClientName (const string &clientName);
		UI32 		getTryCount () const;
		void		setTryCount (const UI32 &tryCount);
        virtual const void *getCStructureForInputs    ();
		virtual void loadOutputsFromCStructure (const void *pOutputCStructure);
// Now the data members

    public:
		UI32  	m_cpState;
		string 	m_clientName;
		UI32 	m_tryCount;
};

}
#endif
