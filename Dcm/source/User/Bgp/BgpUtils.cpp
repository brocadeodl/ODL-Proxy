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
 *   Copyright (C) 2013-2018 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "User/Bgp/BgpUtils.h"
#include <string>
#include <sstream>

namespace DcmNs
{
	string BgpUtils::getASinIntegerFormat(const string &inputAsString)
	{
		string tempString = inputAsString;
		UI32 firstPartOfAS = 0, secondPartOfAS = 0, finalASNum = 0;
		stringstream intAS;
		size_t position = inputAsString.find(".");
		if (position != string::npos) {
			tempString = inputAsString.substr(0, position);
			firstPartOfAS = strtoul(tempString.c_str(), NULL, 10);
			tempString = inputAsString.substr(position+1,string::npos);
			secondPartOfAS = strtoul(tempString.c_str(), NULL, 10);
			finalASNum = firstPartOfAS * 65536 + secondPartOfAS;
			intAS << finalASNum;
			tempString = intAS.str();
		}
		return (tempString);
	}

	//call isValidAsNum() with input equal to the output of API getASinIntegerFormat()
    bool BgpUtils::isValidAsNum(const string &inputAsString)
    {
        UI32 asNum = 0;
        asNum = strtoul(inputAsString.c_str(), NULL, 10);
        if (!asNum)
          return false;
        else
          return true;
    }
}
