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

#include "Modeling/YANG/ObjectModel/YangConfig.h"

namespace WaveNs
{

YangConfig::YangConfig ()
    : YangElement (getYangName (), "")
{
}

YangConfig::~YangConfig()
{
}

string YangConfig::getYangName ()
{
    return ("config");
}

YangElement *YangConfig::createInstance ()
{
    return (new YangConfig ());
}

void YangConfig::updateParentAnnotations ()
{   
//    WaveNs::trace (TRACE_LEVEL_INFO, "Yangconfig:: updateParentAnnotationBitMap-------------------------------------- "  );

    string configValue;
    getAttributeValue ("value", configValue);
    prismAssert ("" != configValue, __FILE__, __LINE__);

    if ("false" == configValue)
    {
        (getPParentElement())->setIsConfiguration (false);
    }
    else
    {
        (getPParentElement())->setIsConfiguration (true);
    }

    (getPParentElement())->setIsConfigurationExplicitlySet (true);
}

}
