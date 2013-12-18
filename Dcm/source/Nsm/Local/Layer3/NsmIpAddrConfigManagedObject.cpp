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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : justinh                                                      *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "vcs.h"

#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

#include "Nsm/Local/Layer3/NsmIpAddrConfigManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrManagedObject.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

namespace DcmNs
{
    class NsmIpAddrManagedObject;

    NsmIpAddrConfigManagedObject::NsmIpAddrConfigManagedObject(
     WaveObjectManager *pNsmLocalObjectManager)
     : PrismElement(pNsmLocalObjectManager),
        PrismPersistableObject(NsmIpAddrConfigManagedObject::getClassName(), 
            DcmLocalManagedObject::getClassName()),
        WaveManagedObject(pNsmLocalObjectManager),
        DcmManagedObject(pNsmLocalObjectManager),
        WaveLocalManagedObjectBase(this),
        DcmLocalManagedObjectBase(this),
        DcmLocalManagedObject(pNsmLocalObjectManager)
    {
    }

    NsmIpAddrConfigManagedObject::~NsmIpAddrConfigManagedObject()
    {
    }

    string  NsmIpAddrConfigManagedObject::getClassName()
    {
        return ("NsmIpAddrConfigManagedObject");
    }

    void  NsmIpAddrConfigManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence();

        addPersistableAttribute(
            new AttributeManagedObjectVectorComposition<NsmIpAddrManagedObject>(
            &m_ipAddr, "ipAddr", getClassName(), getObjectId(), 
            NsmIpAddrManagedObject::getClassName()));
    }

    void  NsmIpAddrConfigManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();

        addPersistableAttributeForCreate(
            new AttributeManagedObjectVectorComposition<NsmIpAddrManagedObject>(
            &m_ipAddr, "ipAddr", getClassName(), getObjectId(), 
            NsmIpAddrManagedObject::getClassName()));
    }

    void  NsmIpAddrConfigManagedObject::setIpAddr(
        const vector<WaveManagedObjectPointer<NsmIpAddrManagedObject> > &ipaddr)
    {
        m_ipAddr = ipaddr;
    }

    vector<WaveManagedObjectPointer<NsmIpAddrManagedObject> > 
        NsmIpAddrConfigManagedObject::getIpAddr() const
    {
        return (m_ipAddr);
    }

    int NsmIpAddrConfigManagedObject::addIpAddrMO(NsmIpAddrManagedObject *pMO)
    {
        // JH TO DO, CHECK MAXIMUM
        WaveManagedObjectPointer<NsmIpAddrManagedObject> ipaddr(pMO);
        m_ipAddr.push_back(ipaddr);

        return 1;
    }

    bool NsmIpAddrConfigManagedObject::searchIpAddr(string ipaddr)
    {
        unsigned int i = 0;

        for(i = 0; i < m_ipAddr.size(); i++)
        {
            if(strcasestr(m_ipAddr[i]->getIpv4AddrMask().c_str(), ipaddr.c_str())) {
                return true; 
            }
        }

        return false;
    }

    int NsmIpAddrConfigManagedObject::findIpAddrIndex(string ipaddr)
    {
        unsigned int i = 0;

        for(i = 0; i < m_ipAddr.size(); i++)
        {
            if(!strcasecmp(ipaddr.c_str(), m_ipAddr[i]->getIpv4AddrMask().c_str())) {
                return i; 
            }
        }

        return -1;
    }

    void NsmIpAddrConfigManagedObject::findOverlapIpAddr
            (vector<string> &overlapIpAddr, string ipaddr)
    {
        unsigned int i = 0, octets[4], prefix, prefixlen;
        unsigned int maskbits[8] = {0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc,
                                    0xfe};

        /* parse ipaddr string */
        sscanf(ipaddr.c_str(), "%u.%u.%u.%u/%u", &octets[0], &octets[1],
               &octets[2], &octets[3], &prefixlen);
        prefix = (octets[0] << 24) + (octets[1] << 16) + (octets[2] << 8) +
                 octets[3];

        /* compare network bits with all existing addresses */
        for(i = 0; i < m_ipAddr.size(); i++)
        {
            unsigned int oldprefix, oldprefixlen, num_octets, num_bits, mask = 0;
            /* parse existing address string */
            sscanf(m_ipAddr[i]->getIpv4AddrMask().c_str(), "%u.%u.%u.%u/%u",
                   &octets[0], &octets[1], &octets[2], &octets[3],
                   &oldprefixlen);
            oldprefix = (octets[0] << 24) + (octets[1] << 16) +
                        (octets[2] << 8) + octets[3];

            /* use smaller prefix len for comparison mask */
            if(oldprefixlen >= prefixlen) {
                oldprefixlen = prefixlen;
            }

            /* construct network mask for comparison */
            num_octets = oldprefixlen / 8;
            num_bits = oldprefixlen % 8;
            octets[0]=octets[1]=octets[2]=octets[3]=0;
            if (num_bits)
                octets[num_octets] = maskbits[num_bits];
            while(num_octets--)
                octets[num_octets] = 0xff; 
            mask = (octets[0] << 24) + (octets[1] << 16) + (octets[2] << 8) +
                   octets[3];

            /* add existing address to overlapIpAddr if network bits match */
            if(!((oldprefix & mask) ^ (prefix & mask))) {
                overlapIpAddr.push_back(m_ipAddr[i]->getIpv4AddrMask());
            }
        }

        return;
    }

    int  NsmIpAddrConfigManagedObject::deleteIpAddr(int index)
    {
        m_ipAddr.erase(m_ipAddr.begin() + index);

        return 1;
    }

    int NsmIpAddrConfigManagedObject::deleteIpAddr(string ipaddr)
    {
        int index = -1;

        if ((index = findIpAddrIndex(ipaddr)) >= 0) {
            deleteIpAddr(index);
        }

       return (index);
    }

    int NsmIpAddrConfigManagedObject::updateIpAddr(string ipaddr)
    {
        int index = -1;

        if ((index = findIpAddrIndex(ipaddr)) >= 0) {
            m_ipAddr[index]->setIpv4AddrMask(ipaddr);
        }

       return (index);
    }

    NsmIpAddrManagedObject* 
        NsmIpAddrConfigManagedObject::getPrimaryIpAddrMO()
    {
        unsigned int i = 0;

        for(i = 0; i < m_ipAddr.size(); i++)
        {
            if(m_ipAddr[i]->isSecond() == false) {
                return (WaveManagedObjectPointer<NsmIpAddrManagedObject>
                    ((m_ipAddr[i]))).operator->();
            }
        }

        return NULL;
    }

    bool NsmIpAddrConfigManagedObject::isPrimaryIpAddr(string ipaddr)
    {
        unsigned int i = 0;
        bool flag = false;

        for(i = 0; i < m_ipAddr.size(); i++)
        {
            if(!strcasecmp(ipaddr.c_str(), 
                m_ipAddr[i]->getIpv4AddrMask().c_str())) 
            {	
                if(m_ipAddr[i]->isSecond() == false) {
                    flag = true; 
                }
                else flag = false;

                break;
            }
        }

        return flag;
    }
	
	UI32 NsmIpAddrConfigManagedObject::matchIPSubnet (string ipAddrStr)
	{
        unsigned int i = 0;
        UI32 ipaddr = ntohl(inet_addr(ipAddrStr.c_str()));
		unsigned int match = NSM_IP_ADDR_NO_MATCH;

        for(i = 0; i < m_ipAddr.size(); i++)
        {
		     NsmIpAddrManagedObject *ipAddr = (WaveManagedObjectPointer<NsmIpAddrManagedObject>
			                     ((m_ipAddr[i]))).operator->();
			 if (ipAddr) {
				 size_t sep = ipAddr->getIpv4AddrMask().find('/');
				 if (sep != string::npos) {
					 string ip_str(ipAddr->getIpv4AddrMask(), 0, sep);
					 string len_str(ipAddr->getIpv4AddrMask(), sep+1);

					 UI32 ip = ntohl(inet_addr(ip_str.c_str()));
					 UI32 prefix_len = atoi(len_str.c_str());

					 if (ip == ipaddr) {
						 return NSM_IP_ADDR_EXACT_MATCH;
					 } else if (ip >> (32 - prefix_len) == ipaddr >> (32 - prefix_len)) {
						 match = NSM_IP_ADDR_PREFIX_MATCH; 
					 }
				 }
			 }
        }

		return match;
	}

} // DcmNs
