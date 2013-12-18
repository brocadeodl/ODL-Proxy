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

#ifndef DISTRIBUTEDLOGENTRYMANAGEDOBJECT_H
#define DISTRIBUTEDLOGENTRYMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Attributes/AttributeDateTime.h"
#include "Framework/Attributes/Attributes.h"

namespace WaveNs
{

class DistributedLogObjectManager;

class DistributedLogEntryManagedObject : public WaveManagedObject
{
    private :
        void                    setupAttributesForPersistence    ();
        void                    setupAttributesForCreate         ();
        void                    setupKeys                        ();

    protected :
    public :
                                DistributedLogEntryManagedObject (DistributedLogObjectManager *pDistributedLogObjectManager);
                                DistributedLogEntryManagedObject (DistributedLogObjectManager *pDistributedLogObjectManager, const string &name);
        virtual                ~DistributedLogEntryManagedObject ();

        static  string          getClassName                     ();

                UI64            getLogId                         () const;
        void                    setLogId                         (const UI64 &logId);
                ResourceId      getLogType                       () const;
        void                    setLogType                       (const ResourceId &logType);
                ResourceId      getLogDescriptionType            () const;
        void                    setLogDescriptionType            (const ResourceId &logDescriptionType);
                DateTime        getDateTimeStamp                 () const;
        void                    setDateTimeStamp                 (const DateTime &dateTimeStamp);
                PrismServiceId  getPrismServiceId                () const;
        void                    setPrismServiceId                (const PrismServiceId &prismServiceId);
                string          getUsername                      () const;
        void                    setUsername                      (const string &username);
                string          getDescription                   () const;
        void                    setDescription                   (const string &description);
                ObjectId        getWaveNodeObjectId              () const;
        void                    setWaveNodeObjectId              (const ObjectId &waveNodeObjectId);
                ObjectId        getManagedObjectId               () const;
        void                    setManagedObjectId               (const ObjectId &managedObjectId);

    // Now the data members
    
    private :
                UI64            m_logId;
                ResourceId      m_logType;
                ResourceId      m_logDescriptionType;
                DateTime        m_dateTimeStamp;
                PrismServiceId  m_prismServiceId;
                string          m_username;
                string          m_description;
                ObjectId        m_waveNodeObjectId;
                ObjectId        m_managedObjectId;

    protected :
    public :
};

}

#endif // DISTRIBUTEDLOGENTRYMANAGEDOBJECT_H
