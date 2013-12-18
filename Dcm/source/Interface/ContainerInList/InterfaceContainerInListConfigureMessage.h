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
 *   Author : jvenkata                                                     *
 **************************************************************************/

#ifndef INTERFACECONTAINERINLISTCONFIGUREMESSAGE_H
#define INTERFACECONTAINERINLISTCONFIGUREMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class InterfaceContainerInListConfigureMessage: public PrismMessage
{
	private:
				void 			setupAttributesForSerialization();

	protected:

    public:
		// constructors & destructors
        						InterfaceContainerInListConfigureMessage ();
        						InterfaceContainerInListConfigureMessage (const ObjectId &interfaceObjectId);
                                InterfaceContainerInListConfigureMessage (const ObjectId &interfaceObjectId, const IpV4Address &ip, const WorldWideName &wwn, const SI32 &mtu32, UI32 &configTypeChoice, SI32 &fcFabricId, SI32 &ethIpAddress);
								InterfaceContainerInListConfigureMessage (const ObjectId &interfaceObjectId, const IpV4Address &ip);
								InterfaceContainerInListConfigureMessage (const ObjectId &interfaceObjectId, const WorldWideName &wwn);
								InterfaceContainerInListConfigureMessage (const ObjectId &interfaceObjectId, const SI32 &mtu32);
                                InterfaceContainerInListConfigureMessage (const ObjectId &interfaceObjectId, const UI32 &configTypeChoice, const SI32 &fcFabricId, const SI32 &ethIpAddress);
        virtual    				~InterfaceContainerInListConfigureMessage();

		// other functions
				ObjectId        getInterfaceObjectId () const;
        		void			setIp (const IpV4Address &ip);
        		IpV4Address 	getIp ()const;

        		void 			setWwn (const WorldWideName &wwn);
        		WorldWideName 	getWwn ()const;

        		void 			setMtu32 (const SI32 &mtu32);
        		SI32 			getMtu32 ()const;
                
                UI32           getConfigTypeChoice () const;
                void           setConfigTypeChoice (const UI32 &configTypeChoice);

                SI32           getFcFabricId () const;
                void           setFcFabricId (const SI32 &fcFabricId);
                
                SI32           getEthIpAddress () const;
                void           setEthIpAddress (const SI32 &ethIpAddress);

    // Now the data members
    private:
    protected:
    public:
		ObjectId 		m_interfaceObjectId;
        IpV4Address 	m_ip;
        WorldWideName  	m_wwn;
        SI32  			m_mtu32;
        UI32            m_configTypeChoice;
        SI32            m_fcFabricId;
        SI32            m_ethIpAddress;
};

}
#endif //INTERFACECONTAINERINLISTCONFIGUREMESSAGE_H
