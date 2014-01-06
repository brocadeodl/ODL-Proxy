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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ServiceInterface/Test/PrismNativeTest1ServiceMessages.h"
#include "Framework/ServiceInterface/Test/PrismNativeTest1Service.h"

namespace WaveNs
{

PrismNativeTest1ServiceMessage1::PrismNativeTest1ServiceMessage1 ()
    : PrismMessage (PrismNativeTest1Service::getPrismServiceId (), WAVE_NATIVE_TEST1_SERVICE_MESSAGE1)
{
}

PrismNativeTest1ServiceMessage1::~PrismNativeTest1ServiceMessage1 ()
{
}

}