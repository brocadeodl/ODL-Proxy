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

#ifndef YANGAUGMENT_H
#define YANGAUGMENT_H

#include "Modeling/YANG/ObjectModel/YangElement.h"

#include <string>

using namespace std;

namespace WaveNs
{

class YangAugment : public YangElement
{
    private :
    protected :
    public :
                             YangAugment          ();
                             YangAugment          (const string &targetNode);
        virtual             ~YangAugment          ();

        static  string       getYangName          ();

        static  YangElement *createInstance       ();

                string       getTargetNode        () const;
                void         setTargetNode        (const string &targetNode);

        virtual void         processAttribute     (const string &attributeName, const string &attributeValue);

                void         getTargetNodeDetails (string &targetNodeModuleName, string &targetNodePathInModule);

                void         transferWhenConditionsToChildren ();

    // Now the data members

    private :
        string m_targetNode;

    protected :
    public :
};

}

#endif // YANGAUGMENT_H
