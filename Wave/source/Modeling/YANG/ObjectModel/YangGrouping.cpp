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

#include "Modeling/YANG/ObjectModel/YangGrouping.h"

namespace WaveNs
{

YangGrouping::YangGrouping ()
    : YangElement                         (getYangName (), ""),
      m_usageCount                        (0),
      m_usageCountForProgrammingLanguages (0)
{
}

YangGrouping::YangGrouping (const string &name)
    : YangElement                         (getYangName (), name),
      m_usageCount                        (0),
      m_usageCountForProgrammingLanguages (0)
{
}

YangGrouping::~YangGrouping()
{
}

string YangGrouping::getYangName ()
{
    return ("grouping");
}

YangElement *YangGrouping::createInstance ()
{
    return (new YangGrouping ());
}

void YangGrouping::incrementUsageCount (const UI32 &count)
{
    m_usageCount += count;
}

UI32 YangGrouping::getUsageCount () const
{
    return (m_usageCount);
}

void YangGrouping ::setUsageCount (const UI32 &usageCount)
{
    m_usageCount = usageCount;
}

void YangGrouping::incrementUsageCountForProgrammingLanguages (const UI32 &countForProgrammingLanguages)
{
    m_usageCountForProgrammingLanguages += countForProgrammingLanguages;
}

UI32 YangGrouping::getUsageCountForProgrammingLanguages () const
{
    return (m_usageCountForProgrammingLanguages);
}

void YangGrouping ::setUsageCountForProgrammingLanguages (const UI32 &usageCountForProgrammingLanguages)
{
    m_usageCountForProgrammingLanguages = usageCountForProgrammingLanguages;
}

bool YangGrouping::considerSubTreeForHFileForCLanguage () const
{
    if (0 < m_usageCountForProgrammingLanguages)
    {
        return (true);
    }
    else
    {
        cout << "SKIPPING ... " << getName () << endl;
        return (false);
    }
}

bool YangGrouping::considerForComputingUsageCountForGroupingsForProgrammingLanguages () const
{
    return (false);
}


}
