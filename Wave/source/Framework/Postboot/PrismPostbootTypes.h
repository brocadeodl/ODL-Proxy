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
 *   Author : Leifang Hu                                                 *
 ***************************************************************************/

#ifndef PRISMPOSTBOOTTYPES_H
#define PRISMPOSTBOOTTYPES_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{
typedef	struct  postbootTableEntry {
		string	passName;
		vector  <PrismServiceId> serviceId;
        vector  <int> recoveryType; // 1:1 mapping from serviceId and it's recoveryType. recoveryType is a bitmap to contain COLD, WARM, etc
} postbootPass;

}

#endif // PRISMPOSTBOOTAGENT_H
