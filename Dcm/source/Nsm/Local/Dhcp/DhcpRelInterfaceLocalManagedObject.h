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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi                                                       *
 **************************************************************************/

#ifndef DHCPRELINTERFACELOCALMANAGEDOBJECT_H
#define DHCPRELINTERFACELOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "ClientInterface/Dhcp/DhcpMessageDef.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeBoolUC.h"
#include "Framework/Attributes/AttributeStringUC.h"
using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
	class DhcpRelIntfClientConfigMessage;
    class DhcpRelInterfaceLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            DhcpRelInterfaceLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);

            virtual    ~DhcpRelInterfaceLocalManagedObject ();
            static string getClassName();
			void setServerAddress(const IpV4Address &serverAddress);
        	IpV4Address getServerAddress()  const;
            void setIfType(const UI32 &ifType);
            UI32 getIfType()  const;
        	void setIfName(const string &ifName);
        	string getIfName()  const;
            void setVlanId(const UI32 &vlanId);
            UI32 getVlanId()  const;



		// Class data members
        private:
        protected:
        public:
			IpV4Address  m_serverAddress;
			string  	 m_ifName;
			UI32  		 m_ifType; 
			UI32  		 m_vlanId; 
    };
}
#endif                                            //DHCPRELINTERFACELOCALMANAGEDOBJECT_H
