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
 *   Author : kverma, asharma                                              *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"

#include "APPM/Global/VlanProfileCfgManagedObject.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMVlanManagedObject.h"
#include "APPM/Global/APPMVlanClassifierManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "APPM/Global/APPMGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/APPM/APPMVlanProfileMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "APPM/Global/APPMGlobalConfWorker.h"

#include <algorithm>

using namespace std;

namespace DcmNs
{

    VlanProfileCfgManagedObject::VlanProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (VlanProfileCfgManagedObject::getClassName (), SubProfileManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        SubProfileManagedObject (pAPPMGlobalObjectManager)
    {
		m_accessVlan = 0;
    }

    VlanProfileCfgManagedObject::VlanProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const bool &swModeFlag,const VlanSwMode &swMode,const bool &isAllowedVlanAll, const bool &isAllowedVlanAllV2, const bool &isAllowedVlanNone,const UI32 &accessVlan,const UI32Range &vlanRangeAdd,const UI32Range &vlanRangeRemove,const UI32Range &vlanRangeExcept,const vector<WaveManagedObjectPointer<APPMVlanClassifierManagedObject> > &vlanClassifierInfo)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (VlanProfileCfgManagedObject::getClassName (), SubProfileManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        SubProfileManagedObject (pAPPMGlobalObjectManager),
        m_swModeFlag    (swModeFlag),
        m_swMode    (swMode),
        m_isAllowedVlanAll    (isAllowedVlanAll),
        m_isAllowedVlanAllV2  (isAllowedVlanAllV2),
        m_isAllowedVlanNone    (isAllowedVlanNone),
		m_accessVlan   (accessVlan),
        m_vlanRangeAdd    (vlanRangeAdd),
        m_vlanRangeRemove    (vlanRangeRemove),
        m_vlanRangeExcept    (vlanRangeExcept),
        m_vlanClassifierInfo    (vlanClassifierInfo)
    {
    }

    VlanProfileCfgManagedObject::~VlanProfileCfgManagedObject ()
    {
    }

    string  VlanProfileCfgManagedObject::getClassName()
    {
        return ("VlanProfileCfgManagedObject");
    }

    void  VlanProfileCfgManagedObject::setupAttributesForPersistence()
    {
		UI8 defaultSwMode = 0;
		const UI32Range defaultData("");
		const UI32 defaultAccessVlan = 0;
        SubProfileManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_swModeFlag,"swModeFlag",appm_switchport));
		addPersistableAttribute (new AttributeEnum((UI32*)(&m_swMode),defaultSwMode,"swMode",appm_vlan_mode));
        addPersistableAttribute (new AttributeBool(&m_isAllowedVlanAll, "isAllowedVlanAll"));
        addPersistableAttribute (new AttributeBool(&m_isAllowedVlanAllV2,"isAllowedVlanAllV2",appm_all));
        addPersistableAttribute (new AttributeBool(&m_isAllowedVlanNone,"isAllowedVlanNone",appm_none));
        addPersistableAttribute (new AttributeUI32(&m_accessVlan,defaultAccessVlan,"accessVlan",appm_name));
        addPersistableAttribute (new AttributeUI32Range(&m_vlanRangeAdd,defaultData,"vlanRangeAdd",appm_add));
        addPersistableAttribute (new AttributeUI32Range(&m_vlanRangeRemove,defaultData,"vlanRangeRemove",appm_remove));
        addPersistableAttribute (new AttributeUI32Range(&m_vlanRangeExcept,defaultData,"vlanRangeExcept",appm_except));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<APPMVlanClassifierManagedObject>(
            &m_vlanClassifierInfo,"vlanClassifierInfo", getClassName (), getObjectId (), APPMVlanClassifierManagedObject::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectComposition<APPMNativeVlanManagedObject>
            (&m_nativeVlanIntfConfig,"nativeVlanIntfConfig", APPMNativeVlanManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<APPMMacAddressVlanClassificationMO>
            (&m_macVlanClassifications, "macVlanClassifications", getClassName (), getObjectId (), APPMMacAddressVlanClassificationMO::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<APPMMacGroupVlanClassificationMO>
            (&m_macGroupVlanClassifications, "macGroupVlanClassifications", getClassName (), getObjectId (), APPMMacGroupVlanClassificationMO::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<APPMCtagVlanClassificationMO>
            (&m_ctagVlanClassifications, "ctagVlanClassifications", getClassName (), getObjectId (), APPMCtagVlanClassificationMO::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<APPMCtagVlanClassificationRemoveMO>
            (&m_removeCtagVlanClassifications, "removeCtagVlanClassifications", getClassName (), getObjectId (), APPMCtagVlanClassificationRemoveMO::getClassName ()));
    }

    void  VlanProfileCfgManagedObject::setupAttributesForCreate()
    {
	    UI8 defaultSwMode = 0;
        addPersistableAttributeForCreate  (new AttributeBool(&m_swModeFlag,"swModeFlag",appm_switchport));
		addPersistableAttributeForCreate  (new AttributeEnum((UI32*)(&m_swMode),defaultSwMode,"swMode",appm_vlan_mode));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isAllowedVlanAll, "isAllowedVlanAll"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isAllowedVlanAllV2,"isAllowedVlanAllV2",appm_all));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isAllowedVlanNone,"isAllowedVlanNone",appm_none));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_accessVlan,"accessVlan",appm_name));
        addPersistableAttributeForCreate  (new AttributeUI32Range(&m_vlanRangeAdd,"vlanRangeAdd",appm_add));
        addPersistableAttributeForCreate  (new AttributeUI32Range(&m_vlanRangeRemove,"vlanRangeRemove",appm_remove));
        addPersistableAttributeForCreate  (new AttributeUI32Range(&m_vlanRangeExcept,"vlanRangeExcept",appm_except));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<APPMVlanClassifierManagedObject>(&m_vlanClassifierInfo,"vlanClassifierInfo", getClassName (), getObjectId (), APPMVlanClassifierManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectComposition<APPMNativeVlanManagedObject>(&m_nativeVlanIntfConfig,"nativeVlanIntfConfig", APPMNativeVlanManagedObject::getClassName ()));    
        addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<APPMMacAddressVlanClassificationMO>
            (&m_macVlanClassifications, "macVlanClassifications", getClassName (), getObjectId (), APPMMacAddressVlanClassificationMO::getClassName ()));
        addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<APPMMacGroupVlanClassificationMO>
            (&m_macGroupVlanClassifications, "macGroupVlanClassifications", getClassName (), getObjectId (), APPMMacGroupVlanClassificationMO::getClassName ()));
        addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<APPMCtagVlanClassificationMO>
            (&m_ctagVlanClassifications, "ctagVlanClassifications", getClassName (), getObjectId (), APPMCtagVlanClassificationMO::getClassName ()));
        addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<APPMCtagVlanClassificationRemoveMO>
            (&m_removeCtagVlanClassifications, "removeCtagVlanClassifications", getClassName (), getObjectId (), APPMCtagVlanClassificationRemoveMO::getClassName ()));

    }

	void VlanProfileCfgManagedObject::setupKeys()
	{
        vector<string> keyName;
        keyName.push_back ("description");
        setUserDefinedKeyCombination (keyName);
    }
	
    void  VlanProfileCfgManagedObject::setSwModeFlag(const bool &swModeFlag)
    {
        m_swModeFlag  =  swModeFlag;
    }

    bool  VlanProfileCfgManagedObject::getSwModeFlag() const
    {
        return (m_swModeFlag);
    }

    void  VlanProfileCfgManagedObject::setSwMode(const  VlanSwMode &swMode)
    {
        m_swMode  =  swMode;
    }

    VlanSwMode  VlanProfileCfgManagedObject::getSwMode() const
    {
        return (m_swMode);
    }

    void  VlanProfileCfgManagedObject::setIsAllowedVlanAll(const bool &isAllowedVlanAll) {
        m_isAllowedVlanAll  =  isAllowedVlanAll;
    }
    bool  VlanProfileCfgManagedObject::getIsAllowedVlanAll() const {
        return (m_isAllowedVlanAll);
    }
    void  VlanProfileCfgManagedObject::setIsAllowedVlanAllV2(const bool &isAllowedVlanAllV2) {
        m_isAllowedVlanAllV2  =  isAllowedVlanAllV2;
    }
    bool  VlanProfileCfgManagedObject::getIsAllowedVlanAllV2() const {
        return (m_isAllowedVlanAllV2);
    }

    void  VlanProfileCfgManagedObject::setIsAllowedVlanNone(const bool &isAllowedVlanNone)
    {
        m_isAllowedVlanNone  =  isAllowedVlanNone;
    }

    bool  VlanProfileCfgManagedObject::getIsAllowedVlanNone() const
    {
        return (m_isAllowedVlanNone);
    }

    void  VlanProfileCfgManagedObject::setAccessVlan(const UI32 &accessVlan)
    {
        m_accessVlan  =  accessVlan;
    }

    UI32  VlanProfileCfgManagedObject::getAccessVlan() const
    {
        return (m_accessVlan);
    }

    void  VlanProfileCfgManagedObject::setVlanRangeAdd(const UI32Range &vlanRangeAdd)
    {
        m_vlanRangeAdd  =  vlanRangeAdd;
    }

    UI32Range  VlanProfileCfgManagedObject::getVlanRangeAdd() const
    {
        return (m_vlanRangeAdd);
    }

    void  VlanProfileCfgManagedObject::setVlanRangeRemove(const UI32Range &vlanRangeRemove)
    {
        m_vlanRangeRemove  =  vlanRangeRemove;
    }

    UI32Range  VlanProfileCfgManagedObject::getVlanRangeRemove() const
    {
        return (m_vlanRangeRemove);
    }
	
    void  VlanProfileCfgManagedObject::setVlanRangeExcept(const UI32Range &vlanRangeExcept)
    {
        m_vlanRangeExcept  =  vlanRangeExcept;
    }

    UI32Range  VlanProfileCfgManagedObject::getVlanRangeExcept() const
    {
        return (m_vlanRangeExcept);
    }

    void  VlanProfileCfgManagedObject::setVlanClassifierInfo(const vector<WaveManagedObjectPointer<APPMVlanClassifierManagedObject> > &vlanClassifierInfo)
    {
        m_vlanClassifierInfo  =  vlanClassifierInfo;
    }

    vector<WaveManagedObjectPointer<APPMVlanClassifierManagedObject> >  VlanProfileCfgManagedObject::getVlanClassifierInfo() const
    {
        return (m_vlanClassifierInfo);
    }

    Attribute *VlanProfileCfgManagedObject::getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined)
    {
        Attribute *pAttribute = NULL; 
        pAttribute = new AttributeBool (true, "appm");
        return (pAttribute);
    }
    void  VlanProfileCfgManagedObject::addGroupIdToVector(APPMVlanClassifierManagedObject *vlanClassifierMO)
    {
        WaveManagedObjectPointer<APPMVlanClassifierManagedObject> vlanClassifier(vlanClassifierMO);
        m_vlanClassifierInfo.push_back(vlanClassifier);
    }

    void  VlanProfileCfgManagedObject::delGroupIdFromVector(int index)
    {
        m_vlanClassifierInfo.erase(m_vlanClassifierInfo.begin() + index);
    }

    AppmReturnCode VlanProfileCfgManagedObject::updateVlanClassifierInfo(APPMGlobalAPPMVlanMessage *pMsg, bool flag)
    {
        unsigned int iter = 0 , loc = 0;
		UI32 vlanclassifieroperation = 0;
        UI32 classifierGroupId = pMsg->getVlanClassifierGroupId();
        UI32 vlanClassifierVlan = pMsg->getVlanClassifierVlan();

        vector<WaveManagedObjectPointer<APPMVlanClassifierManagedObject> >  vlanClassifierInfoVector;
        vlanClassifierInfoVector = getVlanClassifierInfo();

		for(iter = 0 ; iter < vlanClassifierInfoVector.size(); iter++) {
			if(classifierGroupId == (vlanClassifierInfoVector[iter])->getGrpId()){
				loc = iter;
				if (flag) {
					APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, string ("") + " modifing the existing group");
					return (APPM_SUCCESS);
				}
				break;
			}
		}

		if (flag) {
			APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string ("") + " adding group to the profile");
			/* create the new MO */
			APPMVlanClassifierManagedObject *pNewAPPMVlanClassifierManagedObject =
				new APPMVlanClassifierManagedObject(dynamic_cast<APPMGlobalObjectManager *>(getPWaveObjectManager()));

			pNewAPPMVlanClassifierManagedObject->setGrpId(classifierGroupId);
			pNewAPPMVlanClassifierManagedObject->setVlanId(vlanClassifierVlan);
			pNewAPPMVlanClassifierManagedObject->setVlanclassifierOperation(vlanclassifieroperation);

			addGroupIdToVector(pNewAPPMVlanClassifierManagedObject);
		} else {
			/* Handling deletion part */
			APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string("") + " deleting the group from the profile");
			/* remove the mac vector, which also removes from MO as compsite rule*/
			delGroupIdFromVector(loc);
		}
		return (APPM_SUCCESS);
	}

    UI8 VlanProfileCfgManagedObject::sendBulkRequestToClient(const string profile_name, const UI32 profile_id)
    {
        int status = WAVE_MESSAGE_SUCCESS;
        APPMVlanProfileMessage *pMsg = new APPMVlanProfileMessage;
        string vlanRange = "";
        UI32 vlanClassifierGroupId = 0;
        UI32  vlanClassifierVlan = 0;
		APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject = (m_nativeVlanIntfConfig.operator->());
		UI32 nativeVlanId = pAPPMNativeVlanManagedObject->getNativeVlanId();
        UI32 nativeVlanCtagId = pAPPMNativeVlanManagedObject->getCtag();

        /*
         * TODO:add bulk support , not done as of now because of time crunch.
         */

        /*
         * send vlan-profile cli.
         */
        pMsg->setOpCode(APPM_CREATE);
        pMsg->setCmdCode(VLAN_PROFILE_CREATE);
        pMsg->setProfileName(profile_name);

        status = sendSynchronouslyToWaveClient("nsm",pMsg);

        if (status == WAVE_MESSAGE_SUCCESS) {
            if (pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                //bpatel: To avoid memory leak
				delete pMsg;
				return 0;
            }
        }

        delete pMsg;
		//bpatel
		pMsg = NULL;

        if (!m_swModeFlag)
            return 1;

        /*
         * send switchport config.
         */

        pMsg = new APPMVlanProfileMessage (APPM_SET_ELEM, VLAN_SW_MODE_FLAG, profile_name,
            profile_id, m_swModeFlag, m_swMode, m_isAllowedVlanAllV2, m_isAllowedVlanNone,
            m_accessVlan, vlanRange, vlanClassifierGroupId, vlanClassifierVlan);

        status = sendSynchronouslyToWaveClient("nsm",pMsg);

        if (status == WAVE_MESSAGE_SUCCESS) {
            if (pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                //bpatel: To avoid memory leak
				delete pMsg;
				return 0;
            }
        }

        delete pMsg;
		//bpatel
		pMsg = NULL;

        switch (m_swMode)
        {
        case VLAN_SW_MODE_ACCESS:
        {
            /*
             * Send mode.
             */

            pMsg = new APPMVlanProfileMessage (APPM_SET_ELEM, VLAN_SW_MODE, profile_name,
                profile_id, m_swModeFlag, m_swMode, m_isAllowedVlanAllV2, m_isAllowedVlanNone,
                m_accessVlan, vlanRange, vlanClassifierGroupId, vlanClassifierVlan);

            status = sendSynchronouslyToWaveClient("nsm",pMsg);

            if (status == WAVE_MESSAGE_SUCCESS) {
                if (pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                    break;
                }
            }

            delete pMsg;
			//bpatel
			pMsg = NULL;

            if (m_accessVlan)
            {
                pMsg = new APPMVlanProfileMessage (APPM_SET_ELEM, VLAN_ACCESS_VLAN, profile_name,
                    profile_id, m_swModeFlag, m_swMode, m_isAllowedVlanAllV2, m_isAllowedVlanNone,
                    m_accessVlan, vlanRange, vlanClassifierGroupId, vlanClassifierVlan);

                status = sendSynchronouslyToWaveClient("nsm",pMsg);

                delete pMsg;
				//bpatel
				pMsg = NULL;
            }

            //Config replay of the mac based vlan classifications.
            unsigned int macClassIt = 0;
            unsigned int macGroupClassIt = 0;
            for( macClassIt = 0 ; macClassIt < m_macVlanClassifications.size() ; macClassIt++) {
                pMsg = new APPMVlanProfileMessage ();
                pMsg->setOpCode(APPM_SET_ELEM);
                pMsg->setCmdCode(APPM_VLAN_CLASSIFICATION_SET_MAC_ADDR);
                pMsg->setProfileName(profile_name);
                pMsg->setAccessVlan((m_macVlanClassifications[macClassIt])->getAccessVlanId());
                pMsg->setMac((m_macVlanClassifications[macClassIt])->getMac());
                APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string ("Config replay of the mac based vlan classification vlanId<")
                        + pMsg->getAccessVlan() + "> macAddress<" + pMsg->getMac() + ">");
                status = sendSynchronouslyToWaveClient("nsm",pMsg);

                if (status == WAVE_MESSAGE_SUCCESS) {
                    if (pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                        APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string ("Config replay of the mac based vlan classification vlanId<")
                                + pMsg->getAccessVlan() + "> macAddress<" + pMsg->getMac() + "> falied, with status <" + pMsg->getCompletionStatus() + ">");
                    }
                } else {
                    APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string ("Config replay of the mac based vlan classification vlanId<")
                            + pMsg->getAccessVlan() + "> macAddress<" + pMsg->getMac() + "> falied, with status <" + status + ">");
                }

                delete pMsg;
                pMsg = NULL;
            }

            //Config replay of the mac based vlan classifications.
            for( macGroupClassIt = 0 ; macGroupClassIt < m_macGroupVlanClassifications.size() ; macGroupClassIt++) {
                pMsg = new APPMVlanProfileMessage ();
                pMsg->setOpCode(APPM_SET_ELEM);
                pMsg->setCmdCode(APPM_VLAN_CLASSIFICATION_SET_MAC_GROUP);
                pMsg->setProfileName(profile_name);
                pMsg->setAccessVlan((m_macGroupVlanClassifications[macGroupClassIt])->getVlanId());
                pMsg->setMacGroup((m_macGroupVlanClassifications[macGroupClassIt])->getMacGroupId());
                status = sendSynchronouslyToWaveClient("nsm",pMsg);

                if (status == WAVE_MESSAGE_SUCCESS) {
                    if (pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                        APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string ("Config replay of the mac-group based vlan classification vlanId<")
                                + pMsg->getAccessVlan() + "> macAddress<" + pMsg->getMacGroup() + "> falied, with status <" + pMsg->getCompletionStatus() + ">");
                    }
                } else {
                    APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string ("Config replay of the mac based vlan classification vlanId<")
                            + pMsg->getAccessVlan() + "> macAddress<" + pMsg->getMacGroup() + "> falied, with status <" + status + ">");
                }
                delete pMsg;
                pMsg = NULL;

            }

            break;
        }
        case VLAN_SW_MODE_TRUNK: 
        {
        //case VLAN_SW_MODE_HYBRID:

            /*
             * Send the Mode info.
             */
            pMsg = new APPMVlanProfileMessage (APPM_SET_ELEM, VLAN_SW_MODE, profile_name,
                profile_id, m_swModeFlag, m_swMode, m_isAllowedVlanAllV2, m_isAllowedVlanNone,
                m_accessVlan, vlanRange, vlanClassifierGroupId, vlanClassifierVlan);

            status = sendSynchronouslyToWaveClient("nsm",pMsg);

            if (status == WAVE_MESSAGE_SUCCESS) {
                if (pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                    break;
                }
            }

            delete pMsg;
			//bpatel
			pMsg = NULL;

            if (m_isAllowedVlanNone)
            {
                pMsg = new APPMVlanProfileMessage (APPM_SET_ELEM, VLAN_IS_ALLOWED_VLAN_NONE, profile_name,
                    profile_id, m_swModeFlag, m_swMode, m_isAllowedVlanAllV2, m_isAllowedVlanNone,
                    m_accessVlan, vlanRange, vlanClassifierGroupId, vlanClassifierVlan);

                status = sendSynchronouslyToWaveClient("nsm",pMsg);

                delete pMsg;
				//bpatel
				pMsg = NULL;
            
            } else {
                if (m_isAllowedVlanAllV2)
                {
                    pMsg = new APPMVlanProfileMessage (APPM_SET_ELEM, VLAN_IS_ALLOWED_VLAN_ALL, profile_name,
                        profile_id, m_swModeFlag, m_swMode, m_isAllowedVlanAllV2, m_isAllowedVlanNone,
                        m_accessVlan, vlanRange, vlanClassifierGroupId, vlanClassifierVlan);

                    status = sendSynchronouslyToWaveClient("nsm",pMsg);

                    if (status == WAVE_MESSAGE_SUCCESS) {
                        if (pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                            break;
                        }
                    }

                    delete pMsg;
					//bpatel
					pMsg = NULL;
                
                } else if (m_vlanRangeAdd!=vlanRange) {
                    pMsg = new APPMVlanProfileMessage (APPM_SET_ELEM, VLAN_IS_ALLOWED_VLAN_ADD, profile_name,
                        profile_id, m_swModeFlag, m_swMode, m_isAllowedVlanAllV2, m_isAllowedVlanNone,
                        m_accessVlan, m_vlanRangeAdd, vlanClassifierGroupId, vlanClassifierVlan);
                    status = sendSynchronouslyToWaveClient("nsm",pMsg);
                    delete pMsg;
					//bpatel
					pMsg = NULL;
                }
                   
                if (m_vlanRangeRemove!=vlanRange)
                {
                    pMsg = new APPMVlanProfileMessage (APPM_SET_ELEM, VLAN_IS_ALLOWED_VLAN_REMOVE, profile_name,
                        profile_id, m_swModeFlag, m_swMode, m_isAllowedVlanAllV2, m_isAllowedVlanNone,
                        m_accessVlan, m_vlanRangeRemove, vlanClassifierGroupId, vlanClassifierVlan);
                    
                    status = sendSynchronouslyToWaveClient("nsm",pMsg);
                    
                    if (status == WAVE_MESSAGE_SUCCESS) {
                        if (pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                            //bpatel: To avoid memory leak
							delete pMsg;
							return 0;
                        }
                  }
                    delete pMsg;
					//bpatel:/to make sure that it is null
					pMsg = NULL;
                }

                unsigned int ctagClassIter = 0;

                for(ctagClassIter = 0 ; ctagClassIter < m_ctagVlanClassifications.size() ; ctagClassIter++) {
                    pMsg = new APPMVlanProfileMessage ();
                    pMsg->setOpCode(APPM_SET_ELEM);
                    pMsg->setCmdCode(APPM_VLAN_CLASSIFICATION_SET_CTAG);
                    pMsg->setProfileName(profile_name);
                    pMsg->setTrunkVlanId((m_ctagVlanClassifications[ctagClassIter])->getVlanId());
                    pMsg->setCtag((m_ctagVlanClassifications[ctagClassIter])->getCtagId());

                    status = sendSynchronouslyToWaveClient("nsm",pMsg);

                    if (status == WAVE_MESSAGE_SUCCESS) {
                        if (pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                            APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string ("Config replay of the ctag based vlan classification vlanId<")
                                    + pMsg->getTrunkVlanId() + "> macAddress<" + pMsg->getCtag() + "> falied, with status <" + pMsg->getCompletionStatus() + ">");
                        }
                    } else {
                        APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string ("Config replay of the mac based vlan classification vlanId<")
                                + pMsg->getTrunkVlanId() + "> macAddress<" + pMsg->getCtag() + "> falied, with status <" + status + ">");
                    }

                    delete pMsg;
                    pMsg = NULL;
                }   

            }

            if ((APPM_NSM_NATIVE_VLAN_DEFAULT != nativeVlanId)
                    && (APPM_NSM_NATIVE_VLAN_DEFAULT == nativeVlanCtagId)) {
                /*send native vlan config */
                pMsg = new APPMVlanProfileMessage ();
                pMsg->setOpCode(APPM_SET_ELEM);
                pMsg->setCmdCode(VLAN_NATIVE_ID_ADD);
                pMsg->setProfileName(profile_name);
                pMsg->setNativeVlanId(nativeVlanId);

                status = sendSynchronouslyToWaveClient("nsm",pMsg);

                if (status == WAVE_MESSAGE_SUCCESS) {
                    if (pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                        delete pMsg;
                        return 0;//TODO : RETURN ??
                    }
                    delete pMsg;
                    pMsg = NULL;
                }	
            } else if ((APPM_NSM_NATIVE_VLAN_DEFAULT != nativeVlanId)
                    && (APPM_NSM_NATIVE_VLAN_DEFAULT != nativeVlanCtagId)) {

                pMsg = new APPMVlanProfileMessage ();
                pMsg->setOpCode(APPM_SET_ELEM);
                pMsg->setCmdCode(APPM_VLAN_CLASSIFICATION_SET_NATIVE_CTAG);
                pMsg->setProfileName(profile_name);
                pMsg->setNativeVlanId(nativeVlanId);
                pMsg->setCtag(nativeVlanCtagId);

                status = sendSynchronouslyToWaveClient("nsm",pMsg);

                if (status == WAVE_MESSAGE_SUCCESS) {
                    if (pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                        APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string ("Config replay of the ctag based native-vlan classification native-vlanId<")
                                + pMsg->getNativeVlanId() + "> macAddress<" + pMsg->getCtag() + "> falied, with status <" + pMsg->getCompletionStatus() + ">");
                    }
                } else {
                    APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, string ("Config replay of the mac based vlan classification vlanId<")
                            + pMsg->getNativeVlanId() + "> macAddress<" + pMsg->getCtag() + "> falied, with status <" + status + ">");
                }

                delete pMsg;
                pMsg = NULL;


            } else {
                //DO NOTHING
            }
            break;
        }
        default:
        {
            break;
        }
        }

        /*
         * TODO :: Add support Vlan Classifier once its config is implemented.
         */
		//bpatel: To avoid memory leak
		if(pMsg){
			delete pMsg;
		}
        return 1;
    }

	void  VlanProfileCfgManagedObject::setNativeVlanIntfConfig(const WaveManagedObjectPointer<APPMNativeVlanManagedObject> &nativeVlanIntfConfig)
	{
		m_nativeVlanIntfConfig  =  nativeVlanIntfConfig;
	}

	WaveManagedObjectPointer<APPMNativeVlanManagedObject>  VlanProfileCfgManagedObject::getNativeVlanIntfConfig() const
	{
		return (m_nativeVlanIntfConfig);
	}

    bool VlanProfileCfgManagedObject::isVlanPresent(const UI32 vlanId)
    {
        vector<UI32> vlans;
        UI32 iter = 0;
        bool status = false; 
        this->getVlans(vlans);

        if (getIsAllowedVlanAllV2()){
            if ( vlans.size() == 0 ) {
                status = true;
            } else {
                status = true;
                for (iter = 0; iter < vlans.size(); iter++) {
                    if (vlans[iter] == vlanId) {
                        status = false;
                        break;
                    }
                }
            }
            return status;
        } else {
            for (iter = 0; iter < vlans.size(); iter++){
                if (vlans[iter] == vlanId){
                    status = true;
                    break;
                }
            }
        }
        return status;
    }

    void VlanProfileCfgManagedObject::getVlans(vector<UI32>& vlans)
    {
        /*
         * Gets the vlan ids of the vlans configured
         * for vlan-profile excluding the removed vlans,
         * for trunk allowed all its other way round, i.e 
         * now we'll get removed/excepted vlans.
         */

        UI32Range VlanRange = UI32Range("");
        vector<UI32> RemovedVlans;
        vector<UI32> ExceptVlans;
        vector<UI32>::iterator it;
        unsigned int iter = 0;
        UI32 nativeVlanId = 0;
        UI32 accessVlanId = 0;
        APPMNativeVlanManagedObject *pAPPMNativeVlanManagedObject = NULL;

        //If no vlans are allowed, return an empty vlan list.
        if (getIsAllowedVlanNone())
            return;
        
        VlanRange = getVlanRangeRemove();
        VlanRange.getUI32RangeVector(RemovedVlans);

        VlanRange = getVlanRangeExcept();
        VlanRange.getUI32RangeVector(ExceptVlans);

        //In case of allowedVlanAll, return a negation list , i.e return the list of vlan to be excluded.
        if (getIsAllowedVlanAllV2())
        {

            /*
             * Just for security profile activation , avoiding dup entries is
             * not required, but if this function is to be used by other modules
             * then uncomment out the following code.
             */
#if 0
            vlans = RemovedVlans;
            for ( iter = 0; iter < ExceptVlans.size(); iter++ )
            {
                /*
                 * Sanity check to avoid adding duplicate entries to the vector,
                 * duplication may lead to redundant db queries.
                 */
                it = find(vlans.begin(), vlans.end(), ExceptVlans[iter]);
                if (it ==  vlans.end())
                {
                    vlans.push_back(ExceptVlans[iter]);
                }
            }
#endif
            /*
             * This code should be removed if dup entries are to be
             * avoided.
             */
            vlans.reserve( RemovedVlans.size() + ExceptVlans.size() );
            vlans.insert( vlans.end(), RemovedVlans.begin(), RemovedVlans.end());
            vlans.insert( vlans.end(), ExceptVlans.begin(), ExceptVlans.end());

            return;
            
        } else {

            pAPPMNativeVlanManagedObject = (this->getNativeVlanIntfConfig()).operator->();

            if (pAPPMNativeVlanManagedObject)
            nativeVlanId = pAPPMNativeVlanManagedObject->getNativeVlanId();

            VlanRange = getVlanRangeAdd();
            VlanRange.getUI32RangeVector(vlans);

            for ( iter = 0; iter < RemovedVlans.size(); iter++ )
            {
                /*
                 * Search for the vlan in add list & remove it if its there.
                 */
                it = find(vlans.begin(), vlans.end(), RemovedVlans[iter]);
                if (it !=  vlans.end())
                {
                    vlans.erase(it);
                }
            }
            
            for ( iter = 0; iter < ExceptVlans.size(); iter++ )
            {
                /*
                 * Search for the vlan in add list & remove it if its there.
                 */
                it = find(vlans.begin(), vlans.end(), ExceptVlans[iter]);
                if (it !=  vlans.end())
                {
                    vlans.erase(it);
                }
            }

            /*
             * Insert Native Vlan also.
             */
            if (nativeVlanId)
            {
                it = find(vlans.begin(), vlans.end(), nativeVlanId);

                if (it == vlans.end())
                    vlans.push_back(nativeVlanId);
            }

         /*
          * Insert Access Vlan also.
          */

         accessVlanId = getAccessVlan();
         if (accessVlanId)
         {
             it = find(vlans.begin(), vlans.end(),accessVlanId );

             if (it == vlans.end())
                 vlans.push_back(accessVlanId);
             }
        }
    }

    string VlanProfileCfgManagedObject::customConfigurationDisplay ( WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext)
    {  
        string displayString = "";

        if(!m_isAllowedVlanNone)
        {
            displayString = "\r" ;       
        }

        return displayString;
    }

    void VlanProfileCfgManagedObject::deleteAllMacVlanClassifications() {
        m_macVlanClassifications.clear();
    }
    void VlanProfileCfgManagedObject::deleteAllMacGroupVlanClassifications() {
        m_macGroupVlanClassifications.clear();
    }
    void VlanProfileCfgManagedObject::deleteAllCtagVlanClassifications() {
        m_ctagVlanClassifications.clear();
    }
    vector<WaveManagedObjectPointer<APPMMacAddressVlanClassificationMO> > VlanProfileCfgManagedObject::getMacVlanClassifications () const {
        return m_macVlanClassifications;
    }
    vector<WaveManagedObjectPointer<APPMMacGroupVlanClassificationMO> > VlanProfileCfgManagedObject::getMacGroupVlanClassifications() const {
        return m_macGroupVlanClassifications;
    }
    vector<WaveManagedObjectPointer<APPMCtagVlanClassificationMO> > VlanProfileCfgManagedObject::getCtagVlanClassifications() const {
        return m_ctagVlanClassifications;
    }

    bool VlanProfileCfgManagedObject::doesVlanContainMacClassification(UI32 &vlanId) {
        bool macClassificationPresent = false;
        for(unsigned int it = 0 ; it < m_macVlanClassifications.size(); it++) {
            if(vlanId == (m_macVlanClassifications[it]->getAccessVlanId())){
                macClassificationPresent = true;
                goto last;
            }
        }
        for(unsigned int it = 0 ; it < m_macGroupVlanClassifications.size(); it++) {
            if(vlanId == (m_macGroupVlanClassifications[it]->getVlanId())){
                macClassificationPresent = true;
                goto last;
            }
        }
        last:
        return macClassificationPresent;
    }

    bool VlanProfileCfgManagedObject::doesVlanContainCtagClassification(UI32 &vlanId) {
        bool macClassificationPresent = false;
        for(unsigned int it = 0 ; it < m_ctagVlanClassifications.size(); it++) {
            if(vlanId == (m_ctagVlanClassifications[it]->getVlanId())){
                macClassificationPresent = true;
                break;
            }
        }
        return macClassificationPresent;
    }

    void VlanProfileCfgManagedObject::deleteMacVlanClassificationsOfVlan(UI32 &vlanId) {
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("modifing the existing group deleting access vlan") + vlanId + ">" );
        /*for(unsigned int it = 0 ; it < m_macVlanClassifications.size(); it++) {
          if(vlanId == (m_macVlanClassifications[it]->getAccessVlanId())){
          m_macVlanClassifications.erase(m_macVlanClassifications.begin()+it);
          }
          }*/
        vector<WaveManagedObjectPointer<APPMMacAddressVlanClassificationMO> >::iterator mapiterator = m_macVlanClassifications.begin();

        while(mapiterator != m_macVlanClassifications.end()) {
            if(vlanId == ((*mapiterator).operator->()->getAccessVlanId())) {
                tracePrintf(TRACE_LEVEL_INFO, "Erasing the mac vlan <%d>", (*mapiterator).operator->()->getAccessVlanId());
                //m_macVlanClassifications.erase(mapiterator++);
                mapiterator = m_macVlanClassifications.erase(mapiterator);
            } else {
                //mapiterator++;
                ++mapiterator;
            }
        }
    }
    void VlanProfileCfgManagedObject::deleteMacGroupVlanClassificationsOfVlan(UI32 &vlanId) {
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("modifing the existing group deleting access vlanG") + vlanId + ">" );
        vector<WaveManagedObjectPointer<APPMMacGroupVlanClassificationMO> >::iterator mapiterator = m_macGroupVlanClassifications.begin();

        while(mapiterator != m_macGroupVlanClassifications.end()) {
            if(vlanId == ((*mapiterator).operator->()->getVlanId())) {
                tracePrintf(TRACE_LEVEL_INFO, "Erasing the mac vlan <%d>", (*mapiterator).operator->()->getVlanId());
                //m_macGroupVlanClassifications.erase(mapiterator++);
                mapiterator = m_macGroupVlanClassifications.erase(mapiterator);
            } else {
                //mapiterator++;
                ++mapiterator;
            }
        }

        /*for(unsigned int it = 0 ; it < m_macGroupVlanClassifications.size(); it++) {
          if(vlanId == (m_macGroupVlanClassifications[it]->getVlanId())){
          m_macGroupVlanClassifications.erase(m_macGroupVlanClassifications.begin()+it);
          }
          }*/
    }
    void VlanProfileCfgManagedObject::deleteCtagVlanClassificationsOfVlan(UI32 &vlanId) {
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string ("modifing the existing group deleting access ctag") + vlanId + ">" );
        vector<WaveManagedObjectPointer<APPMCtagVlanClassificationMO> >::iterator mapiterator = m_ctagVlanClassifications.begin();

        while(mapiterator != m_ctagVlanClassifications.end()) {
            if(vlanId == ((*mapiterator).operator->()->getVlanId())) {
                tracePrintf(TRACE_LEVEL_INFO, "Erasing the mac vlan <%d>", (*mapiterator).operator->()->getVlanId());
                //m_ctagVlanClassifications.erase(mapiterator++);
                mapiterator = m_ctagVlanClassifications.erase(mapiterator);
            } else {
                //mapiterator++;
                ++mapiterator;
            }
        }

        /*for(unsigned int it = 0 ; it < m_ctagVlanClassifications.size(); it++) {
          if(vlanId == (m_ctagVlanClassifications[it]->getVlanId())){
          m_ctagVlanClassifications.erase(m_ctagVlanClassifications.begin()+it);
          }
          }*/
    }
}
