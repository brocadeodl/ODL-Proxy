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

#ifndef VALINTERFACELOCALOBJECTMANAGERGETINTERFACEMESSAGE_H
#define VALINTERFACELOCALOBJECTMANAGERGETINTERFACEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Interface/ValInterfaceTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{

class ValInterfaceLocalObjectManagerGetInterfaceMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    ValInterfaceLocalObjectManagerGetInterfaceMessage ();
                    ValInterfaceLocalObjectManagerGetInterfaceMessage (const string &name);

    virtual        ~ValInterfaceLocalObjectManagerGetInterfaceMessage ();

            string  getName                        () const;
            void    setName                        (const string &name);

            string  getString                      () const;
            void    setString                      (const string &demoString);

            IpV4Address  getIpAddress              () const;
            void    setIpAddress                   (const IpV4Address &ipAddress);

            IpV4Address  getIpAddressDef           () const;
            void    setIpAddressDef                (const IpV4Address &ipAddress);

            SI8     getMtu8                        () const;
            void    setMtu8                        (const SI8 &mtu8);
                
            SI16    getMtu16                       () const;
            void    setMtu16                       (const SI16 &mtu16);
                
            SI32    getMtu32                       () const;
            void    setMtu32                       (const SI32 &mtu32);

            SI32    getMtu32Def                    () const;
            void    setMtu32Def                    (const SI32 &mtu32);
				
            vector<SI32>    getMtu32Vector         () const;
            void            setMtu32Vector         (const vector<SI32> &mtu32Vector);
                
            UI8     getMtu8u                       () const;
            void    setMtu8u                       (const UI8 &mtu8u);

            UI16    getMtu16u                      () const;
            void    setMtu16u                      (const UI16 &mtu16u);
                
            UI32    getMtu32u                      () const;
            void    setMtu32u                      (const UI32 &mtu32u);

            UI32    getMtu32udef                   () const;
            void    setMtu32udef                   (const UI32 &mtu32udef);            
    
            UI64    getMtu64u                      () const;
            void    setMtu64u                      (const UI64 &mtu64u);
            
            bool    getIsEthernet                                     () const;
            void    setIsEthernet                                     (const bool &isEthernet);

            IpV6Address  getIp6Address             () const;
            void         setIp6Address             (const IpV6Address &ip6Address);
				
            MacAddress   getMacAddress             () const;
            void         setMacAddress             (const MacAddress &macAddress);
				
            MacAddress2  getMacAddress2            () const;
            void         setMacAddress2            (const MacAddress2 &macAddress);

            WorldWideName  getWorldWideName        () const;
            void           setWorldWideName        (const WorldWideName &worldWideName);
			
            bool    getShutdown                                     () const;
            void    setShutdown                                     (const bool &shutdown);

            Protocol getEnum                        () const;
            void setEnum                            (const Protocol &protocol);

            ErrorLevel  getErrorLevelEnum            () const;
            void        setErrorLevelEnum            (const ErrorLevel &errorLevel);

            vector<IpV4Address> getIpV4Vector       () const;
            void                setIpV4Vector       (const vector<IpV4Address> &ipV4Vector);

            vector<IpV6Address> getIpV6Vector       () const;
            void                setIpV6Vector       (const vector<IpV6Address> &ipV6Vector);

            IpVxAddress  getIpVxAddress             () const;
            void         setIpVxAddress             (const IpVxAddress &ipVxAddress);

            SI32    getInterfaceTuple                     () const;
            void    setInterfaceTuple                     (const SI32 &interfaceTuple);

    // Now the data members

    private :
        string                  m_name;
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
        MacAddress2             m_macAddress2;
        WorldWideName           m_wwn;
        bool                    m_shutdown;
        Protocol                m_protocol;
        ErrorLevel              m_errorLevel;
        vector<IpV4Address>     m_ipV4Vector;
        vector<IpV6Address>     m_ipV6Vector;
        IpVxAddress             m_ipVxAddress;
        SI32                    m_interfaceTuple;

    protected :
    public :
};

}

#endif // VALINTERFACELOCALOBJECTMANAGERGETINTERFACEMESSAGE_H
