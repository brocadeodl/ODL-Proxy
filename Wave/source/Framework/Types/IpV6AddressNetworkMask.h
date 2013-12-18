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

#ifndef IPV6ADDRESSNETWORKMASK_H
#define IPV6ADDRESSNETWORKMASK_H

#include "Framework/Types/IpAddressNetworkMask.h"

namespace WaveNs
{

class IpV6AddressNetworkMask : public IpAddressNetworkMask
{
    private :
    protected :
    public :
                             IpV6AddressNetworkMask      ();
                             IpV6AddressNetworkMask      (const string &ipV6AddressNetworkMask);
                             IpV6AddressNetworkMask      (const IpV6AddressNetworkMask &ipV6AddressNetworkMask);
        virtual             ~IpV6AddressNetworkMask      ();
                            
        virtual UI32         getAddressFamily            ();
        virtual ResourceId   loadFromPlainString         (const string &ipV6AddressNetworkMask);

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // IPV6ADDRESSNETWORKMASK_H
