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

#include "vCenterEventsMessage.h"
#include "discover/Framework/discoverMessages.h"
#include "discover/Framework/discoverTypes.h"
#include "discover/Framework/discoverLocalObjectManager.h"
#include "discover/Framework/discoverObjectManager.h"
#include "Framework/Attributes/AttributeSerializableObject.cpp"

namespace DcmNs {


	EventMessage::EventMessage (const UI32 &operationId)
		: ManagementInterfaceMessage (DiscoverObjectManager::getClassName(), operationId)
	{
	}

	EventMessage::~EventMessage ()
	{
	}

	void EventMessage::setupAttributesForSerialization ()
	{
		ManagementInterfaceMessage::setupAttributesForSerialization ();

 		addSerializableAttribute (new AttributeUI32     (&m_opcode,     "opcode"));
 		addSerializableAttribute (new AttributeString   (&m_vcenter,    "vcenter"));
	}

	LocalEventMessage::LocalEventMessage (const UI32 &operationId)
		: PrismMessage (DiscoverLocalObjectManager::getPrismServiceId(), operationId)
	{
	}

	LocalEventMessage::~LocalEventMessage ()
	{
	}

	void LocalEventMessage::copy(EventMessage *message)
 	{
		m_opcode = message->m_opcode;
		m_vcenter = message->m_vcenter;
	}

	void LocalEventMessage::setupAttributesForSerialization ()
	{
		PrismMessage::setupAttributesForSerialization ();

 		addSerializableAttribute (new AttributeUI32     (&m_opcode,     "opcode"));
 		addSerializableAttribute (new AttributeString   (&m_vcenter,    "vcenter"));
	}

	// default constructor message is always sent to Global plugin
    DatacenterEventMessage::DatacenterEventMessage ()
        : EventMessage (DATACENTER_EVENT)
    {
    }

    DatacenterEventMessage::~DatacenterEventMessage ()
    {
    }

    void DatacenterEventMessage::setupAttributesForSerialization ()
    {
        EventMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSerializableObject<DataCenterDomain> (&m_domain, "domain"));
    }

    LocalDatacenterEventMessage::LocalDatacenterEventMessage ()
//    : PrismMessage (DiscoverLocalObjectManager::getPrismServiceId (), LOCAL_DATACENTER_EVENT)

    : LocalEventMessage (LOCAL_DATACENTER_EVENT)
    {
    }

    LocalDatacenterEventMessage::~LocalDatacenterEventMessage ()
    {
    }

    void LocalDatacenterEventMessage::setupAttributesForSerialization ()
    {
    	LocalEventMessage::setupAttributesForSerialization ();
//       addSerializableAttribute (new AttributeSerializableObject<DataCenterDomain> (&m_domain, "domain"));

    	addSerializableAttribute (new AttributeString(&id,"id"));
    	addSerializableAttribute (new AttributeString(&name,"name"));

    }

    void LocalDatacenterEventMessage::copy(
    		DatacenterEventMessage* pDiscoverDistributeMessage) {
        LocalEventMessage::copy(pDiscoverDistributeMessage);

    	this->id = pDiscoverDistributeMessage->m_domain.getId();
    	this->name = pDiscoverDistributeMessage->m_domain.getName();
//    	this->m_vcenter = pDiscoverDistributeMessage->m_vcenter;
//    	this->m_opcode = pDiscoverDistributeMessage->m_opcode;
    }

    void LocalDatacenterEventMessage::copyDomain() {
		this->m_domain.setId(id);
		this->m_domain.setName(name);
    }

    VswitchEventMessage::VswitchEventMessage ()
        : EventMessage (VSWITCH_EVENT)
    {
    }

    VswitchEventMessage::~VswitchEventMessage ()
    {
    }

    void VswitchEventMessage::setupAttributesForSerialization ()
    {
        EventMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeSerializableObject<VsDomain> (&m_domain, "domain"));
    }

    LocalVswitchEventMessage::LocalVswitchEventMessage ()
        : LocalEventMessage (LOCAL_VSWITCH_EVENT)
    {
    }

    LocalVswitchEventMessage::~LocalVswitchEventMessage ()
    {
    }

    void LocalVswitchEventMessage::setupAttributesForSerialization ()
    {
        LocalEventMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&name,     "name"));
        addSerializableAttribute (new AttributeString       (&host_id,  "host_id"));
        addSerializableAttribute (new AttributeString       (&host,     "host"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));
    }

    void LocalVswitchEventMessage::copy (VswitchEventMessage *message)
    {
        LocalEventMessage::copy(message);
        name 	= message->m_domain.name;
        host_id = message->m_domain.host_id;
        host 	= message->m_domain.host;
        dc_id 	= message->m_domain.dc_id;
//		m_domain = message->m_domain;
    }
    void LocalVswitchEventMessage::copyDomain() {

    	m_domain.name = name;
    	m_domain.host_id = host_id;
    	m_domain.host = host;
    	m_domain.dc_id = dc_id;
    }

    VmnicEventMessage::VmnicEventMessage ()
        : EventMessage (VMNIC_EVENT)
    {
    }

    VmnicEventMessage::~VmnicEventMessage ()
    {
    }

    void VmnicEventMessage::setupAttributesForSerialization ()
    {
        EventMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeSerializableObject<VmnicDomain> (&m_domain, "domain"));
    }

    LocalVmnicEventMessage::LocalVmnicEventMessage ()
        : LocalEventMessage (LOCAL_VMNIC_EVENT)
    {
    }

    LocalVmnicEventMessage::~LocalVmnicEventMessage ()
    {
    }

    void LocalVmnicEventMessage::setupAttributesForSerialization ()
    {
        LocalEventMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString       (&name,     "name"));
        addSerializableAttribute (new AttributeString       (&mac,      "mac"));
        addSerializableAttribute (new AttributeString       (&host_id,  "host_id"));
        addSerializableAttribute (new AttributeString       (&iface,    "iface"));
        addSerializableAttribute (new AttributeString       (&dvs,      "dvs"));
        addSerializableAttribute (new AttributeString       (&dvs_nn,   "dvs_nn"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));

//        addSerializableAttribute (new AttributeSerializableObject<VmnicDomain> (&m_domain, "domain"));
    }

    void LocalVmnicEventMessage::copy (VmnicEventMessage *message)
    {
        LocalEventMessage::copy(message);
        name		= message->m_domain.name;
        mac			= message->m_domain.mac;
        host_id		= message->m_domain.host_id;
        iface		= message->m_domain.iface;
        dvs			= message->m_domain.dvs;
        dvs_nn		= message->m_domain.dvs_nn;
        dc_id		= message->m_domain.dc_id;

//		m_domain = message->m_domain;
    }
    void LocalVmnicEventMessage::copyDomain() {
        m_domain.name 		= this->name;
        m_domain.mac  		= this->mac;
        m_domain.host_id	= this->host_id;
        m_domain.iface		= this->iface;
        m_domain.dvs		= this->dvs;
        m_domain.dvs_nn		= this->dvs_nn;
        m_domain.dc_id		= this->dc_id;
    }

    PgHostVsEventMessage::PgHostVsEventMessage ()
        : EventMessage (PGHOSTVS_EVENT)
    {
    }


    PgHostVsEventMessage::~PgHostVsEventMessage ()
    {
    }

    void PgHostVsEventMessage::setupAttributesForSerialization ()
    {
        EventMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeSerializableObject<PgHostVsDomain> (&m_domain, "domain"));
    }

    LocalPgHostVsEventMessage::LocalPgHostVsEventMessage ()
        : LocalEventMessage (LOCAL_PGHOSTVS_EVENT)
    {
    }


    LocalPgHostVsEventMessage::~LocalPgHostVsEventMessage ()
    {
    }

    void LocalPgHostVsEventMessage::setupAttributesForSerialization ()
    {
        LocalEventMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString       (&pg_id,    "pg_id"));
        addSerializableAttribute (new AttributeString       (&host_id,  "host_id"));
        addSerializableAttribute (new AttributeString       (&host_nn,  "host_nn"));
        addSerializableAttribute (new AttributeString       (&vs,       "vs"));
        addSerializableAttribute (new AttributeString       (&vlan,     "vlan"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));
        addSerializableAttribute (new AttributeString       (&pg_nn,    "pg_nn"));

//         addSerializableAttribute (new AttributeSerializableObject<PgHostVsDomain> (&m_domain, "domain"));
    }

    void LocalPgHostVsEventMessage::copy (PgHostVsEventMessage *message)
    {
        LocalEventMessage::copy(message);
        pg_id	=	message->m_domain.pg_id;
        host_id	=	message->m_domain.host_id;
        host_nn	=	message->m_domain.host_nn;
        vs	=	message->m_domain.vs;
        vlan	=	message->m_domain.vlan;
        dc_id	=	message->m_domain.dc_id;
        pg_nn	=	message->m_domain.pg_nn;

        //		m_domain = message->m_domain;
    }
    void LocalPgHostVsEventMessage::copyDomain() {
        m_domain.pg_id		= 	this->pg_id;
        m_domain.host_id	= 	this->host_id;
        m_domain.host_nn	=	this->host_nn;
        m_domain.vs			=	this->vs;
        m_domain.vlan		=	this->vlan;
        m_domain.dc_id		=	this->dc_id;
        m_domain.pg_nn		=	this->pg_nn;
    }

    VMEventMessage::VMEventMessage ()
        : EventMessage (VM_EVENT)
    {
    }

    VMEventMessage::~VMEventMessage ()
    {
    }

    void VMEventMessage::setupAttributesForSerialization ()
    {
        EventMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeSerializableObject<VmDomain> (&m_domain, "domain"));
    }



    LocalVMEventMessage::LocalVMEventMessage ()
        : LocalEventMessage (LOCAL_VM_EVENT)
    {
    }

    LocalVMEventMessage::~LocalVMEventMessage ()
    {
    }

    void LocalVMEventMessage::setupAttributesForSerialization ()
    {
        LocalEventMessage::setupAttributesForSerialization ();
    	addSerializableAttribute (new AttributeString(&id,"id"));

    	addSerializableAttribute (new AttributeString(&name,"name"));
    	addSerializableAttribute (new AttributeString(&host_id,"host_id"));
    	addSerializableAttribute (new AttributeString(&host_nn,"host_nn"));
    	addSerializableAttribute (new AttributeString(&dc_id,"dc_id"));
    }




    void LocalVMEventMessage::copy (VMEventMessage *message)
    {
        LocalEventMessage::copy(message);
        id		=	message->m_domain.id;
        name	=	message->m_domain.name;
        host_id	=	message->m_domain.host_id;
        host_nn	=	message->m_domain.host_nn;
        dc_id	=	message->m_domain.dc_id;

//		m_domain = message->m_domain;
    }
    void LocalVMEventMessage::copyDomain() {
        m_domain.id			=	id;
        m_domain.name		=	name;
        m_domain.host_id	=	host_id;
        m_domain.host_nn	=	host_nn;
        m_domain.dc_id		=	dc_id;
    }


    DVSEventMessage::DVSEventMessage ()
        : EventMessage (DVS_EVENT)
    {
    }

    DVSEventMessage::~DVSEventMessage ()
    {
    }

    void DVSEventMessage::setupAttributesForSerialization ()
    {
        EventMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeSerializableObject<DvsDomain> (&m_domain, "domain"));
    }

    LocalDVSEventMessage::LocalDVSEventMessage ()
        : LocalEventMessage (LOCAL_DVS_EVENT)
    {
    }

    LocalDVSEventMessage::~LocalDVSEventMessage ()
    {
    }

    void LocalDVSEventMessage::setupAttributesForSerialization ()
    {
        LocalEventMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString       (&id,       "id"));
        addSerializableAttribute (new AttributeString       (&name,     "name"));
        addSerializableAttribute (new AttributeString       (&pgroup_id,"pgroup_id"));
        addSerializableAttribute (new AttributeString       (&pgroup_nn,"pgroup_nn"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));

//        addSerializableAttribute (new AttributeSerializableObject<DvsDomain> (&m_domain, "domain"));
    }


    void LocalDVSEventMessage::copy (DVSEventMessage *message)
    {
        LocalEventMessage::copy(message);
        id			=	message->m_domain.id;
        name		=	message->m_domain.name;
        pgroup_id	=	message->m_domain.pgroup_id;
        pgroup_nn	=	message->m_domain.pgroup_nn;
        dc_id		=	message->m_domain.dc_id;

//		m_domain = message->m_domain;
    }
    void LocalDVSEventMessage::copyDomain() {
        m_domain.id			=	id;
        m_domain.name		=	name;
        m_domain.pgroup_id	=	pgroup_id;
        m_domain.pgroup_nn	=	pgroup_nn;
        m_domain.dc_id		=	dc_id;
    }

    DVPGEventMessage::DVPGEventMessage ()
        : EventMessage (DVPG_EVENT)
    {
    }

    DVPGEventMessage::~DVPGEventMessage ()
    {
    }

    void DVPGEventMessage::setupAttributesForSerialization ()
    {
        EventMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeSerializableObject<DvpgDomain> (&m_domain, "domain"));
    }

    LocalDVPGEventMessage::LocalDVPGEventMessage ()
        : LocalEventMessage (LOCAL_DVPG_EVENT)
    {
    }

    LocalDVPGEventMessage::~LocalDVPGEventMessage ()
    {
    }

    void LocalDVPGEventMessage::setupAttributesForSerialization ()
    {
        LocalEventMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&id,           "id"));
        addSerializableAttribute (new AttributeString       (&name,         "name"));
        addSerializableAttribute (new AttributeString       (&vlan,         "vlan"));
        addSerializableAttribute (new AttributeString       (&qos,          "qos"));
        addSerializableAttribute (new AttributeString       (&port_prof,    "port_prof"));
        addSerializableAttribute (new AttributeString       (&dvs_id,       "dvs_id"));
        addSerializableAttribute (new AttributeString       (&dvs_nn,       "dvs_nn"));
        addSerializableAttribute (new AttributeString       (&dc_id,        "dc_id"));
        addSerializableAttribute (new AttributeString       (&nrpk,         "nrpk"));
        addSerializableAttribute (new AttributeString       (&dvpgtype,     "dvpgtype"));

//        addSerializableAttribute (new AttributeSerializableObject<DvpgDomain> (&m_domain, "domain"));
    }


    void LocalDVPGEventMessage::copy (DVPGEventMessage *message)
    {
        LocalEventMessage::copy(message);
        id			=	message->m_domain.id;
        name		=	message->m_domain.name;
        vlan		=	message->m_domain.vlan;
        qos			=	message->m_domain.qos;
        port_prof	=	message->m_domain.port_prof;
        dvs_id		=	message->m_domain.dvs_id;
        dvs_nn		=	message->m_domain.dvs_nn;
        dc_id		=	message->m_domain.dc_id;
        nrpk		=	message->m_domain.nrpk;
        dvpgtype	=	message->m_domain.dvpgtype;
        status		=	message->m_domain.status;

//		m_domain = message->m_domain;
    }
    void LocalDVPGEventMessage::copyDomain() {
    	m_domain.id			=	id;
    	m_domain.name		=	name;
    	m_domain.vlan		=	vlan;
    	m_domain.qos		=	qos;
    	m_domain.port_prof	=	port_prof;
    	m_domain.dvs_id		=	dvs_id;
    	m_domain.dvs_nn		=	dvs_nn;
    	m_domain.dc_id		=	dc_id;
    	m_domain.nrpk		=	nrpk;
    	m_domain.dvpgtype	=	dvpgtype;
    	m_domain.status		=	status;
    }

    HostEventMessage::HostEventMessage ()
        : EventMessage (HOST_EVENT)
    {
    }

    HostEventMessage::~HostEventMessage ()
    {
    }

    void HostEventMessage::setupAttributesForSerialization ()
    {
        EventMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeSerializableObject<HostDomain> (&m_domain, "domain"));
    }

    LocalHostEventMessage::LocalHostEventMessage ()
        : LocalEventMessage (LOCAL_HOST_EVENT)
    {
    }

    LocalHostEventMessage::~LocalHostEventMessage ()
    {
    }

    void LocalHostEventMessage::setupAttributesForSerialization ()
    {
        LocalEventMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString       (&id,      "id"));
        addSerializableAttribute (new AttributeString       (&name,     "name"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));
//         addSerializableAttribute (new AttributeSerializableObject<HostDomain> (&m_domain, "domain"));
    }


    void LocalHostEventMessage::copy (HostEventMessage *message)
    {
        LocalEventMessage::copy(message);
        id		=	message->m_domain.id;
        name	=	message->m_domain.name;
        dc_id	=	message->m_domain.dc_id;

//		m_domain = message->m_domain;
    }
    void LocalHostEventMessage::copyDomain() {
        m_domain.id		=	id;
        m_domain.name	=	name;
        m_domain.dc_id	=	dc_id;
    }

    VNICEventMessage::VNICEventMessage ()
        : EventMessage (VNIC_EVENT)
    {
    }

    VNICEventMessage::~VNICEventMessage ()
    {
    }

    void VNICEventMessage::setupAttributesForSerialization ()
    {
        EventMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeSerializableObject<VnicDomain> (&m_domain, "domain"));
    }

    LocalVNICEventMessage::LocalVNICEventMessage ()
        : LocalEventMessage (LOCAL_VNIC_EVENT)
    {
    }

    LocalVNICEventMessage::~LocalVNICEventMessage ()
    {
    }

    void LocalVNICEventMessage::setupAttributesForSerialization ()
    {
        LocalEventMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString       (&name,     "name"));
        addSerializableAttribute (new AttributeString       (&mac,      "mac"));
        addSerializableAttribute (new AttributeString       (&ip,       "ip"));
        addSerializableAttribute (new AttributeString       (&dvpg_id,  "dvpg_id"));
        addSerializableAttribute (new AttributeString       (&dvpg_nn,  "dvpg_nn"));
        addSerializableAttribute (new AttributeString       (&port_prof,"port_prof"));
        addSerializableAttribute (new AttributeString       (&vm_id,    "vm_id"));
        addSerializableAttribute (new AttributeString       (&dvs,      "dvs"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));

//         addSerializableAttribute (new AttributeSerializableObject<VnicDomain> (&m_domain, "domain"));
    }


    void LocalVNICEventMessage::copy (VNICEventMessage *message)
    {
        LocalEventMessage::copy(message);

        name		= message->m_domain.name;
        mac			= message->m_domain.mac;
        ip			= message->m_domain.ip;
        dvpg_id		= message->m_domain.dvpg_id;
        dvpg_nn		= message->m_domain.dvpg_nn;
        port_prof	= message->m_domain.port_prof;
        vm_id		= message->m_domain.vm_id;
        dvs			= message->m_domain.dvs;
        dc_id		= message->m_domain.dc_id;
        status		= message->m_domain.status;

//		m_domain = message->m_domain;
    }
    void LocalVNICEventMessage::copyDomain() {
    	m_domain.name		= name;
    	m_domain.mac		= mac;
    	m_domain.ip			= ip;
    	m_domain.dvpg_id	= dvpg_id;
    	m_domain.dvpg_nn	= dvpg_nn;
    	m_domain.port_prof	= port_prof;
    	m_domain.vm_id		= vm_id;
    	m_domain.dvs		= dvs;
    	m_domain.dc_id		= dc_id;
    	m_domain.status		= status;
    }

    SPGEventMessage::SPGEventMessage ()
        : EventMessage (SPG_EVENT)
    {
    }


    SPGEventMessage::~SPGEventMessage ()
    {
    }

    void SPGEventMessage::setupAttributesForSerialization ()
    {
        EventMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeSerializableObject<SpecialPgDomain> (&m_domain, "domain"));
    }

    LocalSPGEventMessage::LocalSPGEventMessage ()
        : LocalEventMessage (LOCAL_SPG_EVENT)
    {
    }


    LocalSPGEventMessage::~LocalSPGEventMessage ()
    {
    }

    void LocalSPGEventMessage::setupAttributesForSerialization ()
    {
        LocalEventMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&pg_nn,    "pg_nn"));
        addSerializableAttribute (new AttributeString       (&port_prof,"port_prof"));
        addSerializableAttribute (new AttributeSI32         (&status,   "status"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));

        addSerializableAttribute (new AttributeString       (&host_id,  "host_id"));
        addSerializableAttribute (new AttributeString       (&host_nn,  "host_nn"));
        addSerializableAttribute (new AttributeString       (&vs,       "vs"));
        addSerializableAttribute (new AttributeString       (&vlan,     "vlan"));
 
//         addSerializableAttribute (new AttributeSerializableObject<SpecialPgDomain> (&m_domain, "domain"));
    }


    void LocalSPGEventMessage::copy (SPGEventMessage *message)
    {
        LocalEventMessage::copy(message);
        pg_nn = message->m_domain.pg_nn;
        port_prof = message->m_domain.port_prof;
        status = message->m_domain.status;
        dc_id = message->m_domain.dc_id;
        host_id = message->m_domain.host_id;
        host_nn = message->m_domain.host_nn;
        vs = message->m_domain.vs;
        vlan = message->m_domain.vlan;



//		m_domain = message->m_domain;
    }
    void LocalSPGEventMessage::copyDomain() {
        m_domain.pg_nn		=	pg_nn;
        m_domain.port_prof	=	port_prof;
        m_domain.status		=	status;
        m_domain.dc_id		=	dc_id;

        m_domain.host_id    =   host_id;
        m_domain.host_nn    =   host_nn;
        m_domain.vs         =   vs;
        m_domain.vlan       =   vlan;


    }

    SNICEventMessage::SNICEventMessage ()
        : EventMessage (SNIC_EVENT)
    {
    }


    SNICEventMessage::~SNICEventMessage ()
    {
    }

    void SNICEventMessage::setupAttributesForSerialization ()
    {
        EventMessage::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeSerializableObject<SpecialNicDomain> (&m_domain, "domain"));
    }

    LocalSNICEventMessage::LocalSNICEventMessage ()
        : LocalEventMessage (LOCAL_SNIC_EVENT)
    {
    }


    LocalSNICEventMessage::~LocalSNICEventMessage ()
    {
    }

    void LocalSNICEventMessage::setupAttributesForSerialization ()
    {
        LocalEventMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString       (&device,   "device"));
        addSerializableAttribute (new AttributeString       (&mac,      "mac"));
        addSerializableAttribute (new AttributeString       (&ip,       "ip"));
        addSerializableAttribute (new AttributeString       (&pg_nn,    "pg_nn"));
        addSerializableAttribute (new AttributeString       (&port_prof,"port_prof" ));
        addSerializableAttribute (new AttributeString       (&host_id,  "host_id"));
        addSerializableAttribute (new AttributeString       (&vs,       "vs"));
        addSerializableAttribute (new AttributeString       (&vnic_type, "vnic_type"));
        addSerializableAttribute (new AttributeSI32         (&status,   "status"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));
        addSerializableAttribute (new AttributeString       (&dvpg_id,    "dvpg_id"));

//         addSerializableAttribute (new AttributeSerializableObject<SpecialNicDomain> (&m_domain, "domain"));
    }


    void LocalSNICEventMessage::copy (SNICEventMessage *message)
    {
        LocalEventMessage::copy(message);
        device		=	message->m_domain.device;
        mac			=	message->m_domain.mac;
        ip			=	message->m_domain.ip;
        pg_nn		=	message->m_domain.pg_nn;
        port_prof	=	message->m_domain.port_prof;
        host_id		=	message->m_domain.host_id;
        vs			=	message->m_domain.vs;
        vnic_type	=	message->m_domain.vnic_type;
        status		=	message->m_domain.status;
        dc_id		=	message->m_domain.dc_id;
        dvpg_id		=	message->m_domain.dvpg_id;

//		m_domain = message->m_domain;
    }
    void LocalSNICEventMessage::copyDomain() {
        m_domain.device		=	device;
        m_domain.mac		=	mac;
        m_domain.ip			=	ip;
        m_domain.pg_nn		=	pg_nn;
        m_domain.port_prof	=	port_prof;
        m_domain.host_id	=	host_id;
        m_domain.vs			=	vs;
        m_domain.vnic_type	=	vnic_type;
        m_domain.status		=	status;
        m_domain.dc_id		=	dc_id;
        m_domain.dvpg_id    =	dvpg_id;
    }

    LocalVNMDPMessage::LocalVNMDPMessage ()
         : PrismMessage (DiscoverLocalObjectManager::getPrismServiceId (),LOCAL_DP_EVENT)
     {
     }

    LocalVNMDPMessage::LocalVNMDPMessage (const UI32 &cmdCode,const string &ifName,const string &hostMor,const string &portId, const string &vcenter)
         : PrismMessage (DiscoverLocalObjectManager::getPrismServiceId (),LOCAL_DP_EVENT),
         m_cmdCode    (cmdCode),
         m_ifName    (ifName),
         m_hostMor    (hostMor),
         m_portId    (portId),
         m_vcenter    (vcenter)
     {
     }

    LocalVNMDPMessage::~LocalVNMDPMessage ()
     {
     }

     void  LocalVNMDPMessage::setupAttributesForSerialization()
     {
    	 PrismMessage::setupAttributesForSerialization ();
         addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
         addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
         addSerializableAttribute (new AttributeString(&m_hostMor,"hostMor"));
         addSerializableAttribute (new AttributeString(&m_portId,"portId"));
         addSerializableAttribute (new AttributeString(&m_vcenter,"vcenter"));
     }

} /* namespace DcmNs */
