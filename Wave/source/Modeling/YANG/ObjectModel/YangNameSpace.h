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

#ifndef YANGNAMESPACE_H
#define YANGNAMESPACE_H

#include "Modeling/YANG/ObjectModel/YangElement.h"

namespace WaveNs
{

class YangNameSpace : public YangElement
{
    private :
    protected :
    public :
                             YangNameSpace  ();
                             YangNameSpace  (const string &uri);
        virtual             ~YangNameSpace  ();

        static  string       getYangName    ();

        static  YangElement *createInstance ();

        virtual void         processAttribute                        (const string &attributeName, const string &attributeValue);

                string  getUri              () const;
                void    setUri              (const string &uri);

    // Now the data members
    private :
        string m_uri;

    protected :
    public :
};

}

#endif // YANGNAMESPACE_H
