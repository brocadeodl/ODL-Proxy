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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef INTERFACELOCALMANAGEDOBJECT_H
#define INTERFACELOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Interface/ValInterfaceTypes.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{

class InterfaceLocalManagedObject : public DcmLocalManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();
        virtual void loadOperationalData           (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext);

    protected :
    public :
                        InterfaceLocalManagedObject      (WaveObjectManager *pWaveObjectManager);
        virtual        ~InterfaceLocalManagedObject      ();

        static  string  getClassName                     ();

                string         getString                 () const;
                void           setString                 (const string &demoString);

                IpV4Address    getIpAddress              () const;
                void           setIpAddress              (const IpV4Address &ipAddress);

                IpV4Address    getIpAddressDef           () const;
                void           setIpAddressDef           (const IpV4Address &ipAddress);

                SI8            getMtu8                   () const;
                void           setMtu8                   (const SI8 &mtu8);

                SI16           getMtu16                  () const;
                void           setMtu16                  (const SI16 &mtu16);

                SI32           getMtu32                  () const;
                void           setMtu32                  (const SI32 &mtu32);

                SI32           getMtu32Def               () const;
                void           setMtu32Def               (const SI32 &mtu32);

                vector<SI32>   getMtu32Vector            () const;
                void           setMtu32Vector            (const vector<SI32> &mtu32Vector);

                UI8            getMtu8u                  () const;
                void           setMtu8u                  (const UI8 &mtu8u);

                UI16           getMtu16u                 () const;
                void           setMtu16u                 (const UI16 &mtu16u);

                UI32           getMtu32u                 () const;
                void           setMtu32u                 (const UI32 &mtu32u);

                UI32           getMtu32udef              () const;
                void           setMtu32udef              (const UI32 &mtu32udef);

                UI64           getMtu64u                 () const;
                void           setMtu64u                 (const UI64 &mtu64u);

                bool           getIsEthernet             () const;
                void           setIsEthernet             (const bool &isEthernet);

                IpV6Address    getIp6Address             () const;
                void           setIp6Address             (const IpV6Address &ip6Address);

                IpVxAddress    getIpVxAddress             () const;
                void           setIpVxAddress             (const IpVxAddress &ipVxAddress);

                MacAddress     getMacAddress             () const;
                void           setMacAddress             (const MacAddress &macAddress);

                MacAddress2    getMacAddress2             () const;
                void           setMacAddress2             (const MacAddress2 &macAddress);

                WorldWideName  getWorldWideName          () const;
                void           setWorldWideName          (const WorldWideName &worldWideName);

                vector<IpV4Address> getIpV4Vector            () const;
                void                setIpV4Vector            (const vector<IpV4Address> &ipV4Vector);

                vector<IpV6Address> getIpV6Vector            () const;
                void                setIpV6Vector            (const vector<IpV6Address> &ipV6Vector);

                bool           getShutdown               () const;
                void           setShutdown               (const bool &shutdown);

                Protocol       getEnum	                 () const;
                void           setEnum                   (const Protocol &protocol);
				
                ErrorLevel     getErrorLevelEnum	     () const;
                void           setErrorLevelEnum         (const ErrorLevel &errorLevel);

                SI32           getInterfaceTuple         () const;
                void           setInterfaceTuple         (const SI32 &interfaceTuple);
				
                SI8            getOperationalSI8         () const;
                void           setOperationalSI8         (const SI8 &operationalSI8);

                SI16           getOperationalSI16        () const;
                void           setOperationalSI16        (const SI16 &operationalSI16);
                
                SI32           getOperationalSI32        () const;
                void           setOperationalSI32        (const SI32 &operationalSI32);
				
                vector<SI32>   getOperationalSI32Vector  () const;
                void           setOperationalSI32Vector  (const vector<SI32> &operationalSI32Vector);
                
                SI64           getOperationalSI64        () const;
                void           setOperationalSI64        (const SI64 &operationalSI64);
                
                UI8            getOperationalUI8         () const;
                void           setOperationalUI8         (const UI8 &operationalUI8);

                UI16           getOperationalUI16        () const;
                void           setOperationalUI16        (const UI16 &operationalUI16);
                
                UI32           getOperationalUI32        () const;
                void           setOperationalUI32        (const UI32 &operationalUI32);
                
                vector<UI32>   getOperationalUI32Vector  () const;
                void           setOperationalUI32Vector  (const vector<UI32> &operationalUI32Vector);

                UI64           getOperationalUI64        () const;
                void           setOperationalUI64        (const UI64 &operationalUI64);

                IpV4Address    getOperationalIpAddress   () const;
                void           setOperationalIpAddress   (const IpV4Address &operationalIp4Address);
				
		        bool           getOperationalIsEthernet  () const;
                void           setOperationalIsEthernet  (const bool &operationalIsEthernet);

		        IpV6Address    getOperationalIp6Address  () const;
                void           setOperationalIp6Address  (const IpV6Address &operationalIp6Address);

                MacAddress     getOperationalMacAddress  () const;
                void           setOperationalMacAddress  (const MacAddress &operationalMacAddress);
				
                vector<MacAddress>  getOperationalMacAddressVector   () const;
                void           setOperationalMacAddressVector        (const vector<MacAddress> &operationalMacAddressVector);

                WorldWideName  getOperationalWorldWideName           () const;
                void           setOperationalWorldWideName           (const WorldWideName &operationalWwn);

    // Now the Data Members

    private :
        string              m_string;
        IpV4Address         m_ipAddress;
        IpV4Address         m_ipAddressDef;
        SI8                 m_mtu8;
        SI16                m_mtu16;
        SI32                m_mtu32;
        SI32                m_mtu32Def;
        vector<SI32>        m_mtu32Vector;
        UI8                 m_mtu8u;
        UI16                m_mtu16u;
        UI32                m_mtu32u;
		UI32                m_mtu32udef;
        UI64                m_mtu64u;
        bool                m_isEthernet;
        IpV6Address         m_ip6Address;
        IpVxAddress         m_ipVxAddress;
        MacAddress          m_macAddress;
        WorldWideName       m_wwn;
        bool                m_shutdown;
	    Protocol            m_protocol;
        vector<IpV4Address> m_ipV4Vector;
        vector<IpV6Address> m_ipV6Vector;
        MacAddress2         m_macAddress2;
	    ErrorLevel          m_errorLevel; // this enum is set to default mode in MO
        SI32                m_interfaceTuple;

        // Operational Data Fields
        SI8           m_operationalSI8;
        SI16          m_operationalSI16;
        SI32          m_operationalSI32;
        vector<SI32>  m_operationalSI32Vector;
        SI64          m_operationalSI64;
        UI8           m_operationalUI8;
        UI16          m_operationalUI16;
        UI32          m_operationalUI32;
        vector<UI32>  m_operationalUI32Vector;
        UI64          m_operationalUI64;
        IpV4Address   m_operationalIpAddress;
        bool          m_operationalIsEthernet;
        IpV6Address   m_operationalIp6Address;
        MacAddress    m_operationalMacAddress;
        vector<MacAddress>    m_operationalMacAddressVector;
        WorldWideName m_operationalWwn;

    protected :
    public :
};

}

#endif // INTERFACELOCALMANAGEDOBJECT_H
