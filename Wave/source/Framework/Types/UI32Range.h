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

#ifndef UI32RANGE_H
#define UI32RANGE_H

#include "Framework/Types/Types.h"
#include <set>
#include <map>
using namespace std;

namespace WaveNs
{

class UI32Range
{
    private:
           vector<UI32>         parsingRangeIntegers        (const string &rangeIntegerString) const;
           map<UI32, bool>      parsingRangeIntegersToMap   (const string &rangeIntegerString) const; 
           void                 copy                        (const UI32Range &rangeIntegers);
           void                 copy                        (const string &rangeIntegerString);
    static string               addOneElementToString       (bool &insertComma, UI32 elem);
    static string               addRangeToString            (bool &insertComma, UI32 firstElem, UI32 lastElem);
    static void                 mergeToPreviousMap          (map <UI32, bool> &rangeMap, const UI32 &limit);
    static string               getUI32RangeStringFromMap   (const map <UI32, bool> &rangeIntegers) ;
           void                 getUI32RangeMap             (map <UI32, bool> &rangeIntegers) const;
    map<UI32, bool>             mergeMap                    (map<UI32, bool> range1, map<UI32, bool> range2);
    map<UI32, bool>             complement                  (map<UI32, bool> range);
    protected:

    public:
                         UI32Range            ();
                         UI32Range            (const string &rangeIntegerString);
                         UI32Range            (const UI32Range &rangeIntegers);
                         UI32Range            (const UI32 &startValue, const UI32 &endValue);
                         UI32Range            (const vector<UI32> &rangeIntegers);

              string     toString             () const;
              void       fromString           (const string &rangeIntegerString);
              ResourceId loadFromPlainString  (const string &rangeIntegerString);
              void       getUI32RangeVector   (vector <UI32> &rangeIntegers) const;
              bool       operator ==          (const UI32Range &rangeIntegers) const;
              bool       operator !=          (const UI32Range &rangeIntegers) const;
              UI32Range &operator =           (const UI32Range &rangeIntegers);
              UI32Range  operator +           (const UI32Range &rangeIntegers);
              UI32Range  operator +           (const UI32 &rangeInteger);
              UI32Range  operator -           (const UI32Range &rangeIntegers);

              UI32Range  operator -           (const UI32 &rangeInteger);
              UI32Range  operator +=          (const UI32Range &rangeIntegers);
              UI32Range  operator -=          (const UI32Range &rangeIntegers);
              UI32Range  operator +=          (const UI32 &rangeInteger);
              UI32Range  operator -=          (const UI32 &rangeInteger);
              UI32Range  intersection         (const UI32Range &secondRangeIntegers);
              bool       find                 (const UI32 &rangeInteger);
              UI32       getCount             () const;
              bool       isEmpty              () const;
              bool       containsAny          (const UI32Range &rangeIntegers);
              bool       containsAll          (const UI32Range &rangeIntegers);
             
      static  bool       isAValidString                 (const string &rangeIntegerString);
      static  string     getUI32RangeStringFromVector   (const vector <UI32> &rangeIntegers);
    // now the data members
    private:
           map<UI32, bool>      m_UI32RangeMap;
};

}

#endif   //UI32RANGE_H
