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
 *   Author : Balachandar Mani											   *
 ***************************************************************************/

#ifndef WAVE_CONFIGURATION_SEGMENT_CONTEXT_H
#define WAVE_CONFIGURATION_SEGMENT_CONTEXT_H

#include <string>
#include "Framework/ObjectModel/ObjectId.h"

namespace WaveNs
{

class WaveConfigurationSegmentContext
{
    private :
    protected :
    public :
                WaveConfigurationSegmentContext ();
               ~WaveConfigurationSegmentContext ();

                string          getManagedObjectClassName                       () const;
                void            setManagedObjectClassName                       (const string &managedObjectClassName);

                ObjectId        getOperateOnWaveManagedObjectId                 () const;
                void            setOperateOnWaveManagedObjectId                 (const ObjectId &waveManagedObjectId);

                vector<UI32>    getAttributeUserTags                            () const;
                void            setAttributeUserTags                            (const vector<UI32> &attributeUserTags);

                vector<string>  getAttributeNames                               () const;
                void            setAttributeNames                               (const vector<string> &attributeNames);

                vector<string>  getAttributeValues                              () const;
                void            setAttributeValues                              (const vector<string> &attributeValues);

                ObjectId        getParentObjectId                               () const;
                void            setParentObjectId                               (const ObjectId &parentObjectId);

                UI32            getChildUserTag                                 () const;
                void            setChildUserTag                                 (const UI32 &childUserTag);

                LocationId      getLocationId                                   () const;
                void            setLocationId                                   (const LocationId &locationId);

                ResourceId    getConfigHandlerStatus () const;
                void          setConfigHandlerStatus (const ResourceId configHandlerStatus);

                string        getErrorString () const;

    // Member variables - start
    private :
                LocationId              m_locationId;
    protected :
    public :
        string                  m_managedObjectClassName;
        ObjectId                m_operateOnWaveManagedObjectId;
        vector<UI32>            m_attributeUserTags;
        vector<string>          m_attributeNames;
        vector<string>          m_attributeValues;
        ObjectId                m_parentObjectId;
        UI32                    m_childUserTag;
        ResourceId              m_configHandlerStatus;
};
}

#endif
