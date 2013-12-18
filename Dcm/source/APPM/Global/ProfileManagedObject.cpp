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
 *   Author : kverma, bgangise, asharma,sriramr                            *
 **************************************************************************/

#include "APPM/Global/ProfileManagedObject.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalConfWorker.h"
#include "APPM/Global/ProfileMacManagedObject.h"
#include "APPM/Global/SubProfileManagedObject.h"
#include "APPM/Global/SecProfileCfgManagedObject.h"
#include "APPM/Global/FcoeProfileCfgManagedObject.h"
#include "APPM/Global/QosProfileCfgManagedObject.h"
#include "APPM/Global/VlanProfileCfgManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Utils/FrameworkToolKit.h"
#include "APPM/Global/APPMGlobalTypes.h"
#include "APPM/Global/APPMGlobalAPPMProfileMessage.h"
#include "APPM/Global/APPMGlobalAPPMSecurityMessage.h"
#include "APPM/Global/APPMGlobalAPPMFcoeMessage.h"
#include "Ssm/Global/StdMacAclPolicyManagedObject.h"
#include "Ssm/Global/ExtMacAclPolicyManagedObject.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/APPM/APPMProfileMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
//#include "vcs.h"
#include "brocade-port-profile.h"
#include "DcmCStatus.h"
#include <raslog/raslog.h>
#include <raslog/raslog_nsm.h>

namespace DcmNs
{

    ProfileManagedObject::ProfileManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (ProfileManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager)
    {
		m_isActivated = false;
		m_isAllow	  = false;
		m_isFloodingRestricted = false;
    }

    ProfileManagedObject::ProfileManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const string &profileName,const UI32 &profileId,const bool &isActivated,const vector<WaveManagedObjectPointer<ProfileMacManagedObject> > &assocMacs,const bool &isAllow,const bool &isFloodingRestricted,const vector<WaveManagedObjectPointer<SubProfileManagedObject> > &subProfileCfg)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (ProfileManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_isActivated    (isActivated),
        m_assocMacs    (assocMacs),
		m_isAllow		(isAllow),
		m_isFloodingRestricted  (isFloodingRestricted),
        m_subProfileCfg    (subProfileCfg)
    {
    }

    ProfileManagedObject::~ProfileManagedObject ()
    {
    }

    string  ProfileManagedObject::getClassName()
    {
        return ("ProfileManagedObject");
    }

    void  ProfileManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_profileName,"profileName", appm_name));
        addPersistableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addPersistableAttribute (new AttributeBool(&m_isActivated,"isActivated", appm_activate));
        addPersistableAttribute (new AttributeBool(&m_isAllow,"isAllow",appm_nonprofiledmacs));
		addPersistableAttribute (new AttributeBool(&m_isFloodingRestricted,"isFloodingRestricted",appm_restrict_flooding));
	addPersistableAttribute (new AttributeManagedObjectVectorComposition<ProfileMacManagedObject>(&m_assocMacs,"assocMacs", getClassName (), getObjectId (), ProfileMacManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<SubProfileManagedObject>(&m_subProfileCfg,"subProfileCfg", getClassName (), getObjectId (), SubProfileManagedObject::getClassName ()));

    }

    void  ProfileManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_profileName,"profileName", appm_name));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_profileId,"profileId"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isActivated,"isActivated", appm_activate));
		addPersistableAttributeForCreate (new AttributeBool(&m_isAllow,"isAllow",appm_nonprofiledmacs));
		addPersistableAttributeForCreate (new AttributeBool(&m_isFloodingRestricted,"isFloodingRestricted",appm_restrict_flooding));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<ProfileMacManagedObject>(&m_assocMacs,"assocMacs", getClassName (), getObjectId (), ProfileMacManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<SubProfileManagedObject>(&m_subProfileCfg,"subProfileCfg", getClassName (), getObjectId (), SubProfileManagedObject::getClassName ()));

        vector<string > keyName;
        keyName.push_back ("profileName");
        setUserDefinedKeyCombination (keyName);
    }

    void  ProfileManagedObject::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  ProfileManagedObject::getProfileName() const
    {
        return (m_profileName);
    }

    void  ProfileManagedObject::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  ProfileManagedObject::getProfileId() const
    {
        return (m_profileId);
    }

    void  ProfileManagedObject::setIsActivated(const bool &isActivated)
    {
        m_isActivated  =  isActivated;
    }

    bool  ProfileManagedObject::getIsActivated() const
    {
        return (m_isActivated);
    }
	void  ProfileManagedObject::setIsAllow(const bool &isAllow)
    {
        m_isAllow  =  isAllow;
    }

    bool  ProfileManagedObject::getIsAllow() const
    {
        return (m_isAllow);
    }

	void ProfileManagedObject::setIsFloodingRestricted(const bool &isFloodingRestricted) {
        m_isFloodingRestricted = isFloodingRestricted;
    }

    bool ProfileManagedObject::getIsFloodingRestricted() const {
        return m_isFloodingRestricted;
    }

    void  ProfileManagedObject::setAssocMacs(const vector<WaveManagedObjectPointer<ProfileMacManagedObject> > &assocMacs)
    {
        m_assocMacs  =  assocMacs;
    }

    vector<WaveManagedObjectPointer<ProfileMacManagedObject> >  ProfileManagedObject::getAssocMacs() const
    {
        return (m_assocMacs);
    }
	
    void ProfileManagedObject::addProfileMacToVector(ProfileMacManagedObject *profileMacMO)
    {
		WaveManagedObjectPointer<ProfileMacManagedObject> profileMac(profileMacMO);
		m_assocMacs.push_back(profileMac);
    }

    void  ProfileManagedObject::delProfileMacFromVector(int index)
    {
		m_assocMacs.erase(m_assocMacs.begin() + index );
    }

	/* Overloaded Function */
    int ProfileManagedObject::delProfileMacFromVector(ProfileMacManagedObject *profileMacMO)
    {
		unsigned int iter = 0;
		int isFound = 0;
		string macAddr = profileMacMO->getMac();

		for (iter = 0 ; iter < m_assocMacs.size(); iter++)
		{
			if (macAddr == ((m_assocMacs[iter])->getMac())) {
				isFound = 1;
                break;
			}
		}

		if (isFound) {
			m_assocMacs.erase(m_assocMacs.begin() + iter);
			APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " deleting the "
				"MAC from the vector");
			return (1);
		} else {
			return (0);
		}
    }

    void  ProfileManagedObject::setSubProfileCfg(const vector<WaveManagedObjectPointer<SubProfileManagedObject> > &subProfileCfg)
    {
        m_subProfileCfg  =  subProfileCfg;
    }

    vector<WaveManagedObjectPointer<SubProfileManagedObject> >  ProfileManagedObject::getSubProfileCfg() const
    {
        return (m_subProfileCfg);
    }

    void ProfileManagedObject::updateActivateInfo(APPMGlobalAPPMProfileMessage *pMsg)
	{
		setIsActivated(pMsg->getIsActivated());
	}
	
	void ProfileManagedObject::addSubProfileToVector(SubProfileManagedObject *subProfileMO)
	{
		WaveManagedObjectPointer<SubProfileManagedObject> subProfile(subProfileMO);
		m_subProfileCfg.push_back(subProfile);
	}

    int ProfileManagedObject::delSubProfileFromVector(const string &subProfileName)
    {
		unsigned int iter = 0;
		int isFound = 0;

		for (iter = 0 ; iter < m_subProfileCfg.size(); iter++)
		{
			if (subProfileName == (m_subProfileCfg[iter])->getName()) {
				isFound = 1;
				break;
			}
		}

		if (isFound) {
			m_subProfileCfg.erase(m_subProfileCfg.begin() + iter);
			APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " deleting the "
				+ subProfileName);
			return (1);
		}
		return (0);
    }

	AppmReturnCode ProfileManagedObject::updateSecSubProfileInfo(PrismLinearSequencerContext *pPrismLinearSequencerContext,
																	 APPMGlobalAPPMSecurityMessage *pMsg)
	{
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
		vector<WaveManagedObject *> *pResults = NULL;
		unsigned int secMoCount = 0;
		SecProfileCfgManagedObject *pSecProfileCfgManagedObject = NULL;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SecProfileCfgManagedObject::getClassName());
		string profileName("\0");
		AppmReturnCode ret = APPM_SUCCESS;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		WaveManagedObjectSynchronousQueryContext validateSyncQueryCtxt(SecProfileCfgManagedObject::getClassName());

        int retval = 0;

		switch (pMsg->getCmdCode()) {
		case SEC_PROFILE_CREATE:
				profileName = pMsg->getProfileName();
				validateSyncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
				status = querySynchronouslyForCount (&validateSyncQueryCtxt, secMoCount);

				if (WAVE_MESSAGE_SUCCESS == status) {
					if (0 == secMoCount) {
						APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " Validation successful " 
							" No SecurityProfile Managed object exists with key: " + profileName);
					} else  {
						APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation failure " 
							"Already existing SecurityProfile Managed Object with key: " + profileName);
						goto last;
					}
				} else {
					APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " Validation not completed " 
						"  Quering DB is failed");
					goto last;
				}

			
			pSecProfileCfgManagedObject =
				new SecProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

			prismAssert (NULL != pSecProfileCfgManagedObject, __FILE__, __LINE__);

			/* Description member field is used as ProfileName */
			pSecProfileCfgManagedObject->setDescription(pMsg->getProfileName());
			/* Using name to store the sub profile context */
			pSecProfileCfgManagedObject->setName(string("security-profile"));

			addSubProfileToVector((SubProfileManagedObject *) pSecProfileCfgManagedObject);
			APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " created "
				"the security sub-profile");
			break;

		case SEC_PROFILE_DELETE:
			retval = delSubProfileFromVector("security-profile");
			if (retval == 1) {
				APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " deleted "
					"the security sub-profile");
			} else {
				APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " secutiry "
					"sub-profile is not found");
				return (APPM_FAILURE);
			}
			break;

		case MAC_ACCESS_GRP_ADD:
			profileName = pMsg->getProfileName();

			syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
			pResults = querySynchronously (&syncQueryCtxt);

			if (NULL != pResults)
			{
				UI32 numberOfResults = pResults->size ();
				if (0 == numberOfResults) {
					APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " for "
						" the profile " + profileName + " security-profile is not found");
					ret = APPM_FAILURE;
					goto last;
				}else if (1 == numberOfResults) {
					pSecProfileCfgManagedObject = dynamic_cast<SecProfileCfgManagedObject *>((*pResults)[0]);
					APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " for "
						" the profile " + profileName + " security-profile is found");
				} else {
					APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " for "
						" the profile " + profileName + " multiple security-profiles are found");
					ret = APPM_FAILURE;
					goto last;
				}
			}

			if (pSecProfileCfgManagedObject != NULL) {
				updateWaveManagedObject(pSecProfileCfgManagedObject);
				pSecProfileCfgManagedObject->setAccessGroup(pMsg->getAccessGroup());
				if (pMsg->getAccessGroup().compare("") != 0)
					pSecProfileCfgManagedObject->setAccessIn(true);
			} else {
				prismAssert (NULL != pSecProfileCfgManagedObject, __FILE__, __LINE__);
				ret = APPM_FAILURE;
				goto last;
			}
			break;

		case MAC_ACCESS_GRP_DEL:
			profileName = pMsg->getProfileName();

			syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
			pResults = querySynchronously (&syncQueryCtxt);

			if (NULL != pResults)
			{
				UI32 numberOfResults = pResults->size ();
				if (0 == numberOfResults) {
					APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " for "
						" the profile " + profileName + "security-profile is not found");
					ret = APPM_FAILURE;
					goto last;
				}else if (1 == numberOfResults) {
					pSecProfileCfgManagedObject = dynamic_cast<SecProfileCfgManagedObject *>((*pResults)[0]);
					APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " for "
						" the profile " + profileName + "security-profile is found");
				} else {
					APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " for "
						" the profile " + profileName + " multiple security-profiles are found");
					ret = APPM_FAILURE;
					goto last;
				}
			}

			if (pSecProfileCfgManagedObject != NULL) {
				updateWaveManagedObject(pSecProfileCfgManagedObject);
				pSecProfileCfgManagedObject->setAccessGroup("");
				pSecProfileCfgManagedObject->setAccessIn(false);
			} else {
				prismAssert (NULL != pSecProfileCfgManagedObject, __FILE__, __LINE__);
				return (APPM_FAILURE);
			}
			break;

		default:
			APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " for "
				" the profile " + profileName + "receiving the unknown cmdcode");
			ret = APPM_FAILURE;
			goto last;
		}

last:
        if (pResults != NULL) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			delete (pResults);
        }

		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_INFO);
		return (ret);
	}

	AppmReturnCode ProfileManagedObject::updateFcoeSubProfileInfo(PrismLinearSequencerContext *pPrismLinearSequencerContext,
																	   APPMGlobalAPPMFcoeMessage *pMsg)
	{
		vector<WaveManagedObject *> *pResults = NULL;
		FcoeProfileCfgManagedObject *pFcoeProfileCfgManagedObject = NULL;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeProfileCfgManagedObject::getClassName());
		string profileName("\0");
		string map_name("\0");
		AppmReturnCode ret = APPM_SUCCESS;

		switch (pMsg->getCmdCode()) {
		case FCOE_PROFILE_CREATE:
			pFcoeProfileCfgManagedObject =
				new FcoeProfileCfgManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

			prismAssert (NULL != pFcoeProfileCfgManagedObject, __FILE__, __LINE__);

			/* Description member field is used as ProfileName */
			pFcoeProfileCfgManagedObject->setDescription(pMsg->getProfileName());
			/* Using name to store the sub profile context */
			pFcoeProfileCfgManagedObject->setName(string("fcoe-profile"));
			addSubProfileToVector((SubProfileManagedObject *) pFcoeProfileCfgManagedObject);
			APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " created "
				"the fcoe sub-profile");
			break;

		case FCOE_PROFILE_FCOEPORT_ADD:
			profileName = pMsg->getProfileName();
			map_name = pMsg->getMapName();

			syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
			pResults = querySynchronously (&syncQueryCtxt);

			if (NULL != pResults)
			{
				UI32 numberOfResults = pResults->size ();
				if (0 == numberOfResults) {
					APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " for "
						"the profile " + profileName + " fcoe-profile is not found");
					ret = APPM_FAILURE;
					goto last;
				}else if (1 == numberOfResults) {
					pFcoeProfileCfgManagedObject = dynamic_cast<FcoeProfileCfgManagedObject *>((*pResults)[0]);
					APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " for "
						" the profile " + profileName + "fcoe-profile is found");
				} else {
					APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " for "
						" the profile " + profileName + " multiple fcoe-profiles are found");
					ret = APPM_FAILURE;
					goto last;
				}
			}

			if (pFcoeProfileCfgManagedObject != NULL) {
				updateWaveManagedObject(pFcoeProfileCfgManagedObject);
				pFcoeProfileCfgManagedObject->setFcoePort(pMsg->getFcoePort());
				pFcoeProfileCfgManagedObject->setMapName(pMsg->getMapName());
			} else {
				prismAssert (NULL != pFcoeProfileCfgManagedObject, __FILE__, __LINE__);
				ret = APPM_FAILURE;
				goto last;
			}
			break;

		case FCOE_PROFILE_FCOEPORT_DEL:
			profileName = pMsg->getProfileName();
			syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
			pResults = querySynchronously (&syncQueryCtxt);

			if (NULL != pResults)
			{
				UI32 numberOfResults = pResults->size ();
				if (0 == numberOfResults) {
					APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " for "
						" the profile " + profileName + "fcoe-profile is not found");
					ret = APPM_FAILURE;
					goto last;
				}else if (1 == numberOfResults) {
					pFcoeProfileCfgManagedObject = dynamic_cast<FcoeProfileCfgManagedObject *>((*pResults)[0]);
					APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " for "
						" the profile " + profileName + "fcoe-profile is found");
				} else {
					APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, " for "
						" the profile " + profileName + " multiple fcoe-profiles are found");
					ret = APPM_FAILURE;
					goto last;
				}
			}

			if (pFcoeProfileCfgManagedObject != NULL) {
				updateWaveManagedObject(pFcoeProfileCfgManagedObject);
				pFcoeProfileCfgManagedObject->setFcoePort(false);
				pFcoeProfileCfgManagedObject->setMapName("");
			} else {
				prismAssert (NULL != pFcoeProfileCfgManagedObject, __FILE__, __LINE__);
				ret = APPM_FAILURE;
				goto last;
			}
			break;

		default:
			APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " for "
				" the profile " + profileName + "receiving the unknown cmdcode");
			ret = APPM_FAILURE;
			goto last;
		}

last:
        if (pResults != NULL) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			delete (pResults);
        }

		return (ret);
	}

    ResourceId ProfileManagedObject::verifySecurityProfile(const string aclName)
    {
        string stdAclclass;
        string extAclclass;

        ResourceId status = WAVE_MESSAGE_SUCCESS;

        /* As of now only verification is whether Acl exists or not. */
        stdAclclass = StdMacAclPolicyManagedObject::getClassName();
        extAclclass = ExtMacAclPolicyManagedObject::getClassName();

        WaveManagedObjectSynchronousQueryContext stdAclQueryCtxt(stdAclclass);
        WaveManagedObjectSynchronousQueryContext extAclQueryCtxt(extAclclass);
        stdAclQueryCtxt.addAndAttribute (new AttributeString (aclName, "policyName"));
        extAclQueryCtxt.addAndAttribute (new AttributeString (aclName, "policyName"));
        
        vector<WaveManagedObject *> *pResults1 = querySynchronously (&stdAclQueryCtxt);
        vector<WaveManagedObject *> *pResults2 = querySynchronously (&extAclQueryCtxt);

        if ((pResults1 && (pResults1->size() > 0)) || (pResults2 && (pResults2->size() > 0)))
       		status = WRC_SSM_DCM_ERR_ACL_EEXIST;

		if (pResults1)
			delete pResults1;

		if (pResults2)
			delete pResults2;

        return status;
    }

#if 0
    ResourceId ProfileManagedObject::appmVerifyQosConflict()
    {
        UI8 ret = APPM_DCM_SUCCESS;

        /*
         * TODO :: Add verification Code,
         * change function arguments for OpCode & CmdCode
         * if required.
         */
    
        return ret;
    }
#endif

    UI8 ProfileManagedObject::appmVerifyFcoeQosConflict()
    {
        vector<WaveManagedObject *> *pResults;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(QosProfileCfgManagedObject::getClassName());
        ObjectId ceeMapObjId = ObjectId::NullObjectId;
        QosProfileCfgManagedObject *pQosProfileCfgManagedObject = NULL;
        UI8 ret = APPM_DCM_SUCCESS;

        syncQueryCtxt.addAndAttribute (new AttributeString (m_profileName, "description"));
        pResults = querySynchronously (&syncQueryCtxt);

        /* return success if qos-profile hasn't been configured */
        if (!pResults) {
            return APPM_DCM_SUCCESS;
       	}
        if (!(pResults->size())) {
        	delete pResults;
            return APPM_DCM_SUCCESS;
        }

        pQosProfileCfgManagedObject = dynamic_cast<QosProfileCfgManagedObject *>((*pResults)[0]);
        prismAssert (NULL != pQosProfileCfgManagedObject, __FILE__, __LINE__);
        ceeMapObjId = pQosProfileCfgManagedObject->getCeeMapId();

        if (ceeMapObjId != ObjectId::NullObjectId)
            ret = APPM_DCM_ERROR;

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        return ret;
    }

    void ProfileManagedObject::removeQosSubProfile()
    {
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt
            (QosProfileCfgManagedObject::getClassName());
        QosProfileCfgManagedObject *pQosProfileCfgManagedObject = NULL;
        string profileName = m_profileName;

        syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
        vector<WaveManagedObject *> *pResults;
        pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Deleting qos-profile Managed object : Can't find the policy object");
            }else if (1 == numberOfResults) {
                pQosProfileCfgManagedObject = dynamic_cast<QosProfileCfgManagedObject *>((*pResults)[0]);
                APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Deleting qos-profile Managed object : Found Managed Object");
            } else {
                APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Deleting qos-profile Managed object : Two or more objects exists");
            }
            pResults->clear ();
            delete pResults;
        }

        if (pQosProfileCfgManagedObject != NULL) {

            prismAssert (NULL != pQosProfileCfgManagedObject, __FILE__, __LINE__);
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string (pQosProfileCfgManagedObject->getDescription()));
            delete pQosProfileCfgManagedObject;
        }
    }
/* 
*The function checks whether Vlan subprofile configuration exists or not .
*/ 
	bool ProfileManagedObject::checkVlanSubProfile()
	{
		string profileName = m_profileName;
		bool vlanConfigPresent = false;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = NULL;
		if (NULL != pResults)
		{
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Checking Vlan-profile Managed object : Can't find the policy object");
			}else if (1 == numberOfResults) {
				pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject *>((*pResults)[0]);
				APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG, "Checking Vlan-profile Managed object : Found Managed Object");
			} else {
				APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Checking Vlan-profile Managed object : Two or more objects exists");
			}
		}
		if (pVlanProfileCfgManagedObject != NULL) {
			APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string (pVlanProfileCfgManagedObject->getDescription()));
			if (pVlanProfileCfgManagedObject->getSwModeFlag()){
				APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string(" SwModeFlag is set "));
				vlanConfigPresent = true;			
			}
			else {
				APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, string(" SwModeFlag is not set "));
			}
		}
		if (pResults != NULL) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
		return vlanConfigPresent;	
	}



    void ProfileManagedObject::removeVlanSubProfile()
    {
        string profileName = m_profileName;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanProfileCfgManagedObject::getClassName());

        syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = NULL;

        if (NULL != pResults)
        {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Deleting Vlan-profile Managed object : Can't find the policy object");
            }else if (1 == numberOfResults) {
                pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject *>((*pResults)[0]);
                APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, "Deleting Vlan-profile Managed object : Found Managed Object");
            } else {
                APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, "Deleting Vlan-profile Managed object : Two or more objects exists");
            }
            pResults->clear ();
            delete pResults;
        }

        if (pVlanProfileCfgManagedObject != NULL) {
            prismAssert (NULL != pVlanProfileCfgManagedObject, __FILE__, __LINE__);
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string (pVlanProfileCfgManagedObject->getDescription()));
            delete pVlanProfileCfgManagedObject;
        }
    }

    AppmReturnCode ProfileManagedObject::getAclName(string &aclName, string profileName )
    {
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        AppmReturnCode status = APPM_SUCCESS;
        string profile_name = profileName;
        vector<WaveManagedObject *> *pResults = NULL;
        SecProfileCfgManagedObject *pSecProfileCfgManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SecProfileCfgManagedObject::getClassName());

        syncQueryCtxt.addAndAttribute (new AttributeString (profile_name, "description"));
        syncQueryCtxt.addSelectField("accessGroup");
        pResults = querySynchronously (&syncQueryCtxt);
        
        if (!pResults || !(pResults->size())){

            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("APPM :: Inside :: getAclName Acl Absent"));
			if ( pResults )
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return APPM_FAILURE;
        }

	    pSecProfileCfgManagedObject = dynamic_cast<SecProfileCfgManagedObject *>((*pResults)[0]);
        prismAssert (NULL != pSecProfileCfgManagedObject, __FILE__, __LINE__);
   	    aclName = pSecProfileCfgManagedObject->getAccessGroup();

       	if (aclName == "")
        {
   	        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " Return ERROR");
       	    status = APPM_FAILURE;
        }
        
   	    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, "APPM :: Inside :: getAclName :: " + aclName);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_INFO);
        return status;
    }

    UI8 ProfileManagedObject::sendCreationRequestToClient() {

        //int status = WAVE_MESSAGE_SUCCESS;
        APPMProfileMessage *nsm_msg = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, "APPM_POSTBOOT : Creating Port_Profile <%s>" + m_profileName);

        /* first creating a port-profile */
        nsm_msg = new APPMProfileMessage;
        prismAssert(NULL != nsm_msg, __FILE__, __LINE__);

        nsm_msg->setProfileName(m_profileName);
        nsm_msg->setOpCode(APPM_CREATE);
        nsm_msg->setCmdCode(PORT_PROFILE_CREATE);
        nsm_msg->setProfileId(m_profileId);

        status = sendSynchronouslyToWaveClient ("nsm", nsm_msg);

        if (WAVE_MESSAGE_SUCCESS == status) {
            if( WAVE_MESSAGE_SUCCESS == nsm_msg->getCompletionStatus()) {
                if((WAVE_MESSAGE_SUCCESS == nsm_msg->getClientStatus())
                    || (0 == nsm_msg->getClientStatus())) {
                } else {
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("APPM_POSTBOOT : Creating Port Profile<") + m_profileName 
                            + ">failed with clientstatus<" + FrameworkToolKit::localize(nsm_msg->getClientStatus()) + ">");
                }
            } else {
                APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("APPM_POSTBOOT : Creating Port Profile<") + m_profileName
                        + ">failed with completionstatus<" + FrameworkToolKit::localize(nsm_msg->getCompletionStatus()) + ">");
            }
        } else {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("APPM_POSTBOOT : Creating Port Profile<") + m_profileName
                    + ">failed with status<" + FrameworkToolKit::localize(status) + ">");
        }

        delete nsm_msg;
        DcmNs::setGlobalPPid(m_profileId);
        return 1;
    }


	UI8 ProfileManagedObject::sendIsAllowRequestToClient()
    {
        int status = WAVE_MESSAGE_SUCCESS;
        int ret = 1;
        ResourceId ClientStatus = WAVE_MESSAGE_SUCCESS;
        APPMProfileMessage *nsm_msg = NULL;

        trace (TRACE_LEVEL_DEBUG, "APPM_POSTBOOT : Creating Port_Profile %s"+m_profileName);

        nsm_msg = new APPMProfileMessage;
        nsm_msg->setProfileName(m_profileName);
        nsm_msg->setOpCode(APPM_SET_ELEM);
        nsm_msg->setCmdCode(PORT_PROFILE_ALLOW_NONPROFILEDMACS);
        nsm_msg->setIsAllow(m_isAllow);

        status = sendSynchronouslyToWaveClient ("nsm", nsm_msg);

        if (status == WAVE_MESSAGE_SUCCESS)
        {
            if (nsm_msg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
            {
                ret = 0;
            }
        }
                 
		if (ret)  {
			ClientStatus = nsm_msg->getClientStatus();
			if (ClientStatus  != WAVE_MESSAGE_SUCCESS){
				status = NsmUtils::mapBackEndErrorsToDcmdErrors(ClientStatus);
            if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_DEBUG, "Error From Nsm client");
            ret = 0;
        		}
			}
          }

        delete nsm_msg;
        return ret;
    }
	
   UI8 ProfileManagedObject::sendIsFloodingRestrictedRequestToClient() {
	int status = WAVE_MESSAGE_SUCCESS;
	int ret = 1;
	ResourceId ClientStatus = WAVE_MESSAGE_SUCCESS;
	APPMProfileMessage *nsm_msg = NULL;

	trace(
			TRACE_LEVEL_DEBUG,
			"APPM_POSTBOOT : Sending flooding request Port_Profile %s"
					+ m_profileName);

	nsm_msg = new APPMProfileMessage;
	nsm_msg->setProfileName(m_profileName);
	nsm_msg->setOpCode(APPM_SET_ELEM);
	nsm_msg->setCmdCode(PORT_PROFILE_RESTRICT_FLOODING);
	nsm_msg->setIsFloodingRestricted(m_isFloodingRestricted);

	status = sendSynchronouslyToWaveClient("nsm", nsm_msg);

	if (status == WAVE_MESSAGE_SUCCESS) {
		if (nsm_msg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
			ret = 0;
		}
	}

	if (ret) {
		ClientStatus = nsm_msg->getClientStatus();
		if (ClientStatus != WAVE_MESSAGE_SUCCESS) {
			status = NsmUtils::mapBackEndErrorsToDcmdErrors(ClientStatus);
			if (status != WAVE_MESSAGE_SUCCESS) {
				trace(TRACE_LEVEL_DEBUG, "Error From Nsm client");
				ret = 0;
			}
		}
	}

		delete nsm_msg;
		return ret;
	} 	

    ResourceId ProfileManagedObject::ProfileActivationValidation()
    {
        unsigned int iter = 0;
		string subProfileName;
		UI32 activationStatus = WAVE_MESSAGE_SUCCESS;
        string profileName = this->getProfileName();
        
        /* If Vlan SubProfile Configuration does not exists return error.*/
		if (!(this->checkVlanSubProfile())){
			activationStatus = WRC_APPM_ERR_PROFILE_ACTIVATE_NOT_ALLOWED;
            raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_FRCLOG, NSM_2001, profileName.c_str(), "vlan subprofile not configured");
			return activationStatus;
		}

        for (iter = 0; iter < m_subProfileCfg.size(); iter++)
        {
            activationStatus = (m_subProfileCfg[iter])->activationValidate();
			
            if (activationStatus != WAVE_MESSAGE_SUCCESS)
                break;
        }
		return activationStatus;
	}

    UI8 ProfileManagedObject::sendActivationRequestToClient()
    {
        int status1 = WAVE_MESSAGE_SUCCESS;
        APPMProfileMessage *nsm_msg = NULL;

        /* Sending the profile activation status */
        if (m_isActivated)
        {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, "APPM_POSTBOOT : Activating Port_Profile %s"+m_profileName);
            nsm_msg = new APPMProfileMessage;
	        prismAssert(NULL != nsm_msg, __FILE__, __LINE__);
			
            nsm_msg->setProfileName(m_profileName);
            nsm_msg->setIsActivated(true);
            nsm_msg->setOpCode(APPM_SET_ELEM);
            nsm_msg->setCmdCode(PORT_PROFILE_ACTIVATE);
            nsm_msg->setProfileId(m_profileId);

            status1 = sendSynchronouslyToWaveClient ("nsm",nsm_msg);

            if(status1 == WAVE_MESSAGE_SUCCESS)
            {
                if (nsm_msg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
                {
                    delete nsm_msg;
                    return 0;
                }
            }

            delete nsm_msg;
        } 
        
        return 1;
    }

    UI8 ProfileManagedObject::sendAssociationRequestToClient()
    {
        int status1 = WAVE_MESSAGE_SUCCESS;
        vector<WaveManagedObjectPointer<ProfileMacManagedObject> > profileMacVector;
        string macAddr;
        unsigned int iter = 0;

        APPMProfileMessage *nsm_msg = NULL;
        
        /* sending mac associations. */
        profileMacVector = m_assocMacs;
        if(!(profileMacVector.empty()))
        {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, "APPM_POSTBOOT : Associating Port_Profile %s"+m_profileName);

            for (iter = 0 ; iter < profileMacVector.size(); iter++)
            {
                nsm_msg = new APPMProfileMessage;
		        prismAssert(NULL != nsm_msg, __FILE__, __LINE__);
				
                nsm_msg->setProfileName(m_profileName);
                nsm_msg->setOpCode(APPM_SET_ELEM);
                nsm_msg->setCmdCode(PORT_PROFILE_ASSOCIATE);
                nsm_msg->setProfileId(m_profileId);

                macAddr = (profileMacVector[iter])->getMac();
                nsm_msg->setProfileMac(macAddr);
                status1 = sendSynchronouslyToWaveClient ("nsm",nsm_msg);

                if(status1 == WAVE_MESSAGE_SUCCESS)
                {
                    if (nsm_msg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
                    {
                        delete nsm_msg;
                        return 0;
                    }
                }

                delete nsm_msg;
            }
        }
        return 1;
    }

    string ProfileManagedObject::customConfigurationDisplay (WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext)
    {

         string displayString;
        
         if (m_isActivated)
         {
                 displayString = "port-profile " + m_profileName  + " activate\n";
         }
        
         return displayString;
    }
}
