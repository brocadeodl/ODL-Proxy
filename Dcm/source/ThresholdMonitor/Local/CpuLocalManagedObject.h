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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef CPULOCALMANAGEDOBJECT_H
#define CPULOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
using namespace WaveNs;

namespace DcmNs
{

    class ThresholdMonitorLocalObjectManager;
    class CpuLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            CpuLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager);
            CpuLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager,const UI32 &Poll,const UI32 &Retry,const UI32 &Limit,const actionType &Actions);
            virtual    ~CpuLocalManagedObject ();
            static string getClassName();
            void setPoll(const UI32 &Poll);
            UI32 getPoll()  const;
            void setRetry(const UI32 &Retry);
            UI32 getRetry()  const;
            void setLimit(const UI32 &Limit);
            UI32 getLimit()  const;
            void setActions(const actionType &Actions);
            actionType getActions()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_Poll;
            UI32  m_Retry;
            UI32  m_Limit;
            actionType  m_Actions;
    };
}
#endif                                            //CPULOCALMANAGEDOBJECT_H
