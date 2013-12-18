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


#ifndef VCENTEREVENTSMESSGE_H_
#define VCENTEREVENTSMESSGE_H_

using namespace std;
#include <string.h>
#include "Framework/Types/Types.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "VirtualDb/Domain/DomainInclude.h"
#include "EventDefines.h"

#include <string>
#include <vector>
using namespace WaveNs;
namespace DcmNs {



    class EventMessage: public ManagementInterfaceMessage {
        protected:
             virtual void setupAttributesForSerialization();
 
        public:
            EventMessage(const UI32 &operationId);
            virtual ~EventMessage();

            UI32                m_opcode;
            string              m_vcenter;
    };

    class LocalEventMessage: public PrismMessage {
        protected:
             virtual void setupAttributesForSerialization();
 
        public:
            LocalEventMessage(const UI32 &operationId);
            virtual ~LocalEventMessage();
			virtual void copy(EventMessage *message);
            UI32                m_opcode;
            string              m_vcenter;
	};

    class DatacenterEventMessage: public EventMessage {
    protected:
        virtual void setupAttributesForSerialization();
        public:
            DatacenterEventMessage();
            virtual ~DatacenterEventMessage();

        public:
        	DataCenterDomain    m_domain;
    };

    class LocalDatacenterEventMessage : public LocalEventMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LocalDatacenterEventMessage ();
            virtual    ~LocalDatacenterEventMessage ();
        	void copy(DatacenterEventMessage* pDiscoverDistributeMessage);
        	void copyDomain();
        public:
        	  string              id;
        	  string              name;
        	DataCenterDomain    m_domain;
    };

    class VswitchEventMessage: public EventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
            VswitchEventMessage();
            virtual ~VswitchEventMessage();

            VsDomain            m_domain;
    };

    class LocalVswitchEventMessage: public LocalEventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
            LocalVswitchEventMessage();
            virtual ~LocalVswitchEventMessage();
			virtual void copy(VswitchEventMessage *message);
        	void copyDomain();
            std::string name;
            std::string host_id;
            std::string host;
            std::string dc_id;
            VsDomain            m_domain;
    };
    class VmnicEventMessage: public EventMessage {
        protected:
            virtual void setupAttributesForSerialization();

        public:
            VmnicEventMessage();
            virtual ~VmnicEventMessage();

            VmnicDomain         m_domain;
    };

    class LocalVmnicEventMessage: public LocalEventMessage {
        protected:
            virtual void setupAttributesForSerialization();

        public:
            LocalVmnicEventMessage();
            virtual ~LocalVmnicEventMessage();
			virtual void copy(VmnicEventMessage *message);
        	void copyDomain();
            std::string name;
            std::string mac;
            std::string host_id;
            std::string iface;
            std::string dvs;
            std::string dvs_nn;
            std::string dc_id;

            VmnicDomain         m_domain;
    };
    class PgHostVsEventMessage: public EventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
            PgHostVsEventMessage();
            virtual ~PgHostVsEventMessage();

            PgHostVsDomain            m_domain;
    };

    class LocalPgHostVsEventMessage: public LocalEventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
            LocalPgHostVsEventMessage();
            virtual ~LocalPgHostVsEventMessage();
			virtual void copy(PgHostVsEventMessage *message);
        	void copyDomain();
            std::string pg_id;
            std::string host_id;
            std::string host_nn;
            std::string vs;
            std::string vlan;
            std::string dc_id;
            std::string pg_nn;

            PgHostVsDomain            m_domain;
    };
    class VMEventMessage: public EventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	VMEventMessage();
            virtual ~VMEventMessage();

            VmDomain            m_domain;
    };


    class LocalVMEventMessage: public LocalEventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	LocalVMEventMessage();
            virtual ~LocalVMEventMessage();
			virtual void copy(VMEventMessage  *message);
        	void copyDomain();
            std::string id;
            std::string name;
            std::string host_id;
            std::string host_nn;
            std::string dc_id;

            VmDomain            m_domain;
    };

    class DVSEventMessage: public EventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	DVSEventMessage();
            virtual ~DVSEventMessage();

            DvsDomain            m_domain;
    };

    class LocalDVSEventMessage: public LocalEventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	LocalDVSEventMessage();
            virtual ~LocalDVSEventMessage();
			virtual void copy(DVSEventMessage *message);
        	void copyDomain();
            std::string id;
            std::string name;
            std::string pgroup_id;
            std::string pgroup_nn;
            std::string dc_id;

            DvsDomain            m_domain;
    };
    class DVPGEventMessage: public EventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	DVPGEventMessage();
            virtual ~DVPGEventMessage();

            DvpgDomain            m_domain;
    };

    class LocalDVPGEventMessage: public LocalEventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	LocalDVPGEventMessage();
            virtual ~LocalDVPGEventMessage();
			virtual void copy(DVPGEventMessage *message);
        	void copyDomain();
            std::string id;
            std::string name;
            std::string vlan;
            std::string qos;
            std::string port_prof;
            std::string dvs_id;
            std::string dvs_nn;
            std::string dc_id;
            std::string nrpk;
            std::string dvpgtype;
            int status;
            DvpgDomain            m_domain;
    };
    class HostEventMessage: public EventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	HostEventMessage();
            virtual ~HostEventMessage();

            HostDomain            m_domain;
    };

    class LocalHostEventMessage: public LocalEventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	LocalHostEventMessage();
            virtual ~LocalHostEventMessage();
			virtual void copy(HostEventMessage *message);
        	void copyDomain();
            std::string id;
            std::string name;
            std::string dc_id;

            HostDomain            m_domain;
    };
    class VNICEventMessage: public EventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	VNICEventMessage();
            virtual ~VNICEventMessage();

            VnicDomain            m_domain;
    };

    class LocalVNICEventMessage: public LocalEventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	LocalVNICEventMessage();
            virtual ~LocalVNICEventMessage();
			virtual void copy(VNICEventMessage *message);
        	void copyDomain();
            std::string name;
            std::string mac;
            std::string ip;
            std::string dvpg_id;
            std::string dvpg_nn;
            std::string port_prof;
            std::string vm_id;
            std::string dvs;
            std::string dc_id;
            int status;
            VnicDomain            m_domain;
    };
    class SPGEventMessage: public EventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	SPGEventMessage();
            virtual ~SPGEventMessage();

            SpecialPgDomain            m_domain;
    };

    class LocalSPGEventMessage: public LocalEventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	LocalSPGEventMessage();
            virtual ~LocalSPGEventMessage();
			virtual void copy(SPGEventMessage *message);
        	void copyDomain();
            std::string pg_nn;
            std::string port_prof;
            int status;
            std::string dc_id;
            std::string host_id;
            std::string host_nn;
            std::string vs;
            std::string vlan;

            SpecialPgDomain            m_domain;
    };
    class SNICEventMessage: public EventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	SNICEventMessage();
            virtual ~SNICEventMessage();

            SpecialNicDomain            m_domain;
    };

    class LocalSNICEventMessage: public LocalEventMessage {
        protected:
             virtual void setupAttributesForSerialization();

        public:
        	LocalSNICEventMessage();
            virtual ~LocalSNICEventMessage();
			virtual void copy(SNICEventMessage *message);
        	void copyDomain();
            std::string device;
            std::string mac;
            std::string ip;
            std::string pg_nn;
            std::string port_prof;
            std::string host_id;
            std::string vs;
            std::string vnic_type;
            int status;
            std::string dc_id;
            std::string dvpg_id;

            SpecialNicDomain            m_domain;
    };
// DP = Discovery Protocol
// CDP/LLDP / 


    class LocalVNMDPMessage: public PrismMessage {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
        	LocalVNMDPMessage();
        	LocalVNMDPMessage(const UI32 &cmdCode,const string &ifName,const string &hostMor,const string &portId, const string &vcenter);
            virtual ~LocalVNMDPMessage();

        public:
            UI32  m_cmdCode;
            string  m_ifName;
            string  m_hostMor;
            string  m_portId;
            string  m_vcenter;

    };

}



#endif /* VCENTEREVENTSMESSGE_H_ */
