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
 *   Author : Kunjan Kumar Verma                                           *
 **************************************************************************/

#ifndef APPMVLANPROFILEMESSAGE_H
#define APPMVLANPROFILEMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Attributes/AttributeUI32Range.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMVlanProfileMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            APPMVlanProfileMessage ();
            APPMVlanProfileMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const bool &swModeFlag,const UI8 &swMode,const bool &isAllowedVlanAll,const bool &isAllowedVlanNone,const UI32 &accessVlan,const UI32Range &vlanRange,const UI32 &vlanClassifierGroupId,const UI32 &vlanClassifierVlan);
            virtual    ~APPMVlanProfileMessage ();
            void setOpCode(const UI8 &opCode);
            UI8 getOpCode()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;
            void setProfileName(const string &profileName);
            string getProfileName()  const;
            void setProfileId(const UI32 &profileId);
            UI32 getProfileId()  const;
            void setSwModeFlag(const bool &swModeFlag);
            bool getSwModeFlag()  const;
            void setSwMode(const UI8 &swMode);
            UI8 getSwMode()  const;
            void setIsAllowedVlanAll(const bool &isAllowedVlanAll);
            bool getIsAllowedVlanAll()  const;
            void setIsAllowedVlanNone(const bool &isAllowedVlanNone);
            bool getIsAllowedVlanNone()  const;
            void setAccessVlan(const UI32 &accessVlan);
            UI32 getAccessVlan()  const;
            void setVlanRange(const UI32Range &vlanRange);
            UI32Range getVlanRange()  const;
            void setVlanClassifierGroupId(const UI32 &vlanClassifierGroupId);
            UI32 getVlanClassifierGroupId()  const;
            void setVlanClassifierVlan(const UI32 &vlanClassifierVlan);
			UI32 getVlanClassifierVlan()  const;
			void setNativeVlanId(const UI32 &nativeVlan);
			UI32 getNativeVlanId()const;
            void setTrunkVlanId(const UI32 &trunkVlanId);
            UI32 getTrunkVlanId() const;
            void setVlanClassificationType(const UI32 &vlanClasType);
            UI32 getVlanClassificationType() const;
            void setCtag(const UI32 &ctag);
            UI32 getCtag() const;
            void setMac(const string &mac);
            string getMac() const;
            void setMacGroup(const UI32 &macGroup);
            UI32 getMacGroup() const;

            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
            UI8         m_opCode;
            UI8         m_cmdCode;
            string      m_profileName;
            UI32        m_profileId;
            bool        m_swModeFlag;
            UI8         m_swMode;
            bool        m_isAllowedVlanAll;
            bool        m_isAllowedVlanNone;
            UI32        m_accessVlan;
			UI32Range   m_vlanRange;
            UI32        m_vlanClassifierGroupId;
            UI32        m_vlanClassifierVlan;
            UI32        m_nativeVlan;
            UI32        m_trunkVlanId;
            UI32        m_vlanClassificationType;
            UI32        m_ctag;
            string      m_mac;
            UI32        m_macGroup;
    };
}
#endif                                            //APPMVLANPROFILEMESSAGE_H
