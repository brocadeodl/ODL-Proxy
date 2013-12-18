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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Modeling/YANG/ObjectModel/YangStatus.h"

namespace WaveNs
{

YangStatus::YangStatus ()
    : YangElement (getYangName (), "")
{
}

YangStatus::~YangStatus()
{
}

string YangStatus::getYangName ()
{
    return ("status");
}

YangElement *YangStatus::createInstance ()
{
    return (new YangStatus ());
}

void YangStatus::updateParentAnnotations ()
{   
    //WaveNs::trace (TRACE_LEVEL_INFO, "YangStatus:: updateParentAnnotationBitMap-------------------------------------- "  );
    string statusValue;

    getAttributeValue ("value", statusValue);
    prismAssert ("" != statusValue, __FILE__, __LINE__);

    if ("deprecated" == statusValue)
    {
        (getPParentElement())->setIsDeprecated (true);
    }

}

}
