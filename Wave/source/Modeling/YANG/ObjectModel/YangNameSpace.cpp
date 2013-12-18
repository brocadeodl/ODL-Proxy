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

#include "Modeling/YANG/ObjectModel/YangNameSpace.h"

namespace WaveNs
{

YangNameSpace::YangNameSpace ()
    : YangElement (getYangName (), "")
{
}

YangNameSpace::YangNameSpace (const string &uri)
    : YangElement (getYangName (), ""),
      m_uri       (uri)
{
}

YangNameSpace::~YangNameSpace()
{
}

string YangNameSpace::getYangName ()
{
    return ("namespace");
}

YangElement *YangNameSpace::createInstance ()
{
    return (new YangNameSpace ());
}

void YangNameSpace::processAttribute (const string &attributeName, const string &attributeValue)
{
    if ("uri" == attributeName)
    {
        setUri (attributeValue);
    }
}

string YangNameSpace::getUri () const
{
    return (m_uri);
}

void YangNameSpace::setUri (const string &uri)
{
    m_uri = uri;

    setName (m_uri);
}

}
