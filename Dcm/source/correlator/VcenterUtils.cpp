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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Ankit Jindal                                                 *
 **************************************************************************/

#include "correlator/VcenterUtils.h"
#include <string>
#include <sstream>
#include "Framework/Utils/StringUtils.h"
#include "Framework/Types/UI32Range.h"
#include "Framework/Types/Types.h"
#include "discover/vsphere/Debug.h"
#include <stdio.h>
#include <stdlib.h>

#define ISNOT_UI32RANGE_COMPATIBLE(origStr) (origStr.find(",") != string::npos) && (origStr.find("-") != string::npos)
using namespace std;
using namespace WaveNs;

namespace DcmNs
{

vector<UI32> VcenterUtils::parsingRangeIntegers (const string &rangeIntegerString) 
{
    UI32 i = 0, j = 0;
    string tempStringRange = rangeIntegerString;
    UI32 countFirstLevel = 0;
    UI32 countSecondLevel = 0;
    UI32 totalIntegers = 0;
    vector<UI32> rangeOfIntegers;
    vector<string>  firstLevelToken;
    vector<string>  secondLevelToken;
    set<UI32> uniqueRangeOfIntegers;
    UI32 startValue = 0;
    UI32 endValue   = 0;

    tokenize (tempStringRange, firstLevelToken, ',');

    countFirstLevel = firstLevelToken.size ();

    for (i = 0; i < countFirstLevel; i++)
    {
        tokenize (firstLevelToken[i], secondLevelToken, '-');

        countSecondLevel = secondLevelToken.size ();

        if (2 >= countSecondLevel)
        {
        startValue = strtoul (secondLevelToken[0].c_str (), NULL, 10);
        endValue   = strtoul (secondLevelToken[countSecondLevel - 1].c_str (), NULL, 10);

        for (j = startValue; j <= endValue; j++)
        {
            uniqueRangeOfIntegers.insert(j);
            totalIntegers++;
        }
        }
        else
        {
          uniqueRangeOfIntegers.insert(strtoul (secondLevelToken[0].c_str (), NULL, 10));
          totalIntegers++;
        }
    }
	eraseReservedVlans(uniqueRangeOfIntegers);
	VIC_PLUG_LOG(VIC_TRACE_DEBUG,"parsingRangeIntegers:: uniqueRangeOfIntegers.size = %d", uniqueRangeOfIntegers.size());
    vector<UI32> v( uniqueRangeOfIntegers.begin(), uniqueRangeOfIntegers.end() );
    return (v);
}

void VcenterUtils::eraseReservedVlans (set<UI32> &uniqueRangeOfIntegers)
{
  if(uniqueRangeOfIntegers.size() > 1)
    {
    if (uniqueRangeOfIntegers.find(4091) != uniqueRangeOfIntegers.end())
      uniqueRangeOfIntegers.erase(4091);
    if (uniqueRangeOfIntegers.find(4092) != uniqueRangeOfIntegers.end())
      uniqueRangeOfIntegers.erase(4092);
    if (uniqueRangeOfIntegers.find(4093) != uniqueRangeOfIntegers.end())
      uniqueRangeOfIntegers.erase(4093);
    if (uniqueRangeOfIntegers.find(4094) != uniqueRangeOfIntegers.end())
      uniqueRangeOfIntegers.erase(4094);
   }
}
  
string VcenterUtils::convertToVlanUI32Range(string origStr) {

    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"convertToVlanUI32Range:: orig valn str = %s",origStr.c_str() );
    vector <UI32> rangeIntegers;
    rangeIntegers.clear();
    rangeIntegers = parsingRangeIntegers(origStr);
	return UI32Range::getUI32RangeStringFromVector(rangeIntegers);

	}

}



