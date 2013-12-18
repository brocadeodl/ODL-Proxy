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

#ifndef DISTRIBUTEDLOGCONFIGURATIONMANAGEDOBJECT_H
#define DISTRIBUTEDLOGCONFIGURATIONMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Attributes/Attributes.h"

namespace WaveNs
{

class DistributedLogObjectManager;

class DistributedLogConfigurationManagedObject : public WaveManagedObject
{
    private :
        void                setupAttributesForPersistence    ();
        void                setupAttributesForCreate         ();

    protected :
    public :
                            DistributedLogConfigurationManagedObject (DistributedLogObjectManager *pDistributedLogObjectManager);
                            DistributedLogConfigurationManagedObject (DistributedLogObjectManager *pDistributedLogObjectManager, const string &name);
        virtual            ~DistributedLogConfigurationManagedObject ();

        static  string      getClassName                     ();
                UI64        getMaxLogEntries                 () const;
        void                setMaxLogEntries                 (const UI64 &logId);
                UI64        getFirstLogId                    () const;
        void                setFirstLogId                    (const UI64 &logId);
                UI64        getNextLogId                     () const;
        void                setNextLogId                     (const UI64 &logId);

    // Now the data members
    
    private :
        UI64        m_maxLogEntries;
        UI64        m_firstLogId;
        UI64        m_nextLogId;

    protected :
    public :
};

}

#endif // DISTRIBUTEDLOGCONFIGURATIONMANAGEDOBJECT_H
