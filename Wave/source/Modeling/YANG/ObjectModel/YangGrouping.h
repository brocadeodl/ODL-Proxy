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

#ifndef YANGGROUPING_H
#define YANGGROUPING_H

#include "Modeling/YANG/ObjectModel/YangElement.h"

namespace WaveNs
{

class YangGrouping : public YangElement
{
    private :
    protected :
    public :
                             YangGrouping                               ();
                             YangGrouping                               (const string &name);
        virtual             ~YangGrouping                               ();

        static  string       getYangName                                ();

        static  YangElement *createInstance                             ();

                void         incrementUsageCount                        (const UI32 &count = 1);
                UI32         getUsageCount                              () const;
                void         setUsageCount                              (const UI32 &usageCount);

                void         incrementUsageCountForProgrammingLanguages (const UI32 &countForProgrammingLanguages = 1);
                UI32         getUsageCountForProgrammingLanguages       () const;
                void         setUsageCountForProgrammingLanguages       (const UI32 &usageCountForProgrammingLanguages);

        virtual bool         considerSubTreeForHFileForCLanguage        () const;

        virtual bool         considerForComputingUsageCountForGroupingsForProgrammingLanguages () const;


    // Now the data members

    private :
        UI32 m_usageCount;
        UI32 m_usageCountForProgrammingLanguages;

    protected :
    public :
};

}

#endif // YANGGROUPING_H
