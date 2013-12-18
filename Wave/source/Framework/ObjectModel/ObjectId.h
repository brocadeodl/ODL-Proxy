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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef OBJECTID_H
#define OBJECTID_H

#include "Framework/Types/Types.h"

namespace WaveNs
{

class ObjectId
{
    private :
    protected :
    public :
                  ObjectId      (UI32 classId, UI64 instanceId);
                  ObjectId      (const ObjectId &rhs);
                  ObjectId      ();
        ObjectId &operator =    (const ObjectId &rhs);
        bool      operator ==   (const ObjectId &rhs) const;
        bool      operator !=   (const ObjectId &rhs) const;
        bool      operator <    (const ObjectId &rhs) const;
        bool      operator >    (const ObjectId &rhs) const;
        UI32      getClassId    () const;
        void      setClassId    (const UI32 &classId);
        UI64      getInstanceId () const;
        void      setInstanceId (const UI64 &instanceId);
        string    toString      () const;

    // Now the data members

    private :
        UI32 m_classId;
        UI64 m_instanceId;

    protected :
    public :
        static ObjectId NullObjectId;
};

}

#endif // OBJECTID_H
