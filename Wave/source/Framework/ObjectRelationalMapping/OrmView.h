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
 *   Copyright (C) 2012-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : MITHUN BS                                                    *
 ***************************************************************************/

#ifndef ORMVIEW_H
#define ORMVIEW_H

#include "Framework/Attributes/Attribute.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"

#include <vector>
#include <string>
#include <set>

using namespace std;

namespace WaveNs
{

class OrmView : virtual public OrmTable
{
    private :

    protected :
    public :
                                    OrmView                               (const string &viewName);
                                   ~OrmView                               ();

        string                      getName                                () const;
        string                      getSqlForCreateView                    () const;
        void                        setSqlForCreateView                    (string &viewDefinition);

    // Now the data members


    private :
        string                      m_name;
        string                      m_viewDefinition;

    protected :
    public :
};

}

#endif // ORMVIEW_H
