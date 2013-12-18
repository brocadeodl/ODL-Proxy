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

#include "Modeling/YANG/ObjectModel/YangEnum.h"
#include "Modeling/YANG/ObjectModel/YangValue.h"
#include "Framework/Utils/Integer.h"

namespace WaveNs
{

YangEnum::YangEnum ()
    : YangElement            (getYangName (), ""),
      m_value                (0),
      m_valueIsExplicitlySet (false)
{
}

YangEnum::~YangEnum()
{
}

string YangEnum::getYangName ()
{
    return ("enum");
}

YangElement *YangEnum::createInstance ()
{
    return (new YangEnum ());
}

UI32 YangEnum::getValue () const
{
    return (m_value);
}

void YangEnum::setValue (const UI32 &value)
{
    m_value = value;
}

bool YangEnum::getValueIsExplicitlySet () const
{
    return (m_valueIsExplicitlySet);
}

void YangEnum::processChildElement (YangElement *pYangElement)
{
    YangElement::processChildElement (pYangElement);

    const string yangName = pYangElement->getYangName ();

    if ((YangValue::getYangName()) == yangName)
    {
        YangValue *pYangValue = dynamic_cast<YangValue *> (pYangElement);

        prismAssert (NULL != pYangValue, __FILE__, __LINE__);

        setValue (pYangValue->getValue ());

        m_valueIsExplicitlySet = true;
    }
}

bool YangEnum::considerForHFileForCLanguage () const
{
    return (true);
}

void YangEnum::getNameForHFileForCLanguage (string &actualName, string &processedNameForCLanguage, string &cLanguageValue) const
{
    YangElement::getNameForHFileForCLanguage (actualName, processedNameForCLanguage, cLanguageValue);

    cLanguageValue = (Integer (getValue ())).toString ();

    return;
}

}
