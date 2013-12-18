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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef CLUSTERLOCALGETOBJECTIDMESSAGES_H
#define CLUSTERLOCALGETOBJECTIDMESSAGES_H

#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Messaging/Local/PrismMessage.h"

namespace WaveNs
{

class PrismMessage;

class ClusterLocalGetSlotObjectIdMessage : public PrismMessage
{
    private :
    protected :
    public :
                            ClusterLocalGetSlotObjectIdMessage  (const UI32 &slotNumber, const LocationId &locationId);
        virtual            ~ClusterLocalGetSlotObjectIdMessage  ();

                UI32        getSlotNumber                       () const;
                void        setSlotNumber                       (const UI32 &slotNumber);
                
                LocationId  getLocationId                       () const;
                void        setLocationId                       (const LocationId &locationId);

                ObjectId    getSlotObjectId                     () const;
                void        setSlotObjectId                     (const ObjectId slotObjectId);

    // Now the data members

    private :
        UI32        m_slotNumber;
        LocationId  m_locationId;
        ObjectId    m_slotObjectId;

    protected :
    public :
};

class ClusterLocalGetLocationObjectIdMessage : public PrismMessage
{
    private :
    protected :
    public :
                            ClusterLocalGetLocationObjectIdMessage  (const LocationId &locationId);
        virtual            ~ClusterLocalGetLocationObjectIdMessage  ();

                LocationId  getLocationId                           () const;
                void        setLocationId                           (const LocationId &locationId);

                ObjectId    getLocationObjectId                     () const;
                void        setLocationObjectId                     (const ObjectId locationObjectId);

    // Now the data members

    private :
        LocationId  m_locationId;
        ObjectId    m_locationObjectId;

    protected :
    public :
};

class ClusterLocalGetSlotNumberMessage : public PrismMessage
{
    private :
    protected :
    public :
                            ClusterLocalGetSlotNumberMessage    (const ObjectId slotObjectId);
        virtual            ~ClusterLocalGetSlotNumberMessage    ();

                UI32        getSlotNumber                       () const;
                void        setSlotNumber                       (const UI32 &slotNumber);
                
                ObjectId    getSlotObjectId                     () const;
                void        setSlotObjectId                     (const ObjectId slotObjectId);

    // Now the data members

    private :
        UI32        m_slotNumber;
        ObjectId    m_slotObjectId;

    protected :
    public :
};

class ClusterLocalGetLocationIdMessage : public PrismMessage
{
    private :
    protected :
    public :
                            ClusterLocalGetLocationIdMessage    (const ObjectId locationObjectId);
        virtual            ~ClusterLocalGetLocationIdMessage    ();

                LocationId  getLocationId                       () const;
                void        setLocationId                       (const LocationId &locationId);
                
                ObjectId    getLocationObjectId                 () const;
                void        setLocationObjectId                 (const ObjectId slotObjectId);

    // Now the data members

    private :
        LocationId  m_locationId;
        ObjectId    m_locationObjectId;

    protected :
    public :
};

}

#endif // CLUSTERLOCALSETHAROLEMESSAGE_H
