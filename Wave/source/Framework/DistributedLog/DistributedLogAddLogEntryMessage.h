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
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#ifndef DISTRIBUTEDLOGADDLOGENTRYMESSAGE_H
#define DISTRIBUTEDLOGADDLOGENTRYMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Attributes/AttributeDateTime.h"

namespace WaveNs
{

class DistributedLogAddLogEntryMessage : public ManagementInterfaceMessage
{
    private :
        void                    setupAttributesForSerialization  ();

    protected :
    public :
                                DistributedLogAddLogEntryMessage ();
                                DistributedLogAddLogEntryMessage (const ResourceId &logType, const ResourceId &logDescriptionType, const string &description);
                                DistributedLogAddLogEntryMessage (const ResourceId &logType, const ResourceId &logDescriptionType, const string &description, const string &username);
                                DistributedLogAddLogEntryMessage (const ResourceId &logType, const ResourceId &logDescriptionType, const string &description, const string &username, const PrismServiceId &prismServiceId, const ObjectId &waveNodeObjectId);
                                DistributedLogAddLogEntryMessage (const ResourceId &logType, const ResourceId &logDescriptionType, const string &description, const PrismServiceId &prismServiceId, const ObjectId &waveNodeObjectId);
                                DistributedLogAddLogEntryMessage (const ResourceId &logType, const ResourceId &logDescriptionType, const string &description, const PrismServiceId &prismServiceId, const ObjectId &waveNodeObjectId, const ObjectId &managedObjectId);
        virtual                ~DistributedLogAddLogEntryMessage ();

                ResourceId      getLogType                       () const;
        void                    setLogType                       (const ResourceId &logType);
                ResourceId      getLogDescriptionType            () const;
        void                    setLogDescriptionType            (const ResourceId &logDescriptionType);
                string          getDescription                   () const;
        void                    setDescription                   (const string &description);
                string          getUsername                      () const;
        void                    setUsername                      (const string &username);
                PrismServiceId  getPrismServiceId                () const;
        void                    setPrismServiceId                (PrismServiceId prismServiceId);
                DateTime        getDateTimeStamp                 () const;
        void                    setDateTimeStamp                 (const DateTime &dateTimeStamp);
                ObjectId        getWaveNodeObjectId              () const;
        void                    setWaveNodeObjectId              (const ObjectId &waveNodeObjectId);
                ObjectId        getManagedObjectId               () const;
        void                    setManagedObjectId               (const ObjectId &managedObjectId);

    // Now the data members
    
    private :
                ResourceId      m_logType;
                ResourceId      m_logDescriptionType;
                string          m_description;
                string          m_username;
                PrismServiceId  m_prismServiceId;
                DateTime        m_dateTimeStamp;
                ObjectId        m_waveNodeObjectId;
                ObjectId        m_managedObjectId;

    protected :
    public :
};

}

#endif // DISTRIBUTEDLOGADDLOGENTRYMESSAGE_H
