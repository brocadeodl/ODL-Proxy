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

#ifndef ORMCOMPOSITION_H
#define ORMCOMPOSITION_H

#include "Framework/ObjectRelationalMapping/OrmAssociation.h"

namespace WaveNs
{

class OrmComposition : public OrmAssociation
{
    private :
    protected :
    public :
                                    OrmComposition     (const string &name, const string &relatedTo, const OrmRelationType &relationType, const bool &canBeEmpty = false);
                                   ~OrmComposition     ();
        virtual OrmRelationUmlType  getRelationUmlType () const;

                bool                getDisableValidations       () const;
                void                setDisableValidations       (bool disableValidations);
        virtual string              getSqlForCreate2            (const string &parentName, const string &scehma) const;
                string              getSqlForDisableValidation  (const string &parentName, const string &schema) const;
                string              getSqlForEnableValidation   (const string &parentName, const string &schema) const;

    // Now the data members

    private :
         bool    m_disableValidations;

    protected :
    public :
};

}

#endif // ORMCOMPOSITION_H
