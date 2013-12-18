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

#ifndef WAVEUSERMANAGEDOBJECT_H
#define WAVEUSERMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

class WaveUserManagedObject : public WaveManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
                                  WaveUserManagedObject       (WaveObjectManager *pWaveObjectManager);
                                  WaveUserManagedObject       (WaveObjectManager *pWaveObjectManager, const string &name);
        virtual                  ~WaveUserManagedObject       ();

        static  string            getClassName                ();

                UI32              getUserId                   () const;
                void              setUserId                   (const UI32 &userId);

                ObjectId          getGroupObjectId            () const;
                void              setGroupObjectId            (const ObjectId &groupObjectId);

                vector<ObjectId>  getAssociatedGroupObjectIds () const;
                void              setAssociatedGroupObjectIds (const vector<ObjectId> &associatedGroupObjectIds);

                string            getEncryptedPassword        () const;
                void              setEncryptedPassword        (const string &encryptedPassword);

                bool              getIsActive                 () const;
                void              setIsActive                 (const bool &isActive);

                string            getHomeDirectory            () const;
                void              setHomeDirectory            (const string &homeDirectory);


    // Now the data members

    private :
        UI32             m_userId;
        ObjectId         m_groupObjectId;
        vector<ObjectId> m_associatedGroupObjectIds;
        string           m_encryptedPassword;
        bool             m_isActive;
        string           m_homeDirectory;

    protected :
    public :
};

}
#endif // WAVEUSERMANAGEDOBJECT_H
