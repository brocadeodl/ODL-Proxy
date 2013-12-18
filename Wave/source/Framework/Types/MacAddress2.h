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
 *   Author : Pritee Kadu                                                  *
 ***************************************************************************/

#ifndef MACADDRESS2_H
#define MACADDRESS2_H

#include <string>
#include "Framework/Types/Types.h"

namespace WaveNs
{

class MacAddress2
{
    private :
        static bool isAValidHexaDecimalCharacter    (const char &ch);
               UI16 getValueFromChar                (char ch);

    public :
                    MacAddress2              ();
                    MacAddress2              (const UI16 macAddress[3]);
                    MacAddress2              (const string &macAddressInStringFormat);
                    MacAddress2              (const MacAddress2 &macAddress);
                   ~MacAddress2              ();
                                            
        string      toString                 ()                                           const;
        void        fromString               (const string &macAddressInStringFormat);
        ResourceId  loadFromPlainString      (const string &macAddressInStringFormat);
        static bool isValidMacAddress2String (const string &macAddressInStringFormat);
        UI16        &operator []             (const UI32 &i)                              const;
        bool        operator ==              (const MacAddress2 &macAddress)            const;
        bool        operator !=              (const MacAddress2 &macAddress)            const;
        MacAddress2 &operator =              (const MacAddress2 &macAddress);
        void        setSeparator             (const char &separator);
        char        getSeparator             ()                                           const;

    // Now the data members

    private :
                     UI16  *m_pName;
        static const UI32  m_nameLength = 3;
                     char  m_separator;
        static const UI32  m_maxCharInAUnit = 4;

    protected :
    public :
};

}

#endif // MACADDRESS2_H
