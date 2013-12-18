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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Created on: Oct 9, 2012                                               *
 *   Author : Ankit Jindal                                                 *
 **************************************************************************/


#include <string>
#include <string.h>
#include <algorithm>
#include <vector>
#include "ClientInterface/Nsm/NsmUtils.h"
#include "PortProfileObject.h"
#include "APPM/Global/APPMGlobalObjectManager.h"

using namespace std;
using namespace WaveNs;


namespace DcmNs
{

PortProfileObject::PortProfileObject() : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),VCENTERGETPORTPROFILE)
    {
	  m_profileName = "";
	  m_swMode = NULL;
	  m_isAccessMode = NULL;
	  m_accessVlan = 0;
	  m_vlanRange = UI32Range("");
	  m_isAllowedVlanAll = false;
	  m_isAllowedVlanNone = false;
      m_nativeVlan = 0;		
	  m_macCount = 0;
	  m_removedMacCount = 0;
	  m_qos = "";
    }

PortProfileObject::PortProfileObject (const string &profileName,const bool &swMode,const bool &isAccessMode, const UI32 &accessVlan, const bool &isAllowedVlanAll,const bool &isAllowedVlanNone,const UI32Range &vlanRange) : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),VCENTERGETPORTPROFILE),
        m_profileName    (profileName),
        m_swMode    (swMode),
		m_isAccessMode (isAccessMode),
		m_accessVlan   (accessVlan),
        m_vlanRange    (vlanRange),
		m_isAllowedVlanAll    (isAllowedVlanAll),
        m_isAllowedVlanNone    (isAllowedVlanNone),
		m_nativeVlan	(NSM_NATIVE_VLAN_DEFAULT)
    {
    }

void PortProfileObject::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization (); 

 	addSerializableAttribute (new AttributeString       (&m_profileName,        "profileName"));
 	addSerializableAttribute (new AttributeBool         (&m_swMode,             "swMode"));
 	addSerializableAttribute (new AttributeBool         (&m_isAccessMode,       "isAccessMode"));
 	addSerializableAttribute (new AttributeUI32         (&m_accessVlan,         "accessVlan"));
 	addSerializableAttribute (new AttributeUI32Range    (&m_vlanRange,          "vlanRange"));
 	addSerializableAttribute (new AttributeBool         (&m_isAllowedVlanAll,   "isAllowedVlanAll"));
 	addSerializableAttribute (new AttributeBool         (&m_isAllowedVlanNone,  "isAllowedVlanNone"));
 	addSerializableAttribute (new AttributeUI32         (&m_nativeVlan,         "nativeVlan"));
 	addSerializableAttribute (new AttributeUI32         (&m_macCount,           "macCount"));
 	addSerializableAttribute (new AttributeString       (&m_qos,                "qos"));
 	addSerializableAttribute (new AttributeStringVector (&m_profileMacs,        "profileMacs"));
 	addSerializableAttribute (new AttributeStringVector (&m_removedProfileMacs, "removedProfileMacs"));
 	addSerializableAttribute (new AttributeUI32         (&m_removedMacCount,    "removedMacCount"));
}

	void  PortProfileObject::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  PortProfileObject::getProfileName() const
    {
        return (m_profileName);
    }

	void  PortProfileObject::setSwMode(const bool &swMode)
    {
        m_swMode  =  swMode;
    }

 	bool  PortProfileObject::getSwMode() const
    {
        return (m_swMode);
    }

    void  PortProfileObject::setisAccessMode(const bool &isAccessMode)
    {
        m_isAccessMode  =  isAccessMode;
    }

    bool  PortProfileObject::getisAccessMode() const
    {
        return (m_isAccessMode);
    }

    void  PortProfileObject::setIsAllowedVlanAll(const bool &isAllowedVlanAll)
    {
        m_isAllowedVlanAll  =  isAllowedVlanAll;
    }

    bool  PortProfileObject::getIsAllowedVlanAll() const
    {
        return (m_isAllowedVlanAll);
    }

    void  PortProfileObject::setIsAllowedVlanNone(const bool &isAllowedVlanNone)
    {
        m_isAllowedVlanNone  =  isAllowedVlanNone;
    }

    bool  PortProfileObject::getIsAllowedVlanNone() const
    {
        return (m_isAllowedVlanNone);
    }

    void  PortProfileObject::setAccessVlan(const UI32 &accessVlan)
    {
        m_accessVlan  =  accessVlan;
    }

    UI32  PortProfileObject::getAccessVlan() const
    {
        return (m_accessVlan);
    }

    void  PortProfileObject::setVlanRange(const UI32Range &vlanRange)
    {
        m_vlanRange  =  vlanRange;
    }

    UI32Range  PortProfileObject::getVlanRange() const
    {
        return (m_vlanRange);
    }

	void  PortProfileObject::setNativeVlanId(const UI32 &nativeVlan)
	{
		m_nativeVlan = nativeVlan;
	}

	UI32 PortProfileObject::getNativeVlanId() const
	{
		return (m_nativeVlan);
	}

    void  PortProfileObject::setMacCount(const UI32 &macCount)
    {
        m_macCount  =  macCount;
    }

    UI32  PortProfileObject::getMacCount() const
    {
        return (m_macCount);
    }

    void  PortProfileObject::setRemovedMacCount(const UI32 &removedMacCount)
    {
        m_removedMacCount  =  removedMacCount;
    }

    UI32  PortProfileObject::getRemovedMacCount() const
    {
        return (m_removedMacCount);
    }

    void PortProfileObject::setQos(const string &qos)
	{
	    m_qos = qos;
	}

	string PortProfileObject::getQos() const
	{
		return (m_qos);
	}

    void  PortProfileObject::setProfileMacs(const vector<string> &profileMacs)
    {
        m_profileMacs  =  profileMacs;
    }

    vector<string>  PortProfileObject::getProfileMacs() const
    {
        return (m_profileMacs);
    }

    void  PortProfileObject::setRemovedProfileMacs(const vector<string> &removedProfileMacs)
    {
        m_removedProfileMacs  =  removedProfileMacs;
    }

    vector<string>  PortProfileObject::getRemovedProfileMacs() const
    {
        return (m_removedProfileMacs);
    }


    bool PortProfileObject::compare(PortProfileObject* lhs, PortProfileObject* rhs) {
			return lhs->fncompid(rhs);
    }

	bool PortProfileObject::compareMac(string lhs, string rhs) {
	  	int ret = strcmp(lhs.c_str(),rhs.c_str());
		if (ret == 0)
		  	return false;
		else
		return true;
			      }

    bool PortProfileObject::fncompid(PortProfileObject* rhs) { 
		int ret = strcmp(this->getProfileName().c_str(),rhs->getProfileName().c_str());
        if (ret == 0)
            return true;
        return false;
    }

    void PortProfileObject::isConfigMismatch(PortProfileObject* const &d, bool& updated) {
	  	vector <string> macDiff;
		vector <string> removedMacs;
        std::vector<string>::iterator itrt;
        if (this->getProfileName() != d->getProfileName()){ //PP name is different - this case should not be hit
			updated = true;
			return;
		}
		    /* Checking macs */
            trace(TRACE_LEVEL_INFO,string("Switch DB MacCount() = ") + this->getMacCount() );
            trace(TRACE_LEVEL_INFO,string("vCenter DB MacCount() = ") + d->getMacCount());
			std::vector<string> macsOnSwitch = this->getProfileMacs();
			std::vector<string> macsInVcenterDb = d->getProfileMacs();
			std::sort( macsOnSwitch.begin(), macsOnSwitch.end() );
			std::sort( macsInVcenterDb.begin(), macsInVcenterDb.end() );
            /* Remove mac duplicates from vCenter DB - cloned VMs can have dup macs */
            itrt = std::unique (macsInVcenterDb.begin(), macsInVcenterDb.end());  
            macsInVcenterDb.resize( std::distance(macsInVcenterDb.begin(),itrt));
			/* Find the missing macs on switch */
			set_difference(macsInVcenterDb.begin(),macsInVcenterDb.end(),macsOnSwitch.begin(), macsOnSwitch.end(),back_inserter(macDiff),compareMac );
	        if( macDiff.size() > 0)
    	    {
			  trace(TRACE_LEVEL_ERROR,string("missing macs size: ") + macDiff.size());
            	vector<string>::iterator it;
	            for (it = macDiff.begin(); it < macDiff.end(); it++)
    	        {
        	      string mac = *it;
            	  trace(TRACE_LEVEL_INFO,string("ProfileName: " ) + this->getProfileName().c_str() );
              	  trace(TRACE_LEVEL_INFO,string("mac: ") + mac.c_str());
            	}
            	d->setProfileMacs(macDiff); // Set the missing macs in object to be reconciled
				updated = true;				// Only new macs will
				//return;  //check if some macs were removed also
        	}
			else
				d->setProfileMacs(macDiff); // Set this to empty

			/* Find the extra macs on switch */
			set_difference(macsOnSwitch.begin(),macsOnSwitch.end(),macsInVcenterDb.begin(),macsInVcenterDb.end(),back_inserter(removedMacs),compareMac);

		if( removedMacs.size() > 0)
		{
			trace(TRACE_LEVEL_ERROR,string("removed macs size: ") + removedMacs.size());	
			vector<string>::iterator it;
			for (it = removedMacs.begin(); it < removedMacs.end(); it++)
			{
			  	string mac = *it;
				trace(TRACE_LEVEL_INFO,string("ProfileName: " ) + this->getProfileName().c_str() );
				trace(TRACE_LEVEL_INFO,string("mac: ") + mac.c_str());
			}
			d->setRemovedProfileMacs(removedMacs); // Set the removed macs in object to be reconciled
			updated = true;
			return; //now return as already know profile updated and mac diff
		}
		else
		  d->setRemovedProfileMacs(removedMacs); // Set this to empty

        if (this->getSwMode() != d->getSwMode()){  //Switchport mode status do not match
		  trace(TRACE_LEVEL_ERROR,string("SwMode does not match for PP: ") + this->getProfileName().c_str());
            updated = true;
			return;
		}
		if (this->getisAccessMode() != d->getisAccessMode()){ //Switchport mode do not match
		  trace(TRACE_LEVEL_ERROR,string("AccessMode does not match for PP: ") + this->getProfileName().c_str());
		  trace(TRACE_LEVEL_INFO,string("Switch DB AccessMode() = ") +  this->getisAccessMode());
		  trace(TRACE_LEVEL_INFO,string("vCenter DB AccessMode() = ") + d->getisAccessMode());
			updated = true;
			return;
		}
		if (this->getIsAllowedVlanAll() != d->getIsAllowedVlanAll()){ //AllowedVlanAll do not match
		  trace(TRACE_LEVEL_ERROR,string("AllowedVlanAll does not match for PP: ") + this->getProfileName().c_str());
		   trace(TRACE_LEVEL_INFO,string("Switch DB AllowedVlanAll() = ") + this->getIsAllowedVlanAll() );
		   trace(TRACE_LEVEL_INFO,string("vCenter DB AllowedVlanAll() = " ) + d->getIsAllowedVlanAll() );
			updated = true;
			return;
		}
		if (this->getIsAllowedVlanNone() != d->getIsAllowedVlanNone()){ //AllowedVlanNone config does not match
		  trace(TRACE_LEVEL_ERROR,string("AllowedVlanNone does not match for PP: ") + this->getProfileName().c_str());
           trace(TRACE_LEVEL_INFO,string("Switch DB AllowedVlanNone() = ") + this->getIsAllowedVlanNone() );
           trace(TRACE_LEVEL_INFO,string("vCenter DB AllowedVlanNone() = " ) + d->getIsAllowedVlanNone() );
			updated = true;
			return;
		}
		if (this->getAccessVlan() != d-> getAccessVlan()){  //Access Vlan does not match
		  trace(TRACE_LEVEL_ERROR,string("AccessVlan does not match for PP: ") + this->getProfileName().c_str());
			updated = true;
			return;
		}
		if (this->getVlanRange() != d->getVlanRange()){ 
		    string vlstr = this->getVlanRange().toString();
			string vlstr2 = d->getVlanRange().toString();
			trace(TRACE_LEVEL_ERROR,string("VlanRange does not match for PP: ") + this->getProfileName().c_str());
		  	trace(TRACE_LEVEL_INFO,string("Switch DB VlanRange() = ") + vlstr.c_str() );
			trace(TRACE_LEVEL_INFO,string("vCenter DB VlanRange() = ") + vlstr2.c_str() );
			if (vlstr2 != "4091" && vlstr2 != "4092" && vlstr2 != "4093" && vlstr2 != "4094")
			{
				updated = true;
				return;
			}
			trace(TRACE_LEVEL_INFO,string("vCenter DB VlanRange is reserved vlans, ignoring"));
		}
		if (this->getNativeVlanId() != d->getNativeVlanId()){
		  trace(TRACE_LEVEL_ERROR,string("NativeVlanId does not match for PP: ") + this->getProfileName().c_str());
		  	trace(TRACE_LEVEL_ERROR,string("Switch DB NativeVlanId() = ") + this->getNativeVlanId() );
			trace(TRACE_LEVEL_ERROR,string("vCenter DB NativeVlanId() = ") + d->getNativeVlanId() );
			updated = true;
			return;
		}
		if (this->getQos() != d->getQos())
		{
		  updated = true;
		  trace(TRACE_LEVEL_ERROR,string("Qos does not match for PP: ") + this->getProfileName().c_str());
		  return;
		}
	}
}
