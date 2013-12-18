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

#ifndef IPV4ADDRESSNETWORKMASK_H
#define IPV4ADDRESSNETWORKMASK_H

#include "Framework/Types/IpAddressNetworkMask.h"

namespace WaveNs
{

class IpV4AddressNetworkMask : public IpAddressNetworkMask
{
    private :
                string       ipAddressToBinary           (const string ipAddress, bool inverse );
    protected :
    public :
                             IpV4AddressNetworkMask      ();
                             IpV4AddressNetworkMask      (const string &ipV4AddressNetworkMask);
                             IpV4AddressNetworkMask      (const IpV4AddressNetworkMask &ipV4AddressNetworkMask);
        virtual             ~IpV4AddressNetworkMask      ();
                            
        virtual UI32         getAddressFamily            ();
                ResourceId   loadFromPlainString         (const string &ipV4AddressNetworkMask);
                void         setInverseSubnetMask        (bool inverseSubnetMask);
                bool         getInverseSubnetMask        ();
    // Now the data members

    private :
        bool m_inverseSubnetMask;
    protected :
    public :
};

}

#endif // IPV4ADDRESSNETWORKMASK_H

