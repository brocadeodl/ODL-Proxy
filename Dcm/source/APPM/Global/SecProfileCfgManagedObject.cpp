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
 *   Author : bgangise, asharma                                            *
 **************************************************************************/

#include "APPM/Global/SecProfileCfgManagedObject.h"
#include "APPM/Global/VlanProfileCfgManagedObject.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/APPM/APPMSecProfileMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

//#include "vcs.h"
#include "DcmCStatus.h"
#include <raslog/raslog.h>
#include <raslog/raslog_nsm.h>
#include "brocade-port-profile.h"
#include <algorithm>

using namespace std;

namespace DcmNs
{

    SecProfileCfgManagedObject::SecProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (SecProfileCfgManagedObject::getClassName (), SubProfileManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        SubProfileManagedObject (pAPPMGlobalObjectManager),
        m_accessGroup    (""),
        m_accessIn    (false)
    {
    }

    SecProfileCfgManagedObject::SecProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const string &accessGroup)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (SecProfileCfgManagedObject::getClassName (), SubProfileManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        SubProfileManagedObject (pAPPMGlobalObjectManager),
        m_accessGroup    (accessGroup),
        m_accessIn    (false)
    {
    }

    SecProfileCfgManagedObject::~SecProfileCfgManagedObject ()
    {
    }

    string  SecProfileCfgManagedObject::getClassName()
    {
        return ("SecProfileCfgManagedObject");
    }

    void  SecProfileCfgManagedObject::setupAttributesForPersistence()
    {
        SubProfileManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_accessGroup,true, "", "accessGroup", appm_access_group_name));
        addPersistableAttribute (new AttributeBool(&m_accessIn,"accessIn",appm_in));
    }

    void  SecProfileCfgManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeString(&m_accessGroup,"accessGroup",appm_access_group_name));
        addPersistableAttributeForCreate  (new AttributeBool(&m_accessIn,"accessIn",appm_in));

        vector<string > keyName;
        keyName.push_back ("description");
        setUserDefinedKeyCombination (keyName);
    }

    void  SecProfileCfgManagedObject::setAccessGroup(const string &accessGroup)
    {
        m_accessGroup  =  accessGroup;
    }

    string  SecProfileCfgManagedObject::getAccessGroup() const
    {
        return (m_accessGroup);
    }

    void  SecProfileCfgManagedObject::setAccessIn(const bool &accessIn)
    {
        m_accessIn  =  accessIn;
    }

    bool  SecProfileCfgManagedObject::getAccessIn() const
    {
        return (m_accessIn);
    }

    Attribute *SecProfileCfgManagedObject::getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined)
    {
        Attribute *pAttribute = NULL;
        pAttribute = new AttributeBool (true, "appm");
        return (pAttribute);
    }

    UI32 SecProfileCfgManagedObject::activationValidate()
    {
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        
        vector<WaveManagedObject *> *pResults = NULL;
        bool isAllowedAll = false;
        VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
 
        UI32 status = WAVE_MESSAGE_SUCCESS;
        vector<UI32> vlans;
        string profileName = getDescription();
        vlans.clear();

        if (!m_accessGroup.compare(""))
        {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, " Default Acl, Return Success");
            return status;
        }
                
        syncQueryCtxt.addAndAttribute (new AttributeString (profileName, "description"));
        pResults = querySynchronously (&syncQueryCtxt);

        if (!pResults) {
            return WAVE_MESSAGE_SUCCESS;
        }

        if (!(pResults->size()))
        {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return WAVE_MESSAGE_SUCCESS;
        }

        pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject *>((*pResults)[0]);
        prismAssert (NULL != pVlanProfileCfgManagedObject, __FILE__, __LINE__);

        isAllowedAll = pVlanProfileCfgManagedObject->getIsAllowedVlanAllV2();
        pVlanProfileCfgManagedObject->getVlans(vlans);

        status = SecurityProfileActivationValidate(isAllowedAll, vlans);
        
        if (pResults)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        /*
         * Printing Raslog for failure. For success, raslog will be printed
         * by backend.
         */
        
        if (status != WAVE_MESSAGE_SUCCESS)
        raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_FRCLOG, NSM_2001, profileName.c_str(), "Security Profile Conflict");
        return status; 
    }

    UI32 SecProfileCfgManagedObject::SecurityProfileActivationValidate(bool isAllowedAll, vector<UI32> vlans)
    {
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        //VlanIntfManagedObject *pVlanIntfManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults;
        UI32 i = 0;
        //UI32 vid = 0;
        vector<UI32>::iterator it;

        /*
         * We can optimize the query further to get just those
         * rows which have non null object id, optimization
         * will be added when dcmd support somes in.
         */
        syncQueryCtxt.addSelectField ("id");
        syncQueryCtxt.addSelectFieldForObjectId("ingressAcl");

        pResults = querySynchronously(&syncQueryCtxt);

        if (!pResults)
        {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, "pResults NULL");
            return WAVE_MESSAGE_SUCCESS;
        }

        for (i = 0; i < pResults->size() ; i++)
        {
			#if 0
        	pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject *>((*pResults)[i]);

            vid = pVlanIntfManagedObject->getId();

            Fix Me : It might break functionality of APPM untill takenc care properly.

			if ((pVlanIntfManagedObject->getIngressMacAclPolicy()) != (ObjectId::NullObjectId))
            {
         /*
                 * If we find a vlan with acl applied then,
                 * for allowed all that vlan should be in the
                 * remove/except list , else it should not be 
                 * in the vlan list.
                 */
                it = find(vlans.begin(), vlans.end(), vid);

                if ( isAllowedAll )
                {
                    if (it == vlans.end())
                    {
                        if (pResults)
                            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

                        return WRC_APPM_ERR_PROFILE_ACTIVATION_SEC;
                    }
                } else if (it != vlans.end())
                {
                    if (pResults)
                        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

                    return WRC_APPM_ERR_PROFILE_ACTIVATION_SEC;
                }
            }
			#endif
        }

        if (pResults)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_INFO);
        return WAVE_MESSAGE_SUCCESS;
    }

    UI8 SecProfileCfgManagedObject::sendBulkRequestToClient(const string profile_name, const UI32 profile_id)
    {
        int status = WAVE_MESSAGE_SUCCESS;
        APPMSecProfileMessage *pMsg = new APPMSecProfileMessage;
        prismAssert(NULL != pMsg, __FILE__, __LINE__);

        /* send security-profile cli. */
        pMsg->setOpCode(APPM_CREATE);
        pMsg->setCmdCode(SEC_PROFILE_CREATE);
        pMsg->setProfileName(profile_name);
        pMsg->setProfileId(profile_id);
        pMsg->setAccessGroup(m_accessGroup);

        status = sendSynchronouslyToWaveClient("ssm",pMsg);

        if (status == WAVE_MESSAGE_SUCCESS && pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
			delete pMsg;
			return 0;
        }
        delete pMsg;

        /* send mac-access-group cli. */
        if (m_accessGroup != "")
        {
            pMsg = new APPMSecProfileMessage;
	        prismAssert(NULL != pMsg, __FILE__, __LINE__);
			
            pMsg->setOpCode(APPM_SET_ELEM);
            pMsg->setCmdCode(MAC_ACCESS_GRP_ADD);
            pMsg->setProfileName(profile_name);
            pMsg->setProfileId(profile_id);
            pMsg->setAccessGroup(m_accessGroup);

            status = sendSynchronouslyToWaveClient("ssm",pMsg);

            if (status == WAVE_MESSAGE_SUCCESS && pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
				delete pMsg;
				return 0;
            }
			delete pMsg;
        }

        return 1;
    }
}
