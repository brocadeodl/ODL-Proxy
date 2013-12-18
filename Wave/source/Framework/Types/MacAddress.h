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

#ifndef MACADDRESS_H
#define MACADDRESS_H

#include <string>
#include "Framework/Types/Types.h"

namespace WaveNs
{

class MacAddress
{
    private :
        static bool isAValidHexaDecimalCharacter (const char &ch);

    protected :
    public :
                    MacAddress              ();
                    MacAddress              (const UI8 macAddress[6]);
                    MacAddress              (const string &macAddressInStringFormat);
                    MacAddress              (const MacAddress &macAddress);
                   ~MacAddress              ();
                                           
        string      toString                ()                                           const;
        string      toString2               ()                                           const;
        void        fromString              (const string &macAddressInStringFormat);
        ResourceId  loadFromPlainString     (const string &macAddressInStringFormat);
        static bool isValidMacAddressString (const string &macAddressInStringFormat);

        UI8        &operator []   (const UI32 &i)                              const;
        bool        operator ==   (const MacAddress &macAddress)            const;
        bool        operator !=   (const MacAddress &macAddress)            const;
        MacAddress &operator =    (const MacAddress &macAddress);
        void        setSeparator  (const char &separator);
        char        getSeparator  ()                                           const;

    // Now the data members

    private :
                     UI8  *m_pName;
        static const UI32  m_nameLength = 6;
                     char  m_separator;

    protected :
    public :
};

}

#endif // MACADDRESS_H
