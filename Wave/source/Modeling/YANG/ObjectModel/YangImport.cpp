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

#include "Modeling/YANG/ObjectModel/YangImport.h"
#include "Modeling/YANG/ObjectModel/YangPrefix.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

YangImport::YangImport ()
    : YangElement (getYangName (), "")
{
}

YangImport::~YangImport()
{
}

string YangImport::getYangName ()
{
    return ("import");
}

YangElement *YangImport::createInstance ()
{
    return (new YangImport ());
}

string YangImport::getModule () const
{
    return (m_module);
}

void YangImport::setModule (const string &module)
{
    m_module = module;
}


string YangImport::getPrefix () const
{
    return (m_prefix);
}

void YangImport::setPrefix (const string &prefix)
{
    m_prefix = prefix;
}

void YangImport::processChildElement (YangElement *pYangElement)
{
    YangElement::processChildElement (pYangElement);

    const string yangName = pYangElement->getYangName ();

    if ((YangPrefix::getYangName()) == yangName)
    {
        YangPrefix *pYangPrefix = dynamic_cast<YangPrefix *> (pYangElement);

        prismAssert (NULL != pYangPrefix, __FILE__, __LINE__);

        string prefixValue;

        pYangPrefix->getAttributeValue ("value", prefixValue);

        prismAssert ("" != prefixValue, __FILE__, __LINE__);

        setPrefix (prefixValue);
    }
}
void YangImport::processAttribute (const string &attributeName, const string &attributeValue)
{
    YangElement::processAttribute (attributeName, attributeValue);

    if ("module" == attributeName)
    {
        setModule (attributeValue);
    }
}

}
