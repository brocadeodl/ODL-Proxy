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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef VALCLIENTPHYSICALINTERFACE_H
#define VALCLIENTPHYSICALINTERFACE_H

#include "Framework/Types/Types.h"

using namespace WaveNs;

namespace DcmNs
{

class ValClientPhysicalInterface
{
    private :
    protected :
    public :
            ValClientPhysicalInterface    ();
           ~ValClientPhysicalInterface    ();

    string  getName                       () const;
    void    setName                       (const string &name);

    string  getIpAddress                  () const;
    void    setIpAddress                  (const string &ipAddress);

    UI32    getMtu                        () const;
    void    setMtu                        (const UI32 &mtu);

    // Now the Data Members

    private :
        string m_name;
        string m_ipAddress;
        UI32   m_mtu;

    protected :
    public :
};

}

#endif // VALCLIENTPHYSICALINTERFACE_H
