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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef RMONSTATISTICSLOCALMANAGEDOBJECT_H
#define RMONSTATISTICSLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class RmonStatisticsLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            RmonStatisticsLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            RmonStatisticsLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const SI32 &statsIndex,const string &owner);
            virtual    ~RmonStatisticsLocalManagedObject ();
            static string getClassName();
            void setStatsIndex(const SI32 &statsIndex);
            SI32 getStatsIndex()  const;
            void setOwner(const string &owner);
            string getOwner()  const;

// Now the data members

        private:
        protected:
        public:
            SI32 m_statsIndex;
            string  m_owner;
    };
}
#endif                                            //RMONSTATISTICSLOCALMANAGEDOBJECT_H