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
 *   Author : Anand Kumar Subramanian                                            *
 ***************************************************************************/

#ifndef IPVXADDRESS_H
#define IPVXADDRESS_H

#include "Framework/Types/IpAddress.h"

namespace WaveNs
{

typedef enum
{
    WAVE_INVALID = 0,
    WAVE_IPV4,
    WAVE_IPV6
}IpType;

class IpVxAddress : public IpAddress
{
    private :

        void                   setIpType             (const IpType ipType);
        IpType                 determineIpType       (const string &ipVxAddress);
                              
    protected :               
    public :                  
                               IpVxAddress           ();
                               IpVxAddress           (const string &ipVxAddress);
                               IpVxAddress           (const IpVxAddress &ipVxAddress);
        virtual               ~IpVxAddress           ();
        virtual UI32           getAddressFamily      ();
        virtual void           fromString            (const string &ipVxAddress);
        virtual ResourceId     loadFromPlainString   (const string &ipVxAddress);
        bool                   operator ==           (const IpVxAddress &ipVxAddress) const;
        bool                   operator !=           (const IpVxAddress &ipVxAddress) const;
        IpVxAddress            &operator =           (const IpVxAddress &ipVxAddress);
        static bool            isValidIpV4Address    (const string &ipVxAddress);
        static bool            isValidIpV6Address    (const string &ipVxAddress);
        IpType                 getIpType             () const;

    // Now the data members

    private :
        IpType      m_addressType;

    protected :
    public :
};

}

#endif // IPVXADDRESS_H

