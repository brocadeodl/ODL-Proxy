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
 *   Author : kverma,sriramr                                               *
 **************************************************************************/

#ifndef APPMGLOBALAPPMPROFILEMESSAGE_H
#define APPMGLOBALAPPMPROFILEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMGlobalAPPMProfileMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            APPMGlobalAPPMProfileMessage ();
            APPMGlobalAPPMProfileMessage (const UI8 &opCode,const UI8
				&cmdCode,const string &profileName,const UI32 &profileId,const
				bool &isActivated,const string &profileMac,const bool &isAllow,
				const bool &isFloodingRestricted,
				const vector<string> &profileMacs);
            virtual    ~APPMGlobalAPPMProfileMessage ();
            void setOpCode(const UI8 &opCode);
            UI8 getOpCode()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;
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
            void setProfileMac(const string &profileMac);
            string getProfileMac()  const;
			void setProfileMacs(const vector<string> &profileMacs);
            vector<string> getProfileMacs()  const;

// Now the data members

        private:
        protected:
        public:
            UI8  m_opCode;
            UI8  m_cmdCode;
            string  m_profileName;
            UI32  m_profileId;
            bool  m_isActivated;
            string m_profileMac;
			bool  m_isAllow;
			bool  m_isFloodingRestricted;
			vector<string> m_profileMacs;
    };
}
#endif                                            //APPMGLOBALAPPMPROFILEMESSAGE_H
