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
 *  @file   : PrimaryChangedEvent.h                                        *
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Description : Please, see in PrimaryChangedEvent.cpp                  *
 *   Author      : Pritee Ghosh                                            *
 *   Date        : 03/02/2011                                              *
 ***************************************************************************/

#ifndef PRIMARYCHANGEDEVENT_H
#define PRIMARYCHANGEDEVENT_H
#include "Framework/Messaging/Local/PrismEvent.h"

namespace WaveNs
{

class PrimaryChangedEvent : public PrismEvent
{
    private :

    protected :
        virtual void        setupAttributesForSerialization ();

    public :
                            PrimaryChangedEvent ();
        virtual            ~PrimaryChangedEvent ();
                LocationId  getNewPrimaryLocationId () const;
                void        setNewPrimaryLocationId (const LocationId location);
            

    // Now the data members
    
    private :
                LocationId  m_newPrimaryLocationId;
    protected :
    public :
};

}

#endif // PRIMARYCHANGEDEVENT_H
