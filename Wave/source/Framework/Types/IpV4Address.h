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

#ifndef IPV4ADDRESS_H
#define IPV4ADDRESS_H

#include "Framework/Types/IpAddress.h"

namespace WaveNs
{

class IpV4Address : public IpAddress
{
    private :
    protected :
    public :
                               IpV4Address             ();
                               IpV4Address             (const string &ipV4Address);
                               IpV4Address             (const IpV4Address &ipV4Address);
        virtual               ~IpV4Address             ();
                                                       
        virtual UI32           getAddressFamily        ();
        virtual ResourceId     loadFromPlainString     (const string &valueString);

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // IPV4ADDRESS_H
