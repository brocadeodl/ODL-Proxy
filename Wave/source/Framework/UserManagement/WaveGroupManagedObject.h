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

#ifndef WAVEGROUPMANAGEDOBJECT_H
#define WAVEGROUPMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

class WaveGroupManagedObject : public WaveManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
                                  WaveGroupManagedObject       (WaveObjectManager *pWaveObjectManager);
                                  WaveGroupManagedObject       (WaveObjectManager *pWaveObjectManager, const string &name);
        virtual                  ~WaveGroupManagedObject       ();

        static  string            getClassName                 ();

                UI32              getGroupId                   () const;
                void              setGroupId                   (const UI32 &groupId);

                bool              getIsActive                  () const;
                void              setIsActive                  (const bool &isActive);

                vector<ObjectId>  getAssociatedUserObjectIds   () const;
                void              setAssociatedUserObjectIds   (const vector<ObjectId> &associatedUserObjectIds);
                void              addAssociatedUserObjectId    (const ObjectId &associatedUserObjectId);
                void              deleteAssociatedUserObjectId (const ObjectId &associatedUserObjectId);
                bool              isAnAssociatedUserObjectId   (const ObjectId &associatedUserObjectId) const;

    // Now the data members

    private :
        UI32             m_groupId;
        bool             m_isActive;
        vector<ObjectId> m_associatedUserObjectIds;

    protected :
    public :
};

}

#endif // WAVEGROUPMANAGEDOBJECT_H
