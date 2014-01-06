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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef ORMAGGREGATION_H
#define ORMAGGREGATION_H

#include "Framework/ObjectRelationalMapping/OrmAssociation.h"

namespace WaveNs
{

class OrmAggregation : public OrmAssociation
{
    private :
    protected :
    public :
                                    OrmAggregation     (const string &name, const string &relatedTo, const OrmRelationType &relationType);
                                   ~OrmAggregation     ();
        virtual OrmRelationUmlType  getRelationUmlType () const;

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // ORMAGGREGATION_H