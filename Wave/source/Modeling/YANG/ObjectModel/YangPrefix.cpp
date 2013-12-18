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

#include "Modeling/YANG/ObjectModel/YangPrefix.h"

namespace WaveNs
{

YangPrefix::YangPrefix ()
    : YangElement (getYangName (), "")
{
}

YangPrefix::YangPrefix (const string &value)
    : YangElement (getYangName (), ""),
      m_value     (value)
{
}

YangPrefix::~YangPrefix()
{
}

string YangPrefix::getYangName ()
{
    return ("prefix");
}

YangElement *YangPrefix::createInstance ()
{
    return (new YangPrefix ());
}

string YangPrefix::getValue () const
{
    return (m_value);
}

void YangPrefix::setValue (const string &value)
{
    m_value = value;
}

}
