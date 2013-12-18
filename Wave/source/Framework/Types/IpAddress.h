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

#ifndef IPADDRESS_H
#define IPADDRESS_H

#include "Framework/Types/Types.h"
#include <string>

using namespace std;

namespace WaveNs
{

class IpAddress
{
    private :
    protected :
    public :
                               IpAddress              ();
                               IpAddress              (const string &ipAddress);
                               IpAddress              (const IpAddress &ipAddress);
        virtual               ~IpAddress              ();
                                                      
        virtual UI32           getAddressFamily       () = 0;
                string         toString               () const;
                void           fromString             (const string &ipAddress);
        virtual ResourceId     loadFromPlainString    (const string &ipAddress);
                                                      
                bool           operator ==            (const IpAddress &ipAddress) const;
                bool           operator !=            (const IpAddress &ipAddress) const;
                IpAddress     &operator =             (const IpAddress &ipAddress);
                
    // now the data members

    private :
        string m_ipAddress;

    protected :
    public :
};

}

#endif // IPADDRESS_H
