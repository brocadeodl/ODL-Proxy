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
 *   Author : Jitendra Singh                                    *
 ***************************************************************************/

#ifndef VCSFABSWITCHSERIALNUMBERMAPPEDIDMANAGEDOBJECT_H
#define VCSFABSWITCHSERIALNUMBERMAPPEDIDMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#if 0
using namespace WaveNs;

namespace DcmNs
{

typedef enum
{
	SERIAL_NUMBER_NOT_IN_MAPPED_ID_SNDB = 1,
	SERIAL_NUMBER_ALREADY_IN_MAPPED_ID_SNDB,
	MAPPED_ID_NOT_IN_MAPPED_ID_SNDB,
	SWITCH_NOT_IN_MAPPED_ID_SNDB,
	MULTIPLE_SWITCHES_WITH_SAME_MAPPED_ID_FOUND
} SNDBErrors;

class VcsFabObjectManager;

typedef enum
{
	NOT_IN_SNDB = 0,
	IN_SNDB_ACTIVE,
	IN_SNDB_INACTIVE,
	IN_SNDB_DELETE_PENDING
} SNDBObjectStates;


class VcsSwitchSerialNumberMappedIdManagedObject : public DcmManagedObject
{
    private :
        virtual void		setupAttributesForPersistence			();
        virtual void		setupAttributesForCreate				();
//        VcsSwitchSerialNumberMappedIdManagedObject *findFirstInActiveSwitch (vector<WaveManagedObject *> *pResults);

    protected :
    public :
                            VcsSwitchSerialNumberMappedIdManagedObject (WaveObjectManager *pWaveObjectManager);
		virtual				~VcsSwitchSerialNumberMappedIdManagedObject ();
		static				string getClassName						();
		UI32				removeSwitchSerialNumberFromSNDB		(const string &switchSerialNumber);
		UI32				removeSwitchMappedIdFromSNDB			(UI32 mappedId);
		UI32				deActivateSwitchSerialNumberInSNDB		(const string &switchSerialNumber);
		UI32				reActivateSwitchSerialNumberInSNDB		(const string &switchSerialNumber);
		UI32				deActivateSwitchMappedIdFromSNDB		(const UI32 mappedId);

		void				setSwitchSerialNumber					(string switchSerialNumber);
		void				setMappedId								(UI32 mappedId);
		void				setSwitchActiveOrInactive				(bool activeOrInactive);
		void				setSwitchIpAddressInSNDB				(const string &switchIpAddress);
		void				setSwitchPortInSNDB						(const UI32 &switchPortId);
		void				setSwitchMgmtIpAddressInSNDB			(const string &switchMgmtIpAddress);
		void				setSwitchMgmtPortInSNDB				(const UI32 &switchMgmtPortId);
        bool                getSwitchDeleteAfterFailoverFromSNDB ();
        void                setSwitchDeleteAfterFailoverInSNDB          (const bool &switchDeleteAfterFailover);
        UI32                setSwitchDeleteAfterFailoverUseSNInSNDB      (const string &switchSerialNumber);
        bool                getSwitchDeleteAfterFailoverUseSNInSNDB     (const string &switchSerialNumber);

		bool				getSwitchActiveOrInactive				();
		SNDBObjectStates	getSNDBObjectStateForSN					(const string switchSerialNumber, UI32 &mappedId);
		string				getSwitchIpAddressFromSNDB				();
		UI32				getSwitchPortFromSNDB					();
		string				getSwitchMgmtIpAddressFromSNDB			();
		UI32				getSwitchMgmtPortFromSNDB				();
        UI32                getMappedId                             ();
		UI32				getSwitchIpAddressFromMappedId			(const UI32 mappedId, string &switchIpAddress);
		UI32				getSwitchPortIdFromMappedId				(const UI32 mappedId, UI32 &switchPortId);
        UI32				getMappedIdForThisSerialNumber			(const string switchSerialNumber);
		UI32				getSerialNumberForthisMappedId			(const UI32 mappedId, string &switchSerialNumber);

    // Now the data members

    private :
        string	m_switchSerialNumber;
        UI32	m_mappedId;
        bool	m_switchActive;
		string	m_switchIpAddress;
		UI32	m_switchPortId;
		string	m_switchMgmtIpAddress;
		UI32	m_switchMgmtPortId;
        bool    m_switchDeleteAfterFailover;

    protected :
    public :
};

}
#endif
#endif // VCSFABSWITCHSERIALNUMBERMAPPEDIDMANAGEDOBJECT_H
