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

 /**************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh                                               *
 ***************************************************************************/

#ifndef VCSCLUSTERINFOMESSAGE
#define VCSCLUSTERINFOMESSAGE

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsClusterInfoMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                                                VcsClusterInfoMessage           ();
                                                VcsClusterInfoMessage           (const UI32 domainId);
                                                VcsClusterInfoMessage           (const UI32 startDomainId, const UI32 endDomainId);
                void                            setClusterGenericStatus         (UI32 genericStatus);
                void                            setClusterSpecificStatus        (UI32 specificStatus);
                UI32                            getClusterGenericStatus         ();
                UI32                            getClusterSpecificStatus        ();
                void                            setSwitchesInCluster            (const UI32 &switchesInCluster);
                void                            setStartDomainId                        (const UI32 &startDomainId);
                void                            setEndDomainId                          (const UI32 &endDomainId);

                void                            addSerialNumber                         (const string &serialNumber);
                void                            addDomainId                                     (const UI32 &domainId);
                void                            addVcsId                                        (const UI32 &vcsId);
                void                            addInVcs                                        (const bool &inVcs);
                void                            addVcsMode                                      (const bool &vcsMode);
                void                            addIsPrincipal                          (const bool &isPrincipal);
                void                            addFcfMac                                       (const MacAddress &fcfMac);
                void                            addSwitchMac                            (const MacAddress &switchMac);
                void                            addIpAddress                            (const string &ipAddress);
        void                addPublicIpAddress          (const vector<string> &publicIpAddress);
        void                addClusterFormTime          (const string &clusterFormTime);
                void                            addSpecificStatus                       (const UI32 &specificStatus);
                void                            addGenericStatus                        (const UI32 &genericStauts);
                void                            addLocationId                           (const UI32 &locationId);

                UI32                            getSwitchesInCluster            ();
                vector<string>          getSerialNumber                         ();
                vector<UI32>            getVcsId                                        ();
                vector<UI32>            getDomainId                                     ();
                vector<bool>            getInVcs                                        ();
                vector<bool>            getVcsMode                                      ();
                vector<bool>            getIsPrincipal                          ();
                vector<MacAddress>      getFcfMac                                       ();
                vector<MacAddress>      getSwitchMac                            ();
                UI32                            getStartDomainId                        ();
                UI32                            getEndDomainId                          ();
                vector<string>          getIpAddress                            ();
        vector<string>      getPublicIpAddress          ();
        string              getClusterFormTime          ();
                vector<UI32>            getSpecificStatus                       ();
                vector<UI32>            getGenericStatus                        ();
                vector<UI32>            getLocationId                           ();
                UI32                    isClusterCreated                        ();
                void                    setClusterCreated                       (UI32 clusterCreated);
                UI32                    getNumberOfDisconnctedSwitches          ();
                void                    setNumberOfDisconnectedSwitches         (UI32 numberOfDisconnectedSwitches);                   

    virtual        ~VcsClusterInfoMessage                               ();

        // Now the data members

    private :
                UI32                            m_clusterGenericStatus;
                UI32                            m_clusterSpecificStatus;
                UI32                            m_switchesInCluster;
                UI32                            m_domainId;
                UI32                            m_startDomainId;
                UI32                            m_endDomainId;
                vector<UI32>                    m_switchSpecificStatus;
                vector<UI32>                    m_switchGenericStatus;
        vector<string>      m_serialNumbers;
        vector<UI32>        m_vcsIds;
        vector<UI32>        m_domainIds;
        vector<bool>        m_inVcss;
        vector<bool>        m_vcsModes;
        vector<bool>        m_isPrincipals;
                vector<MacAddress>  m_fcfMacs;
        vector<MacAddress>  m_switchMacs;
                vector<string>          m_ipAddress;
        vector<string>      m_publicIpAddress;
        string              m_clusterFormTime;
                vector<UI32>            m_locationId;
               UI32         m_isClusterCreated;
               UI32         m_numDisconnectedSwitches;
        protected :
    public :
};

}

#endif // VCSCLUSTERINFOMESSAGE


