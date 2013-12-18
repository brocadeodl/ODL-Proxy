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

#include "APPM/Global/FcoeProfileCfgManagedObject.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/APPM/APPMFcoeProfileMessage.h"

namespace DcmNs
{

    FcoeProfileCfgManagedObject::FcoeProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (FcoeProfileCfgManagedObject::getClassName (), SubProfileManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        SubProfileManagedObject (pAPPMGlobalObjectManager)
    {
		m_fcoePort = false;
    }

    FcoeProfileCfgManagedObject::FcoeProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const bool &fcoePort)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (FcoeProfileCfgManagedObject::getClassName (), SubProfileManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        SubProfileManagedObject (pAPPMGlobalObjectManager),
        m_fcoePort    (fcoePort)
	
    {
    }

    FcoeProfileCfgManagedObject::~FcoeProfileCfgManagedObject ()
    {
    }

    string  FcoeProfileCfgManagedObject::getClassName()
    {
        return ("FcoeProfileCfgManagedObject");
    }

    void  FcoeProfileCfgManagedObject::setupAttributesForPersistence()
    {
   	SubProfileManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_fcoePort,"fcoePort",appm_fcoeport));
        addPersistableAttribute (new AttributeString(&m_mapName,true,"" ,"map_name", appm_fcoe_map_name));  
    }

    void  FcoeProfileCfgManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeBool(&m_fcoePort,"fcoePort",appm_fcoeport));
        addPersistableAttributeForCreate  (new AttributeString(&m_mapName,"map_name",appm_fcoe_map_name));
        vector<string > keyName;
        keyName.push_back ("description");
        setUserDefinedKeyCombination (keyName);
    }

    void  FcoeProfileCfgManagedObject::setFcoePort(const bool &fcoePort)
    {
        m_fcoePort  =  fcoePort;
    }

    bool  FcoeProfileCfgManagedObject::getFcoePort() const
    {
        return (m_fcoePort);
    }
    void  FcoeProfileCfgManagedObject::setMapName(const string &map_name)
        {
	        m_mapName  =  map_name;

	}
   string  FcoeProfileCfgManagedObject::getMapName() const
       {
               return (m_mapName);
	          
       }



    UI8 FcoeProfileCfgManagedObject::sendBulkRequestToClient(const string profile_name, const UI32 profile_id)
    {
        int status = WAVE_MESSAGE_SUCCESS;
        APPMFcoeProfileMessage *pMsg = NULL;

        /* Send fcoe-profile config. */
        pMsg = new APPMFcoeProfileMessage;
        pMsg->setOpCode(APPM_CREATE);
        pMsg->setCmdCode(FCOE_PROFILE_CREATE);
        pMsg->setProfileName(profile_name);
        pMsg->setProfileId(profile_id);
		pMsg->setFcoePort(false); 
	 	status = sendSynchronouslyToWaveClient("nsm",pMsg);                                                            
        if (status == WAVE_MESSAGE_SUCCESS && pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
			delete pMsg;
			return 0;
        }

        delete pMsg;

        /* Send fcoeport config */
        if (m_fcoePort)
        {
            pMsg = new APPMFcoeProfileMessage;
            pMsg->setOpCode(APPM_SET_ELEM);
            pMsg->setCmdCode(FCOE_PROFILE_FCOEPORT_ADD);
            pMsg->setProfileName(profile_name);
            pMsg->setProfileId(profile_id);
            pMsg->setFcoePort(true);
            status = sendSynchronouslyToWaveClient("nsm",pMsg);

            if (status == WAVE_MESSAGE_SUCCESS && pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
				delete pMsg;
				return 0;
            }
			delete pMsg;
        }

		return 1;
    }

    Attribute *FcoeProfileCfgManagedObject::getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined)
    {
        Attribute *pAttribute = NULL;
        pAttribute = new AttributeBool (true, "appm");
        return (pAttribute);
    }

}
