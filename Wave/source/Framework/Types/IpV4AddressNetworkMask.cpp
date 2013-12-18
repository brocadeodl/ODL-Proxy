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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Framework/Types/IpV4AddressNetworkMask.h"
#include "Framework/Utils/TraceUtils.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <bitset>
namespace WaveNs
{

IpV4AddressNetworkMask::IpV4AddressNetworkMask ()
    : IpAddressNetworkMask ("255.0.0.0/8"),
      m_inverseSubnetMask (false)
{
}

IpV4AddressNetworkMask::IpV4AddressNetworkMask (const string &ipV4AddressNetworkMask)
    : IpAddressNetworkMask (ipV4AddressNetworkMask),
      m_inverseSubnetMask (false)
{
}

IpV4AddressNetworkMask::IpV4AddressNetworkMask (const IpV4AddressNetworkMask &ipV4AddressNetworkMask)
    : IpAddressNetworkMask (ipV4AddressNetworkMask),
      m_inverseSubnetMask (false)
{
}

IpV4AddressNetworkMask::~IpV4AddressNetworkMask ()
{
}

UI32 IpV4AddressNetworkMask::getAddressFamily ()
{
    return (AF_INET);
}
/*
1.  Pattern for Ipaddress and subnet mask, ip address and ip address in CIDR to be defined in yang.

                '(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\.){3}'
                                + '([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])'
+ '((/(([0-9])|([1-2][0-9])|(3[0-2])))|(\s+(((255\.){3}(255|254|252|248|240|224|192|128|0+))|
((255\.){2}(255|254|252|248|240|224|192|128|0+)\.0)|((255\.)(255|254|252|248|240|224|192|128|0+)(\.0+){2})|((255|254|252|248|240|224|192|128|0+)(\.0+){3})))|())';

2.  Pattern for Ipaddress and (inverse) wildcard subnet mask , ip address and ip address in CIDR to be defined in yang. Also setInverseSubnetMask (true). (default value is false)

'(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\.){3}'
              + '([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])'
              + '((/(([0-9])|([1-2][0-9])|(3[0-2])))|
(\s+(((0\.){3}(0|1|3|7|15|31|63|127|255))|((0\.){2}(0|1|3|7|15|31|63|127|255)\.255)|((0\.)(0|1|3|7|15|31|63|127|255)(\.255){2})|((0|1|3|7|15|31|63|127|255)(\.255+){3})))|())';  
                                
3.  Use loadFromPlainString (string) instead and fromString () and check return status.

*/
ResourceId IpV4AddressNetworkMask::loadFromPlainString (const string &ipV4AddressNetworkMask)
{
    string           tempString = ipV4AddressNetworkMask;
    UI32             count      = 0;
    UI32             countSubnet = 0;
    UI8              len = 0;
    vector <string>  tokenNetworkMask;
    vector <string>  tokenNetworkMaskSubnetMask;
    ResourceId       status     = WAVE_MESSAGE_ERROR;
    string mask     = "";
    string ipadd    = "";
    
    tokenize (tempString, tokenNetworkMask, '/');
    tokenize (ipV4AddressNetworkMask, tokenNetworkMaskSubnetMask, ' ');   

    count       = tokenNetworkMask.size ();
    countSubnet = tokenNetworkMaskSubnetMask.size ();

    if (count == 1) 
    {
        unsigned char buf[sizeof(struct in6_addr)];

        if (inet_pton (AF_INET, tokenNetworkMaskSubnetMask[0].c_str(), &buf) == 1)
        {
            if (countSubnet > 1) /*subnet mask is provided */
            {
                mask    = ipAddressToBinary (tokenNetworkMaskSubnetMask[1],getInverseSubnetMask ());
                ipadd   = ipAddressToBinary (tokenNetworkMaskSubnetMask[1],false);

                len = (UI8) mask.find("0");
                
                /* validating mask */
                if ((strncmp (ipadd.substr(len).c_str (), mask.substr(len).c_str (), 32-len) != 0 ))
                {
                    trace (TRACE_LEVEL_ERROR, string("IpV4AddressNetworkMask::loadFromPlainString : Improper network mask."));
                    status = WAVE_MESSAGE_ERROR; 
                }
                else
                {
                    tokenNetworkMask[0] = tokenNetworkMaskSubnetMask[0];
                    status = WAVE_MESSAGE_SUCCESS;
                }    
            }
            else /*only ipaddress is provided */
            {
                len = 32; //if ther is only 1 token, that means complete ipAddress is network ipaddress & there is no subnet.
                status = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else if (count == 2) /* mask length is provided */
    {
        char* firstInvalidCharacterPosition = NULL;
        len = (UI8) strtoul (tokenNetworkMask[1].c_str (), &(firstInvalidCharacterPosition), 10);

        if ( *firstInvalidCharacterPosition == '\0' )
        {  
            unsigned char buf[sizeof(struct in6_addr)];

            if (inet_pton (AF_INET, tokenNetworkMask[0].c_str(), &buf) == 1)
            {
                std::bitset<32> bin;
                mask    = bin.to_string();
                ipadd   = ipAddressToBinary (tokenNetworkMask[0], false); 

                if ((strncmp (ipadd.substr(len).c_str (), mask.substr(len).c_str (), 32-len) != 0 ))
                {
                    trace (TRACE_LEVEL_ERROR, string("IpV4AddressNetworkMask::loadFromPlainString : Improper network mask."));                
                    status = WAVE_MESSAGE_ERROR;
                }
                else
                {
                    status = WAVE_MESSAGE_SUCCESS;
                }
            }
        }    
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string("IpV4AddressNetworkMask::loadFromPlainString : Improper format of IpV4addressNetworkMask in string"));
        status = WAVE_MESSAGE_ERROR;
    }
    

    if( WAVE_MESSAGE_ERROR != status)
    {
        setIpAddress (tokenNetworkMask[0]);
        setLength    (len);
    }

    return status;
}
void IpV4AddressNetworkMask::setInverseSubnetMask (const bool inverseSubnetMask)
{
        m_inverseSubnetMask = inverseSubnetMask;
}

bool IpV4AddressNetworkMask::getInverseSubnetMask ()
{
    return m_inverseSubnetMask;
}
string IpV4AddressNetworkMask::ipAddressToBinary (const string ipAddress, bool inverse )
{
    vector <string>  octet;
    string ipBinary;
    tokenize (ipAddress, octet, '.');
                
    /* converting ipaddress to binary */
    for (UI32 i=0; i < octet.size (); i++)
    {
        int temp = atoi (octet[i].c_str ());
        std::bitset<8> binIp(temp);
                
        if (inverse)
        {
            binIp.flip ();
        }
        ipBinary = ipBinary + binIp.to_string();
    }
    return ipBinary;
}

}

