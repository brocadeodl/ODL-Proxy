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
 *   Author : Anil Channaveerasetty                                        *
 ***************************************************************************/

#ifndef MACRANGE_H
#define MACRANGE_H

#include "Framework/Types/Types.h"
#include <string>

using namespace std;

namespace WaveNs
{

class MACRange
{
    private:
            vector<string> parsingMacRange          (const string &rangeMacString) const;
            void           copy                     (const MACRange &rangeMac);
            void           copy                     (const string &rangeMacString);
    static  string         addOneElementToString    (bool &insertComma, string elem);
    static  string         addRangeToString         (bool &insertComma, string firstElem, string lastElem);
            bool           isUpperCase              (const char * lastValue) const;
    static  UI32           getRangeValueFromString  (string rangeMacString);
    protected:

    public:
                       MACRange                    ();
                       MACRange                    (const string &rangeMacString);
                       MACRange                    (const MACRange &rangeMac);
                       MACRange                    (const string &startValue, const string &endValue);
                                                  
            string     toString                    () const;
            void       fromString                  (const string &rangeMacString);
            ResourceId loadFromPlainString         (const string &rangeMacString);
            void       getMacRangeVector           (vector <string> &rangeMacVector) const;
            bool       operator ==                 (const MACRange &rangeMac) const;
            bool       operator !=                 (const MACRange &rangeMac) const;
            MACRange   &operator =                 (const MACRange &rangeMac);
    static  bool       isAValidString              (const string &rangeMAcString);
    static  string     getMacRangeStringFromVector (vector <string> &rangeMacVector);

    // now the data members
    private:
           string       m_MacRange;
};

}

#endif   //MACRANGE_H

