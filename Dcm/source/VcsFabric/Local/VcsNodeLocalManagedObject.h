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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef VCSNODELOCALMANAGEDOBJECT_H
#define VCSNODELOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "VcsFabric/Local/VcsSlotLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

namespace DcmNs
{

class VcsNodeLocalManagedObject : public DcmLocalManagedObject
{
    private :
        void setupAttributesForPersistence ();
        void setupAttributesForCreate      ();

    protected :
    public :
        VcsNodeLocalManagedObject ();
        VcsNodeLocalManagedObject (WaveObjectManager *pWaveObjectManager);
        virtual        ~VcsNodeLocalManagedObject ();

        static  string  getClassName        ();
                void    setSerialNumber     (const string &serial_number);
                void    setDomainId         (const UI32 &domain_id);
                void    setVcsId            (const UI32 &vcs_id);
                void    setFcfMac           (const MacAddress &fcf_mac);
                void    setSwitchMac        (const MacAddress &switch_mac);
                void    setVcsMode          (const bool &mode);
                void    setInVcs            (const bool &inVcs);
                void    setIsPrincipal      (const bool &isPrincipal);
				void	setWillingMode		(const bool &willingMode);
                void    setPublicIpAddress  (const vector<string> &publicIpAddress);
                void    setLocationId       (const LocationId &locationId);
                void    addVcsSlotLocalManagedObject (VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject);
                void    removeVcsSlotLocalManagedObject (VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject );
                void    removeAllVcsSlotLocalManagedObjects ();
				void	setSwitchState		(const UI32 &switchState);
                void	setSwitchDeleteAfterFailoverFlag (const bool &switchDeleteAfterFailoverFlag);
                void    setSwitchPort (SI32 switchPort);
                void    setSwitchWWN        (const WorldWideName &switchWWN);
                void    setSWBDModel        (SI32   SWBDModel);
                void    setFwVersion        (const string &fwVersion);
                void    setSwitchname       (const string &switchname);
                void    setSwmodelName      (const string &swmodelName);
                void    setPrincipalPriority(const UI32 principalPriority);
                UI32        getVcsId            () const;
                UI32        getMappedId         () const;
                MacAddress  getSwitchMac        () const;
                MacAddress  getFcfMac           () const;
                UI32        getDomainId         () const;
                string      getSerialNumber     () const;
                bool        getVcsMode          () const;
				bool		getIsPrincipal		() const;
				bool		getWillingMode		() const;
				bool		getInVcs			() const;
                vector<string> getPublicIpAddress () const;
                LocationId  getLocationId       () const;
				UI32 		getSwitchState		() const;
                bool	       getSwitchDeleteAfterFailover () const;
                SI32           getSwitchPort                () const;			
                WorldWideName  getSwitchWWN                 () const;
                SI32           getSWBDModel                 () const;
                string         getFwVersion                 () const;
                string         getSwitchname                () const;
                string         getSwmodelName               () const;
                UI32           getPrincipalPriority         () const;


    // Now the data members

    private :
        string      m_serialNumber;
        UI32        m_vcsId;
        UI32        m_domainId;
        MacAddress  m_fcfMac;
        MacAddress  m_switchMac;
        bool        m_vcsMode;
        bool        m_inVcs;
        bool        m_isPrincipal;
        UI64        m_xactionId;
	    bool	    m_willingMode;
        vector<string>      m_publicIpAddress;
        LocationId  m_locationId;
    	vector<WaveManagedObjectPointer<VcsSlotLocalManagedObject> > m_vcsSlotLocalManagedObjects;
        UI32        m_switchState;
        bool        m_switchDeleteAfterFailover;
        SI32        m_switchPort;
        WorldWideName m_switchWWN;
        SI32        m_SWBDModel;
        string      m_fwVersion;
        string      m_switchname;
        string      m_swmodelName;
        UI32        m_principalPriority;
    protected :
    public :
};

}

#endif // VCSNODELOCALMANAGEDOBJECT_H
