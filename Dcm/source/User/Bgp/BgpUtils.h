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

#ifndef BGP_UTILS_H
#define BGP_UTILS_H

#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{
  class BgpUtils: public WaveObjectManagerToolKit
  {
    private:
    public:
      static string getASinIntegerFormat(const string &inputAsString);
	  static bool isValidAsNum(const string &inputAsString);
  };
}

#endif  //BGP_UTILS_H

