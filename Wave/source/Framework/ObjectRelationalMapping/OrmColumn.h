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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef ORMCOLUMN_H
#define ORMCOLUMN_H

#include "Framework/ObjectRelationalMapping/OrmEntity.h"

#include <string>

using namespace std;

namespace WaveNs
{

class OrmColumn : public OrmEntity
{
    private :
    protected :
    public :
                OrmColumn      (const string &name, const string &sqlType, const string &typeForUpgradeMO, const bool &isPrimary = false);
               ~OrmColumn      ();

        	string  getSqlType     () const;
	virtual string  getTypeForUpgradeMO () const;

        	string getSqlForCreate (const string &schema) const;

        	bool   getIsPrimary    () const;
        	void   setIsPrimary    (const bool &isPrimary);

    // Now the data members

    private :
        string m_sqlType;
        string m_typeForUpgradeMO; // This value indicates the Wave Attribute type of the OrmColumn
        bool   m_isPrimary;

    protected :
    public :
};

}

#endif // ORMCOLUMN_H
