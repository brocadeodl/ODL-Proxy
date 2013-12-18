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

#ifndef IPADDRESSNETWORKMASK_H
#define IPADDRESSNETWORKMASK_H

#include "Framework/Types/Types.h"
#include <string>

using namespace std;

namespace WaveNs
{

class IpAddressNetworkMask
{
    private :
                  void    parseNetworkMask        (string &ipAddressNetworkMask); 
    protected :  
    public :      
                                         IpAddressNetworkMask    ();
                                         IpAddressNetworkMask    (const string &ipAddressNetworkMask);
                                         IpAddressNetworkMask    (const IpAddressNetworkMask &ipAddressNetworkMask);
        virtual                         ~IpAddressNetworkMask    ();
                  
        virtual   UI32                   getAddressFamily        () = 0;
                  string                 toString                () const;
                  void                   fromString              (const string &ipAddressNetworkMask);
                  string                 getIpAddress            () const;
                  void                   setIpAddress            (const string &ipAddress) ;
                  UI8                    getLength               () const;
                  void                   setLength               (const UI8 &length) ;
                  void                   getPlainString          (string &valueString) const;
        virtual   ResourceId             loadFromPlainString     (const string &ipAddressNetworkMask);
                  
                  bool                   operator ==             (const IpAddressNetworkMask &ipAddressNetworkMask) const;
                  bool                   operator !=             (const IpAddressNetworkMask &ipAddressNetworkMask) const;
                  IpAddressNetworkMask  &operator =              (const IpAddressNetworkMask &ipAddressNetworkMask);
                

    // now the data members

    private :
        string m_ipAddress;
        UI8    m_len;

    protected :
    public :
};

}

#endif // IPADDRESSNETWORKMASK_H
