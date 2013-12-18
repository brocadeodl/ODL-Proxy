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
 *   Created on: Oct 7, 2012                                               *
 *   Author : Ankit Jindal                                                 *
 **************************************************************************/
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "Utils/DceClusterUtils.h"
#include "APPM/Global/ProfileMacManagedObject.h"
#include "APPM/Global/QosProfileCfgManagedObject.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"

#include "Utils/Capabilities.h"
#include "correlator/VcenterDiff.h"
#include "correlator/SwitchAdapter.h"
#include "correlator/VcenterUtils.h"

#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include <algorithm>
#include <set>
#include <map>

#include "VirtualDb/DAO/DvpgDAO.h"
#include "VirtualDb/DAO/PgDAO.h"
#include "VirtualDb/DAO/DAO.h"
#include "discover/utils/URLEncode.h"
#include "discover/utils/FileUtils.h"
#include "PortProfileObject.h"
#include "ClientInterface/APPM/APPMMessageDef.h"

using namespace std;
using namespace WaveNs;

#define PROFILE_NAME(vcenter_Name,datacenter,port_group)  "auto_" + vcenter_Name + "_" + URLEncode::urlencode(datacenter) + "_" + URLEncode::urlencode(port_group);

namespace DcmNs
{

    //checks two profiles to be same on the basis of profile name
    //Useful in unique std algorithm
    bool isEquivalentProfile(PortProfileObject* a, PortProfileObject* b) {
        return a->getProfileName() == b->getProfileName();
    }

struct isEquivalentProfileString
{
    bool operator()(PortProfileObject* a, PortProfileObject* b) const
        {
              return !(a->getProfileName() == b->getProfileName());
        }
};

  void VcenterDiff::getPPDiff(string vcName,vector<PortProfileObject*>& vector_added,vector<PortProfileObject*>&  vector_deleted,vector<PortProfileObject*>& vector_updated, bool isReconcile)
	{
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
	string profile_name;
	vector <string> ppnames;
	vector<Domain*> db_pgs;
	vector<Domain*> db_dvpgs;
	vector<Domain*> db_specialpgs;
	vector<Domain*> db_pgsAll;
	vector<Domain*> db_vnic; 
	vector<Domain*> db_specialnic;
	vector<Domain*>	vnicAndSpecialnicDomains;
	ValClientSynchronousConnection *pValClientSynchronousConnection;

	string pattern = "auto_" + vcName + "_%";

	pValClientSynchronousConnection = DcmToolKit::getInterDcmCcmClient();
	pValClientSynchronousConnection->getAllPortProfilesFromDcmd(pattern, ppnames);
	string new_dbfile_r = "";
	string vcenter = vcName;
	string mor ="";

	if (isReconcile) //.db file to be used for reconciliation
		new_dbfile_r = FileUtils::getDBLocation(vcenter);
	else
	  	new_dbfile_r = FileUtils::getDBNewLocation(vcenter);
	trace(TRACE_LEVEL_DEBUG,string("VcenterDiff::getPPDiff: vcenter file") + new_dbfile_r.c_str() + "\n");

	db_pgs = getSource(PG_DAO, new_dbfile_r,mor);
	db_dvpgs = getSource(DVPG_DAO, new_dbfile_r,mor);
	db_specialpgs = getSource(SPECIALPG_DAO, new_dbfile_r,mor);

	db_pgsAll.reserve( db_pgs.size() + db_dvpgs.size() + db_specialpgs.size() );
	db_pgsAll.insert( db_pgsAll.end(), db_pgs.begin(), db_pgs.end() );
	db_pgsAll.insert( db_pgsAll.end(), db_dvpgs.begin(), db_dvpgs.end() );
	db_pgsAll.insert( db_pgsAll.end(), db_specialpgs.begin(), db_specialpgs.end() );

	db_vnic = getSource(VNIC_DAO, new_dbfile_r,mor);
	db_specialnic = getSource(SPECIALNIC_DAO, new_dbfile_r,mor);

	vnicAndSpecialnicDomains.reserve(db_vnic.size() + db_specialnic.size() );
	vnicAndSpecialnicDomains.insert(vnicAndSpecialnicDomains.end(), db_vnic.begin(),db_vnic.end() );
	vnicAndSpecialnicDomains.insert(vnicAndSpecialnicDomains.end(),db_specialnic.begin(),db_specialnic.end() );

	multimap<string, string> portProfileMacMap = createMapOfProfileToMac(vcenter,vnicAndSpecialnicDomains);

	VIC_PLUG_LOG(VIC_TRACE_INFO, "VcenterDiff::getPPDiff: Running Diff for %s \n", vcenter.c_str());

	vector <PortProfileObject*> pp_vector_db;

	vector<Domain*>::iterator it2;
	if(db_pgsAll.size() > 0)
	{
	for (it2 = db_pgsAll.begin(); it2 < db_pgsAll.end(); it2++) {

	  	if((*it2)->getDVPGType().compare("uplink")==0)
		  continue;

		multimap<string, string>::iterator itmap;
		vector<string> macVector;
		pair<multimap<string, string>::iterator ,multimap<string, string>::iterator> retIt;
		string portProfile = PROFILE_NAME(vcenter,(*it2)->getDcId(), (*it2)->getPgNN());
		trace(TRACE_LEVEL_DEBUG,string("Processing: " ) + portProfile );
	    PortProfileObject *pProfileObject = new PortProfileObject();
        pProfileObject->setProfileName(portProfile);
		string vlan = VcenterUtils::convertToVlanUI32Range( (*it2)->getVlan());	
		trace(TRACE_LEVEL_DEBUG, string("VcenterDiff::getPPDiff: vlan string= ") + vlan);

        pProfileObject->setQos((*it2)->getQos());
        if(portProfileMacMap.find(portProfile) != portProfileMacMap.end() )
        {
        	retIt = portProfileMacMap.equal_range(portProfile);
        	for (itmap = retIt.first; itmap != retIt.second; ++itmap) {
                macVector.push_back((*itmap).second); //insert all macs assoiciated with the PP
            }
        	if(macVector.size() > 0) //sort macs if present
            	sort(macVector.begin(), macVector.end());
        	pProfileObject->setProfileMacs(macVector);
        	pProfileObject->setMacCount(macVector.size());
        }
		
		if( vlan == "" || vlan == " " || vlan == ",")
		{
		  pp_vector_db.push_back(pProfileObject);
		  continue;
		}

		vector<UI32> vlanRangeVector;
        UI32Range vlanAddrange = UI32Range("");
        UI32Range vlanAddrange2 = UI32Range("0");;
		vlanAddrange.fromString(vlan);
		vlanAddrange.getUI32RangeVector(vlanRangeVector);

        pProfileObject->setVlanRange(vlanAddrange2); //initializing it to zero
        trace(TRACE_LEVEL_DEBUG, string("VcenterDiff::vlanRangeVector.size = ") + vlanRangeVector.size());
		
		if( ( vlan == "0") || ( (vlanRangeVector.size() < 2) && (vlanRangeVector[0] == 0)) )
		{
			pProfileObject->setSwMode(true);
			pProfileObject->setisAccessMode(true);
			pProfileObject->setAccessVlan(1);
		}
		else if ( ( vlan == "1")  || ( (vlanRangeVector.size() < 2) && (vlanRangeVector[0] == 1)))
		{
		  pProfileObject->setSwMode(true);
		  pProfileObject->setisAccessMode(false);
		  pProfileObject->setIsAllowedVlanNone(true);
		}
		else if ( (vlanRangeVector.size() == 2) && (vlanRangeVector[0] == 0) && (vlanRangeVector[1] == 1))
		{
		  pProfileObject->setSwMode(true);
          pProfileObject->setisAccessMode(false);
		  pProfileObject->setIsAllowedVlanNone(true);
		  pProfileObject->setNativeVlanId(1);
		}
		else if ( (vlanRangeVector.size() > 1) && (vlanRangeVector[0] == 0) )
		{
            vlanRangeVector.erase(vlanRangeVector.begin());
            vlanAddrange2 = UI32Range::getUI32RangeStringFromVector(vlanRangeVector);
			pProfileObject->setSwMode(true);
			pProfileObject->setisAccessMode(false);
			pProfileObject->setVlanRange(vlanAddrange2);
			pProfileObject->setNativeVlanId(1);
		}
		else if ( (vlan == "4095") || ( (vlanRangeVector.size() < 2) &&
			  (vlanRangeVector[0] == 4095)) || ((vlanRangeVector.size() == 2 )
				&& ( vlanRangeVector[0] == 4095 && vlanRangeVector[1] == 4095)
				))
		{
			pProfileObject->setSwMode(true);
			pProfileObject->setisAccessMode(false);
			pProfileObject->setIsAllowedVlanAll(true);
			pProfileObject->setNativeVlanId(1);
		}
        else if((vlan == "4094") || vlanRangeVector[0] == 4094  || (vlan == "4093") || vlanRangeVector[0] == 4093 || (vlan == "4092") || vlanRangeVector[0] == 4092 || vlanRangeVector[0] == 4091 || (vlan == "4091") )
        {
			pProfileObject->setSwMode(true);
			pProfileObject->setisAccessMode(false);
			pProfileObject->setVlanRange(vlanAddrange);
        }
		else
		{
            if(vlanRangeVector[0] == 0)
            {
                vlanRangeVector.erase(vlanRangeVector.begin());
                vlanAddrange = UI32Range::getUI32RangeStringFromVector(vlanRangeVector);
            }
			pProfileObject->setSwMode(true);
			pProfileObject->setisAccessMode(false);
			pProfileObject->setVlanRange(vlanAddrange);
		}
		pp_vector_db.push_back(pProfileObject);
		trace(TRACE_LEVEL_DEBUG, string("VcenterDiff:: Inserting PP: ") + portProfile );
	}
	}

		vector<string>::iterator it_pp;
		vector <PortProfileObject*> pp_vector_sw;

		if(ppnames.size() > 0)
		{
		for (it_pp = ppnames.begin(); it_pp < ppnames.end(); it_pp++)
		{
			PortProfileObject *pProfile = new PortProfileObject();
			string strPname = *it_pp;
			pProfile->setProfileName(strPname);
			pValClientSynchronousConnection->getProfileInfoFromDcmdDB(*pProfile);
		pp_vector_sw.push_back(pProfile);
		}
	}

        trace(TRACE_LEVEL_DEBUG, string("pp_vector_db SIZE = ") + pp_vector_db.size());
        trace(TRACE_LEVEL_DEBUG, string("pp_vector_sw SIZE = ") + pp_vector_sw.size());


        removeDuplicates(pp_vector_db);
        removeDuplicates(pp_vector_sw);

        trace(TRACE_LEVEL_DEBUG, string(" pp_vector_db SIZE = ") + pp_vector_db.size());
        trace(TRACE_LEVEL_DEBUG, string("pp_vector_sw SIZE = ") + pp_vector_sw.size());
        std::sort(pp_vector_db.begin(), pp_vector_db.end(), compareByName);
        std::sort(pp_vector_sw.begin(), pp_vector_sw.end(), compareByName);

	    vector_added = diff(pp_vector_sw, pp_vector_db);
        trace(TRACE_LEVEL_INFO, string("vector_added SIZE = ") + vector_added.size());
	    vector_deleted = diff(pp_vector_db,pp_vector_sw);
        trace(TRACE_LEVEL_INFO, string("vector_deleted SIZE = ") + vector_deleted.size());
	    findPortProfileConfigMismatch(pp_vector_db,pp_vector_sw, vector_updated);
        trace(TRACE_LEVEL_INFO, string("vector_updated SIZE = ") + vector_updated.size());

}

	void VcenterDiff::reconcile(string vcenter, bool useDB) {

        vector<PortProfileObject*> added;
        vector<PortProfileObject*> deleted;
        vector<PortProfileObject*> updated;
        UI32Range VlanRange = UI32Range("");
        SwitchAdapter *switchAdapter = new SwitchAdapter(vcenter);
        switchAdapter->configCdpProtocol(true);
        vector<PortProfileObject*>::iterator it;
        string vlanStr;
        getPPDiff(vcenter, added, deleted, updated, useDB);

        for (it = deleted.begin(); it < deleted.end(); it++) {
            string profileName = (*it)->getProfileName();
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                    "Running reconcile:Deleting Port-Profile for %s %s\n",
                    profileName.c_str(), vcenter.c_str());
            switchAdapter->deActivatePortProfile(profileName);
            switchAdapter->deletePortProfile(profileName);
        }

        configureVlans(vcenter, added, updated);

        // create these port-profiles on switch - added

        for (it = added.begin(); it < added.end(); it++) {
            string profileName = (*it)->getProfileName();
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                    "Running reconcile:Adding Port-Profile for %s %s\n",
                    profileName.c_str(), vcenter.c_str());
            vector<string> macVector = (*it)->getProfileMacs();
            trace(TRACE_LEVEL_DEBUG, string("macVector SIZE =") + macVector.size());

            vlanStr = getVlanFromPPObject(*it);

			VIC_PLUG_LOG(VIC_TRACE_DEBUG,"VcenterDiff: switchAdapter->createProfileAndSubProfiles with vlan %s",vlanStr.c_str());
            switchAdapter->createProfileAndSubProfiles(profileName, vlanStr, true, (*it)->getQos());
            if (macVector.size() > 0)
                switchAdapter->reassociateMacsToPortProfile(profileName, macVector); //will associate for first time
        }

        // update port-profiles on switch

        for (it = updated.begin(); it < updated.end(); it++) {
            vector<string> macVector = (*it)->getProfileMacs();
			vector<string> removedMacVector = (*it)->getRemovedProfileMacs();
            string profileName = (*it)->getProfileName();
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Running reconcile:Updating Port-Profile for %s %s\n", profileName.c_str(), vcenter.c_str());

            vlanStr = getVlanFromPPObject(*it);

            //TODO: need a check to avoid this if no change in QoS
            switchAdapter->updateQoSProfile(profileName, (*it)->getQos());
            //TODO: need a check to avoid this if no change in VLAN profile
			VIC_PLUG_LOG(VIC_TRACE_DEBUG,"VcenterDiff: switchAdapter->updateVlanProfile with vlan %s",vlanStr.c_str());
            switchAdapter->updateVlanProfile(profileName, vlanStr);
			if (removedMacVector.size() > 0)
			  switchAdapter->deAssociateMacsFromPortProfile(profileName,removedMacVector);
            if (macVector.size() > 0)
			{
			  trace(TRACE_LEVEL_DEBUG,string("macVector Size =") + macVector.size());
              switchAdapter->reassociateMacsToPortProfile(profileName,macVector);
			}
        }
        releaseSource(added);
        releaseSource(deleted);
        releaseSource(updated);
        delete switchAdapter;
    }

    void VcenterDiff::releaseSource(vector<PortProfileObject*> v) {
        for (unsigned int i = 0; i < v.size(); i++ )
            delete v[i];
    }

    string VcenterDiff::getVlanFromPPObject(PortProfileObject* a)
    {
        string vlanStr = "";
        UI32Range VlanRange = UI32Range("");

            if (a->getisAccessMode())
                vlanStr = "0";
            else if( a->getIsAllowedVlanNone() && a->getNativeVlanId() == 1)
                vlanStr = "0-1";
            else if(a->getIsAllowedVlanNone())
                vlanStr = "1";
            else if(a->getNativeVlanId() == 1)
               {
                    VlanRange = a->getVlanRange();
                    vlanStr = VlanRange.toString();
                    vlanStr = "0," + vlanStr;
               }
            else if ((a->getIsAllowedVlanAll()) && (a->getNativeVlanId() == 1) )
              vlanStr = "4095";
            else {
                VlanRange = a->getVlanRange();
                vlanStr = VlanRange.toString();
            }
        trace(TRACE_LEVEL_DEBUG, string("VcenterDiff::getVlanFromPPObject: vlan string= ") + vlanStr);
        return vlanStr;

    }

    void VcenterDiff::findPortProfileConfigMismatch(
            vector<PortProfileObject*> pp_vector_db,
            vector<PortProfileObject*> pp_vector_sw,
            vector<PortProfileObject*>& vector_updated) {

        vector<PortProfileObject*> v = intersect(pp_vector_db, pp_vector_sw);
        if (v.size() > 0) {
            vector<PortProfileObject*> w = intersect(pp_vector_sw,
                    pp_vector_db);
            vector<PortProfileObject*>::iterator it1 = v.begin();
            vector<PortProfileObject*>::iterator it2 = w.begin();
            while (it1 != v.end()) {
                bool updated_flag = false;
                (*it1)->isConfigMismatch(*it2, updated_flag);
                if (updated_flag) {
                    vector_updated.insert(vector_updated.begin(), (*it2));
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"Config Mismatch found for Profile  %s",(*it2)->getProfileName().c_str());
                }
                it1++;
                it2++;
            }
        }
    }

	 vector<PortProfileObject*> VcenterDiff::diff(vector<PortProfileObject*> a, vector<PortProfileObject*> b) {
        cout << __FUNCTION__ << endl;
        vector<PortProfileObject*> v;
		set<string> inserts;
		pair<set<string>::iterator,bool> ret;
        if (a.size() > 0) {
		  vector<PortProfileObject*>::iterator it;
		  for (it = a.begin(); it < a.end(); it++) {
		 		ret = inserts.insert((*it)->getProfileName());    
		  }
		  for (it = b.begin(); it < b.end(); it++) {

				ret = inserts.insert((*it)->getProfileName());
				if(ret.second == true )
				  v.push_back(*it);
		  }
        }
        else { // if there are no elements in A
            vector<PortProfileObject*>::iterator it;
            for (it = b.begin(); it < b.end(); it++) {

                v.push_back(*it);
            }
        }
		std::sort(v.begin(), v.end(), compareByName);
        return v;
    }

    void VcenterDiff::removeDuplicates(vector<PortProfileObject*>& a) 
    {

      set<string> inserts;
      vector<PortProfileObject*> v;
      pair<set<string>::iterator,bool> ret;
      vector<PortProfileObject*>::iterator it;
      for (it = a.begin(); it < a.end(); it++) {
            ret = inserts.insert((*it)->getProfileName());
            if(ret.second == true )
              v.push_back(*it);
      }
      a.assign( v.begin(), v.end() );
    }


    void VcenterDiff::deleteLegacyPortProfiles()
    {
	vector <string> ppnames;
	ValClientSynchronousConnection *pValClientSynchronousConnection;
	string pattern = "auto-%";
	vector<string>::iterator it_pp;
	pValClientSynchronousConnection = DcmToolKit::getInterDcmCcmClient();
	pValClientSynchronousConnection->getAllPortProfilesFromDcmd(pattern, ppnames);

    SwitchAdapter *switchAdapter = new SwitchAdapter();
    string profile_name;

    Debug::VIC_PLUG_TRACE(TRACE_LEVEL_DEBUG," deleteLegacyPortProfiles:: PP size: %d", ppnames.size());
	if(ppnames.size() > 0)
	{
	  for (it_pp = ppnames.begin(); it_pp < ppnames.end(); it_pp++){
    	  profile_name = *it_pp;
      	  Debug::VIC_PLUG_TRACE(TRACE_LEVEL_DEBUG,"Deleting Legacy PortProfile : %s\n",profile_name.c_str());
	      switchAdapter->deActivatePortProfile(profile_name);
    	  switchAdapter->deletePortProfile(profile_name);
		}
	}
    /*if (pResults) {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    }*/
    delete switchAdapter;

    }

  vector<PortProfileObject*> VcenterDiff::intersect(vector<PortProfileObject*> a, vector<PortProfileObject*> b) 
	{
        cout << __FUNCTION__ << endl;
        vector<PortProfileObject*> v;
		set<string> inserts;
		pair<set<string>::iterator,bool> ret;
        if (a.size() > 0 && b.size() > 0) {
		  vector<PortProfileObject*>::iterator it;
			for (it = a.begin(); it < a.end(); it++) {
                ret = inserts.insert((*it)->getProfileName());
          }
         for (it = b.begin(); it < b.end(); it++) {

                ret = inserts.insert((*it)->getProfileName());
                if(ret.second == false )
                  v.push_back(*it);
          }

        }

		std::sort(v.begin(), v.end(), compareByName);
        return v;
    }

    multimap<string, string> VcenterDiff::createMapOfProfileToMac(string vCenter, vector<Domain*> v) 
	{
	  	cout << __FUNCTION__ << endl;
        multimap<string, string> portProfileMacMap;
        vector<Domain*>::iterator it;
        string mac;
        string dc_id_pgName;
        for (it = v.begin(); it != v.end(); it++) {
            mac = URLEncode::mac_conver_format((*it)->getMac());
            dc_id_pgName =
                PROFILE_NAME(vCenter,(*it)->getDcId(),(*it)->getPgNN());
            portProfileMacMap.insert(
                    pair<string, string>(dc_id_pgName, mac));
        }
        return portProfileMacMap;
    }

    vector<Domain*> VcenterDiff::getSource(daoType_t type, string db_file, string mor) {
        cout << __FUNCTION__ << " :" << db_file << endl;
        DAO* dao = DaoFactory::getDao(type, db_file);
        vector<Domain*> v = dao->retrieveInfoForCorrelation(mor);
        DAO::closeDb();
        return v;
    }

	void VcenterDiff::configureVlans (string vcenter, vector<PortProfileObject*>  added_vector, vector<PortProfileObject*> updated_vector) {
        vector<PortProfileObject*>::iterator it;
		SwitchAdapter *switchAdapter = new SwitchAdapter(vcenter);
        string vlanScope = "";
		string vlanStr;
		UI32Range VlanRange = UI32Range("");
        for (it = added_vector.begin(); it < added_vector.end();it++) {
			if((*it)->getisAccessMode())
            	vlanScope = vlanScope + "," + (*it)->getAccessVlan();
			else {
                VlanRange = (*it)->getVlanRange();
                vlanStr = VlanRange.toString();
				vlanScope = vlanScope + "," + vlanStr;
			}
        }

        for (it = updated_vector.begin();it < updated_vector.end(); it++) {
            if((*it)->getisAccessMode())
                vlanScope = vlanScope + "," + (*it)->getAccessVlan();
            else {
                VlanRange = (*it)->getVlanRange();
                vlanStr = VlanRange.toString();
                vlanScope = vlanScope + "," + vlanStr;
			}
        }

        std::string vlanScopeStr = VcenterUtils::convertToVlanUI32Range(vlanScope);
        Debug::VIC_PLUG_TRACE(TRACE_LEVEL_DEBUG,"Valn scope2 str = %s\n",vlanScopeStr.c_str());
        switchAdapter->createVlans(vlanScopeStr);
		delete switchAdapter;
    }

	bool VcenterDiff::compareByName(PortProfileObject* a, PortProfileObject* b)
	{
	    string aName = a->getProfileName();
		string bName = b->getProfileName();

        return strcmp (aName.c_str(),bName.c_str() ) < 0;
	}
}//DcmNs
