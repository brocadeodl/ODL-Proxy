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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef VALINTERFACELOCALOBJECTMANAGERCONFIGUREINTERFACEMESSAGE_H
#define VALINTERFACELOCALOBJECTMANAGERCONFIGUREINTERFACEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Interface/ValInterfaceTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{

class ValInterfaceLocalObjectManagerConfigureInterfaceMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage ();
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const string &demoString, const IpV4Address &ipAddress, const IpV4Address &ipAddressDef, const SI8 &mtu8, const SI16  &mtu16, const SI32  &mtu32, const SI32  &mtu32Def, const vector<SI32> &mtu32Vector, const UI8  &mtu8u, const UI16  &mtu16u, const UI32  &mtu32u, const UI32  &mtu32udef, const UI64  &mtu64u, const bool &isEthernet, const IpV6Address &ip6Address, const MacAddress &macAddress, const WorldWideName &worldWideName, const bool &shutdown, const Protocol &protocol, const ErrorLevel &errorLevel, const vector<IpV4Address> &ipV4Vector, const vector<IpV6Address> &ipV6Vector, const IpVxAddress &ipVxAddress, const SI32  &interfaceTuple, const MacAddress2 &macAddress2);

                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const string &demoString);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const IpV4Address &ipAddress, const bool isDefault = 0);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const SI8  &mtu8);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const SI16 &mtu16);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const SI32 &mtu32, const UI16 flag = 1, const bool isDefault = 0);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const vector<SI32> &mtu32Vector);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const UI8  &mtu8u);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const UI16 &mtu16u);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const UI32 &mtu32u);
						  ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const UI32 &mtu32u, const bool &isDefault);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const UI64 &mtu64u);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const bool &isEthernet);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const IpV6Address &ip6Address);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const MacAddress &macAddress);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const WorldWideName &worldWideName);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const Protocol &protocol);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const ErrorLevel &errorLevel);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const vector<IpV4Address> &ipV4Vector);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const vector<IpV6Address> &ipV6Vector);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const IpVxAddress &ipVxAddress);
                          ValInterfaceLocalObjectManagerConfigureInterfaceMessage (const ObjectId &interfaceObjectId, const MacAddress2 &macAddress2);



        	virtual   ~ValInterfaceLocalObjectManagerConfigureInterfaceMessage ();

    		          ObjectId              getInterfaceObjectId                     () const;
                      string                getString                                () const;
                      IpV4Address           getIpAddress                             () const;
                      IpV4Address           getIpAddressDef                          () const;
	                  SI8                   getMtu8                                  () const;
                      SI16                  getMtu16                                 () const;
	                  SI32                  getMtu32                                 () const;
	                  SI32                  getMtu32Def                              () const;
                      vector<SI32>          getMtu32Vector                           () const;
	                  UI8                   getMtu8u                                 () const;
          	          UI16                  getMtu16u                                () const;
	                  UI32                  getMtu32u                                () const;
	                  UI32                  getMtu32udef                             () const;
	                  UI64                  getMtu64u                                () const;
                	  bool                  getIsEthernet                            () const;
        			  IpV6Address           getIp6Address                            () const;
	                  MacAddress            getMacAddress                            () const;
		        	  WorldWideName         getWorldWideName                         () const;
        			  bool                  getShutdown                              () const;
                      Protocol              getEnum                                  () const;
                      ErrorLevel            getErrorLevelEnum                        () const;
                      vector<IpV4Address>   getIpV4Vector                            () const;
                      vector<IpV6Address>   getIpV6Vector                            () const;
                      IpVxAddress           getIpVxAddress                           () const;
                      SI32                  getInterfaceTuple                        () const;
	                  MacAddress2           getMacAddress2                           () const;

    // Now the data members

    private :
	        ObjectId                m_interfaceObjectId;
            string                  m_string;
        	IpV4Address             m_ipAddress;
        	IpV4Address             m_ipAddressDef;
	        SI8                     m_mtu8;
       		SI16                    m_mtu16;
	        SI32                    m_mtu32;
	        SI32                    m_mtu32Def;
        	vector<SI32>            m_mtu32Vector;
	        UI8                     m_mtu8u;
        	UI16                    m_mtu16u;
	        UI32                    m_mtu32u;
			UI32                    m_mtu32udef;
        	UI64                    m_mtu64u;
    		bool                    m_isEthernet;
	    	IpV6Address             m_ip6Address;
		    MacAddress              m_macAddress;
    		WorldWideName           m_wwn;
	    	bool                    m_shutdown;
		    Protocol                m_protocol;
            ErrorLevel              m_errorLevel;
            vector<IpV4Address>     m_ipV4Vector;
            vector<IpV6Address>     m_ipV6Vector;
            IpVxAddress             m_ipVxAddress;
            SI32                    m_interfaceTuple;
		    MacAddress2             m_macAddress2;

    protected :
    public :
};

}

#endif // VALINTERFACELOCALOBJECTMANAGERCONFIGUREINTERFACEMESSAGE_H
