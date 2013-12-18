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

#include "Modeling/YANG/ObjectModel/YangValue.h"

#include <stdlib.h>

namespace WaveNs
{

YangValue::YangValue ()
    : YangElement (getYangName (), ""),
      m_value     (0)
{
}

YangValue::~YangValue()
{
}

string YangValue::getYangName ()
{
    return ("value");
}

YangElement *YangValue::createInstance ()
{
    return (new YangValue ());
}

void YangValue::loadValue (const string &value)
{
    YangElement::setValue (value);
}


void YangValue::processAttribute (const string &attributeName, const string &attributeValue)
{
    if ("value" == attributeName)
    {
                     setName  (attributeName);
        YangElement::setValue (attributeValue);
                     setValue (strtoul (attributeValue.c_str (), NULL, 10));
    }
}

UI32 YangValue::getValue () const
{
    return (m_value);
}

void YangValue::setValue (const UI32 &value)
{
    m_value = value;
}

}
