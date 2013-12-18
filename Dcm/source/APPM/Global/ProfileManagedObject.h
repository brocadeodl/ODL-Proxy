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

#ifndef PROFILEMANAGEDOBJECT_H
#define PROFILEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "APPM/Global/APPMGlobalTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

#include "vcs.h"

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

    class APPMGlobalObjectManager;
    class ProfileMacManagedObject;
    class SubProfileManagedObject;
	class APPMGlobalAPPMProfileMessage;
	class APPMGlobalAPPMSecurityMessage;
	class APPMGlobalAPPMFcoeMessage;
    class ProfileManagedObject : virtual public DcmManagedObject
    {
        private:
            UI32 getNextPPid();
            void setGlobalPPid(UI32 ppId);
            void unSetGlobalPPid(UI32 ppId);
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            ProfileManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager);
            ProfileManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const string &profileName,const UI32 &profileId,const bool &isActivated,const vector<WaveManagedObjectPointer<ProfileMacManagedObject> > &assocMacs,const bool &isAllow,const bool &isFloodingRestricted,const vector<WaveManagedObjectPointer<SubProfileManagedObject> > &subProfileCfg);
            virtual    ~ProfileManagedObject ();
            static string getClassName();
            void setProfileName(const string &profileName);
            string getProfileName()  const;
            void setProfileId(const UI32 &profileId);
            UI32 getProfileId()  const;
            void setIsActivated(const bool &isActivated);
            bool getIsActivated()  const;
			void setIsAllow(const bool &isAllow);
            bool getIsAllow()  const;
			void setIsFloodingRestricted(const bool &isFloodingRestricted);
            bool getIsFloodingRestricted() const;
            void setAssocMacs(const vector<WaveManagedObjectPointer<ProfileMacManagedObject> > &assocMacs);
            vector<WaveManagedObjectPointer<ProfileMacManagedObject> > getAssocMacs()  const;
            void setSubProfileCfg(const vector<WaveManagedObjectPointer<SubProfileManagedObject> > &subProfileCfg);
			vector<WaveManagedObjectPointer<SubProfileManagedObject> > getSubProfileCfg()  const;
			void setSubProfileName(const string &subProfileName);
            string getSubProfileName()  const;

			void addProfileMacToVector(ProfileMacManagedObject *profileMacMO);
			/* Overloaded functions */
			void delProfileMacFromVector(int index);
			int delProfileMacFromVector(ProfileMacManagedObject *profileMacMO);

			void addSubProfileToVector(SubProfileManagedObject *subProfileMO);
			/* Overloaded functions */
			void delSubProfileFromVector(int index);
			int delSubProfileFromVector(const string &subProfileName);

			void updateActivateInfo(APPMGlobalAPPMProfileMessage *pMsg);
			AppmReturnCode updateSecSubProfileInfo(PrismLinearSequencerContext *pPrismLinearSequencerContext,
										APPMGlobalAPPMSecurityMessage *pMsg);
			AppmReturnCode updateFcoeSubProfileInfo(PrismLinearSequencerContext *pPrismLinearSequencerContext,
										APPMGlobalAPPMFcoeMessage *pMsg);
            UI8 sendCreationRequestToClient();
            UI8 sendActivationRequestToClient();
            UI8 sendAssociationRequestToClient();
			UI8 sendIsAllowRequestToClient();
			UI8 sendIsFloodingRestrictedRequestToClient();
            ResourceId verifySecurityProfile(const string aclName);
            UI8 appmVerifyFcoeQosConflict();
            //ResourceId appmVerifyQosConflict();
            bool isProfileActivated();
            void removeQosSubProfile();
            void removeVlanSubProfile();
            AppmReturnCode getAclName ( string &aclName , string profileName );
            ResourceId ProfileActivationValidation();
			bool checkVlanSubProfile();
            virtual string customConfigurationDisplay (WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext);
// Now the data members

        private:
        protected:
        public:
            string  m_profileName;
            UI32  m_profileId;
            bool  m_isActivated;
			string m_mapName;
			vector<WaveManagedObjectPointer<ProfileMacManagedObject> >  m_assocMacs;
			bool m_isAllow;
			bool m_isFloodingRestricted;
            vector<WaveManagedObjectPointer<SubProfileManagedObject> >  m_subProfileCfg;
    };
}
#endif                                            //PROFILEMANAGEDOBJECT_H
