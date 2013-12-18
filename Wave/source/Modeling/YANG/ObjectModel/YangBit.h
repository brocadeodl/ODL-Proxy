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

#ifndef YANGBIT_H
#define YANGBIT_H

#include "Modeling/YANG/ObjectModel/YangElement.h"

namespace WaveNs
{

class YangBit : public YangElement
{
    private :
    protected :
    public :
                             YangBit                      ();
        virtual             ~YangBit                      ();

        static  string       getYangName                  ();

        static  YangElement *createInstance               ();

        virtual void         processChildElement          (YangElement *pYangElement);

        virtual void         getNameForHFileForCLanguage  (string &actualName, string &processedNameForCLanguage, string &cLanguageValue) const;

        virtual bool         considerForHFileForCLanguage () const;

    // Now the data members

    private :
        UI32 m_position;

    protected :
    public :
};

}

#endif // YANGBIT_H
