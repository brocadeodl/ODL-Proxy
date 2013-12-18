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

#include "APPM/Utils/APPMUtils.h"
#include "APPM/Global/ProfileManagedObject.h"
#include "APPM/Global/ProfileDomainManagedObject.h"
#include "APPM/Global/VlanProfileCfgManagedObject.h"
#include "APPM/Global/AMPPProfileCompositeManagedObject.h"
#include "APPM/Global/AMPPProfileDomainCompositeManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/MacGroupManagedObject.h"
#include "APPM/Global/APPMCtagBasedVlanClassificationMO.h"
#include "APPM/Global/APPMMacBasedVlanClassificationMO.h"
#include "DcmCStatus.h"
#include "DcmResourceIdEnums.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

namespace DcmNs {

    ResourceId APPMUtils::isPortProfileDomainActiveOnAnInterface(string &portProfileDomainName) {

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 count = 0;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AMPPProfileDomainCompositeManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString (portProfileDomainName, "portProfileDomainName"));
        status = querySynchronouslyForCount(&syncQueryCtxt,count);

        if ( (WAVE_MESSAGE_SUCCESS == status) ) {
            if (count > 0) {
                status = WRC_APPM_PORT_PROFILE_DOMAIN_IS_ASSOCIATED_WITH_AN_INTERFACE;
                tracePrintf(TRACE_LEVEL_INFO, "port-profile-domain <%s> is associated with atleast an interface.",
                        portProfileDomainName.c_str());
            }
        }
        return status;
    }

    ResourceId APPMUtils::isPortProfileActiveOnAnInterface(string &portProfileName) {

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 count = 0;
        //string portProfileDomain = "";
        //ProfileDomainCompositeManagedObject* pProfileDomainCompositeManagedObject = NULL;
        //ProfileDomainManagedObject* pProfileDomainManagedObject = NULL;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AMPPProfileCompositeManagedObject::getClassName());
        //WaveManagedObjectSynchronousQueryContext profileDomainCmpSyncQueryCtxt(ProfileDomainCompositeManagedObject::getClassName());

        syncQueryCtxt.addAndAttribute (new AttributeString (portProfileName, "portProfileName"));
        status = querySynchronouslyForCount(&syncQueryCtxt,count);

        if ( (WAVE_MESSAGE_SUCCESS == status) ) {
            if (count > 0) {
                status = WRC_APPM_PORT_PROFILE_IS_ASSOCIATED_WITH_AN_INTERFACE;
                tracePrintf(TRACE_LEVEL_INFO, "port-profile <%s> is associated with atleast an interface.",
                        portProfileName.c_str());
                goto last;
            }
        }

        /*Get the port-profile-domain of which the port-profile is part of and 
          check whether the port-profile-domain is associated with an interface.*/

        /*profileDomainCmpSyncQueryCtxt.addAndAttribute(new AttributeString (portProfileName, "portProfileName"));
        vector<WaveManagedObject *> *profDomainCompositeMos; 
        vector<WaveManagedObject *> *profDomainMos;

        profDomainCompositeMos = querySynchronously(&profileDomainCmpSyncQueryCtxt);

        if (NULL != profDomainCompositeMos) {
            UI32 numberOfResults = profDomainCompositeMos->size ();
            if (0 == numberOfResults) {
                tracePrintf(TRACE_LEVEL_INFO, "profiledomaincomposite mo is not found, meaning the profile is not part of any of the domain." );
            }else {
                tracePrintf(TRACE_LEVEL_INFO, "profiledomaincomposite mo is not found, meaning the profile is part of <%d> domain.", numberOfResults );
                for(unsigned int i = 0 ; i < profDomainCompositeMos->size(); i++ ) {
                    pProfileDomainCompositeManagedObject = dynamic_cast<ProfileDomainCompositeManagedObject *>((*profDomainCompositeMos)[i]);

                    WaveManagedObjectSynchronousQueryContext profileDomainSyncQueryCtxt(ProfileDomainManagedObject::getClassName());
                    profileDomainSyncQueryCtxt.addAndAttribute(new AttributeUI32 (
                                (pProfileDomainCompositeManagedObject->getOwnerManagedObjectId()).getClassId(), "objectidclassid"));
                    profileDomainSyncQueryCtxt.addAndAttribute(new AttributeUI64 (
                                (pProfileDomainCompositeManagedObject->getOwnerManagedObjectId()).getInstanceId(), "objectidinstanceid"));
                    profileDomainSyncQueryCtxt.addSelectField("portProfileDomainName");

                    profDomainMos = querySynchronously(&profileDomainSyncQueryCtxt);

                    if (NULL != profDomainMos) {
                        UI32 numOfDomains = profDomainMos->size();
                        if(0 == numOfDomains ) {
                        } else if (1 == numOfDomains ) {
                            pProfileDomainManagedObject = dynamic_cast<ProfileDomainManagedObject *>((*profDomainMos)[0]);
                            portProfileDomain = pProfileDomainManagedObject->getPortProfileDomainName();
                            status = isPortProfileDomainActiveOnAnInterface (portProfileDomain);
                            if(WAVE_MESSAGE_SUCCESS != status) {
                                goto last;
                            }
                        } else {
                            status = WAVE_MESSAGE_ERROR;
                            goto last;
                        }
                    }
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(profDomainMos);
                }
                tracePrintf(TRACE_LEVEL_INFO, "profiledomain <default> is found");
            }
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(profDomainCompositeMos);
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(profDomainMos);*/

last:
        return status;
    }

    ResourceId APPMUtils::isPortProfileActiveOnADomain(string &portProfileName) {

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 count = 0;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileDomainCompositeManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString (portProfileName, "portProfileName"));
        status = querySynchronouslyForCount(&syncQueryCtxt,count);

        if ( (WAVE_MESSAGE_SUCCESS == status) ) {
            if (count > 0) {
                status = WRC_APPM_PORT_PROFILE_DOMAIN_IS_ASSOCIATED_WITH_A_DOMAIN;
                tracePrintf(TRACE_LEVEL_INFO, "port-profile <%s> is associated with atleast a port-profile-domain.",
                        portProfileName.c_str());
            }
        }
        return status;
    }

    ResourceId APPMUtils::isPortProfileConflictingWithOtherProfilesOfTheDomain
        (string &portProfileName, string &portProfileDomainName) {
            return WAVE_MESSAGE_SUCCESS;
        }

    ResourceId APPMUtils::doesPortProfileExist(string &portProfileName) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 count = 0;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString (portProfileName, "profileName"));
        status = querySynchronouslyForCount(&syncQueryCtxt,count);

        if ( (WAVE_MESSAGE_SUCCESS == status) ) {
            if (count == 0) {
                status = WRC_APPM_PORT_PROFILE_DOESNT_EXIST;
                tracePrintf(TRACE_LEVEL_INFO, "port-profile <%s> is not created in the system.",
                        portProfileName.c_str());
            }
        }
        return status;
    }

    ResourceId APPMUtils::doesPortProfileDomainExist(string &portProfileDomainName) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 count = 0;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileDomainManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString (portProfileDomainName, "portProfileDomainName"));
        status = querySynchronouslyForCount(&syncQueryCtxt,count);

        if ( (WAVE_MESSAGE_SUCCESS == status) ) {
            if (count == 0) {
                status = WRC_APPM_PORT_PROFILE_DOMAIN_DOESNT_EXIST;
                tracePrintf(TRACE_LEVEL_INFO, "port-profile-domain <%s> is not created in the system.",
                        portProfileDomainName.c_str());
            }
        }
        return status;
    }

    ResourceId APPMUtils::validateMutuallyExclusivePortProfilePortModes
        (PoPhyIntfManagedObject* pPoPhyIntfManagedObject, UI32 cmdCode) {

            ResourceId status = WAVE_MESSAGE_SUCCESS;

            bool isPortProfiled, isPortAssociatedWithPortProfile, isPortAssociatedWithPortProfileDomain;
            bool intentPortProfiled, intentPortAssociatedWithPortProfile, intentPortAssociatedWithPortProfileDomain;

            intentPortProfiled = (NSMUPDATE_PO_PORT_PROFILED == cmdCode) 
                || (NSMUPDATE_PHY_PORT_PROFILED == cmdCode);

            intentPortAssociatedWithPortProfile = (NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PO == cmdCode) 
                || (NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT == cmdCode);

            intentPortAssociatedWithPortProfileDomain = (NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PO == cmdCode)
                || (NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT == cmdCode);

            isPortProfiled = pPoPhyIntfManagedObject->getIsPortProfiled();

            isPortAssociatedWithPortProfile = (((pPoPhyIntfManagedObject->getAssociatedPortProfiles()).size()) > 0);

            isPortAssociatedWithPortProfileDomain = 
                (NULL != (pPoPhyIntfManagedObject->getAssociatedPortProfileDomain()).operator->());

            if(isPortProfiled && (intentPortAssociatedWithPortProfile || intentPortAssociatedWithPortProfileDomain)) {
                status = WRC_APPM_PORT_PROFILE_PORT_MODES_CONFLICT_WITH_EACH_OTHER;
            }
            if(isPortAssociatedWithPortProfile && (intentPortProfiled || intentPortAssociatedWithPortProfileDomain) ) {
                status = WRC_APPM_PORT_PROFILE_PORT_MODES_CONFLICT_WITH_EACH_OTHER;
            }
            if(isPortAssociatedWithPortProfileDomain && (intentPortProfiled || intentPortAssociatedWithPortProfile)) {
                status = WRC_APPM_PORT_PROFILE_PORT_MODES_CONFLICT_WITH_EACH_OTHER;
            }
            return status;
        }

    ResourceId APPMUtils::isInterfaceAlreadyAssociatedWithAPortProfileDomain
        (PoPhyIntfManagedObject* pPoPhyIntfManagedObject) {

            ResourceId status = WAVE_MESSAGE_SUCCESS;
            if(NULL != (pPoPhyIntfManagedObject->getAssociatedPortProfileDomain()).operator->()) {
                status = WRC_APPM_INTERFACE_ALREADY_BELONGS_TO_A_PORT_PROFILE_DOMAIN;
            }
            return status;
        }

    ResourceId APPMUtils::isPortProfilePort(PoPhyIntfManagedObject* pPoPhyIntfManagedObject, bool &isPortProfileConfig) {

        bool isPortProfiled, isPortAssociatedWithPortProfile, isPortAssociatedWithPortProfileDomain;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        isPortProfiled = pPoPhyIntfManagedObject->getIsPortProfiled();
        isPortAssociatedWithPortProfile = ((pPoPhyIntfManagedObject->getAssociatedPortProfiles()).size()) > 0;
        isPortAssociatedWithPortProfileDomain =
            (NULL != (pPoPhyIntfManagedObject->getAssociatedPortProfileDomain()).operator->());

        isPortProfileConfig = isPortProfiled || isPortAssociatedWithPortProfile || isPortAssociatedWithPortProfileDomain;
        return status;
    }

    ResourceId APPMUtils::setShutdownStateOfInterface
        (PoPhyIntfManagedObject* pPoPhyIntfManagedObject, bool &isPortProfileConfig) {

            ResourceId status = WAVE_MESSAGE_SUCCESS;
            bool isProfilePort = false;
            APPMUtils::isPortProfilePort(pPoPhyIntfManagedObject, isProfilePort);
            isPortProfileConfig = !(isProfilePort);
            return status;
    }

    ResourceId APPMUtils::doesPortProfileBelongToPortProfileDomain
        (string &portProfileName, string &portProfileDomainName) {

            ResourceId status = WAVE_MESSAGE_SUCCESS;
            return status;
        }

    ResourceId APPMUtils::isVlanPresent(UI32 &vlanId) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 count = 0;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeUI32 (vlanId, "id"));
        status = querySynchronouslyForCount(&syncQueryCtxt,count);

        if ( (WAVE_MESSAGE_SUCCESS == status) ) {
            if (count == 0) {
                status = WRC_NSM_API_ERR_VLAN_NOT_CONFIGURED;
                tracePrintf(TRACE_LEVEL_INFO, "vlan <%d> is not created in the system.",
                        vlanId);
            }
            else {
                tracePrintf(TRACE_LEVEL_INFO, "vlanCount <%d>",
                        count);
            }
        }
        return status;
    }

    ResourceId APPMUtils::isVlanValidForClassification(UI32 &vlanId) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        if(vlanId <= APPM_MAX_CTAG_ID) {
            status = WRC_APPM_VLAN_CANNOT_HAVE_CLASSIFICATION;
        }
        return status;
    }

    ResourceId APPMUtils::getPortProfilesOfAPortProfileDomain
        (string &portProfileDomainName, vector<string> &portProfiles) {

            ResourceId status = WAVE_MESSAGE_SUCCESS;
            unsigned int it = 0;
            vector<WaveManagedObjectPointer<ProfileDomainCompositeManagedObject> >  associatedPortProfiles;
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ProfileDomainManagedObject::getClassName());
            syncQueryCtxt.addAndAttribute (new AttributeString (portProfileDomainName, "portProfileDomainName"));
            vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

            ProfileDomainManagedObject *pProfileDomainManagedObject = NULL;

            if (NULL != pResults) {
                UI32 numberOfResults = pResults->size ();
                if (0 == numberOfResults) {
                    tracePrintf(TRACE_LEVEL_INFO, "profiledomain <%s> is not found", portProfileDomainName.c_str());
                }else if (1 == numberOfResults) {
                    pProfileDomainManagedObject = dynamic_cast<ProfileDomainManagedObject *>((*pResults)[0]);
                    associatedPortProfiles =  pProfileDomainManagedObject->getAssociatedPortProfiles();
                    tracePrintf(TRACE_LEVEL_INFO, "profiledomain <%s> is found", portProfileDomainName.c_str());
                } else {
                    tracePrintf(TRACE_LEVEL_INFO, "more than one profiledomain <%s>  found", portProfileDomainName.c_str());
                    status = WAVE_MESSAGE_ERROR;
                }
            }

            for(it = 0 ; it < associatedPortProfiles.size(); it++) {
                portProfiles.push_back(associatedPortProfiles[it]->getPortProfileName());
            }

            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

            return status;

        }
        
    ResourceId APPMUtils::getProfilesAndDomainsofPoPhy
        (PoPhyIntfManagedObject* pMO, vector <string> &portProfiles, string &portProfileDomainName) {

            ResourceId status = WAVE_MESSAGE_SUCCESS;
            unsigned int index = 0;

            WaveManagedObjectPointer<AMPPProfileDomainCompositeManagedObject> associatedPortProfileDomain =
                pMO->getAssociatedPortProfileDomain() ;
            vector<WaveManagedObjectPointer<AMPPProfileCompositeManagedObject> > associatedPortProfiles =
                pMO->getAssociatedPortProfiles();

            if(NULL != associatedPortProfileDomain.operator->()) {
                portProfileDomainName = associatedPortProfileDomain->getPortProfileDomainName();
            }

            for(index = 0 ; index < associatedPortProfiles.size(); index++) {
                string profileName = associatedPortProfiles[index]->getPortProfileName();
                portProfiles.push_back(profileName);
            }

            return status;
        }

    ResourceId APPMUtils::isMacGroupPresent(UI32 &macGroupId) {

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        unsigned int count = 0;
        WaveManagedObjectSynchronousQueryContext macGroupQueryCtxt (MacGroupManagedObject::getClassName ());
        macGroupQueryCtxt.addAndAttribute (new AttributeUI32 (macGroupId, "id"));
        status = querySynchronouslyForCount (&macGroupQueryCtxt, count);

        if((WAVE_MESSAGE_SUCCESS == status) && (count != 1)) {
            status = WRC_NSM_ERR_MAC_GROUP_NOT_CONFIGURED;
        }
        return status;
    }

    ResourceId APPMUtils::isGvlanSpecificAPPMConfigPresent() {

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 count = 0;
        UI32 profilesCount = 0;
        UI32 profilesInDefaultDomain = 0;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(AMPPProfileDomainCompositeManagedObject::getClassName());
        WaveManagedObjectSynchronousQueryContext appmProfileCmpCtxt(AMPPProfileCompositeManagedObject::getClassName());
        WaveManagedObjectSynchronousQueryContext profileDomainSyncCtxt(ProfileDomainManagedObject::getClassName());
        WaveManagedObjectSynchronousQueryContext profileSyncCtxt(ProfileManagedObject::getClassName());
        WaveManagedObjectSynchronousQueryContext profileCmpSyncCxt(ProfileDomainCompositeManagedObject::getClassName());
        WaveManagedObjectSynchronousQueryContext macVlanClassSyncCtxt(APPMMacAddressVlanClassificationMO::getClassName()); 
        WaveManagedObjectSynchronousQueryContext macGroupVlanClassSyncCtxt(APPMMacGroupVlanClassificationMO::getClassName());


        /* Check whether there exist any port-profile-domain to interface associations */
        status = querySynchronouslyForCount(&syncQueryCtxt,count);

        if ( (WAVE_MESSAGE_SUCCESS == status) ) {
            if (count > 0) {
                status = WRC_APPM_SYSTEM_HAS_PORT_PROFILE_DOMAINS_ASSOCIATED_WITH_INTERFACES;
                tracePrintf(TRACE_LEVEL_INFO, "system has port-profile-domains associated with at least an interface.");
                goto last;
            }
        }

        /* Check whether there exist any port-profile to interface associations */
        count = 0;
        status = querySynchronouslyForCount(&appmProfileCmpCtxt,count);

        if ( (WAVE_MESSAGE_SUCCESS == status) ) {
            if (count > 0) {
                status = WRC_APPM_SYSTEM_HAS_PORT_PROFILES_ASSOCIATED_WITH_INTERFACES;
                tracePrintf(TRACE_LEVEL_INFO, "system has port-profiles associated with at least an interface.");
                goto last;
            }
        }

        /* Check whether there exist any user created port-profile-domains */
        count = 0;
        profileDomainSyncCtxt.addAndAttribute (new AttributeString ("default", "portProfileDomainName"),WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);

        status = querySynchronouslyForCount(&profileDomainSyncCtxt,count);

        if ( (WAVE_MESSAGE_SUCCESS == status) ) {
            if (count > 0) {
                status = WRC_APPM_SYSTEM_HAS_USER_CREATED_PORT_PROFILE_DOMAINS;
                tracePrintf(TRACE_LEVEL_INFO, "system has user created port-profile-domain.");
                goto last;
            }
        }

        /* Check whether there exist any vlan classifications defined in any vlan-profile.
           Deletion of all non-dot1q vlans will delete all the vlan classification MOs.
           Need to check only the MOs of not-dot1q MacBasedClassification and delete them.*/

        count = 0 ;
        status = querySynchronouslyForCount(&macVlanClassSyncCtxt, count);
        if ( (WAVE_MESSAGE_SUCCESS == status) ) {
            if (count > 0) {
                status = WRC_APPM_SYSTEM_HAS_VLAN_PROFILE_WITH_MAC_BASED_VLAN_CLASSIFICATIONS;
                tracePrintf(TRACE_LEVEL_INFO, "system has vlan-profiles which belong to no domain.");
                goto last;
            }
        }

        count = 0 ;
        status = querySynchronouslyForCount(&macGroupVlanClassSyncCtxt, count);
        if ( (WAVE_MESSAGE_SUCCESS == status) ) {
            if (count > 0) {
                status = WRC_APPM_SYSTEM_HAS_VLAN_PROFILE_WITH_MAC_GROUP_BASED_VLAN_CLASSIFICATIONS;
                tracePrintf(TRACE_LEVEL_INFO, "system has vlan-profiles witch mac-group based vlan classifications.");
                goto last;
            }
        }

        /* Check whether there exist any port-profile not belonging to the "default" domain */
        /* Get the count of port-profiles present in the default domain and the count of the port-profiles.
           If the counts differ, implies there exist isolated port-profiles (except default profile),
           which doesn't belong to any domain */


        profileSyncCtxt.addAndAttribute (new AttributeString ("default", "profileName"), 
                WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);
        status = querySynchronouslyForCount(&profileSyncCtxt,profilesCount);

        status = querySynchronouslyForCount(&profileCmpSyncCxt,profilesInDefaultDomain);

        if ( (WAVE_MESSAGE_SUCCESS == status) ) {
            if (profilesCount > profilesInDefaultDomain) {
                status = WRC_APPM_SYSTEM_HAS_USER_CREATED_PORT_PROFILES_BELONGING_TO_NO_DOMAIN;
                tracePrintf(TRACE_LEVEL_INFO, "system has port-profiles which belong to no domain.");
                goto last;
            }
        }

last:
        return status;

    }



    ResourceId APPMUtils::doesCtagOrTrunkVlanAlreadyExist
        ( string &profileName, UI32 &trunkVlanId, UI32 &ctagId) {

            ResourceId status = WAVE_MESSAGE_SUCCESS;
            UI32 ctagOrTrunkVlanCount = 0;
            //UI32 trunkVlanCount = 0;

            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
            WaveManagedObjectSynchronousQueryContext ctagClassSyncQueryCtxt(APPMCtagVlanClassificationMO::getClassName());

            vector<WaveManagedObject *> *vlanPResults;

            VlanProfileCfgManagedObject* pVlanProfileCfgManagedObject = NULL;

            syncQueryCtxt.addSelectField("objectId");
            syncQueryCtxt.addAndAttribute(new AttributeString(profileName, "description"));

            vlanPResults = querySynchronously(&syncQueryCtxt);

            if(NULL != vlanPResults ) {
                UI32 numOfRecords = vlanPResults->size();
                if ( 0 == (numOfRecords) ) {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                } else if (1 == (numOfRecords)) {
                    pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject*>((*vlanPResults)[0]);
                } else {
                    status = WAVE_MESSAGE_ERROR;
                    goto last;
                }
            } else {
                status = WAVE_MESSAGE_ERROR;
                goto last;
            }

            ctagClassSyncQueryCtxt.addAndAttribute(new AttributeUI32 (
                        (pVlanProfileCfgManagedObject->getObjectId()).getClassId(), "ownermanagedobjectidclassid"));
            ctagClassSyncQueryCtxt.addAndAttribute(new AttributeUI64 (
                        (pVlanProfileCfgManagedObject->getObjectId()).getInstanceId(), "ownermanagedobjectidinstanceid"));
            ctagClassSyncQueryCtxt.addOrAttribute(new AttributeUI32 (trunkVlanId, "vlanId"));
            ctagClassSyncQueryCtxt.addOrAttribute(new AttributeUI32 (ctagId, "ctagId"));
            status = querySynchronouslyForCount(&ctagClassSyncQueryCtxt, ctagOrTrunkVlanCount);

            if ( (WAVE_MESSAGE_SUCCESS == status) ) {
                if (ctagOrTrunkVlanCount > 0) {
                    status = WRC_APPM_CTAG_VLAN_MAPPING_SHOULD_BE_ONE_TO_ONE;
                    tracePrintf(TRACE_LEVEL_INFO, "port-profile <%s> already has a ctag-vlan mapping.",
                            profileName.c_str());
                    goto last;
                }
            }

last:
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(vlanPResults);
            return status;

        }


}
