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

#ifndef IPV6ADDRESS_H
#define IPV6ADDRESS_H

#include "Framework/Types/IpAddress.h"

namespace WaveNs
{

class IpV6Address : public IpAddress
{
    private :
    protected :
    public :
                               IpV6Address             ();
                               IpV6Address             (const string &ipV6Address);
                               IpV6Address             (const IpV6Address &ipV6Address);
        virtual               ~IpV6Address             ();
                                                       
        virtual UI32           getAddressFamily        ();
        virtual ResourceId     loadFromPlainString     (const string &valueString);

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // IPV6ADDRESS_H
