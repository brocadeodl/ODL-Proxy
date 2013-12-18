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
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef APPMLOCALOBJECTMANAGER_H
#define APPMLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "APPM/Global/ProfileManagedObject.h"
#include "APPM/Global/ProfileDomainManagedObject.h"
#include "APPM/Global/FcoeProfileCfgManagedObject.h"
#include "APPM/Global/QosProfileCfgManagedObject.h"
#include "APPM/Global/SecProfileCfgManagedObject.h"
#include "APPM/Global/VlanProfileCfgManagedObject.h"

using namespace WaveNs;

namespace DcmNs {

class APPMLocalConfWorker;
class APPMLocalShowWorker;
class APPMVnmLocalWorker;

class APPMLocalObjectManager: public WaveLocalObjectManager {
private:
	PrismMessage *createMessageInstance(const UI32 &operationCode);
	WaveManagedObject *createManagedObjectInstance(
			const string &managedClassName);
			
	void APPMConfigReplayPortProfileAll();
	
	UI8 APPMConfigReplayPortProfileCreate(
			ProfileManagedObject *pProfileManagedObject);
			
	UI8 APPMConfigReplayPortProfileActivate(
			ProfileManagedObject *pProfileManagedObject);
			
	UI8 APPMConfigReplayPortProfileAssociate(
			ProfileManagedObject *pProfileManagedObject);
			
	UI8 APPMConfigReplayIsAllow(ProfileManagedObject *pProfileManagedObject);
	
	UI8 APPMConfigReplayIsFloodingRestricted(ProfileManagedObject *pProfileManagedObject);
	
	void APPMConfigReplayVlanProfile(
			VlanProfileCfgManagedObject *vlanProfileCfgManagedObject,
			const string profile_name, const UI32 profile_id);
			
	void APPMConfigReplaySecurityProfile(
			SecProfileCfgManagedObject *secProfileCfgManagedObject,
			const string profile_name, const UI32 profile_id);
			
	void APPMConfigReplayFcoeProfile(
			FcoeProfileCfgManagedObject *fcoeProfileCfgManagedObject,
			const string profile_name, const UI32 profile_id);
			
	void APPMConfigReplayQosProfile(
			QosProfileCfgManagedObject *qosProfileCfgManagedObject,
			const string profile_name, const UI32 profile_id);
			
	/*Postboot config replay of the port profile domains.*/
	void APPMConfigReplayAllPortProfileDomains();

	/*Create all the port profile domains.*/
	UI32 APPMConfigReplayPortProfileDomainCreate(
			ProfileDomainManagedObject *pProfileDomainManagedObject);
	
	/*Edit all the port profile domains.*/
	UI32 APPMConfigReplayPortProfileDomainEdit(
		ProfileDomainManagedObject *pProfileDomainManagedObject);			

protected:
public:
	APPMLocalObjectManager();
	virtual ~APPMLocalObjectManager();
	static APPMLocalObjectManager *getInstance();
	static string getClassName();
	static PrismServiceId getPrismServiceId();
	void postboot(
			WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);

// Now the data members

private:
	APPMLocalConfWorker *m_pAPPMLocalConfWorker;
	APPMLocalShowWorker *m_pAPPMLocalShowWorker;
	APPMVnmLocalWorker *m_pAPPMVnmLocalWorker;

protected:
public:
};

inline  string  APPMLocalObjectManager::getClassName()
{
    return ("APPM");
}

}
#endif                                            //APPMLOCALOBJECTMANAGER_H
