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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Interface/InterfaceLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectLoadOperationalDataContext.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeSI64.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "DcmInterface.h"

namespace DcmNs
{

InterfaceLocalManagedObject::InterfaceLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (InterfaceLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
    //setEmptyNeededOnPersistentBoot (false);
}

InterfaceLocalManagedObject::~InterfaceLocalManagedObject ()
{
}

string InterfaceLocalManagedObject::getClassName ()
{
    return ("InterfaceLocalManagedObject");
}

void InterfaceLocalManagedObject::setupAttributesForPersistence ()
{
    UI32 defaultVal = 0;
    SI32 defSIVal = 100;
    IpV4Address defaultIp("0.0.0.0");

    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString        (&m_string, true, "defaultstring", "string",  dcminterface_demostring));    
    addPersistableAttribute (new AttributeIpV4Address   (&m_ipAddress,       "ipAddress",       dcminterface_ip));
    addPersistableAttribute (new AttributeIpV4Address   (&m_ipAddressDef, defaultIp,      "ipAddressdef",       dcminterface_ipdef));
    addPersistableAttribute (new AttributeSI8           (&m_mtu8,            "mtu8",            dcminterface_mtu8));
    addPersistableAttribute (new AttributeSI16          (&m_mtu16,           "mtu16",           dcminterface_mtu16));
    addPersistableAttribute (new AttributeSI32          (&m_mtu32,           "mtu32",           dcminterface_mtu32));
    addPersistableAttribute (new AttributeSI32          (&m_mtu32Def, defSIVal, "mtu32def",           dcminterface_mtu32def));
    addPersistableAttribute (new AttributeSI32Vector    (&m_mtu32Vector,     "mtu32Vector",     dcminterface_mtu32Vector));
    addPersistableAttribute (new AttributeUI8           (&m_mtu8u,           "mtu8u",  	        dcminterface_mtu8u));
    addPersistableAttribute (new AttributeUI16          (&m_mtu16u,          "mtu16u",	        dcminterface_mtu16u));
    addPersistableAttribute (new AttributeUI32          (&m_mtu32u,          "mtu32u",          dcminterface_mtu32u));
    defaultVal = 100;
    addPersistableAttribute (new AttributeUI32          (&m_mtu32udef, defaultVal, "mtu32udef", dcminterface_mtu32udef));
    addPersistableAttribute (new AttributeUI64          (&m_mtu64u,          "mtu64u", 	        dcminterface_mtu64u));
    addPersistableAttribute (new AttributeBool          (&m_isEthernet,      "isEthernet",      dcminterface_isEthernet));
    addPersistableAttribute (new AttributeIpV6Address   (&m_ip6Address,      "ip6Address",      dcminterface_ip6));
    addPersistableAttribute (new AttributeIpVxAddress   (&m_ipVxAddress,     "ipVxAddress",     dcminterface_ipVx));
    addPersistableAttribute (new AttributeMacAddress    (&m_macAddress,      "macAddress",      dcminterface_mac));
    addPersistableAttribute (new AttributeMacAddress2    (&m_macAddress2,      "macAddress2",    dcminterface_mac2));
    addPersistableAttribute (new AttributeWorldWideName (&m_wwn,             "wwn",             dcminterface_wwn));
    addPersistableAttribute (new AttributeBool          (&m_shutdown,        "shutdown",        dcminterface_shutdown));
    addPersistableAttribute (new AttributeSI32          (&m_interfaceTuple,  "interfaceTuple",  dcminterface_interfaceTuple));
    
	addPersistableAttribute (new AttributeSI8           (&m_operationalSI8 , "operationalSI8",  dcminterface_operationalsi8,  true));
    addPersistableAttribute (new AttributeSI16          (&m_operationalSI16, "operationalSI16", dcminterface_operationalsi16, true));
    addPersistableAttribute (new AttributeSI32          (&m_operationalSI32, "operationalSI32", dcminterface_operationalsi32, true));
    addPersistableAttribute (new AttributeSI32Vector    (&m_operationalSI32Vector, "operationalSI32Vector", dcminterface_operationalsi32Vector, true));
    addPersistableAttribute (new AttributeSI64          (&m_operationalSI64, "operationalSI64", dcminterface_operationalsi64, true));

    addPersistableAttribute (new AttributeUI8           (&m_operationalUI8 , "operationalUI8",  dcminterface_operationalui8,  true));
    addPersistableAttribute (new AttributeUI16          (&m_operationalUI16, "operationalUI16", dcminterface_operationalui16, true));
    addPersistableAttribute (new AttributeUI32          (&m_operationalUI32, "operationalUI32", dcminterface_operationalui32, true));
    addPersistableAttribute (new AttributeUI32Vector    (&m_operationalUI32Vector, "operationalUI32Vector", dcminterface_operationalui32Vector, true));
    addPersistableAttribute (new AttributeUI64          (&m_operationalUI64, "operationalUI64", dcminterface_operationalui64, true));
    
	addPersistableAttribute (new AttributeIpV4Address   (&m_operationalIpAddress,       "operationalIpAddress",       dcminterface_operationalIp,  true));
	addPersistableAttribute (new AttributeBool          (&m_operationalIsEthernet,      "operationalIsEthernet",      dcminterface_operationalIsEthernet,  true));
    addPersistableAttribute (new AttributeIpV6Address   (&m_operationalIp6Address,      "operationalIp6Address",      dcminterface_operationalIp6,  true));
    addPersistableAttribute (new AttributeMacAddress    (&m_operationalMacAddress,      "operationalMacAddress",      dcminterface_operationalMac,  true));
    addPersistableAttribute (new AttributeMacAddressVector    (&m_operationalMacAddressVector,      "operationalMacAddressVector",      dcminterface_operationalMacVector,  true));
    addPersistableAttribute (new AttributeWorldWideName (&m_operationalWwn,             "operationalWwn",             dcminterface_operationalWwn,  true));

    addPersistableAttribute (new AttributeEnum		((UI32 * )(&m_protocol), "enumtype", dcminterface_enumtype));
    defaultVal = DCM_ERRORLEVEL_MAX;
    addPersistableAttribute (new AttributeEnum		((UI32 * )(&m_errorLevel), defaultVal, "enumtype2", dcminterface_enumtype2));

    setUserTagForAttribute ("name", dcminterface_name);
    addPersistableAttribute (new AttributeIpV4AddressVector (&m_ipV4Vector,             "ipV4Vector",             dcminterface_ipV4Vector));
    addPersistableAttribute (new AttributeIpV6AddressVector (&m_ipV6Vector,             "ipV6Vector",             dcminterface_ipV6Vector));
}

void InterfaceLocalManagedObject::setupAttributesForCreate ()
{
    UI32 defaultVal = 0;
    SI32 defSIVal = 100;
    IpV4Address defaultIp("0.0.0.0");

    DcmLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString        (&m_string, true, "defaultstring", "string",  dcminterface_demostring));
    addPersistableAttributeForCreate (new AttributeIpV4Address   (&m_ipAddress,       "ipAddress",       dcminterface_ip));
    addPersistableAttributeForCreate (new AttributeIpV4Address   (&m_ipAddressDef, defaultIp,       "ipAddressdef",       dcminterface_ipdef));
    addPersistableAttributeForCreate (new AttributeSI8           (&m_mtu8,            "mtu8",            dcminterface_mtu8));
    addPersistableAttributeForCreate (new AttributeSI16          (&m_mtu16,           "mtu16",           dcminterface_mtu16));
    addPersistableAttributeForCreate (new AttributeSI32          (&m_mtu32,           "mtu32",           dcminterface_mtu32));
    addPersistableAttributeForCreate (new AttributeSI32          (&m_mtu32Def, defSIVal, "mtu32def",        dcminterface_mtu32def));
    addPersistableAttributeForCreate (new AttributeSI32Vector    (&m_mtu32Vector,     "mtu32Vector",     dcminterface_mtu32Vector));
    addPersistableAttributeForCreate (new AttributeUI8           (&m_mtu8u,           "mtu8u",           dcminterface_mtu8u));
    addPersistableAttributeForCreate (new AttributeUI16          (&m_mtu16u,          "mtu16u",          dcminterface_mtu16u));
    addPersistableAttributeForCreate (new AttributeUI32          (&m_mtu32u,          "mtu32u",          dcminterface_mtu32u));
    defaultVal = 100;
    addPersistableAttributeForCreate (new AttributeUI32          (&m_mtu32udef, defaultVal, "mtu32udef", dcminterface_mtu32udef));
    addPersistableAttributeForCreate (new AttributeUI64          (&m_mtu64u,          "mtu64u",          dcminterface_mtu64u));
    addPersistableAttributeForCreate (new AttributeBool          (&m_isEthernet,      "isEthernet",      dcminterface_isEthernet));
    addPersistableAttributeForCreate (new AttributeIpV6Address   (&m_ip6Address,      "ip6Address",      dcminterface_ip6));
    addPersistableAttributeForCreate (new AttributeIpVxAddress   (&m_ipVxAddress,     "ipVxAddress",     dcminterface_ipVx));
    addPersistableAttributeForCreate (new AttributeMacAddress    (&m_macAddress,      "macAddress",      dcminterface_mac));
    addPersistableAttributeForCreate (new AttributeMacAddress2   (&m_macAddress2,      "macAddress2",    dcminterface_mac2));
    addPersistableAttributeForCreate (new AttributeWorldWideName (&m_wwn,             "wwn",             dcminterface_wwn));
    addPersistableAttributeForCreate (new AttributeBool          (&m_shutdown,        "shutdown",        dcminterface_shutdown));
    addPersistableAttributeForCreate (new AttributeIpV4AddressVector (&m_ipV4Vector,  "ipV4Vector",      dcminterface_ipV4Vector));
    addPersistableAttributeForCreate (new AttributeIpV6AddressVector (&m_ipV6Vector,  "ipV6Vector",      dcminterface_ipV6Vector));
    addPersistableAttributeForCreate (new AttributeSI32          (&m_interfaceTuple,  "interfaceTuple",  dcminterface_interfaceTuple));

	addPersistableAttributeForCreate (new AttributeSI8           (&m_operationalSI8 , "operationalSI8",  dcminterface_operationalsi8,  true));
    addPersistableAttributeForCreate (new AttributeSI16          (&m_operationalSI16, "operationalSI16", dcminterface_operationalsi16, true));
    addPersistableAttributeForCreate (new AttributeSI32          (&m_operationalSI32, "operationalSI32", dcminterface_operationalsi32, true));
    addPersistableAttributeForCreate (new AttributeSI32Vector    (&m_operationalSI32Vector, "operationalSI32Vector", dcminterface_operationalsi32Vector, true));
    addPersistableAttributeForCreate (new AttributeSI64          (&m_operationalSI64, "operationalSI64", dcminterface_operationalsi64, true));

    addPersistableAttributeForCreate (new AttributeUI8           (&m_operationalUI8 , "operationalUI8",  dcminterface_operationalui8,  true));
    addPersistableAttributeForCreate (new AttributeUI16          (&m_operationalUI16, "operationalUI16", dcminterface_operationalui16, true));
    addPersistableAttributeForCreate (new AttributeUI32          (&m_operationalUI32, "operationalUI32", dcminterface_operationalui32, true));
    addPersistableAttributeForCreate (new AttributeUI32Vector    (&m_operationalUI32Vector, "operationalUI32Vector", dcminterface_operationalui32Vector, true));
    addPersistableAttributeForCreate (new AttributeUI64          (&m_operationalUI64, "operationalUI64", dcminterface_operationalui64, true));
    
	addPersistableAttributeForCreate (new AttributeIpV4Address   (&m_operationalIpAddress,       "operationalIpAddress",       dcminterface_operationalIp,  true));
	addPersistableAttributeForCreate (new AttributeBool          (&m_operationalIsEthernet,      "operationalIsEthernet",      dcminterface_operationalIsEthernet,  true));
    addPersistableAttributeForCreate (new AttributeIpV6Address   (&m_operationalIp6Address,      "operationalIp6Address",      dcminterface_operationalIp6,  true));
    addPersistableAttributeForCreate (new AttributeMacAddress    (&m_operationalMacAddress,      "operationalMacAddress",      dcminterface_operationalMac,  true));
    addPersistableAttributeForCreate (new AttributeMacAddressVector    (&m_operationalMacAddressVector,      "operationalMacAddressVector",      dcminterface_operationalMacVector,  true));
    addPersistableAttributeForCreate (new AttributeWorldWideName (&m_operationalWwn,             "operationalWwn",             dcminterface_operationalWwn,  true));

    addPersistableAttributeForCreate (new AttributeEnum          ((UI32 *) (&m_protocol), "enumtype", dcminterface_enumtype));
    defaultVal = DCM_ERRORLEVEL_MAX;
    addPersistableAttributeForCreate (new AttributeEnum          ((UI32 *) (&m_errorLevel), defaultVal, "enumtype2", dcminterface_enumtype2));

    vector<string > keyName;
    keyName.push_back ("name");
    setUserDefinedKeyCombination (keyName);
}

void InterfaceLocalManagedObject::loadOperationalData (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext)
{
    // This example code simply returns.
    // In real implementation, one does IPC to backed daemons (OR whatever) to fetch the operational data and set the corresponding fields in the Managed Object.

    vector<string> operationalFields         = pWaveManagedObjectLoadOperationalDataContext->getOperationalDataFields ();
    UI32           numberOfOperationalFields = operationalFields.size ();
    UI32           i                         = 0;
	static UI32    j                         = 0;
	UI32           k                         = 0;
    static SI8     operationalSI8            = 0;
    static SI16    operationalSI16           = 0x10;
    static SI32    operationalSI32           = 0x10000;
    static SI64    operationalSI64           = 0x100000000LL;
    static UI8     operationalUI8            = 0U;
    static UI16    operationalUI16           = 0x10U;
    static UI32    operationalUI32           = 0x10000;
    static UI64    operationalUI64           = 0x100000000LLU;
	static IpV4Address   ip ("192.168.1.1");
	static IpV6Address   ip6 ("fe80::21e:bff:febf:47d4");
	static MacAddress    macAddr ("00:1E:0B:BF:47:D4");
	static WorldWideName wwn ("20:00:00:e0:69:41:54:02");
	vector<SI32>         operationalSI32Vector; 
	vector<UI32>         operationalUI32Vector; 
	vector<MacAddress>   operationalMacAddressVector;



    if (0 == numberOfOperationalFields)
    {
        // Fill All Operational Fields
        setOperationalSI8  (operationalSI8++);
        setOperationalSI16 (operationalSI16++);
        setOperationalSI32 (operationalSI32++);
	    for (k=0;k < 20;k++)
		    operationalSI32Vector.push_back (j++);
		setOperationalSI32Vector (operationalSI32Vector);
	    for (k=0;k < 20;k++)
		    operationalUI32Vector.push_back (j++);
		setOperationalUI32Vector (operationalUI32Vector);
        setOperationalSI64 (operationalSI64++);
        setOperationalUI8  (operationalUI8++);
        setOperationalUI16 (operationalUI16++);
        setOperationalUI32 (operationalUI32++);
        setOperationalUI64 (operationalUI64++);
		setOperationalIpAddress (ip);
		setOperationalIp6Address (ip6);
		setOperationalMacAddress (macAddr);
		
		MacAddress m1 ("00:1E:0B:BF:47:D4");
		MacAddress m2 ("00:1E:0B:BF:48:D6");
		operationalMacAddressVector.push_back (m1);
		operationalMacAddressVector.push_back (m2);
        setOperationalMacAddressVector (operationalMacAddressVector);
          
		setOperationalWorldWideName (wwn);
		setOperationalIsEthernet (true);
    }
    else
    {
        for (i = 0; i < numberOfOperationalFields; i++)
        {
			if (!operationalFields[i].compare ("operationalSI8")) 
			{
                setOperationalSI8 (operationalSI8++);
			}
			else if (!operationalFields[i].compare ("operationalSI16")) 
			{
                setOperationalSI16 (operationalSI16++);
			}
			else if (!operationalFields[i].compare ("operationalSI32")) 
			{
                setOperationalSI32 (operationalSI32++);
			}
			else if (!operationalFields[i].compare ("operationalSI32Vector")) 
			{
	            for (k=0;k < 20;k++)
		            operationalSI32Vector.push_back (j++);
		        setOperationalSI32Vector (operationalSI32Vector);
			}
			else if (!operationalFields[i].compare ("operationalSI64")) 
			{
                setOperationalSI64 (operationalSI64++);
			}
			else if (!operationalFields[i].compare ("operationalUI8")) 
			{
                setOperationalUI8 (operationalUI8++);
			}
			else if (!operationalFields[i].compare ("operationalUI16")) 
			{
                setOperationalUI16 (operationalUI16++);
			}
			else if (!operationalFields[i].compare ("operationalUI32")) 
			{
                setOperationalUI32 (operationalUI32++);
			}
			else if (!operationalFields[i].compare ("operationalUI32Vector")) 
			{
	            for (k=0;k < 20;k++)
		            operationalUI32Vector.push_back (j++);
		        setOperationalUI32Vector (operationalUI32Vector);
			}
			else if (!operationalFields[i].compare ("operationalUI64")) 
			{
                setOperationalUI64 (operationalUI64++);
			}
			else if (!operationalFields[i].compare ("operationalIpAddress")) 
			{
		        setOperationalIpAddress (ip);
			}
			else if (!operationalFields[i].compare ("operationalIp6Address")) 
			{
		        setOperationalIp6Address (ip6);
			}
			else if (!operationalFields[i].compare ("operationalIsEthernet")) 
			{
                setOperationalIsEthernet (true);
			}
			else if (!operationalFields[i].compare ("operationalMacAddress")) 
			{
                setOperationalMacAddress (macAddr);
			}
			else if (!operationalFields[i].compare ("operationalMacAddressVector")) 
			{
		        MacAddress m1 ("00:1E:0B:BF:47:D4");
		        MacAddress m2 ("00:1E:0B:BF:48:D6");
		        operationalMacAddressVector.push_back (m1);
		        operationalMacAddressVector.push_back (m2);
                setOperationalMacAddressVector (operationalMacAddressVector);
			}
			else if (!operationalFields[i].compare ("operationalWwn")) 
			{
                setOperationalWorldWideName (wwn);
			}
        }
    }

    pWaveManagedObjectLoadOperationalDataContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveManagedObjectLoadOperationalDataContext->callback ();
}

string InterfaceLocalManagedObject::getString () const
{
    return (m_string);
}

void InterfaceLocalManagedObject::setString (const string &demoString)
{
    m_string = demoString;
}

IpV4Address InterfaceLocalManagedObject::getIpAddress () const
{
    return (m_ipAddress);
}

void InterfaceLocalManagedObject::setIpAddress (const IpV4Address &ipAddress)
{
    m_ipAddress = ipAddress;
}
IpV4Address InterfaceLocalManagedObject::getIpAddressDef () const
{
    return (m_ipAddressDef);
}

void InterfaceLocalManagedObject::setIpAddressDef (const IpV4Address &ipAddress) 
{
    m_ipAddressDef = ipAddress;
}

SI8 InterfaceLocalManagedObject::getMtu8 () const
{
    return (m_mtu8);
}

void InterfaceLocalManagedObject::setMtu8 (const SI8 &mtu8)
{
    m_mtu8 = mtu8;
}

SI16 InterfaceLocalManagedObject::getMtu16 () const
{
    return (m_mtu16);
}

void InterfaceLocalManagedObject::setMtu16 (const SI16 &mtu16)
{
    m_mtu16 = mtu16;
}

SI32 InterfaceLocalManagedObject::getMtu32 () const
{
    return (m_mtu32);
    //return (m_mtu32);
}

void InterfaceLocalManagedObject::setMtu32 (const SI32 &mtu32)
{
        m_mtu32 = mtu32;
}

SI32 InterfaceLocalManagedObject::getMtu32Def () const
{
    return (m_mtu32Def);
}

void InterfaceLocalManagedObject::setMtu32Def (const SI32 &mtu32) 
{
        m_mtu32Def = mtu32;
}

vector<SI32> InterfaceLocalManagedObject::getMtu32Vector () const
{
    return (m_mtu32Vector);
}

void InterfaceLocalManagedObject::setMtu32Vector (const vector<SI32> &mtu32Vector)
{
    m_mtu32Vector = mtu32Vector;
}

UI8 InterfaceLocalManagedObject::getMtu8u () const
{
    return (m_mtu8u);
}

void InterfaceLocalManagedObject::setMtu8u (const UI8 &mtu8u)
{
    m_mtu8u = mtu8u;
}

UI16 InterfaceLocalManagedObject::getMtu16u () const
{
    return (m_mtu16u);
}

void InterfaceLocalManagedObject::setMtu16u (const UI16 &mtu16u)
{
    m_mtu16u = mtu16u;
}

UI32 InterfaceLocalManagedObject::getMtu32u () const
{
    return (m_mtu32u);
}

void InterfaceLocalManagedObject::setMtu32u (const UI32 &mtu32u)
{
    m_mtu32u = mtu32u;
}

UI64 InterfaceLocalManagedObject::getMtu64u () const
{
    return (m_mtu64u);
}

void InterfaceLocalManagedObject::setMtu64u (const UI64 &mtu64u)
{
    m_mtu64u = mtu64u;
}

bool InterfaceLocalManagedObject::getIsEthernet () const
{
    return (m_isEthernet);
}

void InterfaceLocalManagedObject::setIsEthernet (const bool &isEthernet)
{
    m_isEthernet = isEthernet;
}

IpV6Address InterfaceLocalManagedObject::getIp6Address () const
{
    return (m_ip6Address);
}

void InterfaceLocalManagedObject::setIp6Address (const IpV6Address &ip6Address)
{
    m_ip6Address = ip6Address;
}

IpVxAddress InterfaceLocalManagedObject::getIpVxAddress () const
{
    return (m_ipVxAddress);
}

void InterfaceLocalManagedObject::setIpVxAddress (const IpVxAddress &ipVxAddress)
{
    m_ipVxAddress = ipVxAddress;
}

MacAddress InterfaceLocalManagedObject::getMacAddress () const
{
    return (m_macAddress);
}

void InterfaceLocalManagedObject::setMacAddress (const MacAddress &macAddress)
{
    m_macAddress = macAddress;
}

MacAddress2 InterfaceLocalManagedObject::getMacAddress2 () const
{
    return (m_macAddress2);
}

void InterfaceLocalManagedObject::setMacAddress2 (const MacAddress2 &macAddress)
{
    m_macAddress2 = macAddress;
}
WorldWideName InterfaceLocalManagedObject::getWorldWideName () const
{
    return (m_wwn);
}

void InterfaceLocalManagedObject::setWorldWideName (const WorldWideName &worldWideName)
{
    m_wwn = worldWideName;
}

vector<IpV4Address> InterfaceLocalManagedObject::getIpV4Vector () const
{
    return m_ipV4Vector;
}

void InterfaceLocalManagedObject::setIpV4Vector (const vector<IpV4Address> &ipV4Vector)
{
    m_ipV4Vector = ipV4Vector;
}

vector<IpV6Address> InterfaceLocalManagedObject::getIpV6Vector () const
{
    return m_ipV6Vector;
}

void InterfaceLocalManagedObject::setIpV6Vector (const vector<IpV6Address> &ipV6Vector)
{
    m_ipV6Vector = ipV6Vector;
}

bool InterfaceLocalManagedObject::getShutdown () const
{
    return (m_shutdown);
}

void InterfaceLocalManagedObject::setShutdown (const bool &shutdown)
{
    m_shutdown = shutdown;
}

void InterfaceLocalManagedObject::setInterfaceTuple (const SI32 &interfaceTuple)
{
    m_interfaceTuple = interfaceTuple;
}

SI32 InterfaceLocalManagedObject::getInterfaceTuple () const
{
    return (m_interfaceTuple);
}

SI8 InterfaceLocalManagedObject::getOperationalSI8 () const
{
    return (m_operationalSI8);
}

void InterfaceLocalManagedObject::setOperationalSI8 (const SI8 &operationalSI8)
{
    m_operationalSI8 = operationalSI8;
}

SI16 InterfaceLocalManagedObject::getOperationalSI16 () const
{
    return (m_operationalSI16);
}

void InterfaceLocalManagedObject::setOperationalSI16 (const SI16 &operationalSI16)
{
    m_operationalSI16 = operationalSI16;
}

SI32 InterfaceLocalManagedObject::getOperationalSI32 () const
{
    return (m_operationalSI32);
}

void InterfaceLocalManagedObject::setOperationalSI32 (const SI32 &operationalSI32)
{
    m_operationalSI32 = operationalSI32;
}

vector<SI32> InterfaceLocalManagedObject::getOperationalSI32Vector () const
{
    return (m_operationalSI32Vector);
}

void InterfaceLocalManagedObject::setOperationalSI32Vector (const vector<SI32> &operationalSI32Vector)
{
    m_operationalSI32Vector = operationalSI32Vector;
}

SI64 InterfaceLocalManagedObject::getOperationalSI64 () const
{
    return (m_operationalSI64);
}

void InterfaceLocalManagedObject::setOperationalSI64 (const SI64 &operationalSI64)
{
    m_operationalSI64 = operationalSI64;
}

UI8 InterfaceLocalManagedObject::getOperationalUI8 () const
{
    return (m_operationalUI8);
}

void InterfaceLocalManagedObject::setOperationalUI8 (const UI8 &operationalUI8)
{
    m_operationalUI8 = operationalUI8;
}

UI16 InterfaceLocalManagedObject::getOperationalUI16 () const
{
    return (m_operationalUI16);
}

void InterfaceLocalManagedObject::setOperationalUI16 (const UI16 &operationalUI16)
{
    m_operationalUI16 = operationalUI16;
}

UI32 InterfaceLocalManagedObject::getOperationalUI32 () const
{
    return (m_operationalUI32);
}

void InterfaceLocalManagedObject::setOperationalUI32 (const UI32 &operationalUI32)
{
    m_operationalUI32 = operationalUI32;
}

vector<UI32> InterfaceLocalManagedObject::getOperationalUI32Vector () const
{
    return (m_operationalUI32Vector);
}

void InterfaceLocalManagedObject::setOperationalUI32Vector (const vector<UI32> &operationalUI32Vector)
{
    m_operationalUI32Vector = operationalUI32Vector;
}

UI64 InterfaceLocalManagedObject::getOperationalUI64 () const
{
    return (m_operationalUI64);
}

void InterfaceLocalManagedObject::setOperationalUI64 (const UI64 &operationalUI64)
{
    m_operationalUI64 = operationalUI64;
}

bool InterfaceLocalManagedObject::getOperationalIsEthernet () const
{
    return (m_operationalIsEthernet);
}

void InterfaceLocalManagedObject::setOperationalIsEthernet (const bool &operationalIsEthernet)
{
    m_operationalIsEthernet = operationalIsEthernet;
}

IpV4Address InterfaceLocalManagedObject::getOperationalIpAddress () const
{
    return (m_operationalIpAddress);
}

void InterfaceLocalManagedObject::setOperationalIpAddress (const IpV4Address &operationalIpAddress)
{
    m_operationalIpAddress = operationalIpAddress;
}

IpV6Address InterfaceLocalManagedObject::getOperationalIp6Address () const
{
    return (m_operationalIp6Address);
}

void InterfaceLocalManagedObject::setOperationalIp6Address (const IpV6Address &operationalIp6Address)
{
    m_operationalIp6Address = operationalIp6Address;
}

MacAddress InterfaceLocalManagedObject::getOperationalMacAddress () const
{
    return (m_operationalMacAddress);
}

void InterfaceLocalManagedObject::setOperationalMacAddress (const MacAddress &operationalMacAddress)
{
    m_operationalMacAddress = operationalMacAddress;
}

vector<MacAddress> InterfaceLocalManagedObject::getOperationalMacAddressVector () const
{
    return (m_operationalMacAddressVector);
}

void InterfaceLocalManagedObject::setOperationalMacAddressVector (const vector<MacAddress> &operationalMacAddressVector)
{
    m_operationalMacAddressVector = operationalMacAddressVector;
}

WorldWideName InterfaceLocalManagedObject::getOperationalWorldWideName () const
{
    return (m_operationalWwn);
}

void InterfaceLocalManagedObject::setOperationalWorldWideName (const WorldWideName &operationalWwn)
{
    m_operationalWwn = operationalWwn;
}

Protocol InterfaceLocalManagedObject::getEnum() const
{
    return (m_protocol);
}

void InterfaceLocalManagedObject::setEnum (const Protocol &protocol)
{
    m_protocol = protocol;
}

ErrorLevel InterfaceLocalManagedObject::getErrorLevelEnum() const
{
    return (m_errorLevel);
}

void InterfaceLocalManagedObject::setErrorLevelEnum (const ErrorLevel &errorLevel)
{
    m_errorLevel = errorLevel;
}

UI32 InterfaceLocalManagedObject::getMtu32udef () const
{
    return (m_mtu32udef);
}

void InterfaceLocalManagedObject::setMtu32udef (const UI32 &mtu32udef)
{
    m_mtu32udef = mtu32udef;
}

}

