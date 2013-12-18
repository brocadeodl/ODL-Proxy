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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Mayur Mahajan                                                *
 ***************************************************************************/

#ifndef SCHEMADIFFERENCE_H
#define SCHEMADIFFERENCE_H

#include "Framework/Types/Types.h"

#include <vector>

namespace WaveNs
{
template <class _Tdiff>
class SchemaDifference
{
    private :
    protected :
    public :
                                SchemaDifference                ();
                                ~SchemaDifference               ();

                bool            hasDifferences                  () const;
          const vector<_Tdiff>  &getAdditions                   () const;
          const vector<_Tdiff>  &getDeletions                   () const;
          const vector<_Tdiff>  &getChanges                     () const;
                void            insertAddition                  (const _Tdiff &difference);
                void            insertDeletion                  (const _Tdiff &difference);
                void            insertChange                    (const _Tdiff &difference);

    // Now the data members

    private :
            vector<_Tdiff>  m_additionsVec;
            vector<_Tdiff>  m_deletionsVec;
            vector<_Tdiff>  m_changesVec;
    protected :
    public :
};

}
#endif //SCHEMADIFFERENCE_H

