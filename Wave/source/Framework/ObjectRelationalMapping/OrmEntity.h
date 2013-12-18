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

#ifndef ORMENTITY_H
#define ORMENTITY_H

#include <string>

using namespace std;

namespace WaveNs
{

class OrmEntity
{
    private :
    protected :
    public :
                        OrmEntity       (const string &name);
        virtual        ~OrmEntity       ();
		string  getName         () const;
        virtual string  getSqlForCreate (const string &schema) const = 0;
	virtual string  getTypeForUpgradeMO () const = 0;

    // Now the data members

    private :
	        string          m_name;
    protected :
    public :
};

}

#endif // ORMENTITY_H
