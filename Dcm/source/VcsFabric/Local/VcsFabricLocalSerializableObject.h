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
 *   Copyright (C) 2008-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            * 
 ***************************************************************************/

#ifndef VCSFABRICLOCALSERIALIZABLEOBJECT
#define VCSFABRICLOCALSERIALIZABLEOBJECT

#include "Framework/ObjectModel/SerializableObject.h"
#include "Framework/Types/WorldWideName.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsFabricLocalSwitchInfoObject : virtual public SerializableObject
{
    private:
    protected:
        virtual void                                setupAttributesForSerialization         ();
    public:
                                                    VcsFabricLocalSwitchInfoObject          ();
                                                    VcsFabricLocalSwitchInfoObject          (const VcsFabricLocalSwitchInfoObject &vcsFabricLocalSwitchInfoObject);
                                                   ~VcsFabricLocalSwitchInfoObject          ();

                LocationId                          getSwitchLocationId                     () const;
                void                                setSwitchLocationId                     (const LocationId &locationId);

                UI32                                getPrincipalSwitchDomainId              () const;
                void                                setPrincipalSwitchDomainId              (const UI32 &domainId);

                bool                                getIsSwitchUnconfirmPrincipal           () const;
                void                                setIsSwitchUnconfirmPrincipal           (const bool &isPrincipal);

                UI32                                getLocationRole                         () const;
                void                                setLocationRole                         (const UI32 &locationRole);

                UI32                                getNodeState                            () const;
                void                                setNodeState                            (const UI32 &nodeState);

                BitMap                              getDcaBitMap                            () const;
                void                                setDcaBitMap                            (const BitMap &dcaBitMap);

                bool                                getIsPrincipal                          () const;
                void                                setIsPrincipal                          (const bool &isPrincipal);

                bool                                getIsSwitchInCluster                    () const;
                void                                setIsSwitchInCluster                    (const bool &inCluster);

       vector<string>                               getSwitchPublicIpAddress                () const;
                void                                setSwitchPublicIpAddress                (const vector<string> &ipaddress);

                UI32                                getSwitchClusterManagementPort          () const;
                void                                setSwitchClusterManagementPort          (const UI32 &port);

                string                              getSwitchClusterManagementIpAddress     () const;
                void                                setSwitchClusterManagementIpAddress     (const string &ipAddress);

                UI32                                getSwitchVcsId                          () const;
                void                                setSwitchVcsId                          (const UI32 &vcsId);

                string                              getSwitchSWVersion                      () const;
                void                                setSwitchSWVersion                      (const string &swVersion);

                string                              getSwitchSerialNumber                   () const;
                void                                setSwitchSerialNumber                   (const string &serialNumber);

                UI32                                getSwitchDomainId                       () const;
                void                                setSwitchDomainId                       (const UI32 &domainId);
				vector<SI32>						getReplacementSwitchSwbd 			() const;
				void 								addReplacementSwitchSwbd 			(const SI32 &replacementSwitchSwbd);
				vector<WorldWideName>				getReplacementSwitchWWN 				() const;
				void 								addReplacementSwitchWWN				(const WorldWideName &replacementSwitchWWN);

    //data members
    private:
            UI32            m_switchDomainId;
            string          m_switchSerialNumber;
            string          m_switchSWVersion;
            UI32            m_switchVcsId;
            string          m_switchClusterMgmtIpAddress;
            UI32            m_switchClusterMgmtPort;
            vector<string>  m_switchPublicIpAddress;
            bool            m_switchInCluster;
            bool            m_switchIsPrincipal;
            BitMap          m_dcaDomainsBitMap;
            UI32            m_nodeState;
            UI32            m_locationRole;
            bool            m_switchIsUnconfirmPrincipal;
            UI32            m_myPrincipalDomainId;
            LocationId      m_switchLocationId;
			vector<SI32> 	m_replacementSwitchSwbd;
			vector<WorldWideName>	m_replacementSwitchWWN;
			
    protected:
    public:
};

}

#endif
