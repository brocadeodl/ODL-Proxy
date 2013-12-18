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

#ifndef WORLDWIDENAME_H
#define WORLDWIDENAME_H

#include <string>
#include "Framework/Types/Types.h"

namespace WaveNs
{

class WorldWideName
{
    private :
        static bool isAValidHexaDecimalCharacter (const char &ch);

    protected :
    public :
                       WorldWideName              ();
                       WorldWideName              (const UI8 worldWideName[8]);
                       WorldWideName              (const string &worldWideNameInStringFormat);
                       WorldWideName              (const WorldWideName &worldWideName);
                      ~WorldWideName              ();
                                                  
        string         toString                   ()                                           const;
        void           fromString                 (const string &worldWideNameInStringFormat);
        static bool    isValidWorldWideNameString (const string &worldWideNameInStringFormat);
        UI8           &operator []                (const UI32 &i)                              const;
        bool           operator ==                (const WorldWideName &worldWideName)         const;
        bool           operator !=                (const WorldWideName &worldWideName)         const;
        WorldWideName &operator =                 (const WorldWideName &worldWideName);
        void           setSeparator               (const char &separator);
        char           getSeparator               ()                                           const;
        ResourceId     loadFromPlainString        (const string &worldWideNameInPlainStringFormat);
    // Now the data members

    private :
                     UI8  *m_pName;
        static const UI32  m_nameLength = 8;
                     char  m_separator;

    protected :
    public :
};

}

#endif // WORLDWIDENAME_H
