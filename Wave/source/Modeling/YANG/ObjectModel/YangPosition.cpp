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

#include "Modeling/YANG/ObjectModel/YangPosition.h"

namespace WaveNs
{

YangPosition::YangPosition ()
    : YangElement (getYangName (), "")
{
}

YangPosition::~YangPosition()
{
}

string YangPosition::getYangName ()
{
    return ("position");
}

YangElement *YangPosition::createInstance ()
{
    return (new YangPosition ());
}

void YangPosition::processAttribute (const string &attributeName, const string &attributeValue)
{
    if ("value" == attributeName)
    {
                     setName    (attributeName);
        YangElement::setValue   (attributeValue);

                     setValue   (strtoul (attributeValue.c_str (), NULL, 10));
    }
}

UI32 YangPosition::getValue () const
{
    return (m_value);
}

void YangPosition::setValue (const UI32 &value)
{
    m_value = value;
}

}
